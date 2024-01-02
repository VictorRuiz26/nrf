
#include <stdbool.h>
#include <stdint.h>
#include "nordic_common.h"
#include "bsp.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "ble_advdata.h"
#include "app_timer.h"
#include "nrf_drv_clock.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include <time.h>


#define APP_BLE_CONN_CFG_TAG            1                                  /**< A tag identifying the SoftDevice BLE configuration. */

// ################# VALOR QUE INDICA EL INTERVALO DE ADVERTISEMENT
#define NON_CONNECTABLE_ADV_INTERVAL    MSEC_TO_UNITS(ADV_INTERVAL_MS, UNIT_0_625_MS)  /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s */

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

// ********* BUTTON AND LED ASSIGNMENT FOR CONTROL ********

#define SENDING_ADV_LED                 BSP_BOARD_LED_0
#define PHY_SELECTION_LED               BSP_BOARD_LED_1		/**< LED indicating which phy is in use. */
#define OUTPUT_POWER_SELECTION_LED      BSP_BOARD_LED_2		/**< LED indicating at which ouput power the radio is transmitting */
#define NON_CONN_ADV_LED                BSP_BOARD_LED_3         /**< LED indicting if the device is advertising non-connectable advertising or not. */
#define CONN_ADV_CONN_STATE_LED         BSP_BOARD_LED_3         /**< LED indicating that if device is advertising with connectable advertising, in a connected state, or none. */

#define PHY_SELECTION_BUTTON                  BSP_BUTTON_1
#define PHY_SELECTION_BUTTON_EVENT            BSP_EVENT_KEY_1
#define OUTPUT_POWER_SELECTION_BUTTON         BSP_BUTTON_2
#define OUTPUT_POWER_SELECTION_BUTTON_EVENT   BSP_EVENT_KEY_2
#define NON_CONN_OR_CONN_ADV_BUTTON           BSP_BUTTON_3
#define NON_CONN_OR_CONN_ADV_BUTTON_EVENT     BSP_EVENT_KEY_3
#define BUTTON_NOT_IN_USE                     BSP_BUTTON_0
#define BUTTON_NOT_IN_USE_EVENT               BSP_EVENT_KEY_0
// ********************************************************


#define CONN_INTERVAL_DEFAULT           (uint16_t)(MSEC_TO_UNITS(7.5, UNIT_1_25_MS))    /**< Default connection interval used at connection establishment by central side. */

#define CONN_INTERVAL_MIN               (uint16_t)(MSEC_TO_UNITS(7.5, UNIT_1_25_MS))    /**< Minimum acceptable connection interval, in 1.25 ms units. */
#define CONN_INTERVAL_MAX               (uint16_t)(MSEC_TO_UNITS(500, UNIT_1_25_MS))    /**< Maximum acceptable connection interval, in 1.25 ms units. */
#define CONN_SUP_TIMEOUT                (uint16_t)(MSEC_TO_UNITS(8000,  UNIT_10_MS))    /**< Connection supervisory timeout (4 seconds). */
#define SLAVE_LATENCY                   0                                               /**< Slave latency. */


// ******* TIMER DEFINITIONS *******
#define ADV_INTERVAL                    APP_TIMER_TICKS(SEGUNDOS_DELAY*1000)
#define FAST_BLINK_INTERVAL		APP_TIMER_TICKS(200)
#define SLOW_BLINK_INTERVAL		APP_TIMER_TICKS(750)
#define BLINK_SEDNDING_ADV              APP_TIMER_TICKS(100)
    //De momento el modo conectable o no conectable no se contempla
APP_TIMER_DEF(m_non_conn_fast_blink_timer_id);                /**< Timer used to toggle LED indicating non-connectable advertising on the dev. kit. */
APP_TIMER_DEF(m_conn_adv_fast_blink_timer_id);                /**< Timer used to toggle LED indicating connectable advertising on the dev. kit. */

APP_TIMER_DEF(m_1Mbps_led_slow_blink_timer_id);                /**< Timer used to toggle LED for phy selection indication on the dev.kit. */                        
APP_TIMER_DEF(m_8dBm_led_slow_blink_timer_id);                 /**< Timer used to toggle LED for output power selection indication on the dev.kit. */ 
APP_TIMER_DEF(m_timer_ble);
APP_TIMER_DEF(m_adv_sent_led_show_timer_id);
// *********************************

