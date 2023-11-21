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
  SPI_REG(SPI_REG_CTRL) = SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_TSIZE(SPI_LEN_0) | SPI_CTRL_SCK(SPI_CLOCK_DIV4) | SPI_CTRL_MODE (SPI_MODE0) | SPI_CTRL_BIT_ENDIAN(SPI_ENDIAN_BIG);

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

  SPI_REG(SPI_REG_CTRL) = SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_TSIZE(SPI_LEN_0) | SPI_CTRL_SCK(settings.sckdiv) | SPI_CTRL_MODE (settings.sckmode) | SPI_CTRL_BIT_ENDIAN(SPI_ENDIAN_BIG);
  

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
  SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_SCK(_divider);
}

void SPIClass::setClockDivider(uint8_t _pin, uint8_t _divider) {
}


byte SPIClass::transfer(uint8_t _data, SPITransferMode _mode) {

  // Wait for HW REQ=0
  volatile int32_t x;
  while ((x =SPI_REG(SPI_REG_CTRL)) & SPI_CTRL_OP_REQ(1)) ;
  SPI_REG(SPI_REG_WDATA) = _data;
  
  SPI_REG(SPI_REG_CTRL) &= ~SPI_CTRL_OP(3) ;    // Reset the Type
  SPI_REG(SPI_REG_CTRL) &= ~SPI_CTRL_TSIZE(3) ; // Reset Transfer Size
  SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_OP_REQ(1) |  SPI_CTRL_TSIZE(SPI_LEN_0);  // Set to Write Mode & Transfer Size: 1 Byte & Request 

  
  while ((x =SPI_REG(SPI_REG_CTRL)) & SPI_CTRL_OP_REQ(1)) ;
  // return SPI_Read(spi);
  x = SPI_REG(SPI_REG_RDATA);
  
}

byte SPIClass::write_transfer(uint8_t _data, SPITransferMode _mode) {

  // Wait for HW REQ=0
  volatile int32_t x;
  while ((x =SPI_REG(SPI_REG_CTRL)) & SPI_CTRL_OP_REQ(1)) ;
  SPI_REG(SPI_REG_WDATA) = _data;
  
  SPI_REG(SPI_REG_CTRL) &= ~SPI_CTRL_OP(3) ;    // Reset the Type
  SPI_REG(SPI_REG_CTRL) &= ~SPI_CTRL_TSIZE(3) ; // Reset Transfer Size
  SPI_REG(SPI_REG_CTRL) |= SPI_CTRL_OP(SPI_DIR_TX_RX) | SPI_CTRL_OP_REQ(1) |  SPI_CTRL_TSIZE(SPI_LEN_0);  // Set to Write Mode & Transfer Size: 1 Byte & Request 
}

byte SPIClass::transfer(byte _pin, uint8_t _data, SPITransferMode _mode) {

  // No need to do anything with the pin, because that was already
  // set up earlier.
  return this->transfer(_data, _mode);
 
}

// Transfer 16 bit
uint16_t SPIClass::transfer16(byte _pin, uint16_t _data, SPITransferMode _mode) {
	union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;
	uint32_t ch = SS_PIN_TO_CS_ID(_pin);

	t.val = _data;

	if (bitOrder[ch] == LSBFIRST) {
		t.lsb = transfer(_pin, t.lsb, SPI_CONTINUE);
		t.msb = transfer(_pin, t.msb, _mode);
	} else {
		t.msb = transfer(_pin, t.msb, SPI_CONTINUE);
		t.lsb = transfer(_pin, t.lsb, _mode);
	}

	return t.val;
}

// 16 bit transfer, assumed LSB first - Need to cross-check DineshA
uint16_t SPIClass::transfer16(uint16_t _data, SPITransferMode _mode) {
	union { uint16_t val; struct { uint8_t lsb; uint8_t msb; }; } t;

	t.val = _data;

    t.lsb = transfer(t.lsb, SPI_CONTINUE);
	t.msb = transfer(t.msb, _mode);

	return t.val;
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
  uint8_t r,d;
  while (_count > 1) {
    // Prepare next byte
    d = *(buffer+1);
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

