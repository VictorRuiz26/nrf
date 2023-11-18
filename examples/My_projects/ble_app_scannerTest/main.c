

/**
 * Copyright (c) 2014 - 2021, Nordic Semiconductor ASA
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form, except as embedded into a Nordic
 *    Semiconductor ASA integrated circuit in a product or a software update for
 *    such product, must reproduce the above copyright notice, this list of
 *    conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * 3. Neither the name of Nordic Semiconductor ASA nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * 4. This software, with or without modification, must only be used with a
 *    Nordic Semiconductor ASA integrated circuit.
 *
 * 5. Any software provided in binary form under this license must not be reverse
 *    engineered, decompiled, modified and/or disassembled.
 *
 * THIS SOFTWARE IS PROVIDED BY NORDIC SEMICONDUCTOR ASA "AS IS" AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL NORDIC SEMICONDUCTOR ASA OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/** @file
 *
 * @defgroup ble_sdk_app_beacon_main main.c
 * @{
 * @ingroup ble_sdk_app_beacon
 * @brief Beacon Transmitter Sample Application main file.
 *
 * This file contains the source code for an Beacon transmitter sample application.
 */

#include <stdbool.h>
#include <stdint.h>
#include "nordic_common.h"
#include "bsp.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "ble_advdata.h"
#include "app_timer.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include <time.h>
#include "nrf_ble_scan.h"

#include "ble.h"
#include "ble_gap.h"
#include "ble_hci.h"

// ######## VARIABLES PARA ESCANEO
#define APP_BLE_CONN_CFG_TAG        1                                   /**< Tag that refers to the BLE stack configuration set with @ref sd_ble_cfg_set. The default tag is @ref BLE_CONN_CFG_TAG_DEFAULT. */
#define APP_BLE_OBSERVER_PRIO       3                                   /**< BLE observer priority of the application. There is no need to modify this value. */
#define SCAN_INTERVAL               0x00A0                              /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW                 0x0050                              /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_DURATION               0x0000                              /**< Duration of the scanning in units of 10 milliseconds. If set to 0x0000, scanning will continue until it is explicitly disabled. */

static  nrf_ble_scan_t m_scan;
static uint8_t  m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_MIN]; /**< buffer where advertising reports will be stored by the SoftDevice. */

/**@brief Pointer to the buffer where advertising reports will be stored by the SoftDevice. */
static ble_data_t m_scan_buffer =
{
    m_scan_buffer_data,
    BLE_GAP_SCAN_BUFFER_MIN
};

#define APP_BLE_CONN_CFG_TAG            1                                  /**< A tag identifying the SoftDevice BLE configuration. */

// ################# VALOR QUE INDICA EL INTERVALO DE ADVERTISEMENT
//#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(100, UNIT_0_625_MS)  /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s). */
#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(500, UNIT_0_625_MS)

#define APP_BEACON_INFO_LENGTH          0x17                               /**< Total length of information advertised by the Beacon. */
#define APP_ADV_DATA_LENGTH             0x15                               /**< Length of manufacturer specific data in the advertisement. */
#define APP_DEVICE_TYPE                 0x02                               /**< 0x02 refers to Beacon. */
#define APP_MEASURED_RSSI               0xC3                               /**< The Beacon's measured RSSI at 1 meter distance in dBm. */
#define APP_COMPANY_IDENTIFIER          0x0059                             /**< Company identifier for Nordic Semiconductor ASA. as per www.bluetooth.org. */
//#define APP_MAJOR_VALUE                 0x01, 0x02                         /**< Major value used to identify Beacons. */
//#define APP_MINOR_VALUE                 0x03, 0x04                         /**< Minor value used to identify Beacons. */
/*#define APP_BEACON_UUID                 0x01, 0x12, 0x23, 0x34, \
                                        0x45, 0x56, 0x67, 0x78, \
                                        0x89, 0x9a, 0xab, 0xbc, \
                                        0xcd, 0xde, 0xef, 0xf0            /**< Proprietary UUID for Beacon. */