static void advertising_stop(void);
// ******* STRUCTS AND VARIABLES FOR MODE OPERATION ******
// Type holding the two output power options for this application.
typedef enum
{
    SELECTION_0_dBm = 0, 
    SELECTION_8_dBm = 8
} output_power_seclection_t;


// Type holding the two advertising selection modes. --> DE MOMENTO NO SE USA
typedef enum
{
    SELECTION_CONNECTABLE = 0, 
    SELECTION_NON_CONNECTABLE
} adv_scan_type_seclection_t;


// Type holding the two possible phy options.
typedef enum
{
    SELECTION_1M_PHY = 0, 
    SELECTION_CODED_PHY
} adv_scan_phy_seclection_t;

static adv_scan_type_seclection_t   m_adv_scan_type_selected = SELECTION_NON_CONNECTABLE;   /**< Global variable holding the current scan selection mode. */
static adv_scan_phy_seclection_t    m_adv_scan_phy_selected  = SELECTION_1M_PHY;      /**< Global variable holding the current phy selection. */
static output_power_seclection_t    m_output_power_selected  = SELECTION_8_dBm;          /**< Global variable holding the current output power selection. */
static bool    m_app_initiated_disconnect  = false;                //The application has initiated disconnect. Used to "tell" on_ble_gap_evt_disconnected() to not start advertising.
static bool    m_waiting_for_disconnect_evt     = false;          // Disconnect is initiated. The application has to wait for BLE_GAP_EVT_DISCONNECTED before proceeding.

static void set_current_adv_params_and_start_advertising(void);
static void disconnect_stop_adv(void);
// *****************************************


#define APP_BLE_CONN_CFG_TAG            1                                               /**< A tag that refers to the BLE stack configuration. */
#define APP_BLE_OBSERVER_PRIO           3                                               /**< Application's BLE observer priority. You shouldn't need to modify this value. */


static ble_gap_adv_params_t m_adv_params;                                  /**< Parameters to be passed to the stack when starting advertising. */
static uint8_t              m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET; /**< Advertising handle used to identify an advertising set. */
static uint8_t              m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];  /**< Buffer for storing an encoded advertising set. */

/**@brief Struct that contains pointers to the encoded advertising data. */
static ble_gap_adv_data_t m_adv_data =
{
    .adv_data =
    {
        .p_data = m_enc_advdata,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_MAX
    },
    .scan_rsp_data =
    {
        .p_data = NULL,
        .len    = 0

    }
};

static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;    /**< Handle of the current BLE connection .*/
static uint8_t m_gap_role     = BLE_GAP_ROLE_INVALID;       /**< BLE role for this connection, see @ref BLE_GAP_ROLES */



// Connection parameters requested for connection.
static ble_gap_conn_params_t m_conn_param =
{
    .min_conn_interval = CONN_INTERVAL_MIN,   // Minimum connection interval.
    .max_conn_interval = CONN_INTERVAL_MAX,   // Maximum connection interval.
    .slave_latency     = SLAVE_LATENCY,       // Slave latency.
    .conn_sup_timeout  = CONN_SUP_TIMEOUT     // Supervisory timeout.
};


static void instructions_print(void)
{
    NRF_LOG_INFO("Press the buttons to set up the advertiser in wanted mode:");
    NRF_LOG_INFO("Button 2: switch between coded phy and 1Mbps");
    NRF_LOG_INFO("Button 3: switch between 0 dbm and 8 dBm output power.");
    //NRF_LOG_INFO("Button 4: switch between non-connectable (slow blink LED 4) and connectable advertising (fast blink LED4).");
}

static uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH] =                    /**< Information advertised by the Beacon. */
{
    APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                         // implementation.
    APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                         // manufacturer specific data in this implementation.
    APP_BEACON_UUID,     // 128 bit UUID value.
    APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
    APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
    APP_MEASURED_RSSI    // Manufacturer specific information. The Beacon's measured TX power in
                         // this implementation.
};


/**@brief Function for handling BLE_GAP_EVT_CONNECTED events.
 * Save the connection handle and GAP role.
 * Turn on the "connected state" LED.
 */
