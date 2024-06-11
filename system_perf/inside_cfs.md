## CFS

### 1. 核心类
```c
/* file: include/linux/sched.h */

/**
 * @brief 调度实体
 */
struct sched_entity {
    /* 记录当前se的权重信息，由进程的nice值计算得到 */
    struct load_weight load;
    /* 指向该se在红黑树中的节点 */
    struct rb_node run_node;
    /* 是否在 runqueue 上，1 则表示在 rq 中 */
    unsigned int on_rq;

    /* 当该se被调度时，记录其开始执行的时间，以便后期用来计算该se此次调度的时长 */
    u64 exec_start;
    /* 记录总运行时间 */
    u64 sum_exec_runtime;
    /* 该进程的虚拟运行时间，该值是红黑树中的 key, CFS 依据该值来保证公平调度 */
    u64 vruntime;
    /* 截止该调度周期开始时，进程的总运行时间，在check_preempt_tick中会使用到 */
    u64 prev_sum_exec_runtime;
};

struct load_weight {
    /* nice 对应的权重数组sched_prio_to_weight中的值 */
    unsigned long weight;
    /* 记录 sched_prio_to_wmult 数组中的值 */
    u32 inv_weight;
};
```

```c
/* file: kernel/sched/sched.h */
/**
 * @brief CFS 运行队列
 */
struct cfs_rq {
    /* 保存 CFS 整个运行队列的总权重，用于给每个调度实体计算 vruntime */
    struct load_weight load;
    /* 队列里面处于 runnable 状态的 se 的总数 */
    unsigned int nr_running;
    /* 记录该 cfs_rq 的总运行时间，用于统计 */
    u64 exec_clock;
    /* 记录队列中当前最小的 vruntime */ 为了初始化进程赋值
    u64 min_vruntime;

    /* 保存一个指向红黑树根节点与最左子节点的缓存 */
    struct rb_root_cached tasks_timeline;

    /* 指向当前正在运行的 se */
    struct sched_entity *curr;
    struct sched_entity *next;
    struct sched_entity *last;
    struct sched_entity *skip;
};

/* file: tools/include/linux/rbtree.h */
/* 指向红黑树根节点与最左子节点的缓存结构 */
struct rb_root_cached {
    struct rb_root rb_root;
    struct rb_node *rb_leftmost;
};

```

### 调度周期存在的原因
由于 CFS 总是选择虚拟运行时间最短的任务运行（CFS并不是预先给任务分配时间片，而是根据大家当前的运行时间来判断谁应该是下一个该执行的任务，这样所有任务都随着时间的推进齐头并进），因此理论上说调度周期的存在没有必要。  
引入的调度周期的原因: `控制调度延迟`。

#### 什么是调度延迟？
CFS不仅需要保证时间分配的公平，还要保证各个任务每隔一段时间就能够执行一次，`一个任务在两次被调度到的时间间隔`就是调度延迟.  
相反，调度器还需要保证任务在每次得到机会执行时，除了任务主动放弃CPU,尽量不要太快地被踢出来，因为太频繁的上下文切换会导致系统的总体性能降低。所以 CFS 没有使用固定的时间长度作为调度周期，而是根据当前队列中的任务数量动态计算出调度周期的长度.
```c
/* file: kernel/sched/fair.c */

/**
 * @brief 计算调度周期
 * @pram nr_running 当前 cfs_rq 中的任务总数
 * @note sysctl_sched_latency 和 sysctl_sched_min_granularity 可以使用 proc 文件系统修改
 */
static u64 __sched_period(unsigned long nr_running) {
if (unlikely(nr_running > sched_nr_latency))   // sched_nr_latency = 8
    return nr_running * sysctl_sched_min_granularity;
else
    return sysctl_sched_latency;
}
```
算出调度周期之后，系统还需要为任务计算其在`一个调度周期内的时间配额`，函数 sched_slice 用来实现该逻辑：
```c
/* file: kernel/sched/fair.c */
static u64 sched_slice(struct cfs_rq *cfs_rq, struct sched_entity *se) {
    unsigned int nr_running = cfs_rq->nr_running;
    u64 slice;

    // 计算出调度周期
    slice = __sched_period(nr_running + !se->on_rq);

    // 暂时不考虑组调度，此处的循环只会执行一次
    for_each_sched_entity(se) {
        struct load_weight *load;
        struct load_weight lw;

        cfs_rq = cfs_rq_of(se);
        /* 整个运行队列 cfs_rq 的总权重 */
        load = &cfs_rq->load;

        /* se->load.weight为se的权重，调用函数__calc_delta得到slice*se->load.weight/load.weight,
        * 即根据 se 在整个队列中的权重比例分配时间 */
        slice = __calc_delta(slice, se->load.weight, load);
    }

    if (sched_feat(BASE_SLICE))
        slice = max(slice, (u64)sysctl_sched_min_granularity);

    return slice;
}
```