// ################# MODIFICACIÓN DEL UUID DEL BEACON PARA QUE SEA TODO 1's y cambios MAJOR Y MINOR
#define APP_MAJOR_VALUE                 0x00, 0x00                         /**< Major value used to identify Beacons. */
#define APP_MINOR_VALUE                 0x00, 0x00                         /**< Minor value used to identify Beacons. */

// definición de variables de major y minor -> modificables en cada bloque de advertisements
//uint16_t APP_MAJOR_VALUE = 0;
//uint16_t APP_MINOR_VALUE = 1;

// Posición de dichos valores en la estructura de advertisements
#define APP_MAJOR_POSITION              19
#define APP_MINOR_POSITION              21

#define APP_BEACON_UUID                 0x11, 0x11, 0x11, 0x11, \
                                        0x11, 0x11, 0x11, 0x11, \
                                        0x11, 0x11, 0x11, 0x11, \
                                        0x11, 0x11, 0x11, 0x11            /**< Proprietary UUID for Beacon. */

#define DEAD_BEEF                       0xDEADBEEF                         /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#if defined(USE_UICR_FOR_MAJ_MIN_VALUES)
#define MAJ_VAL_OFFSET_IN_BEACON_INFO   18                                 /**< Position of the MSB of the Major Value in m_beacon_info array. */
#define UICR_ADDRESS                    0x10001080                         /**< Address of the UICR register used by this example. The major and minor versions to be encoded into the advertising data will be picked up from this location. */
#endif

uint8_t testCallback();

static ble_gap_adv_params_t m_adv_params;                                  /**< Parameters to be passed to the stack when starting advertising. */
static uint8_t              m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET; /**< Advertising handle used to identify an advertising set. */
static uint8_t              m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];  /**< Buffer for storing an encoded advertising set. */

/**@brief Callback function for asserts in the SoftDevice.
 *
 * @details This function will be called in case of an assert in the SoftDevice.
 *
 * @warning This handler is an example only and does not fit a final product. You need to analyze
 *          how your product is supposed to react in case of Assert.
 * @warning On assert from the SoftDevice, the system can only recover on reset.
 *
 * @param[in]   line_num   Line number of the failing ASSERT call.
 * @param[in]   file_name  File name of the failing ASSERT call.
 */
void assert_nrf_callback(uint16_t line_num, const uint8_t * p_file_name)
{
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
}

/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */

/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    ret_code_t            err_code;
    ble_gap_evt_adv_report_t const * p_adv_report = &p_ble_evt->evt.gap_evt.params.adv_report;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_REPORT:

            if (p_adv_report->type.scan_response)
            {
                if (p_adv_report->data.len > 0)
                {
                    NRF_LOG_INFO("Scan response received:");
                    NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);
                }
                else
                {
                    NRF_LOG_INFO("Empty scan response received.");
                }
            }
            else
            {
                NRF_LOG_INFO("Advertising packet received:");
                NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);
            }

            // Continue scanning.
            sd_ble_gap_scan_start(NULL, &m_scan_buffer);
            break;
        default:
            break;
    }
}

/**@brief Function to start scanning. */
/*static void scan_start(void)
{
    ret_code_t ret;

    m_scan_param.active         = 1;
    m_scan_param.interval       = SCAN_INTERVAL;
    m_scan_param.window         = SCAN_WINDOW;
    m_scan_param.timeout        = SCAN_DURATION;
    m_scan_param.scan_phys      = BLE_GAP_PHY_1MBPS;
    m_scan_param.filter_policy  = BLE_GAP_SCAN_FP_WHITELIST;

    ret = sd_ble_gap_scan_start(&m_scan_param, &m_scan_buffer);
    APP_ERROR_CHECK(ret);
}*/

