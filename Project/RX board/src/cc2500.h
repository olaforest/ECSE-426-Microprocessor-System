/*
ECSE 426 - Wireless 3D printing machine project
Maxim Goukhshtein (ID: 260429739)
Olivier Laforest  (ID: 260469066)
Nuri Ege Kozan    (ID: 260359680)
Genevieve Nantel  (ID: 260481768)
Group #4
Date:	April 14th, 2015
*/ 
#ifndef __CC2500__
#define __CC2500__

#include "stm32f4xx_spi.h"

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
   
#define CC2500_FLAG_TIMEOUT         		 ((uint32_t)0x1000)

#define CC2500_TIMED_OUT								 ((uint8_t)0xFF)
#define CC2500_ADDR_MASK								 ((uint8_t)0x3F)

//CC2500 SPI Interface pins
#define CC2500_SPI                       SPI2
#define CC2500_SPI_CLK                   RCC_APB1Periph_SPI2

#define CC2500_SPI_SCK_PIN               GPIO_Pin_13                  
#define CC2500_SPI_SCK_GPIO_PORT         GPIOB                       
#define CC2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
#define CC2500_SPI_SCK_SOURCE            GPIO_PinSource13
#define CC2500_SPI_SCK_AF                GPIO_AF_SPI2

#define CC2500_SPI_MISO_PIN              GPIO_Pin_14                
#define CC2500_SPI_MISO_GPIO_PORT        GPIOB                      
#define CC2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define CC2500_SPI_MISO_SOURCE           GPIO_PinSource14
#define CC2500_SPI_MISO_AF               GPIO_AF_SPI2

#define CC2500_SPI_MOSI_PIN              GPIO_Pin_15                
#define CC2500_SPI_MOSI_GPIO_PORT        GPIOB                       
#define CC2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
#define CC2500_SPI_MOSI_SOURCE           GPIO_PinSource15
#define CC2500_SPI_MOSI_AF               GPIO_AF_SPI2

#define CC2500_SPI_CS_PIN                GPIO_Pin_4                
#define CC2500_SPI_CS_GPIO_PORT          GPIOC                       
#define CC2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOC

#define CC2500_SPI_INT1_PIN              GPIO_Pin_5                  
#define CC2500_SPI_INT1_GPIO_PORT        GPIOC                      
#define CC2500_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOC
#define CC2500_SPI_INT1_EXTI_LINE        EXTI_Line0
#define CC2500_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOC
#define CC2500_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource5
#define CC2500_SPI_INT1_EXTI_IRQn        EXTI0_IRQn 

// macros to set the chip select low/high 
#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)

#define NUM_CONF_REG				 ((uint8_t)0x2f)

// CC2500 configuration registers addresses
#define CC2500_REG_IOCFG2	 ((uint8_t)0x00)
#define CC2500_REG_IOCFG1    ((uint8_t)0x01)
#define CC2500_REG_IOCFG0    ((uint8_t)0x02)
#define CC2500_REG_FIFOTHR   ((uint8_t)0x03)
#define CC2500_REG_SYNC1     ((uint8_t)0x04)
#define CC2500_REG_SYNC0     ((uint8_t)0x05)
#define CC2500_REG_PKTLEN    ((uint8_t)0x06)
#define CC2500_REG_PKTCTRL1  ((uint8_t)0x07)
#define CC2500_REG_PKTCTRL0  ((uint8_t)0x08)
#define CC2500_REG_ADDR      ((uint8_t)0x09)
#define CC2500_REG_CHANNR    ((uint8_t)0x0A)
#define CC2500_REG_FSCTRL1   ((uint8_t)0x0B)
#define CC2500_REG_FSCTRL0   ((uint8_t)0x0C)
#define CC2500_REG_FREQ2     ((uint8_t)0x0D)
#define CC2500_REG_FREQ1     ((uint8_t)0x0E)
#define CC2500_REG_FREQ0     ((uint8_t)0x0F)
#define CC2500_REG_MDMCFG4   ((uint8_t)0x10)
#define CC2500_REG_MDMCFG3   ((uint8_t)0x11)
#define CC2500_REG_MDMCFG2   ((uint8_t)0x12)
#define CC2500_REG_MDMCFG1   ((uint8_t)0x13)
#define CC2500_REG_MDMCFG0   ((uint8_t)0x14)
#define CC2500_REG_DEVIATN   ((uint8_t)0x15)
#define CC2500_REG_MCSM2     ((uint8_t)0x16)
#define CC2500_REG_MCSM1     ((uint8_t)0x17)
#define CC2500_REG_MCSM0     ((uint8_t)0x18)
#define CC2500_REG_FOCCFG    ((uint8_t)0x19)
#define CC2500_REG_BSCFG     ((uint8_t)0x1A)
#define CC2500_REG_AGCTRL2   ((uint8_t)0x1B)
#define CC2500_REG_AGCTRL1   ((uint8_t)0x1C)
#define CC2500_REG_AGCTRL0   ((uint8_t)0x1D)
#define CC2500_REG_WOREVT1   ((uint8_t)0x1E)
#define CC2500_REG_WOREVT0   ((uint8_t)0x1F)
#define CC2500_REG_WORCTRL   ((uint8_t)0x20)
#define CC2500_REG_FREND1    ((uint8_t)0x21)
#define CC2500_REG_FREND0    ((uint8_t)0x22)
#define CC2500_REG_FSCAL3    ((uint8_t)0x23)
#define CC2500_REG_FSCAL2    ((uint8_t)0x24)
#define CC2500_REG_FSCAL1    ((uint8_t)0x25)
#define CC2500_REG_FSCAL0    ((uint8_t)0x26)
#define CC2500_REG_RCCTRL1   ((uint8_t)0x27)
#define CC2500_REG_RCCTRL0   ((uint8_t)0x28)
#define CC2500_REG_FSTEST    ((uint8_t)0x29)
#define CC2500_REG_PTEST     ((uint8_t)0x2A)
#define CC2500_REG_AGCTEST   ((uint8_t)0x2B)
#define CC2500_REG_TEST2     ((uint8_t)0x2C)
#define CC2500_REG_TEST1     ((uint8_t)0x2D)
#define CC2500_REG_TEST0     ((uint8_t)0x2E)

