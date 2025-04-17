#ifndef LD2410X_DEFS_H_
#define LD2410X_DEFS_H_

#ifdef __KERNEL__
  #include <linux/types.h>
  #include <linux/kernel.h>
#else
  #include <stdint.h>
  #include <stddef.h>
#endif

#ifdef __KERNEL__
  #if !defined(UINT8_C) && !defined(INT8_C)
    #define INT8_C(x)  S8_C(x)
    #define UINT8_C(x) U8_C(x)
  #endif

  #if !defined(UINT16_C) && !defined(INT16_C)
    #define INT16_C(x)  S16_C(x)
    #define UINT16_C(x) U16_C(x)
  #endif

  #if !defined(INT32_C) && !defined(UINT32_C)
    #define INT32_C(x)  S32_C(x)
    #define UINT32_C(x) U32_C(x)
  #endif

  #if !defined(INT64_C) && !defined(UINT64_C)
    #define INT64_C(x)  S64_C(x)
    #define UINT64_C(x) U64_C(x)
  #endif
#endif

/* Macro for merge two uint8_t vars to uint16_t */
#define LD2410X_MERGE_UINT8(msb, lsb)    (uint16_t)((msb << 8) | lsb)

/* Commands definition */
#define LD2410X_CMD_EN_CONFIG            0x00FF      /* Enable configuration command */
#define LD2410X_CMD_END_CONFIG           0x00FE      /* End configuration command */
#define LD2410X_CMD_SET_DIST_DUR         0x0060      /* Set radar's maximum detection range gate (moving & static), and the parameter of unmanned duration */
#define LD2410X_CMD_READ_CONFIG          0x0061      /* Get configuration parameters command */
#define LD2410X_CMD_EN_ENG               0x0062      /* Enable engineering mode command */
#define LD2410X_CMD_CL_ENG               0x0063      /* Close engineering mode command */
#define LD2410X_CMD_GATE_SENS_CONF       0x0064      /* Range gate sensitivity configuration command */
#define LD2410X_CMD_READ_FW              0x00A0      /* Get firmware version command */
#define LD2410X_CMD_SET_BAUDRATE         0x00A1      /* Set serial port baud rate */
#define LD2410X_CMD_RESET                0x00A2      /* Restore all configuration values to their original values */
#define LD2410X_CMD_RESTART              0x00A3      /* Restart the module */
#define LD2410X_CMD_BLUETOOTH_MODE       0x00A4      /* Control bluetooth function */
#define LD2410X_CMD_READ_MAC             0x00A5      /* Get mac address */
#define LD2410X_CMD_BLUETOOTH_PERM       0x00A8      /* Obtain the Bluetooth permission (only answers to Bluetooth, not to the serial port) */
#define LD2410X_CMD_SET_BLUETOOTH_PASSWD 0x00A9      /* Set the password for Bluetooth control */
#define LD2410X_CMD_SET_DIST_GATE_RES    0x00AA      /* Set the distance resolution, that is how far away each distance gate represents */
#define LD2410X_CMD_GET_DIST_GATE_RES    0x00AB      /* Get current distance resolution setting */
#define LD2410X_NO_CMD                   0x0000      /* Handle target data */

#define LD2410X_BLUETOOTH_DEF_PASSWD     "HiLink"    /* Default password for bluetooth connection */
#define LD2410X_BLUETOOTH_PASSWD_LEN     UINT8_C(6)  /* Bluetooth connection password length  */
#define LD2410X_MAX_LEN_INTRA_BUFFER     UINT8_C(35) /* Max length of the intra-frame buffer */

#ifndef LD2410X_FRAME_CORR_SPACE
  #define LD2410X_FRAME_CORR_SPACE UINT8_C(1) /* The space to be added to the frame buffer to correct it */
#endif

#define LD2410X_MAX_FRAME_LEN      UINT8_C(45 + LD2410X_FRAME_CORR_SPACE) /* Max length of the frame buffer (45 + LD2410X_FRAME_CORR_SPACE (for frame correction purpose)) */

#define LD2410X_MAX_DIST_GATE      UINT8_C(8)                             /* Radar's maximum detection range gate (moving & static) */
#define LD2410X_MIN_DIST_GATE      UINT8_C(2)                             /* Radar's minimum detection range gate (moving & static) */

