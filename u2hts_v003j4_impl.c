#include "lib_i2c.h"
#include "rv003usb.h"
#include "u2hts_core.h"
#include "u2hts_v003j4.h"

__attribute__((__aligned__(
    4))) static uint8_t usb_tx_buf[sizeof(u2hts_hid_report) + 1] = {0};
static volatile bool transfer_done = false;
static uint8_t tx_offset = 0;

i2c_device_t dev = {
    .clkr = 0,
    .type = I2C_ADDR_7BIT,
    .addr = 0x00,
    .regb = 1,
    .tout = 2000,
};

inline void u2hts_i2c_init(uint32_t speed_hz) {
  dev.clkr = speed_hz;
  i2c_init(&dev);
}

void u2hts_i2c_set_speed(uint32_t speed_hz) {
  dev.clkr = speed_hz;
  i2c_init(&dev);
}

bool u2hts_i2c_write(uint8_t slave_addr, void* buf, size_t len, bool stop) {
  dev.addr = slave_addr;
  return !i2c_write_raw(&dev, buf, len);
}

bool u2hts_i2c_read(uint8_t slave_addr, void* buf, size_t len) {
  dev.addr = slave_addr;
  return !i2c_read_raw(&dev, buf, len);
}

bool u2hts_i2c_detect_slave(uint8_t addr) { return !i2c_ping(addr); }

void u2hts_spi_init(u2hts_spi_config* config) {}
bool u2hts_spi_transfer(void* buf, size_t len) { return true; }

void u2hts_tpint_set_mode(bool mode, bool pull) {
  U2HTS_UNUSED(mode);
  U2HTS_UNUSED(pull);
}

void u2hts_tpint_set(bool value) { U2HTS_UNUSED(value); }
bool u2hts_tpint_get() { return true; }
void u2hts_ts_irq_set(bool enable) { UNUSED(enable); }
void u2hts_ts_irq_init(U2HTS_IRQ_TYPES irq_type) { UNUSED(irq_type); }

void u2hts_tprst_set(bool value) { funDigitalWrite(PC4, value); }
inline void u2hts_delay_ms(uint32_t ms) { Delay_Ms(ms); }
inline void u2hts_delay_us(uint32_t us) { Delay_Us(us); }

void u2hts_usb_report(uint8_t report_id, const u2hts_hid_report* report) {
  usb_tx_buf[0] = report_id;
  memcpy(usb_tx_buf + 1, report, sizeof(u2hts_hid_report));
  tx_offset = 0;
  transfer_done = false;
}

bool u2hts_usb_init() {
  usb_setup();
  return true;
}

uint16_t u2hts_get_timestamp() { return SysTick->CNT & 0xFFFF; }
void u2hts_led_set(bool on) { UNUSED(on); }
void u2hts_write_config(uint16_t cfg) { UNUSED(cfg); }
uint16_t u2hts_read_config() { return 0; }
bool u2hts_usrkey_get() { return false; }

void usb_handle_hid_get_report_start(struct usb_endpoint* e, int reqLen,
                                     uint32_t lValueLSBIndexMSB) {
  switch (lValueLSBIndexMSB - 0x0300) {
    case U2HTS_HID_REPORT_TP_MAX_COUNT_ID:
      e->max_len = reqLen < 2 ? reqLen : 2;
      usb_tx_buf[0] = U2HTS_HID_REPORT_TP_MAX_COUNT_ID;
      usb_tx_buf[1] = u2hts_get_max_tps();
      transfer_done = true;
      break;

    default:
      e->max_len = 0;
      break;
  }
  e->opaque = usb_tx_buf;
}

void usb_handle_hid_set_report_start(struct usb_endpoint* e, int reqLen,
                                     uint32_t lValueLSBIndexMSB) {
  e->opaque = usb_tx_buf;
  e->max_len = reqLen;
}

void usb_handle_user_in_request(struct usb_endpoint* e, uint8_t* scratchpad,
                                int endp, uint32_t sendtok,
                                struct rv003usb_internal* ist) {
  if (endp && !transfer_done) {
    int8_t remaining_bytes = sizeof(usb_tx_buf) - tx_offset;
    if (remaining_bytes <= 0) {
      transfer_done = true;
      usb_send_empty(sendtok);
    } else {
      usb_send_data(usb_tx_buf + tx_offset,
                    remaining_bytes > 8 ? 8 : remaining_bytes, 0, sendtok);
      tx_offset += 8;
    }
  } else
    usb_send_empty(sendtok);
}

void usb_handle_other_control_message(struct usb_endpoint* e, struct usb_urb* s,
                                      struct rv003usb_internal* ist) {
  // LogUEvent( SysTick->CNT, s->wRequestTypeLSBRequestMSB,
  // s->lValueLSBIndexMSB, s->wLength );
}

// true = okay false = busy
inline bool u2hts_get_usb_status() { return transfer_done; }