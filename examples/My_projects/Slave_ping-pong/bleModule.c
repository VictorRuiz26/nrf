#include "bleModule.h"
#include "buttons.h"
#include "config.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "timers.h"
#include "nrf_log.h"
#include "app_timer.h"
#include "data.h"

adv_codec_phy_data_size_t m_codec_phy_data_size = CODEC_DATA_SIZE_50B; //Default value, could be changed depending on the data size received

uint8_t m_adv_handle = BLE_GAP_ADV_SET_HANDLE_NOT_SET;                           /**< Advertising handle used to identify an advertising set. */
uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                            /**< Buffer for storing an encoded advertising set. */
uint8_t m_enc_scandata[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN];                            /**< Buffer for storing an encoded advertising set. */


//scan_type_seclection_t    m_scan_type_selected    = SELECTION_SCAN_NON_CONN;
adv_scan_phy_seclection_t m_adv_scan_phy_selected = SELECTION_CODED_PHY; 
output_power_seclection_t m_output_power_selected = SELECTION_8_dBm;

uint8_t m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN]; /**< buffer where advertising reports will be stored by the SoftDevice. */

/**@brief Pointer to the buffer where advertising reports will be stored by the SoftDevice. */
ble_data_t m_scan_buffer =
{
    m_scan_buffer_data,
    BLE_GAP_SCAN_BUFFER_EXTENDED_MIN
};


ble_gap_scan_params_t m_scan_param_coded_phy =
{
    .extended       = 1,
    .active        = 0x01,
    .interval      = NRF_BLE_SCAN_SCAN_INTERVAL,
    .window        = NRF_BLE_SCAN_SCAN_WINDOW,
    .timeout       = 0x0000, // No timeout.
    .scan_phys     = BLE_GAP_PHY_CODED, 
    .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
};

ble_gap_scan_params_t m_scan_param_1MBps =
{
    .active        = 0x01,
    .interval      = NRF_BLE_SCAN_SCAN_INTERVAL,
    .window        = NRF_BLE_SCAN_SCAN_WINDOW,
    .timeout       = 0x0000, // No timeout.
    .scan_phys     = BLE_GAP_PHY_1MBPS,
    .filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL,
};


uint8_t m_enc_advdata     [BLE_GAP_ADV_SET_DATA_SIZE_MAX];  /**< Buffer for storing an encoded advertising set. */
uint8_t m_enc_advdata_ext [BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED];  /**< Buffer for storing an encoded advertising set for codec PHY. */


/**@brief Struct that contains pointers to the encoded advertising data. */
ble_gap_adv_data_t m_adv_data =
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

/**@brief Struct that contains pointers to the encoded advertising data. */
ble_gap_adv_data_t m_adv_data_ext =
{
    .adv_data =
    {
        .p_data = m_enc_advdata_ext,
        .len    = BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED
    },
    .scan_rsp_data =
    {
        .p_data = NULL,
        .len    = 0

    }
};

uint16_t time_between_advs = ADV_INTERVAL_MS;
uint8_t num_adv_2_send = NUM_ADVERTISEMENTS;


uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH] =                    //< Information advertised by the Beacon. 
{
    APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                         // implementation.
    APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                         // manufacturer specific data in this implementation.
    APP_BEACON_UUID,     // 128 bit UUID value.
    DEF_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
    DEF_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
    APP_MEASURED_RSSI,    // Manufacturer specific information. The Beacon's measured TX power in
                         // this implementation.
};