#define LD2410X_MAX_SENS           100                                    /* Radar's maximum sensitivity range (moving & static) */
#define LD2410X_MIN_SENS           0                                      /* Radar's minimum sensitivity range (moving & static) */

/* Return code definitions */
#define LD2410X_OK                 UINT8_C(0) /* Success */

/* Errors */
#define LD2410X_E_NULL_PTR         UINT8_C(1)  /* Null pointer passed */
#define LD2410X_E_COM_FAIL         UINT8_C(2)  /* Communication failure */
#define LD2410X_E_INTF_FAIL        UINT8_C(3)  /* Interface config failure */
#define LD2410X_E_CONFIG_FAIL      UINT8_C(4)  /* Configuration fail response */
#define LD2410X_E_INIT_FAIL        UINT8_C(5)  /* Init failure */
#define LD2410X_E_DEV_BUSY         UINT8_C(6)  /* Device is busy (waiting for response) */
#define LD2410X_E_INVALID_PARAM    UINT8_C(7)  /* Invalid parameter passed */
#define LD2410X_E_INVALID_RESPONSE UINT8_C(8)  /* Invalid frame response */
#define LD2410X_E_COMMAND_MISMATCH UINT8_C(9)  /* Invalid command received */
#define LD2410X_E_INVALID_LENGTH   UINT8_C(10) /* Invalid value length received */
#define LD2410X_E_SELF_TEST        UINT8_C(11) /* Failed self test during data analyzing */

#ifndef LD2410X_INTF_RET_TYPE
  /*
   * The return type from read/write interface.
   * Can be overwritten by the build system
   */
  #define LD2410X_INTF_RET_TYPE uint8_t
#endif

#ifndef LD2410X_INTF_RET_SUCCESS
  /*
   * The success return value from read/write interface.
   * Can be overwritten by the build system.
   * Used to check for a successful execution of the read/write functions
   */
  #define LD2410X_INTF_RET_SUCCESS 0
#endif

/**
 * @brief Bus communication function pointer which should be mapped to
 * the platform specific read functions of the user
 *
 * @param[out]    data     Array to place response from sensor
 * @param[in]     length   Length of the data array
 * @param[in,out] intf_ptr Void pointer that can enable the linking of descriptors
 *                         for interface related callbacks
 * @retval LD2410X_INTF_RET_SUCCESS for success
 */
typedef LD2410X_INTF_RET_TYPE (*ld2410x_read_fptr_t)(uint8_t *data, const uint8_t length, void *intf_ptr);

/**
 * @brief Bus communication function pointer which should be mapped to
 * the platform specific write functions of the user
 *
 * @param[in]     data     Data to be transmitted to sensor
 * @param[in]     length   Length of the data array
 * @param[in,out] intf_ptr Void pointer that can enable the linking of descriptors
 *                         for interface related callbacks
 * @retval LD2410X_INTF_RET_SUCCESS for success
 */
typedef LD2410X_INTF_RET_TYPE (*ld2410x_write_fptr_t)(const uint8_t *data, const uint8_t length, void *intf_ptr);

/* Common selection definitions */

/**
 * @brief Serial port baud rate selection
 */
typedef enum ld2410x_serial_bdrt {
  LD2410X_SERIAL_BDRT_9600 = 1,
  LD2410X_SERIAL_BDRT_19200,
  LD2410X_SERIAL_BDRT_38400,
  LD2410X_SERIAL_BDRT_57600,
  LD2410X_SERIAL_BDRT_115200,
  LD2410X_SERIAL_BDRT_230400,
  LD2410X_SERIAL_BDRT_256000,
  LD2410X_SERIAL_BDRT_460800,
  LD2410X_SERIAL_BDRT_NUM
} ld2410x_serial_bdrt_t;

/**
 * @brief Operation mode selection
 */
typedef enum ld2410x_op_mode {
  LD2410X_CONFIGURATION_MODE,
  LD2410X_CONFIGURATION_DIS,
  LD2410X_ENGINEERING_MODE,
  LD2410X_ENGINEERING_DIS
} ld2410x_op_mode_t;

/**
 * @brief Distance resolution selection
 */
typedef enum ld2410x_dist_res {
  LD2410X_DIST_RES_075,
  LD2410X_DIST_RES_020,
  LD2410X_DIST_RES_NUM
} ld2410x_dist_res_t;

