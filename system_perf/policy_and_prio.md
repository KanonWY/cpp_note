## 调度策略与优先级

### 调度策略
```c
// sched.h
#define SCHED_OTHER    0
#define SCHED_FIFO     1
#define SCHED_RR       2
#define SCHED_BATCH    3
#define SCHED_IDLE     5
#define SCHED_DEADLINE 6

```
### 优先级

内核使用 `0-139` 数值表示进程的优先级，数值越低优先级越高，`0-99`给实时进程使用，`100-139` 给普通进程使用. `nice`值将`100-139`映射为
`-20-19`.  
PCB 进程描述符有多个结构来描述当前进程的优先级相关信息.
```c
struct task_struct {
    int prio;                       ///<<< 
    int static_prio;                ///<<< 静态优先级，进程启动时分配,它不会随时间改变，可以修改nice值或者 sched_setscheduler来改变
    int normal_prio;                ///<<< static_prio 和调度策略计算出来的优先级.在创建 进程时会继承父进程的 normal_prio
    unsigned int rt_priority;       ///<<< 
};
```

以下内核中设置代码表明了他们之间的关系:
```c
// 内核进程初始化函数
void init_task(struct task_struct *p)
{
    // 根据 nice 值计算出优先级
    p->static_prio = NICE_TO_PRIO(p->static_nice);
    // 计算 normal_prio, 一般等于静态优先级
    p->normal_prio = p->static_prio;
    // 计算 prio
    p->prio = p->normal_prio;
}

// 实时进程的优先级设置函数
void set_rt_task_priority(struct task_struct *p, int rt_priority)
{
    p->rt_priority = rt_priority;
    p->prio = MAX_RT_PRIO - p->rt_priority;
}

// 普通进程的优先级设置函数
void set_normal_task_priority(struct task_struct *p, int nice)
{
    p->static_prio = NICE_TO_PRIO(nice);
    p->normal_prio = p->static_prio;
    p->prio = p->normal_prio;
}
```
对于普通进程取决于nice值，对于实时进程取决于设置时指定的优先级。

### CFS 调度
完全公平调度器（CFS，Completely Fair Scheduler）是 Linux 内核的默认进程调度器，它旨在提供一种高效且公平的调度机制。CFS 的核心原理可以通过以下几个关键点来描述:

#### 虚拟运行时间
（1）CFS 调度使用每一个进程的虚拟运行时间作为调度的主要指标。  
（2）`vruntime` 表示进程的累计运行时间，并且是根据进程的权重调节过的时间  
（3）`vruntime`越小的进程，表示它在 CPU 上消耗的时间少，因此优先级越高，下次调度时更有可能被选择  
（4）现代处理器一般是多核的，因此`vruntime` 表示在多个核心上的运行时间的累加  

#### 红黑树
内核使用红黑树来维护一个 CPU 上所有的可调度任务，其 key 值为`vruntime`，因此可以快速找到一个 CPU 上可执行队列中最小的`vruntime` 的任务.

#### 时间片
（1）每一个进程的运行时间是根据其权重来分配的，权重越高的进程获取的时间片越长  
（2）CFS 没有固定的时间片，而是在调度周期内动态调节每一个进程的运行时间达到公平的目的  

#### 权重与优先级
前面已经介绍过了优先级的的计算，内核并不是直接使用优先级来计算虚拟运行时间的，而是会把优先级使用查表的方式转换为权重，然后使用权重计算虚拟运行时间。
$$
虚拟运行时间=\frac{实际运行时间 * nice值为0的权重}{该进程的权重}
$$
从上面公式可以看出，实际权重越大的进程其虚拟运行时间越短，即其 `vruntime` 增长越慢，因此在调度中可以获取更多的时间.


#### 调度时机
1、进程主动发起调度。  
2、定时器发现当前进程耗尽时间，会选择调度（此时不是直接调度，而是设置一个标识位)
```c++
/* file: kernel/timer.c */

void update_process_times(int user_tick) {
    struct task_struct *p = current;
    int cpu = smp_processor_id(), system = user_tick ^ 1;

    update_one_process(p, user_tick, system, cpu);
    if (p->pid) {
        /* 运行时间耗尽，设置调度标记 */
        if (--p->counter <= 0) {
            p->counter = 0;
            p->need_resched = 1;
        }
        if (p->nice > 0)
            kstat.per_cpu_nice[cpu] += user_tick;
        else
            kstat.per_cpu_user[cpu] += user_tick;
        kstat.per_cpu_system[cpu] += system;
    } else if (local_bh_count(cpu) || local_irq_count(cpu) > 1)
        kstat.per_cpu_system[cpu] += system;
}
```
3、用户修改进程优先级的时候，这一点很好理解。  
4、系统通过 fork() 创建新任务的时候，新任务的产生会导致系统产生调度需求。  


