/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

/****************************************************************************
 *
 * This demo showcases BLE GATT server. It can send adv data, be connected by client.
 * Run the gatt_client demo, the client demo will automatically connect to the gatt_server demo.
 * Client demo will enable gatt_server's notify after connection. The two devices will then exchange
 * data.
 *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_bt.h"

#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_defs.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gatt_common_api.h"

#include "sdkconfig.h"

#define GATTS_TAG "TELEMETRY_UNIT_GATTS"

#define GATTS_SERVICE_UUID_TEST 0x00FF
#define GATTS_CHAR_UUID_TEST 0xFF01
#define GATTS_DESCR_UUID_TEST 0x3333
#define GATTS_NUM_HANDLE_TEST 4

// Define UUIDs we are using

// Battery Service
#define GATTS_BATTERY_SERVICE_UUID 0x180F
#define GATTS_BATTERY_LEVEL_CHAR_UUID 0x2A19
#define GATTS_BATTERY_STATUS_CHAR_UUID 0x2BED
#define GATTS_NUM_HANDLE_BAT 5

// Custom PSP Service
static esp_bt_uuid_t gatts_psp_service_uuid = {
    .len = ESP_UUID_LEN_128,
    .uuid.uuid128 = {0x2c, 0xe3, 0x1d, 0x4e, 0xb7, 0x8d, 0x97, 0x84,
                     0x6e, 0x44, 0x6b, 0x8f, 0xca, 0x0d, 0x9a, 0x14}};

static esp_bt_uuid_t gatts_psp_char_uuid = {
    .len = ESP_UUID_LEN_128,
    .uuid.uuid128 = {0xe7, 0xb1, 0xd3, 0xc0, 0xfd, 0x2e, 0xc4, 0x93,
                     0xda, 0x44, 0x12, 0xda, 0xb7, 0xc8, 0x1d, 0xa6}};
#define GATTS_NUM_HANDLE_PSP 3

static char device_name[ESP_BLE_ADV_NAME_LEN_MAX] = "PSP Telemetry Unit";

#define TEST_MANUFACTURER_DATA_LEN 17
#define GATTS_DEMO_CHAR_VAL_LEN_MAX 0x40
#define PREPARE_BUF_MAX_SIZE 1024

static uint8_t char1_str[] = {0x11, 0x22, 0x33};
static uint16_t descr_value = 0x0;
/**
 * Current MTU (Maximum Transmission Unit) size for the active connection.
 *
 * This simplified implementation assumes a single connection.
 * For multi-connection scenarios, the MTU should be stored per connection ID.
 */
static uint16_t local_mtu = 23;
static uint8_t char_value_read[CONFIG_EXAMPLE_CHAR_READ_DATA_LEN] = {0xDE, 0xED, 0xBE, 0xEF};

static uint8_t adv_config_done = 0;
#define adv_config_flag (1 << 0)
#define scan_rsp_config_flag (1 << 1)

static uint8_t adv_service_uuid128[32] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    // first uuid, 16bit, [12],[13] is the value
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0xEE,
    0x00,
    0x00,
    0x00,
    // second uuid, 32bit, [12], [13], [14], [15] is the value
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
};

// The length of adv data must be less than 31 bytes
// static uint8_t test_manufacturer[TEST_MANUFACTURER_DATA_LEN] =  {0x12, 0x23, 0x45, 0x56};
// adv data
static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = false,
    .min_interval = 0x0006, // slave connection min interval, Time = min_interval * 1.25 msec
    .max_interval = 0x0010, // slave connection max interval, Time = max_interval * 1.25 msec
    .appearance = 0x00,
    .manufacturer_len = 0,       // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
