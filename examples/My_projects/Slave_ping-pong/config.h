#ifndef CONFIG_H__
#define CONFIG_H__
  #include "boards.h"
  #include "nrf_gpio.h"

  #define APP_BLE_CONN_CFG_TAG            1                                               /**< A tag that refers to the BLE stack configuration. */
  #define APP_BLE_OBSERVER_PRIO           3                                               /**< Application's BLE observer priority. You shouldn't need to modify this value. */

  // VARIABLE PARA COMPROBAR UUID DEL ADVERTISEMENT
  #define APP_BEACON_UUID_POINTER        ((uint8_t[]){ 0x11, 0x11, 0x11, 0x11, \
                                                      0x11, 0x11, 0x11, 0x11, \
                                                      0x11, 0x11, 0x11, 0x11, \
                                                      0x11, 0x11, 0x11, 0x11 })

  #define DEAD_BEEF                       0xDEADBEEF                         /**< Value used as error code on stack dump, can be used to identify stack location on stack unwind. */

  #define TIME_BETWEEN_EACH_ADV    MSEC_TO_UNITS(ADV_INTERVAL_MS, UNIT_0_625_MS)  /**< The advertising interval for non-connectable advertisement (100 ms). This value can vary between 100ms to 10.24s */
  #define APP_BEACON_INFO_LENGTH          (0x17+PDU_EXTRA_BYTES)             /**< Total length of information advertised by the Beacon. */
  #define APP_ADV_DATA_LENGTH             0x15                               /**< Length of manufacturer specific data in the advertisement. */
  #define APP_DEVICE_TYPE                 0x02                               /**< 0x02 refers to Beacon. */
  #define APP_MEASURED_RSSI               0xC3                               /**< The Beacon's measured RSSI at 1 meter distance in dBm. */
  #define APP_COMPANY_IDENTIFIER          0x0059                             /**< Company identifier for Nordic Semiconductor ASA. as per www.bluetooth.org. */

  #define APP_MAJOR_POSITION              19
  #define APP_MINOR_POSITION              21

  #define APP_BEACON_UUID                 0x11, 0x11, 0x11, 0x11, \
                                          0x11, 0x11, 0x11, 0x11, \
                                          0x11, 0x11, 0x11, 0x11, \
                                          0x11, 0x11, 0x11, 0x11            /**< Proprietary UUID for Beacon. */


  #define COORD_EXTRA_BYTES     3
  #define PDU_EXTRA_BYTES       5

  #define MAX_NUM_ADVERTISEMENTS 30

  #define DEF_MAJOR_VALUE       0x00, 0x00
  #define DEF_MINOR_VALUE       0x00, 0x00

  #define MESSAGE_TEST_TYPE     0xAE
  #define COORDINATOR_ID        0xFF
  #define SLAVE_ID              0x01
  #define DEF_NSEQ              0
  #define DEF_NUM_ADV_RECEIVED  0
  #define DEF_MEAN_RSSI         0xFF

  //Indexes for the packet to sent
  #define IDX_MESSAGE_TYPE      23
  #define IDX_COORDINATOR_ID    24
  #define IDX_SLAVE_ID          25
  #define IDX_NUM_ADV_RECIEVED  26
  #define IDX_MEAN_RSSI         27

  //Indexes for obtaining fields in adv received
  #define IDX_MAJOR_RX      25
  #define IDX_MINOR_RX      27
  #define IDX_RSSI_RX       29
  #define IDX_MSG_TYPE_RX   30
  #define IDX_COORD_ID_RX   31
  #define IDX_SLAVE_ID_RX   32
  #define IDX_TX_POWER_RX   33
  #define IDX_TIME_BETW_ADV_LSB_RX 35
  #define IDX_NUM_ADV_SENT_RX  36
 

  // *************** DEFINES Y VARIABLES PARA EL ESCANEO *************
  #define PHY_SELECTION_LED               BSP_BOARD_LED_0																	/**< LED indicating which phy is in use. */
  #define OUTPUT_POWER_SELECTION_LED      BSP_BOARD_LED_1																	/**< LED indicating at which ouput power the radio is transmitting */
  #define SCAN_LED                        BSP_BOARD_LED_2																	/**< LED indicting if the device is advertising non-connectable advertising or not. */
  #define SENDING_ADV_LED                 BSP_BOARD_LED_3	
  
  	
#endif