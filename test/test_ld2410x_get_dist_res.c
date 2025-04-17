#include "unity.h"
#include "ld2410x.h"
#include "common.h"

/* Query the module's current distance resolution setting, i.e. how far away each distance gate represents. */
void test_ld2410x_get_dist_res()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  for (uint8_t i = 0; i < LD2410X_DIST_RES_NUM; i++) {
    ld2410x_dist_res_t dist_res = (ld2410x_dist_res_t)i;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xAB, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x06, 0x00, 0xAB, 0x01, 0x00, 0x00, dist_res, 0x00, 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_get_dist_res(&dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_OK, rslt);
    TEST_ASSERT_EQUAL(dist_res, dev.conf.gate_dist_resolution);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }
}

void test_ld2410x_get_dist_res_inv_rx_param()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  const uint8_t gate_dist_resolution_initial = 0;

  dev.conf.gate_dist_resolution = gate_dist_resolution_initial;

  for (uint16_t i = LD2410X_DIST_RES_NUM; i < UINT16_MAX / 2; i += 100) {
    uint16_t dist_res = i;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xAB, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x06, 0x00, 0xAB, 0x01, 0x00, 0x00, (uint8_t)(dist_res), (uint8_t)(dist_res >> 8), 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_get_dist_res(&dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_E_SELF_TEST, rslt);
    TEST_ASSERT_EQUAL(gate_dist_resolution_initial, dev.conf.gate_dist_resolution);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }

  for (uint16_t i = UINT16_MAX / 2; i < UINT16_MAX / 2; i += 100) {
    uint16_t dist_res = i;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xAB, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x06, 0x00, 0xAB, 0x01, 0x00, 0x00, (uint8_t)(dist_res), (uint8_t)(dist_res >> 8), 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_get_dist_res(&dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_E_SELF_TEST, rslt);
    TEST_ASSERT_EQUAL(gate_dist_resolution_initial, dev.conf.gate_dist_resolution);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }
}

/* ERROR TEST */
/* Failure ACK status */
void test_ld2410x_get_dist_res_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_rx_fail1[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xAB, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size    = sizeof(exp_rx_fail1) / sizeof(exp_rx_fail1[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail1, rx_fail_size);

  uint8_t rslt = ld2410x_get_dist_res(&dev);
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

  RUN_TEST(test_ld2410x_get_dist_res);
  RUN_TEST(test_ld2410x_get_dist_res_inv_rx_param);
  RUN_TEST(test_ld2410x_get_dist_res_fail_ack);

  return UNITY_END();
}