// scan response data
static esp_ble_adv_data_t scan_rsp_data = {
    .set_scan_rsp = true,
    .include_name = true,
    .include_txpower = true,
    //.min_interval = 0x0006,
    //.max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0,       // TEST_MANUFACTURER_DATA_LEN,
    .p_manufacturer_data = NULL, //&test_manufacturer[0],
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    //.peer_addr            =
    //.peer_addr_type       =
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

// struct gatts_profile_inst
// {
//     esp_gatts_cb_t gatts_cb;
//     uint16_t gatts_if;
//     uint16_t app_id;
//     uint16_t conn_id;
//     uint16_t service_handle;
//     esp_gatt_srvc_id_t service_id;
//     uint16_t char_handle;
//     esp_bt_uuid_t char_uuid;
//     esp_gatt_perm_t perm;
//     esp_gatt_char_prop_t property;
//     uint16_t descr_handle;
//     esp_bt_uuid_t descr_uuid;
// };

// /* One gatt-based profile one app_id and one gatts_if, this array will store the gatts_if returned by ESP_GATTS_REG_EVT */
// static struct gatts_profile_inst gatts_profile = {
//     .gatts_cb = gatts_profile_event_handler,
//     .gatts_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
// }

static uint16_t s_gatts_if;
static uint16_t s_app_id;
static uint16_t s_conn_id;
static esp_gatt_perm_t s_perm;

typedef struct
{
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_level_handle;
    esp_bt_uuid_t char_level_uuid;
    esp_gatt_char_prop_t char_level_property;
    uint16_t char_status_handle;
    esp_bt_uuid_t char_status_uuid;
    esp_gatt_char_prop_t char_status_property;
} gatts_battery_service_t;

static gatts_battery_service_t s_gatts_battery_service = {
    .char_level_handle = 0,
    .service_id = {
        .is_primary = true,
        .id = {
            .inst_id = 0x00,
            .uuid = {
                .len = ESP_UUID_LEN_16,
                .uuid.uuid16 = GATTS_BATTERY_SERVICE_UUID}}},
    .char_level_uuid = {
        .len = ESP_UUID_LEN_16,
        .uuid.uuid16 = GATTS_BATTERY_LEVEL_CHAR_UUID,
    },
    .char_status_uuid = {
        .len = ESP_UUID_LEN_16,
        .uuid.uuid16 = GATTS_BATTERY_STATUS_CHAR_UUID,
    },
};

typedef struct
{
    uint16_t service_handle;
    esp_gatt_srvc_id_t service_id;
    uint16_t char_psp_handle;
    esp_bt_uuid_t char_psp_uuid;
    esp_gatt_char_prop_t char_psp_property;
} gatts_psp_service_t;

static gatts_psp_service_t s_gatts_psp_service = {
    .service_id = {
        .is_primary = true,
        .id = {
            .inst_id = 0x00,
            .uuid = gatts_psp_service_uuid,
        }
    },
    .char_psp_uuid = gatts_psp_char_uuid,
}

typedef struct
{
    uint8_t *prepare_buf;
    int prepare_len;
} prepare_type_env_t;

static prepare_type_env_t a_prepare_write_env;

void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);
void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param);

void example_write_event_env(esp_gatt_if_t gatts_if, prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param)
{
    esp_gatt_status_t status = ESP_GATT_OK;
    if (param->write.need_rsp)
    {
        if (param->write.is_prep)
        {
            if (param->write.offset > PREPARE_BUF_MAX_SIZE)
            {
                status = ESP_GATT_INVALID_OFFSET;
            }
            else if ((param->write.offset + param->write.len) > PREPARE_BUF_MAX_SIZE)
            {
                status = ESP_GATT_INVALID_ATTR_LEN;
            }
            if (status == ESP_GATT_OK && prepare_write_env->prepare_buf == NULL)
            {
                prepare_write_env->prepare_buf = (uint8_t *)malloc(PREPARE_BUF_MAX_SIZE * sizeof(uint8_t));
                prepare_write_env->prepare_len = 0;
                if (prepare_write_env->prepare_buf == NULL)
                {
                    ESP_LOGE(GATTS_TAG, "Gatt_server prep no mem");
                    status = ESP_GATT_NO_RESOURCES;
                }
            }

            esp_gatt_rsp_t *gatt_rsp = (esp_gatt_rsp_t *)malloc(sizeof(esp_gatt_rsp_t));
            if (gatt_rsp)
            {
                gatt_rsp->attr_value.len = param->write.len;
                gatt_rsp->attr_value.handle = param->write.handle;
                gatt_rsp->attr_value.offset = param->write.offset;
                gatt_rsp->attr_value.auth_req = ESP_GATT_AUTH_REQ_NONE;
                memcpy(gatt_rsp->attr_value.value, param->write.value, param->write.len);
                esp_err_t response_err = esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, gatt_rsp);
                if (response_err != ESP_OK)
                {
                    ESP_LOGE(GATTS_TAG, "Send response error\n");
                }
                free(gatt_rsp);
            }
            else
            {
                ESP_LOGE(GATTS_TAG, "malloc failed, no resource to send response error\n");
                status = ESP_GATT_NO_RESOURCES;
            }
            if (status != ESP_GATT_OK)
            {
                return;
            }
            memcpy(prepare_write_env->prepare_buf + param->write.offset,
                   param->write.value,
                   param->write.len);
            prepare_write_env->prepare_len += param->write.len;
        }
        else
        {
            esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, status, NULL);
        }
    }
}

