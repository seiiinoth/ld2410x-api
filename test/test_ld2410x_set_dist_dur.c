#include "unity.h"
#include "ld2410x.h"

#include "common.h"

#include <stdbool.h>
#include <memory.h>

void test_ld2410x_set_dist_dur_valid_inputs()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read      = ld2410x_read;
  dev.write     = ld2410x_write;
  dev.intf_mode = LD2410X_CALLBACK_INTF;

  for (uint8_t i = LD2410X_MIN_DIST_GATE; i < (LD2410X_MAX_DIST_GATE + 1); i++) {
    uint8_t mov_dist  = i;
    uint8_t stat_dist = i;
    uint16_t u_dur    = 5;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x60, 0x00, 0x00, 0x00, mov_dist, 0x00, 0x00, 0x00, 0x01, 0x00, stat_dist, 0x00, 0x00, 0x00, 0x02, 0x00, (uint8_t)(u_dur), (uint8_t)(u_dur >> 8), 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x60, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_set_dist_dur(mov_dist, stat_dist, u_dur, &dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_OK, rslt);

    TEST_ASSERT_EQUAL(mov_dist, dev.conf.max_mov_dist_gate);
    TEST_ASSERT_EQUAL(stat_dist, dev.conf.max_stat_dist_gate);
    TEST_ASSERT_EQUAL(u_dur, dev.conf.unmanned_duration);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }

  for (uint16_t i = 0; i < UINT16_MAX / 2; i += 1000) {
    uint8_t mov_dist  = 5;
    uint8_t stat_dist = 5;
    uint16_t u_dur    = i;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x60, 0x00, 0x00, 0x00, mov_dist, 0x00, 0x00, 0x00, 0x01, 0x00, stat_dist, 0x00, 0x00, 0x00, 0x02, 0x00, (uint8_t)(u_dur), (uint8_t)(u_dur >> 8), 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x60, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_set_dist_dur(mov_dist, stat_dist, u_dur, &dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_OK, rslt);

    TEST_ASSERT_EQUAL(mov_dist, dev.conf.max_mov_dist_gate);
    TEST_ASSERT_EQUAL(stat_dist, dev.conf.max_stat_dist_gate);
    TEST_ASSERT_EQUAL(u_dur, dev.conf.unmanned_duration);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }

  for (uint16_t i = UINT16_MAX / 2; i < UINT16_MAX; i += 1000) {
    uint8_t mov_dist  = 5;
    uint8_t stat_dist = 5;
    uint16_t u_dur    = i;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x60, 0x00, 0x00, 0x00, mov_dist, 0x00, 0x00, 0x00, 0x01, 0x00, stat_dist, 0x00, 0x00, 0x00, 0x02, 0x00, (uint8_t)(u_dur), (uint8_t)(u_dur >> 8), 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x60, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_set_dist_dur(mov_dist, stat_dist, u_dur, &dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_OK, rslt);

    TEST_ASSERT_EQUAL(mov_dist, dev.conf.max_mov_dist_gate);
    TEST_ASSERT_EQUAL(stat_dist, dev.conf.max_stat_dist_gate);
    TEST_ASSERT_EQUAL(u_dur, dev.conf.unmanned_duration);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }
}

/* Correction of limit values */
void test_ld2410x_set_dist_dur_clamp_max_distance()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read      = ld2410x_read;
  dev.write     = ld2410x_write;
  dev.intf_mode = LD2410X_CALLBACK_INTF;

  for (uint8_t i = (LD2410X_MAX_DIST_GATE + 1); i < UINT8_MAX; i++) {
    uint8_t mov_dist  = i;
    uint8_t stat_dist = i;
    uint16_t u_dur    = 5;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x60, 0x00, 0x00, 0x00, LD2410X_MAX_DIST_GATE, 0x00, 0x00, 0x00, 0x01, 0x00, LD2410X_MAX_DIST_GATE, 0x00, 0x00, 0x00, 0x02, 0x00, (uint8_t)(u_dur), (uint8_t)(u_dur >> 8), 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x60, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_set_dist_dur(mov_dist, stat_dist, u_dur, &dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_OK, rslt);

    TEST_ASSERT_EQUAL(LD2410X_MAX_DIST_GATE, dev.conf.max_mov_dist_gate);
    TEST_ASSERT_EQUAL(LD2410X_MAX_DIST_GATE, dev.conf.max_stat_dist_gate);
    TEST_ASSERT_EQUAL(u_dur, dev.conf.unmanned_duration);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }
}

void test_ld2410x_set_dist_dur_clamp_min_distance()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read      = ld2410x_read;
  dev.write     = ld2410x_write;
  dev.intf_mode = LD2410X_CALLBACK_INTF;

  for (uint8_t i = 0; i < LD2410X_MIN_DIST_GATE; i++) {
    uint8_t mov_dist  = i;
    uint8_t stat_dist = i;
    uint16_t u_dur    = 5;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x60, 0x00, 0x00, 0x00, LD2410X_MIN_DIST_GATE, 0x00, 0x00, 0x00, 0x01, 0x00, LD2410X_MIN_DIST_GATE, 0x00, 0x00, 0x00, 0x02, 0x00, (uint8_t)(u_dur), (uint8_t)(u_dur >> 8), 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x60, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_set_dist_dur(mov_dist, stat_dist, u_dur, &dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_OK, rslt);

    TEST_ASSERT_EQUAL(LD2410X_MIN_DIST_GATE, dev.conf.max_mov_dist_gate);
    TEST_ASSERT_EQUAL(LD2410X_MIN_DIST_GATE, dev.conf.max_stat_dist_gate);
    TEST_ASSERT_EQUAL(u_dur, dev.conf.unmanned_duration);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }
}

void test_ld2410x_set_dist_dur_failure_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read      = ld2410x_read;
  dev.write     = ld2410x_write;
  dev.intf_mode = LD2410X_CALLBACK_INTF;

  uint8_t mov_dist  = 10;
  uint8_t stat_dist = 15;
  uint16_t u_dur    = 20;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x60, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_set_dist_dur(mov_dist, stat_dist, u_dur, &dev);

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

  RUN_TEST(test_ld2410x_set_dist_dur_valid_inputs);
  RUN_TEST(test_ld2410x_set_dist_dur_clamp_max_distance);
  RUN_TEST(test_ld2410x_set_dist_dur_clamp_min_distance);
  RUN_TEST(test_ld2410x_set_dist_dur_failure_ack);

  return UNITY_END();
}