uint8_t m_beacon_info_50B[CODEC_DATA_SIZE_50B+APP_BEACON_INFO_LENGTH] =                    //< Information advertised by the Beacon. 
{
    APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                         // implementation.
    APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                         // manufacturer specific data in this implementation.
    APP_BEACON_UUID,     // 128 bit UUID value.
    DEF_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
    DEF_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
    APP_MEASURED_RSSI,   // Manufacturer specific information. The Beacon's measured TX power in
                         // this implementation.
    MESSAGE_TEST_TYPE,   // 23 
    COORDINATOR_ID,      // 24 
    SLAVE_ID,            // 25
    DEF_NUM_ADV_RECEIVED,// 26
    DEF_MEAN_RSSI,       // 27
    
    [APP_BEACON_INFO_LENGTH ... CODEC_DATA_SIZE_50B+APP_BEACON_INFO_LENGTH-1] = 0 //Dummy data for filling the array size
};
uint8_t m_beacon_info_100B[CODEC_DATA_SIZE_100B+APP_BEACON_INFO_LENGTH] =                    //< Information advertised by the Beacon. 
{
    APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                         // implementation.
    APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                         // manufacturer specific data in this implementation.
    APP_BEACON_UUID,     // 128 bit UUID value.
    DEF_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
    DEF_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
    APP_MEASURED_RSSI,    // Manufacturer specific information. The Beacon's measured TX power in
                         // this implementation.
    MESSAGE_TEST_TYPE,   // 23 
    COORDINATOR_ID,      // 24 
    SLAVE_ID,            // 25
    DEF_NUM_ADV_RECEIVED,// 26
    DEF_MEAN_RSSI,       // 27
    [APP_BEACON_INFO_LENGTH ... CODEC_DATA_SIZE_100B+APP_BEACON_INFO_LENGTH-1] = 0 //Dummy data for filling the array size
};

uint8_t m_beacon_info_150B[CODEC_DATA_SIZE_150B+APP_BEACON_INFO_LENGTH] =                    //< Information advertised by the Beacon. 
{
    APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                         // implementation.
    APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                         // manufacturer specific data in this implementation.
    APP_BEACON_UUID,     // 128 bit UUID value.
    DEF_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
    DEF_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
    APP_MEASURED_RSSI,    // Manufacturer specific information. The Beacon's measured TX power in
                         // this implementation.
    MESSAGE_TEST_TYPE,   // 23 
    COORDINATOR_ID,      // 24 
    SLAVE_ID,            // 25
    DEF_NUM_ADV_RECEIVED,// 26
    DEF_MEAN_RSSI,       // 27
    [APP_BEACON_INFO_LENGTH ... CODEC_DATA_SIZE_150B+APP_BEACON_INFO_LENGTH-1] = 0 //Dummy data for filling the array size
};

uint8_t m_beacon_info_200B[CODEC_DATA_SIZE_200B+APP_BEACON_INFO_LENGTH] =                    //< Information advertised by the Beacon. 
{
    APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                         // implementation.
    APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                         // manufacturer specific data in this implementation.
    APP_BEACON_UUID,     // 128 bit UUID value.
    DEF_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
    DEF_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
    APP_MEASURED_RSSI,    // Manufacturer specific information. The Beacon's measured TX power in
                         // this implementation.
    MESSAGE_TEST_TYPE,   // 23 
    COORDINATOR_ID,      // 24 
    SLAVE_ID,            // 25
    DEF_NUM_ADV_RECEIVED,// 26
    DEF_MEAN_RSSI,       // 27
    [APP_BEACON_INFO_LENGTH ... CODEC_DATA_SIZE_200B+APP_BEACON_INFO_LENGTH-1] = 0 //Dummy data for filling the array size
};

uint8_t m_beacon_info_250B[CODEC_DATA_SIZE_250B+APP_BEACON_INFO_LENGTH] =                    //< Information advertised by the Beacon. 
{
    APP_DEVICE_TYPE,     // Manufacturer specific information. Specifies the device type in this
                         // implementation.
    APP_ADV_DATA_LENGTH, // Manufacturer specific information. Specifies the length of the
                         // manufacturer specific data in this implementation.
    APP_BEACON_UUID,     // 128 bit UUID value.
    DEF_MAJOR_VALUE,     // Major arbitrary value that can be used to distinguish between Beacons.
    DEF_MINOR_VALUE,     // Minor arbitrary value that can be used to distinguish between Beacons.
    APP_MEASURED_RSSI,    // Manufacturer specific information. The Beacon's measured TX power in
                         // this implementation.
    MESSAGE_TEST_TYPE,   // 23 
    COORDINATOR_ID,      // 24 
    SLAVE_ID,            // 25
    DEF_NUM_ADV_RECEIVED,// 26
    DEF_MEAN_RSSI,       // 27
    [APP_BEACON_INFO_LENGTH ... CODEC_DATA_SIZE_250B+APP_BEACON_INFO_LENGTH-1] = 0 //Dummy data for filling the array size
};


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt.
 */