static ble_gap_scan_params_t const m_scan_param =
{
 .active = 0x01,
 .interval = SCAN_INTERVAL,
 .window = SCAN_WINDOW,
 .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
 .timeout = SCAN_DURATION,
 .scan_phys = BLE_GAP_PHY_1MBPS
};

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
static void ble_stack_init(void)
{
    ret_code_t err_code;

    err_code = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err_code);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err_code = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err_code);

    // Enable BLE stack.
    err_code = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err_code);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(m_ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler, NULL);
}

/**@brief Function for initializing logging. */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Function for initializing LEDs. */
static void leds_init(void)
{
    ret_code_t err_code = bsp_init(BSP_INIT_LEDS, NULL);
    APP_ERROR_CHECK(err_code);
}





/*
static void nrf_ble_scan_default_param_set(nrf_ble_scan_t * const p_scan_ctx)
 {
     // Set the default parameters.
     p_scan_ctx->scan_params.active        = 1;
     p_scan_ctx->scan_params.interval      = SCAN_INTERVAL;
     p_scan_ctx->scan_params.window        = SCAN_WINDOW;
     p_scan_ctx->scan_params.timeout       = SCAN_DURATION;
     p_scan_ctx->scan_params.filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL;
     p_scan_ctx->scan_params.scan_phys     = BLE_GAP_PHY_1MBPS;
 }

static void nrf_ble_scan_default_conn_param_set(nrf_ble_scan_t * const p_scan_ctx)
 {
     p_scan_ctx->conn_params.conn_sup_timeout =
         (uint16_t)MSEC_TO_UNITS(NRF_BLE_SCAN_SUPERVISION_TIMEOUT, UNIT_10_MS);
     p_scan_ctx->conn_params.min_conn_interval =
         (uint16_t)MSEC_TO_UNITS(NRF_BLE_SCAN_MIN_CONNECTION_INTERVAL, UNIT_1_25_MS);
     p_scan_ctx->conn_params.max_conn_interval =
         (uint16_t)MSEC_TO_UNITS(NRF_BLE_SCAN_MAX_CONNECTION_INTERVAL, UNIT_1_25_MS);
     p_scan_ctx->conn_params.slave_latency =
         (uint16_t)NRF_BLE_SCAN_SLAVE_LATENCY;

     p_scan_ctx->conn_params.conn_sup_timeout =
         (uint16_t)MSEC_TO_UNITS(50, UNIT_10_MS);
     p_scan_ctx->conn_params.min_conn_interval =
         (uint16_t)MSEC_TO_UNITS(50, UNIT_1_25_MS);
     p_scan_ctx->conn_params.max_conn_interval =
         (uint16_t)MSEC_TO_UNITS(60, UNIT_1_25_MS);
     p_scan_ctx->conn_params.slave_latency =
         (uint16_t)25;
 }


ret_code_t nrf_ble_scan_init(nrf_ble_scan_t            * const p_scan_ctx,
                              nrf_ble_scan_init_t const * const p_init,
                              nrf_ble_scan_evt_handler_t        evt_handler)
 {
     VERIFY_PARAM_NOT_NULL(p_scan_ctx);
  
     p_scan_ctx->evt_handler = evt_handler;
  
 #if (NRF_BLE_SCAN_FILTER_ENABLE == 1)
     // Disable all scanning filters.
     memset(&p_scan_ctx->scan_filters, 0, sizeof(p_scan_ctx->scan_filters));
 #endif
  
     // If the pointer to the initialization structure exist, use it to scan the configuration.
     if (p_init != NULL)
     {
         p_scan_ctx->connect_if_match = p_init->connect_if_match;
         p_scan_ctx->conn_cfg_tag     = p_init->conn_cfg_tag;
  
         if (p_init->p_scan_param != NULL)
         {
             p_scan_ctx->scan_params = *p_init->p_scan_param;
         }
         else
         {
             // Use the default static configuration.
             nrf_ble_scan_default_param_set(p_scan_ctx);
         }
  
         if (p_init->p_conn_param != NULL)
         {
             p_scan_ctx->conn_params = *p_init->p_conn_param;
         }
         else
         {
             // Use the default static configuration.
             nrf_ble_scan_default_conn_param_set(p_scan_ctx);
         }
     }
     // If pointer is NULL, use the static default configuration.
     else
     {
         nrf_ble_scan_default_param_set(p_scan_ctx);
         nrf_ble_scan_default_conn_param_set(p_scan_ctx);
  
         p_scan_ctx->connect_if_match = false;
     }
  
     // Assign a buffer where the advertising reports are to be stored by the SoftDevice.
     p_scan_ctx->scan_buffer.p_data = p_scan_ctx->scan_buffer_data;
     p_scan_ctx->scan_buffer.len    = NRF_BLE_SCAN_BUFFER;
  
     return NRF_SUCCESS;
 }



 ret_code_t nrf_ble_scan_start(nrf_ble_scan_t const * const p_scan_ctx)
 {
     VERIFY_PARAM_NOT_NULL(p_scan_ctx);
  
     ret_code_t err_code;
     scan_evt_t scan_evt;
  
     memset(&scan_evt, 0, sizeof(scan_evt));
  
     nrf_ble_scan_stop();
  
     // If the whitelist is used and the event handler is not NULL, send the whitelist request to the main application.
     if (is_whitelist_used(p_scan_ctx))
     {
         if (p_scan_ctx->evt_handler != NULL)
         {
             scan_evt.scan_evt_id = NRF_BLE_SCAN_EVT_WHITELIST_REQUEST;
             p_scan_ctx->evt_handler(&scan_evt);
         }
     }
  
     // Start the scanning.
     err_code = sd_ble_gap_scan_start(&p_scan_ctx->scan_params, &p_scan_ctx->scan_buffer);
  
     // It is okay to ignore this error, because the scan stopped earlier.
     if ((err_code != NRF_ERROR_INVALID_STATE) && (err_code != NRF_SUCCESS))
     {
         NRF_LOG_ERROR("sd_ble_gap_scan_start returned 0x%x", err_code);
         return (err_code);
     }
     NRF_LOG_DEBUG("Scanning");
  
     return NRF_SUCCESS;
 }
  */
  
 ret_code_t nrf_ble_scan_params_set(nrf_ble_scan_t              * const p_scan_ctx,
                                    ble_gap_scan_params_t const * const p_scan_param)
 {
     VERIFY_PARAM_NOT_NULL(p_scan_ctx);
  
     nrf_ble_scan_stop();
  
     if (p_scan_param != NULL)
     {
         // Assign new scanning parameters.
         p_scan_ctx->scan_params = *p_scan_param;
     }
     else
     {
         // If NULL, use the default static configuration.
         nrf_ble_scan_default_param_set(p_scan_ctx);
     }
  
     NRF_LOG_DEBUG("Scanning parameters have been changed successfully");
  
     return NRF_SUCCESS;
 }











  



/**@brief Function for initializing timers. */
static void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    if (NRF_LOG_PROCESS() == false)
    {
        nrf_pwr_mgmt_run();        
    }
}


/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    log_init();
    timers_init();
    leds_init();
    power_management_init();
    ble_stack_init();

    // Start scanning.
    NRF_LOG_INFO("BLE active scanner started.");

/*  ret_code_t err_code;
    nrf_ble_scan_init_t scan_init;

    memset(&scan_init, 0, sizeof(scan_init));
    scan_init.p_scan_param = &m_scan_param;

    err_code = nrf_ble_scan_init(&m_scan_ble, &scan_init, scan_evt_handler);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_ble_scan_start(&m_scan_ble);
    APP_ERROR_CHECK(err_code);
*/
    ret_code_t err_code;
    err_code = nrf_ble_scan_init(&m_scan, NULL, NULL);
    APP_ERROR_CHECK(err_code);
    err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(err_code);

    // Enter main loop.
    for (;;)
    {
        NRF_LOG_PROCESS();
    }
}


/**
 * @}
 */