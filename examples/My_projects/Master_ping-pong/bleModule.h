#ifndef BLEMODULE_H__
#define BLEMODULE_H__
  #include "data.h"
  #include "config.h"
  #include "ble_gap.h"
  #include "ble_advdata.h"
  
  #define TIME_MS_1ADV(size) ((80 + 256 + 16 + 24 + 8 * 8 * (size + 8) + 192 + 24) / 1000) // Due to const definition problems, use const CODEC_DATA_SIZE_50B at first tests
  #define TIME_10MS_1ADV(size) (TIME_MS_1ADV(size) / 10)
  #define EXTRA_SCAN_DURATION 500 // Extra 5s for scanning (in 10ms units)

  //TODO: PARA SER MAS EXACTOS, METER AQUÍ EL TIEMPO ENTRE ADV QUE ES UNA VARIABLE CONOCIDA, Y SON ms QUE, 
  //      CON ESTOS CAMBIOS, EL ESCLAVO PARAMETRIZA. time between viene dado en ms, tengo que pasaro a ud 10ms
  #define TIMEOUT_SCAN_ADV(num, size, time_between) (TIME_10MS_1ADV(size)*num + (num-1)*(time_between/10) + EXTRA_SCAN_DURATION)

  #define APP_BLE_CONN_CFG_TAG 1  /**< A tag that refers to the BLE stack configuration. */
  #define APP_BLE_OBSERVER_PRIO 3 /**< Application's BLE observer priority. You shouldn't need to modify this value. */

  #define MODBUS_DATA_SIZE_50    0
  #define MODBUS_DATA_SIZE_100   1
  #define MODBUS_DATA_SIZE_150   2
  #define MODBUS_DATA_SIZE_200   3
  #define MODBUS_DATA_SIZE_250   4

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

  typedef struct {
    uint8_t *adv_pdu;
    uint16_t size;
  } adv_pdu_t;

  extern adv_pdu_t adv_PDU;

  extern adv_scan_type_seclection_t m_adv_scan_type_selected; /**< Global variable holding the current scan selection mode. */
  extern adv_scan_phy_seclection_t m_adv_scan_phy_selected;         /**< Global variable holding the current phy selection. */
  extern output_power_seclection_t m_output_power_selected;             /**< Global variable holding the current output power selection. */
  extern bool m_app_initiated_disconnect;                                         // The application has initiated disconnect. Used to "tell" on_ble_gap_evt_disconnected() to not start advertising.
  extern bool m_waiting_for_disconnect_evt;                                       // Disconnect is initiated. The application has to wait for BLE_GAP_EVT_DISCONNECTED before proceeding.

  extern adv_codec_phy_data_size_t m_codec_phy_data_size;

  extern ble_gap_adv_params_t m_adv_params;                                           /**< Parameters to be passed to the stack when starting advertising. */
  extern uint8_t m_adv_handle;                       /**< Advertising handle used to identify an advertising set. */
  extern uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];                        /**< Buffer for storing an encoded advertising set. */
  extern uint8_t m_enc_advdata_ext[BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED]; /**< Buffer for storing an encoded advertising set for codec PHY. */

  extern ble_gap_adv_data_t m_adv_data;
  extern ble_gap_adv_data_t m_adv_data_ext;

  extern ble_data_t m_scan_buffer;
  extern uint8_t m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN];

  extern ble_gap_scan_params_t m_scan_param_1MBps;
  extern ble_gap_scan_params_t m_scan_param_coded_phy;

  extern uint16_t time_between_advs;
  extern uint8_t num_adv_2_send;

  extern uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_50B[CODEC_DATA_SIZE_50B + APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_100B[CODEC_DATA_SIZE_100B + APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_150B[CODEC_DATA_SIZE_150B + APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_200B[CODEC_DATA_SIZE_200B + APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_250B[CODEC_DATA_SIZE_250B + APP_BEACON_INFO_LENGTH];

  void ble_stack_init(void);
  void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context);

  void scan_stop(void);
  void scan_start(void);
  void on_adv_report(ble_gap_evt_adv_report_t const *p_adv_report);

  void advertising_init(void);
  void advertising_start(void);
  void advertising_stop(void);

  void set_current_adv_params_and_start_advertising(void);

  void disconnect_stop_adv(void);

  void getAdvPDU (uint8_t dataSize, adv_pdu_t *PDU);

  unsigned char getRealPDUSize(adv_codec_phy_data_size_t data_size);

#endif