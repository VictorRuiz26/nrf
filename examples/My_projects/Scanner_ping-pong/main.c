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

#include "boards.h"
#include "nrf_uart.h"
#include "app_uart.h"
#include "app_error.h"

#include "nrf_gpio.h"

#define FILE_NAME   "pruebaLog.txt"

#define IDX_MAJOR 25 //8 bytes de inicio y 16 del uuid
#define IDX_MINOR 27
#define IDX_TIPO  29
#define IDX_COORD_ID  30
#define IDX_SLAVE_ID  31


// ********************** CONFIGURACION UART **********************
#define UART_TX_BUFF_SIZE 128
#define UART_RX_BUFF_SIZE 128

#define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

void uart_err_handle(app_uart_evt_type_t *p)
{
  
}

void myPrintf(char *message)
{
  uint8_t i = 0;
  while(message[i] != NULL)
  {
    app_uart_put(message[i++]);
  }
}

// Aquí es donde crearé el paquete definido para luego escanear con python. Se llamará en el handler del ble
void send_adv (const ble_gap_evt_adv_report_t *adv_data) {
  
  uint8_t *pChar;

  app_uart_put(0x7E); //Inicio trama
  
  app_uart_put(0xAA); //2 Bytes fijos de relleno
  app_uart_put(0xBB);

  for (int i = 0; i < BLE_GAP_ADDR_LEN; i++)
  {
    app_uart_put(adv_data->peer_addr.addr[i]);
  }

  app_uart_put(adv_data->data.p_data[IDX_MAJOR]);
  app_uart_put(adv_data->data.p_data[IDX_MAJOR+1]);

  app_uart_put(adv_data->data.p_data[IDX_MINOR]);
  app_uart_put(adv_data->data.p_data[IDX_MINOR+1]);

  app_uart_put(adv_data->rssi);

  app_uart_put(adv_data->data.len);

  app_uart_put(0xFF);
}


const app_uart_comm_params_t comms_params = 
{
  RX_PIN_NUMBER,
  TX_PIN_NUMBER,
  RTS_PIN_NUMBER,
  CTS_PIN_NUMBER,
  UART_HWFC,
  false,
  NRF_UART_BAUDRATE_115200
};
// ****************************************************************

#define CONN_INTERVAL_DEFAULT           (uint16_t)(MSEC_TO_UNITS(7.5, UNIT_1_25_MS))    /**< Default connection interval used at connection establishment by central side. */

#define CONN_INTERVAL_MIN               (uint16_t)(MSEC_TO_UNITS(7.5, UNIT_1_25_MS))    /**< Minimum acceptable connection interval, in 1.25 ms units. */
#define CONN_INTERVAL_MAX               (uint16_t)(MSEC_TO_UNITS(500, UNIT_1_25_MS))    /**< Maximum acceptable connection interval, in 1.25 ms units. */
#define CONN_SUP_TIMEOUT                (uint16_t)(MSEC_TO_UNITS(8000,  UNIT_10_MS))    /**< Connection supervisory timeout (4 seconds). */
#define SLAVE_LATENCY                   0    

// *************** DEFINES Y VARIABLES PARA EL ESCANEO *************
#define PHY_SELECTION_LED               BSP_BOARD_LED_0																	/**< LED indicating which phy is in use. */
#define OUTPUT_POWER_SELECTION_LED      BSP_BOARD_LED_1																	/**< LED indicating at which ouput power the radio is transmitting */
#define SCAN_LED                        BSP_BOARD_LED_2																	/**< LED indicting if the device is advertising non-connectable advertising or not. */
#define ADV_REPORT_LED                  BSP_BOARD_LED_3																	/**< LED indicating that if device is advertising with connectable advertising, in a connected state, or none. */