void example_exec_write_event_env(prepare_type_env_t *prepare_write_env, esp_ble_gatts_cb_param_t *param)
{
    if (param->exec_write.exec_write_flag == ESP_GATT_PREP_WRITE_EXEC)
    {
        ESP_LOG_BUFFER_HEX(GATTS_TAG, prepare_write_env->prepare_buf, prepare_write_env->prepare_len);
    }
    else
    {
        ESP_LOGI(GATTS_TAG, "Prepare write cancel");
    }
    if (prepare_write_env->prepare_buf)
    {
        free(prepare_write_env->prepare_buf);
        prepare_write_env->prepare_buf = NULL;
    }
    prepare_write_env->prepare_len = 0;
}

// Event Handlers
static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    /* If event is register event, store the gatts_if for each profile */
    if (event == ESP_GATTS_REG_EVT)
    {
        if (param->reg.status == ESP_GATT_OK)
        {
            s_gatts_if = gatts_if;
        }
        else
        {
            ESP_LOGI(GATTS_TAG, "Reg app failed, app_id %04x, status %d",
                     param->reg.app_id,
                     param->reg.status);
            return;
        }
    }

    switch (event)
    {
    case ESP_GATTS_REG_EVT:
        ESP_LOGI(GATTS_TAG, "GATT server register, status %d, app_id %d, gatts_if %d", param->reg.status, param->reg.app_id, gatts_if);

        // Create PSP Service
        esp_err_t ret = esp_ble_gatts_create_service(gatts_if, &s_gatts_psp_service.service_id, GATTS_NUM_HANDLE_PSP);
        if (ret)
        {
            ESP_LOGE(GATTS_TAG, "Create PSP service failed, error code = %x", ret);
        }

        // Create Battery Service
        esp_err_t ret = esp_ble_gatts_create_service(gatts_if, &s_gatts_battery_service.service_id, GATTS_NUM_HANDLE_BAT);
        if (ret)
        {
            ESP_LOGE(GATTS_TAG, "Create battery service failed, error code = %x", ret);
        }

        esp_err_t set_dev_name_ret = esp_ble_gap_set_device_name(device_name);
        if (set_dev_name_ret)
        {
            ESP_LOGE(GATTS_TAG, "set device name failed, error code = %x", set_dev_name_ret);
        }

        // config adv data
        esp_err_t ret = esp_ble_gap_config_adv_data(&adv_data);
        if (ret)
        {
            ESP_LOGE(GATTS_TAG, "config adv data failed, error code = %x", ret);
        }
        adv_config_done |= adv_config_flag;

        // config scan response data
        ret = esp_ble_gap_config_adv_data(&scan_rsp_data);
        if (ret)
        {
            ESP_LOGE(GATTS_TAG, "config scan response data failed, error code = %x", ret);
        }
        adv_config_done |= scan_rsp_config_flag;

        break;
    case ESP_GATTS_READ_EVT:
    {
        ESP_LOGI(GATTS_TAG,
                 "Characteristic read request: conn_id=%d, trans_id=%" PRIu32 ", handle=%d, is_long=%d, offset=%d, need_rsp=%d",
                 param->read.conn_id, param->read.trans_id, param->read.handle,
                 param->read.is_long, param->read.offset, param->read.need_rsp);

        // If no response is needed, exit early (stack handles it automatically)
        if (!param->read.need_rsp)
        {
            return;
        }

        esp_gatt_rsp_t rsp;
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;

        // Handle descriptor read request
        if (param->read.handle == gatts_profile.descr_handle)
        {
            memcpy(rsp.attr_value.value, &descr_value, 2);
            rsp.attr_value.len = 2;
            esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
            return;
        }

        // Handle characteristic read request
        if (param->read.handle == gatts_profile.char_handle)
        {
            uint16_t offset = param->read.offset;

            // Validate read offset
            if (param->read.is_long && offset > CONFIG_EXAMPLE_CHAR_READ_DATA_LEN)
            {
                ESP_LOGW(GATTS_TAG, "Read offset (%d) out of range (0-%d)", offset, CONFIG_EXAMPLE_CHAR_READ_DATA_LEN);
                rsp.attr_value.len = 0;
                esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_INVALID_OFFSET, &rsp);
                return;
            }

            // Determine response length based on MTU
            uint16_t mtu_size = local_mtu - 1; // ATT header (1 byte)
            uint16_t send_len = (CONFIG_EXAMPLE_CHAR_READ_DATA_LEN - offset > mtu_size) ? mtu_size : (CONFIG_EXAMPLE_CHAR_READ_DATA_LEN - offset);

            memcpy(rsp.attr_value.value, &char_value_read[offset], send_len);
            rsp.attr_value.len = send_len;

            // Send response to GATT client
            esp_err_t err = esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id, ESP_GATT_OK, &rsp);
            if (err != ESP_OK)
            {
                ESP_LOGE(GATTS_TAG, "Failed to send response: %s", esp_err_to_name(err));
            }
        }
        break;
    }
    case ESP_GATTS_WRITE_EVT:
    {
        ESP_LOGI(GATTS_TAG, "Characteristic write, conn_id %d, trans_id %" PRIu32 ", handle %d", param->write.conn_id, param->write.trans_id, param->write.handle);
        if (!param->write.is_prep)
        {
            ESP_LOGI(GATTS_TAG, "value len %d, value ", param->write.len);
            ESP_LOG_BUFFER_HEX(GATTS_TAG, param->write.value, param->write.len);
            if (gatts_profile.descr_handle == param->write.handle && param->write.len == 2)
            {
                descr_value = param->write.value[1] << 8 | param->write.value[0];
                if (descr_value == 0x0001)
                {
                    if (a_property & ESP_GATT_CHAR_PROP_BIT_NOTIFY)
                    {
                        ESP_LOGI(GATTS_TAG, "Notification enable");
                        uint8_t notify_data[15];
                        for (int i = 0; i < sizeof(notify_data); ++i)
                        {
                            notify_data[i] = i % 0xff;
                        }
                        // the size of notify_data[] need less than MTU size
                        esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gatts_profile.char_handle,
                                                    sizeof(notify_data), notify_data, false);
                    }
                }
                else if (descr_value == 0x0002)
                {
                    if (a_property & ESP_GATT_CHAR_PROP_BIT_INDICATE)
                    {
                        ESP_LOGI(GATTS_TAG, "Indication enable");
                        uint8_t indicate_data[15];
                        for (int i = 0; i < sizeof(indicate_data); ++i)
                        {
                            indicate_data[i] = i % 0xff;
                        }
                        // the size of indicate_data[] need less than MTU size
                        esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gatts_profile.char_handle,
                                                    sizeof(indicate_data), indicate_data, true);
                    }
                }
                else if (descr_value == 0x0000)
                {
                    ESP_LOGI(GATTS_TAG, "Notification/Indication disable");
                }
                else
                {
                    ESP_LOGE(GATTS_TAG, "Unknown descriptor value");
                    ESP_LOG_BUFFER_HEX(GATTS_TAG, param->write.value, param->write.len);
                }
            }
        }
        example_write_event_env(gatts_if, &a_prepare_write_env, param);
        break;
    }
    case ESP_GATTS_EXEC_WRITE_EVT:
        ESP_LOGI(GATTS_TAG, "Execute write");
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, ESP_GATT_OK, NULL);
        example_exec_write_event_env(&a_prepare_write_env, param);
        break;
    case ESP_GATTS_MTU_EVT:
        ESP_LOGI(GATTS_TAG, "MTU exchange, MTU %d", param->mtu.mtu);
        local_mtu = param->mtu.mtu;
        break;
    case ESP_GATTS_UNREG_EVT:
        break;
    case ESP_GATTS_CREATE_EVT:
        ESP_LOGI(GATTS_TAG, "Service create, status %d, service_handle %d", param->create.status, param->create.service_handle);
        if (param->create.service_id.id.uuid == GATTS_BATTERY_SERVICE_UUID)
        {
            s_gatts_battery_service.service_handle = param->create.service_handle;

            // Here when we are adding characteristics there's something called the attribute value (I think we need to pass in a pointer to the characteristic value), 
            // gatts_demo_char1_val is still from the demo code, are we just going to create one for our test messages?

            // Start Battery Service
            esp_ble_gatts_start_service(s_gatts_battery_service.service_handle);

            // Add Battery Level Characteristic
            esp_err_t add_char_ret = esp_ble_gatts_add_char(s_gatts_battery_service.char_level_handle, s_gatts_battery_service.char_level_uuid,
                                                            ESP_GATT_PERM_READ,
                                                            ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY,
                                                            &gatts_demo_char1_val, NULL);

            if (add_char_ret)
            {
                ESP_LOGE(GATTS_TAG, "add battery level char failed, error code =%x", add_char_ret);
            }

            // Add Battery Status Characteristic
            esp_err_t add_char_ret = esp_ble_gatts_add_char(s_gatts_battery_service.char_status_handle, s_gatts_battery_service.char_status_uuid,
                                                            ESP_GATT_PERM_READ,
                                                            ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY,
                                                            &gatts_demo_char1_val, NULL);

            if (add_char_ret)
            {
                ESP_LOGE(GATTS_TAG, "add battery status char failed, error code =%x", add_char_ret);
            }
        } else if (param->create.service_id.id.uuid == gatts_psp_service_uuid) {
            s_gatts_psp_service.service_handle = param->create.service_handle;

            //Start PSP Service
            esp_ble_gatts_start_service(s_gatts_psp_service.service_handle);

            //Add PSP Characteristic
            esp_err_t add_char_ret = esp_ble_gatts_add_char(s_gatts_psp_service.char_psp_handle, s_gatts_psp_service.char_psp_uuid,
                                                            ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                            ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_NOTIFY,
                                                            &gatts_demo_char1_val, NULL);

            if (add_char_ret)
            {
                ESP_LOGE(GATTS_TAG, "add psp char failed, error code =%x", add_char_ret);
            }
        }

        break;
    case ESP_GATTS_ADD_INCL_SRVC_EVT:
        break;
    case ESP_GATTS_ADD_CHAR_EVT:
    {
        uint16_t length = 0;
        const uint8_t *prf_char;

        ESP_LOGI(GATTS_TAG, "Characteristic add, status %d, attr_handle %d, service_handle %d",
                 param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        gatts_profile.char_handle = param->add_char.attr_handle;
        gatts_profile.descr_uuid.len = ESP_UUID_LEN_16;
        gatts_profile.descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(param->add_char.attr_handle, &length, &prf_char);
        if (get_attr_ret == ESP_FAIL)
        {
            ESP_LOGE(GATTS_TAG, "ILLEGAL HANDLE");
        }

        ESP_LOGI(GATTS_TAG, "the gatts demo char length = %x", length);
        for (int i = 0; i < length; i++)
        {
            ESP_LOGI(GATTS_TAG, "prf_char[%x] =%x", i, prf_char[i]);
        }
        esp_err_t add_descr_ret = esp_ble_gatts_add_char_descr(gatts_profile.service_handle, &gatts_profile.descr_uuid,
                                                               ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);
        if (add_descr_ret)
        {
            ESP_LOGE(GATTS_TAG, "add char descr failed, error code =%x", add_descr_ret);
        }
        break;
    }
    case ESP_GATTS_ADD_CHAR_DESCR_EVT:
        gatts_profile.descr_handle = param->add_char_descr.attr_handle;
        ESP_LOGI(GATTS_TAG, "Descriptor add, status %d, attr_handle %d, service_handle %d",
                 param->add_char_descr.status, param->add_char_descr.attr_handle, param->add_char_descr.service_handle);
        break;
    case ESP_GATTS_DELETE_EVT:
        break;
    case ESP_GATTS_START_EVT:
        ESP_LOGI(GATTS_TAG, "Service start, status %d, service_handle %d",
                 param->start.status, param->start.service_handle);
        break;
    case ESP_GATTS_STOP_EVT:
        break;
    case ESP_GATTS_CONNECT_EVT:
    {
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
        conn_params.latency = 0;
        conn_params.max_int = 0x20; // max_int = 0x20*1.25ms = 40ms
        conn_params.min_int = 0x10; // min_int = 0x10*1.25ms = 20ms
        conn_params.timeout = 400;  // timeout = 400*10ms = 4000ms
        ESP_LOGI(GATTS_TAG, "Connected, conn_id %u, remote " ESP_BD_ADDR_STR "",
                 param->connect.conn_id, ESP_BD_ADDR_HEX(param->connect.remote_bda));
        gatts_profile.conn_id = param->connect.conn_id;
        // start sent the update connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);
        break;
    }
    case ESP_GATTS_DISCONNECT_EVT:
        ESP_LOGI(GATTS_TAG, "Disconnected, remote " ESP_BD_ADDR_STR ", reason 0x%02x",
                 ESP_BD_ADDR_HEX(param->disconnect.remote_bda), param->disconnect.reason);
        esp_ble_gap_start_advertising(&adv_params);
        local_mtu = 23; // Reset MTU for a single connection
        break;
    case ESP_GATTS_CONF_EVT:
        ESP_LOGI(GATTS_TAG, "Confirm receive, status %d, attr_handle %d", param->conf.status, param->conf.handle);
        if (param->conf.status != ESP_GATT_OK)
        {
            ESP_LOG_BUFFER_HEX(GATTS_TAG, param->conf.value, param->conf.len);
        }
        break;
    case ESP_GATTS_OPEN_EVT:
    case ESP_GATTS_CANCEL_OPEN_EVT:
    case ESP_GATTS_CLOSE_EVT:
    case ESP_GATTS_LISTEN_EVT:
    case ESP_GATTS_CONGEST_EVT:
    default:
        break;
    }
}

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event)
    {

    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        adv_config_done &= (~adv_config_flag);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
        adv_config_done &= (~scan_rsp_config_flag);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&adv_params);
        }
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        // advertising start complete event to indicate advertising start successfully or failed
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(GATTS_TAG, "Advertising start failed, status %d", param->adv_start_cmpl.status);
            break;
        }
        ESP_LOGI(GATTS_TAG, "Advertising start successfully");
        break;
    case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            ESP_LOGE(GATTS_TAG, "Advertising stop failed, status %d", param->adv_stop_cmpl.status);
            break;
        }
        ESP_LOGI(GATTS_TAG, "Advertising stop successfully");
        break;
    case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
        ESP_LOGI(GATTS_TAG, "Connection params update, status %d, conn_int %d, latency %d, timeout %d",
                 param->update_conn_params.status,
                 param->update_conn_params.conn_int,
                 param->update_conn_params.latency,
                 param->update_conn_params.timeout);
        break;
    case ESP_GAP_BLE_SET_PKT_LENGTH_COMPLETE_EVT:
        ESP_LOGI(GATTS_TAG, "Packet length update, status %d, rx %d, tx %d",
                 param->pkt_data_length_cmpl.status,
                 param->pkt_data_length_cmpl.params.rx_len,
                 param->pkt_data_length_cmpl.params.tx_len);
        break;
    default:
        break;
    }
}

