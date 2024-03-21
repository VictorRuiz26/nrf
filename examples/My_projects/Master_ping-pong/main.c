
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
#include "buttons.h"


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