#define PHY_SELECTION_BUTTON                  BSP_BUTTON_0
#define PHY_SELECTION_BUTTON_EVENT            BSP_EVENT_KEY_0
#define OUTPUT_POWER_SELECTION_BUTTON         BSP_BUTTON_1
#define OUTPUT_POWER_SELECTION_BUTTON_EVENT   BSP_EVENT_KEY_1
#define SCAN_SELECTION_BUTTON                 BSP_BUTTON_2
#define SCAN_SELECTION_BUTTON_EVENT           BSP_EVENT_KEY_2
#define BUTTON_NOT_IN_USE                     BSP_BUTTON_3
#define BUTTON_NOT_IN_USE_EVENT               BSP_EVENT_KEY_3


#define FAST_BLINK_INTERVAL		APP_TIMER_TIKCS(200) 
APP_TIMER_DEF(m_scan_slow_blink_timer_id);                    /**< Timer used to toggle LED for "scan mode" indication on the dev.kit. */                  

#define SLOW_BLINK_INTERVAL		APP_TIMER_TICKS(750)
APP_TIMER_DEF(m_1Mbps_led_slow_blink_timer_id);                /**< Timer used to toggle LED for phy selection indication on the dev.kit. */                  
APP_TIMER_DEF(m_8dBm_led_slow_blink_timer_id);                 /**< Timer used to toggle LED for output power selection indication on the dev.kit. */                  
APP_TIMER_DEF(m_time_keeper_timer_id);                         /**< Timer used to make sure app_timer runs continuously. Used by the log module.*/

                         
#define APP_BLE_CONN_CFG_TAG            1                                               /**< A tag that refers to the BLE stack configuration. */
#define APP_BLE_OBSERVER_PRIO           3                                               /**< Application's BLE observer priority. You shouldn't need to modify this value. */

static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;                           /**< Advertising handle used to identify an advertising set. */
static uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                            /**< Buffer for storing an encoded advertising set. */
static uint8_t m_enc_scandata[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN];                            /**< Buffer for storing an encoded advertising set. */

static void set_current_scan_params_and_start_scanning(void);
// *****************************************************************


// VARIABLE PARA COMPROBAR UUID DEL ADVERTISEMENT
#define APP_BEACON_UUID_POINTER        ((uint8_t[]){ 0x11, 0x11, 0x11, 0x11, \
                                                    0x11, 0x11, 0x11, 0x11, \
                                                    0x11, 0x11, 0x11, 0x11, \
                                                    0x11, 0x11, 0x11, 0x11 })

#define DEAD_BEEF                       0xDEADBEEF                         /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */


// ******* VARIABLES PARA EL TIPO DE FUNCIONAMIENTO *******

// Type holding the two output power options for this application.
typedef enum
{
    SELECTION_0_dBm = 0, 
    SELECTION_8_dBm = 8
} output_power_seclection_t;


// Type holding the two scan selection modes.
typedef enum
{
    SELECTION_SCAN_NON_CONN = 0, // Scanning,  not trying to connect.
    SELECTION_SCAN_CONN         //  Scanning, trying to connect when target name matches.
} scan_type_seclection_t;


// Type holding the two possible phy options.
typedef enum
{
    SELECTION_1M_PHY = 0, 
    SELECTION_CODED_PHY
} adv_scan_phy_seclection_t;

static scan_type_seclection_t    m_scan_type_selected    = SELECTION_SCAN_NON_CONN;
static adv_scan_phy_seclection_t m_adv_scan_phy_selected = SELECTION_CODED_PHY; 
static output_power_seclection_t m_output_power_selected = SELECTION_8_dBm;
static bool    m_app_initiated_disconnect   = false; //The application has initiated disconnect. Used to "tell" on_ble_gap_evt_disconnected() to not start scanning.
static bool    m_waiting_for_disconnect_evt = false; // Disconnect is initiated. The application has to wait for BLE_GAP_EVT_DISCONNECTED before proceeding.
// ***********************************************************


static uint8_t m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN]; /**< buffer where advertising reports will be stored by the SoftDevice. */

/**@brief Pointer to the buffer where advertising reports will be stored by the SoftDevice. */
static ble_data_t m_scan_buffer =
{
    m_scan_buffer_data,
    BLE_GAP_SCAN_BUFFER_EXTENDED_MIN
};


static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID;    /**< Handle of the current BLE connection .*/
static uint8_t m_gap_role     = BLE_GAP_ROLE_INVALID;       /**< BLE role for this connection, see @ref BLE_GAP_ROLES */

