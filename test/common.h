#include "ld2410x.h"

#include <memory.h>
#include <string.h>

extern uint8_t tx_buffer[LD2410X_MAX_FRAME_LEN];
extern uint8_t rx_buffer[LD2410X_MAX_FRAME_LEN];

LD2410X_INTF_RET_TYPE ld2410x_read(uint8_t *buffer, uint8_t buff_len, void *intf_desc);

LD2410X_INTF_RET_TYPE ld2410x_write(const uint8_t *buffer, uint8_t buff_len, void *intf_desc);
