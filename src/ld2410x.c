#include "ld2410x.h"
#include <string.h>

/* Internal method used to send command to sensor */
static uint8_t send_cmd(uint8_t cmd_word, const uint8_t *cmd_val, uint8_t val_len, struct ld2410x_dev *dev);

/* Internal method used to receive response from sensor */
static uint8_t receive_frame(uint8_t frame_len, struct ld2410x_dev *dev);

/* Internal method used for checking sensor respond for frame format and ACK status for success */
static uint8_t check_frame(uint8_t cmd_word, uint8_t *buffer, uint8_t frame_len, uint8_t frame_corr);

/* Internal method used for verify response from sensor */
static inline uint8_t read_frame(struct ld2410x_dev *dev);

/* Internal method used for check the ld2410x_dev struct for null pointers */
static inline uint8_t null_ptr_check(const struct ld2410x_dev *dev);

/* Internal method used for limit the max and min value of a parameter */
static inline uint8_t boundary_check(uint8_t *value, uint8_t max, uint8_t min);

/* Internal method used for extract return data from sensor ACK */
static inline uint8_t extract_return_data(const uint8_t buff_offset, const uint8_t *buffer, uint8_t *data_buffer);

/* Internal method used for compose whole frame with internal data */
static inline uint8_t compose_frame_buff(uint8_t *buffer, const uint8_t *data, uint8_t len);

/* Internal method used for compose data inside intra-frame buffer */
static inline uint8_t compose_frame_data(uint8_t *buffer, uint8_t command, const uint8_t *value, uint8_t len);

/* Internal method used for transfer temporary configuration changes to device structure */
static inline void config_approve(struct ld2410x_dev *dev);

/* Internal method used for sort data from sensor  */
static uint8_t sort_sensor_data(const uint8_t *data, struct ld2410x_dev *dev);

inline ld2410x_rsp_status_t ld2410x_rsp_status(const struct ld2410x_dev *dev)
{
  return dev->comm.rsp_status;
}

inline void ld2410x_intf_reset(struct ld2410x_dev *dev)
{
  dev->intf_status = LD2410X_INTF_READY;
}

