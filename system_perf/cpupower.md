## cpupower

CPU动态节能技术用于降低服务器功耗，通过选择系统空闲状态不同的电源管理策略，可以实现不同程度降低服务器功耗，更低的功耗策略意味着CPU唤醒更慢，这对 cpu 性能有一定的影响。

对于对时延和性能要求高的应用，建议关闭CPU的动态调节功能，禁止CPU休眠，并把CPU频率固定到最高。

### 查看cpu调频策略

```bash
cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_governor
```



查看见的策略类型：

1. **ondemand**：这个策略会根据 CPU 的负载情况来动态调整 CPU 的频率。当 CPU 负载较低时，频率会降低以节省能量，当负载增加时，频率会提高以提升性能。
2. **powersave**：该策略会尽可能地将 CPU 保持在最低频率，以最大程度地节省能量。这对于需要长时间运行的低负载任务很有用。
3. **conservative**：类似于 `ondemand`，但是在频率调整时更加保守，频率的变化会更加平滑，避免频繁的频率切换，从而减少功耗。
4. **userspace**：这个策略允许用户手动设置 CPU 的频率，用户可以通过修改相关文件来控制 CPU 的频率。
5. **performance**：这个策略会让 CPU 一直保持在最高频率，以获取最大的性能。这在需要高性能的任务或者实时应用中非常有用，比如游戏、视频编辑等。
6. **ondemand**：与 `performance` 相反，`ondemand` 策略会根据 CPU 负载情况动态调整 CPU 的频率。当 CPU 负载较低时，频率会降低以节省能量，当负载增加时，频率会提高以提升性能。这种策略适合于一般的桌面和移动设备，可以在保证性能的同时最大程度地节省能量。

### 查看频率信息

```bash
cpufreq-info
```

### 设置频率

```bash
cpufreq-set 是 Linux 中用于设置 CPU 频率和调频策略的命令行工具。以下是它的用法和选项：

-c CPU, --cpu CPU：指定要修改 cpufreq 设置的 CPU 编号。
-d FREQ, --min FREQ：设置新的最小 CPU 频率，即 governor 可以选择的最低频率。
-u FREQ, --max FREQ：设置新的最大 CPU 频率，即 governor 可以选择的最高频率。
-g GOV, --governor GOV：设置新的 cpufreq governor。
-f FREQ, --freq FREQ：设置特定的频率。需要可用且加载的 userspace governor。
-r, --related：切换所有相关的硬件 CPU。
-h, --help：显示帮助信息。
```

