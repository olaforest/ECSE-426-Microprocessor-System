#include "cc2500.h"

/* Read/Write command */
#define READ_CMD     ((uint8_t)0x80) 

/* Multiple byte read/write command */ 
#define MULTIPLEBYTE_CMD  ((uint8_t)0x40)

/* Dummy Byte Send by the SPI Master device in order to generate the Clock to the Slave device */
#define DUMMY_BYTE        ((uint8_t)0x00)

static void CC2500_LowLevel_Init(void);
static uint8_t CC2500_SendByte(uint8_t byte);
static void CC2500_send_byte(uint8_t addr, uint8_t byte);
uint8_t CC2500_CS(void);

void CC2500_init(){
	// set the configuration registers
	uint8_t configuration[NUM_CONF_REG];
	configuration[CC2500_REG_IOCFG2]   = CC2500_VAL_IOCFG2;		
	configuration[CC2500_REG_IOCFG1]   = CC2500_VAL_IOCFG1;	
	configuration[CC2500_REG_IOCFG0]   = CC2500_VAL_IOCFG0;   
	configuration[CC2500_REG_FIFOTHR]  = CC2500_VAL_FIFOTHR; 	
	configuration[CC2500_REG_SYNC1]    = CC2500_VAL_SYNC1;    
	configuration[CC2500_REG_SYNC0]    = CC2500_VAL_SYNC0;		
	configuration[CC2500_REG_PKTLEN]   = CC2500_VAL_PKTLEN;		
	configuration[CC2500_REG_PKTCTRL1] = CC2500_VAL_PKTCTRL1;	
	configuration[CC2500_REG_PKTCTRL0] = CC2500_VAL_PKTCTRL0;	
	configuration[CC2500_REG_ADDR]     = CC2500_VAL_ADDR;			
	configuration[CC2500_REG_CHANNR]   = CC2500_VAL_CHANNR;		
	configuration[CC2500_REG_FSCTRL1]  = CC2500_VAL_FSCTRL1;	
	configuration[CC2500_REG_FSCTRL0]  = CC2500_VAL_FSCTRL0;  
	configuration[CC2500_REG_FREQ2]    = CC2500_VAL_FREQ2; 		
	configuration[CC2500_REG_FREQ1]    = CC2500_VAL_FREQ1;		
	configuration[CC2500_REG_FREQ0]    = CC2500_VAL_FREQ0;		
	configuration[CC2500_REG_MDMCFG4]  = CC2500_VAL_MDMCFG4;	
	configuration[CC2500_REG_MDMCFG3]  = CC2500_VAL_MDMCFG3;  
	configuration[CC2500_REG_MDMCFG2]  = CC2500_VAL_MDMCFG2;  
	configuration[CC2500_REG_MDMCFG1]  = CC2500_VAL_MDMCFG1;  
	configuration[CC2500_REG_MDMCFG0]  = CC2500_VAL_MDMCFG0;  
	configuration[CC2500_REG_DEVIATN]  = CC2500_VAL_DEVIATN;  
	configuration[CC2500_REG_MCSM2]    = CC2500_VAL_MCSM2;    
	configuration[CC2500_REG_MCSM1]    = CC2500_VAL_MCSM1;    
	configuration[CC2500_REG_MCSM0]    = CC2500_VAL_MCSM0;    
	configuration[CC2500_REG_FOCCFG]   = CC2500_VAL_FOCCFG;   
	configuration[CC2500_REG_BSCFG]    = CC2500_VAL_BSCFG;		
	configuration[CC2500_REG_AGCTRL2]  = CC2500_VAL_AGCTRL2; 
	configuration[CC2500_REG_AGCTRL1]  = CC2500_VAL_AGCTRL1; 
	configuration[CC2500_REG_AGCTRL0]  = CC2500_VAL_AGCTRL0; 
	configuration[CC2500_REG_WOREVT1]  = CC2500_VAL_WOREVT1; 
	configuration[CC2500_REG_WOREVT0]  = CC2500_VAL_WOREVT0;  
	configuration[CC2500_REG_WORCTRL]  = CC2500_VAL_WORCTRL;  
	configuration[CC2500_REG_FREND1]   = CC2500_VAL_FREND1;   
	configuration[CC2500_REG_FREND0]   = CC2500_VAL_FREND0;   
	configuration[CC2500_REG_FSCAL3]   = CC2500_VAL_FSCAL3;   
	configuration[CC2500_REG_FSCAL2]   = CC2500_VAL_FSCAL2;   
	configuration[CC2500_REG_FSCAL1]   = CC2500_VAL_FSCAL1;   
	configuration[CC2500_REG_FSCAL0]   = CC2500_VAL_FSCAL0;   
	configuration[CC2500_REG_RCCTRL1]  = CC2500_VAL_RCCTRL1;  
	configuration[CC2500_REG_RCCTRL0]  = CC2500_VAL_RCCTRL0;  
	configuration[CC2500_REG_FSTEST]   = CC2500_VAL_FSTEST; 	
	configuration[CC2500_REG_PTEST]    = CC2500_VAL_PTEST;		
	configuration[CC2500_REG_AGCTEST]  = CC2500_VAL_AGCTEST; 	
	configuration[CC2500_REG_TEST2]    = CC2500_VAL_TEST2;		
	configuration[CC2500_REG_TEST1]    = CC2500_VAL_TEST1;		
	configuration[CC2500_REG_TEST0]    = CC2500_VAL_TEST0;		
		
	CC2500_LowLevel_Init();	
		
	chip_reset();	
		
	// write the set values into the registers
	CC2500_write(configuration, CC2500_REG_IOCFG2, NUM_CONF_REG);		
		
//	CC2500_send_byte(CC2500_REG_IOCFG2  , CC2500_VAL_IOCFG2);		
//	CC2500_send_byte(CC2500_REG_IOCFG0  , CC2500_VAL_IOCFG0);   
//	CC2500_send_byte(CC2500_REG_FIFOTHR , CC2500_VAL_FIFOTHR); 		
//	CC2500_send_byte(CC2500_REG_PKTLEN  , CC2500_VAL_PKTLEN);		
//	CC2500_send_byte(CC2500_REG_PKTCTRL1, CC2500_VAL_PKTCTRL1);	
//	CC2500_send_byte(CC2500_REG_PKTCTRL0, CC2500_VAL_PKTCTRL0);	
//	CC2500_send_byte(CC2500_REG_ADDR    , CC2500_VAL_ADDR);			
//	CC2500_send_byte(CC2500_REG_CHANNR  , CC2500_VAL_CHANNR);		
//	CC2500_send_byte(CC2500_REG_FSCTRL1 , CC2500_VAL_FSCTRL1);	
//	CC2500_send_byte(CC2500_REG_FSCTRL0 , CC2500_VAL_FSCTRL0);  
//	CC2500_send_byte(CC2500_REG_FREQ2   , CC2500_VAL_FREQ2); 		
//	CC2500_send_byte(CC2500_REG_FREQ1   , CC2500_VAL_FREQ1);		
//	CC2500_send_byte(CC2500_REG_FREQ0   , CC2500_VAL_FREQ0);		
//	CC2500_send_byte(CC2500_REG_MDMCFG4 , CC2500_VAL_MDMCFG4);	
//	CC2500_send_byte(CC2500_REG_MDMCFG3 , CC2500_VAL_MDMCFG3);  
//	CC2500_send_byte(CC2500_REG_MDMCFG2 , CC2500_VAL_MDMCFG2);  
//	CC2500_send_byte(CC2500_REG_MDMCFG1 , CC2500_VAL_MDMCFG1);  
//	CC2500_send_byte(CC2500_REG_MDMCFG0 , CC2500_VAL_MDMCFG0);  
//	CC2500_send_byte(CC2500_REG_DEVIATN , CC2500_VAL_DEVIATN);  
//	CC2500_send_byte(CC2500_REG_MCSM1   , CC2500_VAL_MCSM1);    
//	CC2500_send_byte(CC2500_REG_MCSM0   , CC2500_VAL_MCSM0);    
//	CC2500_send_byte(CC2500_REG_FOCCFG  , CC2500_VAL_FOCCFG);   
//	CC2500_send_byte(CC2500_REG_BSCFG   , CC2500_VAL_BSCFG);		
//	CC2500_send_byte(CC2500_REG_AGCTRL2 , CC2500_VAL_AGCTRL2); 
//	CC2500_send_byte(CC2500_REG_AGCTRL1 , CC2500_VAL_AGCTRL1); 
//	CC2500_send_byte(CC2500_REG_AGCTRL0 , CC2500_VAL_AGCTRL0); 
//	CC2500_send_byte(CC2500_REG_FREND1  , CC2500_VAL_FREND1);   
//	CC2500_send_byte(CC2500_REG_FREND0  , CC2500_VAL_FREND0);   
//	CC2500_send_byte(CC2500_REG_FSCAL3  , CC2500_VAL_FSCAL3);   
//	CC2500_send_byte(CC2500_REG_FSCAL2  , CC2500_VAL_FSCAL2);   
//	CC2500_send_byte(CC2500_REG_FSCAL1  , CC2500_VAL_FSCAL1);   
//	CC2500_send_byte(CC2500_REG_FSCAL0  , CC2500_VAL_FSCAL0);   
//	CC2500_send_byte(CC2500_REG_FSTEST  , CC2500_VAL_FSTEST); 	 	
//	CC2500_send_byte(CC2500_REG_TEST2   , CC2500_VAL_TEST2);		
//	CC2500_send_byte(CC2500_REG_TEST1   , CC2500_VAL_TEST1);		
//	CC2500_send_byte(CC2500_REG_TEST0   , CC2500_VAL_TEST0);			

//	flush_rx_fifo();
//	flush_tx_fifo();
//	set_idle();
}

