#include "bleModule.h"
#include "timers.h"
#include "nrf_sdh_ble.h"
#include "nrf_log.h"
#include "buttons.h"
#include "nrf_sdh.h"
#include "buttons.h"
#include "uart.h"

adv_pdu_t adv_PDU = {
  .adv_pdu = m_beacon_info_50B,
  .size = CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH,
};

adv_scan_type_seclection_t m_adv_scan_type_selected = SELECTION_NON_CONNECTABLE; /**< Global variable holding the current scan selection mode. */
adv_scan_phy_seclection_t m_adv_scan_phy_selected = SELECTION_CODED_PHY;         /**< Global variable holding the current phy selection. */
output_power_seclection_t m_output_power_selected = SELECTION_8_dBm;             /**< Global variable holding the current output power selection. */
bool m_app_initiated_disconnect = false;                                         // The application has initiated disconnect. Used to "tell" on_ble_gap_evt_disconnected() to not start advertising.
bool m_waiting_for_disconnect_evt = false;                                       // Disconnect is initiated. The application has to wait for BLE_GAP_EVT_DISCONNECTED before proceeding.

adv_codec_phy_data_size_t m_codec_phy_data_size = CODEC_DATA_SIZE_50B;

ble_gap_adv_params_t m_adv_params;                                           /**< Parameters to be passed to the stack when starting advertising. */
uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;                       /**< Advertising handle used to identify an advertising set. */
uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                        /**< Buffer for storing an encoded advertising set. */
uint8_t m_enc_advdata_ext[BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED]; /**< Buffer for storing an encoded advertising set for codec PHY. */

