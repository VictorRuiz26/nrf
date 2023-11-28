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

#include <stdio.h>

#include <stdlib.h> 
#include <string.h>

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

#include "nrf_ble_lesc.h"
#include "nrf_ble_scan.h"

#include <time.h>

#include "ble.h"
#include "ble_gap.h"
#include "ble_hci.h"

#define FILE_NAME   "pruebaLog.txt"

// ######## VARIABLES PARA ESCANEO
#define APP_BLE_CONN_CFG_TAG        1                                   /**< Tag that refers to the BLE stack configuration set with @ref sd_ble_cfg_set. The default tag is @ref BLE_CONN_CFG_TAG_DEFAULT. */
#define APP_BLE_OBSERVER_PRIO       3                                   /**< BLE observer priority of the application. There is no need to modify this value. */
#define NRF_BLE_SCAN_INTERVAL               0x00A0                              /**< Determines scan interval in units of 0.625 millisecond. */
#define NRF_BLE_SCAN_WINDOW                 0x2580                              /**< Determines scan window in units of 0.625 millisecond. --> 6 segundos 6segundos×1,000,000microsegundos/segundo/625microsegundos/unidad=9600unidades */
#define SCAN_DURATION_WHITELIST             0x0000                              /**< Duration of the scanning in units of 10 milliseconds. If set to 0x0000, scanning will continue until it is explicitly disabled. */

//static  ble_gap_scan_params_t m_scan_param;   /**< Scan parameters requested for scanning and connection. */
static uint8_t  m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_MIN]; /**< buffer where advertising reports will be stored by the SoftDevice. */

/**@brief Pointer to the buffer where advertising reports will be stored by the SoftDevice. */
static ble_data_t m_scan_buffer =
{
    m_scan_buffer_data,
    BLE_GAP_SCAN_BUFFER_MIN
};

static bool     m_memory_access_in_progress; 

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

// VARIABLE PARA COMPROBAR UUID DEL ADVERTISEMENT
#define APP_BEACON_UUID_POINTER        ((uint8_t[]){ 0x11, 0x11, 0x11, 0x11, \
                                                    0x11, 0x11, 0x11, 0x11, \
                                                    0x11, 0x11, 0x11, 0x11, \
                                                    0x11, 0x11, 0x11, 0x11 })

#define DEAD_BEEF                       0xDEADBEEF                         /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

#if defined(USE_UICR_FOR_MAJ_MIN_VALUES)
#define MAJ_VAL_OFFSET_IN_BEACON_INFO   18                                 /**< Position of the MSB of the Major Value in m_beacon_info array. */
#define UICR_ADDRESS                    0x10001080                         /**< Address of the UICR register used by this example. The major and minor versions to be encoded into the advertising data will be picked up from this location. */
#endif

uint8_t testCallback();

static ble_gap_adv_params_t m_adv_params;                                  /**< Parameters to be passed to the stack when starting advertising. */
static uint8_t              m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET; /**< Advertising handle used to identify an advertising set. */
static uint8_t              m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];  /**< Buffer for storing an encoded advertising set. */

static ble_gap_scan_params_t const m_scan_param =
{
 .active = 0x01,
 .interval = NRF_BLE_SCAN_INTERVAL,
 .window = NRF_BLE_SCAN_WINDOW,
 //.filter_policy = BLE_GAP_SCAN_FP_WHITELIST,
 .timeout = SCAN_DURATION_WHITELIST,
 .scan_phys = BLE_GAP_PHY_1MBPS,
 //.extended = true,
};