#### 调度节拍
周期性调度也叫调度节拍，它的入口是函数 schedule_tick(), 该函数最终会调用调度类的 task_tick() 方法完成操作：
```c
/* file: kernel/sched/core.c */
/**
 * @brief 调度节拍，由时钟中断触发
 */
void scheduler_tick(void) {
    int cpu = smp_processor_id();
    struct rq *rq = cpu_rq(cpu);
    struct task_struct *curr = rq->curr;

    // 调用当前任务的调度类的 task_tick 方法
    curr->sched_class->task_tick(rq, curr, 0);

#ifdef CONFIG_SMP
    /* SMP 架构下触发负载均衡 */
    rq->idle_balance = idle_cpu(cpu);
    trigger_load_balance(rq);
#endif
}

```

```c
/* file: kernel/sched/fair.c */
static void task_tick_fair(struct rq *rq, struct task_struct *curr,
                    int queued) {
    struct cfs_rq *cfs_rq;
    struct sched_entity *se = &curr->se;

    //在不考虑组调度的情况下，此处的循环只会迭代一次，处理的就是当前任务
    for_each_sched_entity(se) {
        cfs_rq = cfs_rq_of(se);
        entity_tick(cfs_rq, se, queued);
    }
}

/**
 * @brief 实际调度
 */
/* file: kernel/sched/fair.c */
static void entity_tick(struct cfs_rq *cfs_rq, struct sched_entity *curr,
                int queued)
{
    // 首先更新当前任务及队列的各种时间信息
    update_curr(cfs_rq);

    if (cfs_rq->nr_running > 1) {//检查是否需要抢占当前任务 
        check_preempt_tick(cfs_rq, curr);
    }
}
```

```c
/* file: kernel/sched/fair.c */
/**
 * @brief 检查任务时间是否耗尽
 * @param cfs_rq cfs 运行队列
 * @param curr 当前调度实体
 */
static void check_preempt_tick(struct cfs_rq *cfs_rq,
                        struct sched_entity *curr) {
    unsigned long ideal_runtime, delta_exec;
    struct sched_entity *se;
      s64 delta;

    //计算出当前任务在一个调度周期内的时间配额
    ideal_runtime = sched_slice(cfs_rq, curr);
    //计算出当前任务已经运行了多长时间
    delta_exec = curr->sum_exec_runtime - curr->prev_sum_exec_runtime;
    if (delta_exec > ideal_runtime) { //如果运行时长已经超过了任务自己的时间配额，则对任务进行抢占
        resched_curr(rq_of(cfs_rq));
        return;
    }

    // 避免任务抢占发生得太过频繁
    if (delta_exec < sysctl_sched_min_granularity)
        return;

    // 从cfs_fq中挑出vruntime最小的任务，即红黑树中最左子节点；并计算出当前任务与该任务的vruntime的差值
    se = __pick_first_entity(cfs_rq);
    delta = curr->vruntime - se->vruntime;

    // 如果当前任务的vruntime依然小于红黑树中所有任务的vruntime, 则不发生抢占
    if (delta < 0)
        return;

    //如果已经多除了相当部分，则可以抢占当前任务了
    if (delta > ideal_runtime)
        resched_curr(rq_of(cfs_rq));
}
```