/**
 * @brief Target state value description
 */
typedef enum ld2410x_trgt_state {
  LD2410X_NO_TARGET,
  LD2410X_MOVING_TARGET,
  LD2410X_STATIC_TARGET,
  LD2410X_MOV_STAT_TARGET,
} ld2410x_trgt_state_t;

/**
 * @brief Interface mode selection
 */
typedef enum ld2410x_intf_mode {
  /* Waiting in the ld2410x_dev->read until the data is received  */
  LD2410X_CONTINUOUS_INTF,

  /*
   * Using of the callback method ensures non-blocking operation
   * of the program while waiting for a response via the interface
   */
  LD2410X_CALLBACK_INTF,
} ld2410x_intf_mode_t;

/**
 * @brief Interface selection
 */
typedef enum ld2410x_intf {
  /* Communication through serial port */
  LD2410X_SERIAL_INTF,

  /*
   * Communication through bluetooth connection
   * (Require to send a permission request with password)
   */
  LD2410X_BLUETOOTH_INTF,
} ld2410x_intf_t;

/**
 * @brief Interface status for resolve interface collisions
 */
typedef enum ld2410x_intf_status {
  LD2410X_INTF_READY,
  LD2410X_INTF_BUSY,
} ld2410x_intf_status_t;

/* Enable/Disable definitions */

/**
 * @brief Engineering mode selection
 */
typedef enum ld2410x_engineering_mode {
  LD2410X_ENGINEERING_MODE_DISABLED,
  LD2410X_ENGINEERING_MODE_ENABLED,
  LD2410X_ENGINEERING_MODE_NUM,
} ld2410x_engineering_mode_t;

/**
 * @brief Configuration mode selection
 */
typedef enum ld2410x_config_mode {
  LD2410X_CONFIG_MODE_DISABLED,
  LD2410X_CONFIG_MODE_ENABLED,
  LD2410X_CONFIG_MODE_NUM,
} ld2410x_config_mode_t;

/**
 * @brief Configuration approving mode selection
 */
typedef enum ld2410x_config_wait_mode {
  LD2410X_CONFIG_APPROVED,
  LD2410X_CONFIG_WAIT_APPROVE,
} ld2410x_config_wait_mode_t;

/**
 * @brief Bluetooth mode selection
 */
typedef enum ld2410x_bluetooth_mode {
  LD2410X_BLUETOOTH_MODE_DISABLED,
  LD2410X_BLUETOOTH_MODE_ENABLED,
  LD2410X_BLUETOOTH_MODE_NUM,
} ld2410x_bluetooth_mode_t;

/**
 * @brief Frame correction mode selection
 */
typedef enum ld2410x_frame_corr {
  LD2410X_FRAME_CORR_DISABLED,
  LD2410X_FRAME_CORR_ENABLED,
} ld2410x_frame_corr_t;

/**
 * @brief Communication atomicity mode selection
 */
typedef enum ld2410x_comm_atomicity {
  LD2410X_TRANSACTION_DISABLED,
  LD2410X_TRANSACTION_ENABLED,
} ld2410x_comm_atomicity_t;

/**
 * @brief Device response status
 */
typedef enum ld2410x_rsp_status {
  LD2410X_DEV_RSP_STATUS_UNKNOWN,
  LD2410X_DEV_RSP_STATUS_SUCCESS,
  LD2410X_DEV_RSP_STATUS_ERROR,
  LD2410X_DEV_RSP_STATUS_TIMEOUT,
} ld2410x_rsp_status_t;

/**
 * @brief Sensor field data structure
 */
struct ld2410x_data {
  ld2410x_trgt_state_t trgt_stat; /* Target status */

  uint16_t mov_trgt_dist;         /* Distance to moving target (cm) */

  uint8_t mov_trgt_e;             /* Moving target energy value */

  uint16_t stat_trgt_dist;        /* Distance to static target (cm) */

  uint8_t stat_trgt_e;            /* Static target energy value */

  uint16_t detect_dist;           /* Detection distance(cm) */

  /* Moving target energy value by index of distance gate */
  uint8_t mov_gate_e_val[LD2410X_MAX_DIST_GATE + 1];

  /* Static target energy value by index of distance gate */
  uint8_t stat_gate_e_val[LD2410X_MAX_DIST_GATE + 1];
};