NRF_BLE_SCAN_DEF(m_scan); 

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

    // Declarar una variable para almacenar el UUID
    uint8_t uuid_data[16];

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_REPORT:
            // Se ha comprobado que los advertisements requeridos son de longitud 30, 
            // aunque quizás con filtrar únicamente por el UUID sea suficiente
            if (p_adv_report->data.len == 30)
            {
                // Extract and print UUID if it follows 0xFF in the advertisement
                uint8_t *p_data = p_adv_report->data.p_data;
                uint8_t data_len = p_adv_report->data.len;

                for (int i = 0; i < data_len - 1; ++i)
                {
                    if (p_data[i] == 0x15 && (i + 17) < data_len) // Assuming UUID is 16 bytes
                    {
                        //NRF_LOG_INFO("UUID in advertisement:");
                        //NRF_LOG_RAW_HEXDUMP_INFO(&p_data[i + 1], 16);

                        // Copiar el UUID a la variable
                        memcpy(uuid_data, &p_data[i + 1], 16);
                        break;
                    }
                }

                // Imprimir la variable que contiene el UUID
                //NRF_LOG_INFO("UUID stored in variable:");
                //NRF_LOG_RAW_HEXDUMP_INFO(uuid_data, 16);

                //NRF_LOG_INFO(" ------- ");

                // Comparar el UUID almacenado con el UUID definido
                if (memcmp(uuid_data, APP_BEACON_UUID_POINTER, sizeof(uuid_data)) == 0)
                {               
    
                    //FILE *archivo = fopen("pruebaLog.txt", "w");  // Abre el archivo para escribir (sobrescribe si ya existe)

                    NRF_LOG_INFO("************************************************************");
                    NRF_LOG_INFO("UUID matches the defined UUID!");
                    NRF_LOG_INFO("Advertising packet received (length: %d):", p_adv_report->data.len);
                    printf("Advertising packet received (length: %d):\n\r", p_adv_report->data.len);
                    //fprintf(archivo, "Advertising packet received (length: %d):\n\r", p_adv_report->data.len);
                    //NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);
                    NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);  
                    for (int i = 0; i < p_adv_report->data.len; i++) {
                      printf("%02X ", p_adv_report->data.p_data[i]);
                      //fprintf(archivo, "%02X ", p_adv_report->data.p_data[i]);
                    }
                    printf("\n\r");
                    //fprintf(archivo, "\n\r");
                    NRF_LOG_INFO("************************************************************");
                    printf("************************************************************\n\r");
                    //fprintf(archivo, "************************************************************\n\r");
                }
                else
                {
                    NRF_LOG_INFO("UUID does not match the defined UUID.");
                }                           
                
            
            }

            // Continue scanning.
            sd_ble_gap_scan_start(NULL, &m_scan_buffer);
            break;
        default:
            break;
    }
}

//scanning functionality for the lighthouse
static void scan_evt_handler(scan_evt_t const * p_scan_evt)
{
    ret_code_t err_code;
    int rx_rssi;
    switch(p_scan_evt->scan_evt_id)
    {
        case NRF_BLE_SCAN_EVT_CONNECTING_ERROR:
            err_code = p_scan_evt->params.connecting_err.err_code;
            APP_ERROR_CHECK(err_code);
            break;
        case NRF_BLE_SCAN_EVT_FILTER_MATCH:
          rx_rssi = p_scan_evt -> params.p_not_found-> rssi;
          NRF_LOG_INFO("device detected, RSSI %d", rx_rssi);
          break;
        default:
          break;
    }
}

// Definir el UUID que deseas filtrar (todos los bits en 1)
static ble_uuid128_t m_adv_uuid =
{
    .uuid128 = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11}
};
// Declaración de whitelisted_uuids como un arreglo de uint8_t con el mismo tamaño que APP_BEACON_UUID
//static uint8_t whitelisted_uuids[] = { APP_BEACON_UUID };

/**@brief Function to start scanning. */
static void scan_start(void)
{
    ret_code_t err_code;

    /*
    m_scan_param.active         = 1;
    m_scan_param.interval       = SCAN_INTERVAL;
    m_scan_param.window         = SCAN_WINDOW;
    m_scan_param.timeout        = SCAN_DURATION;
    m_scan_param.scan_phys      = BLE_GAP_PHY_1MBPS;
    m_scan_param.filter_policy  = BLE_GAP_SCAN_FP_ACCEPT_ALL; */

    nrf_ble_scan_init_t scan_init;

    memset(&scan_init, 0, sizeof(scan_init));
    scan_init.p_scan_param = &m_scan_param;

    err_code = nrf_ble_scan_init(&m_scan, &scan_init, scan_evt_handler);
    APP_ERROR_CHECK(err_code);

    // Setting filters for scanning.
    // Enable the filter on the name and UUID in the normal mode.
    err_code = nrf_ble_scan_filters_enable(&m_scan, NRF_BLE_SCAN_UUID_FILTER, false);
    err_code = nrf_ble_scan_filter_set(&m_scan,
                                       SCAN_UUID_FILTER,
                                       &m_adv_uuid);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_ble_scan_start(&m_scan);
    APP_ERROR_CHECK(err_code);
}

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
    scan_start();

    // Enter main loop.
    for (;;)
    {
        NRF_LOG_PROCESS();
    }
}


/**
 * @}
 */