// Application entry point

void app_main(void)
{
    esp_err_t ret;

    // Initialize NVS.
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

#if CONFIG_EXAMPLE_CI_PIPELINE_ID
    memcpy(device_name, esp_bluedroid_get_example_name(), ESP_BLE_ADV_NAME_LEN_MAX);
#endif

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);
    if (ret)
    {
        ESP_LOGE(GATTS_TAG, "%s initialize controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);
    if (ret)
    {
        ESP_LOGE(GATTS_TAG, "%s enable controller failed: %s", __func__, esp_err_to_name(ret));
        return;
    }

    ret = esp_bluedroid_init();
    if (ret)
    {
        ESP_LOGE(GATTS_TAG, "%s init bluetooth failed: %s", __func__, esp_err_to_name(ret));
        return;
    }
    ret = esp_bluedroid_enable();
    if (ret)
    {
        ESP_LOGE(GATTS_TAG, "%s enable bluetooth failed: %s", __func__, esp_err_to_name(ret));
        return;
    }
    // Note: Avoid performing time-consuming operations within cb functions.
    ret = esp_ble_gatts_register_cb(gatts_event_handler);
    if (ret)
    {
        ESP_LOGE(GATTS_TAG, "gatts register error, error code = %x", ret);
        return;
    }
    ret = esp_ble_gap_register_cb(gap_event_handler);
    if (ret)
    {
        ESP_LOGE(GATTS_TAG, "gap register error, error code = %x", ret);
        return;
    }
    ret = esp_ble_gatts_app_register(0);
    if (ret)
    {
        ESP_LOGE(GATTS_TAG, "gatts app register error, error code = %x", ret);
        return;
    }
    esp_err_t local_mtu_ret = esp_ble_gatt_set_local_mtu(500);
    if (local_mtu_ret)
    {
        ESP_LOGE(GATTS_TAG, "set local  MTU failed, error code = %x", local_mtu_ret);
    }

    return;
}