// Name to use for advertising and connection.
static char const m_target_periph_name[] = DEVICE_NAME;



// ****************** PARAMS EN FUNCION DEL MODO PHY *******************
// Scan parameters requested for scanning and connection.
static ble_gap_scan_params_t const m_scan_param_coded_phy =
{
    .extended       = 1,
    .active        = 0x01,
    .interval      = NRF_BLE_SCAN_SCAN_INTERVAL,
    .window        = NRF_BLE_SCAN_SCAN_WINDOW,
    .timeout       = 0x0000, // No timeout.
    .scan_phys     = BLE_GAP_PHY_CODED, 
    .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
};

static ble_gap_scan_params_t const m_scan_param_1MBps =
{
    .active        = 0x01,
    .interval      = NRF_BLE_SCAN_SCAN_INTERVAL,
    .window        = NRF_BLE_SCAN_SCAN_WINDOW,
    .timeout       = 0x0000, // No timeout.
    .scan_phys     = BLE_GAP_PHY_1MBPS,
    .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
};


// Connection parameters requested for connection.
static ble_gap_conn_params_t m_conn_param =
{
    .min_conn_interval = CONN_INTERVAL_MIN,   // Minimum connection interval.
    .max_conn_interval = CONN_INTERVAL_MAX,   // Maximum connection interval.
    .slave_latency     = SLAVE_LATENCY,       // Slave latency.
    .conn_sup_timeout  = CONN_SUP_TIMEOUT     // Supervisory timeout.
};
// *********************************************************************
 
 static void instructions_print(void)
{
    NRF_LOG_INFO("Press the buttons to set up the central in wanted mode:");
    NRF_LOG_INFO("Button 1: switch between coded phy and 1Mbps");
    NRF_LOG_INFO("Button 2: switch between 0 dbm and 8 dBm output power.");
   // NRF_LOG_INFO("Button 3: switch between scanning modes.");
}


// ************ HANDLERS DE LOS TIMER ***************
static void scan_slow_blink_timeout_handler(void * p_context)
{
	bsp_board_led_invert(SCAN_LED);
}


static void led_1Mbps_slow_blink_timeout_handler(void * p_context)
{
	bsp_board_led_invert(PHY_SELECTION_LED);    
}                     


static void led_8dBm_slow_blink_timeout_handler(void * p_context)
{
	bsp_board_led_invert(OUTPUT_POWER_SELECTION_LED);
}


static void time_keeper_timeout_handler(void * p_context)
{
    // Dummy handler
}
// **************************************************

/**@brief Function to start scanning.
 * Scanning is started based on the internal parameters (global variables) set.
 */
