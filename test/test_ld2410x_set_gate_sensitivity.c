#include "unity.h"
#include "ld2410x.h"
#include "common.h"

/*
 * Range Gate Sensitivity Configuration Commands
 * configure the motion sensitivity of distance gate 0-8 to value from 0 to 100, and the static sensitivity of value from 0 to 100
 */
void test_ld2410x_set_gate_sensitivity_single()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  for (uint8_t i = LD2410X_MIN_DIST_GATE; i <= LD2410X_MAX_DIST_GATE; i++) {
    uint8_t dist_gate = i;

    for (uint8_t j = LD2410X_MIN_SENS; j <= LD2410X_MAX_SENS; j += 10) {
      uint8_t mov_sens  = j;
      uint8_t stat_sens = j;

      uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x64, 0x00, 0x00, 0x00, dist_gate, 0x00, 0x00, 0x00, 0x01, 0x00, mov_sens, 0x00, 0x00, 0x00, 0x02, 0x00, stat_sens, 0x00, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
      uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x64, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

      size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
      size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

      memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
      memcpy(rx_buffer, exp_rx, rx_size);

      uint8_t rslt = ld2410x_set_gate_sensitivity(dist_gate, mov_sens, stat_sens, 0, &dev);
      if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
      TEST_ASSERT_EQUAL(LD2410X_OK, rslt);

      TEST_ASSERT_EQUAL(mov_sens, dev.conf.mov_sensitivity[dist_gate]);
      TEST_ASSERT_EQUAL(stat_sens, dev.conf.stat_sensitivity[dist_gate]);

      TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
      TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
    }
  }
}

/* ERROR TEST */
/* Failure ACK status */
void test_ld2410x_set_gate_sensitivity_single_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  for (uint8_t i = LD2410X_MIN_DIST_GATE; i <= LD2410X_MAX_DIST_GATE; i++) {
    uint8_t dist_gate = i;

    for (uint8_t j = LD2410X_MIN_SENS; j <= LD2410X_MAX_SENS; j += 10) {
      uint8_t mov_sens  = j;
      uint8_t stat_sens = j;

      uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x64, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
      size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

      memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
      memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

      uint8_t rslt = ld2410x_set_gate_sensitivity(dist_gate, mov_sens, stat_sens, 0, &dev);
      if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
      TEST_ASSERT_EQUAL(LD2410X_E_CONFIG_FAIL, rslt);
    }
  }
}

/*
 * Range Gate Sensitivity Configuration Commands
 * configure the motion sensitivity of all distance gates to value from 0 to 100, and the static sensitivity to value from 0 to 100
 */
void test_ld2410x_set_gate_sensitivity_unite()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  for (uint8_t i = LD2410X_MIN_DIST_GATE; i <= LD2410X_MAX_DIST_GATE; i++) {
    uint8_t dist_gate = i;

    for (uint8_t j = LD2410X_MIN_SENS; j <= LD2410X_MAX_SENS; j += 10) {
      uint8_t mov_sens  = j;
      uint8_t stat_sens = j;

      uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x64, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0x01, 0x00, mov_sens, 0x00, 0x00, 0x00, 0x02, 0x00, stat_sens, 0x00, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
      uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x64, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

      size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
      size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

      memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
      memcpy(rx_buffer, exp_rx, rx_size);

      dev.conf.max_mov_dist_gate  = 8;
      dev.conf.max_stat_dist_gate = 8;

      uint8_t rslt = ld2410x_set_gate_sensitivity(dist_gate, mov_sens, stat_sens, 1, &dev);
      if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
      TEST_ASSERT_EQUAL(LD2410X_OK, rslt);

      uint8_t exp_conf_arr[LD2410X_MAX_DIST_GATE + 1] = {j, j, j, j, j, j, j, j, j};

      TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_conf_arr, dev.conf.mov_sensitivity, LD2410X_MAX_DIST_GATE + 1);
      TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_conf_arr, dev.conf.stat_sensitivity, LD2410X_MAX_DIST_GATE + 1);

      TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
      TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
    }
  }
}

void test_ld2410x_set_gate_sensitivity_inv_param()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  for (uint8_t i = LD2410X_MAX_DIST_GATE; i <= UINT8_MAX; i++) {
    uint8_t dist_gate = i;

    for (uint8_t j = LD2410X_MIN_SENS; j <= LD2410X_MAX_SENS; j += 10) {
      uint8_t mov_sens  = j;
      uint8_t stat_sens = j;

      uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x14, 0x00, 0x64, 0x00, 0x00, 0x00, dist_gate, 0x00, 0x00, 0x00, 0x01, 0x00, mov_sens, 0x00, 0x00, 0x00, 0x02, 0x00, stat_sens, 0x00, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
      uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x64, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

      size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
      size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

      memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
      memcpy(rx_buffer, exp_rx, rx_size);

      uint8_t rslt = ld2410x_set_gate_sensitivity(dist_gate, mov_sens, stat_sens, 0, &dev);
      if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
      TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, rslt);
    }
  }
}

/* ERROR TEST */
/* Failure ACK status */
void test_ld2410x_set_gate_sensitivity_unite_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  for (uint8_t i = LD2410X_MIN_DIST_GATE; i <= LD2410X_MAX_DIST_GATE; i++) {
    uint8_t dist_gate = i;

    for (uint8_t j = LD2410X_MIN_SENS; j <= LD2410X_MAX_SENS; j += 10) {
      uint8_t mov_sens  = j;
      uint8_t stat_sens = j;

      uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x64, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
      size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

      memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
      memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

      uint8_t rslt = ld2410x_set_gate_sensitivity(dist_gate, mov_sens, stat_sens, 1, &dev);
      if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
      TEST_ASSERT_EQUAL(LD2410X_E_CONFIG_FAIL, rslt);
    }
  }
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

  RUN_TEST(test_ld2410x_set_gate_sensitivity_single);
  RUN_TEST(test_ld2410x_set_gate_sensitivity_single_fail_ack);
  RUN_TEST(test_ld2410x_set_gate_sensitivity_unite);
  RUN_TEST(test_ld2410x_set_gate_sensitivity_unite_fail_ack);

  return UNITY_END();
}
