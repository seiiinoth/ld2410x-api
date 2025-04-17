/**
 * @file test_ld2410x_struct_init.c
 * @brief Test assigning defined values ​​to library structures
 *
 * Test is designed to check that there are no errors
 * when assigning default values ​​to library structures.
 * The tests themselves are meaningless and exist only so that the compiler
 * does not optimize the process and remove variables that are not used
 */

#include "unity.h"
#include "ld2410x.h"
#include "common.h"

#include <memory.h>

void test_ld2410x_dev_default()
{
  uint8_t struct_mem[sizeof(struct ld2410x_dev)] = {0};
  memset(struct_mem, 0, sizeof(struct_mem));

  struct ld2410x_dev *dev_default    = (struct ld2410x_dev *)(struct_mem);
  struct ld2410x_dev dev_default_cmp = ld2410x_dev_init_default;

  *dev_default = ld2410x_dev_init_default;

  TEST_ASSERT_EQUAL_MEMORY(dev_default, &dev_default_cmp, sizeof(struct ld2410x_dev));
}

void test_ld2410x_dev_zero()
{
  uint8_t struct_mem[sizeof(struct ld2410x_dev)] = {0};
  memset(struct_mem, 0, sizeof(struct_mem));

  struct ld2410x_dev *dev_zero = (struct ld2410x_dev *)(struct_mem);

  *dev_zero = ld2410x_dev_init_zero;

  TEST_ASSERT_EQUAL_MEMORY(dev_zero, struct_mem, sizeof(struct ld2410x_dev));
}

void test_ld2410x_conf_default()
{
  uint8_t struct_mem[sizeof(struct ld2410x_conf)] = {0};
  memset(struct_mem, 0, sizeof(struct_mem));

  struct ld2410x_conf *conf_default = (struct ld2410x_conf *)(struct_mem);

  *conf_default = ld2410x_conf_init_default;

  TEST_ASSERT_EQUAL_MEMORY(conf_default, struct_mem, sizeof(struct ld2410x_conf));
}

void test_ld2410x_conf_zero()
{
  uint8_t struct_mem[sizeof(struct ld2410x_conf)] = {0};
  memset(struct_mem, 0, sizeof(struct_mem));

  struct ld2410x_conf *conf_zero = (struct ld2410x_conf *)(struct_mem);

  *conf_zero = ld2410x_conf_init_zero;

  TEST_ASSERT_EQUAL_MEMORY(conf_zero, struct_mem, sizeof(struct ld2410x_conf));
}

void test_ld2410x_data_zero()
{
  uint8_t struct_mem[sizeof(struct ld2410x_data)] = {0};
  memset(struct_mem, 0, sizeof(struct_mem));

  struct ld2410x_data *data_zero = (struct ld2410x_data *)(struct_mem);

  *data_zero = ld2410x_data_init_zero;

  TEST_ASSERT_EQUAL_MEMORY(data_zero, struct_mem, sizeof(struct ld2410x_data));
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

  RUN_TEST(test_ld2410x_dev_default);
  RUN_TEST(test_ld2410x_dev_zero);
  RUN_TEST(test_ld2410x_conf_default);
  RUN_TEST(test_ld2410x_conf_zero);
  RUN_TEST(test_ld2410x_data_zero);

  return UNITY_END();
}
