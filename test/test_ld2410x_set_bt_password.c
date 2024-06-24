#include "unity.h"
#include "ld2410x.h"
#include "common.h"

#include <time.h>

char get_char(int index)
{
  char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  return charset[index];
}

void generate_string(char *string, uint8_t len)
{
  for (uint8_t i = 0; i < len; i++) {
    uint8_t index = rand() % 62;
    string[i]     = get_char(index);
  }
  string[len] = '\0';
}

/* Set Bluetooth password */
void test_ld2410x_set_bt_password()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_tx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x08, 0x00, 0xA9, 0x00, 0x48, 0x69, 0x4c, 0x69, 0x6e, 0x65, 0x04, 0x03, 0x02, 0x01};
  uint8_t exp_rx[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA9, 0x01, 0x00, 0x00, 0x04, 0x03, 0x02, 0x01};

  size_t tx_size = sizeof(exp_tx) / sizeof(exp_tx[0]);
  size_t rx_size = sizeof(exp_rx) / sizeof(exp_rx[0]);

  srand(time(NULL));

  char bt_password[LD2410X_BLUETOOTH_PASSWD_LEN + 1];

  for (uint8_t i = 0; i < UINT8_MAX; i++) {
    generate_string(bt_password, LD2410X_BLUETOOTH_PASSWD_LEN);

    memset(tx_buffer, 0, LD2410X_MAX_FRAME_LEN);
    memcpy(rx_buffer, exp_rx, rx_size);
    memcpy(exp_tx + 8, bt_password, LD2410X_BLUETOOTH_PASSWD_LEN);

    uint8_t rslt = ld2410x_set_bt_password(bt_password, &dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_OK, rslt);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(bt_password, dev.conf.bt_passwd, LD2410X_BLUETOOTH_PASSWD_LEN);

    TEST_ASSERT_EQUAL_UINT8_ARRAY(exp_tx, tx_buffer, tx_size);
    TEST_ASSERT_EQUAL_MEMORY(exp_tx, tx_buffer, tx_size);
  }
}

/* Pass password sting with invalid length */
void test_ld2410x_set_bt_password_inv_length()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  srand(time(NULL));

  char bt_password[UINT8_MAX];

  for (uint8_t i = 0; i < UINT8_MAX; i++) {
    if (i == LD2410X_BLUETOOTH_PASSWD_LEN) continue;
    generate_string(bt_password, i);

    uint8_t rslt = ld2410x_set_bt_password(bt_password, &dev);
    if (dev.intf_mode == LD2410X_CALLBACK_INTF) rslt = ld2410x_intf_callback(&dev);
    TEST_ASSERT_EQUAL(LD2410X_E_INVALID_PARAM, rslt);
  }
}

/* ERROR TEST */
/* Failure ACK status */
void test_ld2410x_set_bt_password_fail_ack()
{
  struct ld2410x_dev dev = ld2410x_dev_init_zero;

  dev.read  = ld2410x_read;
  dev.write = ld2410x_write;

  uint8_t exp_rx_fail[] = {0xFD, 0xFC, 0xFB, 0xFA, 0x04, 0x00, 0xA9, 0x01, 0x01, 0x00, 0x04, 0x03, 0x02, 0x01};
  size_t rx_fail_size   = sizeof(exp_rx_fail) / sizeof(exp_rx_fail[0]);

  memset(rx_buffer, 0, LD2410X_MAX_FRAME_LEN);
  memcpy(rx_buffer, exp_rx_fail, rx_fail_size);

  uint8_t rslt = ld2410x_set_bt_password("HiLink", &dev);
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

  RUN_TEST(test_ld2410x_set_bt_password);
  RUN_TEST(test_ld2410x_set_bt_password_inv_length);
  RUN_TEST(test_ld2410x_set_bt_password_fail_ack);

  return UNITY_END();
}
