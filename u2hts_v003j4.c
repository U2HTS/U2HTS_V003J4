#include "u2hts_v003j4.h"
int main() {
  SystemInit();
  Delay_Ms(1);  // Ensures USB re-enumeration after bootloader or reset; Spec
                // demand >2.5µs ( TDDIS )
                
  u2hts_pins_init();
  u2hts_config cfg = {
      .controller = "gt9xx",
      .coord_config = {
        .max_tps = 5,
        .x_max = 720,
        .y_max = 1280
      },
      .report_delay=16,
  };
  cfg.polling_mode = true;  // no irq
  U2HTS_ERROR_CODES ret = u2hts_init(&cfg);
  if (ret)
    while (1);
  while (1) u2hts_task();
}