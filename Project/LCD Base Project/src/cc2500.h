#ifndef __CC2500__
#define __CC2500__

#include "stm32f4xx_spi.h"

// CC2500 SPI Interface pins
#define CC2500_SPI                       SPI4
#define CC2500_SPI_CLK                   RCC_APB2Periph_SPI4

#define CC2500_SPI_SCK_PIN               GPIO_Pin_2                  /* PE.02 */
#define CC2500_SPI_SCK_GPIO_PORT         GPIOE                       /* GPIOE */
#define CC2500_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOE
#define CC2500_SPI_SCK_SOURCE            GPIO_PinSource2
#define CC2500_SPI_SCK_AF                GPIO_AF_SPI4

#define CC2500_SPI_MISO_PIN              GPIO_Pin_5                  /* PE.05 */
#define CC2500_SPI_MISO_GPIO_PORT        GPIOE                       /* GPIOE */
#define CC2500_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_MISO_SOURCE           GPIO_PinSource5
#define CC2500_SPI_MISO_AF               GPIO_AF_SPI4

#define CC2500_SPI_MOSI_PIN              GPIO_Pin_6                  /* PE.06 */
#define CC2500_SPI_MOSI_GPIO_PORT        GPIOE                       /* GPIOE */
#define CC2500_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_MOSI_SOURCE           GPIO_PinSource6
#define CC2500_SPI_MOSI_AF               GPIO_AF_SPI4

#define CC2500_SPI_CS_PIN                GPIO_Pin_4                  /* PE.04 */
#define CC2500_SPI_CS_GPIO_PORT          GPIOE                       /* GPIOE */
#define CC2500_SPI_CS_GPIO_CLK           RCC_AHB1Periph_GPIOE

#define CC2500_SPI_INT1_PIN              GPIO_Pin_0                  /* PE.00 */
#define CC2500_SPI_INT1_GPIO_PORT        GPIOE                       /* GPIOE */
#define CC2500_SPI_INT1_GPIO_CLK         RCC_AHB1Periph_GPIOE
#define CC2500_SPI_INT1_EXTI_LINE        EXTI_Line0
#define CC2500_SPI_INT1_EXTI_PORT_SOURCE EXTI_PortSourceGPIOE
#define CC2500_SPI_INT1_EXTI_PIN_SOURCE  EXTI_PinSource0
#define CC2500_SPI_INT1_EXTI_IRQn        EXTI0_IRQn 

// macros to set the chip select low/high 
#define CC2500_CS_LOW()       GPIO_ResetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)
#define CC2500_CS_HIGH()      GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN)

// CC2500 configuration registers 
#define CC2500_REG_IOCFG2		 ((uint8_t)0x00)
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

#endif