/**
  * @brief  Writes one byte to the CC2500.
  * @param  pBuffer : pointer to the buffer  containing the data to be written to the CC2500.
  * @param  WriteAddr : CC2500's internal address to write to.
  * @param  NumByteToWrite: Number of bytes to write.
  * @retval None
  */
void CC2500_write(uint8_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite){	
	/* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
  */
  if(NumByteToWrite > 0x01)
  {
    WriteAddr |= (uint8_t)MULTIPLEBYTE_CMD;
  }
 
	/* Set chip select Low at the start of the transmission */
  if (CC2500_CS() == CC2500_TIMED_OUT) return;
  
  /* Send the Address of the indexed register */
  CC2500_SendByte(WriteAddr);
	
  /* Send the data that will be written into the device (MSB First) */
  while(NumByteToWrite >= 0x01)
  {
    CC2500_SendByte(*pBuffer);
    NumByteToWrite--;
    pBuffer++;
  }
  
  /* Set chip select high at the end of the transmission */ 
  CC2500_CS_HIGH();
}

/**
  * @brief  Reads a block of data from the CC2500.
  * @param  pBuffer : pointer to the buffer that receives the data read from the CC2500.
  * @param  ReadAddr : CC2500's internal address to read from.
  * @param  NumByteToRead : number of bytes to read from the CC2500.
  * @retval None
  */
