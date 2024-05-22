## netstat

用于显示各种网络相关信息，包括正在建立的连接、路由表、接口统计等。它可以帮助你查看当前系统的网络状态，有助于网络故障排除和监控。

### 1、显示组播部分信息

组播对应的网络接口，加入组播的数量，以及组播的地址。

```bash
netstat -g
IPv6/IPv4 Group Memberships
Interface       RefCnt Group
--------------- ------ ---------------------
lo              1      mdns.mcast.net
lo              1      all-systems.mcast.net
enp0s31f6       1      all-systems.mcast.net
enx00e04c3      1      239.0.0.6
enx00e04c3      3      239.0.0.1
enx00e04c3      2      mdns.mcast.net
enx00e04c3      1      all-systems.mcast.net
```

启动了三个 ecal 的程序，他们会使用组播进行互相的通信，这里可以看到组播的地址为：239.0.0.1 中有 3 个 成员。

- Interface 网口名
- RefCnt 成员数量
- Group 组播地址

**mdns.mcast.net**：这是用于多播 DNS（mDNS）服务的地址。mDNS 是一种在局域网内提供主机名解析服务的协议，它允许局域网内的设备通过多播方式进行主机名解析和服务发现，而无需使用专用的 DNS 服务器。多播组地址 `mdns.mcast.net` 是 mDNS 使用的多播地址之一。

**all-systems.mcast.net**：这是一个保留的多播组地址，用于向局域网内的所有系统广播消息。当系统希望向局域网内的所有设备发送消息时，可以使用这个地址。

### 2、查看路由表信息

查看内核路由表，使用 route 可以获取更加详细的信息

```bash
netstat -r
Kernel IP routing table
Destination     Gateway         Genmask         Flags   MSS Window  irtt Iface
default         _gateway        0.0.0.0         UG        0 0          0 enx00e04c360206
link-local      0.0.0.0         255.255.0.0     U         0 0          0 enx00e04c360206
172.17.0.0      0.0.0.0         255.255.0.0     U         0 0          0 docker0
172.18.0.0      0.0.0.0         255.255.0.0     U         0 0          0 br-4053aac84efa
192.168.9.0     0.0.0.0         255.255.255.0   U         0 0          0 enx00e04c360206
```

### 3、显示网络接口以及他们详细的信息统计

```bash
netstat -i
Kernel Interface table
Iface      MTU    RX-OK RX-ERR RX-DRP RX-OVR    TX-OK TX-ERR TX-DRP TX-OVR Flg
br-4053a  1500   121849      0      0 0        124384      0      0      0 BMRU
docker0   1500        0      0      0 0             0      0      0      0 BMU
enp0s31f  1500        0      0      0 0             0      0      0      0 BMU
enx00e04  1500  1189037      0      0 0        482602      0      0      0 BMRU
lo       65536  1192814      0      0 0       1192814      0      0      0 LRU
veth30a0  1500   121849      0      0 0        124485      0      0      0 BMRU
```

- **Iface**：网络接口的名称。
- **MTU**：最大传输单元（Maximum Transmission Unit），指网络接口可以发送的最大数据包大小。
- **RX-OK**：接收成功的数据包数量。
- **RX-ERR**：接收错误的数据包数量。
- **RX-DRP**：接收丢弃的数据包数量。
- **RX-OVR**：接收数据包的溢出数量。
- **TX-OK**：发送成功的数据包数量。
- **TX-ERR**：发送错误的数据包数量。
- **TX-DRP**：发送丢弃的数据包数量。
- **TX-OVR**：发送数据包的溢出数量。
- **Flg**：接口标志，显示了接口的一些属性。

flag 标志：

- **B**：此接口是一个桥接接口。
- **M**：此接口是多播设备。
- **R**：此接口支持 ARP 协议。
- **U**：此接口是 UP（启用）状态。

"LRU" 标志表示 "Loopback Receive Only。

### 4、各类协议的统计

