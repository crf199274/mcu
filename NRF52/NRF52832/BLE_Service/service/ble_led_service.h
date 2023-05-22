#ifndef __ble_led_service_H
#define __ble_led_service_H

#include <stdint.h>
#include <stdbool.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "nrf_sdh_ble.h"

#ifdef __cplusplus
extern "C" {
#endif

#define BLE_LS_DEF(_name) 																																					\
static ble_ls_t _name;                                                                              \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                                                                 \
										 BLE_LBS_BLE_OBSERVER_PRIO,                                                     \
										 ble_ls_on_ble_evt, &_name)
										 
#define LS_UUID_BASE        {0x23, 0xD1, 0xBC, 0xEA, 0x5F, 0x78, 0x23, 0x15, \
                              0xDE, 0xEF, 0x12, 0x12, 0x00, 0x00, 0x00, 0x00}

#define LS_UUID_SERVICE     0x1523
#define LS_UUID_BUTTON_CHAR 0x1524
#define LS_UUID_LED_CHAR    0x1525
#define LS_UUID_LED_REPORT  0x1526

//															
typedef struct ble_ls_s ble_ls_t;

typedef void (*ble_lbs_led_write_handler_t) (uint16_t conn_handle, ble_ls_t * p_ls, uint8_t new_state);

//(LED)服务初始化结构体															
typedef struct
{
	ble_lbs_led_write_handler_t led_write_handler; /**< Event handler to be called when the LED Characteristic is written. */
	bool                   support_notification;           /**< TRUE if notification of Battery Level measurement is supported. */
	ble_srv_report_ref_t * p_report_ref;                   /**< If not NULL, a Report Reference descriptor with the specified value will be added to the Battery Level characteristic */
//	uint8_t                initial_batt_level;             /**< Initial battery level */
	security_req_t         bl_rd_sec;                      /**< Security requirement for reading the BL characteristic value. */
	security_req_t         bl_cccd_wr_sec;                 /**< Security requirement for writing the BL characteristic CCCD. */
	security_req_t         bl_report_rd_sec;               /**< Security requirement for reading the BL characteristic descriptor. */
	
} ble_ls_init_t;															

//(LED)服务结构体
struct ble_ls_s
{
	uint16_t										conn_handle;
	uint16_t                    service_handle;      /**< Handle of LED Button Service (as provided by the BLE stack). */
	ble_gatts_char_handles_t    led_char_handles;    /**< Handles related to the LED Characteristic. */
//    ble_gatts_char_handles_t    button_char_handles; /**< Handles related to the Button Characteristic. */
	uint16_t                 report_ref_handle;         /**< Handle of the Report Reference descriptor. */
	uint8_t                     uuid_type;           /**< UUID type for the LED Button Service. */
	ble_lbs_led_write_handler_t led_write_handler;   /**< Event handler to be called when the LED Characteristic is written. */
	bool                     is_notification_supported; /**< TRUE if notification of Battery Level is supported. */
};

//(LED)服务初始化
uint32_t ble_ls_init(ble_ls_t * p_ls, const ble_ls_init_t * p_ls_init);

//(LED)服务事件处理
void ble_ls_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);

//(LED)更新属性值
ret_code_t ble_led_value_updata(ble_ls_t *p_ls,uint8_t value,uint16_t conn_handle);

#ifdef __cplusplus
}
#endif

#endif
