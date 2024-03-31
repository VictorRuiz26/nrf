#ifndef BUTTONS_H__
#define BUTTONS_H__
  #include "data.h"
  #include "config.h"
  #include "bleModule.h"
  #include "bsp.h"

  #define PHY_SELECTION_BUTTON                  BSP_BUTTON_0
  #define PHY_SELECTION_BUTTON_EVENT            BSP_EVENT_KEY_0
  #define OUTPUT_POWER_SELECTION_BUTTON         BSP_BUTTON_1
  #define OUTPUT_POWER_SELECTION_BUTTON_EVENT   BSP_EVENT_KEY_1
  #define SCAN_SELECTION_BUTTON                 BSP_BUTTON_2
  #define SCAN_SELECTION_BUTTON_EVENT           BSP_EVENT_KEY_2
  #define BUTTON_NOT_IN_USE                     BSP_BUTTON_3
  #define BUTTON_NOT_IN_USE_EVENT               BSP_EVENT_KEY_3

  void buttons_leds_init(void);

  void output_power_selection_set(output_power_seclection_t output_power);
  void on_output_power_selection_button(void);
  void on_adv_data_size_selection_set(adv_codec_phy_data_size_t adv_data_size);
  void phy_selection_set_state(adv_scan_phy_seclection_t new_phy_selection);
  void on_phy_selection_button(void);
  void bsp_evt_handler(bsp_event_t event);
#endif