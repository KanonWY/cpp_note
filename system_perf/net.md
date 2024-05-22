## net

#### RX error

表示总的收包的错误数量：

 too-long-frames 错误，Ring Buffer 溢出错误，crc 校验错误，帧同步错误，fifo overruns 以及 missed pkg 等等。



-  **too-long-frames 错误**：指接收到的数据帧长度超过了网络硬件或软件所能处理的最大长度。可能是由于网络链路上的干扰或故障导致的。

- **Ring Buffer 溢出错误**：表示环形缓冲区溢出，即缓冲区无法及时处理数据而导致数据丢失。可能是由于网络负载过重或数据处理速度不足导致的。

- **CRC 校验错误**：CRC（Cyclic Redundancy Check）校验错误表示数据帧在传输过程中发生了损坏。这可能是由于网络噪声、电磁干扰或设备故障导致的。
- **帧同步错误**：指接收到的数据帧与网络同步时钟不匹配，可能导致数据丢失或传输错误。通常是由于网络同步问题或硬件故障引起的。
- **FIFO overruns**：表示先进先出（FIFO）缓冲区溢出，即数据在入队列时，队列已满而导致数据丢失。可能是由于数据处理速度不足或硬件故障引起的。
- **Missed pkg**：指未能成功接收到的数据包数量。可能是由于网络链路质量差、设备故障或网络拥塞等原因导致的。

#### RX drop

表示数据包已经进入了 Ring Buffer，但是由于内存不够等系统原因，导致在拷贝到内存的过程中被丢弃。

#### RX overruns

表示了 fifo 的 overruns，这是由于 Ring Buffer(aka Driver Queue) 传输的 IO 大于 kernel 能够处理的 IO 导致的，而 Ring Buffer 则是指在发起 IRQ 请求之前的那块 buffer。很明显，overruns 的增大意味着数据包没到 Ring Buffer 就被网卡物理层给丢弃了，而 CPU 无法即使的软处理中断是造成 Ring Buffer 满的原因之一。常见的问题是机器就是因为 interruprs 分布的不均匀，导致软中断全部挤压在一个核上，可以通过做 affinity 解决。



#### 网卡收包流程

网线上的packet首先被网卡获取，网卡会检查packet的CRC校验，保证完整性，然后将packet头去掉，得到frame。网卡会检查MAC包内的目的MAC地址，如果和本网卡的MAC地址不一样则丢弃。

### 查看网卡包传输信息

#### ifconfig

```bash
ifconfig
enx00e04c360206: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.9.22  netmask 255.255.255.0  broadcast 192.168.9.255
        inet6 fe80::df84:d726:3af7:7572  prefixlen 64  scopeid 0x20<link>
        ether 00:e0:4c:36:02:06  txqueuelen 1000  (Ethernet)
        RX packets 1277716  bytes 660698010 (660.6 MB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 577311  bytes 240582501 (240.5 MB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0
```

#### ip

```bash
ip -s link show enx00e04c360206
4: enx00e04c360206: <BROADCAST,MULTICAST,UP,LOWER_UP> mtu 1500 qdisc fq_codel state UP mode DEFAULT group default qlen 1000
    link/ether 00:e0:4c:36:02:06 brd ff:ff:ff:ff:ff:ff
    RX: bytes  packets  errors  dropped overrun mcast
    660796657  1279023  0       0       0       0
    TX: bytes  packets  errors  dropped carrier collsns
    242411526  578754   0       0       0       0
```

`RX: bytes  packets  errors  dropped overrun mcast` 是接收的字节数、数据包数、错误数、丢包数、溢出数和多播数据包数。

`TX: bytes  packets  errors  dropped carrier collsns` 是发送的字节数、数据包数、错误数、丢包数、载波错误数和碰撞数。

1. **载波错误数（Carrier Errors）**：
   - 载波错误指示在发送数据时发生的问题，这可能是由于无法检测到网络上的载波信号，或者与另一个设备的通信冲突导致的。
   - 这种错误通常与物理层的问题相关，例如网线连接不良、网线损坏或设备硬件故障等。
   - 载波错误数的增加可能表明存在网络连接问题，需要进一步检查物理连接或设备状态。
2. **碰撞数（Collisions）**：
   - 在早期的以太网中，当两个或多个设备同时尝试发送数据包到网络上时，会发生碰撞。这种情况通常是因为多个设备在同一时间发送数据，导致信号相互干扰，造成数据包的损坏。
   - 碰撞是网络中的一种正常现象，但是在现代以太网中，通过使用碰撞检测和以太网交换机等技术，碰撞的发生已经大大减少。
   - 碰撞数的增加可能表明网络负载过重，或者网络拓扑结构存在问题，需要优化网络配置以减少碰撞的发生。

#### ethtool

- 安装

```bash
sudo apt-get install ethtool
```

- 基本使用

```bash
ethtool -S enx00e04c360206
NIC statistics:
     tx_packets: 586598
     rx_packets: 1286125
     tx_errors: 0
     rx_errors: 0
     rx_missed: 0
     align_errors: 0
     tx_single_collisions: 0
     tx_multi_collisions: 0
     rx_unicast: 730386
     rx_broadcast: 181430
     rx_multicast: 374309
     tx_aborted: 0
     tx_underrun: 0
```

- `tx_packets`: 发送的数据包总数。
- `rx_packets`: 接收的数据包总数。
- `tx_errors`: 发送错误的数据包数量。
- `rx_errors`: 接收错误的数据包数量。
- `rx_missed`: 由于接收队列溢出而丢失的数据包数量。
- `align_errors`: 接收数据包中出现的字节对齐错误的数量。
- `tx_single_collisions`: 发送数据包时发生的单次碰撞次数。
- `tx_multi_collisions`: 发送数据包时发生的多次碰撞次数。
- `rx_unicast`: 接收的单播数据包数量。
- `rx_broadcast`: 接收的广播数据包数量。
- `rx_multicast`: 接收的多播数据包数量。
- `tx_aborted`: 发送过程中由于太多重试而放弃发送的数据包数量。
- `tx_underrun`: 发送数据时由于发送缓冲区中没有足够的数据而发生的欠流错误的数量。

### 丢包排查

```text
ethtool eth0 | egrep 'Speed|Duplex'
```

- 查看硬件速度和是否是全双工模式

```bash
ethtool enx00e04c360206  | grep 'Speed|Duplex'
        Speed: 100Mb/s
        Duplex: Full
```

- 检查 crc

```bash
ethtool -S enx00e04c360206 | grep crc
```

- 查看是否在网卡处丢包

```bash
# ifconfig 中的 RX 中的 overruns， 是否持续增长
for i in `seq 1 100`; do ifconfig enx00e04c360206: | grep RX | grep overruns; sleep 1; done
```

如果确认了丢包可以尝试增加 ringbuffer 的大小，尝试修改中断的 CPU 亲和。

#### 优化

1. 查看网卡支持多队列的情况，`ethtool -l enx00e04c360206`
2. 开启 irqbalance 服务，让系统自动调整网络中断在多个 CPU 核上的分配, `systemctl start irqbalance`
3. 开启 RPS/RFS 特性，软件方式实现 CPU 均衡，接收包中断的优化
