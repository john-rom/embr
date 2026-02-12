#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/reboot.h>

#include "embr_app.h"

LOG_MODULE_REGISTER(app);

int main(void) {
  int err = 0;

  err = embr_app_init();
  if (err) {
    LOG_ERR("embr init failed: %d", err);
    goto fatal;
  }
  LOG_INF("embr init success");

  err = embr_app_start();
  if (err) {
    LOG_ERR("embr start failed: %d", err);
    goto fatal;
  }
  LOG_INF("embr start success");

  k_sleep(K_FOREVER);

fatal:
  // Brief delay to complete logging
  k_sleep(K_MSEC(200));
  sys_reboot(SYS_REBOOT_COLD);
}
