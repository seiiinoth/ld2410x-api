#include "unity.h"
#include "ld2410x.h"
#include "common.h"

/* Enable configuration mode */
void test_ld2410x_set_config_mode_enabled()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  for (uint8_t i = 0; i < UINT8_MAX; i++) {
    uint8_t protocol_version = i;
    uint8_t buffer_size      = i + 40;

    if (buffer_size == 0) continue;

    uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFF, 0x00, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
    uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x08, 0x00, 0xFF, 0x01, 0x00, 0x00, protocol_version, 0x00, buffer_size, 0x00, 0x04, 0x03, 0x02, 0x01};

    size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
    size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);

    uint8_t rslt = ld2410x_set_config_mode(LD2410X_CONFIG_MODE_ENABLED, &dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_OK, rslt);
    TEST_ASSERT_EQUAL(LD2410X_CONFIG_MODE_ENABLED, dev.conf.config_mode);
    TEST_ASSERT_EQUAL_UINT16(LD2410X_MERGE_UINT8(protocol_version, buffer_size), dev.intf_i);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }
}

/* Disable configuration mode */
void test_ld2410x_set_config_mode_disabled()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  dev.conf.config_mode = LD2410X_CONFIG_MODE_ENABLED;

  uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x02, 0x00, 0xFE, 0x00, 0x04, 0x03, 0x02, 0x01};
  uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFE, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

  size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
  size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

  memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx, rx_size);

  uint8_t rslt = ld2410x_set_config_mode(LD2410X_CONFIG_MODE_DISABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_OK, rslt);
  TEST_ASSERT_EQUAL(LD2410X_CONFIG_MODE_DISABLED, dev.conf.config_mode);

  TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
  TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
}

void test_ld2410x_set_config_mode_inv_param()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  const uint8_t conf_mode_initial = LD2410X_CONFIG_MODE_DISABLED;

  dev.conf.config_mode = conf_mode_initial;

  for (uint8_t i = LD2410X_CONFIG_MODE_NUM; i < UINT8_MAX; i++) {
    uint8_t rslt = ld2410x_set_config_mode(i, &dev);
    TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, rslt);

    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);

    TEST_ASSERT_EQUAL(conf_mode_initial, dev.conf.config_mode);
  }
}

/* ERROR TEST */
/* Failure ACK status */
void test_ld2410x_set_config_mode_enable_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFF, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_set_config_mode(LD2410X_CONFIG_MODE_ENABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_CONFIG_FAIL, rslt);
}

void test_ld2410x_set_config_mode_disable_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xFE, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_set_config_mode(LD2410X_CONFIG_MODE_DISABLED, &dev);
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

  RUN_TEST(test_ld2410x_set_config_mode_enabled);
  RUN_TEST(test_ld2410x_set_config_mode_disabled);
  RUN_TEST(test_ld2410x_set_config_mode_inv_param);
  RUN_TEST(test_ld2410x_set_config_mode_enable_fail_ack);
  RUN_TEST(test_ld2410x_set_config_mode_disable_fail_ack);

  return UNITY_END();
}
