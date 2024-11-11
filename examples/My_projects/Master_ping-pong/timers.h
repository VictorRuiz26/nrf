#ifndef TIMERS_H__
#define TIMERS_H__
  #include "app_timer.h"
  #include "data.h"
  #include <time.h>

  #define TIMERS_LEDS 0

    // ******* TIMER DEFINITIONS *******
  #define ADV_EVT_INTERVAL APP_TIMER_TICKS(SEGUNDOS_DELAY * 1000)
  #define FAST_BLINK_INTERVAL APP_TIMER_TICKS(300)
  #define MEDIUM_BLINK_INTERVAL APP_TIMER_TICKS(650)
  #define SLOW_BLINK_INTERVAL APP_TIMER_TICKS(1000)
  #define BLINK_SENDING_ADV APP_TIMER_TICKS(100)

  void timers_init                              (void);

  void adv_interval_timeout_handler             (void *p_context);
  void adv_sent_led_show_timeout_handler        (void *p_context);
  void conn_adv_fast_blink_timeout_handler      (void *p_context);
  void adv_data_size_100_codec_timeout_handler  (void *p_context);
  void adv_data_size_200_codec_timeout_handler  (void *p_context);
  void adv_data_size_150_codec_timeout_handler  (void *p_context);
  void led_1Mbps_slow_blink_timeout_handler     (void *p_context);
  void led_8dBm_slow_blink_timeout_handler      (void *p_context);
  void time_for_metrics_packet_handler          (void *p_context);

  void analize_buffer_uart_timeout_handler      (void *p_context);

  //Como los timers_id se definen en el SDK como static, debo crear funciones que actúen a modo de API para arrancara cada timer.
  void adv_interval             (uint32_t timer_ticks, bool start, void * p_context);
  void adv_sent_led_show        (uint32_t timer_ticks, bool start, void * p_context);
  void time_for_metrics_packet  (uint32_t timer_ticks, bool start, void * p_context);
  void analize_buffer_uart      (uint32_t timer_ticks, bool start, void * p_context);
  void adv_data_size_100_codec  (uint32_t timer_ticks, bool start, void * p_context);
  void adv_data_size_150_codec  (uint32_t timer_ticks, bool start, void * p_context);
  void adv_data_size_200_codec  (uint32_t timer_ticks, bool start, void * p_context);


#endif