void CC2500_read(uint8_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{  	
  if(NumByteToRead > 0x01) {
    ReadAddr |= (uint8_t)(READ_CMD | MULTIPLEBYTE_CMD);
  } else {
    ReadAddr |= (uint8_t) READ_CMD;
  }
  
	/* Set chip select Low at the start of the transmission */
  if (CC2500_CS() == CC2500_TIMED_OUT) return;
 
  /* Send the Address of the indexed register */
  CC2500_SendByte(ReadAddr);
  
  /* Receive the data that will be read from the device (MSB First) */
  while(NumByteToRead > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to LIS3DSH (Slave device) */
    *pBuffer = CC2500_SendByte(DUMMY_BYTE);
    NumByteToRead--;
    pBuffer++;
  }
  
  /* Set chip select High at the end of the transmission */ 
  CC2500_CS_HIGH();
}

/**
  * @brief  Sends a Byte through the SPI interface and return the Byte received 
  *         from the SPI bus.
  * @param  Byte : Byte send.
  * @retval The received byte value
  */
static uint8_t CC2500_SendByte(uint8_t byte)
{
  /* Loop while DR register in not emplty */
  uint32_t CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((CC2500Timeout--) == 0) return 0;
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(CC2500_SPI, byte);
  
  /* Wait to receive a Byte */
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((CC2500Timeout--) == 0) return 0;
  }
  
  /* Return the Byte read from the SPI bus */
  return (uint8_t)SPI_I2S_ReceiveData(CC2500_SPI);
}

