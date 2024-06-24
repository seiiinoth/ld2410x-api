#include "unity.h"
#include "ld2410x.h"
#include "common.h"

#include <memory.h>

/* Read firmware version */
void test_ld2410x_get_fw_ver()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  for (uint8_t i = 0; i <= 1; i++) {
    uint8_t firmware_type = i;

    for (uint8_t j = 0; j < UINT8_MAX; j++) {
      uint16_t expected_ver16[3] = {0};
      uint8_t expected_ver8[6]   = {0};

      for (uint8_t k = 0; k < 6; k++) {
        expected_ver8[k] = k + j;
      }

      expected_ver16[0] = LD2410X_MERGE_UINT8(expected_ver8[0], expected_ver8[1]);
      expected_ver16[1] = LD2410X_MERGE_UINT8(expected_ver8[2], expected_ver8[3]);
      expected_ver16[2] = LD2410X_MERGE_UINT8(expected_ver8[4], expected_ver8[5]);

      uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xA0, 0x00, 0x04, 0x03, 0x02, 0x01};
      uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x0C, 0x00, 0xA0, 0x01, 0x00, 0x00, 0x00, firmware_type, expected_ver8[1], expected_ver8[0], expected_ver8[5], expected_ver8[4], expected_ver8[3], expected_ver8[2], 0x04, 0x03, 0x02, 0x01};

      size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
      size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

      memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
      memcpy(rx_buffer, exp_rx, rx_size);

      uint8_t rslt = ld2410x_get_fw_ver(&dev);
      if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
      TEST_ASSERT_EQUAL(LD2410X_OK, rslt);
      TEST_ASSERT_EQUAL(firmware_type, dev.fw_type);
      TEST_ASSERT_EQUAL_INT16_ARRAY(expected_ver16, dev.fw_ver, sizeof(expected_ver16) / sizeof(expected_ver16[0]));

      TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
      TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
    }
  }
}

/* ERROR TEST */
/* Failure ACK status */
void test_ld2410x_get_fw_ver_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA0, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_get_fw_ver(&dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_CONFIG_FAIL, rslt);
}

void setUp()
{
}

void tearDown()
{
}

int main(void)
{
  UNITY_BEGIN();

  RUN_TEST(test_ld2410x_get_fw_ver);
  RUN_TEST(test_ld2410x_get_fw_ver_fail_ack);

  return UNITY_END();
}
