/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@arduino.cc>
 * Copyright (c) 2014 by Paul Stoffregen <paul@pjrc.com> (Transaction API)
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "SPI.h"

SPIClass::SPIClass(uint32_t _id) :
  id(_id)
{
	// Empty
}

void SPIClass::begin() {
  
  GLBL_REG(GLBL_CFG0)  |= IOF0_SPI_RST;
  GLBL_REG(GLBL_MULTI_FUNC)  |= IOF0_SPI_ENB;
  GLBL_REG(GLBL_MULTI_FUNC)  |= IOF0_SPI_CS0_ENB;
  SPI_REG(SPI_REG_CTRL) = SPI_CTRL_CS_BIT(0x40) | SPI_CTRL_CS_TIM (0x2) | SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_TSIZE(SPI_LEN_0) | SPI_CTRL_SCK(SPI_CLOCK_DIV2) | SPI_CTRL_MODE (SPI_MODE0) | SPI_CTRL_BIT_ENDIAN(SPI_BIT_ENDIAN_BIG);

}

// specifies chip select pin to attach to hardware SPI interface
void SPIClass::begin(uint8_t _pin) {
  	
  // enable CS pin for selected channel/pin
  uint32_t iof_mask = digitalPinToBitMask(_pin);
  GPIO_REG(GPIO_DSEL)   |=  iof_mask;

	this->begin();

}

void SPIClass::usingInterrupt(uint8_t interruptNumber)
{
}

// start an SPI transaction using specified SPIsettings
void SPIClass::beginTransaction(SPISettings settings)
{
  // before starting a transaction, set SPI peripheral to desired mode

  SPI_REG(SPI_REG_CTRL) = SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_TSIZE(SPI_LEN_0) | SPI_CTRL_SCK(settings.sckdiv) | SPI_CTRL_MODE (settings.sckmode) | SPI_CTRL_BIT_ENDIAN(SPI_BIT_ENDIAN_BIG) | SPI_CTRL_BYTE_ENDIAN(SPI_BYTE_ENDIAN_LITTEL);
  

}


// start an SPI transaction using specified CS pin and SPIsettings
void SPIClass::beginTransaction(uint8_t pin, SPISettings settings)
{
  
  this->beginTransaction(settings);

}

void SPIClass::endTransaction(void) {
}

void SPIClass::end(uint8_t _pin) {
  uint32_t iof_mask = digitalPinToBitMask(_pin);
  GPIO_REG(GPIO_DSEL)   &=  ~iof_mask;
  GLBL_REG(GLBL_MULTI_FUNC)  &= ~SPI_IOF_MASK;
}

void SPIClass::end() {
  GLBL_REG(GLBL_MULTI_FUNC)  &= ~SPI_IOF_MASK;
}

void SPIClass::setBitOrder(BitOrder _bitOrder) {
}

void SPIClass::setBitOrder(uint8_t _pin, BitOrder _bitOrder) {
}

void SPIClass::setDataMode(uint8_t _mode) {
}

void SPIClass::setDataMode(uint8_t _pin, uint8_t _mode) {
}

void SPIClass::setClockDivider(uint8_t _divider) {
  SPI_REG(SPI_REG_CTRL) &= ~SPI_CTRL_SCK(0x3F) ;    // Reset the Type
  //SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_SCK(_divider); -- Temp Masked
  SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_SCK(SPI_CLOCK_DIV2);
}

void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider) {
}


byte SPIClass::transfer(byte _pin, uint8_t _data, SPITransferMode _mode) {

  // No need to do anything with the pin, because that was already
  // set up earlier.
  return this->transfer(_data, _mode);
 
}


// For Single Byte Transfer, LITTLE ENDIAN USED,
byte SPIClass::transfer(uint8_t _data, SPITransferMode _mode) {

  // Wait for HW REQ=0
  volatile int32_t x;
  SPI_REG(SPI_REG_WDATA) = _data;
  SPI_REG(SPI_REG_CTRL) &= (~SPI_CTRL_OP(3)) & (~SPI_CTRL_TSIZE(3)) & (~SPI_CTRL_BYTE_ENDIAN(1)) & (~SPI_CTRL_CS_BIT(0xFF)) ;    // Reset the Type
  SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_CS_BIT(0x40) | SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_OP_REQ(1) |  SPI_CTRL_TSIZE(SPI_LEN_0) | SPI_CTRL_BYTE_ENDIAN(SPI_BYTE_ENDIAN_LITTEL);  // Set to Write Mode & Transfer Size: 1 Byte & Request 

  
  while ((x =SPI_REG(SPI_REG_CTRL)) & SPI_CTRL_OP_REQ(1)) ;
  // return SPI_Read(spi);
  x = SPI_REG(SPI_REG_RDATA);
  return x;
  
}

