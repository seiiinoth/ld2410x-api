#include "unity.h"
#include "ld2410x.h"
#include "common.h"

/* Invalid parameter passed */
void test_ld2410x_invalid_param()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;
  dev.intf  = LD2410X_BLUETOOTH_INTF;

  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, ld2410x_set_baudrate(11, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, ld2410x_obtain_bt_permission(NULL, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, ld2410x_obtain_bt_permission("Hi!", &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, ld2410x_set_bt_password(NULL, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, ld2410x_set_bt_password("Hi!", &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, ld2410x_set_dist_res(11, &dev));
  TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, ld2410x_set_dist_res(-11, &dev));
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

  RUN_TEST(test_ld2410x_invalid_param);

  return UNITY_END();
}
