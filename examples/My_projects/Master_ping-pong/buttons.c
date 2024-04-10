#include "buttons.h"
#include "timers.h"
#include "nrf_log.h"

/**@brief Function for setting new output power selection: LEDs start to blink according to the selected state.
   * Note: this function sets the output power/TX power.
   *       The new output power will be set when (re-)starting to scan.
   */
  void output_power_selection_set(output_power_seclection_t output_power) {
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
      default: {
        NRF_LOG_INFO("Tx power not supported (%d). Restarting to 8dBm...", m_output_power_selected);
        m_output_power_selected = SELECTION_8_dBm;
        // 8 dBm is the current output power, off LED.
        bsp_board_led_off(OUTPUT_POWER_SELECTION_LED); // not necessary because the LED should start to blink.
      } break;
    }
  }



  /**@brief Function for switching output power/TX power: 0 dBm or 8 dBm.
   * Note:  this function does only set the internal state, it does apply the new setting.
   *        The new output power will be set when (re-)starting to scan.
   */
  void on_output_power_selection_button(void) {
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
  void on_non_conn_or_conn_adv_selection_state_set(adv_scan_type_seclection_t adv_selection) {
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
  void on_non_conn_or_conn_adv_selection(void) {
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
  void on_adv_data_size_selection_set(adv_codec_phy_data_size_t adv_data_size) {
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
  void phy_selection_set_state(adv_scan_phy_seclection_t new_phy_selection) {
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
  void on_phy_selection_button(void) {
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
    //    on_phy_selection_button();
      } break;

      case OUTPUT_POWER_SELECTION_BUTTON_EVENT: {
    //    on_output_power_selection_button();
      } break;

      case NON_CONN_OR_CONN_ADV_BUTTON_EVENT: // Esto ahora actúa como modificador del tamaño de los datos!
      {
        // on_non_conn_or_conn_adv_selection();
    //    on_adv_data_size_selection();
      } break;

      default:
        break;
      }

    //  disconnect_stop_adv();
    //  advertising_init();
    //  advertising_start();
    }
  }

