## ethtool

ethtool - 查询或控制网络驱动程序和硬件设置.

- 查看驱动信息
- 查看 ringbuffer 的信息
- 修改 ringbuffer 大小

#### 1、查看网卡驱动

```bash
> ethtool -i enx00e04c360206
driver: r8152
version: v1.12.13
firmware-version:
expansion-rom-version:
bus-info: usb-0000:00:14.0-3.2
supports-statistics: yes
supports-test: no
supports-eeprom-access: no
supports-register-dump: no
supports-priv-flags: no
```

- `driver`: 网卡所使用的驱动程序名称，这里是 r8152。
- `version`: 网卡驱动程序的版本号，此处为 v1.12.13。
- `firmware-version`: 网卡的固件版本号，通常在这里显示网卡的固件版本信息，但此处未提供。
- `expansion-rom-version`: 扩展只读存储器 (ROM) 的版本号，通常在这里显示网卡扩展 ROM 的版本信息，但此处未提供。
- `bus-info`: 网卡所连接的总线信息，这里显示网卡连接的 USB 总线信息。
- `supports-statistics`: 显示网卡是否支持统计信息功能。在此处是支持的。
- `supports-test`: 显示网卡是否支持测试功能。在此处是不支持的。
- `supports-eeprom-access`: 显示网卡是否支持 EEPROM 存储器的访问功能。在此处是不支持的。
- `supports-register-dump`: 显示网卡是否支持寄存器转储功能。在此处是不支持的。
- `supports-priv-flags`: 显示网卡是否支持私有标志位。在此处是不支持的。

#### 2、网络使用统计

```bash
ethtool -S enx00e04c360206
NIC statistics:
     tx_packets: 596939
     rx_packets: 1296233
     tx_errors: 0
     rx_errors: 0
     rx_missed: 0
     align_errors: 0
     tx_single_collisions: 0
     tx_multi_collisions: 0
     rx_unicast: 738162
     rx_broadcast: 182168
     rx_multicast: 375903
     tx_aborted: 0
     tx_underrun: 0
```

#### 3、查看 ring-buffer

```bash
> ethtool -g enx00e04c360206
Ring parameters for enx00e04c360206:
Pre-set maximums:
RX:             4096
RX Mini:        0
RX Jumbo:       0
TX:             0
Current hardware settings:
RX:             100
RX Mini:        0
RX Jumbo:       0
TX:             0
```

- **Pre-set maximums**：显示了接口的预设最大值。在这个示例中：
  - RX：表示接收环形缓冲区的预设最大值为4096。
  - RX Mini：表示接收小型帧的预设最大值为0。
  - RX Jumbo：表示接收大型帧的预设最大值为0。
  - TX：表示发送环形缓冲区的预设最大值为0。
- **Current hardware settings**：显示了接口的当前硬件设置。在这个示例中：
  - RX：表示接收环形缓冲区的当前设置为100。

注意到发送的 ringbuffer 为 0，发送缓冲区的预设最大值为0表示没有预设的发送环形缓冲区大小限制。这通常意味着发送缓冲区的大小由系统动态调整，可能会根据系统负载和其他因素进行自适应调整。



#### 4、修改 ringbuffer 大小

```bash
# 修改buffer size大小
ethtool -G enx00e04c360206 rx 2048
ethtool -G enx00e04c360206 tx 2048

# 修改后
ethtool -g enx00e04c360206
Ring parameters for enx00e04c360206:
Pre-set maximums:
RX:             4096
RX Mini:        0
RX Jumbo:       0
TX:             0
Current hardware settings:
RX:             2048
RX Mini:        0
RX Jumbo:       0
TX:             0
```

