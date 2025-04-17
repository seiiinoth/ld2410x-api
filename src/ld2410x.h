#ifndef _LD2410X_H
#define _LD2410X_H

#include <stdlib.h>
#include "ld2410x_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief This API returns the response status from the sensor,
 * which is determined after checking the self_test response
 *
 * @param[in] dev Structure instance of ld2410x_dev
 * @return Response status defined in ld2410x_rsp_status_t
 */
ld2410x_rsp_status_t ld2410x_rsp_status(const struct ld2410x_dev *dev);

/**
 * @brief This API is used to reset the state of the interface
 * (when the timeout handler is triggered)
 *
 * @param[in] dev Structure instance of ld2410x_dev
 */
void ld2410x_intf_reset(struct ld2410x_dev *dev);

/**
 * @brief This API is used to enable or disable the configuration mode of the sensor
 *
 * @param[in] config_mode Desired configuration mode (Enabled/Disabled)
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_set_config_mode(const ld2410x_config_mode_t config_mode, struct ld2410x_dev *dev);

/**
 * @brief This API is used to enable or disable the engineering mode of the sensor
 *
 * @param[in] engineering_mode Desired engineering mode (Enabled/Disabled)
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_set_engineering_mode(const ld2410x_engineering_mode_t engineering_mode, struct ld2410x_dev *dev);

/**
 * @brief This API is used to set the radar's maximum detection range segment
 * (moving & static)(configuration range 2-8) and the parameter
 * of 'unmanned' duration (configuration range 0-65535 seconds)
 *
 * @param[in] mov_dist  Maximum detection range segment for moving target
 * @param[in] stat_dist Maximum detection range segment for static target
 * @param[in] u_dur     'Unmanned' duration
 * @param[in,out] dev   Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_set_dist_dur(uint8_t mov_dist, uint8_t stat_dist, const uint16_t u_dur, struct ld2410x_dev *dev);

/**
 * @brief This API is used to request configuration data of the sensor
 *
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_get_config_data(struct ld2410x_dev *dev);

/**
 * @brief This API is used to configure the sensitivity of distance segments
 *
 * @param[in] dist_gate Distance segment index
 * @param[in] mov_sens  Motion sensitivity value
 * @param[in] stat_sens Static sensitivity value
 * @param[in] unite     If 1 -> Set sensitivity of all distance segments to the same value
 * @param[in,out] dev   Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_set_gate_sensitivity(uint8_t dist_gate, uint8_t mov_sens, uint8_t stat_sens, const uint8_t unite, struct ld2410x_dev *dev);

/**
 * @brief This API is used to request the firmware version of the sensor
 *
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_get_fw_ver(struct ld2410x_dev *dev);

/**
 * @brief This API is used to set serial port baud rate
 *
 * @param[in] serial_bdrt Serial baud rate index
 * @param[in,out] dev     Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_set_baudrate(const ld2410x_serial_bdrt_t serial_bdrt, struct ld2410x_dev *dev);

/**
 * @brief This API is used to restore all configuration values to their original values
 *
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_restore_configuration(struct ld2410x_dev *dev);

/**
 * @brief This API is used for restarting the sensor
 *
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_restart(struct ld2410x_dev *dev);

/**
 * @brief This API is used to control the Bluetooth mode
 *
 * @param[in] bt_mode Desired operation mode
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_set_bluetooth_mode(const ld2410x_bluetooth_mode_t bt_mode, struct ld2410x_dev *dev);

/**
 * @brief This API is used to request the mac address of sensor
 *
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_get_mac_addr(struct ld2410x_dev *dev);

/**
 * @brief This API is used to get the Bluetooth permission for communication
 *
 * @param[in] bt_passwd String with 6 chars of password value
 * @param[in,out] dev   Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_obtain_bt_permission(const char *bt_passwd, struct ld2410x_dev *dev);

/**
 * @brief This API is used to set the Bluetooth password
 *
 * @param[in] bt_passwd String with 6 chars of password value
 * @param[in,out] dev   Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_set_bt_password(const char *bt_passwd, struct ld2410x_dev *dev);

/**
 * @brief This API is used to set the distance resolution, that is how far away each distance segment represents
 *
 * @param[in] res_indx Distance resolution index
 * @param[in,out] dev  Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_set_dist_res(const ld2410x_dist_res_t res_indx, struct ld2410x_dev *dev);

/**
 * @brief This API is used to get the distance resolution, that is how far away each distance segment represents
 *
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_get_dist_res(struct ld2410x_dev *dev);

/**
 * @brief This API is used to proceed the radar detection result
 *
 * @param[in,out] dev  Structure instance of ld2410x_dev
 * @param[out]    data Structure instance of ld2410x_data
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_get_target_data(struct ld2410x_data *data, struct ld2410x_dev *dev);

/**
 * @brief This API provides a callback method for non non-blocking interface
 *
 * @param[in,out] dev Structure instance of ld2410x_dev
 *
 * @return Result of API execution status
 * @retval LD2410X_OK in the case of success, otherwise error code
 */
uint8_t ld2410x_intf_callback(struct ld2410x_dev *dev);

#ifdef __cplusplus
}
#endif
#endif /* _LD2410X_H */
