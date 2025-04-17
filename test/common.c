#include "common.h"
#include <memory.h>

uint8_t tx_buffer[LD2410X_MAX_FRAME_LEN] = {0};
uint8_t rx_buffer[LD2410X_MAX_FRAME_LEN] = {0};

LD2410X_INTF_RET_TYPE ld2410x_read(uint8_t *buffer, uint8_t buff_len, void *intf_desc)
{
  (void)intf_desc;
  memcpy(buffer, rx_buffer, buff_len);
  return 0;
}

LD2410X_INTF_RET_TYPE ld2410x_write(const uint8_t *buffer, uint8_t buff_len, void *intf_desc)
{
  (void)intf_desc;
  memcpy(tx_buffer, buffer, buff_len);
  return 0;
}
