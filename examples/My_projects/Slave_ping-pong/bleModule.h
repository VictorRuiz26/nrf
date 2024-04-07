#ifndef BLEMODULE_H__
#define BLEMODULE_H__
  #include "ble.h"
  #include "ble_gap.h"
  #include "ble_hci.h"
  #include "ble_advdata.h"
  #include "nrf_ble_scan.h"
  #include "config.h"

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

  // Para asignar dinámicamente los datos del adv
  typedef enum
  {
    CODEC_DATA_SIZE_50B  = 50-APP_BEACON_INFO_LENGTH-AD_TYPE_MANUF_SPEC_DATA_ID_SIZE,
    CODEC_DATA_SIZE_100B = 100-APP_BEACON_INFO_LENGTH-AD_TYPE_MANUF_SPEC_DATA_ID_SIZE,
    CODEC_DATA_SIZE_150B = 150-APP_BEACON_INFO_LENGTH-AD_TYPE_MANUF_SPEC_DATA_ID_SIZE,
    CODEC_DATA_SIZE_200B = 200-APP_BEACON_INFO_LENGTH-AD_TYPE_MANUF_SPEC_DATA_ID_SIZE,
    CODEC_DATA_SIZE_250B = 250-APP_BEACON_INFO_LENGTH-AD_TYPE_MANUF_SPEC_DATA_ID_SIZE
  } adv_codec_phy_data_size_t;


  extern adv_codec_phy_data_size_t m_codec_phy_data_size;

  extern uint8_t m_adv_handle;
  extern uint8_t m_enc_advdata[BLE_GAP_ADV_SET_DATA_SIZE_MAX];
  extern uint8_t m_enc_scandata[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN];

  //extern scan_type_seclection_t    m_scan_type_selected;
  extern adv_scan_phy_seclection_t m_adv_scan_phy_selected; 
  extern output_power_seclection_t m_output_power_selected;

  extern uint8_t m_scan_buffer_data[BLE_GAP_SCAN_BUFFER_EXTENDED_MIN];

  extern ble_data_t m_scan_buffer;

  extern ble_gap_scan_params_t m_scan_param_coded_phy;
  extern ble_gap_scan_params_t m_scan_param_1MBps;

  extern uint8_t m_enc_advdata     [BLE_GAP_ADV_SET_DATA_SIZE_MAX];
  extern uint8_t m_enc_advdata_ext [BLE_GAP_ADV_SET_DATA_SIZE_EXTENDED_MAX_SUPPORTED];

  extern ble_gap_adv_data_t m_adv_data;
  extern ble_gap_adv_data_t m_adv_data_ext;


  extern uint16_t time_between_advs;
  extern uint8_t num_adv_2_send;

  extern uint8_t m_beacon_info[APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_50B[CODEC_DATA_SIZE_50B+APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_100B[CODEC_DATA_SIZE_100B+APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_150B[CODEC_DATA_SIZE_150B+APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_200B[CODEC_DATA_SIZE_200B+APP_BEACON_INFO_LENGTH];
  extern uint8_t m_beacon_info_250B[CODEC_DATA_SIZE_250B+APP_BEACON_INFO_LENGTH];

  void ble_stack_init(void);
  void ble_evt_handler(ble_evt_t const * p_ble_evt, void * p_context);

  void scan_stop(void);
  void scan_start(void);
  void on_adv_report(ble_gap_evt_adv_report_t const * p_adv_report);

  void advertising_init(void);
  void advertising_start(void);
  void advertising_stop(void);

  void set_current_scan_params_and_start_scanning(void);

  unsigned char getRealPDUSize(adv_codec_phy_data_size_t data_size);

#endif