/**
 * @brief LD2410X configuration structure
 */
struct ld2410x_conf {
  ld2410x_serial_bdrt_t baud_rate;              /* Serial port baud rate */

  ld2410x_engineering_mode_t engineering_mode;  /* Engineering mode */

  ld2410x_config_mode_t config_mode;            /* Configuration mode */

  ld2410x_bluetooth_mode_t bt_mode;             /* Bluetooth mode */

  char bt_passwd[LD2410X_BLUETOOTH_PASSWD_LEN]; /* Bluetooth password */

  /*
   * Radar's maximum detection range gate (moving & static).
   * Should be equal to LD2410X_MAX_DIST_GATE (self-test).
   * Set in units of distance gates, and each distance gate is 0.75m or 0.25m (depends of gate_dist_resolution)
   */
  uint8_t max_dist_gate;

  /*
   * Maximum detection range gate to moving target (configuration range 2~8)
   * Set in units of distance gates, and each distance gate is 0.75m or 0.25m (depends of gate_dist_resolution)
   */
  uint8_t max_mov_dist_gate;

  /*
   * Maximum detection range gate to static target (configuration range 2~8)
   * Set in units of distance gates, and each distance gate is 0.75m or 0.25m (depends of gate_dist_resolution)
   */
  uint8_t max_stat_dist_gate;

  /* Sensitivity (range 0~100) of each gate (index is unit of distance gates) for moving target */
  uint8_t mov_sensitivity[LD2410X_MAX_DIST_GATE + 1];

  /* Sensitivity (range 0~100) of each gate (index is unit of distance gates) for static target */
  uint8_t stat_sensitivity[LD2410X_MAX_DIST_GATE + 1];

  /* After the signal disappear, the output state will be unmanned only after the end of the given duration */
  uint16_t unmanned_duration;

  /* Distance resolution, that is how far away each distance gate represents */
  ld2410x_dist_res_t gate_dist_resolution;
};

/**
 * @brief LD2410X device structure
 */
struct ld2410x_dev {
  uint16_t fw_ver[3];                /* Device firmware version */

  uint8_t mac_addr[6];               /* Device mac address */

  ld2410x_intf_t intf;               /* SERIAL/BLUETOOTH interface */

  ld2410x_intf_mode_t intf_mode;     /* Library interface operating mode */

  ld2410x_intf_status_t intf_status; /* Working state of interface */

  struct ld2410x_conf conf;          /* Sensor configuration data */

  void *intf_ptr;                    /* Interface descriptor */

  /*
   * Device firmware type
   * (0x00 - Formal version; 0x01 - Trial version)
   */
  uint8_t fw_type;

  /*
   * Device interface info
   * (1st byte - protocol version; 2nd byte - buffer size)
   */
  uint16_t intf_i;

  /*
   * Enabling frame correction during frame checking
   * for scenarios when the interface receives incorrectly indexed data
   */
  uint8_t frame_corr;

  /*
   * Bluetooth permission status
   * (0 - bt communication not available; 1 - permission obtained)
   */
  uint8_t bt_perm_obtained;

  /* Structure for storing communication data */
  struct {
    ld2410x_rsp_status_t rsp_status; /* Response received */

    uint8_t cmd_word;                /* Which command was sent */

    uint8_t frame_len;               /* Expected frame length */

    uint8_t rx_buffer[LD2410X_MAX_FRAME_LEN];

    struct ld2410x_data *t_data;             /* Pointer to a data storage structure */

    struct ld2410x_conf t_conf;              /* Temporary copy of the configuration structure (used to wait for a successful configuration operation) */

    ld2410x_config_wait_mode_t wait_approve; /* Determine whether temporary config must be moved to main in case of success sensor config */
  } comm;

  ld2410x_read_fptr_t read;        /* Read function pointer */

  ld2410x_write_fptr_t write;      /* Write function pointer */

  LD2410X_INTF_RET_TYPE intf_rslt; /* To store interface pointer error */
};

#define ld2410x_conf_init_zero   \
  (struct ld2410x_conf)          \
  {                              \
    .baud_rate            = 0,   \
    .bt_mode              = 0,   \
    .config_mode          = 0,   \
    .engineering_mode     = 0,   \
    .bt_passwd            = {0}, \
    .max_dist_gate        = 0,   \
    .max_mov_dist_gate    = 0,   \
    .max_stat_dist_gate   = 0,   \
    .mov_sensitivity      = {0}, \
    .stat_sensitivity     = {0}, \
    .unmanned_duration    = 0,   \
    .gate_dist_resolution = 0,   \
  }

