# U2HTS_V003J4
Experimental U2HTS CH32V003J4M6 port, based on rv003usb software usb stack  
[zh_CN(简体中文)](./README_zh.md)  
<img width="639" height="356" alt="image" src="https://github.com/user-attachments/assets/e0d4ac09-5b0e-463c-b538-af61cf0c27c6" />

# WARNING
This port is built upon **Bit-banging USB Stack**. Expect unstability and bugs!  

# Build
```bash
sudo apt install libnewlib-dev gcc-riscv64-unknown-elf make
git clone https://github.com/U2HTS/U2HTS_V003J4.git --depth 1 --recursive
cd U2HTS_V003J4
# for gt9xx
TOUCH_CONTROLLER = ./u2hts_touch_controllers/gt9xx.c make build
```

# Report Rate
`rv003usb` only supports USB LowSpeed, with a limit of maximum endpoint transfer size `8`.
| Item | size |
| --- | --- |
| Report ID | 1 |
| Touch Point | 4 * `U2HTS_MAX_TPS` |
| Scan Time | 2 |
| Touch Count | 1 |

The HID report length equal `1 + 4 * 5 + 2 + 1 = 24` with `U2HTS_MAX_TPS` = `5`.  
USB host polls device under 1ms if `bInterval` set to 1, which means to complete a report needs `24 bytes / 8 bytes = 3` transfers.  
When set `U2HTS_MAX_TPS` to `10`, it will need `44 bytes / 8 bytes = 6` transfers.  
Under most conditions ,`5` touch points could deal daily works.  