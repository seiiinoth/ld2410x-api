#include "unity.h"
#include "ld2410x.h"
#include "common.h"

/* Enable engineering mode */
void test_ld2410x_set_engineering_mode_enable()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x62, 0x00, 0x04, 0x03, 0x02, 0x01};
  uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x62, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

  size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
  size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

  memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx, rx_size);

  uint8_t rslt = ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_ENABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_OK, rslt);
  TEST_ASSERT_EQUAL(LD2410X_ENGINEERING_MODE_ENABLED, dev.conf.engineering_mode);

  TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
  TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
}

/* Disable engineering mode */
void test_ld2410x_set_engineering_mode_disable()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0x63, 0x00, 0x04, 0x03, 0x02, 0x01};
  uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x63, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

  size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
  size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

  memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx, rx_size);

  uint8_t rslt = ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_DISABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_OK, rslt);
  TEST_ASSERT_EQUAL(LD2410X_ENGINEERING_MODE_DISABLED, dev.conf.engineering_mode);

  TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
  TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
}

void test_ld2410x_set_engineering_mode_inv_param()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  const uint8_t engineering_mode_initial = LD2410X_ENGINEERING_MODE_DISABLED;

  dev.conf.engineering_mode = engineering_mode_initial;

  for (uint8_t i = LD2410X_ENGINEERING_MODE_NUM; i < UINT8_MAX; i++) {
    uint8_t rslt = ld2410x_set_engineering_mode(i, &dev);
    TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, rslt);

    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);

    TEST_ASSERT_EQUAL(engineering_mode_initial, dev.conf.engineering_mode);
  }
}

/* ERROR TEST */
/* Failure ACK status */
void test_ld2410x_set_engineering_mode_enable_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x62, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_ENABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_CONFIG_FAIL, rslt);
}

void test_ld2410x_set_engineering_mode_disable_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0x63, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_DISABLED, &dev);
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

  RUN_TEST(test_ld2410x_set_engineering_mode_enable);
  RUN_TEST(test_ld2410x_set_engineering_mode_disable);
  RUN_TEST(test_ld2410x_set_engineering_mode_inv_param);
  RUN_TEST(test_ld2410x_set_engineering_mode_enable_fail_ack);
  RUN_TEST(test_ld2410x_set_engineering_mode_disable_fail_ack);

  return UNITY_END();
}
