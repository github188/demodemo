top - 08:52:39 up 20 days, 23:27,  1 user,  load average: 3.54, 3.21, 3.03
Tasks: 190 total,   1 running, 189 sleeping,   0 stopped,   0 zombie
Cpu(s): 10.7%us,  3.8%sy,  0.0%ni, 75.2%id, 10.3%wa,  0.0%hi,  0.0%si,  0.0%st
Mem:   4142036k total,  3939860k used,   202176k free,    46324k buffers
Swap:  2048276k total,    40224k used,  2008052k free,  3321872k cached

> PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
> 9621 mysql     15   0 52552  27m 3968 S 30.6  0.7   8145:29 mysqld
19433 root      15   0  129m  91m 3032 S 12.0  2.3 763:05.89 python
> > 608 root      10  -5     0    0    0 S  0.3  0.0   7:01.54 scsi\_eh\_0

> 2012 root      16   0 71388  53m 2708 S  0.3  1.3 149:27.51 python

load average: 3.54, 3.21, 3.03 <== 1, 5, 15 分钟内平均等待的进程数。

wa = wiating for IO
us = userspace
sy = system/kernel
ni = nice processs
id = idle
hi = hardware interrupts
si = software interrupts

Tasks: 29 total 进程总数
1 running 正在运行的进程数
28 sleeping 睡眠的进程数
0 stopped 停止的进程数
0 zombie 僵尸进程数
Cpu(s): 0.3% us 用户空间占用CPU百分比
1.0% sy 内核空间占用CPU百分比
0.0% ni 用户进程空间内改变过优先级的进程占用CPU百分比
98.7% id 空闲CPU百分比
0.0% wa 等待输入输出的CPU时间百分比
0.0% hi
0.0% si

最后两行为内存信息。内容如下：

Mem: 191272k total 物理内存总量
173656k used 使用的物理内存总量
17616k free 空闲内存总量
22052k buffers 用作内核缓存的内存量

Swap: 192772k total 交换区总量
0k used 使用的交换区总量
192772k free 空闲交换区总量
123988k cached 缓冲的交换区总量。

[root@localhost data\_v2]# vmstat 1 100
procs -----------memory---------- ---swap-- -----io---- --system-- -----cpu------
> r  b   swpd   free   buff  cache   si   so    bi    bo   in   cs us sy id wa st
> 0  2  40224 215796 122456 3273124    0    0    74   111    1    0  8  3 83  7  0
> 0  2  40224 215676 122504 3273444    0    0    60  1316 1147 3878  2  0 84 14  0
> 0  1  40224 215492 122536 3273412    0    0    52  1340 1116 3135  2  0 85 13  0

> Procs
> > r: The number of processes waiting for run time.
> > b: The number of processes in uninterruptible sleep.

> Memory
> > swpd: the amount of virtual memory used.
> > free: the amount of idle memory.
> > buff: the amount of memory used as buffers.
> > cache: the amount of memory used as cache.

> Swap
> > si: Amount of memory swapped in from disk (/s).
> > so: Amount of memory swapped to disk (/s).

> IO
> > bi: Blocks received from a block device (blocks/s).
> > bo: Blocks sent to a block device (blocks/s).

> System
> > in: The number of interrupts per second, including the clock.
> > cs: The number of context switches per second.

> CPU
> > These are percentages of total CPU time.
> > us: Time spent running non-kernel code. (user time, including nice time)
> > sy: Time spent running kernel code. (system time)
> > id: Time spent idle. Prior to Linux 2.5.41, this includes IO-wait time.
> > wa: Time spent waiting for IO. Prior to Linux 2.5.41, included in idle.
> > st: Time stolen from a virtual machine. Prior to Linux 2.6.11, unknown.
