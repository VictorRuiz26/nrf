#ifndef CONFIG_H__
#define CONFIG_H__
  #include "boards.h"
  #include "bsp.h"

  //#define AUTONOMO 1

  #define APP_BLE_CONN_CFG_TAG 1 /**< A tag identifying the SoftDevice BLE configuration. */

  // ################# VALOR QUE INDICA EL INTERVALO DE ADVERTISEMENT
  #define TIME_BETWEEN_EACH_ADV MSEC_TO_UNITS(ADV_INTERVAL_MS, UNIT_0_625_MS) /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s */

  #define APP_BEACON_INFO_LENGTH (0x17 + PDU_EXTRA_BYTES) /**< Total length of information advertised by the Beacon. */
  #define APP_ADV_DATA_LENGTH 0x15                        /**< Length of manufacturer specific data in the advertisement. */
  #define APP_DEVICE_TYPE 0x02                            /**< 0x02 refers to Beacon. */
  #define APP_MEASURED_RSSI 0xC3                          /**< The Beacon's measured RSSI at 1 meter distance in dBm. */
  #define APP_COMPANY_IDENTIFIER 0x0059                   /**< Company identifier for Nordic Semiconductor ASA. as per www.bluetooth.org. */


  // ################# MODIFICACIÓN DEL UUID DEL BEACON PARA QUE SEA TODO 1's y cambios MAJOR Y MINOR
  #define APP_MAJOR_VALUE 0x00, 0x00 /**< Major value used to identify Beacons. */
  #define APP_MINOR_VALUE 0x00, 0x00 /**< Minor value used to identify Beacons. */

  // VARIABLE PARA COMPROBAR UUID DEL ADVERTISEMENT
  #define APP_BEACON_UUID_POINTER ((uint8_t[]){0x11, 0x11, 0x11, 0x11, \
      0x11, 0x11, 0x11, 0x11,                                          \
      0x11, 0x11, 0x11, 0x11,                                          \
      0x11, 0x11, 0x11, 0x11})

  // Posición de dichos valores en la estructura de advertisements
  #define APP_MAJOR_POSITION    19
  #define APP_MINOR_POSITION    21
  #define APP_MSG_TYPE_POSITION 23
  #define APP_COORD_ID_POSITION 24
  #define APP_SLAVE_ID_POSITION 25

  #define APP_BEACON_UUID 0x11, 0x11, 0x11, 0x11, \
                          0x11, 0x11, 0x11, 0x11, \
                          0x11, 0x11, 0x11, 0x11, \
                          0x11, 0x11, 0x11, 0x11 /**< Proprietary UUID for Beacon. */

  #define DEAD_BEEF 0xDEADBEEF /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */


    // ********* BUTTON AND LED ASSIGNMENT FOR CONTROL ********

  #define ADV_REPORT_LED BSP_BOARD_LED_0
  #define SENDING_ADV_LED BSP_BOARD_LED_0
  #define PHY_SELECTION_LED BSP_BOARD_LED_1          /**< LED indicating which phy is in use. */
  #define OUTPUT_POWER_SELECTION_LED BSP_BOARD_LED_2 /**< LED indicating at which ouput power the radio is transmitting */
  #define NON_CONN_ADV_LED BSP_BOARD_LED_3           /**< LED indicting if the device is advertising non-connectable advertising or not. */
  #define CONN_ADV_CONN_STATE_LED BSP_BOARD_LED_3    /**< LED indicating that if device is advertising with connectable advertising, in a connected state, or none. */

  #define PHY_SELECTION_BUTTON BSP_BUTTON_1
  #define PHY_SELECTION_BUTTON_EVENT BSP_EVENT_KEY_1
  #define OUTPUT_POWER_SELECTION_BUTTON BSP_BUTTON_2
  #define OUTPUT_POWER_SELECTION_BUTTON_EVENT BSP_EVENT_KEY_2
  #define NON_CONN_OR_CONN_ADV_BUTTON BSP_BUTTON_3
  #define NON_CONN_OR_CONN_ADV_BUTTON_EVENT BSP_EVENT_KEY_3
  #define BUTTON_NOT_IN_USE BSP_BUTTON_0
  #define BUTTON_NOT_IN_USE_EVENT BSP_EVENT_KEY_0
  // ********************************************************

  #define MAX_NUM_ADVERTISEMENTS 30
  // ************ NEW PING PONG MODE DEFINITIONS ***********
  #define MESSAGE_TEST_TYPE 0xAE
  #define COORDINATOR_ID 255
  #define DEFAULT_SLAVE_ID 0x01
  #define MAX_SLAVES 10
  #define PDU_EXTRA_BYTES 0x03 // For mantaining previous structure, add these bytes to final (packet type, MasterID)
    //BLE data indexes
  #define IDX_MAJOR_RX 25 // 8 bytes de inicio y 16 del uuid
  #define IDX_MINOR_RX 27
  #define IDX_RSSI_RX 29
  #define IDX_TIPO_RX 30
  #define IDX_COORD_ID_RX 31
  #define IDX_SLAVE_ID_RX 32
  #define IDX_nADV_RX 33 // The amount of adv received for the slave in the sent stage
  #define IDX_MEAN_RSSI_RX 34

  #define CONN_INTERVAL_DEFAULT (uint16_t)(MSEC_TO_UNITS(7.5, UNIT_1_25_MS)) /**< Default connection interval used at connection establishment by central side. */

  #define CONN_INTERVAL_MIN (uint16_t)(MSEC_TO_UNITS(7.5, UNIT_1_25_MS)) /**< Minimum acceptable connection interval, in 1.25 ms units. */
  #define CONN_INTERVAL_MAX (uint16_t)(MSEC_TO_UNITS(500, UNIT_1_25_MS)) /**< Maximum acceptable connection interval, in 1.25 ms units. */
  #define CONN_SUP_TIMEOUT (uint16_t)(MSEC_TO_UNITS(8000, UNIT_10_MS))   /**< Connection supervisory timeout (4 seconds). */
  #define SLAVE_LATENCY 0                                                /**< Slave latency. */




#endif