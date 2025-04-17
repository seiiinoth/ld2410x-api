#include "unity.h"
#include "ld2410x.h"
#include "common.h"

void test_ld2410x_intf_busy()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read      = ld2410x_read;
  dev.write     = ld2410x_write;
  dev.intf_mode = LD2410X_CALLBACK_INTF;

  (void)ld2410x_restart(&dev);

  TEST_ASSERT_EQUAL(LD2410X_E_DEV_BUSY, ld2410x_restart(&dev));
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

  RUN_TEST(test_ld2410x_intf_busy);

  return UNITY_END();
}
