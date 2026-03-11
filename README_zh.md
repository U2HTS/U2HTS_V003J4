# U2HTS_V003J4
U2HTS 的 CH32V003J4M6 移植版, 基于 rv003usb 软件USB栈。  
<img width="639" height="356" alt="image" src="https://github.com/user-attachments/assets/e0d4ac09-5b0e-463c-b538-af61cf0c27c6" />

# 警告
此版本使用**软件模拟USB**协议栈，可能存在不稳定现象。  

# 构建
```bash
sudo apt install libnewlib-dev gcc-riscv64-unknown-elf make
git clone https://github.com/U2HTS/U2HTS_V003J4.git --depth 1
cd U2HTS_V003J4
# for gt9xx
TOUCH_CONTROLLER = ./u2hts_touch_controllers/gt9xx.c make build
```

# 回报率
`rv003usb`模拟USB对应的速率为USB LowSpeed，`Endpoint`最大被限制在`8`个字节。  
| 项目 | 长度 |
| --- | --- |
| Report ID | 1 |
| Touch Point | 4 * `U2HTS_MAX_TPS` |
| Scan Time | 2 |
| Touch Count | 1 |

如果设置`U2HTS_MAX_TPS`为`5`，那么整个HID回报包的长度为`1 + 4 * 5 + 2 + 1 = 24`。  
在`bInterval = 1`的情况下，主机轮询USB设备的频率为`1 ms`，则触摸包需要`24 bytes / 8 bytes = 3`次传输。  
当`U2HTS_MAX_TPS`为`10`，那么需要`44 bytes / 8 bytes = 6`次传输。  
在大多数情况下，`5`点触摸在日常场景中已经够用了。