byte SPIClass::write_transfer(uint8_t _data, SPITransferMode _mode) {

  // Wait for HW REQ=0
  volatile int32_t x;
  SPI_REG(SPI_REG_WDATA) = _data;
  SPI_REG(SPI_REG_CTRL) &= (~SPI_CTRL_OP(3)) & (~SPI_CTRL_TSIZE(3)) & (~SPI_CTRL_BYTE_ENDIAN(1)) & (~SPI_CTRL_CS_BIT(0xFF)) ;    // Reset the Type
  SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_CS_BIT(0x40) | SPI_CTRL_OP(SPI_DIR_TX) | SPI_CTRL_OP_REQ(1) |  SPI_CTRL_TSIZE(SPI_LEN_0) | SPI_CTRL_BYTE_ENDIAN(SPI_BYTE_ENDIAN_LITTEL);  // Set to Write Mode & Transfer Size: 1 Byte & Request 
  
  while ((x =SPI_REG(SPI_REG_CTRL)) & SPI_CTRL_OP_REQ(1)) ;
  return 0;

}


// 32 bit Write
// For 32 Bit Transfer, BIG ENDIAN USED,
uint32_t SPIClass::transfer32(uint32_t _data) {

  // Wait for HW REQ=0
  volatile int32_t x;
  SPI_REG(SPI_REG_WDATA) = _data;
  SPI_REG(SPI_REG_CTRL) &= (~SPI_CTRL_OP(3)) & (~SPI_CTRL_TSIZE(3)) & (~SPI_CTRL_BYTE_ENDIAN(1)) & (~SPI_CTRL_CS_BIT(0xFF)) ;    // Reset the Type
  SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_CS_BIT(0x01) | SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_OP_REQ(1) |  SPI_CTRL_TSIZE(SPI_LEN_3) | SPI_CTRL_BYTE_ENDIAN(SPI_BYTE_ENDIAN_BIG);  // Set to Write Mode & Transfer Size: 1 Byte & Request 

  
  while ((x =SPI_REG(SPI_REG_CTRL)) & SPI_CTRL_OP_REQ(1)) ;
  // return SPI_Read(spi);
  x = SPI_REG(SPI_REG_RDATA);
  return x;
  
}


// Transfer 16 bit
uint16_t SPIClass::transfer16(byte _pin, uint16_t _data, SPITransferMode _mode) {

	return transfer16(_data);
}

// 16 bit transfer, assumed MSB Byte first
uint16_t SPIClass::transfer16(uint16_t _data, SPITransferMode _mode) {

	return transfer16(_data);
}

// 16 bit Write
uint16_t SPIClass::transfer16(uint16_t _data) {

  // Wait for HW REQ=0
  volatile int32_t x;
  SPI_REG(SPI_REG_WDATA) = _data << 16;
  SPI_REG(SPI_REG_CTRL) &= (~SPI_CTRL_OP(3)) & (~SPI_CTRL_TSIZE(3)) & (~SPI_CTRL_BYTE_ENDIAN(1)) & (~SPI_CTRL_CS_BIT(0xFF)) ;    // Reset the Type
  SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_CS_BIT(0x10) | SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_OP_REQ(1) |  SPI_CTRL_TSIZE(SPI_LEN_1) | SPI_CTRL_BYTE_ENDIAN(SPI_BYTE_ENDIAN_BIG);  // Set to Write Mode & Transfer Size: 1 Byte & Request 

  
  while ((x =SPI_REG(SPI_REG_CTRL)) & SPI_CTRL_OP_REQ(1)) ;
  // return SPI_Read(spi);
  x = SPI_REG(SPI_REG_RDATA);
  return x;
  
}


// Need Update - Dinesh-A
void SPIClass::transfer(byte _pin, void *_buf, size_t _count, SPITransferMode _mode) {
  
  if (_count == 0)
    return;
  
  uint8_t *buffer = (uint8_t *)_buf;
  if (_count == 1) {
    *buffer = transfer(_pin, *buffer, _mode);
    return;
  }


  volatile int32_t x;
  uint8_t r;
  while (_count > 1) {
    // Read transferred byte and send next one straight away
    r = x & 0xFF;

		// Save read byte
		*buffer = r;
		buffer++;
		_count--;
	}

  r = x & 0xFF;
  *buffer = r;
}

void SPIClass::attachInterrupt(void) {
	// Should be enableInterrupt()
}

void SPIClass::detachInterrupt(void) {
	// Should be disableInterrupt()
}

#if SPI_INTERFACES_COUNT > 0
SPIClass SPI(1);
#endif

