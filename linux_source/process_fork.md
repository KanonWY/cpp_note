## fork

```c
#define CSIGNAL 0x000000ff /* signal mask to be sent at exit */ 
#define CLONE_VM 0x00000100 /* 父子进程之间共享内存空间 */
#define CLONE_FS 0x00000200 /* 父子进程之间共享相同的文件系统 */
#define CLONE_FILES 0x00000400 /* 父子进程共享相同的文件描述符 */ 
#define CLONE_SIGHAND 0x00000800 /* 父子进程共享相同的信号处理等相关信息 */ 
#define CLONE_PTRACE 0x00002000 /* 父进程被trace，子进程也同样被trace */
#define CLONE_VFORK 0x00004000 /* 父进程被挂起，直到子进程释放了虚拟内存资源 */
#define CLONE_PARENT 0x00008000/* 新进程和创建它的进程是兄弟关系，而不是父子关系 */
#define CLONE_THREAD 0x00010000/* 父子进程共享相同的线程群*/
```

#### 核心函数

```c++
static struct task_struct *copy_process(unsigned long clone_flags,
                            unsigned long stack_start, unsigned long stack_size,
                            int __user *child_tidptr, struct pid *pid,
                            int trace);

//  |
//  |
// \|/
// 复制task_struct
static struct task_struct *dup_task_struct(struct task_struct *orig)
{
   
}

// |
// |
//\|/
// 初始化进程调度
int sched_fork(unsigned long clone_flags, struct task_struct *p)
{
  	unsigned long flags;
  	int cpu = get_cpu();							// 获取 CPU
  	__sched_fork(clone_flags, p);			// 初始化进程调度相关的数据结构
  	p->state = TASK_RUNNING;					// 设置进程状态
  	p->prio = current->normal_prio;		// 设置优先级
}
```

#### 初始化调度相关

调度实体用 `struct sched_entity` 数据结构来抽象，每个进程或线程都是一个调度实体，另外也包括组调度。

```c++
static void __sched_fork(unsigned long clone_flags, struct task_struct *p)
{
  
}
```

#### 初始化内存空间

do**_**fork()->copy**_**process()->copy_mm()

```c
static int copy_mm(unsigned long clone_flags, struct task_struct *tsk)
{
  struct mm_struct *mm, *oldmm;
  //...
  tsk->mm = NULL;
  task->active_mm = NULL;
  
  old_mm = current->mm;
  if(!oldmm)
  {
    return 0;
  }
  if(clone_flags & CLONE_VM)
  {
    atomic_inc(&oldmm->mm_users);
    mm = oldmm;
    goto goog_mm;
  }
  retval = -ENOMEM;
  mm = dup_mm(tsk);
  if (!mm)
  {
    goto fail_nomem;
  }
good_mm:
  tsk->mm = mm;
  tsk->active_mm = mm;
  return 0;
}
```

