void ble_stack_init(void)
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


/**@brief Function for handling BLE Stack events.
 *
 * @param[in] p_ble_evt  Bluetooth stack event.
 */
void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context)
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
        case BLE_GAP_EVT_ADV_SET_TERMINATED:
            NRF_LOG_INFO("Advertisements terminado. Motivo: %d. Paso a SCAN pero ya de ya.", p_ble_evt->evt.gap_evt.params.adv_set_terminated.reason);
            m_adv_sent_led_show(NULL, false, NULL);
            /*err_code = app_timer_stop(m_adv_sent_led_show_timer_id); 
            APP_ERROR_CHECK(err_code);*/
            countAdvReceived = 0; //Por seguridad para evitar desborde rssiValues...
            scan_start();
            break;

         case BLE_GAP_EVT_TIMEOUT: //Not necessary, my continuous scan will stop by a timer
            /*NRF_LOG_INFO("Scan timeout Expired!!! Wait short time (m_timer_ble timer) before start the adv again");
            ret_code_t err_code = app_timer_start(m_timer_ble, ADV_EVT_INTERVAL, NULL); 
            APP_ERROR_CHECK(err_code);*/
            break;

        default:
        {
          NRF_LOG_DEBUG("Received an unimplemented BLE event.");
            // No implementation needed.
        } break;
    }
}




/**@brief Function for stopping advertising.
 */
void scan_stop(void)
{
    ret_code_t err_code;

    err_code = sd_ble_gap_scan_stop();
    APP_ERROR_CHECK(err_code);
    NRF_LOG_INFO("Stopping scanning");
}

/**@brief Function to start scanning.
 * Scanning is started based on the internal parameters (global variables) set.
 */
void scan_start(void) 
{
   ret_code_t err_code;

   // If already scanning, stop scanning.
   (void) sd_ble_gap_scan_stop();
     
   // Set the correct TX power.
   err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_SCAN_INIT, NULL, m_output_power_selected);
   APP_ERROR_CHECK(err_code);

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
  
}

/**@brief Function for handling BLE_GAP_ADV_REPORT events.
 * Log adv report upon device name match or if "non-connectable" advertising on coded phy is received.
 * If in state "scanning, trying to connect", send connection request to device with matching device name.
 */
