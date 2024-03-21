#include "data.h"
#include "uart.h"
#include "config.h"
#include "ble_gap.h"
#include "nrf_log.h"

TBufferUART UART_PC;
const app_uart_comm_params_t comms_params =
  {   
      RX_PIN_NUMBER,
      TX_PIN_NUMBER,
      RTS_PIN_NUMBER,
      CTS_PIN_NUMBER,
      UART_HWFC,
      false,
      NRF_UART_BAUDRATE_115200
  };

void InicializaBufferUART(void) {
  uint16_t i;

  UART_PC.indiceLeido = 0;
  UART_PC.indiceMetido = 0;

  for (i = 0; i < UART_BUFFER_RX_SIZE; i++) {
    UART_PC.data[i] = '\0';
  }
}

void echoPacketUART(uint8_t longPaquete, uint8_t inicio) {
  uint8_t i, index;
  index = inicio;
  for (i = 0; i < longPaquete; i++) {
    app_uart_put(UART_PC.data[index]);
    incrementaIndice(index, 1);
  }
}

void AnalizaBufferUART(void) {
  /* No hay que olvidar que aqui trabajamos como ESCLAVOS */
    static unsigned char longPaquete, bytesDescartados, paquetesLeidos, indiceAuxiliar;

    if (ocupadosBufferUART(UART_PC) < 5)
    {
        return;
    }

    paquetesLeidos = 0;
    bytesDescartados = 0;

    while ((ocupadosBufferUART(UART_PC) > 1) && (paquetesLeidos < 100) /*&& (bytesDescartados < 200)*/) 
    {
      if (UART_PC.data[UART_PC.indiceLeido] == COORDINATOR_ID) {
        NRF_LOG_INFO("Paquete modbus con mi direccion: %d", UART_PC.data[UART_PC.indiceLeido]);
        //Ya tengo un byte con mi direcci�n. Ahora tengo que ver si es un paquete modbus lo que viene o no 
        indiceAuxiliar = UART_PC.indiceLeido;
        incrementaIndice(indiceAuxiliar, 1); //Apunta al tipo de funci�n 

        switch (UART_PC.data[indiceAuxiliar]) 
        {
            //Si lo que tengo entre manos es un paquete MODBUS correcto, aqu� debo de tener el tipo de funci�n. Si la reconozco, pues bien. Pero si no la reconozco paso del paquete 				
            case MODBUS_PRUEBA_PC:
              NRF_LOG_INFO("MODBUS_PRUEBA_PC");
              //Aqui indicar el tamaño mínimo conocido para este paquete
              if (ocupadosBufferUART(UART_PC) >= 5) {
                paquetesLeidos++;
                incrementaIndice(indiceAuxiliar, 1);
                longPaquete = UART_PC.data[indiceAuxiliar] + 5; //1 de la direcci�n + 2 del CRC + 2 de cabecera

                //Reviso si me han llegado todos los bytes que indica la cabecera
                if (ocupadosBufferUART(UART_PC) >= longPaquete) {
                  if(1/*TODO: aqui va el chequeo de CRC*/) {
                    NRF_LOG_INFO("RECIBO PAQUETE COMPLETO!");
                    echoPacketUART(longPaquete, UART_PC.indiceLeido);
                    incrementaIndice(UART_PC.indiceLeido, longPaquete);
                  }
                  else {
                    /* El CRC ha fallado, descarto este byte y sigo buscando un paquete v�lido */
                    incrementaIndice(UART_PC.indiceLeido, 1);
                    bytesDescartados++;
                  }  
                } 
                else
                {
                  return; //Todavia no ha llegado
                }
              } 
              else 
              {
                return; //Todavia no ha llegado
              }
              break;
            case MODBUS_INICIO_PING_PONG:
              NRF_LOG_INFO("MODBUS_INICIO_PING_PONG");
              //Aqui indicar el tamaño mínimo conocido para este paquete
              if (ocupadosBufferUART(UART_PC) >= 5) {
                paquetesLeidos++;
                incrementaIndice(indiceAuxiliar, 1);
                longPaquete = UART_PC.data[indiceAuxiliar] + 5; //1 de la direcci�n + 2 del CRC + 2 de cabecera

                //Reviso si me han llegado todos los bytes que indica la cabecera
                if (ocupadosBufferUART(UART_PC) >= longPaquete) {
                  if(1) {
                    NRF_LOG_INFO("RECIBO PAQUETE. Procedo a iniciar advertisement con los datos recibidos");
                    echoPacketUART(longPaquete, UART_PC.indiceLeido);
                    incrementaIndice(UART_PC.indiceLeido, longPaquete);
                  }
                  else {
                    /* El CRC ha fallado, descarto este byte y sigo buscando un paquete v�lido */
                    incrementaIndice(UART_PC.indiceLeido, 1);
                    bytesDescartados++;
                  }  
                } 
                else
                {
                  return; //Todavia no ha llegado
                }
              } 
              else 
              {
                return; //Todavia no ha llegado
              }
              break;
        }
      }
      else {
        incrementaIndice(UART_PC.indiceLeido, 1);
        bytesDescartados++;
      }
    }
}

