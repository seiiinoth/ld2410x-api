#include "unity.h"
#include "ld2410x.h"
#include "common.h"

/* Obtain Bluetooth permissions */
void test_ld2410x_obtain_bt_permission()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;
  dev.intf  = LD2410X_BLUETOOTH_INTF;

  uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x08, 0x00, 0xA8, 0x00, 0x48, 0x69, 0x4c, 0x69, 0x6e, 0x6b, 0x04, 0x03, 0x02, 0x01};
  uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA8, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

  size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
  size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

  memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx, rx_size);

  uint8_t rslt = ld2410x_obtain_bt_permission(LD2410X_BLUETOOTH_DEF_PASSWD, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_OK, rslt);
  TEST_ASSERT_EQUAL(1, dev.bt_perm_obtained);
  TEST_ASSERT_EQUAL_UINT8_ARRAY(LD2410X_BLUETOOTH_DEF_PASSWD, dev.conf.bt_passwd, LD2410X_BLUETOOTH_PASSWD_LEN);

  TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
  TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
}

void test_ld2410x_obtain_bt_permission_inv_intf()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;
  dev.intf  = LD2410X_SERIAL_INTF;

  uint8_t rslt = ld2410x_obtain_bt_permission(LD2410X_BLUETOOTH_DEF_PASSWD, &dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INTF_FAIL, rslt);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(0, dev.bt_perm_obtained);
}

/* ERROR TEST */
/* Failure ACK status */
void test_ld2410x_obtain_bt_permission_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;
  dev.intf  = LD2410X_BLUETOOTH_INTF;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA8, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_obtain_bt_permission("HiLink", &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_CONFIG_FAIL, rslt);
  TEST_ASSERT_EQUAL(0, dev.bt_perm_obtained);
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

  RUN_TEST(test_ld2410x_obtain_bt_permission);
  RUN_TEST(test_ld2410x_obtain_bt_permission_inv_intf);
  RUN_TEST(test_ld2410x_obtain_bt_permission_fail_ack);

  return UNITY_END();
}