```c
/* file: kernel/sched/core.c */
/**
 * @brief 重新调度当前任务
 * @param rq 运行队列
 * @note 并不会立即发生重新调度，而是设置一个标识位而已
 */
void resched_curr(struct rq *rq) {
    struct task_struct *curr = rq->curr;
    int cpu;

    lockdep_assert_held(&rq->lock);

    //如果当前任务已经设置了 TIF_NEED_RESCHED 标记位，则返回
    if (test_tsk_need_resched(curr))
        return;

    cpu = cpu_of(rq);

    if (cpu == smp_processor_id()) {
        set_tsk_need_resched(curr);  //设置标识位
        set_preempt_need_resched();
        return;
    }
}
```

#### 真实的调度时机
从调度器的角度来看，真正的调度（即调度器完成上下文切换，换一个任务来执行）仅发生在函数 schedule() 中.
```c
/* file: kernel/sched/core.c */
/**
 * @brief 实际调度函数
 */
asmlinkage __visible void __sched schedule(void) {
    struct task_struct *tsk = current;

    //need_resched用来判断当前任务是否应该被抢占，此时的当前任务就是函数__schedule最新选择的任务
    //如果是的话那么继续调用函数__schedule以便调用下一个合适的任务
    do {
        // 禁止抢占确保在调度过程中不会被抢占
        preempt_disable();
        //调用函数 __schedule 来做具体的工作,false 表示当前不是任务抢占导致的
        __schedule(false);
        // 启用抢占，但是不会触发重新调度，使用该函数来恢复抢占，避免在下一个循环中立即重新调度
        sched_preempt_enable_no_resched();
    } while (need_resched());
}

/**
 * @brief 调度实现
 * @param preempt 表示当前调度是否由任务抢占导致的, true 表示是， false 表示不是
 */
static void __sched notrace __schedule(bool preempt) {
    struct task_struct *prev, *next;
    unsigned long *switch_count;
    unsigned long prev_state;
    struct rq_flags rf;
    struct rq *rq;
    int cpu;

    //获取到当前CPU序号，进而获取到其runqueue
    cpu = smp_processor_id();
    rq = cpu_rq(cpu);
    //rq->curr 是当前正在执行的任务
    prev = rq->curr;

    prev_state = prev->state;
    if (!preempt && prev_state) {
        if (signal_pending_state(prev_state, prev)) {
            prev->state = TASK_RUNNING;
        } else {
        /* preempt 如果为false,
            则说明此次调度不是由于任务抢占导致的，那么导致调度发生的原因就是任务主动要求让出CPU,
            对于由于IO事件进入睡眠的任务而言，需要先将其从运行队列中踢出去。该函数最终会调用调度类（sched_class）的
            dequeue_task 方法完成具体工作。*/
           deactivate_task(rq, prev, DEQUEUE_SLEEP | DEQUEUE_NOCLOCK);
        }
    }

    //从队列中选择下一个任务，该函数最终会调用调度类（sched_class的函数pick_next_task方法。对于CFS而言，就是选择vruntime最小的任务
    next = pick_next_task(rq, prev, &rf);

    //清除 prev 任务的TIF_NEED_RESCHED标记，因为此时它已经被抢占了
    clear_tsk_need_resched(prev);

    if (likely(prev != next)) {
        //完成上下文切换，CPU将开始执行刚刚挑出来的任务next
        rq = context_switch(rq, prev, next, &rf);
    }
}

```

#### 调度的情况
调用了 schedule() 函数通常发生在内核需要进行任务调度的情况下。以下是一些常见的情况：

- 抢占性调度：当一个更高优先级的任务需要立即执行时，内核会调用 schedule() 函数以切换到该任务执行，这通常是由中断处理程序或其他紧急事件引起的。

- 睡眠唤醒：当一个任务因等待某些事件（如 I/O 完成）而进入睡眠状态时，内核会调用 schedule() 函数来挂起当前任务，并选择下一个可运行的任务执行。

- 系统调用：一些系统调用可能会导致任务进入睡眠状态，如等待某个信号量或锁的释放。当系统调用返回时，内核会调用 schedule() 函数重新调度任务。

- 时间片用尽：当任务的时间片用尽时，内核会调用 schedule() 函数来选择下一个任务执行，以确保公平地分配 CPU 时间片

