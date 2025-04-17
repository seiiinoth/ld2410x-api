#include "unity.h"
#include "ld2410x.h"
#include "common.h"

/* Invalid length of received package */
void test_ld2410x_invalid_length()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t rslt;
  uint8_t inv_rx[]   = {0xFD, 0xFC, 0xFB, 0xFA, 0x55, 0x00, 0x63, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t inv_rx_size = sizeof(inv_rx) / sizeof(inv_rx[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, inv_rx, inv_rx_size);

  rslt = ld2410x_set_config_mode(LD2410X_CONFIG_MODE_DISABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_config_mode(LD2410X_CONFIG_MODE_ENABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_DISABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_ENABLED, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_dist_dur(8, 8, 5, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_get_config_data(&dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_gate_sensitivity(3, 40, 40, 0, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_get_fw_ver(&dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_baudrate(LD2410X_SERIAL_BDRT_256000, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_restore_configuration(&dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_restart(&dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_bluetooth_mode(1, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_get_mac_addr(&dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  dev.intf = LD2410X_BLUETOOTH_INTF;
  rslt     = ld2410x_obtain_bt_permission("HiLink", &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  dev.bt_perm_obtained = 1;

  dev.intf = LD2410X_SERIAL_INTF;
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_bt_password("HiLiee", &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_set_dist_res(LD2410X_DIST_RES_075, &dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);

  rslt = ld2410x_get_dist_res(&dev);
  if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_LENGTH, rslt);
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

  RUN_TEST(test_ld2410x_invalid_length);

  return UNITY_END();
}