static void on_ble_gap_evt_connected(ble_gap_evt_t const * p_gap_evt)
{
    ret_code_t err_code;
    
    m_conn_handle = p_gap_evt->conn_handle;
    m_gap_role    = p_gap_evt->params.connected.role;

    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_CONN, m_conn_handle, m_output_power_selected);
    APP_ERROR_CHECK(err_code);
    
    if (m_gap_role == BLE_GAP_ROLE_PERIPH)
    {
        NRF_LOG_INFO("Connected as a peripheral.");
    }
    else if (m_gap_role == BLE_GAP_ROLE_CENTRAL)
    {
        NRF_LOG_INFO("Connected as a central. Should not happen.");
    }
    
    //If advertising, stop advertising.
    (void) sd_ble_gap_adv_stop(m_adv_handle);

    // Stop advertising LED timer, turn on "connected state" LED
    err_code = app_timer_stop(m_conn_adv_fast_blink_timer_id);
    APP_ERROR_CHECK(err_code);
    bsp_board_led_on(CONN_ADV_CONN_STATE_LED);
  
}


/**@brief Function for handling BLE_GAP_EVT_DISCONNECTED events.
 * Unset the connection handle and restart advertising if needed. 
 */
static void on_ble_gap_evt_disconnected(ble_gap_evt_t const * p_gap_evt)
{
    m_conn_handle = BLE_CONN_HANDLE_INVALID;
    m_waiting_for_disconnect_evt = false;

    NRF_LOG_DEBUG("Disconnected: reason 0x%x.", p_gap_evt->params.disconnected.reason);

    if (m_app_initiated_disconnect == false) // (If a the app itself (button push) has initiated a disconnect, bsp_evt_handler will start the advertising.)
    {
      // Start advertising with the current setup.
      bsp_board_leds_off();
      set_current_adv_params_and_start_advertising();
    }

}

/**@brief Function for handling scan request report.
 * Print the RSSI and address of the initiator if the RSSI has changed.
 */
static void on_scan_req_report(ble_gap_evt_scan_req_report_t const * p_scan_req_report)
{
  static int8_t         rssi_value = 0;

  if(rssi_value != p_scan_req_report->rssi)
     {
       rssi_value = p_scan_req_report->rssi;
       NRF_LOG_INFO("Received scan request with RSSI %d .",rssi_value);
       NRF_LOG_INFO("addr %02x:%02x:%02x:%02x:%02x:%02x",
               p_scan_req_report->peer_addr.addr[0],
               p_scan_req_report->peer_addr.addr[1],
               p_scan_req_report->peer_addr.addr[2],
               p_scan_req_report->peer_addr.addr[3],
               p_scan_req_report->peer_addr.addr[4],
               p_scan_req_report->peer_addr.addr[5]);
     }
}

