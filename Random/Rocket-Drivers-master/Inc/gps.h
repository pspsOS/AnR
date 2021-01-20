#ifndef GPS_H
#define GPS_H

//Includes
#include "FreeRTOS.h"
#include "genericSpi.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_uart.h"

//Variable Build Defines

//Generic Defines
#define GPS_STACK_SIZE    256
#define GPS_PRIORITY      1
#define GPS_RATE          5 / portTICK_RATE_MS //Run at a rate of 200 Hz. The 5 is indicative of a period of 5 ms.
extern UART_HandleTypeDef   huart4;
#define GPS_UART            &huart4;
#define RX_SIZE             86 //Size of the longest message to recieve
#define TX_SIZE             10 //Size of the longest message to send 

//Structs
typedef struct rx_t {
    uint16_t size;
    uint8_t data[RX_SIZE];
} rx_t;

typefed struct tx_t {
    uint16_t size;
    uint8_t data[TX_SIZE];
} tx_t;

//Variables
QueueHandle_t tx_uart;
QueueHandle_t rx_uart;

//Prototypes

#endif