/**@brief Structs that contains pointers to the encoded advertising data. */
ble_gap_adv_data_t m_adv_data =
  {
      .adv_data =
          {
              .p_data = m_enc_advdata,
              .len = BLE_GAP_ADV_SET_DATA_SIZE_MAX},
      .scan_rsp_data =
          {
              .p_data = NULL,
              .len = 0

          }
  };
  ble_gap_adv_data_t m_adv_data_ext =
  {
      .adv_data =
          {
              .p_data = m_enc_advdata_ext,
              .len = BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED},
      .scan_rsp_data =
          {
              .p_data = NULL,
              .len = 0

          }
  };

  uint8_t m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN]; /**< buffer where advertising reports will be stored by the SoftDevice. */

  /**@brief Pointer to the buffer where advertising reports will be stored by the SoftDevice. */
  ble_data_t m_scan_buffer =
  {
      m_scan_buffer_data,
      BLE_GAP_SCAN_BUFFER_EXTENDED_MIN
  };

  ble_gap_scan_params_t m_scan_param_1MBps =
  {
      .active = 0x01,
      .interval = NRF_BLE_SCAN_SCAN_INTERVAL,
      .window = NRF_BLE_SCAN_SCAN_WINDOW,
      .timeout = 0x0000, // No timeout.
      .scan_phys = BLE_GAP_PHY_1MBPS,
      .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
  };

  //Estructura por defecto, luego, puede cambiar en recepción info por uart
  ble_gap_scan_params_t m_scan_param_coded_phy =
  {
      .extended = 1,
      .active = 0x01,
      .interval = NRF_BLE_SCAN_SCAN_INTERVAL,
      .window = NRF_BLE_SCAN_SCAN_WINDOW,
      .timeout = TIMEOUT_SCAN_ADV(NUM_ADVERTISEMENTS,CODEC_DATA_SIZE_50B, ADV_INTERVAL_MS), //(TIME_10MS_1ADV(CODEC_DATA_SIZE_50B) * NUM_ADVERTISEMENTS) + EXTRA_SCAN_DURATION,
      .scan_phys = BLE_GAP_PHY_CODED,
      .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
  };

  uint16_t time_between_advs = ADV_INTERVAL_MS;
  uint8_t num_adv_2_send = NUM_ADVERTISEMENTS;

  uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
  {
      APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                           // implementation.
      APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                           // manufacturer specific data in this implementation.
      APP_BEACON_UUID,     // 128 bit UUID value.
      APP_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
      APP_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
      APP_MEASURED_RSSI,   // Manufacturer specific information. The Beacon's measured TX power in
                           // this implementation.,
      DEFAULT_TX_POWER,
      (ADV_INTERVAL_MS&0xFF00)>>8,
      ADV_INTERVAL_MS&0xFF,
      NUM_ADVERTISEMENTS,
  };

  uint8_t m_beacon_info_50B[CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
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
      DEFAULT_TX_POWER,
      (ADV_INTERVAL_MS&0xFF00)>>8,
      ADV_INTERVAL_MS&0xFF,
      NUM_ADVERTISEMENTS,

      [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
  };

  uint8_t m_beacon_info_100B[CODEC_DATA_SIZE_100B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
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
      DEFAULT_TX_POWER,
      (ADV_INTERVAL_MS&0xFF00)>>8,
      ADV_INTERVAL_MS&0xFF,
      NUM_ADVERTISEMENTS,

      [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_100B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
  };

  uint8_t m_beacon_info_150B[CODEC_DATA_SIZE_150B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
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
      DEFAULT_TX_POWER,
      (ADV_INTERVAL_MS&0xFF00)>>8,
      ADV_INTERVAL_MS&0xFF,
      NUM_ADVERTISEMENTS,

      [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_150B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
  };

  uint8_t m_beacon_info_200B[CODEC_DATA_SIZE_200B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
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
      DEFAULT_TX_POWER,
      (ADV_INTERVAL_MS&0xFF00)>>8,
      ADV_INTERVAL_MS&0xFF,
      NUM_ADVERTISEMENTS,

      [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_200B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
  };

  uint8_t m_beacon_info_250B[CODEC_DATA_SIZE_250B + APP_BEACON_INFO_LENGTH] = //< Information advertised by the Beacon.
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
      DEFAULT_TX_POWER,
      (ADV_INTERVAL_MS&0xFF00)>>8,
      ADV_INTERVAL_MS&0xFF,
      NUM_ADVERTISEMENTS,

      [APP_BEACON_INFO_LENGTH... CODEC_DATA_SIZE_250B + APP_BEACON_INFO_LENGTH - 1] = 0 // Dummy data for filling the array size
  };


/******************************* FUNCTIONS ****************************************/
    /**@brief Function for stopping advertising.
   */
  void scan_stop(void) {
    ret_code_t err_code;

    err_code = sd_ble_gap_scan_stop();
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Stopping scanning");
  }

    /**@brief Function to start scanning.
   * Scanning is started based on the internal parameters (global variables) set.
   */
  void scan_start(void) {
    ret_code_t err_code;

    // If already scanning, stop scanning.
    (void)sd_ble_gap_scan_stop();

    // Set the correct TX power.
    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_SCAN_INIT, NULL, m_output_power_selected);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_INFO("Output power set to %d dBm", m_output_power_selected);

    switch (m_adv_scan_phy_selected) {
    case SELECTION_CODED_PHY: {
    //Now, as num_adv and other params are variable, the timeout is needed to be adjusted
      m_scan_param_coded_phy.timeout = TIMEOUT_SCAN_ADV(num_adv_2_send, getRealPDUSize(m_codec_phy_data_size), time_between_advs);
      NRF_LOG_INFO("Starting scan on coded phy. Timeout %d/10ms", m_scan_param_coded_phy.timeout);
      err_code = sd_ble_gap_scan_start(&m_scan_param_coded_phy, &m_scan_buffer);
      APP_ERROR_CHECK(err_code);
      break;
    }
    case SELECTION_1M_PHY: {
      NRF_LOG_INFO("Starting scan on 1Mbps.");
      m_scan_param_1MBps.timeout = TIMEOUT_SCAN_ADV(num_adv_2_send, APP_BEACON_INFO_LENGTH, time_between_advs);
      err_code = sd_ble_gap_scan_start(&m_scan_param_1MBps, &m_scan_buffer);
      APP_ERROR_CHECK(err_code);
    } break;
    default: {
      NRF_LOG_INFO("Phy selection did not match setup. Scan not started.");
    }
    }
    m_app_initiated_disconnect = false;
  }

  /**@brief Function for handling BLE_GAP_ADV_REPORT events.
   * Log adv report upon device name match or if "non-connectable" advertising on coded phy is received.
   * If in state "scanning, trying to connect", send connection request to device with matching device name.
   */
  void on_adv_report(ble_gap_evt_adv_report_t const *p_adv_report) {
    static int8_t rssi_value = 0;
    // Declarar una variable para almacenar el UUID
    uint8_t uuid_data[16];
    ret_code_t err_code;

    rssi_value = p_adv_report->rssi;
    if (p_adv_report->primary_phy == BLE_GAP_PHY_1MBPS) {
      NRF_LOG_INFO("Received ADV report, RSSI %d, phy: 1 Mbps", rssi_value);
    } else if (p_adv_report->primary_phy == BLE_GAP_PHY_CODED) {
      NRF_LOG_INFO("Received ADV report, RSSI %d, phy: coded, data size: %d", rssi_value, p_adv_report->data.len);
    } else {
      NRF_LOG_INFO("Received ADV report, RSSI %d, phy: unknown", rssi_value);
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
          NRF_LOG_INFO("nseq rx 0X%08X", nSeqReceived);
          minorValue = nSeqReceived & 0x00FF;
          majorValue = (nSeqReceived & 0xFF00) >> 8;

          //TODO : chequear aquí también que el tipo paquete coincide
          if (nseqSent == nSeqReceived) {
            /* In the second version, master acts similar to slave in terms of obtaining metrics of the communication,
               and then it will send complete metrics packet by UART */
            if (advReceived == false) {
              advReceived = true;
              uint16_t timeExpected = (80 + 256 + 16 + 24 + 8 * 8 * (p_adv_report->data.len + 8) + 192 + 24) / 1000; // Time expected for receiving one adv
              uint16_t extraTime = 1000;
              time_for_metrics_packet(APP_TIMER_TICKS(timeExpected * num_adv_2_send + (num_adv_2_send-1)*time_between_advs + extraTime), true, NULL);
              /*err_code = app_timer_start(m_time_for_metrics_packet, APP_TIMER_TICKS(timeExpected * NUM_ADVERTISEMENTS + extraTime), NULL);
              APP_ERROR_CHECK(err_code);*/
              NRF_LOG_INFO("Time expected for 1 adv: %dms.", timeExpected);
              NRF_LOG_INFO("Primer ADV recibido, arranco timer que dura %dms!", timeExpected * num_adv_2_send + (num_adv_2_send-1)*time_between_advs + extraTime);
            }

            // Fulfilling all variables of uplink that will be sent in metrics (overwritten in each adv received)
            slaveID = p_adv_report->data.p_data[IDX_SLAVE_ID_RX];
            for (int i = 0; i < BLE_GAP_ADDR_LEN; i++) {
              slaveAddr.addr[i] = p_adv_report->peer_addr.addr[i];
            }
            downlinkMsgType = p_adv_report->data.p_data[IDX_TIPO_RX];
            NRF_LOG_INFO("Envié mensaje %d y recibo %d", msgTypeSent, downlinkMsgType);
            packetBLESize = p_adv_report->data.len;
            downlinkMeanRSSI = p_adv_report->data.p_data[IDX_MEAN_RSSI_RX];
            downlinkNAdvRx = p_adv_report->data.p_data[IDX_nADV_RX];
            rssiValues[countAdvReceived++] = p_adv_report->rssi;
            NRF_LOG_INFO("Recibo adv para nseq %d. Llevo %d", nSeqReceived, countAdvReceived);

          } else {
            NRF_LOG_INFO("el nseq no me coincide! recibo %d y mandé %d", nSeqReceived, nseqSent);
          }
        } else {
          NRF_LOG_INFO("UUID does not match the defined UUID.");
        }
      } else {
        NRF_LOG_INFO("Segun esto, el coord no soy yo: 0x%02X", p_adv_report->data.p_data[IDX_COORD_ID_RX]);
        NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len);
      }
      // Continue scanning
      err_code = sd_ble_gap_scan_start(NULL, &m_scan_buffer);
      APP_ERROR_CHECK(err_code);

      return;
    }
  }


  /**@brief Function for handling BLE Stack events.
   *
   * @param[in] p_ble_evt  Bluetooth stack event.
   */
  void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context) {

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
      NRF_LOG_INFO("Scan timeout Expired!!! empty UPLINK-DOWNLIK metrics are sent.");
      #ifdef AUTONOMO 
        adv_interval(ADV_EVT_INTERVAL, true, NULL);
        NRF_LOG_INFO("Arranco timer de %ds antes de start adv de nuevo", SEGUNDOS_DELAY);
      #endif

      //TODO: Aqui generar una función de inicialización de campos, que se llame tanto en el main como aquí. Y sea más estructurado el código
      for (int i = 0; i < BLE_GAP_ADDR_LEN; i++) {
        slaveAddr.addr[i] = '\0';
      }
      countAdvReceived = 0;
      downlinkMeanRSSI = 0;
      downlinkNAdvRx = 0;
      send_metrics();
      break;

    default: {
      NRF_LOG_DEBUG("Received an unimplemented BLE event.");
      // No implementation needed.
    } break;
    }
  }

  /**@brief Function for initializing the Advertising functionality.
   *
   * @details Encodes the required advertising data and passes it to the stack.
   *          Also builds a structure to be passed to the stack when starting advertising.
   */
  void advertising_init(void) {
    ret_code_t ret;
    unsigned char *pChar;
    /*uint8_t *adv_pdu;
    uint16_t size;*/

    ble_advdata_manuf_data_t manuf_specific_data;

    manuf_specific_data.company_identifier = APP_COMPANY_IDENTIFIER;

    // NRF_LOG_INFO("tengo seleccionado el %d PHY", m_adv_scan_phy_selected);

    /* Todo esto ya se ha rellenado en el Trata_Modbus_Inicio_PingPong
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
*/
    #ifdef AUTONOMO
      adv_PDU.adv_pdu[APP_COORD_ID_POSITION] = COORDINATOR_ID;
      nseqSent++;
      pChar = (unsigned char *)&nseqSent; //Esto seguro se puede hacer mejor
      adv_PDU.adv_pdu[APP_MINOR_POSITION] = pChar[0];
      adv_PDU.adv_pdu[APP_MINOR_POSITION-1] = pChar[1];
      adv_PDU.adv_pdu[APP_MAJOR_POSITION] = pChar[2];
      adv_PDU.adv_pdu[APP_MAJOR_POSITION-1] = pChar[3];
    #endif

    NRF_LOG_INFO("ARRANCO ADVERTISEMENT nSeq %d, numAdv %d (en Paq %d)", nseqSent, num_adv_2_send, adv_PDU.adv_pdu[APP_NUM_ADV_POSITION]);
    manuf_specific_data.data.p_data = (uint8_t *)adv_PDU.adv_pdu;
    manuf_specific_data.data.size = adv_PDU.size;

    ble_gap_adv_params_t adv_params =
        {
            .properties =
                {
                    .type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED,
                },
            .p_peer_addr = NULL,
            .filter_policy = BLE_GAP_ADV_FP_ANY,
            .interval = MSEC_TO_UNITS(time_between_advs, UNIT_0_625_MS),
            .duration = 0,
            .max_adv_evts = num_adv_2_send,

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
  void advertising_start(void) {
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
  void advertising_stop(void) {
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
  void ble_stack_init(void) {
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


 /**@brief Function for starting advertising with the current selections of output power, phy, and connectable or non-connectable advertising.
   */
  void set_current_adv_params_and_start_advertising(void) {
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

    /**@brief This function will disconnect if connected, and stop advertising if advertising. */
  void disconnect_stop_adv(void) {
    ret_code_t err_code;
    // If advertising, stop advertising.
    (void)sd_ble_gap_adv_stop(m_adv_handle);
  }

void getAdvPDU (uint8_t dataSize, adv_pdu_t *PDU) {

  switch (dataSize) {
    case MODBUS_DATA_SIZE_50:
      //m_codec_phy_data_size = CODEC_DATA_SIZE_50B; Para manejar los timers, lo dejo como abajo
      on_adv_data_size_selection_set(CODEC_DATA_SIZE_50B);
      PDU->adv_pdu = m_beacon_info_50B;
      PDU->size = CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH;
      break;
    case MODBUS_DATA_SIZE_100:
      on_adv_data_size_selection_set(CODEC_DATA_SIZE_100B);
      PDU->adv_pdu = m_beacon_info_100B;
      PDU->size = CODEC_DATA_SIZE_100B + APP_BEACON_INFO_LENGTH;
      break;
    case MODBUS_DATA_SIZE_150:
      on_adv_data_size_selection_set(CODEC_DATA_SIZE_150B);
      PDU->adv_pdu = m_beacon_info_150B;
      PDU->size = CODEC_DATA_SIZE_150B + APP_BEACON_INFO_LENGTH;
      break;
    case MODBUS_DATA_SIZE_200:
      on_adv_data_size_selection_set(CODEC_DATA_SIZE_200B);
      PDU->adv_pdu = m_beacon_info_200B;
      PDU->size = CODEC_DATA_SIZE_200B + APP_BEACON_INFO_LENGTH;
      break;
    case MODBUS_DATA_SIZE_250:
      on_adv_data_size_selection_set(CODEC_DATA_SIZE_250B);
      PDU->adv_pdu = m_beacon_info_250B;
      PDU->size = CODEC_DATA_SIZE_250B + APP_BEACON_INFO_LENGTH;  
      break;
    default:
      on_adv_data_size_selection_set(CODEC_DATA_SIZE_50B);
      PDU->adv_pdu = m_beacon_info_50B;
      PDU->size = CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH;
      break;
  }

}

unsigned char getRealPDUSize(adv_codec_phy_data_size_t data_size) {
  return (unsigned char)(data_size + APP_BEACON_INFO_LENGTH + AD_TYPE_MANUF_SPEC_DATA_ID_SIZE + 5); //El +5Por tema de flags
}