uint8_t ld2410x_set_config_mode(const ld2410x_config_mode_t config_mode, struct ld2410x_dev *dev)
{
  uint8_t rslt, ret_frame_len;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    dev->comm.t_conf = dev->conf;

    dev->comm.wait_approve = LD2410X_CONFIG_WAIT_APPROVE;

    switch (config_mode) {
      case LD2410X_CONFIG_MODE_DISABLED:

        dev->comm.t_conf.config_mode = LD2410X_CONFIG_MODE_DISABLED;

        rslt = send_cmd(LD2410X_CMD_END_CONFIG, NULL, 0, dev);

        ret_frame_len = 14;

        break;

      case LD2410X_CONFIG_MODE_ENABLED:

      {
        const uint8_t value[2] = {0x01, 0x00};

        dev->comm.t_conf.config_mode = LD2410X_CONFIG_MODE_ENABLED;

        rslt = send_cmd(LD2410X_CMD_EN_CONFIG, value, sizeof(value), dev);

        ret_frame_len = 18;
      }

      break;

      default:
        rslt = LD2410X_E_INVALID_PARAM;
        break;
    }

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(ret_frame_len, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_set_engineering_mode(const ld2410x_engineering_mode_t engineering_mode, struct ld2410x_dev *dev)
{
  uint8_t rslt, ret_frame_len;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    dev->comm.t_conf = dev->conf;

    dev->comm.wait_approve = LD2410X_CONFIG_WAIT_APPROVE;

    switch (engineering_mode) {
      case LD2410X_ENGINEERING_MODE_DISABLED:

        dev->comm.t_conf.engineering_mode = LD2410X_ENGINEERING_MODE_DISABLED;

        rslt = send_cmd(LD2410X_CMD_CL_ENG, NULL, 0, dev);

        ret_frame_len = 14;

        break;

      case LD2410X_ENGINEERING_MODE_ENABLED:

        dev->comm.t_conf.engineering_mode = LD2410X_ENGINEERING_MODE_ENABLED;

        rslt = send_cmd(LD2410X_CMD_EN_ENG, NULL, 0, dev);

        ret_frame_len = 14;

        break;

      default:
        rslt = LD2410X_E_INVALID_PARAM;
        break;
    }

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(ret_frame_len, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_set_dist_dur(uint8_t mov_dist, uint8_t stat_dist, const uint16_t u_dur, struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  boundary_check(&mov_dist, LD2410X_MAX_DIST_GATE, LD2410X_MIN_DIST_GATE);
  boundary_check(&stat_dist, LD2410X_MAX_DIST_GATE, LD2410X_MIN_DIST_GATE);

  if (rslt == LD2410X_OK) {
    uint8_t value[18] = {0x00, 0x00, mov_dist, 0x00, 0x00, 0x00, 0x01, 0x00, stat_dist, 0x00, 0x00, 0x00, 0x02, 0x00, (uint8_t)(u_dur), (uint8_t)(u_dur >> 8), 0x00, 0x00};

    memcpy(&dev->comm.t_conf, &dev->conf, sizeof(struct ld2410x_conf));

    dev->comm.wait_approve              = LD2410X_CONFIG_WAIT_APPROVE;
    dev->comm.t_conf.max_mov_dist_gate  = mov_dist;
    dev->comm.t_conf.max_stat_dist_gate = stat_dist;
    dev->comm.t_conf.unmanned_duration  = u_dur;

    rslt = send_cmd(LD2410X_CMD_SET_DIST_DUR, value, 18, dev);

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(14, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_get_config_data(struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    rslt = send_cmd(LD2410X_CMD_READ_CONFIG, NULL, 0, dev);

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(38, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_set_gate_sensitivity(uint8_t dist_gate, uint8_t mov_sens, uint8_t stat_sens, const uint8_t unite, struct ld2410x_dev *dev)
{
  uint8_t rslt;

  if (dist_gate <= LD2410X_MAX_DIST_GATE || unite == 1) {
    rslt = null_ptr_check(dev);

    boundary_check(&mov_sens, LD2410X_MAX_SENS, LD2410X_MIN_SENS);
    boundary_check(&stat_sens, LD2410X_MAX_SENS, LD2410X_MIN_SENS);

    if (rslt == LD2410X_OK) {
      uint8_t value[18] = {0x00, 0x00, dist_gate, 0x00, 0x00, 0x00, 0x01, 0x00, mov_sens, 0x00, 0x00, 0x00, 0x02, 0x00, stat_sens, 0x00, 0x00, 0x00};

      memcpy(&dev->comm.t_conf, &dev->conf, sizeof(dev->conf));

      dev->comm.wait_approve = LD2410X_CONFIG_WAIT_APPROVE;

      if (unite) {
        uint8_t temp[2] = {0xFF, 0xFF};

        memcpy(&value[2], &temp, sizeof(temp));

        boundary_check(&dev->conf.max_mov_dist_gate, LD2410X_MAX_DIST_GATE, LD2410X_MIN_DIST_GATE);
        boundary_check(&dev->conf.max_stat_dist_gate, LD2410X_MAX_DIST_GATE, LD2410X_MIN_DIST_GATE);

        memset(dev->comm.t_conf.mov_sensitivity, mov_sens, dev->conf.max_mov_dist_gate + 1);
        memset(dev->comm.t_conf.stat_sensitivity, stat_sens, dev->conf.max_stat_dist_gate + 1);
      } else {
        dev->comm.t_conf.mov_sensitivity[dist_gate]  = mov_sens;
        dev->comm.t_conf.stat_sensitivity[dist_gate] = stat_sens;
      }

      rslt = send_cmd(LD2410X_CMD_GATE_SENS_CONF, value, 18, dev);

      if (rslt == LD2410X_OK) {
        rslt = receive_frame(14, dev);
      }
    }
  } else {
    rslt = LD2410X_E_INVALID_PARAM;
  }

  return rslt;
}

uint8_t ld2410x_get_fw_ver(struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    rslt = send_cmd(LD2410X_CMD_READ_FW, NULL, 0, dev);

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(22, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_set_baudrate(const ld2410x_serial_bdrt_t serial_bdrt, struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    switch (serial_bdrt) {
      case LD2410X_SERIAL_BDRT_9600:
      case LD2410X_SERIAL_BDRT_19200:
      case LD2410X_SERIAL_BDRT_38400:
      case LD2410X_SERIAL_BDRT_57600:
      case LD2410X_SERIAL_BDRT_115200:
      case LD2410X_SERIAL_BDRT_230400:
      case LD2410X_SERIAL_BDRT_256000:
      case LD2410X_SERIAL_BDRT_460800:

      {
        uint8_t value[2] = {serial_bdrt, 0x00};

        memcpy(&dev->comm.t_conf, &dev->conf, sizeof(dev->comm.t_conf));

        dev->comm.wait_approve     = LD2410X_CONFIG_WAIT_APPROVE;
        dev->comm.t_conf.baud_rate = serial_bdrt;

        rslt = send_cmd(LD2410X_CMD_SET_BAUDRATE, value, sizeof(value), dev);

        if (rslt == LD2410X_OK) {
          rslt = receive_frame(14, dev);
        }
      }

      break;

      default:

        rslt = LD2410X_E_INVALID_PARAM;
        break;
    }
  }

  return rslt;
}

uint8_t ld2410x_restore_configuration(struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    rslt = send_cmd(LD2410X_CMD_RESET, NULL, 0, dev);

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(14, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_restart(struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    memcpy(&dev->comm.t_conf, &dev->conf, sizeof(dev->conf));

    dev->comm.wait_approve = LD2410X_CONFIG_WAIT_APPROVE;

    // Engineering mode disabled automatically after reset
    dev->comm.t_conf.engineering_mode = LD2410X_ENGINEERING_MODE_DISABLED;

    rslt = send_cmd(LD2410X_CMD_RESTART, NULL, 0, dev);

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(14, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_set_bluetooth_mode(const ld2410x_bluetooth_mode_t bt_mode, struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    switch (bt_mode) {
      case LD2410X_BLUETOOTH_MODE_DISABLED:
      case LD2410X_BLUETOOTH_MODE_ENABLED:

      {
        const uint8_t value[2] = {bt_mode, 0x00};

        memcpy(&dev->comm.t_conf, &dev->conf, sizeof(dev->comm.t_conf));

        dev->comm.wait_approve   = LD2410X_CONFIG_WAIT_APPROVE;
        dev->comm.t_conf.bt_mode = bt_mode;

        rslt = send_cmd(LD2410X_CMD_BLUETOOTH_MODE, value, 2, dev);

        if (rslt == LD2410X_OK) {
          rslt = receive_frame(14, dev);
        }
      }

      break;

      default:
        rslt = LD2410X_E_INVALID_PARAM;
        break;
    }
  }

  return rslt;
}

uint8_t ld2410x_get_mac_addr(struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    const uint8_t value[2] = {0x01, 0x00};

    rslt = send_cmd(LD2410X_CMD_READ_MAC, value, sizeof(value), dev);

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(20, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_obtain_bt_permission(const char *bt_passwd, struct ld2410x_dev *dev)
{
  uint8_t rslt;

  if (dev->intf == LD2410X_BLUETOOTH_INTF) {
    rslt = null_ptr_check(dev);

    if (rslt == LD2410X_OK) {
      if (bt_passwd != NULL && strlen(bt_passwd) == LD2410X_BLUETOOTH_PASSWD_LEN) {
        dev->bt_perm_obtained  = 0;
        dev->comm.wait_approve = LD2410X_CONFIG_WAIT_APPROVE;

        memcpy(&dev->comm.t_conf, &dev->conf, sizeof(dev->comm.t_conf));
        memcpy(&dev->comm.t_conf.bt_passwd, bt_passwd, sizeof(dev->comm.t_conf.bt_passwd));

        rslt = send_cmd(LD2410X_CMD_BLUETOOTH_PERM, (uint8_t *)bt_passwd, LD2410X_BLUETOOTH_PASSWD_LEN, dev);

        if (rslt == LD2410X_OK) {
          rslt = receive_frame(14, dev);
        }
      } else {
        rslt = LD2410X_E_INVALID_PARAM;
      }
    }
  } else {
    rslt = LD2410X_E_INTF_FAIL;
  }

  return rslt;
}

uint8_t ld2410x_set_bt_password(const char *bt_passwd, struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    if (bt_passwd != NULL && strlen(bt_passwd) == LD2410X_BLUETOOTH_PASSWD_LEN) {
      dev->comm.wait_approve = LD2410X_CONFIG_WAIT_APPROVE;

      memcpy(&dev->comm.t_conf, &dev->conf, sizeof(dev->comm.t_conf));
      memcpy(&dev->comm.t_conf.bt_passwd, bt_passwd, sizeof(dev->comm.t_conf.bt_passwd));

      rslt = send_cmd(LD2410X_CMD_SET_BLUETOOTH_PASSWD, (uint8_t *)bt_passwd, LD2410X_BLUETOOTH_PASSWD_LEN, dev);

      if (rslt == LD2410X_OK) {
        rslt = receive_frame(14, dev);
      }
    } else {
      rslt = LD2410X_E_INVALID_PARAM;
    }
  }

  return rslt;
}

uint8_t ld2410x_set_dist_res(const ld2410x_dist_res_t res_indx, struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    switch (res_indx) {
      case LD2410X_DIST_RES_075:
      case LD2410X_DIST_RES_020:

      {
        uint8_t value[2] = {res_indx, 0x00};

        memcpy(&dev->comm.t_conf, &dev->conf, sizeof(dev->comm.t_conf));

        dev->comm.wait_approve = LD2410X_CONFIG_WAIT_APPROVE;

        dev->comm.t_conf.gate_dist_resolution = res_indx;

        rslt = send_cmd(LD2410X_CMD_SET_DIST_GATE_RES, value, sizeof(value), dev);

        if (rslt == LD2410X_OK) {
          rslt = receive_frame(14, dev);
        }
      }

      break;

      default:
        rslt = LD2410X_E_INVALID_PARAM;
        break;
    }
  }

  return rslt;
}

uint8_t ld2410x_get_dist_res(struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (rslt == LD2410X_OK) {
    rslt = send_cmd(LD2410X_CMD_GET_DIST_GATE_RES, NULL, 0, dev);

    if (rslt == LD2410X_OK) {
      rslt = receive_frame(16, dev);
    }
  }

  return rslt;
}

uint8_t ld2410x_get_target_data(struct ld2410x_data *data, struct ld2410x_dev *dev)
{
  uint8_t rslt;

  rslt = null_ptr_check(dev);

  if (data == NULL) rslt = LD2410X_E_NULL_PTR;

  if (rslt == LD2410X_OK) {
    if (dev->intf_status == LD2410X_INTF_READY) {
      uint8_t frame_len = 0;

      dev->intf_status = LD2410X_INTF_BUSY;
      dev->comm.t_data = data;

      switch (dev->conf.engineering_mode) {
        case LD2410X_ENGINEERING_MODE_DISABLED:
          frame_len = 23;
          break;

        case LD2410X_ENGINEERING_MODE_ENABLED:
          frame_len = 45;
          break;

        default:
          break;
      }

      rslt = receive_frame(frame_len, dev);
    } else {
      rslt = LD2410X_E_DEV_BUSY;
    }
  }

  return rslt;
}

static uint8_t send_cmd(uint8_t cmd_word, const uint8_t *cmd_val, uint8_t val_len, struct ld2410x_dev *dev)
{
  uint8_t rslt = LD2410X_OK;
  uint8_t buff_len;
  uint8_t intra_frame_buffer[LD2410X_MAX_LEN_INTRA_BUFFER] = {0};
  uint8_t frame_buffer[LD2410X_MAX_FRAME_LEN]              = {0};

  dev->comm.rsp_status = LD2410X_DEV_RSP_STATUS_UNKNOWN;

  buff_len = compose_frame_data(intra_frame_buffer, cmd_word, cmd_val, val_len);
  buff_len = compose_frame_buff(frame_buffer, intra_frame_buffer, buff_len);

  if (dev->intf == LD2410X_SERIAL_INTF || dev->bt_perm_obtained || cmd_word == LD2410X_CMD_BLUETOOTH_PERM) {
    if (dev->intf_status == LD2410X_INTF_READY) {
      dev->intf_status = LD2410X_INTF_BUSY;

      dev->intf_rslt = dev->write(frame_buffer, buff_len, dev->intf_ptr);

      if (dev->intf_rslt == LD2410X_INTF_RET_SUCCESS) {
        dev->comm.cmd_word = cmd_word;
      } else {
        rslt = LD2410X_E_COM_FAIL;
      }
    } else {
      rslt = LD2410X_E_DEV_BUSY;
    }
  } else {
    rslt = LD2410X_E_INTF_FAIL;
  }

  return rslt;
}

uint8_t ld2410x_intf_callback(struct ld2410x_dev *dev)
{
  uint8_t rslt = LD2410X_OK;

  rslt = null_ptr_check(dev);

  if (dev->intf_mode == LD2410X_CALLBACK_INTF) {
    rslt = read_frame(dev);

    dev->comm.frame_len    = 0;
    dev->comm.t_data       = NULL;
    dev->comm.wait_approve = LD2410X_CONFIG_APPROVED;
    dev->comm.cmd_word     = LD2410X_NO_CMD;
  }

  return rslt;
}

static uint8_t receive_frame(uint8_t frame_len, struct ld2410x_dev *dev)
{
  uint8_t rslt = LD2410X_OK;

  dev->comm.frame_len = frame_len;

  if (dev->frame_corr) {
    frame_len += LD2410X_FRAME_CORR_SPACE;
  }

  dev->intf_rslt = dev->read(dev->comm.rx_buffer, frame_len, dev->intf_ptr);

  if (dev->intf_rslt == LD2410X_INTF_RET_SUCCESS) {
    if (dev->intf_mode == LD2410X_CONTINUOUS_INTF) {
      rslt = read_frame(dev);
    }
  } else {
    rslt = LD2410X_E_COM_FAIL;
  }

  if (dev->intf_mode == LD2410X_CONTINUOUS_INTF) {
    dev->comm.frame_len    = 0;
    dev->comm.t_data       = NULL;
    dev->comm.wait_approve = LD2410X_CONFIG_APPROVED;
    dev->comm.cmd_word     = LD2410X_NO_CMD;
  }

  return rslt;
}

static uint8_t check_frame(uint8_t cmd_word, uint8_t *buffer, uint8_t frame_len, uint8_t frame_corr)
{
  uint8_t rslt      = LD2410X_OK;
  int8_t end_f_indx = -1, hdr_f_indx = -1;
  const uint8_t frame_hdr_conf[] = {0xFD, 0xFC, 0xFB, 0xFA};
  const uint8_t frame_end_conf[] = {0x04, 0x03, 0x02, 0x01};
  const uint8_t frame_hdr_targ[] = {0xF4, 0xF3, 0xF2, 0xF1};
  const uint8_t frame_end_targ[] = {0xF8, 0xF7, 0xF6, 0xF5};
  const uint8_t *frame_hdr_ptr = frame_hdr_targ, *frame_end_ptr = frame_end_targ;

  if (cmd_word) {
    frame_hdr_ptr = frame_hdr_conf;
    frame_end_ptr = frame_end_conf;
  }

  if (frame_corr) {
    for (uint8_t i = 0; i < (frame_len - 10); i++) {
      if (memcmp(buffer + i, frame_hdr_ptr, 4) == 0) {
        hdr_f_indx = i;
        if (i != 0) {
          uint8_t temp[LD2410X_MAX_FRAME_LEN] = {0};
          memcpy(temp, buffer + i, frame_len);
          memcpy(buffer, temp, frame_len);
        }
        break;
      }
    }
  } else {
    if (memcmp(buffer, frame_hdr_ptr, 4) == 0) {
      hdr_f_indx = 0;
    }
  }

  for (uint8_t i = (frame_len - 4); i > 4; i--) {
    if (memcmp(buffer + i, frame_end_ptr, 4) == 0) {
      end_f_indx = i + 4;
      break;
    }
  }

  if (hdr_f_indx != -1 && end_f_indx != -1) {
    uint8_t value_len = buffer[4];

    if (value_len == end_f_indx - 10) {
      if (cmd_word) {
        /* Check command word response */
        if ((uint16_t)((buffer[7] << 8) | buffer[6]) == (uint16_t)(cmd_word | 0x0100)) {
          if (buffer[8] == 0x01) {
            rslt = LD2410X_E_CONFIG_FAIL;
          }
        } else {
          rslt = LD2410X_E_COMMAND_MISMATCH;
        }
      }
    } else {
      rslt = LD2410X_E_INVALID_LENGTH;
    }
  } else {
    rslt = LD2410X_E_INVALID_RESPONSE;
  }

  return rslt;
}

static inline uint8_t read_frame(struct ld2410x_dev *dev)
{
  uint8_t rslt       = LD2410X_OK;
  uint8_t cmd_word   = dev->comm.cmd_word;
  uint8_t frame_len  = dev->comm.frame_len;
  uint8_t *rx_buffer = dev->comm.rx_buffer;

  dev->intf_status = LD2410X_INTF_READY;

  rslt = check_frame(cmd_word, rx_buffer, frame_len, dev->frame_corr);

  if (rslt == LD2410X_OK) {
    uint8_t buff_offset;
    uint8_t return_value[LD2410X_MAX_LEN_INTRA_BUFFER] = {0};

    buff_offset = (cmd_word != 0) ? 10 : 6;

    config_approve(dev);

    extract_return_data(buff_offset, rx_buffer, return_value);

    rslt = sort_sensor_data(return_value, dev);
  }

  if (rslt != LD2410X_OK) {
    dev->comm.rsp_status = LD2410X_DEV_RSP_STATUS_ERROR;
  }

  return rslt;
}

static inline uint8_t null_ptr_check(const struct ld2410x_dev *dev)
{
  uint8_t rslt = LD2410X_OK;

  if ((dev == NULL) || (dev->read == NULL) || (dev->write == NULL)) {
    /* Device structure is not valid */
    rslt = LD2410X_E_NULL_PTR;
  }

  return rslt;
}

static inline uint8_t boundary_check(uint8_t *value, uint8_t max, uint8_t min)
{
  uint8_t rslt = LD2410X_OK;

  if (value == NULL) {
    return LD2410X_E_NULL_PTR;
  }

  *value = (*value > max) ? max : ((*value < min) ? min : *value);

  return rslt;
}

static inline uint8_t extract_return_data(const uint8_t buff_offset, const uint8_t *buffer, uint8_t *data_buffer)
{
  uint8_t value_len = buffer[4];

  if (value_len >= 2 && buff_offset == 10) value_len -= 2;

  memcpy(data_buffer, buffer + buff_offset, value_len);

  return value_len;
}

static inline uint8_t compose_frame_buff(uint8_t *buffer, const uint8_t *data, uint8_t len)
{
  uint8_t ret_len = 0;

  if (buffer != NULL && data != NULL && len) {
    const uint8_t frame_header[]   = {0xFD, 0xFC, 0xFB, 0xFA};
    const uint8_t data_frame_len[] = {(uint8_t)(len & 0xFF), (uint8_t)((len >> 8) & 0xFF)};
    const uint8_t frame_end[]      = {0x04, 0x03, 0x02, 0x01};

    ret_len = sizeof(frame_header) + sizeof(data_frame_len) + len + sizeof(frame_end);

    memcpy(buffer, (void *)frame_header, sizeof(frame_header));
    memcpy(buffer + sizeof(frame_header), (void *)data_frame_len, sizeof(data_frame_len));
    memcpy(buffer + sizeof(frame_header) + sizeof(data_frame_len), data, len);
    memcpy(buffer + sizeof(frame_header) + sizeof(data_frame_len) + len, (void *)frame_end, sizeof(frame_end));
  }

  return ret_len;
}

static inline uint8_t compose_frame_data(uint8_t *buffer, uint8_t command, const uint8_t *value, uint8_t val_len)
{
  uint8_t ret_len = 0;

  if (command) {
    uint8_t command_word[] = {(uint8_t)(command & 0xFF), (uint8_t)((command >> 8) & 0xFF)};
    memcpy(buffer, command_word, sizeof(command_word));
    ret_len = sizeof(command_word);

    if (value != NULL && val_len > 0) {
      memcpy(buffer + sizeof(command_word), value, val_len);
      ret_len += val_len;
    }
  }

  return ret_len;
}

static inline void config_approve(struct ld2410x_dev *dev)
{
  if (dev->comm.wait_approve == LD2410X_CONFIG_WAIT_APPROVE) {
    memcpy(&dev->conf, &dev->comm.t_conf, sizeof(dev->conf));
  }
  return;
}

static uint8_t sort_sensor_data(const uint8_t *data, struct ld2410x_dev *dev)
{
  uint8_t rslt = LD2410X_OK, self_test = 0, value_len = 0;

  switch (dev->comm.cmd_word) {
    case LD2410X_NO_CMD:

      value_len = dev->comm.frame_len - 11;

      self_test |= (data[1] == 0xAA);
      self_test &= (data[0] == (2 - (dev->conf.engineering_mode == LD2410X_ENGINEERING_MODE_ENABLED)));
      self_test &= (data[value_len - 1] == 0x55);
      self_test &= (data[value_len] == 0x00);

      if (self_test) {
        dev->comm.t_data->trgt_stat = data[2];

        dev->comm.t_data->mov_trgt_dist = LD2410X_MERGE_UINT8(data[4], data[3]);
        dev->comm.t_data->mov_trgt_e    = data[5];

        dev->comm.t_data->stat_trgt_dist = LD2410X_MERGE_UINT8(data[7], data[6]);
        dev->comm.t_data->mov_trgt_e     = data[8];

        dev->comm.t_data->detect_dist = LD2410X_MERGE_UINT8(data[10], data[9]);

        if (dev->conf.engineering_mode == LD2410X_ENGINEERING_MODE_ENABLED) {
          memcpy(dev->comm.t_data->mov_gate_e_val, data + 13, (dev->conf.max_mov_dist_gate + 1));
          memcpy(dev->comm.t_data->stat_gate_e_val, data + 13 + (dev->conf.max_mov_dist_gate + 1), (dev->conf.max_stat_dist_gate + 1));
        }
      }

      break;

    case LD2410X_CMD_EN_CONFIG:

      dev->intf_i = LD2410X_MERGE_UINT8(data[0], data[2]);

      self_test = (uint8_t)dev->intf_i;

      break;

    case LD2410X_CMD_READ_MAC:

      memcpy(dev->mac_addr, data, sizeof(dev->mac_addr));

      self_test = 1;

      break;

    case LD2410X_CMD_READ_FW:

      dev->fw_type = data[1];

      dev->fw_ver[0] = LD2410X_MERGE_UINT8(data[3], data[2]);
      dev->fw_ver[1] = LD2410X_MERGE_UINT8(data[7], data[6]);
      dev->fw_ver[2] = LD2410X_MERGE_UINT8(data[5], data[4]);

      for (uint8_t i = 0; i < (sizeof(dev->fw_ver) / sizeof(dev->fw_ver[0])); i++) {
        self_test |= (dev->fw_ver[i] > 0);
      }

      break;

    case LD2410X_CMD_READ_CONFIG:

      dev->conf.max_dist_gate      = data[1];
      dev->conf.max_mov_dist_gate  = data[2];
      dev->conf.max_stat_dist_gate = data[3];

      memcpy(dev->conf.mov_sensitivity, data + 4, sizeof(dev->conf.mov_sensitivity));
      memcpy(dev->conf.stat_sensitivity, data + 13, sizeof(dev->conf.stat_sensitivity));

      dev->conf.unmanned_duration = LD2410X_MERGE_UINT8(data[23], data[22]);

      self_test |= (data[0] == 0xAA);
      self_test &= (dev->conf.max_dist_gate == LD2410X_MAX_DIST_GATE);
      self_test &= (dev->conf.max_mov_dist_gate > 0);
      self_test &= (dev->conf.max_stat_dist_gate > 0);

      break;

    case LD2410X_CMD_GET_DIST_GATE_RES:

      if (LD2410X_MERGE_UINT8(data[1], data[0]) < LD2410X_DIST_RES_NUM) {
        dev->conf.gate_dist_resolution = LD2410X_MERGE_UINT8(data[1], data[0]);

        self_test = 1;
      }

      break;

    case LD2410X_CMD_BLUETOOTH_PERM:

      dev->bt_perm_obtained = 1;
      self_test             = 1;

      break;

    case LD2410X_CMD_RESTART:

      dev->conf.config_mode = LD2410X_CONFIG_MODE_DISABLED;
      self_test             = 1;

      break;

    default:

      self_test = 1;

      break;
  }

  if (self_test) {
    dev->comm.rsp_status = LD2410X_DEV_RSP_STATUS_SUCCESS;
  } else {
    rslt = LD2410X_E_SELF_TEST;
  }

  return rslt;
}