/**@brief Function for handling BLE Stack events.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    
    uint32_t              err_code;
    ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_SET_TERMINATED:
            //disconnect_stop_adv();
            //NRF_LOG_INFO("Advertisement terminado");
            //bsp_board_led_off(SENDING_ADV_LED);
            //err_code = app_timer_stop(m_adv_sent_led_show_timer_id); 
           // APP_ERROR_CHECK(err_code);
            break;
        case BLE_GAP_EVT_CONNECTED:
            on_ble_gap_evt_connected(p_gap_evt);
            break;
        
        case BLE_GAP_EVT_DISCONNECTED:
            on_ble_gap_evt_disconnected(p_gap_evt);
            break;
        
        case BLE_GAP_EVT_CONN_PARAM_UPDATE:
        {
            NRF_LOG_INFO("Connection interval updated: 0x%x, 0x%x.",
                p_gap_evt->params.conn_param_update.conn_params.min_conn_interval,
                p_gap_evt->params.conn_param_update.conn_params.max_conn_interval);
        } break;
        
       case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
       {
           // Accept parameters requested by the peer.
           ble_gap_conn_params_t params;
           params = p_gap_evt->params.conn_param_update_request.conn_params;
           err_code = sd_ble_gap_conn_param_update(p_gap_evt->conn_handle, &params);
           APP_ERROR_CHECK(err_code);
       
           NRF_LOG_INFO("Connection interval updated (upon request): 0x%x, 0x%x.",
               p_gap_evt->params.conn_param_update_request.conn_params.min_conn_interval,
               p_gap_evt->params.conn_param_update_request.conn_params.max_conn_interval);
       } break;
       
       case BLE_GATTS_EVT_SYS_ATTR_MISSING:
       {
           NRF_LOG_DEBUG("BLE_GATTS_EVT_SYS_ATTR_MISSING");
           err_code = sd_ble_gatts_sys_attr_set(p_gap_evt->conn_handle, NULL, 0, 0);
           APP_ERROR_CHECK(err_code);
       } break;
       
       case BLE_GATTC_EVT_TIMEOUT: // Fallthrough.
       case BLE_GATTS_EVT_TIMEOUT:
       {
           NRF_LOG_DEBUG("GATT timeout, disconnecting.");
           err_code = sd_ble_gap_disconnect(m_conn_handle,
                                            BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
           APP_ERROR_CHECK(err_code);
       } break;
       
       case BLE_GAP_EVT_PHY_UPDATE:
       {
            NRF_LOG_DEBUG("BLE_GAP_EVT_PHY_UPDATE: not implemented.");
           
       } break;
       
       case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
       { 
           NRF_LOG_DEBUG("BLE_GAP_EVT_PHY_UPDATE_REQUEST: not implemented.");
       } break;

       case BLE_GAP_EVT_SCAN_REQ_REPORT:
       {
          //on_scan_req_report(&p_gap_evt->params.scan_req_report);
       } break;

        default:
        {
          NRF_LOG_DEBUG("Received an unimplemented BLE event.");
            // No implementation needed.
        } break;
    }
}

/**@brief This function will disconnect if connected, and stop advertising if advertising. */
static void disconnect_stop_adv(void)
{
  ret_code_t err_code;
  // If advertising, stop advertising.
        (void) sd_ble_gap_adv_stop(m_adv_handle);
        
        // If connected, disconnect.
	if(m_conn_handle != BLE_CONN_HANDLE_INVALID)
	{
	  NRF_LOG_INFO("Disconnecting...");
	  
	  m_app_initiated_disconnect = true;
          m_waiting_for_disconnect_evt    = true;
	  err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
	  
	  if (err_code != NRF_SUCCESS)
	  {
                   NRF_LOG_ERROR("disconnect_stop_adv, sd_ble_gap_disconnect() failed: 0x%0x.", err_code);
                   m_app_initiated_disconnect = false;
                   m_waiting_for_disconnect_evt    = false;
	  }
          while (m_waiting_for_disconnect_evt == true)
          {
           // Wait until BLE_GAP_EVT_DISCONNECT has occured.
          }
         
	}
}


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
static void advertising_init(void)
{
    ret_code_t ret;

    ble_advdata_manuf_data_t manuf_specific_data;

    manuf_specific_data.company_identifier = APP_COMPANY_IDENTIFIER;

#if defined(USE_UICR_FOR_MAJ_MIN_VALUES)
    // If USE_UICR_FOR_MAJ_MIN_VALUES is defined, the major and minor values will be read from the
    // UICR instead of using the default values. The major and minor values obtained from the UICR
    // are encoded into advertising data in big endian order (MSB First).
    // To set the UICR used by this example to a desired value, write to the address 0x10001080
    // using the nrfjprog tool. The command to be used is as follows.
    // nrfjprog --snr <Segger-chip-Serial-Number> --memwr 0x10001080 --val <your major/minor value>
    // For example, for a major value and minor value of 0xabcd and 0x0102 respectively, the
    // the following command should be used.
    // nrfjprog --snr <Segger-chip-Serial-Number> --memwr 0x10001080 --val 0xabcd0102
    uint16_t major_value = ((*(uint32_t *)UICR_ADDRESS) & 0xFFFF0000) >> 16;
    uint16_t minor_value = ((*(uint32_t *)UICR_ADDRESS) & 0x0000FFFF);

    uint8_t index = MAJ_VAL_OFFSET_IN_BEACON_INFO;

    m_beacon_info[index++] = MSB_16(major_value);
    m_beacon_info[index++] = LSB_16(major_value);

    m_beacon_info[index++] = MSB_16(minor_value);
    m_beacon_info[index++] = LSB_16(minor_value);
#endif

    m_beacon_info[APP_MINOR_POSITION] += 1;

    if (m_beacon_info[APP_MINOR_POSITION] == 0) {
      // Incrementa el MSB minor solo si el LSB minor ha desbordado
      m_beacon_info[APP_MINOR_POSITION-1] += 1;
      if (m_beacon_info[APP_MINOR_POSITION-1] == 0) {
          m_beacon_info[APP_MAJOR_POSITION] += 1;
          if (m_beacon_info[APP_MAJOR_POSITION] == 0) {
            m_beacon_info[APP_MAJOR_POSITION-1] += 1;
          }
      }
    }

    manuf_specific_data.data.p_data = (uint8_t *) m_beacon_info;
    manuf_specific_data.data.size   = APP_BEACON_INFO_LENGTH;

    ble_gap_adv_params_t adv_params =
    {
        .properties    =
        {
          .type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED,
        },
        .p_peer_addr   = NULL,
        .filter_policy = BLE_GAP_ADV_FP_ANY,
        .interval      = ADV_INTERVAL,
        .duration      = 0,
        .max_adv_evts  = NUM_ADVERTISEMENTS,

        .primary_phy   = BLE_GAP_PHY_1MBPS, // Must be changed to connect in long range. (BLE_GAP_PHY_CODED)
        .secondary_phy = BLE_GAP_PHY_1MBPS,
        .scan_req_notification = 1,
    };

    // Build and set advertising data.
    ble_advdata_t const adv_data =
    {
        .name_type          = BLE_ADVDATA_NO_NAME,
        .flags              = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED,
        .include_appearance = false,
        .p_manuf_specific_data = &manuf_specific_data,
    };

    //Now, parameters of advertisements are setted using variables modified through bsp buttons
    if(m_adv_scan_phy_selected == SELECTION_1M_PHY)
    {
        // 1M coded for adv
   //     NRF_LOG_INFO("Setting adv params PHY to 1M. ");
        adv_params.primary_phy     = BLE_GAP_PHY_1MBPS;
        adv_params.secondary_phy   = BLE_GAP_PHY_1MBPS;
        
        if(m_adv_scan_type_selected == SELECTION_CONNECTABLE)
        {
   //         NRF_LOG_INFO("Advertising type set to CONNECTABLE_SCANNABLE_UNDIRECTED ");
            adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
        }
        else if(m_adv_scan_type_selected == SELECTION_NON_CONNECTABLE)
        {
   //         NRF_LOG_INFO("Advertising type set to NONCONNECTABLE_SCANNABLE_UNDIRECTED ");
            adv_params.properties.type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_SCANNABLE_UNDIRECTED;
        }
        
        ret = ble_advdata_encode(&adv_data, m_adv_data.adv_data.p_data, &m_adv_data.adv_data.len);
        APP_ERROR_CHECK(ret);

        ret = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &adv_params);
        APP_ERROR_CHECK(ret);
    }

    else if(m_adv_scan_phy_selected == SELECTION_CODED_PHY)
    {
        // only extended advertising will allow primary phy to be coded
   //     NRF_LOG_INFO("Setting adv params phy to coded phy .. ");
        adv_params.primary_phy     = BLE_GAP_PHY_CODED;
        adv_params.secondary_phy   = BLE_GAP_PHY_CODED;
        
        if(m_adv_scan_type_selected == SELECTION_CONNECTABLE)
        {
   //         NRF_LOG_INFO("Advertising type set to EXTENDED_CONNECTABLE_NONSCANNABLE_UNDIRECTED ");
            adv_params.properties.type = BLE_GAP_ADV_TYPE_EXTENDED_CONNECTABLE_NONSCANNABLE_UNDIRECTED;
                      
            ret = ble_advdata_encode(&adv_data, m_adv_data.adv_data.p_data, &m_adv_data.adv_data.len);
            APP_ERROR_CHECK(ret);

            ret = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &adv_params);
            APP_ERROR_CHECK(ret);
        }
        else if(m_adv_scan_type_selected == SELECTION_NON_CONNECTABLE)
        {
    //        NRF_LOG_INFO("Advertising type set to EXTENDED_NONCONNECTABLE_SCANNABLE_UNDIRECTED ");
            adv_params.properties.type = BLE_GAP_ADV_TYPE_EXTENDED_NONCONNECTABLE_SCANNABLE_UNDIRECTED;


            // ret = ble_advdata_encode(&adv_data, m_adv_data_ext.scan_rsp_data.p_data, &m_adv_data_ext.scan_rsp_data.len);
            // APP_ERROR_CHECK(ret);

            ret = sd_ble_gap_adv_set_configure(&m_adv_handle, NULL, &adv_params);
            NRF_LOG_INFO("err_code after sd_ble_gap_adv_set_configure in advertising_init: %d", ret); 
            APP_ERROR_CHECK(ret);			
        }
    }
}

