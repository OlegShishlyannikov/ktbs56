#include "adc_spi_driver.hpp"

adc_spi_driver::adc_spi_driver()
{
  this->adc_spi_driver_gpio_init();
  this->adc_spi_driver_periph_init();
}

void adc_spi_driver::adc_spi_driver_gpio_init()
{
  GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = ADC_SPI_GPIO_MISO_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( ADC_SPI_GPIO_PORT, GPIO_InitStruct ); 

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = ADC_SPI_GPIO_SCK_PIN | ADC_SPI_GPIO_MOSI_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( ADC_SPI_GPIO_PORT,  GPIO_InitStruct );
  
  delete GPIO_InitStruct;
}

void adc_spi_driver::adc_spi_driver_gpio_deinit()
{
    GPIO_InitTypeDef * GPIO_InitStruct = new GPIO_InitTypeDef;

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = ADC_SPI_GPIO_MISO_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( ADC_SPI_GPIO_PORT, GPIO_InitStruct ); 

  GPIO_StructInit( GPIO_InitStruct );
  GPIO_InitStruct->GPIO_Pin = ADC_SPI_GPIO_SCK_PIN | ADC_SPI_GPIO_MOSI_PIN;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IPU;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init( ADC_SPI_GPIO_PORT,  GPIO_InitStruct );
  
  delete GPIO_InitStruct;
}

void adc_spi_driver::adc_spi_driver_periph_init()
{
  SPI_InitTypeDef * SPI_InitStruct = new SPI_InitTypeDef;

  SPI_StructInit( SPI_InitStruct );
  SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
  SPI_InitStruct->SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStruct->SPI_DataSize = SPI_DataSize_16b;
  SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStruct->SPI_Mode = SPI_Mode_Master;
  SPI_InitStruct->SPI_NSS = SPI_NSS_Soft;
  SPI_Init( ADC_SPI_PERIPH, SPI_InitStruct );
  SPI_Cmd( ADC_SPI_PERIPH, ENABLE );
  
  delete SPI_InitStruct;
}

uint16_t adc_spi_driver::adc_get_data( ADS1118_ConfRegInitTypeDef * ADS1118_ConfRegInitStruct )
{
  std::bitset< 16 > * data_packet = new std::bitset< 16 >(( ADS1118_ConfRegInitStruct->OS.to_ulong() << ADS1118_SS_POS ) |
														  ( ADS1118_ConfRegInitStruct->MUX.to_ulong() << ADS1118_MUX_POS ) |
														  ( ADS1118_ConfRegInitStruct->PGA.to_ulong() << ADS1118_PGA_POS ) |
														  ( ADS1118_ConfRegInitStruct->MODE.to_ulong() << ADS1118_OPMODE_POS ) |
														  ( ADS1118_ConfRegInitStruct->DR.to_ulong() << ADS1118_DR_POS ) |
														  ( ADS1118_ConfRegInitStruct->TS_MODE.to_ulong() << ADS1118_TSMODE_POS ) |
														  ( ADS1118_ConfRegInitStruct->PULL_UP_EN.to_ulong() << ADS1118_PULLUPEN_POS ) |
														  ( ADS1118_ConfRegInitStruct->NOP.to_ulong() << ADS1118_NOP_POS ) |
														  ( ADS1118_ConfRegInitStruct->RDY_FLAG.to_ulong() << ADS1118_RDY_FLAG_POS )
														  );

  while( SPI_I2S_GetFlagStatus( ADC_SPI_PERIPH, SPI_I2S_FLAG_BSY ));
  SPI_I2S_SendData( ADC_SPI_PERIPH, data_packet->to_ulong() );
  while( SPI_I2S_GetFlagStatus( ADC_SPI_PERIPH, SPI_I2S_FLAG_BSY ));
  SPI_I2S_SendData( ADC_SPI_PERIPH, data_packet->to_ulong() );

  /* Receive data & start new conversion */
  while( GPIO_ReadInputDataBit( ADC_SPI_GPIO_PORT, ADC_SPI_GPIO_MISO_PIN ));
  while( SPI_I2S_GetFlagStatus( ADC_SPI_PERIPH, SPI_I2S_FLAG_BSY ));
  SPI_I2S_SendData( ADC_SPI_PERIPH, data_packet->to_ulong() );
  uint16_t data = SPI_I2S_ReceiveData( ADC_SPI_PERIPH );
  while( SPI_I2S_GetFlagStatus( ADC_SPI_PERIPH, SPI_I2S_FLAG_BSY ));
  SPI_I2S_SendData( ADC_SPI_PERIPH, data_packet->to_ulong() );
  delete data_packet;
  return data;
}

void adc_spi_driver::adc_transmit_data( ADS1118_ConfRegInitTypeDef * ADS1118_ConfRegInitStruct )
{
  std::bitset< 16 > * data_packet = new std::bitset< 16 >(( ADS1118_ConfRegInitStruct->OS.to_ulong() << ADS1118_SS_POS ) |
														  ( ADS1118_ConfRegInitStruct->MUX.to_ulong() << ADS1118_MUX_POS ) |
														  ( ADS1118_ConfRegInitStruct->PGA.to_ulong() << ADS1118_PGA_POS ) |
														  ( ADS1118_ConfRegInitStruct->MODE.to_ulong() << ADS1118_OPMODE_POS ) |
														  ( ADS1118_ConfRegInitStruct->DR.to_ulong() << ADS1118_DR_POS ) |
														  ( ADS1118_ConfRegInitStruct->TS_MODE.to_ulong() << ADS1118_TSMODE_POS ) |
														  ( ADS1118_ConfRegInitStruct->PULL_UP_EN.to_ulong() << ADS1118_PULLUPEN_POS ) |
														  ( ADS1118_ConfRegInitStruct->NOP.to_ulong() << ADS1118_NOP_POS ) |
														  ( ADS1118_ConfRegInitStruct->RDY_FLAG.to_ulong() << ADS1118_RDY_FLAG_POS )
														  );
  while( SPI_I2S_GetFlagStatus( ADC_SPI_PERIPH, SPI_I2S_FLAG_BSY ));
  SPI_I2S_SendData( ADC_SPI_PERIPH, data_packet->to_ulong() );
  delete data_packet; 
}

void adc_spi_driver::adc_spi_driver_periph_deinit()
{
  SPI_I2S_DeInit( ADC_SPI_PERIPH );
}

adc_spi_driver::~adc_spi_driver()
{
  this->adc_spi_driver_gpio_deinit();
  this->adc_spi_driver_periph_deinit();
}