void uart_evt_handle(app_uart_evt_type_t *p) {
  uint8_t byte;
  switch (*p) {
  case APP_UART_DATA_READY:
    app_uart_get(&byte);
    UART_PC.data[UART_PC.indiceMetido] = byte;
    incrementaIndice(UART_PC.indiceMetido, 1);
    break;
  }
}
  // Aquí es donde crearé el paquete definido para luego escanear con python. Se llamará en el handler del ble
  void send_metrics(void) {
    unsigned char *pChar;
    app_uart_put(0x7E); // Inicio trama

    app_uart_put(0xAA); // 2 Bytes fijos de relleno
    app_uart_put(0xBB);

    // Devices IDs
    app_uart_put(COORDINATOR_ID);
    for (int i = 0; i < BLE_GAP_ADDR_LEN; i++) {
      app_uart_put(coordAddr.addr[i]);
    }

    app_uart_put(slaveID);
    for (int i = 0; i < BLE_GAP_ADDR_LEN; i++) {
      app_uart_put(slaveAddr.addr[i]);
    }

    // Msg info (type, nseq, nadv, len)
    app_uart_put(downlinkMsgType);
    pChar = (unsigned char *)&nseqSent;
    app_uart_put(pChar[3]);
    app_uart_put(pChar[2]);
    app_uart_put(pChar[1]);
    app_uart_put(pChar[0]);
    app_uart_put(NUM_ADVERTISEMENTS);
    app_uart_put(packetBLESize);

    // DOWNLINK metrics
    // num adv received by slave and mean RSSI of them
    app_uart_put(downlinkNAdvRx);
    app_uart_put(downlinkMeanRSSI);

    // UPLINK metrics
    app_uart_put(countAdvReceived);
    int16_t acumRssi = 0;
    for (uint8_t i = 0; i < countAdvReceived; i++) {
      acumRssi += rssiValues[i];
    }
    app_uart_put((int8_t)(acumRssi / countAdvReceived));

    app_uart_put(0xFF);

    NRF_LOG_INFO("--- Vale, recibo, tengo que sacar paquete ---");
    NRF_LOG_INFO(" ID msg: %d, Slave ID: %d", downlinkMsgType, slaveID);
    NRF_LOG_INFO("[UPLINK] nAdv Rx: %d, mean RSSI: %d.",
        countAdvReceived, (int8_t)(acumRssi / countAdvReceived));
    NRF_LOG_INFO("[DOWNLINK] nAdv Rx: %d, mean RSSI: %d",
        downlinkNAdvRx, downlinkMeanRSSI);

    countAdvReceived = 0;
    advReceived = false;
  }