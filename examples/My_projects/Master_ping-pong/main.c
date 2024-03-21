
#include "ble_advdata.h"
#include "bsp.h"
#include "nordic_common.h"
#include "nrf_delay.h"
#include "nrf_drv_clock.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "nrf_soc.h"
#include <stdbool.h>
#include <stdint.h>

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "app_error.h"
#include "app_uart.h"
#include "boards.h"
#include "nrf_uart.h"

#include "nrf_gpio.h"
#include <time.h>
#include "timers.h"
#include "config.h"
#include "uart.h"
#include "data.h"


#if defined(USE_UICR_FOR_MAJ_MIN_VALUES)
#define MAJ_VAL_OFFSET_IN_BEACON_INFO 18 /**< Position of the MSB of the Major Value in m_beacon_info array. */
#define UICR_ADDRESS 0x10001080          /**< Address of the UICR register used by this example. The major and minor versions to be encoded into the advertising data will be picked up from this location. */
#endif

 
static uint8_t VAR_NUM_ADVERTISEMENTS = NUM_ADVERTISEMENTS;


static uint8_t slaves_array[MAX_SLAVES] = {0x01}; // Starting defining first SlaveID.
static uint8_t actualSlave = 0;

  static void advertising_stop(void);
  // ******* STRUCTS AND VARIABLES FOR MODE OPERATION ******
  // Type holding the two output power options for this application.
  typedef enum {
    SELECTION_0_dBm = 0,
    SELECTION_8_dBm = 8
  } output_power_seclection_t;

  // Type holding the two advertising selection modes. --> DE MOMENTO NO SE USA
  typedef enum {
    SELECTION_CONNECTABLE = 0,
    SELECTION_NON_CONNECTABLE
  } adv_scan_type_seclection_t;

  // Type holding the two possible phy options.
  typedef enum {
    SELECTION_1M_PHY = 0,
    SELECTION_CODED_PHY
  } adv_scan_phy_seclection_t;

  // Para asignar dinámicamente los datos del adv
  typedef enum {
    CODEC_DATA_SIZE_50B = 50 - APP_BEACON_INFO_LENGTH - AD_TYPE_MANUF_SPEC_DATA_ID_SIZE,
    CODEC_DATA_SIZE_100B = 100 - APP_BEACON_INFO_LENGTH - AD_TYPE_MANUF_SPEC_DATA_ID_SIZE,
    CODEC_DATA_SIZE_150B = 150 - APP_BEACON_INFO_LENGTH - AD_TYPE_MANUF_SPEC_DATA_ID_SIZE,
    CODEC_DATA_SIZE_200B = 200 - APP_BEACON_INFO_LENGTH - AD_TYPE_MANUF_SPEC_DATA_ID_SIZE,
    CODEC_DATA_SIZE_250B = 250 - APP_BEACON_INFO_LENGTH - AD_TYPE_MANUF_SPEC_DATA_ID_SIZE
  } adv_codec_phy_data_size_t;

  static adv_scan_type_seclection_t m_adv_scan_type_selected = SELECTION_NON_CONNECTABLE; /**< Global variable holding the current scan selection mode. */
  static adv_scan_phy_seclection_t m_adv_scan_phy_selected = SELECTION_CODED_PHY;         /**< Global variable holding the current phy selection. */
  static output_power_seclection_t m_output_power_selected = SELECTION_8_dBm;             /**< Global variable holding the current output power selection. */
  static bool m_app_initiated_disconnect = false;                                         // The application has initiated disconnect. Used to "tell" on_ble_gap_evt_disconnected() to not start advertising.
  static bool m_waiting_for_disconnect_evt = false;                                       // Disconnect is initiated. The application has to wait for BLE_GAP_EVT_DISCONNECTED before proceeding.

  static adv_codec_phy_data_size_t m_codec_phy_data_size = CODEC_DATA_SIZE_50B;

#define TIME_MS_1ADV (80 + 256 + 16 + 24 + 8 * 8 * (CODEC_DATA_SIZE_50B + 5 + 8) + 192 + 24) / 1000 // Due to const definition problems, use const CODEC_DATA_SIZE_50B at first tests
#define TIME_10MS_1ADV TIME_MS_1ADV * 10
#define EXTRA_SCAN_DURATION 500 // Extra 5s for scanning (in 10ms units)

  static void set_current_adv_params_and_start_advertising(void);
  static void disconnect_stop_adv(void);
  // *****************************************

#define APP_BLE_CONN_CFG_TAG 1  /**< A tag that refers to the BLE stack configuration. */
#define APP_BLE_OBSERVER_PRIO 3 /**< Application's BLE observer priority. You shouldn't need to modify this value. */

  static ble_gap_adv_params_t m_adv_params;                                           /**< Parameters to be passed to the stack when starting advertising. */
  static uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;                       /**< Advertising handle used to identify an advertising set. */
  static uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                        /**< Buffer for storing an encoded advertising set. */
  static uint8_t m_enc_advdata_ext[BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED]; /**< Buffer for storing an encoded advertising set for codec PHY. */

  /**@brief Struct that contains pointers to the encoded advertising data. */
  static ble_gap_adv_data_t m_adv_data =
      {
          .adv_data =
              {
                  .p_data = m_enc_advdata,
                  .len = BLE_GAP_ADV_SET_DATA_SIZE_MAX},
          .scan_rsp_data =
              {
                  .p_data = NULL,
                  .len = 0

              }};

  /**@brief Struct that contains pointers to the encoded advertising data. */
  static ble_gap_adv_data_t m_adv_data_ext =
      {
          .adv_data =
              {
                  .p_data = m_enc_advdata_ext,
                  .len = BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED},
          .scan_rsp_data =
              {
                  .p_data = NULL,
                  .len = 0

              }};

  static uint16_t m_conn_handle = BLE_CONN_HANDLE_INVALID; /**< Handle of the current BLE connection .*/
  static uint8_t m_gap_role = BLE_GAP_ROLE_INVALID;        /**< BLE role for this connection, see @ref BLE_GAP_ROLES */

  //  ************-*-*-*- DEFINITIONS FOR BLE STACK SCAN MODE -*-*-*-*-**************
  static uint8_t m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN]; /**< buffer where advertising reports will be stored by the SoftDevice. */

  /**@brief Pointer to the buffer where advertising reports will be stored by the SoftDevice. */
  static ble_data_t m_scan_buffer =
      {
          m_scan_buffer_data,
          BLE_GAP_SCAN_BUFFER_EXTENDED_MIN};

  // Name to use for advertising and connection.
  static char const m_target_periph_name[] = DEVICE_NAME;