void on_adv_report(ble_gap_evt_adv_report_t const * p_adv_report)
{
    static int8_t   rssi_value = 0;
    // Declarar una variable para almacenar el UUID
    uint8_t uuid_data[16];
    ret_code_t err_code;

    NRF_LOG_INFO("************************************************************");
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

    if (1)//m_scan_type_selected == SELECTION_SCAN_NON_CONN
    {
      //Procesamiento para solo quedarnos con los UUID
        if (/*(p_adv_report->data.len == 30)  || De momento quito esto
            (p_adv_report->data.len == 255) || 
            (p_adv_report->data.len == 205) || 
            (p_adv_report->data.len == 155) || 
            (p_adv_report->data.len == 105) ||
            (p_adv_report->data.len == 55)*/
            p_adv_report->data.p_data[IDX_SLAVE_ID_RX] == SLAVE_ID)
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
                // Comparar el UUID almacenado con el UUID definido
                if (memcmp(uuid_data, APP_BEACON_UUID_POINTER, sizeof(uuid_data)) == 0)
                {               
                    bsp_board_led_invert(SCAN_LED);

                    NRF_LOG_INFO("UUID matches the defined UUID!");
                    NRF_LOG_RAW_HEXDUMP_INFO(p_adv_report->data.p_data, p_adv_report->data.len); 

                    // Mando paquete por UART -> not now! the scan will obtain info that will be sent once timer expired
                    //send_adv(p_adv_report);
                    
                    //Antes de nada, saber cuántos adv tengo en este ciclo ping-pong, y cómo vienen de espaciados. Cuando lo sepa, puedo hilar fino en timers
                    num_adv_2_send = p_adv_report->data.p_data[IDX_NUM_ADV_SENT_RX];
                    uint8_t *pChar;
                    pChar = (unsigned char*)&time_between_advs;
                    pChar[0] = p_adv_report->data.p_data[IDX_TIME_BETW_ADV_LSB_RX];
                    pChar[1] = p_adv_report->data.p_data[IDX_TIME_BETW_ADV_LSB_RX-1];

                    if (advReceived == false)
                    {
                        advReceived = true;
                        uint16_t timeExpected = (80 + 256 + 16 + 24 + 8*8*(p_adv_report->data.len+8) + 192 + 24)/1000; //Time expected for receiving one adv
                        uint16_t extraTime = 1000;
                        m_timeout_for_advertise(APP_TIMER_TICKS(timeExpected*num_adv_2_send + (num_adv_2_send-1)*time_between_advs + extraTime), true, NULL);
                        /*err_code = app_timer_start(m_timeout_for_advertise, APP_TIMER_TICKS(timeExpected*NUM_ADVERTISEMENTS+extraTime), NULL);
                        APP_ERROR_CHECK(err_code);*/
                        NRF_LOG_INFO("Time expected for 1 adv: %dms.", timeExpected);
                        NRF_LOG_INFO("Time ms betw advs %dms", time_between_advs);
                        NRF_LOG_INFO("Primer ADV recibido, arranco timer que dura %dms!", timeExpected*num_adv_2_send + (num_adv_2_send-1)*time_between_advs + extraTime);
                    }
                    uint8_t * pLong;
                    uint32_t nseq;
                    pLong = (unsigned char*)&nseq;
                    pLong[0] = p_adv_report->data.p_data[IDX_MINOR_RX+1];
                    pLong[1] = p_adv_report->data.p_data[IDX_MINOR_RX];
                    pLong[2] = p_adv_report->data.p_data[IDX_MAJOR_RX+1];
                    pLong[3] = p_adv_report->data.p_data[IDX_MAJOR_RX];
                    minorValue = nseq & 0xFFFF;
                    majorValue = (nseq & 0xFFFF0000) >> 16;

                    coordinatorID = p_adv_report->data.p_data[IDX_COORD_ID_RX];
                    messageType = p_adv_report->data.p_data[IDX_MSG_TYPE_RX];

                    output_power_seclection_t aux_tx_power = p_adv_report->data.p_data[IDX_TX_POWER_RX];

                    uint8_t aux_data_size;
                    if (p_adv_report->data.len == 55) {
                      aux_data_size = CODEC_DATA_SIZE_50B;
                    } else if (p_adv_report->data.len == 105) {
                      aux_data_size = CODEC_DATA_SIZE_100B;
                    } else if (p_adv_report->data.len == 155) {
                      aux_data_size = CODEC_DATA_SIZE_150B;
                    } else if (p_adv_report->data.len == 205) {
                      aux_data_size = CODEC_DATA_SIZE_200B;
                    } else if (p_adv_report->data.len == 255) {
                      aux_data_size = CODEC_DATA_SIZE_250B;
                    } else {
                      NRF_LOG_INFO("FALLO AL INFERIR DATA SIZE");
                    }
                    


                    if (nSeqReceived != nseq || aux_data_size != m_codec_phy_data_size || m_output_power_selected != aux_tx_power) {
                        if (m_output_power_selected != p_adv_report->data.p_data[IDX_TX_POWER_RX]) {
                          output_power_selection_set(p_adv_report->data.p_data[IDX_TX_POWER_RX]);
                        }
                        
                        if (aux_data_size != m_codec_phy_data_size) {
                          /* As the change adv size is supported, it is needed to restart the .len byte of m_adv_data_ext, because in ble_advdata_encode
                             a pointer to .len is passed, so next time the function is called, the size is setted to before len array. It is needed to restart
                             each time the adv data size is changed.
                          */
                          m_adv_data_ext.adv_data.len = BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED;
                          m_codec_phy_data_size = aux_data_size;
                        }

                        int16_t acum = 0;
                        for (uint8_t i = 0; i < countAdvReceived; i++) {
                          acum += rssiValues[i];
                        }
                        NRF_LOG_INFO("OJO, Nuevo nseq recibido: %d. O diferente tamaño %d vs %d. Antes: %d con rssi media de %d", nseq, aux_data_size, m_codec_phy_data_size, nSeqReceived, acum/countAdvReceived);
                        nSeqReceived = nseq;
                        countAdvReceived = 0; //If a new nseq is received, reset the count of adv of the same nseq.
                    }

                    rssiValues[countAdvReceived++] = p_adv_report->rssi;
                    NRF_LOG_INFO("Recibo adv para nseq %d. Llevo %d", nSeqReceived, countAdvReceived);
                    

                }
                else
                {
                    NRF_LOG_INFO("UUID does not match the defined UUID.");
                }                           
                
              NRF_LOG_INFO("************************************************************");
       } else {
          NRF_LOG_INFO("Pues resulta que el slave ID no coincide con el mío. Recibo %d y tengo %d", p_adv_report->data.p_data[IDX_SLAVE_ID_RX], SLAVE_ID);
       }

      // Continue scanning
       err_code = sd_ble_gap_scan_start(NULL, &m_scan_buffer);
       APP_ERROR_CHECK(err_code);
         
      return;
     }
}



