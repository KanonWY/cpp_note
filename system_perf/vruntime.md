## 理解 vruntime


### 1. 什么是 vruntime ?
vruntime 是每一个任务（线程）已经使用 CPU 时间的加权值。它与实际运行时间不同。它会根据任务的优先级进行调整，较高优先级
的任务其 vruntime 增加缓慢，较低优先级的任务 vruntime 增加快速。

### 2. vruntime 的作用？
CFS 调度器通过 vruntime 来选择下一个需要执行的任务。具体而言，调度器会选择 vruntime 最小的任务作为下一个运行的任务。


### 3. vruntime 如何在内核中维护和更新

```c
/**
 * @brief 更新虚拟运行时间
 */
static inline void update_curr(struct cfs_rq *cfs_rq)
{
    // 获取获取当前运行的调度实体
    struct sched_entity *curr = cfs_rq->curr;

    // 获取当前的时钟时间
    u64 now = rq_clock_task(rq_of(cfs_rq));
    u64 delta_exec;

    if (unlikely(!curr))
        return;

    // 任务在当前 CPU 上实际执行时间
    //计算当前时间和任务开始执行的时间
    delta_exec = now - curr->exec_start;
    if ((s64)delta_exec <= 0)
        return;

    // 更新当前任务的虚拟运行时间
    curr->vruntime += calc_delta_fair(delta_exec, curr);
    curr->exec_start = now;

    // 更新 CFS 就绪队列的最小 虚拟运行时间
    update_min_vruntime(cfs_rq);
}
```
