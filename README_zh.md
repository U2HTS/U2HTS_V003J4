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

# 已知问题
在Windows平台上，当触摸屏幕后，触摸事件会延迟很长一段时间之后才出现...  
Linux平台(Android)可以正常工作, 回报率125 Hz(`getevent -r`) .