```bash
netstat -s
Ip:
    Forwarding: 1
    2199702 total packets received
    4 with invalid addresses
    0 forwarded
    0 incoming packets discarded
    2138382 incoming packets delivered
    1767875 requests sent out
    20 outgoing packets dropped
    2 dropped because of missing route
Icmp:
    162 ICMP messages received
    17 input ICMP message failed
    ICMP input histogram:
        destination unreachable: 162
    1960 ICMP messages sent
    0 ICMP messages failed
    ICMP output histogram:
        destination unreachable: 1960
IcmpMsg:
        InType3: 162
        OutType3: 1960
Tcp:
    59141 active connection openings
    26279 passive connection openings
    2426 failed connection attempts
    258 connection resets received
    34 connections established
    1752027 segments received
    1732253 segments sent out
    5978 segments retransmitted
    154 bad segments received
    3433 resets sent
Udp:
    799380 packets received
    1864 packets to unknown port received
    0 packet receive errors
    64930 packets sent
    0 receive buffer errors
    0 send buffer errors
    IgnoredMulti: 51504
UdpLite:
TcpExt:
    3 invalid SYN cookies received
    44 ICMP packets dropped because they were out-of-window
    16107 TCP sockets finished time wait in fast timer
    28 time wait sockets recycled by time stamp
    2 packetes rejected in established connections because of timestamp
    22192 delayed acks sent
    28 delayed acks further delayed because of locked socket
    Quick ack mode was activated 1361 times
    487847 packet headers predicted
    312916 acknowledgments not containing data payload received
    411426 predicted acknowledgments
    TCPSackRecovery: 75
    Detected reordering 1466 times using SACK
    Detected reordering 2 times using time stamp
    17 congestion windows fully recovered without slow start
    1 congestion windows partially recovered using Hoe heuristic
    TCPDSACKUndo: 2
    13 congestion windows recovered without slow start after partial ack
    TCPLostRetransmit: 4033
    TCPSackFailures: 5
    7 timeouts in loss state
    121 fast retransmits
    36 retransmits in slow start
    TCPTimeouts: 5661
    TCPLossProbes: 522
    TCPLossProbeRecovery: 28
    TCPSackRecoveryFail: 8
    TCPBacklogCoalesce: 67832
    TCPDSACKOldSent: 1384
    TCPDSACKOfoSent: 16
    TCPDSACKRecv: 231
    TCPDSACKOfoRecv: 1
    355 connections reset due to unexpected data
    92 connections reset due to early user close
    327 connections aborted due to timeout
    TCPDSACKIgnoredNoUndo: 192
    TCPSpuriousRTOs: 1
    TCPSackShifted: 24
    TCPSackMerged: 47
    TCPSackShiftFallback: 2140
    IPReversePathFilter: 1
    TCPRcvCoalesce: 158194
    TCPOFOQueue: 41032
    TCPOFOMerge: 16
    TCPChallengeACK: 163
    TCPSYNChallenge: 154
    TCPSpuriousRtxHostQueues: 24
    TCPAutoCorking: 359
    TCPWantZeroWindowAdv: 4
    TCPSynRetrans: 3914
    TCPOrigDataSent: 865181
    TCPHystartTrainDetect: 13
    TCPHystartTrainCwnd: 326
    TCPHystartDelayDetect: 3
    TCPHystartDelayCwnd: 83
    TCPACKSkippedSeq: 1
    TCPACKSkippedFinWait2: 1
    TCPACKSkippedChallenge: 4
    TCPKeepAlive: 25674
    TCPDelivered: 920369
    TCPAckCompressed: 18927
    TcpTimeoutRehash: 5340
    TcpDuplicateDataRehash: 117
    TCPDSACKRecvSegs: 232
IpExt:
    InMcastPkts: 287996
    OutMcastPkts: 7294
    InBcastPkts: 62985
    OutBcastPkts: 11510
    InOctets: 7351701839
    OutOctets: 6842417091
    InMcastOctets: 79688546
    OutMcastOctets: 1387610
    InBcastOctets: 9048223
    OutBcastOctets: 4354279
    InNoECTPkts: 2373941
MPTcpExt:
```

`IpExt` 的输出提供了一些关于 IP 协议扩展的统计信息，其中包括多播和广播数据包的收发情况以及字节计数。

- **InMcastPkts**：接收到的多播数据包的数量。
- **OutMcastPkts**：发送的多播数据包的数量。
- **InBcastPkts**：接收到的广播数据包的数量。
- **OutBcastPkts**：发送的广播数据包的数量。
- **InOctets**：接收到的总字节数。
- **OutOctets**：发送的总字节数。
- **InMcastOctets**：接收到的多播数据包的总字节数。
- **OutMcastOctets**：发送的多播数据包的总字节数。
- **InBcastOctets**：接收到的广播数据包的总字节数。
- **OutBcastOctets**：发送的广播数据包的总字节数。
- **InNoECTPkts**：未使用 ECN（Explicit Congestion Notification）字段的数据包数量。

`UDP` 的输出提供了 UDP 相关信息

- **packets received**：接收到的 UDP 数据包总数。
- **packets to unknown port received**：接收到的 UDP 数据包中，发送到未知端口的数据包数量。
- **packet receive errors**：接收 UDP 数据包时发生的错误数量。
- **packets sent**：发送的 UDP 数据包总数。
- **receive buffer errors**：接收缓冲区发生的错误数量。
- **send buffer errors**：发送缓冲区发生的错误数量。
- **IgnoredMulti**：忽略的多播数据包数量。

### 5、常见的组合使用

#### 基础复合指令

```bash
# 显示 pid 和 程序名称
netstat -p

# 显示特定 ip 类型，如 ipv4、ipv6
netstat -4   # 仅显示 IPv4 连接
netstat -6   # 仅显示 IPv6 连接

# 显示特定协议
netstat -t   # 仅显示 tcp
netstat -u   # 仅显示 udp

# 显示指定状态
netstat -l  # 仅显示监听状态

# 显示扩展信息
netstat -e 
```

- 查看 TCP 的 send-Q 和 recive-Q

```bash
> netstat -tape
(Not all processes could be identified, non-owned process info
 will not be shown, you would have to be root to see it all.)
Active Internet connections (servers and established)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name
tcp        0      0 localhost:35877         0.0.0.0:*               LISTEN      4550/clash-linux
tcp        0      0 localhost:ipp           0.0.0.0:*               LISTEN      -
tcp        0      0 0.0.0.0:ssh             0.0.0.0:*               LISTEN      -
tcp        0      0 localhost:domain        0.0.0.0:*               LISTEN      -
tcp        0      0 localhost:16067         0.0.0.0:*               LISTEN      -
tcp        0      0 localhost:7890          0.0.0.0:*               LISTEN      4550/clash-linux
tcp        0      0 0.0.0.0:9001            0.0.0.0:*               LISTEN      -
tcp        0      0 0.0.0.0:1883            0.0.0.0:*               LISTEN      -
tcp        0      0 ka-ubuntu20:36934       124.193.244.233:https   ESTABLISHED 5971/feishu
```

- **Proto**：协议类型（例如 TCP、UDP、TCP6 等）。
- **Recv-Q**：接收队列中的字节数。
- **Send-Q**：发送队列中的字节数。
- **Local Address**：本地地址和端口。
- **Foreign Address**：远程地址和端口。
- **State**：连接状态。
- **User**：拥有该连接的用户。
- **Inode**：连接对应的 inode（索引节点）。
- **PID/Program name**：关联的进程标识符（PID）和程序名称