#define ld2410x_conf_init_default                                 \
  (struct ld2410x_conf)                                           \
  {                                                               \
    .baud_rate            = LD2410X_SERIAL_BDRT_256000,           \
    .bt_mode              = LD2410X_BLUETOOTH_MODE_ENABLED,       \
    .config_mode          = LD2410X_CONFIG_MODE_DISABLED,         \
    .engineering_mode     = LD2410X_ENGINEERING_MODE_DISABLED,    \
    .bt_passwd            = {0},                                  \
    .max_dist_gate        = LD2410X_MAX_DIST_GATE,                \
    .max_mov_dist_gate    = 8,                                    \
    .max_stat_dist_gate   = 8,                                    \
    .mov_sensitivity      = {50, 50, 40, 30, 20, 15, 15, 15, 15}, \
    .stat_sensitivity     = {0, 0, 40, 40, 30, 30, 20, 20, 20},   \
    .unmanned_duration    = 5,                                    \
    .gate_dist_resolution = LD2410X_DIST_RES_075,                 \
  }

#define ld2410x_dev_comm_init_zero            \
  {                                           \
      .rsp_status   = 0,                      \
      .cmd_word     = 0,                      \
      .frame_len    = 0,                      \
      .rx_buffer    = {0},                    \
      .t_data       = NULL,                   \
      .t_conf       = ld2410x_conf_init_zero, \
      .wait_approve = 0,                      \
  }

#define ld2410x_dev_comm_init_default                 \
  {                                                   \
      .rsp_status   = LD2410X_DEV_RSP_STATUS_UNKNOWN, \
      .cmd_word     = 0,                              \
      .frame_len    = 0,                              \
      .rx_buffer    = {0},                            \
      .t_data       = NULL,                           \
      .t_conf       = ld2410x_conf_init_default,      \
      .wait_approve = LD2410X_CONFIG_APPROVED,        \
  }

#define ld2410x_dev_init_zero                       \
  (struct ld2410x_dev)                              \
  {                                                 \
    .fw_ver           = {0},                        \
    .mac_addr         = {0},                        \
    .intf             = 0,                          \
    .intf_mode        = 0,                          \
    .intf_status      = 0,                          \
    .conf             = ld2410x_conf_init_zero,     \
    .intf_ptr         = NULL,                       \
    .fw_type          = 0,                          \
    .intf_i           = 0,                          \
    .frame_corr       = 0,                          \
    .bt_perm_obtained = 0,                          \
    .comm             = ld2410x_dev_comm_init_zero, \
    .read             = NULL,                       \
    .write            = NULL,                       \
    .intf_rslt        = 0,                          \
  }

#define ld2410x_dev_init_default                       \
  (struct ld2410x_dev)                                 \
  {                                                    \
    .fw_ver           = {0},                           \
    .mac_addr         = {0},                           \
    .intf             = LD2410X_SERIAL_INTF,           \
    .intf_mode        = LD2410X_CONTINUOUS_INTF,       \
    .intf_status      = LD2410X_INTF_READY,            \
    .conf             = ld2410x_conf_init_default,     \
    .intf_ptr         = NULL,                          \
    .fw_type          = 0,                             \
    .intf_i           = 0,                             \
    .frame_corr       = 0,                             \
    .bt_perm_obtained = 0,                             \
    .comm             = ld2410x_dev_comm_init_default, \
    .read             = NULL,                          \
    .write            = NULL,                          \
    .intf_rslt        = 0,                             \
  }

#define ld2410x_data_init_zero            \
  (struct ld2410x_data)                   \
  {                                       \
    .trgt_stat       = LD2410X_NO_TARGET, \
    .mov_trgt_dist   = 0,                 \
    .mov_trgt_e      = 0,                 \
    .stat_trgt_dist  = 0,                 \
    .stat_trgt_e     = 0,                 \
    .detect_dist     = 0,                 \
    .mov_gate_e_val  = {0},               \
    .stat_gate_e_val = {0},               \
  }

#endif /* LD2410X_DEFS_H_ */