static void scan_start(void) 
{
   ret_code_t err_code;

   if(m_conn_handle != BLE_CONN_HANDLE_INVALID)
   {
    // Disconnect from connected peripheral.
    m_app_initiated_disconnect = true;
    m_waiting_for_disconnect_evt    = true;
    err_code = sd_ble_gap_disconnect(m_conn_handle,BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
     if (err_code != NRF_SUCCESS)
	  {
           NRF_LOG_ERROR("scan_start, sd_ble_gap_disconnect() failed: 0x%0x.", err_code);
           m_app_initiated_disconnect = false;
           m_waiting_for_disconnect_evt = false;
	  }

    while(m_waiting_for_disconnect_evt == true)
    {
      // Wait for BLE_GAP_EVT_DISCONNECTED
    }

   }
   // If already scanning, stop scanning.
   (void) sd_ble_gap_scan_stop();
     
   // Set the correct TX power.
   err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_SCAN_INIT, NULL, m_output_power_selected);
   APP_ERROR_CHECK(err_code);
   
   NRF_LOG_INFO("Output power set to %d dBm", m_output_power_selected);

   switch(m_adv_scan_phy_selected)
   {
     case SELECTION_CODED_PHY:
    {
      NRF_LOG_INFO("Starting scan on coded phy.");
       err_code = sd_ble_gap_scan_start(&m_scan_param_coded_phy, &m_scan_buffer);
      APP_ERROR_CHECK(err_code);
      break;
    }
    case SELECTION_1M_PHY:
    {
      NRF_LOG_INFO("Starting scan on 1Mbps.");
      err_code = sd_ble_gap_scan_start(&m_scan_param_1MBps, &m_scan_buffer);
      APP_ERROR_CHECK(err_code);
    }break;
    default:
    {
     NRF_LOG_INFO("Phy selection did not match setup. Scan not started.");
    }
   }
   m_app_initiated_disconnect = false;
  
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


// ********* HANDLERS PARA EVENTOS BLE *********
/**@brief Function for handling BLE_GAP_ADV_REPORT events.
 * Log adv report upon device name match or if "non-connectable" advertising on coded phy is received.
 * If in state "scanning, trying to connect", send connection request to device with matching device name.
 */
static void on_adv_report(ble_gap_evt_adv_report_t const * p_adv_report)
{
    static int8_t   rssi_value = 0;
    // Declarar una variable para almacenar el UUID
    uint8_t uuid_data[16];
    ret_code_t err_code;
    bool adv_target_name_found = ble_advdata_name_find(p_adv_report->data.p_data,
                                    p_adv_report->data.len,
                                    m_target_periph_name);
    
    bool     adv_is_connectable;          // Variable telling if the advertising device is connectable or not.
    if(p_adv_report->type.connectable == 1)
    {
      adv_is_connectable = true;
    } else // The advertising is non-connectable.
    {
      adv_is_connectable = false;
    }

    // Check if the adv report is "non-connectable" on coded phy.
    bool adv_report_non_conn_coded_phy = (adv_is_connectable == false) && (SELECTION_CODED_PHY==m_adv_scan_phy_selected);

     if ((adv_target_name_found== true) || 
         (adv_report_non_conn_coded_phy == true))
     {   

        // TODO: "Enable" if-statement if RSSI shuold only be logged when changed.
        //if(rssi_value != p_adv_report->rssi)
        //{
            rssi_value = p_adv_report->rssi;
            if(p_adv_report->primary_phy ==  BLE_GAP_PHY_1MBPS)
            {
              NRF_LOG_INFO("Received ADV report, RSSI %d, phy: 1 Mbps",rssi_value);
            }else if (p_adv_report->primary_phy == BLE_GAP_PHY_CODED )
            {
               NRF_LOG_INFO("Received ADV report, RSSI %d, phy: coded, data size: %d", rssi_value, p_adv_report->data.len);
            } else {
               NRF_LOG_INFO("Received ADV report, RSSI %d, phy: unknown",rssi_value);
            }
                                                                
            /*NRF_LOG_INFO("addr %02x:%02x:%02x:%02x:%02x:%02x",
                         p_adv_report->peer_addr.addr[0],
                         p_adv_report->peer_addr.addr[1],
                         p_adv_report->peer_addr.addr[2],
                         p_adv_report->peer_addr.addr[3],
                         p_adv_report->peer_addr.addr[4],
                         p_adv_report->peer_addr.addr[5]
                         );
             */  
             
        //}  
    }
 

    if((m_scan_type_selected == SELECTION_SCAN_CONN) && (adv_is_connectable==true))
    { 
        // Connect if the received adv_report has the set target "DEVICE_NAME" (sdk_config.h)
         if (!ble_advdata_name_find(p_adv_report->data.p_data,
                                    p_adv_report->data.len,
                                    m_target_periph_name))
         {
             err_code = sd_ble_gap_scan_start(NULL, &m_scan_buffer);
             APP_ERROR_CHECK(err_code);
         
             return;
         }
         
         NRF_LOG_INFO("Device \"%s\" found, sending a connection request.",
                      (uint32_t) m_target_periph_name);
         
         // Stop advertising.
         (void) sd_ble_gap_adv_stop(m_adv_handle);
         
         // Initiate connection.
         m_conn_param.min_conn_interval = CONN_INTERVAL_DEFAULT;
         m_conn_param.max_conn_interval = CONN_INTERVAL_DEFAULT;
         
         if(m_adv_scan_phy_selected == SELECTION_CODED_PHY)
         {
           NRF_LOG_INFO("Connecting on coded phy.");
           err_code = sd_ble_gap_connect(&p_adv_report->peer_addr,
                                           &m_scan_param_coded_phy,
                                           &m_conn_param,
                                           APP_BLE_CONN_CFG_TAG);
           if (err_code != NRF_SUCCESS)
             {
                 NRF_LOG_ERROR("sd_ble_gap_connect() failed: 0x%x.", err_code);
             }
             
         } else // SELECTION_1M_PHY
           
         {
           NRF_LOG_INFO("Connecting on 1Mbps.");
         
            err_code = sd_ble_gap_connect(&p_adv_report->peer_addr,
                                          &m_scan_param_1MBps,
                                          &m_conn_param,
                                          APP_BLE_CONN_CFG_TAG);    
            
         } 
         if (err_code != NRF_SUCCESS)
         {
             NRF_LOG_ERROR("sd_ble_gap_connect() failed: 0x%x.", err_code);
             scan_start();
         }
     } else //m_scan_type_selected == SELECTION_SCAN_NON_CONN
     {
      //Procesamiento para solo quedarnos con los UUID
        if ((p_adv_report->data.len == 30)  || 
            (p_adv_report->data.len == 255) || 
            (p_adv_report->data.len == 205) || 
            (p_adv_report->data.len == 155) || 
            (p_adv_report->data.len == 105) ||
            (p_adv_report->data.len == 55))
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
                    bsp_board_led_invert(ADV_REPORT_LED);
    
                    //FILE *archivo = fopen("pruebaLog.txt", "w");  // Abre el archivo para escribir (sobrescribe si ya existe)

                    NRF_LOG_INFO("************************************************************");
                    NRF_LOG_INFO("UUID matches the defined UUID!");
                    NRF_LOG_INFO("Advertising packet received (length: %d):", p_adv_report->data.len);
                    printf("Advertising packet received (length: %d):\n\r", p_adv_report->data.len);
                    //fprintf(archivo, "Advertising packet received (length: %d):\n\r", p_adv_report->data.len);
                    //NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);
                    NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);  
                //    for (int i = 0; i < p_adv_report->data.len; i++) {
                //      printf("%02X ", p_adv_report->data.p_data[i]);
                      //fprintf(archivo, "%02X ", p_adv_report->data.p_data[i]);
                //    }
                //    printf("\n\r");
                    //fprintf(archivo, "\n\r");
                    NRF_LOG_INFO("************************************************************");
                    printf("************************************************************\n\r");
                    //fprintf(archivo, "************************************************************\n\r");


                    // Mando paquete por UART
                    send_adv(p_adv_report);

                }
                else
                {
                    NRF_LOG_INFO("UUID does not match the defined UUID.");
                }                           
                
            
            }
      // Continue scanning
       err_code = sd_ble_gap_scan_start(NULL, &m_scan_buffer);
       APP_ERROR_CHECK(err_code);
         
      return;
     }
}


