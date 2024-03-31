#ifndef TIMERS_H__
#define TIMERS_H__
  #include <time.h>
  #include <stdint.h>
  #include <stdbool.h>

  #define FAST_BLINK_INTERVAL		APP_TIMER_TIKCS(200) 
  #define SLOW_BLINK_INTERVAL		APP_TIMER_TICKS(750)
  #define BLINK_SEDNDING_ADV            APP_TIMER_TICKS(100)

  void led_1Mbps_slow_blink_timeout_handler(void * p_context);
  void adv_sent_led_show_timeout_handler (void *p_context);
  void led_8dBm_slow_blink_timeout_handler(void * p_context);
  void timeout_for_advertise_timeout_handler(void * p_context);
  void time_keeper_timeout_handler(void * p_context);

  void timers_init(void);

  void m_1Mbps_led_slow_blink(uint32_t timer_ticks, bool start, void * p_context);
  void m_adv_sent_led_show(uint32_t timer_ticks, bool start, void * p_context);
  void m_8dBm_led_slow_blink(uint32_t timer_ticks, bool start, void * p_context);
  void m_timeout_for_advertise(uint32_t timer_ticks, bool start, void * p_context);
  void m_time_keeper(uint32_t timer_ticks, bool start, void * p_context);




#endif