/**@brief Function for initializing the Advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 *          Also builds a structure to be passed to the stack when starting advertising.
 */
void advertising_init(void)
{
    ret_code_t ret;

    uint8_t * adv_pdu;
    uint16_t size;

    ble_advdata_manuf_data_t manuf_specific_data;

    manuf_specific_data.company_identifier = APP_COMPANY_IDENTIFIER;
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
          size = CODEC_DATA_SIZE_50B+APP_BEACON_INFO_LENGTH;
        break;
        case CODEC_DATA_SIZE_100B:
   //     NRF_LOG_INFO("Selecciono 100B");
          adv_pdu = m_beacon_info_100B;
          size = CODEC_DATA_SIZE_100B+APP_BEACON_INFO_LENGTH;
        break;
        case CODEC_DATA_SIZE_150B:
   //     NRF_LOG_INFO("Selecciono 150B, y tengo en memoria %d", CODEC_DATA_SIZE_150B+APP_BEACON_INFO_LENGTH);
          adv_pdu = m_beacon_info_150B;
          size = CODEC_DATA_SIZE_150B+APP_BEACON_INFO_LENGTH;
        break;
        case CODEC_DATA_SIZE_200B:
   //     NRF_LOG_INFO("Selecciono 200B");
          adv_pdu = m_beacon_info_200B;
          size = CODEC_DATA_SIZE_200B+APP_BEACON_INFO_LENGTH;
        break;
        case CODEC_DATA_SIZE_250B:
   //     NRF_LOG_INFO("Selecciono 250B");
        default:
          adv_pdu = m_beacon_info_250B;
          size = CODEC_DATA_SIZE_250B+APP_BEACON_INFO_LENGTH;
        break;
      }
    } else { // Por si un caso algo falla, cojo el del advertisement normal
    NRF_LOG_INFO("Selecciono 30B defecto!");
      adv_pdu = m_beacon_info;
      size = APP_BEACON_INFO_LENGTH;
    }

    //Fill the variables values inside the adv PDU according the previous advertisements received
    adv_pdu[APP_MINOR_POSITION]   = (uint8_t)(minorValue & 0xFF);
    adv_pdu[APP_MINOR_POSITION-1]   = (uint8_t)((minorValue & 0xFF00) >> 8);
    adv_pdu[APP_MAJOR_POSITION]   = (uint8_t)(majorValue & 0xFF);
    adv_pdu[APP_MAJOR_POSITION-1]   = (uint8_t)((majorValue & 0xFF00) >> 8);
    adv_pdu[IDX_MESSAGE_TYPE]     = messageType;
    adv_pdu[IDX_COORDINATOR_ID]   = coordinatorID;
    adv_pdu[IDX_SLAVE_ID] = SLAVE_ID;
    //The SLAVE_ID is always sent as the default value (it is flashed).
    //adv_pdu[IDX_NSEQ]             = nSeqReceived; //So, as i response with the same nseq (in major/minor fields), this field is not necessary
    adv_pdu[IDX_NUM_ADV_RECIEVED] = countAdvReceived;

    int16_t acumRssi = 0;
    for (uint8_t i = 0; i < countAdvReceived; i++) {
        acumRssi += rssiValues[i];       
    }
    NRF_LOG_INFO("Acum rssi: %d", acumRssi);
    adv_pdu[IDX_MEAN_RSSI] = (int8_t) (acumRssi/countAdvReceived);


    NRF_LOG_INFO("Inicializo advertising. Major: %04X, Minor %04X. Size %d. RSSI: %d (0x%02X). Data size: %d", majorValue, minorValue, size, (int8_t) (acumRssi/countAdvReceived), (int8_t) (acumRssi/countAdvReceived), m_codec_phy_data_size);
    /*for (uint8_t i = 0; i < size; i++) {
      NRF_LOG_INFO("%02X ", adv_pdu[i]);
    }*/
    manuf_specific_data.data.p_data = (uint8_t *) adv_pdu;
    manuf_specific_data.data.size   = size;


    ble_gap_adv_params_t adv_params =
    {
        .properties    =
        {
          .type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED,
        },
        .p_peer_addr   = NULL,
        .filter_policy = BLE_GAP_ADV_FP_ANY,
        .interval      = MSEC_TO_UNITS(time_between_advs, UNIT_0_625_MS),
        .duration      = 0,
        .max_adv_evts  = num_adv_2_send,

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
        
        //Here was the distinguision of type (connectable or not) ot the advertisement. I set it always nonconnectable
        adv_params.properties.type = BLE_GAP_ADV_TYPE_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED;

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
        
        //Here was the distinguision of type (connectable or not) ot the advertisement. I set it always nonconnectable
        adv_params.properties.type = BLE_GAP_ADV_TYPE_EXTENDED_NONCONNECTABLE_NONSCANNABLE_UNDIRECTED;

        ret = ble_advdata_encode(&adv_data, m_adv_data_ext.adv_data.p_data, &m_adv_data_ext.adv_data.len);
        APP_ERROR_CHECK(ret);

        ret = sd_ble_gap_adv_set_configure(&m_adv_handle, &m_adv_data_ext, &adv_params);
        APP_ERROR_CHECK(ret);			
    }
}


