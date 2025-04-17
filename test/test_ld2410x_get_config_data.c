#include "unity.h"
#include "ld2410x.h"
#include "common.h"
#include "memory.h"
#include <stdio.h>

/* Read configuration data */
void test_ld2410x_get_config_data()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t max_dist_gate = 8;

  for (uint8_t i = LD2410X_MIN_DIST_GATE; i <= LD2410X_MAX_DIST_GATE; i++) {
    uint8_t max_mov_dist_gate  = i;
    uint8_t max_stat_dist_gate = i;
    uint16_t unmanned_duration = i * 300;

    for (uint8_t j = LD2410X_MIN_SENS; j <= LD2410X_MAX_SENS; j += 5) {
      uint8_t mov_sensitivity[LD2410X_MAX_DIST_GATE + 1];
      uint8_t stat_sensitivity[LD2410X_MAX_DIST_GATE + 1];

      uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x61, 0x00, 0x04, 0x03, 0x02, 0x01};
      uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x1C, 0x00, 0x61, 0x01, 0x00, 0x00, 0xAA, max_dist_gate, max_mov_dist_gate, max_stat_dist_gate, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x14, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, (uint8_t)(unmanned_duration), (uint8_t)(unmanned_duration >> 8), 0x04, 0x03, 0x02, 0x01};

      size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
      size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

      for (uint8_t k = 0; k <= LD2410X_MAX_DIST_GATE; k++) {
        mov_sensitivity[k]  = j;
        stat_sensitivity[k] = LD2410X_MAX_SENS - j;
      }

      memcpy(exp_rx + 14, mov_sensitivity, LD2410X_MAX_DIST_GATE + 1);
      memcpy(exp_rx + 14 + LD2410X_MAX_DIST_GATE + 1, stat_sensitivity, LD2410X_MAX_DIST_GATE + 1);

      memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
      memcpy(rx_buffer, exp_rx, rx_size);

      uint8_t rslt = ld2410x_get_config_data(&dev);
      if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
      TEST_ASSERT_EQUAL(LD2410X_OK, rslt);

      TEST_ASSERT_EQUAL(max_dist_gate, dev.conf.max_dist_gate);
      TEST_ASSERT_EQUAL(max_mov_dist_gate, dev.conf.max_mov_dist_gate);
      TEST_ASSERT_EQUAL(max_stat_dist_gate, dev.conf.max_stat_dist_gate);
      TEST_ASSERT_EQUAL(unmanned_duration, dev.conf.unmanned_duration);

      TEST_ASSERT_EQUAL_UINT8_ARRAY(mov_sensitivity, dev.conf.mov_sensitivity, LD2410X_MAX_DIST_GATE + 1);
      TEST_ASSERT_EQUAL_UINT8_ARRAY(stat_sensitivity, dev.conf.stat_sensitivity, LD2410X_MAX_DIST_GATE + 1);

      TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
      TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
    }
  }
}

void test_ld2410x_get_config_data_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x61, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_get_config_data(&dev);
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

  RUN_TEST(test_ld2410x_get_config_data);
  RUN_TEST(test_ld2410x_get_config_data_fail_ack);

  return UNITY_END();
}
