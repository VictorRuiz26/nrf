#include "data.h"

  ble_gap_addr_t coordAddr;

  // Cloning slave adv processing variables
  bool advReceived = false;
  uint16_t majorValue = 0;
  uint16_t minorValue = 0;
  uint32_t nseqSent = 0;
  uint8_t msgTypeSent = MESSAGE_TEST_TYPE;
  uint8_t slaveID = DEFAULT_SLAVE_ID;
  uint8_t countAdvReceived = 0;
  uint32_t nSeqReceived = 0;
  int8_t rssiValues[MAX_NUM_ADVERTISEMENTS];

  ble_gap_addr_t slaveAddr;
  uint8_t downlinkMsgType;
  uint8_t packetBLESize;
  uint8_t downlinkNAdvRx;
  int8_t downlinkMeanRSSI;