/**@brief Function for handling BLE_GAP_EVT_CONNECTED events.
 * Save the connection handle and GAP role. 
 * Start RSSI measurements and turn on the LED associated with the "connected state".
 */
static void on_ble_gap_evt_connected(ble_gap_evt_t const * p_gap_evt)
{
    ret_code_t err_code;
    
    m_conn_handle = p_gap_evt->conn_handle;
    m_gap_role    = p_gap_evt->params.connected.role;
    
    if (m_gap_role == BLE_GAP_ROLE_PERIPH)
    {
        NRF_LOG_INFO("Connected as a peripheral. Should not happend.");
    }
    else if (m_gap_role == BLE_GAP_ROLE_CENTRAL)
    {
        NRF_LOG_INFO("Connected as a central.");
    }
    
    // Stop scanning.
    (void) sd_ble_gap_scan_stop();
     err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_CONN, m_conn_handle, m_output_power_selected);
     APP_ERROR_CHECK(err_code);
    
    bsp_board_led_off(SCAN_LED);
    bsp_board_led_on(ADV_REPORT_LED);

//    rssi_measurements_start();

   
}


/**@brief Function for handling BLE_GAP_EVT_DISCONNECTED events.
 * Unset the connection handle and restart scanning if needed.
 */
static void on_ble_gap_evt_disconnected(ble_gap_evt_t const * p_gap_evt)
{
    m_conn_handle = BLE_CONN_HANDLE_INVALID;
    m_waiting_for_disconnect_evt = false;

    NRF_LOG_DEBUG("Disconnected: reason 0x%x.", p_gap_evt->params.disconnected.reason);

    
    if(m_app_initiated_disconnect == false)  // (If a the app itself (button push) has initiated a disconnect, bsp_evt_handler will start the scanning.)
    {
      bsp_board_leds_off();
      set_current_scan_params_and_start_scanning();
    }
    

}

