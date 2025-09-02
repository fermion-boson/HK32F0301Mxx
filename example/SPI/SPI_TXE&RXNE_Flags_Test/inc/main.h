/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************

  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes --------------------------------------------------------------------- */
#include <stdio.h>
#include "hk32f0301mxxc.h"

#include "uart.h"

#define countof(a)   (sizeof(a) / sizeof(*(a)))
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Exported define -----------------------------------------------------------*/

/* Uncomment the line below if you will use the SPI peripheral as a Master */
#define SPI_MASTER 
/* Uncomment the line below if you will use the SPI peripheral as a Slave */
//#define SPI_SLAVE

/* Uncomment the size of data to be transmetted (only one data size must be selected) */
#define SPI_DATASIZE_8
//#define SPI_DATASIZE_7
//#define SPI_DATASIZE_6
//#define SPI_DATASIZE_5


/* USE//R_TIMEOUT value for waiting loops. This timeout is just guarantee that the
   application will not remain stuck if the USART communication is corrupted. 
   You may modify this timeout value depending on CPU frequency and application
   conditions (interrupts routines, number of data to transfer, baudrate, CPU
   frequency...). */ 
#define USER_TIMEOUT                    ((uint32_t)0x64) /* Waiting 1s */

/* Communication boards SPIx Interface */
#define SPI_SEL                         SPI
#define SPI_CLK                         RCC_APBPeriph2_SPI
#define SPI_IRQn                        SPI_IRQn
#define SPI_IRQHandler                  SPI_IRQHandler

#define SPI_SCK_PIN                     GPIO_Pin_3                  /* PD.03 */
#define SPI_SCK_GPIO_PORT               GPIOD                       /* GPIOA */
#define SPI_SCK_GPIO_CLK                RCC_AHBPeriph_GPIOD
#define SPI_SCK_SOURCE                  GPIO_PinSource3
#define SPI_SCK_AF                      GPIO_AF_2

#define SPI_MISO_PIN                    GPIO_Pin_7                  /* PD.07 */
#define SPI_MISO_GPIO_PORT              GPIOC                       /* GPIOC */
#define SPI_MISO_GPIO_CLK               RCC_AHBPeriph_GPIOC
#define SPI_MISO_SOURCE                 GPIO_PinSource7
#define SPI_MISO_AF                     GPIO_AF_2

#define SPI_MOSI_PIN                    GPIO_Pin_2                   /* PD.02 */
#define SPI_MOSI_GPIO_PORT              GPIOD                        /* GPIOD */
#define SPI_MOSI_GPIO_CLK               RCC_AHBPeriph_GPIOD
#define SPI_MOSI_SOURCE                 GPIO_PinSource2
#define SPI_MOSI_AF                     GPIO_AF_2

#define SPI_NSS_PIN                     GPIO_Pin_7                    /* PD.07 */
#define SPI_NSS_GPIO_PORT               GPIOD                         /* GPIOD */
#define SPI_NSS_GPIO_CLK                RCC_AHBPeriph_GPIOD
#define SPI_NSS_SOURCE                  GPIO_PinSource7
#define SPI_NSS_AF                      GPIO_AF_2


#define TXBUFFERSIZE                     10
#define RXBUFFERSIZE                     TXBUFFERSIZE


#define CMD_ACK                          0x16 

/* Define numbers of bytes to transmit from TxBuffer */
#define DATA_SIZE                        TXBUFFERSIZE

/* Define data size and data masks */
#ifdef SPI_DATASIZE_8
 #define SPI_DATASIZE                     SPI_DataSize_8b
 #define SPI_DATAMASK                     (uint8_t)0xFF
#elif defined (SPI_DATASIZE_7)
 #define SPI_DATASIZE                     SPI_DataSize_7b
 #define SPI_DATAMASK                     (uint8_t)0x7F
#elif defined (SPI_DATASIZE_6)
 #define SPI_DATASIZE                     SPI_DataSize_6b
 #define SPI_DATAMASK                     (uint8_t)0x3F
#elif defined (SPI_DATASIZE_5)
 #define SPI_DATASIZE                     SPI_DataSize_5b
 #define SPI_DATAMASK                     (uint8_t)0x1F
#else
 #error " Select at least one data size in main.h file " 
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
#endif /* __MAIN_H */

