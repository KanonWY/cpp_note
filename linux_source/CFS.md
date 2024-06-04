## CFS

Completely Fair Scheduler。

内核为每一个 CPU 维护了一个 runqueue，CFS 有一个可配置的调度周期。调度过程可以简单理解为：

- CFS 根据当前可运行进程的数量 N，计算得到每个进程应该执行的时间 sched_latency/N；
- 依次取出进程执行以上计算出的时间；
- 如果 runqueue 有变化，再重新计算可执行时间。

