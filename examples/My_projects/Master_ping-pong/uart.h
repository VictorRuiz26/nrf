#ifndef UART_H__
#define UART_H__
  #include "app_error.h"
  #include "app_uart.h"
  #include "boards.h"
  #include "nrf_uart.h"

  //************* UART BUFFERS AND DEFINITIONS *********** 
  #define UART_BUFFER_RX_SIZE 256
  
  #define MODBUS_PRUEBA_PC        0xAE
  #define MODBUS_INICIO_PING_PONG 0xAB

  #define UART_TX_BUFF_SIZE 128
  #define UART_RX_BUFF_SIZE 128
  
  #define TIMEOUT_MODBUS  10 //Tiempo máximo (en unidades de 50ms, tiempo en que se llama AnalizaModbus) de espera a un paquete completo con mi dirección

  #define UART_HWFC APP_UART_FLOW_CONTROL_DISABLED

  typedef struct {
    uint8_t data[UART_BUFFER_RX_SIZE];
    uint8_t indiceLeido;
    uint8_t indiceMetido;
    uint16_t tiempoSinLlegarPaquete;
  } TBufferUART;

  extern TBufferUART UART_PC;

  extern const app_uart_comm_params_t comms_params;


  #define ocupadosBufferUART(x) ((x.indiceMetido >= x.indiceLeido) ? (x.indiceMetido - x.indiceLeido) : (UART_BUFFER_RX_SIZE + x.indiceMetido - x.indiceLeido))
  #define incrementaIndice(x, cantidad) {x+=cantidad;if(x>=UART_BUFFER_RX_SIZE) x-=UART_BUFFER_RX_SIZE;}


  uint32_t SacaLong_Modbus (uint8_t *indice);
  uint16_t SacaInt_Modbus (uint8_t *indice);

  void InicializaBufferUART(void);
  void echoPacketUART(uint8_t longPaquete, uint8_t inicio);
  void AnalizaBufferUART(void);
  void send_metrics(void);
  void uart_evt_handle(app_uart_evt_t *p);

  void Trata_Modbus_Inicio_PingPong (uint8_t indiceAuxiliar);
#endif