## PREEMPT-RT
PREEMPT-RT 补丁（Real-Time Patch）对 Linux 内核进行了多项关键改进，以实现低延迟和高确定性的实时性能。以下是 PREEMPT-RT 补丁主要的技术改进和操作。



https://wiki.linuxfoundation.org/realtime/preempt_rt_versions

https://lwn.net/Articles/146861/

根据这两个文章描述：该补丁在最小代码修改的前提下，尽可能的修改不可抢占的代码路径，



### 1. 完全可抢占内核（Fully Preemptible Kernel）

- **抢占性增强**：将内核中的几乎所有代码路径都变为可抢占状态。即使在内核中执行关键路径代码时，高优先级任务也可以中断当前任务执行。
- **自旋锁转变为互斥锁**：将自旋锁（spinlocks）转换为可以被抢占的互斥锁（mutexes），从而避免在持有锁时长时间阻塞高优先级任务。

TODO：







### 2. 中断线程化（Threaded Interrupts）

- **中断处理线程化**：将硬件中断处理程序分为上半部和下半部。上半部处理时间极短，主要负责响应中断并唤醒下半部处理程序。下半部作为内核线程运行，可以被调度和抢占，从而减少中断处理对系统实时性的影响。
- **request_threaded_irq**：使用 `request_threaded_irq()` 注册中断时，可以指定线程化的中断处理程序，增强对中断的控制和管理。

但是默认现在内核已经是中断线程化了。

### 3. 高精度定时器（High-Resolution Timers）

- **高精度计时**：增强内核定时器的精度，支持微秒级的定时需求。这对于实现高精度的周期性任务和精确的延迟时间非常重要。

### 4. 实时调度器改进（Scheduler Improvements）

- **实时调度策略**：增强调度器以支持实时调度策略，包括 FIFO（First In, First Out）和 RR（Round Robin）。这些策略确保高优先级实时任务能够按照预期的优先级顺序执行。
- **预防优先级反转**：通过优先级继承机制（priority inheritance），防止优先级反转问题，确保高优先级任务不会被低优先级任务长时间阻塞。

