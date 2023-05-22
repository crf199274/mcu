#include "sdk_common.h"
#include "ble_led_service.h"
#include "ble_conn_state.h"
#include "ble_srv_common.h"
#include "nrf_log.h"

static bool led_value_enable = false;

/**写操作
 *
 * @param[in] p_lbs      LED Button Service structure.
 * @param[in] p_ble_evt  Event received from the BLE stack.
 */
static void on_write(ble_ls_t * p_ls, ble_evt_t const * p_ble_evt)
{
    ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

    if (   (p_evt_write->handle == p_ls->led_char_handles.value_handle)
        && (p_evt_write->len == 1)
        && (p_ls->led_write_handler != NULL))
    {
        p_ls->led_write_handler(p_ble_evt->evt.gap_evt.conn_handle, p_ls, p_evt_write->data[0]);
    }else if((p_evt_write->handle == p_ls->led_char_handles.cccd_handle)&&(p_evt_write->len == 2)&&(p_ls->led_write_handler != NULL))
		{
			if(ble_srv_is_notification_enabled(p_evt_write->data))
			{
				led_value_enable = true;
			}
			else
			{
				led_value_enable = false;
			}
		}
}

//服务事件处理函数
void ble_ls_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    ble_ls_t * p_ls = (ble_ls_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
			case BLE_GAP_EVT_CONNECTED:
					p_ls->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
			
			case BLE_GATTS_EVT_WRITE:
					on_write(p_ls, p_ble_evt);
					break;

			default:
					// No implementation needed.
					break;
    }
}

//服务初始化
uint32_t ble_ls_init(ble_ls_t * p_ls, const ble_ls_init_t * p_ls_init)
{
    uint32_t              err_code;
    ble_uuid_t            ble_uuid;
    ble_add_char_params_t add_char_params;
		
		ble_add_descr_params_t add_descr_params;
//    uint8_t                initial_battery_level;
    uint8_t                init_len;
    uint8_t                encoded_report_ref[BLE_SRV_ENCODED_REPORT_REF_LEN];

    // 初始化主服务----主服务的建立
    p_ls->led_write_handler = p_ls_init->led_write_handler;//本次操作的句柄进行赋值
		p_ls->is_notification_supported = p_ls_init->support_notification;

    // Add service.添加基础UUID
    ble_uuid128_t base_uuid = {LS_UUID_BASE};
    err_code = sd_ble_uuid_vs_add(&base_uuid, &p_ls->uuid_type);//专门针对私有服务
    VERIFY_SUCCESS(err_code);

    ble_uuid.type = p_ls->uuid_type;
    ble_uuid.uuid = LS_UUID_SERVICE;
     //添加主服务
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_ls->service_handle);
    VERIFY_SUCCESS(err_code);
     

    // Add LED characteristic.
    memset(&add_char_params, 0, sizeof(add_char_params));
    add_char_params.uuid             = LS_UUID_LED_CHAR;//特性的UUID
    add_char_params.uuid_type        = p_ls->uuid_type;//UUID类型
    add_char_params.init_len         = sizeof(uint8_t);//数据长度
    add_char_params.max_len          = sizeof(uint8_t);//最大的数据长度
    add_char_params.char_props.read  = 1;//空中属性配置
    add_char_params.char_props.write = 1;//空中属性被读和写
		add_char_params.char_props.notify = p_ls->is_notification_supported;
		
		add_char_params.cccd_write_access = p_ls_init->bl_cccd_wr_sec;
    add_char_params.read_access  = p_ls_init->bl_rd_sec;
    add_char_params.write_access = SEC_OPEN;//安全性
		
    err_code = characteristic_add(p_ls->service_handle, &add_char_params, &p_ls->led_char_handles);
		
		if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
		
		if (p_ls_init->p_report_ref != NULL)
    {
        // Add Report Reference descriptor
        init_len = ble_srv_report_ref_encode(encoded_report_ref, p_ls_init->p_report_ref);

        memset(&add_descr_params, 0, sizeof(add_descr_params));
        add_descr_params.uuid        = LS_UUID_LED_REPORT;
				add_char_params.uuid_type    = p_ls->uuid_type;
        add_descr_params.read_access = p_ls_init->bl_report_rd_sec;
        add_descr_params.init_len    = init_len;
        add_descr_params.max_len     = add_descr_params.init_len;
        add_descr_params.p_value     = encoded_report_ref;

        err_code = descriptor_add(p_ls->led_char_handles.value_handle,
                                  &add_descr_params,
                                  &p_ls->report_ref_handle);
        return err_code;
    }
    else
    {
        p_ls->report_ref_handle = BLE_GATT_HANDLE_INVALID;
    }
		
		return err_code;
		
}

ret_code_t ble_led_value_updata(ble_ls_t *p_ls,uint8_t value,uint16_t conn_handle)
{
	if (p_ls == NULL)
	{
			return NRF_ERROR_NULL;
	}
	
	ret_code_t err_code = NRF_SUCCESS;
	ble_gatts_value_t  gatts_value;
	
	memset(&gatts_value, 0, sizeof(gatts_value));
	
	gatts_value.len     = sizeof(uint8_t);
	gatts_value.offset  = 0;
	gatts_value.p_value = &value;
	
//	err_code = sd_ble_gatts_value_set(BLE_CONN_HANDLE_INVALID,p_ls->led_char_handles.value_handle,&gatts_value);
//	
//	if (err_code == NRF_SUCCESS)
//	{
//			NRF_LOG_INFO("Battery level has been updated: %d%%", value)
//	}
//	else
//	{
//			NRF_LOG_DEBUG("Error during battery level update: 0x%08X", err_code)

//			return err_code;
//	}
	
	if (p_ls->is_notification_supported)
	{
		if(led_value_enable)
		{
			ble_gatts_hvx_params_t hvx_params;
			
			memset(&hvx_params, 0, sizeof(hvx_params));

			hvx_params.handle = p_ls->led_char_handles.value_handle;
			hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
			hvx_params.offset = gatts_value.offset;
			hvx_params.p_len  = &gatts_value.len;
			hvx_params.p_data = gatts_value.p_value;
			
			err_code = sd_ble_gatts_hvx(p_ls->conn_handle,&hvx_params);
			
			if(err_code == NRF_SUCCESS)
			{
				NRF_LOG_INFO("Battery notification has been sent using conn_handle: 0x%04X", p_ls->conn_handle);
			}
			else
			{
				NRF_LOG_DEBUG("Error: 0x%08X while sending notification with conn_handle: 0x%04X",
																err_code,
																p_ls->conn_handle);
			}
			
//			ble_conn_state_conn_handle_list_t conn_handles = ble_conn_state_conn_handles();
//			
//			for (uint32_t i = 0; i < conn_handles.len; i++)
//			{
//				if (ble_conn_state_status(conn_handles.conn_handles[i]) == BLE_CONN_STATUS_CONNECTED)
//				{
//					if (err_code == NRF_SUCCESS)
//					{
//							NRF_LOG_INFO("Battery notification has been sent using conn_handle: 0x%04X", p_ls->service_handle);
//							err_code = sd_ble_gatts_hvx(conn_handles.conn_handles[i],&hvx_params);
//					}
//					else
//					{
//							NRF_LOG_DEBUG("Error: 0x%08X while sending notification with conn_handle: 0x%04X",
//														err_code,
//														p_ls->service_handle);
//							UNUSED_RETURN_VALUE(sd_ble_gatts_hvx(conn_handles.conn_handles[i],
//																													  &hvx_params));
//					}
//				}
//			}
		}
	}
	
	return err_code;
}
