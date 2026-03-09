# U2HTS_V003J4
Experimental U2HTS CH32V003J4M6 port, based on rv003usb software usb stack  
[zh_CN(简体中文)](./README_zh.md)  
<img width="639" height="356" alt="image" src="https://github.com/user-attachments/assets/e0d4ac09-5b0e-463c-b538-af61cf0c27c6" />

# WARNING
This port is built upon **Bit-banging USB Stack**. Expect unstability and bugs!  

# Build
```bash
sudo apt install libnewlib-dev gcc-riscv64-unknown-elf make
git clone https://github.com/U2HTS/U2HTS_V003J4.git --depth 1
cd U2HTS_V003J4
# for gt9xx
TOUCH_CONTROLLER = ./u2hts_touch_controllers/gt9xx.c make build
```

# Known bugs
On Windows platforms the finger event will delay a huge time to appear in system after touching screen...  
Linux platforms (Android) works well, report rate ~125 Hz (`getevent -r`).
