#include "timers.h"
#include "app_timer.h"
#include "boards.h"
#include "config.h"
#include "bleModule.h"
#include "uart.h"


  APP_TIMER_DEF(m_adv_data_size_100_codec_timer_id);
  APP_TIMER_DEF(m_adv_data_size_150_codec_timer_id);
  APP_TIMER_DEF(m_adv_data_size_200_codec_timer_id);

  APP_TIMER_DEF(m_timer_ble);
  APP_TIMER_DEF(m_adv_sent_led_show_timer_id);

  APP_TIMER_DEF(m_time_for_metrics_packet);
  APP_TIMER_DEF(m_analize_buffer_uart);

  void adv_interval_timeout_handler(void *p_context) //NOW starting Adv is not using timer, but it is done via uart
  {
    UNUSED_PARAMETER(p_context);
    // Inicializamos características advertisement
    #ifdef AUTONOMO   
      disconnect_stop_adv();
      advertising_init();
      advertising_start();
    #endif
  }

  void adv_sent_led_show_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(SENDING_ADV_LED);
  }

  void conn_adv_fast_blink_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(NON_CONN_ADV_LED);
  }

  void adv_data_size_100_codec_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(CONN_ADV_CONN_STATE_LED);
  }

  void adv_data_size_200_codec_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(CONN_ADV_CONN_STATE_LED);
  }

  void adv_data_size_150_codec_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(CONN_ADV_CONN_STATE_LED);
  }

  void led_1Mbps_slow_blink_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(PHY_SELECTION_LED);
  }

  void led_8dBm_slow_blink_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    bsp_board_led_invert(OUTPUT_POWER_SELECTION_LED);
  }

  void time_for_metrics_packet_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    scan_stop();

    send_metrics();

    // On this version, the sending adv stage is not automatic, it is managed by MODBUS packets
    //ret_code_t err_code = app_timer_start(m_timer_ble, ADV_EVT_INTERVAL, NULL);
    //APP_ERROR_CHECK(err_code);
    #ifdef AUTONOMO 
      adv_interval(ADV_EVT_INTERVAL, true, NULL);
    #endif
  }

  void analize_buffer_uart_timeout_handler(void *p_context) {
    UNUSED_PARAMETER(p_context);
    AnalizaBufferUART();
  }

  /**@brief Function for initializing timers. */
  void timers_init(void) {
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);

    // Single shot! porque solo quiero que salte 1 vez, cuando nos vayamos a dormir (reposo ciclo envío adv)
    err_code = app_timer_create(&m_timer_ble, APP_TIMER_MODE_SINGLE_SHOT, adv_interval_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_adv_sent_led_show_timer_id, APP_TIMER_MODE_REPEATED, adv_sent_led_show_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_adv_data_size_100_codec_timer_id, APP_TIMER_MODE_REPEATED, adv_data_size_100_codec_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_adv_data_size_150_codec_timer_id, APP_TIMER_MODE_REPEATED, adv_data_size_150_codec_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_adv_data_size_200_codec_timer_id, APP_TIMER_MODE_REPEATED, adv_data_size_200_codec_timeout_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_time_for_metrics_packet, APP_TIMER_MODE_SINGLE_SHOT, time_for_metrics_packet_handler);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_create(&m_analize_buffer_uart, APP_TIMER_MODE_REPEATED, analize_buffer_uart_timeout_handler);
    APP_ERROR_CHECK(err_code);
  }

  void adv_interval(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_timer_ble, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_timer_ble);
    }
    
    APP_ERROR_CHECK(err_code);
  }
  void adv_sent_led_show(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_adv_sent_led_show_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_adv_sent_led_show_timer_id);
    }
    APP_ERROR_CHECK(err_code);
  }
  void time_for_metrics_packet(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_time_for_metrics_packet, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_time_for_metrics_packet);
    }
    APP_ERROR_CHECK(err_code);
  }
  void analize_buffer_uart(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_analize_buffer_uart, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_analize_buffer_uart);
    }
    APP_ERROR_CHECK(err_code);
  }
  void adv_data_size_100_codec(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_adv_data_size_100_codec_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_adv_data_size_100_codec_timer_id);
    }
    APP_ERROR_CHECK(err_code);
  }
  void adv_data_size_150_codec(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_adv_data_size_150_codec_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_adv_data_size_150_codec_timer_id);
    }
    APP_ERROR_CHECK(err_code);
  }
  void adv_data_size_200_codec(uint32_t timer_ticks, bool start, void * p_context) {
    uint32_t err_code;
    if (start) {
      err_code = app_timer_start(m_adv_data_size_200_codec_timer_id, timer_ticks, p_context);
    } else {
      err_code = app_timer_stop(m_adv_data_size_200_codec_timer_id);
    }
    APP_ERROR_CHECK(err_code);
  }