// VARIABLE PARA COMPROBAR UUID DEL ADVERTISEMENT
#define APP_BEACON_UUID_POINTER ((uint8_t[]){0x11, 0x11, 0x11, 0x11, \
    0x11, 0x11, 0x11, 0x11,                                          \
    0x11, 0x11, 0x11, 0x11,                                          \
    0x11, 0x11, 0x11, 0x11})

  // ****************** PARAMS EN FUNCION DEL MODO PHY *******************
  // Scan parameters requested for scanning and connection.
  static ble_gap_scan_params_t m_scan_param_coded_phy =
      {
          .extended = 1,
          .active = 0x01,
          .interval = NRF_BLE_SCAN_SCAN_INTERVAL,
          .window = NRF_BLE_SCAN_SCAN_WINDOW,
          .timeout = (TIME_10MS_1ADV * NUM_ADVERTISEMENTS) + EXTRA_SCAN_DURATION,
          .scan_phys = BLE_GAP_PHY_CODED,
          .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
      };

//TODO: si declaro una variable así y luego la inicializo, puedo meterla en la inicialización de scan
  ble_gap_scan_params_t m_scan_param_coded_phy_2;
 /*m_scan_param_coded_phy_2.extended = 1;
  m_scan_param_coded_phy_2.active = 0x01;
  m_scan_param_coded_phy_2.interval = NRF_BLE_SCAN_SCAN_INTERVAL;
  m_scan_param_coded_phy_2.window = NRF_BLE_SCAN_SCAN_WINDOW;
  m_scan_param_coded_phy_2.timeout = (TIME_10MS_1ADV * VAR_NUM_ADVERTISEMENTS) + EXTRA_SCAN_DURATION;
  m_scan_param_coded_phy_2.scan_phys = BLE_GAP_PHY_CODED;
  m_scan_param_coded_phy_2.filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL;*/

  static ble_gap_scan_params_t const m_scan_param_1MBps =
      {
          .active = 0x01,
          .interval = NRF_BLE_SCAN_SCAN_INTERVAL,
          .window = NRF_BLE_SCAN_SCAN_WINDOW,
          .timeout = 0x0000, // No timeout.
          .scan_phys = BLE_GAP_PHY_1MBPS,
          .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
      };

  /**@brief Function to start scanning.
   * Scanning is started based on the internal parameters (global variables) set.
   */
  static void scan_start(void) {
    ret_code_t err_code;

    if (m_conn_handle != BLE_CONN_HANDLE_INVALID) {
      // Disconnect from connected peripheral.
      m_app_initiated_disconnect = true;
      m_waiting_for_disconnect_evt = true;
      err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      if (err_code != NRF_SUCCESS) {
        NRF_LOG_ERROR("scan_start, sd_ble_gap_disconnect() failed: 0x%0x.", err_code);
        m_app_initiated_disconnect = false;
        m_waiting_for_disconnect_evt = false;
      }

      while (m_waiting_for_disconnect_evt == true) {
        // Wait for BLE_GAP_EVT_DISCONNECTED
      }
    }
    // If already scanning, stop scanning.
    (void)sd_ble_gap_scan_stop();

    // Set the correct TX power.
    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_SCAN_INIT, NULL, m_output_power_selected);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Output power set to %d dBm", m_output_power_selected);

    switch (m_adv_scan_phy_selected) {
    case SELECTION_CODED_PHY: {
      NRF_LOG_INFO("Starting scan on coded phy. Timeout %d/10ms", m_scan_param_coded_phy.timeout);
      err_code = sd_ble_gap_scan_start(&m_scan_param_coded_phy, &m_scan_buffer);
      APP_ERROR_CHECK(err_code);
      break;
    }
    case SELECTION_1M_PHY: {
      NRF_LOG_INFO("Starting scan on 1Mbps.");
      err_code = sd_ble_gap_scan_start(&m_scan_param_1MBps, &m_scan_buffer);
      APP_ERROR_CHECK(err_code);
    } break;
    default: {
      NRF_LOG_INFO("Phy selection did not match setup. Scan not started.");
    }
    }
    m_app_initiated_disconnect = false;
  }

  /**@brief Function for stopping advertising.
   */
  static void scan_stop(void) {
    ret_code_t err_code;

    err_code = sd_ble_gap_scan_stop();
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Stopping scanning");
  }

  /**@brief Function for handling BLE_GAP_ADV_REPORT events.
   * Log adv report upon device name match or if "non-connectable" advertising on coded phy is received.
   * If in state "scanning, trying to connect", send connection request to device with matching device name.
   */
  static void on_adv_report(ble_gap_evt_adv_report_t const *p_adv_report) {
    static int8_t rssi_value = 0;
    // Declarar una variable para almacenar el UUID
    uint8_t uuid_data[16];
    ret_code_t err_code;
    bool adv_target_name_found = ble_advdata_name_find(p_adv_report->data.p_data,
        p_adv_report->data.len,
        m_target_periph_name);

    bool adv_is_connectable; // Variable telling if the advertising device is connectable or not.
    if (p_adv_report->type.connectable == 1) {
      adv_is_connectable = true;
    } else // The advertising is non-connectable.
    {
      adv_is_connectable = false;
    }

    // Check if the adv report is "non-connectable" on coded phy.
    bool adv_report_non_conn_coded_phy = (adv_is_connectable == false) && (SELECTION_CODED_PHY == m_adv_scan_phy_selected);

    if ((adv_target_name_found == true) ||
        (adv_report_non_conn_coded_phy == true)) {

      rssi_value = p_adv_report->rssi;
      if (p_adv_report->primary_phy == BLE_GAP_PHY_1MBPS) {
        NRF_LOG_INFO("Received ADV report, RSSI %d, phy: 1 Mbps", rssi_value);
      } else if (p_adv_report->primary_phy == BLE_GAP_PHY_CODED) {
        NRF_LOG_INFO("Received ADV report, RSSI %d, phy: coded, data size: %d", rssi_value, p_adv_report->data.len);
      } else {
        NRF_LOG_INFO("Received ADV report, RSSI %d, phy: unknown", rssi_value);
      }
    }

    if (1) // m_scan_type_selected == SELECTION_SCAN_NON_CONN
    {
      // Procesamiento para solo quedarnos con los UUID
      if (/*(p_adv_report->data.len == 30)  || Esto se quita hasta que se estandarice la cosa
          (p_adv_report->data.len == 255) ||
          (p_adv_report->data.len == 205) ||
          (p_adv_report->data.len == 155) ||
          (p_adv_report->data.len == 105) ||
          (p_adv_report->data.len == 55)*/
          p_adv_report->data.p_data[IDX_COORD_ID_RX] == COORDINATOR_ID) {
        // Extract and print UUID if it follows 0xFF in the advertisement
        uint8_t *p_data = p_adv_report->data.p_data;
        uint8_t data_len = p_adv_report->data.len;

        for (int i = 0; i < data_len - 1; ++i) {
          if (p_data[i] == 0x15 && (i + 17) < data_len) // Assuming UUID is 16 bytes
          {
            // Copiar el UUID a la variable
            memcpy(uuid_data, &p_data[i + 1], 16);
            break;
          }
        }

        // Comparar el UUID almacenado con el UUID definido
        if (memcmp(uuid_data, APP_BEACON_UUID_POINTER, sizeof(uuid_data)) == 0) {
          bsp_board_led_invert(ADV_REPORT_LED);

          NRF_LOG_INFO("************************************************************");
          NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);
          NRF_LOG_INFO("************************************************************");

          uint8_t *pLong;
          uint32_t nseq;
          pLong = (unsigned char *)&nSeqReceived;
          pLong[0] = p_adv_report->data.p_data[IDX_MINOR_RX + 1];
          pLong[1] = p_adv_report->data.p_data[IDX_MINOR_RX];
          pLong[2] = p_adv_report->data.p_data[IDX_MAJOR_RX + 1];
          pLong[3] = p_adv_report->data.p_data[IDX_MAJOR_RX];
          minorValue = nSeqReceived & 0x00FF;
          majorValue = (nSeqReceived & 0xFF00) >> 8;

          if (nseqSent == nSeqReceived) {
            // Mando paquete por UART
            // send_metrics(p_adv_report
            /* In the second version, master acts similar to slave in terms of obtaining metrics of the communication,
               and then it will send complete metrics packet by UART */
            if (advReceived == false) {
              advReceived = true;
              uint16_t timeExpected = (80 + 256 + 16 + 24 + 8 * 8 * (p_adv_report->data.len + 8) + 192 + 24) / 1000; // Time expected for receiving one adv
              uint16_t extraTime = 1000;
              time_for_metrics_packet(APP_TIMER_TICKS(timeExpected * NUM_ADVERTISEMENTS + extraTime), true, NULL);
              /*err_code = app_timer_start(m_time_for_metrics_packet, APP_TIMER_TICKS(timeExpected * NUM_ADVERTISEMENTS + extraTime), NULL);
              APP_ERROR_CHECK(err_code);*/
              NRF_LOG_INFO("Time expected for 1 adv: %dms.", timeExpected);
              NRF_LOG_INFO("Primer ADV recibido, arranco timer que dura %dms!", timeExpected * NUM_ADVERTISEMENTS + extraTime);
            }

            // Fulfilling all variables of uplink that will be sent in metrics (overwritten in each adv received)
            slaveID = p_adv_report->data.p_data[IDX_SLAVE_ID_RX];
            for (int i = 0; i < BLE_GAP_ADDR_LEN; i++) {
              slaveAddr.addr[i] = p_adv_report->peer_addr.addr[i];
            }
            downlinkMsgType = p_adv_report->data.p_data[IDX_TIPO_RX];
            packetBLESize = p_adv_report->data.len;
            downlinkMeanRSSI = p_adv_report->data.p_data[IDX_MEAN_RSSI_RX];
            downlinkNAdvRx = p_adv_report->data.p_data[IDX_nADV_RX];
            rssiValues[countAdvReceived++] = p_adv_report->rssi;
            NRF_LOG_INFO("Recibo adv para nseq %d. Llevo %d", nSeqReceived, countAdvReceived);

          } else {
            NRF_LOG_INFO("el nseq no me coincide! recibo %d y mandé %d", nseq, nseqSent);
          }
        } else {
          NRF_LOG_INFO("UUID does not match the defined UUID.");
        }
      }
      // Continue scanning
      err_code = sd_ble_gap_scan_start(NULL, &m_scan_buffer);
      APP_ERROR_CHECK(err_code);

      return;
    }
  }

  // *-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*

  // Connection parameters requested for connection.
  static ble_gap_conn_params_t m_conn_param =
      {
          .min_conn_interval = CONN_INTERVAL_MIN, // Minimum connection interval.
          .max_conn_interval = CONN_INTERVAL_MAX, // Maximum connection interval.
          .slave_latency = SLAVE_LATENCY,         // Slave latency.
          .conn_sup_timeout = CONN_SUP_TIMEOUT    // Supervisory timeout.
      };

  static void instructions_print(void) {
    NRF_LOG_INFO("Press the buttons to set up the advertiser in wanted mode:");
    NRF_LOG_INFO("Button 2: switch between coded phy (LED2 setted on) and 1Mbps (LED2 blinking)");
    NRF_LOG_INFO("Button 3: switch between 0 dbm (LED3 setted on) and 8 dBm output power (LED3 blinkink).");
    // NRF_LOG_INFO("Button 4: switch between non-connectable (slow blink LED 4) and connectable advertising (fast blink LED4).");
    NRF_LOG_INFO("Button 4: rotate between 50B (LED4 Off), 100B(LED4 slow blinking), 150B(LED4 medium blinking), 200B(LED4 fast blinking) and 250B(LED4 on) adv data size (only codec PHY!)");
  }

  static uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
      {
          APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                               // implementation.
          APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                               // manufacturer specific data in this implementation.
          APP_BEACON_UUID,     // 128 bit UUID value.
          APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
          APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
          APP_MEASURED_RSSI,   // Manufacturer specific information. The Beacon's measured TX power in
                               // this implementation.
      };

  static uint8_t m_beacon_info_50B[CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
      {
          APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                               // implementation.
          APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                               // manufacturer specific data in this implementation.
          APP_BEACON_UUID,     // 128 bit UUID value.
          APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
          APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
          APP_MEASURED_RSSI,   // Manufacturer specific information. The Beacon's measured TX power in
                               // this implementation.
          MESSAGE_TEST_TYPE,
          COORDINATOR_ID,
          DEFAULT_SLAVE_ID,

          [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
      };

  static uint8_t m_beacon_info_100B[CODEC_DATA_SIZE_100B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
      {
          APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                               // implementation.
          APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                               // manufacturer specific data in this implementation.
          APP_BEACON_UUID,     // 128 bit UUID value.
          APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
          APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
          APP_MEASURED_RSSI,   // Manufacturer specific information. The Beacon's measured TX power in
                               // this implementation.
          0,
          0,
          0,
          [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_100B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
      };

  static uint8_t m_beacon_info_150B[CODEC_DATA_SIZE_150B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
      {
          APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                               // implementation.
          APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                               // manufacturer specific data in this implementation.
          APP_BEACON_UUID,     // 128 bit UUID value.
          APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
          APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
          APP_MEASURED_RSSI,   // Manufacturer specific information. The Beacon's measured TX power in
                               // this implementation.
          0,
          0,
          0,
          [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_150B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
      };

  static uint8_t m_beacon_info_200B[CODEC_DATA_SIZE_200B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
      {
          APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                               // implementation.
          APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                               // manufacturer specific data in this implementation.
          APP_BEACON_UUID,     // 128 bit UUID value.
          APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
          APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
          APP_MEASURED_RSSI,   // Manufacturer specific information. The Beacon's measured TX power in
                               // this implementation.
          0,
          0,
          0,
          [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_200B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
      };

  static uint8_t m_beacon_info_250B[CODEC_DATA_SIZE_250B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
      {
          APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                               // implementation.
          APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                               // manufacturer specific data in this implementation.
          APP_BEACON_UUID,     // 128 bit UUID value.
          APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
          APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
          APP_MEASURED_RSSI,   // Manufacturer specific information. The Beacon's measured TX power in
                               // this implementation.
          0,
          0,
          0,
          [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_250B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
      };

  /**@brief Function for handling scan request report.
   * Print the RSSI and address of the initiator if the RSSI has changed.
   */
  static void on_scan_req_report(ble_gap_evt_scan_req_report_t const *p_scan_req_report) {
    static int8_t rssi_value = 0;

    if (rssi_value != p_scan_req_report->rssi) {
      rssi_value = p_scan_req_report->rssi;
      NRF_LOG_INFO("Received scan request with RSSI %d .", rssi_value);
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
  static void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context) {

    uint32_t err_code;
    ble_gap_evt_t const *p_gap_evt = &p_ble_evt->evt.gap_evt;

    switch (p_ble_evt->header.evt_id) {
    case BLE_GAP_EVT_ADV_SET_TERMINATED:
      // disconnect_stop_adv();
      // NRF_LOG_INFO("Advertisement terminado. Motivo: %d", p_ble_evt->evt.gap_evt.params.adv_set_terminated.reason);
      // NRF_LOG_INFO("Enviados %d adv de %d configurados (max)", p_ble_evt->evt.gap_evt.params.adv_set_terminated.num_completed_adv_events, NUM_ADVERTISEMENTS);
      // bsp_board_led_off(SENDING_ADV_LED);

      adv_sent_led_show(NULL, false, NULL);
      /*err_code = app_timer_stop(m_adv_sent_led_show_timer_id);
      APP_ERROR_CHECK(err_code);*/

      scan_start();

      /*ret_code_t err_code = app_timer_start(m_timer_ble, ADV_EVT_INTERVAL, NULL);
      APP_ERROR_CHECK(err_code);*/
      break;
    case BLE_GAP_EVT_ADV_REPORT:
      on_adv_report(&p_gap_evt->params.adv_report);
      break;

    case BLE_GAP_EVT_TIMEOUT: // The scanner timeout expired, so wait some seconds, and start again the adv process
      // TODO: poner aquí también el envío de paquete por si algún caso no diera tiempo el inicial, o se quede a medias?
      NRF_LOG_INFO("Scan timeout Expired!!! Wait short time (m_timer_ble timer) before start the adv again");
      adv_interval(ADV_EVT_INTERVAL, true, NULL);
      /*ret_code_t err_code = app_timer_start(m_timer_ble, ADV_EVT_INTERVAL, NULL);
      APP_ERROR_CHECK(err_code);*/
      break;

    default: {
      NRF_LOG_DEBUG("Received an unimplemented BLE event.");
      // No implementation needed.
    } break;
    }
  }

  /**@brief This function will disconnect if connected, and stop advertising if advertising. */
  static void disconnect_stop_adv(void) {
    ret_code_t err_code;
    // If advertising, stop advertising.
    (void)sd_ble_gap_adv_stop(m_adv_handle);

    // If connected, disconnect.
    if (m_conn_handle != BLE_CONN_HANDLE_INVALID) {
      NRF_LOG_INFO("Disconnecting...");

      m_app_initiated_disconnect = true;
      m_waiting_for_disconnect_evt = true;
      err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);

      if (err_code != NRF_SUCCESS) {
        NRF_LOG_ERROR("disconnect_stop_adv, sd_ble_gap_disconnect() failed: 0x%0x.", err_code);
        m_app_initiated_disconnect = false;
        m_waiting_for_disconnect_evt = false;
      }
      while (m_waiting_for_disconnect_evt == true) {
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
  void assert_nrf_callback(uint16_t line_num, const uint8_t *p_file_name) {
    app_error_handler(DEAD_BEEF, line_num, p_file_name);
  }

  /**@brief Function for initializing the Advertising functionality.
   *
   * @details Encodes the required advertising data and passes it to the stack.
   *          Also builds a structure to be passed to the stack when starting advertising.
   */
  static void advertising_init(void) {
    ret_code_t ret;

    uint8_t *adv_pdu;
    uint16_t size;

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

    // NRF_LOG_INFO("tengo seleccionado el %d PHY", m_adv_scan_phy_selected);

    if (m_adv_scan_phy_selected == SELECTION_1M_PHY) {
      //   NRF_LOG_INFO("Entro a 1M");
      adv_pdu = m_beacon_info;
      size = APP_BEACON_INFO_LENGTH;
    } else if (m_adv_scan_phy_selected == SELECTION_CODED_PHY) {
      //   NRF_LOG_INFO("Entro a Codec");
      switch (m_codec_phy_data_size) {
      case CODEC_DATA_SIZE_50B:
        adv_pdu = m_beacon_info_50B;
        size = CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH;
        break;
      case CODEC_DATA_SIZE_100B:
        //     NRF_LOG_INFO("Selecciono 100B");
        adv_pdu = m_beacon_info_100B;
        size = CODEC_DATA_SIZE_100B + APP_BEACON_INFO_LENGTH;
        break;
      case CODEC_DATA_SIZE_150B:
        //     NRF_LOG_INFO("Selecciono 150B, y tengo en memoria %d", CODEC_DATA_SIZE_150B+APP_BEACON_INFO_LENGTH);
        adv_pdu = m_beacon_info_150B;
        size = CODEC_DATA_SIZE_150B + APP_BEACON_INFO_LENGTH;
        break;
      case CODEC_DATA_SIZE_200B:
        //     NRF_LOG_INFO("Selecciono 200B");
        adv_pdu = m_beacon_info_200B;
        size = CODEC_DATA_SIZE_200B + APP_BEACON_INFO_LENGTH;
        break;
      case CODEC_DATA_SIZE_250B:
        //     NRF_LOG_INFO("Selecciono 250B");
      default:
        adv_pdu = m_beacon_info_250B;
        size = CODEC_DATA_SIZE_250B + APP_BEACON_INFO_LENGTH;
        break;
      }
    } else { // Por si un caso algo falla, cojo el del advertisement normal
      NRF_LOG_INFO("Selecciono 30B defecto!");
      adv_pdu = m_beacon_info;
      size = APP_BEACON_INFO_LENGTH;
    }

    adv_pdu[APP_MINOR_POSITION] += 1;

    if (adv_pdu[APP_MINOR_POSITION] == 0) {
      // Incrementa el MSB minor solo si el LSB minor ha desbordado
      adv_pdu[APP_MINOR_POSITION - 1] += 1;
      if (adv_pdu[APP_MINOR_POSITION - 1] == 0) {
        adv_pdu[APP_MAJOR_POSITION] += 1;
        if (adv_pdu[APP_MAJOR_POSITION] == 0) {
          adv_pdu[APP_MAJOR_POSITION - 1] += 1;
        }
      }
    }
    nseqSent = (adv_pdu[APP_MAJOR_POSITION - 1] << 24) | (adv_pdu[APP_MAJOR_POSITION] << 16) | (adv_pdu[APP_MINOR_POSITION - 1] << 8) | (adv_pdu[APP_MINOR_POSITION]);

    manuf_specific_data.data.p_data = (uint8_t *)adv_pdu;
    manuf_specific_data.data.size = size;

    ble_gap_adv_params_t adv_params =
        {
            .properties =
                {
                    .type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED,
                },
            .p_peer_addr = NULL,
            .filter_policy = BLE_GAP_ADV_FP_ANY,
            .interval = TIME_BETWEEN_EACH_ADV,
            .duration = 0,
            .max_adv_evts = NUM_ADVERTISEMENTS,

            .primary_phy = BLE_GAP_PHY_1MBPS, // Must be changed to connect in long range. (BLE_GAP_PHY_CODED)
            .secondary_phy = BLE_GAP_PHY_1MBPS,
            .scan_req_notification = 1,
        };

    // Build and set advertising data.
    ble_advdata_t const adv_data =
        {
            .name_type = BLE_ADVDATA_NO_NAME,
            .flags = BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED,
            .include_appearance = false,
            .p_manuf_specific_data = &manuf_specific_data,
        };

    // Now, parameters of advertisements are setted using variables modified through bsp buttons
    if (m_adv_scan_phy_selected == SELECTION_1M_PHY) {
      // 1M coded for adv
      //     NRF_LOG_INFO("Setting adv params PHY to 1M. ");
      adv_params.primary_phy = BLE_GAP_PHY_1MBPS;
      adv_params.secondary_phy = BLE_GAP_PHY_1MBPS;

      if (m_adv_scan_type_selected == SELECTION_CONNECTABLE) {
        //         NRF_LOG_INFO("Advertising type set to CONNECTABLE_SCANNABLE_UNDIRECTED ");
        adv_params.properties.type = BLE_GAP_ADV_TYPE_CONNECTABLE_SCANNABLE_UNDIRECTED;
      } else if (m_adv_scan_type_selected == SELECTION_NON_CONNECTABLE) {
        //         NRF_LOG_INFO("Advertising type set to NONCONNECTABLE_SCANNABLE_UNDIRECTED ");
        adv_params.properties.type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED;
      }

      ret = ble_advdata_encode(&adv_data, m_adv_data.adv_data.p_data, &m_adv_data.adv_data.len);
      APP_ERROR_CHECK(ret);

      ret = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data, &adv_params);
      APP_ERROR_CHECK(ret);
    }

    else if (m_adv_scan_phy_selected == SELECTION_CODED_PHY) {
      // only extended advertising will allow primary phy to be coded
      //     NRF_LOG_INFO("Setting adv params phy to coded phy .. ");
      adv_params.primary_phy = BLE_GAP_PHY_CODED;
      adv_params.secondary_phy = BLE_GAP_PHY_CODED;

      if (m_adv_scan_type_selected == SELECTION_CONNECTABLE) {
        //         NRF_LOG_INFO("Advertising type set to EXTENDED_CONNECTABLE_NONSCANNABLE_UNDIRECTED ");
        adv_params.properties.type = BLE_GAP_ADV_TYPE_EXTENDED_CONNECTABLE_NONSCANNABLE_UNDIRECTED;

        ret = ble_advdata_encode(&adv_data, m_adv_data_ext.adv_data.p_data, &m_adv_data_ext.adv_data.len);
        APP_ERROR_CHECK(ret);

        ret = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data_ext, &adv_params);
        APP_ERROR_CHECK(ret);
      } else if (m_adv_scan_type_selected == SELECTION_NON_CONNECTABLE) {
        //        NRF_LOG_INFO("Advertising type set to EXTENDED_NONCONNECTABLE_SCANNABLE_UNDIRECTED ");
        adv_params.properties.type = BLE_GAP_ADV_TYPE_EXTENDED_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED;

        ret = ble_advdata_encode(&adv_data, m_adv_data_ext.adv_data.p_data, &m_adv_data_ext.adv_data.len);
        APP_ERROR_CHECK(ret);

        ret = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data_ext, &adv_params);
        APP_ERROR_CHECK(ret);
      }
    }
  }

  /**@brief Function for starting advertising.
   */
  static void advertising_start(void) {
    ret_code_t err_code;

    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, m_adv_handle, m_output_power_selected);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_adv_start(m_adv_handle, APP_BLE_CONN_CFG_TAG);
    // NRF_LOG_INFO("err_code after sd_ble_gap_adv_start: %d", err_code);
    APP_ERROR_CHECK(err_code);

    adv_sent_led_show(BLINK_SENDING_ADV, true, NULL);
    /*err_code = app_timer_start(m_adv_sent_led_show_timer_id, BLINK_SENDING_ADV, NULL);
    APP_ERROR_CHECK(err_code);*/

    m_app_initiated_disconnect = false;
  }

  /**@brief Function for stopping advertising.
   */
  static void advertising_stop(void) {
    ret_code_t err_code;

    err_code = sd_ble_gap_adv_stop(m_adv_handle);
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Stopping advertising");

    // err_code = sd_ble_gap_adv_start(m_adv_handle, APP_BLE_CONN_CFG_TAG);
    // APP_ERROR_CHECK(err_code);
  }

  /**@brief Function for initializing the BLE stack.
   *
   * @details Initializes the SoftDevice and the BLE event interrupt.
   */
  static void ble_stack_init(void) {
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
  static void output_power_selection_set(output_power_seclection_t output_power) {
    ret_code_t err_code;
    m_output_power_selected = output_power;
    switch (m_output_power_selected) {
    case SELECTION_8_dBm: {
      NRF_LOG_INFO("Power selection set to 8dBm");
      // 8 dBm is the current output power, off LED.
      bsp_board_led_off(OUTPUT_POWER_SELECTION_LED); // not necessary because the LED should start to blink.
    } break;

    case SELECTION_0_dBm: {
      NRF_LOG_INFO("Power selection set to 0dBm");
      // 0 dBm is the current output power, turn on LED.
      bsp_board_led_on(OUTPUT_POWER_SELECTION_LED);

    } break;
    }
  }

  /**@brief Function for switching output power/TX power: 0 dBm or 8 dBm.
   * Note:  this function does only set the internal state, it does apply the new setting.
   *        The new output power will be set when (re-)starting to scan.
   */
  static void on_output_power_selection_button(void) {
    // Change the output power.
    output_power_seclection_t new_output_power;
    switch (m_output_power_selected) {
    case SELECTION_0_dBm: // 0 dBm is the previous output power.
    {
      // 8 dBm is the current output power, start blinking LED.
      new_output_power = SELECTION_8_dBm;

    } break;

    case SELECTION_8_dBm: {
      // 0 dBm is the current output power, turn on LED.
      new_output_power = SELECTION_0_dBm;

    } break;
    }
    output_power_selection_set(new_output_power);
  }

  /**@brief Function setting the internal advertising state.
   * Note: this function does only set the internal "advertising state", it does not start advertising.
   *       The new advertising mode will be used when (re-)starting to advertise.
   */
  static void on_non_conn_or_conn_adv_selection_state_set(adv_scan_type_seclection_t adv_selection) {
    ret_code_t err_code;

    m_adv_scan_type_selected = adv_selection;
    bsp_board_led_off(CONN_ADV_CONN_STATE_LED);
    switch (adv_selection) {
    case SELECTION_NON_CONNECTABLE: {
      // Current state is non-connectable advertising. Start blinking associated LED.
      NRF_LOG_INFO("Conn mode changed to NON_CONNECTABLE");
    } break;

    case SELECTION_CONNECTABLE: {
      // Current state is connectable advertising. Start blinking associated LED.
      NRF_LOG_INFO("Conn mode changed to CONNECTABLE");
    } break;
    }
  }

  /**@brief Function for switching between "non-connectable and connectable advertising" selection.
   * Note: this function does only set the internal "advertising state", it does not start advertising.
   *       The new advertising mode will be used when (re-)starting to advertise.
   */
  static void on_non_conn_or_conn_adv_selection(void) {
    // Change the advertising type
    adv_scan_type_seclection_t new_adv_selection;

    switch (m_adv_scan_type_selected) {
    case SELECTION_CONNECTABLE: // Connectable advertising is the previous state.
    {
      // Current state is non-connectable advertising. Start blinking associated LED.
      new_adv_selection = SELECTION_NON_CONNECTABLE;

    } break;

    case SELECTION_NON_CONNECTABLE: {
      // Current state is connectable advertising. Start blinking associated LED.
      new_adv_selection = SELECTION_CONNECTABLE;

    } break;
    }
    on_non_conn_or_conn_adv_selection_state_set(new_adv_selection);
  }

  /**@brief Function setting the advertising data size.
   * Note: this function does only set the adv data size, it does not start advertising.
   *       The new advertising mode will be used when (re-)starting to advertise.
   */
  static void on_adv_data_size_selection_set(adv_codec_phy_data_size_t adv_data_size) {
    ret_code_t err_code;

    if (m_adv_scan_phy_selected == SELECTION_1M_PHY) {
      NRF_LOG_INFO("Data size using 1M PHY: %d", APP_BEACON_INFO_LENGTH);
    } else {
      /* As the change adv size is supported, it is needed to restart the .len byte of m_adv_data_ext, because in ble_advdata_encode
         a pointer to .len is passed, so next time the function is called, the size is setted to before len array. It is needed to restart
         each time the adv data size is changed.
      */
      m_adv_data_ext.adv_data.len = BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED;
      m_codec_phy_data_size = adv_data_size;

      bsp_board_led_off(CONN_ADV_CONN_STATE_LED);
      switch (adv_data_size) {
      case CODEC_DATA_SIZE_50B: {
        NRF_LOG_INFO("Adv size changed to 50 bytes");
        adv_data_size_100_codec(NULL, false, NULL);
        adv_data_size_150_codec(NULL, false, NULL);
        adv_data_size_200_codec(NULL, false, NULL);
        /*
        err_code = app_timer_stop(m_adv_data_size_200_codec_timer_id);
        APP_ERROR_CHECK(err_code);
        err_code = app_timer_stop(m_adv_data_size_150_codec_timer_id);
        APP_ERROR_CHECK(err_code);
        err_code = app_timer_stop(m_adv_data_size_100_codec_timer_id);
        APP_ERROR_CHECK(err_code);*/

        bsp_board_led_off(CONN_ADV_CONN_STATE_LED);
      } break;

      case CODEC_DATA_SIZE_100B: {
        NRF_LOG_INFO("Adv size changed to 100 bytes");
        bsp_board_led_off(CONN_ADV_CONN_STATE_LED); // Not necessary, led should start to blink

        adv_data_size_150_codec(NULL, false, NULL);
        adv_data_size_200_codec(NULL, false, NULL);

        adv_data_size_100_codec(SLOW_BLINK_INTERVAL, true, NULL);
        /*err_code = app_timer_stop(m_adv_data_size_200_codec_timer_id);
        APP_ERROR_CHECK(err_code);
        err_code = app_timer_stop(m_adv_data_size_150_codec_timer_id);
        APP_ERROR_CHECK(err_code);

        err_code = app_timer_start(m_adv_data_size_100_codec_timer_id, SLOW_BLINK_INTERVAL, NULL);
        APP_ERROR_CHECK(err_code);*/
      } break;

      case CODEC_DATA_SIZE_150B: {
        NRF_LOG_INFO("Adv size changed to 150 bytes");
        bsp_board_led_off(CONN_ADV_CONN_STATE_LED); // Not necessary, led should start to blink

        adv_data_size_100_codec(NULL, false, NULL);
        adv_data_size_200_codec(NULL, false, NULL);

        adv_data_size_150_codec(SLOW_BLINK_INTERVAL, true, NULL);
        /*err_code = app_timer_stop(m_adv_data_size_200_codec_timer_id);
        APP_ERROR_CHECK(err_code);
        err_code = app_timer_stop(m_adv_data_size_100_codec_timer_id);
        APP_ERROR_CHECK(err_code);

        err_code = app_timer_start(m_adv_data_size_150_codec_timer_id, MEDIUM_BLINK_INTERVAL, NULL);
        APP_ERROR_CHECK(err_code);*/
      } break;

      case CODEC_DATA_SIZE_200B: {
        NRF_LOG_INFO("Adv size changed to 200 bytes");
        bsp_board_led_off(CONN_ADV_CONN_STATE_LED); // Not necessary, led should start to blink

        adv_data_size_100_codec(NULL, false, NULL);
        adv_data_size_150_codec(NULL, false, NULL);

        adv_data_size_200_codec(SLOW_BLINK_INTERVAL, true, NULL);
        /*err_code = app_timer_stop(m_adv_data_size_150_codec_timer_id);
        APP_ERROR_CHECK(err_code);
        err_code = app_timer_stop(m_adv_data_size_100_codec_timer_id);
        APP_ERROR_CHECK(err_code);

        err_code = app_timer_start(m_adv_data_size_200_codec_timer_id, FAST_BLINK_INTERVAL, NULL);
        APP_ERROR_CHECK(err_code);*/
      } break;

      case CODEC_DATA_SIZE_250B: {
        NRF_LOG_INFO("Adv size changed to 250 bytes");

        adv_data_size_100_codec(NULL, false, NULL);
        adv_data_size_150_codec(NULL, false, NULL);
        adv_data_size_200_codec(NULL, false, NULL);
        /*err_code = app_timer_stop(m_adv_data_size_200_codec_timer_id);
        APP_ERROR_CHECK(err_code);
        err_code = app_timer_stop(m_adv_data_size_150_codec_timer_id);
        APP_ERROR_CHECK(err_code);
        err_code = app_timer_stop(m_adv_data_size_100_codec_timer_id);
        APP_ERROR_CHECK(err_code);*/

        bsp_board_led_on(CONN_ADV_CONN_STATE_LED);
      } break;
      }
    }
  }

  /**@brief Function for switching between codec adv data size selection.
   * Note: this function does only set the internal adv-data-size value, it does not start advertising.
   *       The new advertising mode will be used when (re-)starting to advertise.
   */
  static void on_adv_data_size_selection(void) {
    // Change the advertising type
    adv_codec_phy_data_size_t new_adv_data_size_selection;

    // Only do anything when in Codec
    if (m_adv_scan_phy_selected == SELECTION_CODED_PHY) {
      switch (m_codec_phy_data_size) {
      case CODEC_DATA_SIZE_50B: // Connectable advertising is the previous state.
      {
        // Current state is 50 bytes of data. Set off associated LED
        new_adv_data_size_selection = CODEC_DATA_SIZE_100B;
      } break;

      case CODEC_DATA_SIZE_100B: // Connectable advertising is the previous state.
      {
        // Current state is 100 bytes of data. Set slow blinking associated LED
        new_adv_data_size_selection = CODEC_DATA_SIZE_150B;
      } break;

      case CODEC_DATA_SIZE_150B: {
        // Current state is 150 bytes of data. Start medimum blinking associated LED.
        new_adv_data_size_selection = CODEC_DATA_SIZE_200B;
      } break;

      case CODEC_DATA_SIZE_200B: {
        // Current state is 200 bytes of data. Start fast blinking associated LED.
        new_adv_data_size_selection = CODEC_DATA_SIZE_250B;
      } break;

      case CODEC_DATA_SIZE_250B: {
        // Current state is 250 bytes of data. Set on associated LED.
        new_adv_data_size_selection = CODEC_DATA_SIZE_50B;
      } break;
      }
      on_adv_data_size_selection_set(new_adv_data_size_selection);
    }
  }

  /**@brief Function for setting new phy selection. LEDs start to blink according to the selected state.
   * Note: this function does only set the internal state, it does apply the new setting.
   *       The new phy will be be used when (re-)starting scanning.
   */
  static void phy_selection_set_state(adv_scan_phy_seclection_t new_phy_selection) {
    ret_code_t err_code;
    m_adv_scan_phy_selected = new_phy_selection;

    switch (new_phy_selection) {
    case SELECTION_1M_PHY: // SELECTION_1M_PHY is the current "state".
    {
      // 1 Mbps is the current state, LED setted off.
      NRF_LOG_INFO("PHY mode changed to 1M");
      bsp_board_led_off(PHY_SELECTION_LED);
    } break;

    case SELECTION_CODED_PHY: {
      // Coded phy is the current sate, turn on LED.
      NRF_LOG_INFO("PHY mode changed to Codec");
      bsp_board_led_on(PHY_SELECTION_LED);
      on_adv_data_size_selection_set(m_codec_phy_data_size);
    } break;
    }
  }

  /**@brief Function for switching PHY: coded phy or 1 Mbps
   * Note: this function does only set the internal state, it does apply the new setting.
   *       The new phy will be be used when (re-)starting scanning.
   */
  static void on_phy_selection_button(void) {
    // Change the selected phy.
    adv_scan_phy_seclection_t new_phy_selection;
    switch (m_adv_scan_phy_selected) {
    case SELECTION_CODED_PHY: // SELECTION_CODED_PHY is the previous "state".
    {
      // 1 Mbps is the current state, LED should start blinking.
      new_phy_selection = SELECTION_1M_PHY;
    } break;

    case SELECTION_1M_PHY: {
      // Coded phy is the current sate, turn on LED.
      new_phy_selection = SELECTION_CODED_PHY;
    } break;
    }
    phy_selection_set_state(new_phy_selection);
  }
  // ***********************************************************************************************************

  /**@brief Function for handling events from the button handler module.
   *
   * @param[in] pin_no        The pin that the event applies to.
   * @param[in] button_action The button action (press/release).
   */
  void bsp_evt_handler(bsp_event_t event) {
    ret_code_t err_code;

    if (event != BUTTON_NOT_IN_USE_EVENT) {

      // Set the correct parameters, depending on the button pushed.
      switch (event) {
      case PHY_SELECTION_BUTTON_EVENT: {
        on_phy_selection_button();
      } break;

      case OUTPUT_POWER_SELECTION_BUTTON_EVENT: {
        on_output_power_selection_button();
      } break;

      case NON_CONN_OR_CONN_ADV_BUTTON_EVENT: // Esto ahora actúa como modificador del tamaño de los datos!
      {
        // on_non_conn_or_conn_adv_selection();
        on_adv_data_size_selection();
      } break;

      default:
        break;
      }

      disconnect_stop_adv();
      advertising_init();
      ;
      advertising_start();
    }
  }

  /**@brief Function for initializing logging. */
  static void log_init(void) {
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
  }

  /**@brief Function for initializing LEDs. */
  static void buttons_leds_init(void) {
    ret_code_t err_code;

    err_code = bsp_init(BSP_INIT_LEDS | BSP_INIT_BUTTONS, bsp_evt_handler);
    APP_ERROR_CHECK(err_code);
  }

  /**@brief Function for initializing GAP parameters.
   *
   * @details This function sets up all the necessary GAP (Generic Access Profile) parameters of the
   *          device including the device name and the preferred connection parameters.
   */
  static void gap_params_init(void) {
    ret_code_t err_code;
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
  static void power_management_init(void) {
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
  }

  /**@brief Function for handling the idle state (main loop).
   *
   * @details If there is no pending log operation, then sleep until next the next event occurs.
   */
  static void idle_state_handle(void) {
    if (NRF_LOG_PROCESS() == false) {
      nrf_pwr_mgmt_run();
    }
  }

  // ***************** FUNCIONES TIMER ******************
  static void lfclk_config(void) {
#ifndef SOFTDEVICE_PRESENT
    ret_code_t err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code;)

    nrf_drv_clock_lfclk_request(NULL);
#endif
  }

  /**@brief Function for starting advertising with the current selections of output power, phy, and connectable or non-connectable advertising.
   */
  static void set_current_adv_params_and_start_advertising(void) {

    phy_selection_set_state(m_adv_scan_phy_selected);
    // on_non_conn_or_conn_adv_selection_state_set(m_adv_scan_type_selected); Para un nuevo timer de más cantidad de datos
    output_power_selection_set(m_output_power_selected);
    on_adv_data_size_selection_set(m_codec_phy_data_size);

    // NO!, es la cuenta para dormir, debo empezarla cuando se haya acabado el envío (BLE_GAP_EVT_ADV_SET_TERMINATED de ble_evt_handler)
    // ret_code_t err_code = app_timer_start(m_timer_ble, ADV_EVT_INTERVAL, NULL);
    // APP_ERROR_CHECK(err_code);

    // Estas funciones se ejecutan en el handler del timer
    advertising_init();
    advertising_start();
  }

  /**
   * @brief Function for application main entry.
   */
  int main(void) {
    uint32_t err_code;
    // Initialize.
    lfclk_config();
    log_init();
    timers_init();
    buttons_leds_init();
    power_management_init();
    ble_stack_init();
    gap_params_init();
    InicializaBufferUART();

    instructions_print();

    err_code = sd_ble_gap_addr_get(&coordAddr);
    APP_ERROR_CHECK(err_code);

    APP_UART_FIFO_INIT(&comms_params, UART_RX_BUFF_SIZE, UART_TX_BUFF_SIZE, uart_evt_handle, APP_IRQ_PRIORITY_LOWEST, err_code);
    APP_ERROR_CHECK(err_code);

//    set_current_adv_
    analize_buffer_uart(APP_TIMER_TICKS(50), true, NULL);
    /*err_code = app_timer_start(m_analize_buffer_uart, APP_TIMER_TICKS(50), NULL);
    APP_ERROR_CHECK(err_code);*/
    while (true) {
      idle_state_handle();
    }
  }

  /**
   * @}
   */