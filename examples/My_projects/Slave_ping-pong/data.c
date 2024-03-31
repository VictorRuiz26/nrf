#include "data.h"

//I need these variables for responsing the coordinator with the same values (or the corresponding ones). The adv_data pdu is changed in advertising_init, the same way that major and minor
bool advReceived = false; //Flag for starting the timer at first adv received.
uint16_t majorValue      = 0;
uint16_t minorValue      = 0;
uint8_t coordinatorID    = COORDINATOR_ID;
uint8_t messageType      = MESSAGE_TEST_TYPE;
uint8_t countAdvReceived = DEF_NUM_ADV_RECEIVED; //For the response to the coordinator. It will icrease each valid adv.
uint32_t nSeqReceived    = DEF_NSEQ;
int8_t rssiValues[MAX_NUM_ADVERTISEMENTS];