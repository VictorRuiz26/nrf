#ifndef BUTTONS_H__
#define BUTTONS_H__
  #include "data.h"
  #include "config.h"
  #include "bleModule.h"

  void output_power_selection_set(output_power_seclection_t output_power);
  void on_output_power_selection_button(void);
  void on_non_conn_or_conn_adv_selection_state_set(adv_scan_type_seclection_t adv_selection);
  void on_non_conn_or_conn_adv_selection(void);
  void on_adv_data_size_selection_set(adv_codec_phy_data_size_t adv_data_size);
  void phy_selection_set_state(adv_scan_phy_seclection_t new_phy_selection);
  void on_phy_selection_button(void);
  void bsp_evt_handler(bsp_event_t event);
#endif