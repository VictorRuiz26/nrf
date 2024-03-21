
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
#include "bleModule.h"


//TODO: si declaro una variable así y luego la inicializo, puedo meterla en la inicialización de scan
  ble_gap_scan_params_t m_scan_param_coded_phy_2;
 /*m_scan_param_coded_phy_2.extended = 1;
  m_scan_param_coded_phy_2.active = 0x01;
  m_scan_param_coded_phy_2.interval = NRF_BLE_SCAN_SCAN_INTERVAL;
  m_scan_param_coded_phy_2.window = NRF_BLE_SCAN_SCAN_WINDOW;
  m_scan_param_coded_phy_2.timeout = (TIME_10MS_1ADV * VAR_NUM_ADVERTISEMENTS) + EXTRA_SCAN_DURATION;
  m_scan_param_coded_phy_2.scan_phys = BLE_GAP_PHY_CODED;
  m_scan_param_coded_phy_2.filter_policy = BLE_GAP_SCAN_FP_ACCEPT_ALL;*/

  static void instructions_print(void) {
    NRF_LOG_INFO("Press the buttons to set up the advertiser in wanted mode:");
    NRF_LOG_INFO("Button 2: switch between coded phy (LED2 setted on) and 1Mbps (LED2 blinking)");
    NRF_LOG_INFO("Button 3: switch between 0 dbm (LED3 setted on) and 8 dBm output power (LED3 blinkink).");
    // NRF_LOG_INFO("Button 4: switch between non-connectable (slow blink LED 4) and connectable advertising (fast blink LED4).");
    NRF_LOG_INFO("Button 4: rotate between 50B (LED4 Off), 100B(LED4 slow blinking), 150B(LED4 medium blinking), 200B(LED4 fast blinking) and 250B(LED4 on) adv data size (only codec PHY!)");
  }

  

  /**@brief This function will disconnect if connected, and stop advertising if advertising. */
  static void disconnect_stop_adv(void) {
    ret_code_t err_code;
    // If advertising, stop advertising.
    (void)sd_ble_gap_adv_stop(m_adv_handle);
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