static void CC2500_send_byte(uint8_t addr, uint8_t byte){
	if (CC2500_CS() == CC2500_TIMED_OUT) return;
	
	 /* Loop while DR register in not emplty */
  uint32_t CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((CC2500Timeout--) == 0) return;
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(CC2500_SPI, addr);
  
  /* Wait to receive a Byte */
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((CC2500Timeout--) == 0) return;
  }
	
	 /* Loop while DR register in not emplty */
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_TXE) == RESET)
  {
    if((CC2500Timeout--) == 0) return;
  }
  
  /* Send a Byte through the SPI peripheral */
  SPI_I2S_SendData(CC2500_SPI, byte);
  
  /* Wait to receive a Byte */
  CC2500Timeout = CC2500_FLAG_TIMEOUT;
  while (SPI_I2S_GetFlagStatus(CC2500_SPI, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((CC2500Timeout--) == 0) return;
  }
  
	CC2500_CS_HIGH();
}

uint8_t CC2500_CS(){
	CC2500_CS_LOW();
  uint32_t CC2500Timeout = CC2500_FLAG_TIMEOUT;
  
	while (GPIO_ReadInputDataBit(CC2500_SPI_MISO_GPIO_PORT,CC2500_SPI_MISO_PIN) != RESET){
		if((CC2500Timeout--) == 0){
			CC2500_CS_HIGH();
			return CC2500_TIMED_OUT;
		}
  }
	
	return 0;
}

uint8_t send_strobe(uint8_t addr){
	/* Set chip select Low at the start of the transmission */
  if (CC2500_CS() == CC2500_TIMED_OUT) return CC2500_TIMED_OUT;
 
	uint8_t status_byte = CC2500_SendByte(addr);
	
	CC2500_CS_HIGH();
	
  /* Send the Address of the indexed register */
  return status_byte;
}

uint8_t get_rx_buffer_size(){
	if (CC2500_CS() == CC2500_TIMED_OUT) return CC2500_TIMED_OUT;
	
	uint8_t tttt = CC2500_SendByte(0xFB);	
	uint8_t num_rx_bytes = CC2500_SendByte(DUMMY_BYTE);
	
	CC2500_CS_HIGH();
	
	return num_rx_bytes;
}

uint8_t get_tx_buffer_size(){
	return send_strobe(CC2500_REG_SNOP) & 0x0F;
}

void chip_reset(){
	send_strobe(CC2500_REG_SRES);
	
	uint32_t CC2500Timeout = CC2500_FLAG_TIMEOUT;
  
	while (GPIO_ReadInputDataBit(CC2500_SPI_MISO_GPIO_PORT,CC2500_SPI_MISO_PIN) != RESET){
		if((CC2500Timeout--) == 0){
			CC2500_CS_HIGH();
		  return;			
		}
  }
}

