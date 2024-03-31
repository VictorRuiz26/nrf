#ifndef DATA_H__
#define DATA_H__
  #include <stdbool.h>
  #include <stdint.h>
  #include "config.h"
  extern bool advReceived; //Flag for starting the timer at first adv received.
  extern uint16_t majorValue;
  extern uint16_t minorValue;
  extern uint8_t coordinatorID;
  extern uint8_t messageType;
  extern uint8_t countAdvReceived; //For the response to the coordinator. It will icrease each valid adv.
  extern uint32_t nSeqReceived;
  extern int8_t rssiValues[MAX_NUM_ADVERTISEMENTS];

#endif