// *********************************************



/**@brief Function for handling BLE Stack events.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
static void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
{
    static int8_t              rssi_value = 0;
    uint8_t                    channel_rssi;
    uint32_t              err_code;
    ble_gap_evt_t const * p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_ADV_REPORT:
            on_adv_report(&p_gap_evt->params.adv_report);
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

       case BLE_GAP_EVT_RSSI_CHANGED:
       {
         rssi_value =  p_gap_evt->params.rssi_changed.rssi;
         channel_rssi =  p_gap_evt->params.rssi_changed.ch_index;
         NRF_LOG_INFO("RSSI changed, new: %d, channel: %d",rssi_value, channel_rssi); 
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
           NRF_LOG_INFO("BLE_GAP_EVT_PHY_UPDATE: not implemented");
        } break;
        
        case BLE_GAP_EVT_PHY_UPDATE_REQUEST:
        {
           NRF_LOG_INFO("BLE_GAP_EVT_PHY_UPDATE_REQUEST: not implemented");

        } break;

        default:
        {
          NRF_LOG_DEBUG("Received an unimplemented BLE event.");
            // No implementation needed.
        } break;
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


/**@brief Function for initializing timers. */
static void timers_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    // Creating the timers used to indicate the state/selection mode of the board.
    err_code = app_timer_create(&m_scan_slow_blink_timer_id, APP_TIMER_MODE_REPEATED, scan_slow_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);
  
    err_code = app_timer_create(&m_1Mbps_led_slow_blink_timer_id, APP_TIMER_MODE_REPEATED, led_1Mbps_slow_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);
  
    err_code = app_timer_create(&m_8dBm_led_slow_blink_timer_id, APP_TIMER_MODE_REPEATED, led_8dBm_slow_blink_timeout_handler);
    APP_ERROR_CHECK(err_code);

    // Creating the timer used by the log module. 
    err_code = app_timer_create(&m_time_keeper_timer_id, APP_TIMER_MODE_REPEATED, time_keeper_timeout_handler);
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