void calibrate_frequency_synthesizer(){
	send_strobe(CC2500_REG_SCAL);
}

void rx_enable(){
	send_strobe(CC2500_REG_SRX);
}

void tx_enable(){
	send_strobe(CC2500_REG_STX);
}

void set_idle(){
	send_strobe(CC2500_REG_SIDLE);
}

void flush_rx_fifo(){
	send_strobe(CC2500_REG_SFRX);
}

void flush_tx_fifo(){
	send_strobe(CC2500_REG_SFTX);
}

uint8_t get_status_byte(uint8_t read){
	return send_strobe(read == 0 ? CC2500_REG_SNOP : CC2500_REG_SNOP | READ_CMD);
}

void read_rx_fifo(uint8_t* pBuffer, uint16_t NumByteToRead){
	CC2500_read(pBuffer, CC2500_REG_FIFO, NumByteToRead);
}

void write_tx_fifo(uint8_t* pBuffer, uint16_t NumByteToRead){
	CC2500_write(pBuffer, CC2500_REG_FIFO, NumByteToRead);
}

/**
  * @brief  Initializes the low level interface used to drive the CC2500
  * @param  None
  * @retval None
  */
static void CC2500_LowLevel_Init(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

  /* Enable the SPI periph */
  RCC_APB2PeriphClockCmd(CC2500_SPI_CLK, ENABLE);

  /* Enable SCK, MOSI and MISO GPIO clocks */
  RCC_AHB1PeriphClockCmd(CC2500_SPI_SCK_GPIO_CLK | CC2500_SPI_MISO_GPIO_CLK | CC2500_SPI_MOSI_GPIO_CLK, ENABLE);

  /* Enable CS  GPIO clock */
  RCC_AHB1PeriphClockCmd(CC2500_SPI_CS_GPIO_CLK, ENABLE);
  
  /* Enable INT1 GPIO clock */
  RCC_AHB1PeriphClockCmd(CC2500_SPI_INT1_GPIO_CLK, ENABLE);
  
  GPIO_PinAFConfig(CC2500_SPI_SCK_GPIO_PORT, CC2500_SPI_SCK_SOURCE, CC2500_SPI_SCK_AF);
  GPIO_PinAFConfig(CC2500_SPI_MISO_GPIO_PORT, CC2500_SPI_MISO_SOURCE, CC2500_SPI_MISO_AF);
  GPIO_PinAFConfig(CC2500_SPI_MOSI_GPIO_PORT, CC2500_SPI_MOSI_SOURCE, CC2500_SPI_MOSI_AF);

  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_SCK_PIN;
  GPIO_Init(CC2500_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  CC2500_SPI_MOSI_PIN;
  GPIO_Init(CC2500_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI MISO pin configuration */
  GPIO_InitStructure.GPIO_Pin = CC2500_SPI_MISO_PIN;
  GPIO_Init(CC2500_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  /* SPI configuration */
  SPI_I2S_DeInit(CC2500_SPI);
  SPI_InitStructure.SPI_Direction 				= SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_DataSize 					= SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS 							= SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
  SPI_InitStructure.SPI_FirstBit 					= SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;
  SPI_Init(CC2500_SPI, &SPI_InitStructure);

  /* Enable SPI1  */
  SPI_Cmd(CC2500_SPI, ENABLE);

  /* Configure GPIO PIN for Lis Chip select */
  GPIO_InitStructure.GPIO_Pin 	= CC2500_SPI_CS_PIN;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CC2500_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  /* Deselect : Chip Select high */
  GPIO_SetBits(CC2500_SPI_CS_GPIO_PORT, CC2500_SPI_CS_PIN);
  
  /* Configure GPIO PINs to detect Interrupts */
  GPIO_InitStructure.GPIO_Pin 	= CC2500_SPI_INT1_PIN;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(CC2500_SPI_INT1_GPIO_PORT, &GPIO_InitStructure);  	
}
