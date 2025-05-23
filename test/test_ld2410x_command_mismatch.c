#include "unity.h"
#include "ld2410x.h"
#include "common.h"

#include <memory.h>

void test(struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = ld2410x_set_config_mode(LD2410X_CONFIG_MODE_DISABLED, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_config_mode(LD2410X_CONFIG_MODE_ENABLED, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_DISABLED, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_ENABLED, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_dist_dur(8, 8, 5, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_get_config_data(dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_gate_sensitivity(3, 40, 40, 0, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_get_fw_ver(dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_baudrate(LD2410X_SERIAL_BDRT_256000, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_restore_configuration(dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_restart(dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_bluetooth_mode(1, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_get_mac_addr(dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  dev->intf = LD2410X_BLUETOOTH_INTF;
  rslt      = ld2410x_obtain_bt_permission("HiLink", dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  dev->bt_perm_obtained = 1;

  dev->intf = LD2410X_SERIAL_INTF;

  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_bt_password("HiLiee", dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_set_dist_res(LD2410X_DIST_RES_075, dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);

  rslt = ld2410x_get_dist_res(dev);
  if (dev->intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(dev);
  TEST_ASSERT_EQUAL(LD2410X_E_COMMAND_MISMATCH, rslt);
}

/* Command mismatch */
void test_ld2410x_command_mismatch_intf_callback()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read      = ld2410x_read;
  dev.write     = ld2410x_write;
  dev.intf_mode = LD2410X_CALLBACK_INTF;

  uint8_t inv_comm   = 0x02;
  uint8_t inv_rx[]   = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, inv_comm, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t inv_rx_size = sizeof(inv_rx) / sizeof(inv_rx[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, inv_rx, inv_rx_size);

  test(&dev);
}

void test_ld2410x_command_mismatch_intf_continuous()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read      = ld2410x_read;
  dev.write     = ld2410x_write;
  dev.intf_mode = LD2410X_CONTINUOUS_INTF;

  uint8_t inv_comm   = 0x02;
  uint8_t inv_rx[]   = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, inv_comm, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t inv_rx_size = sizeof(inv_rx) / sizeof(inv_rx[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, inv_rx, inv_rx_size);

  test(&dev);
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

  RUN_TEST(test_ld2410x_command_mismatch_intf_callback);
  RUN_TEST(test_ld2410x_command_mismatch_intf_continuous);

  return UNITY_END();
}