// ************ MANEJADORES Y SETTERS DE LOS PARÁMETROS DE ESCANEO ************

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
    	// 8 dBm is the current output power, start blinking LED.
        bsp_board_led_off(OUTPUT_POWER_SELECTION_LED); // not necessary because the LED should start to blink. 
        err_code = app_timer_start(m_8dBm_led_slow_blink_timer_id, SLOW_BLINK_INTERVAL, NULL);
    	APP_ERROR_CHECK(err_code);
    	
    	
    	
    } break;
    
    case SELECTION_0_dBm:
    {
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
      err_code = app_timer_start(m_1Mbps_led_slow_blink_timer_id, SLOW_BLINK_INTERVAL, NULL);
      APP_ERROR_CHECK(err_code);
    } break;
    
    case SELECTION_CODED_PHY:
    {
      // Coded phy is the current sate, turn on LED.
      err_code = app_timer_stop(m_1Mbps_led_slow_blink_timer_id); 
      APP_ERROR_CHECK(err_code);
      
      bsp_board_led_on(PHY_SELECTION_LED);
      bsp_board_led_off(ADV_REPORT_LED); // todo for debugging.
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

/**@brief Function setting the internal scan state.
 * Note: this function sets the internal "scan selection state", it does not start scanning.
 *       The new scan mode will be used when (re-)starting to scan.
 */
static void scan_selection_state_set(scan_type_seclection_t scan_selection)
{
 ret_code_t err_code;

  m_scan_type_selected = scan_selection;
  bsp_board_led_off(ADV_REPORT_LED);
  switch (scan_selection)
  {
    case SELECTION_SCAN_CONN: 
    {
      // Current state is scanning,  trying to connect. Turn on associated LED.
      err_code = app_timer_stop(m_scan_slow_blink_timer_id);
      APP_ERROR_CHECK(err_code);

      bsp_board_led_on(SCAN_LED);
      NRF_LOG_INFO("Scan mode set: scanning, trying to connect. Will be applied when scanning is re-started.");
    
    } break;
    
    case SELECTION_SCAN_NON_CONN:
    {
    
      // Current state is scanning, not trying to connect. Start blinking associated LED
      err_code = app_timer_start(m_scan_slow_blink_timer_id, SLOW_BLINK_INTERVAL, NULL);
      APP_ERROR_CHECK(err_code);

      NRF_LOG_INFO("Scan mode set: scanning only. Will be applied when scanning is re-started.");
    } break;
  }

}

/**@brief Function for switching scan selection: "scan-only" or "scan, trying to connect" 
 * Note: this function does only set the internal "scan state", it does not start scanning.
 *       The new scan mode will be used when (re-)starting to scan.
 */
static void on_scan_selection(void)
{
 // Change the advertising type
  scan_type_seclection_t new_scan_selection;

     
  switch (m_scan_type_selected)
  {
    case SELECTION_SCAN_CONN:  
    {
      // Current state is scanning, not trying to connect. 
      new_scan_selection = SELECTION_SCAN_NON_CONN; 

    } break;
    
    case SELECTION_SCAN_NON_CONN:
    {
      // Current state scanning, trying to connect when target found.
      new_scan_selection = SELECTION_SCAN_CONN;

    } break;
  }
  scan_selection_state_set(new_scan_selection);

}
// ****************************************************************************

/**@brief Function for handling events from the button handler module.
 *
 * @param[in] pin_no        The pin that the event applies to.
 * @param[in] button_action The button action (press/release).
 */
void bsp_evt_handler(bsp_event_t event)
{
   
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

          case SCAN_SELECTION_BUTTON_EVENT:
          {
           // on_scan_selection(); DESHABILITO CAMBIO EN EL TIPO DE ESCANEO
          } break;
                        
          default:
            break;
        }
				
	scan_start();
    }
   
}

/**@brief Function for initializing LEDs. */
static void buttons_leds_init(void)
{
    ret_code_t err_code;
   
    err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_evt_handler);
    APP_ERROR_CHECK(err_code);

    bsp_board_leds_off();
}

/**@brief Function for start scanning with the current selections of output power, phy, and scanning "mode".
 */
static void set_current_scan_params_and_start_scanning(void)
{
  
  phy_selection_set_state(m_adv_scan_phy_selected);
  scan_selection_state_set(m_scan_type_selected);
  output_power_selection_set(m_output_power_selected);

  scan_start();

}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    uint32_t err_code;
    // Initialize.
//    log_init();

    
    timers_init();
    buttons_leds_init();

    power_management_init();
    ble_stack_init();
//    gap_params_init();


    APP_UART_FIFO_INIT(&comms_params, UART_RX_BUFF_SIZE, UART_TX_BUFF_SIZE, uart_err_handle, APP_IRQ_PRIORITY_LOWEST, err_code);
    APP_ERROR_CHECK(err_code);

    //nrf_gpio_cfg_input(BUTTON_4, NRF_GPIO_PIN_PULLUP);
    
    
    NRF_LOG_INFO("Long range demo  --central--");
    instructions_print();

    set_current_scan_params_and_start_scanning();

    // Start the timer for the LOG module
    err_code = app_timer_start(m_time_keeper_timer_id, SLOW_BLINK_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);

    // Enter main loop.
    while (true)
    {
      idle_state_handle();
    }
}


/**
 * @}
 */