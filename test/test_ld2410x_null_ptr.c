#include "unity.h"
#include "ld2410x.h"
#include "common.h"

/* Null pointer device test */
void test_ld2410x_null_ptr()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.write = NULL;
  dev.read  = NULL;
  dev.intf  = LD2410X_BLUETOOTH_INTF;

  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_config_mode(LD2410X_CONFIG_MODE_DISABLED, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_config_mode(LD2410X_CONFIG_MODE_ENABLED, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_DISABLED, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_engineering_mode(LD2410X_ENGINEERING_MODE_ENABLED, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_dist_dur(8, 8, 5, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_get_config_data(&dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_gate_sensitivity(3, 40, 40, 0, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_get_fw_ver(&dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_baudrate(LD2410X_SERIAL_BDRT_256000, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_restore_configuration(&dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_restart(&dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_bluetooth_mode(1, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_get_mac_addr(&dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_obtain_bt_permission("HiLink", &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_bt_password("HiLink", &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_set_dist_res(LD2410X_DIST_RES_075, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_NULL_PTR, ld2410x_get_dist_res(&dev));
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

  RUN_TEST(test_ld2410x_null_ptr);

  return UNITY_END();
}