// Command strobes
#define CC2500_REG_SRES     ((uint8_t)0x30)
#define CC2500_REG_SCAL		((uint8_t)0x33)
#define CC2500_REG_SRX		((uint8_t)0x34)
#define CC2500_REG_STX		((uint8_t)0x35)
#define CC2500_REG_SIDLE	((uint8_t)0x36)
#define CC2500_REG_SFRX		((uint8_t)0x3A)
#define CC2500_REG_SFTX		((uint8_t)0x3B)
#define CC2500_REG_SNOP		((uint8_t)0x3D)

// FIFO register
#define CC2500_REG_FIFO		((uint8_t)0x3F)

// CC2500 configuration registers values
#define CC2500_VAL_IOCFG2 	0x29   // default: 0x29  
#define	CC2500_VAL_IOCFG1	0x2E 
#define CC2500_VAL_IOCFG0 	0x06	
#define CC2500_VAL_FIFOTHR 	0x07	 // default: 0x07
#define	CC2500_VAL_SYNC1    0xD3        
#define	CC2500_VAL_SYNC0	0x91
#define CC2500_VAL_PKTLEN 	0x01
#define CC2500_VAL_PKTCTRL1 0x08	
#define CC2500_VAL_PKTCTRL0 0x04	
#define CC2500_VAL_ADDR 	0x00	
#define CC2500_VAL_CHANNR 	0x20	 // channel 32 (8 * 4)
#define CC2500_VAL_FSCTRL1 	0x0C	
#define CC2500_VAL_FSCTRL0 	0x00	
#define CC2500_VAL_FREQ2 	0x5D	 
#define CC2500_VAL_FREQ1 	0x94	
#define CC2500_VAL_FREQ0 	0x02	
#define CC2500_VAL_MDMCFG4 	0x0E
#define CC2500_VAL_MDMCFG3 	0x3B
#define CC2500_VAL_MDMCFG2 	0x73	
#define CC2500_VAL_MDMCFG1 	0x42	
#define CC2500_VAL_MDMCFG0 	0xF8	
#define CC2500_VAL_DEVIATN 	0x00	
#define	CC2500_VAL_MCSM2    0x07  
#define CC2500_VAL_MCSM1 	0x02	
#define CC2500_VAL_MCSM0 	0x18	
#define CC2500_VAL_FOCCFG 	0x1D	
#define CC2500_VAL_BSCFG 	0x1C	
#define CC2500_VAL_AGCTRL2 	0xC7	 
#define CC2500_VAL_AGCTRL1 	0x40	
#define CC2500_VAL_AGCTRL0 	0xB0
#define	CC2500_VAL_WOREVT1  0x87     
#define	CC2500_VAL_WOREVT0  0x6B      
#define	CC2500_VAL_WORCTRL  0xF8
#define CC2500_VAL_FREND1 	0xB6	 
#define CC2500_VAL_FREND0 	0x10	
#define CC2500_VAL_FSCAL3 	0xEA	 
#define CC2500_VAL_FSCAL2 	0x0A	
#define CC2500_VAL_FSCAL1 	0x00	 
#define CC2500_VAL_FSCAL0 	0x19	
#define	CC2500_VAL_RCCTRL1  0x41      
#define	CC2500_VAL_RCCTRL0  0x00 
#define CC2500_VAL_FSTEST 	0x59
#define	CC2500_VAL_PTEST	0x7F        
#define	CC2500_VAL_AGCTEST 	0x3F 
#define CC2500_VAL_TEST2 	0x88	
#define CC2500_VAL_TEST1 	0x31	
#define CC2500_VAL_TEST0 	0x0B

/* Exported functions --------------------------------------------------------*/
void CC2500_init(void);
void CC2500_write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
void CC2500_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);
void chip_reset(void);
void calibrate_frequency_synthesizer(void);
void rx_enable(void);
void tx_enable(void);
void set_idle(void);
void flush_rx_fifo(void);
void flush_tx_fifo(void);
void read_rx_fifo(uint8_t* pBuffer, uint16_t NumByteToRead);
void write_tx_fifo(uint8_t* pBuffer, uint16_t NumByteToRead);
uint8_t get_status_byte(uint8_t read);
uint8_t get_rx_buffer_size(void);
uint8_t get_tx_buffer_size(void);

#endif