/**@brief Function for starting advertising.
 */
void advertising_start(void)
{
    ret_code_t err_code;

    err_code = sd_ble_gap_tx_power_set(BLE_GAP_TX_POWER_ROLE_ADV, m_adv_handle, m_output_power_selected);
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_adv_start(m_adv_handle, APP_BLE_CONN_CFG_TAG);
    //NRF_LOG_INFO("err_code after sd_ble_gap_adv_start: %d", err_code); 
    APP_ERROR_CHECK(err_code);    

    m_adv_sent_led_show(BLINK_SEDNDING_ADV, true, NULL);
    /*err_code = app_timer_start(m_adv_sent_led_show_timer_id, BLINK_SEDNDING_ADV, NULL);
    APP_ERROR_CHECK(err_code);*/

}

/**@brief Function for stopping advertising, if advertising.
 */
void advertising_stop(void)
{
    ret_code_t err_code;

    NRF_LOG_INFO("Entro a parar el advertising");
    (void) sd_ble_gap_adv_stop(m_adv_handle);
    NRF_LOG_INFO("Stopping advertising");
}

/**@brief Function for start scanning with the current selections of output power, phy, and scanning "mode".
 */
void set_current_scan_params_and_start_scanning(void)
{
  
  phy_selection_set_state(m_adv_scan_phy_selected);
  //scan_selection_state_set(m_scan_type_selected);
  output_power_selection_set(m_output_power_selected);

  scan_start();

}