/**@brief Function for starting advertising.
 */
static void advertising_start(void)
{
    ret_code_t err_code;

    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, m_adv_handle, m_output_power_selected);
    APP_ERROR_CHECK(err_code);

    //TODO: Esto casca cuando pongo modo connectable no sé por qué!
    err_code = sd_ble_gap_adv_start(m_adv_handle, APP_BLE_CONN_CFG_TAG);
    //NRF_LOG_INFO("err_code after sd_ble_gap_adv_start: %d", err_code); 
    APP_ERROR_CHECK(err_code);    

    //TODO: implementar timer de advertiser que maneje el led 1 rápido mientras se estén enviando
   // err_code = app_timer_start(m_adv_sent_led_show_timer_id, BLINK_SEDNDING_ADV, NULL);
   // APP_ERROR_CHECK(err_code);

    m_app_initiated_disconnect = false;

}

/**@brief Function for stopping advertising.
 */
static void advertising_stop(void)
{
    ret_code_t err_code;

    err_code = sd_ble_gap_adv_stop(m_adv_handle);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Stopping advertising");

    //err_code = sd_ble_gap_adv_start(m_adv_handle, APP_BLE_CONN_CFG_TAG);
    //APP_ERROR_CHECK(err_code);
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


// ************** FUNCTIONS FOR SETTINGS THE VARIABLES AND THEIR RESPECTIVE ON_EVENT HANDLERS ****************

/**@brief Function for setting new output power selection: LEDs start to blink according to the selected state.
 * Note: this function sets the output power/TX power.
 *       The new output power will be set when (re-)starting to scan.
 */
static void output_power_selection_set(output_power_seclection_t output_power)
{
 ret_code_t err_code;
 m_output_power_selected = output_power;
  switch ( m_output_power_selected)
  {
    case SELECTION_8_dBm: 
    {
        NRF_LOG_INFO("Power selection set to 8dBm");
    	// 8 dBm is the current output power, start blinking LED.
        bsp_board_led_off(OUTPUT_POWER_SELECTION_LED); // not necessary because the LED should start to blink. 
        err_code = app_timer_start(m_8dBm_led_slow_blink_timer_id, SLOW_BLINK_INTERVAL, NULL);
    	APP_ERROR_CHECK(err_code);
    	
    	
    	
    } break;
    
    case SELECTION_0_dBm:
    {
        NRF_LOG_INFO("Power selection set to 0dBm");
    	// 0 dBm is the current output power, turn on LED.
        err_code = app_timer_stop(m_8dBm_led_slow_blink_timer_id); 
    	APP_ERROR_CHECK(err_code);
    	
    	bsp_board_led_on(OUTPUT_POWER_SELECTION_LED);

    } break;
  }
}


/**@brief Function for switching output power/TX power: 0 dBm or 8 dBm.
 * Note:  this function does only set the internal state, it does apply the new setting. 
 *        The new output power will be set when (re-)starting to scan.
 */
static void on_output_power_selection_button(void)
{
	// Change the output power.
  output_power_seclection_t new_output_power;
  switch ( m_output_power_selected)
  {
    case SELECTION_0_dBm:  // 0 dBm is the previous output power.
    {
    	// 8 dBm is the current output power, start blinking LED.
    	new_output_power = SELECTION_8_dBm; 
    	
    } break;
    
    case SELECTION_8_dBm:
    {
    	// 0 dBm is the current output power, turn on LED.
    	new_output_power = SELECTION_0_dBm;   	
    	
    } break;
    
  }
  output_power_selection_set(new_output_power);
}

/**@brief Function for setting new phy selection. LEDs start to blink according to the selected state.
 * Note: this function does only set the internal state, it does apply the new setting.
 *       The new phy will be be used when (re-)starting scanning.
 */
static void phy_selection_set_state(adv_scan_phy_seclection_t new_phy_selection)
{
  ret_code_t err_code;
  m_adv_scan_phy_selected = new_phy_selection;


  switch (new_phy_selection)
  {
    case SELECTION_1M_PHY:  // SELECTION_1M_PHY is the current "state".
    {
      // 1 Mbps is the current state, LED should start blinking.
      NRF_LOG_INFO("PHY mode changed to 1M");
      err_code = app_timer_start(m_1Mbps_led_slow_blink_timer_id, SLOW_BLINK_INTERVAL, NULL);
      APP_ERROR_CHECK(err_code);
    } break;
    
    case SELECTION_CODED_PHY:
    {
      // Coded phy is the current sate, turn on LED.
      NRF_LOG_INFO("PHY mode changed to Codec");
      err_code = app_timer_stop(m_1Mbps_led_slow_blink_timer_id); 
      APP_ERROR_CHECK(err_code);
      bsp_board_led_on(PHY_SELECTION_LED);
    } break;
  }

}

/**@brief Function for switching PHY: coded phy or 1 Mbps
 * Note: this function does only set the internal state, it does apply the new setting.
 *       The new phy will be be used when (re-)starting scanning.
 */
static void on_phy_selection_button(void)
{
  // Change the selected phy.
  adv_scan_phy_seclection_t new_phy_selection;
  switch (m_adv_scan_phy_selected)
  {
    case SELECTION_CODED_PHY:  // SELECTION_CODED_PHY is the previous "state".
    {
      // 1 Mbps is the current state, LED should start blinking.
      new_phy_selection = SELECTION_1M_PHY; 
    } break;
    
    case SELECTION_1M_PHY:
    {
      // Coded phy is the current sate, turn on LED.
      new_phy_selection = SELECTION_CODED_PHY;
    } break;
    

  }
  phy_selection_set_state(new_phy_selection);
}

/**@brief Function setting the internal advertising state.
 * Note: this function does only set the internal "advertising state", it does not start advertising.
 *       The new advertising mode will be used when (re-)starting to advertise.
 */
static void on_non_conn_or_conn_adv_selection_state_set(adv_scan_type_seclection_t adv_selection)
{
 ret_code_t err_code;

  m_adv_scan_type_selected = adv_selection;
  bsp_board_led_off(CONN_ADV_CONN_STATE_LED);
  switch (adv_selection)
  {
    case SELECTION_NON_CONNECTABLE: 
    {
      // Current state is non-connectable advertising. Start blinking associated LED.
      NRF_LOG_INFO("Conn mode changed to NON_CONNECTABLE");
      err_code = app_timer_stop(m_conn_adv_fast_blink_timer_id); 
      APP_ERROR_CHECK(err_code);
      bsp_board_led_off(CONN_ADV_CONN_STATE_LED);

      //err_code = app_timer_start(m_non_conn_fast_blink_timer_id, SLOW_BLINK_INTERVAL, NULL);
      //APP_ERROR_CHECK(err_code);
       bsp_board_led_off(NON_CONN_ADV_LED); //Always off, not using timer
    } break;
    
    case SELECTION_CONNECTABLE:
    {
      // Current state is connectable advertising. Start blinking associated LED.
      NRF_LOG_INFO("Conn mode changed to CONNECTABLE");
      err_code = app_timer_stop(m_non_conn_fast_blink_timer_id); 
      APP_ERROR_CHECK(err_code);
      bsp_board_led_off(NON_CONN_ADV_LED);


      err_code = app_timer_start(m_conn_adv_fast_blink_timer_id, FAST_BLINK_INTERVAL, NULL);
      APP_ERROR_CHECK(err_code);

    } break;
  }



}

/**@brief Function for switching between "non-connectable and connectable advertising" selection.
 * Note: this function does only set the internal "advertising state", it does not start advertising.
 *       The new advertising mode will be used when (re-)starting to advertise.
 */
static void on_non_conn_or_conn_adv_selection(void)
{
 // Change the advertising type
  adv_scan_type_seclection_t new_adv_selection;

  switch (m_adv_scan_type_selected)
  {
    case SELECTION_CONNECTABLE:  // Connectable advertising is the previous state.
    {
      // Current state is non-connectable advertising. Start blinking associated LED.
      new_adv_selection = SELECTION_NON_CONNECTABLE; 

    } break;
    
    case SELECTION_NON_CONNECTABLE:
    {
      // Current state is connectable advertising. Start blinking associated LED.
      new_adv_selection = SELECTION_CONNECTABLE;

    } break;
  }
  on_non_conn_or_conn_adv_selection_state_set(new_adv_selection);

}
// ***********************************************************************************************************

/**@brief Function for handling events from the button handler module.
 *
 * @param[in] pin_no        The pin that the event applies to.
 * @param[in] button_action The button action (press/release).
 */
void bsp_evt_handler(bsp_event_t event)
{
  ret_code_t err_code;	
   
    if(event != BUTTON_NOT_IN_USE_EVENT)
    {
    
	 // Set the correct parameters, depending on the button pushed.		
        switch (event)
        {
          case PHY_SELECTION_BUTTON_EVENT:
          {
            on_phy_selection_button();
          } break;
						
	  case OUTPUT_POWER_SELECTION_BUTTON_EVENT:
	  {
            on_output_power_selection_button();
	  } break;

          case NON_CONN_OR_CONN_ADV_BUTTON_EVENT:
          {
            //on_non_conn_or_conn_adv_selection(); DOES NOT WORK. Fatal error (nº18) NRF_ERROR_CONN_COUNT The limit of available connections for this connection configuration * tag has been reached; 
          } break;
                        
          default:
            break;
        }
        
        disconnect_stop_adv();
	advertising_init(); ;
	advertising_start();
    }
   
}

/**@brief Function for initializing logging. */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

/**@brief Function for initializing LEDs. */
static void buttons_leds_init(void)
{
    ret_code_t err_code;

    err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_evt_handler);
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for initializing GAP parameters.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
 *          device including the device name and the preferred connection parameters.
 */
static void gap_params_init(void)
{
    ret_code_t              err_code;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (uint8_t const *)DEVICE_NAME,
                                          strlen(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_ppcp_set(&m_conn_param);
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

// ***************** FUNCIONES TIMER ******************
static void lfclk_config (void) 
{
  #ifndef SOFTDEVICE_PRESENT
  ret_code_t err_code = nrf_drv_clock_init();
  APP_ERROR_CHECK(err_code;)

  nrf_drv_clock_lfclk_request(NULL);
  #endif
}

// ---------HANDLERS DE LOS TIMERS -----------
static void adv_interval_timeout_handler (void *p_context)
{
    UNUSED_PARAMETER(p_context);
    // Inicializamos características advertisement
    disconnect_stop_adv();
    advertising_init();
    advertising_start();
}

static void adv_sent_led_show_timeout_handler (void *p_context)
{
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(SENDING_ADV_LED);
}

static void non_conn_adv_fast_blink_timeout_handler(void * p_context)
{
  UNUSED_PARAMETER(p_context); 
  bsp_board_led_invert(NON_CONN_ADV_LED);
}

static void conn_adv_fast_blink_timeout_handler(void * p_context)
{
  UNUSED_PARAMETER(p_context);  
  bsp_board_led_invert(CONN_ADV_CONN_STATE_LED);
}


static void led_1Mbps_slow_blink_timeout_handler(void * p_context)
{
  UNUSED_PARAMETER(p_context);    
  bsp_board_led_invert(PHY_SELECTION_LED);    
}                     


static void led_8dBm_slow_blink_timeout_handler(void * p_context)
{
  UNUSED_PARAMETER(p_context);
  bsp_board_led_invert(OUTPUT_POWER_SELECTION_LED);
}


/**@brief Function for initializing timers. */
static void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_timer_ble, APP_TIMER_MODE_REPEATED, adv_interval_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_adv_sent_led_show_timer_id, APP_TIMER_MODE_REPEATED, adv_sent_led_show_timeout_handler);
    APP_ERROR_CHECK(err_code);

    // Creating the timers used to indicate the state/selection mode of the board.
    err_code = app_timer_create(&m_non_conn_fast_blink_timer_id, APP_TIMER_MODE_REPEATED, non_conn_adv_fast_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);
  
    err_code = app_timer_create(&m_conn_adv_fast_blink_timer_id, APP_TIMER_MODE_REPEATED, conn_adv_fast_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);
  
    err_code = app_timer_create(&m_1Mbps_led_slow_blink_timer_id, APP_TIMER_MODE_REPEATED, led_1Mbps_slow_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);
  
    err_code = app_timer_create(&m_8dBm_led_slow_blink_timer_id, APP_TIMER_MODE_REPEATED, led_8dBm_slow_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);

}
// ***************************************************

/**@brief Function for starting advertising with the current selections of output power, phy, and connectable or non-connectable advertising.
 */
static void set_current_adv_params_and_start_advertising(void)
{
  
  phy_selection_set_state(m_adv_scan_phy_selected);
  on_non_conn_or_conn_adv_selection_state_set(m_adv_scan_type_selected);
  output_power_selection_set(m_output_power_selected);
  
  ret_code_t err_code = app_timer_start(m_timer_ble, ADV_INTERVAL, NULL);
  APP_ERROR_CHECK(err_code);
  
  //Estas funciones se ejecutan en el handler del timer
  //advertising_init();
  //advertising_start();

}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    lfclk_config();
    log_init();
    timers_init();
    buttons_leds_init();
    power_management_init();
    ble_stack_init();

    instructions_print();

    set_current_adv_params_and_start_advertising();

    while (true) 
    {
      idle_state_handle();
    }
}


/**
 * @}
 */