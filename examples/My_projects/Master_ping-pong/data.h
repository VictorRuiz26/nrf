#ifndef DATA_H__
#define DATA_H__
  #include <stdbool.h>
  #include <stdint.h>
  #include "ble_gap.h"
  #include "config.h"

  extern ble_gap_addr_t coordAddr;

  // Cloning slave adv processing variables
  extern bool advReceived;
  extern uint16_t majorValue;
  extern uint16_t minorValue;
  extern uint32_t nseqSent;
  extern uint8_t msgTypeSent;
  extern uint8_t slaveID;
  extern uint8_t countAdvReceived;
  extern uint32_t nSeqReceived;
  extern int8_t rssiValues[MAX_NUM_ADVERTISEMENTS];

  extern ble_gap_addr_t slaveAddr;
  extern uint8_t downlinkMsgType;
  extern uint8_t packetBLESize;
  extern uint8_t downlinkNAdvRx;
  extern int8_t downlinkMeanRSSI;
#endif