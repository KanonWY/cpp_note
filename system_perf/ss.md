## ss

`ss` 是一个用于查看套接字（sockets）详细信息的实用工具。它可以显示当前系统上的各种套接字连接信息，包括 TCP、UDP、Unix 套接字、RAW 和 PACKET 等.

[ss 使用](https://zhuanlan.zhihu.com/p/513972766)

### 基础命令

#### 显示协议的摘要信息

```bash
> ss -s
Total: 1492
TCP:   63 (estab 39, closed 11, orphaned 0, timewait 3)

Transport Total     IP        IPv6
RAW       1         0         1
UDP       26        24        2
TCP       52        47        5
INET      79        71        8
FRAG      0         0         0
```

- **Total**: 总的套接字数量为 1492。
- **TCP**: 显示了 TCP 协议相关的统计信息。
  - **estab**: 已建立的 TCP 连接数量为 39。
  - **closed**: 已关闭的 TCP 连接数量为 11。
  - **orphaned**: 孤立的 TCP 连接数量为 0。
  - **timewait**: 处于 TIME_WAIT 状态的 TCP 连接数量为 3。
- **Transport**: 显示了不同传输层协议（RAW、UDP、TCP）的统计信息。
  - **Total**: 不同传输层协议的总数量为 79。
  - **IP**: IPv4 的传输层协议数量为 71。
  - **IPv6**: IPv6 的传输层协议数量为 8。
- **RAW**: 显示了 RAW 套接字的统计信息，总数量为 1。
  - **IP**: 在 IPv4 下的 RAW 套接字数量为 0。
  - **IPv6**: 在 IPv6 下的 RAW 套接字数量为 1。
- **UDP**: 显示了 UDP 套接字的统计信息，总数量为 26。
  - **IP**: 在 IPv4 下的 UDP 套接字数量为 24。
  - **IPv6**: 在 IPv6 下的 UDP 套接字数量为 2。
- **TCP**: 显示了 TCP 套接字的统计信息，总数量为 52。
  - **IP**: 在 IPv4 下的 TCP 套接字数量为 47。
  - **IPv6**: 在 IPv6 下的 TCP 套接字数量为 5。
- **INET**: 显示了总的 IPv4 和 IPv6 套接字数量，总数量为 79。
- **FRAG**: 显示了分片套接字的统计信息，总数量为 0。

#### 显示所有套接字信息

```bash
ss -a
```

Unix 套接字：

1. **u_str（Stream）**：
   - **数据传输方式**：提供面向连接的、可靠的、基于字节流的数据传输。
   - **通信特性**：类似于 TCP 套接字，数据按照发送顺序到达，并且保证可靠地交付。支持双向通信。
   - **适用场景**：适用于需要可靠的、面向连接的字节流传输的场景，如进程间通信、管道等。
2. **u_seq（Sequenced Packet）**：
   - **数据传输方式**：提供面向连接的、可靠的、基于消息的数据传输。
   - **通信特性**：类似于 UDP 套接字，每个消息是独立的，可以以任意顺序到达，但是保证按照发送的顺序交付。支持双向通信。
   - **适用场景**：适用于需要按消息处理、但仍然需要可靠传输的场景，如日志记录、消息传递等。
3. **u_dgr（Datagram）**：
   - **数据传输方式**：提供无连接的、不可靠的数据传输，类似于 UDP 套接字。
   - **通信特性**：每个数据报是独立的，可能以任意顺序到达，不保证按照发送的顺序交付。不支持连接状态。
   - **适用场景**：适用于不需要保持连接状态、每个数据报都是独立处理的场景，如网络广播、简单通信等。

#### 显示指定类型的套接字信息

```bash
ss -ta  # TCP socket
ss -ua  # UDP socket
ss -wa  # RAW socket
ss -xa  # UNIX socket
ss -4a  # IPV4 socket
ss -6a  # IPV6 socket
```

#### 仅显示监听的套接字信息

```bash
ss -lat # 显示监听的 tcp 套接字信息
```

#### 使用端口号而不是服务名

```bash
ss -latn # -n 可以将服务名显示为port
```

-p 可以显示程序名

```bash
ss -latnp
```

#### 支持状态过滤和ip过滤

```bash
# 列举出处于 FIN-WAIT-1状态的源端口为 80或者 443，目标网络为 193.233.7/24所有 tcp套接字
ss -o state FIN-WAIT-1 dst 192.168.25.100/24
```

#### 支持查看 socket 内存使用

```bash
s -mul
State                 Recv-Q                Send-Q                               Local Address:Port                                   Peer Address:Port                Process
UNCONN                0                     0                                          0.0.0.0:57330                                       0.0.0.0:*
         skmem:(r0,rb212992,t0,tb212992,f4096,w0,o0,bl0,d0)
```

```bash
skmem:(r<rmem_alloc>,rb<rcv_buf>,t<wmem_alloc>,tb<snd_buf>,
                            f<fwd_alloc>,w<wmem_queued>,o<opt_mem>,
                            bl<back_log>,d<sock_drop>)
```

- `<rmem_alloc>`: 用于接收数据包的内存分配。
- `<rcv_buf>`: 可用于接收数据包的总内存。
- `<wmem_alloc>`: 已发送到第3层的数据包的发送内存使用情况。
- `<snd_buf>`: 可用于发送数据包的总内存。
- `<fwd_alloc>`: 套接字分配的作为缓存的内存，但尚未用于接收/发送数据包。如果需要内存来发送/接收数据包，则将首先使用此缓存中的内存，然后再分配额外的内存。
- `<wmem_queued>`: 用于发送数据包的内存（尚未发送到第3层）。
- `<opt_mem>`: 用于存储套接字选项的内存，例如 TCP MD5 签名的密钥。
- `<back_log>`: 用于 sk 后台队列的内存。在进程上下文中，如果进程正在接收数据包，并且接收到新的数据包，则将其放入 sk 后台队列中，以便进程可以立即接收到。
- `<sock_drop>`: 在数据包解复用到套接字之前丢弃的数据包数量。