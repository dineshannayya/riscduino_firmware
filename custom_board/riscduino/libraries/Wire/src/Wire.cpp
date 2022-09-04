/*
  TwoWire.cpp - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 
  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2017 by Chuck Todd (ctodd@cableone.net) to correct Unconfigured Slave Mode reboot
  Modified 2020 by Greyson Christoforo (grey@christoforo.net) to implement timeouts
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "Wire.h"

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;
uint8_t TwoWire::status = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(void)
{

  status  = WIRE_SUCESS;
  rxBufferIndex = 0;
  rxBufferLength = 0;

  txBufferIndex = 0;
  txBufferLength = 0;

  GLBL_REG(GLBL_MULTI_FUNC)  |= WIRE_IOF_MASK; // Enable I2C GPIO I/F
 
  // I2C Clock to 1Mhz 
  uint16_t  i2c_speed = (F_CPU/1000000) -1;
  WIRE_REG(WIRE_REG_BITRATE_L)  = i2c_speed & 0xFF; 
  WIRE_REG(WIRE_REG_BITRATE_H)  = (i2c_speed >> 8) & 0xFF; 
  
  WIRE_REG(WIRE_REG_CTRL)    |= WIRE_CTRL_ENB(1); // Enable I2C core
}

void TwoWire::begin(uint8_t address)
{
  begin();
  //twi_setAddress(address);
}

void TwoWire::begin(int address)
{
  begin((uint8_t)address);
}

void TwoWire::end(void)
{
  //twi_disable();
}

void TwoWire::setClock(uint32_t clock)
{
  uint16_t  i2c_speed = (F_CPU/clock) -1;
  WIRE_REG(WIRE_REG_BITRATE_L)  = i2c_speed & 0xFF; 
  WIRE_REG(WIRE_REG_BITRATE_H)  = (i2c_speed >> 8) & 0xFF; 
}

/***
 * Sets the TWI timeout.
 *
 * This limits the maximum time to wait for the TWI hardware. If more time passes, the bus is assumed
 * to have locked up (e.g. due to noise-induced glitches or faulty slaves) and the transaction is aborted.
 * Optionally, the TWI hardware is also reset, which can be required to allow subsequent transactions to
 * succeed in some cases (in particular when noise has made the TWI hardware think there is a second
 * master that has claimed the bus).
 *
 * When a timeout is triggered, a flag is set that can be queried with `getWireTimeoutFlag()` and is cleared
 * when `clearWireTimeoutFlag()` or `setWireTimeoutUs()` is called.
 *
 * Note that this timeout can also trigger while waiting for clock stretching or waiting for a second master
 * to complete its transaction. So make sure to adapt the timeout to accommodate for those cases if needed.
 * A typical timeout would be 25ms (which is the maximum clock stretching allowed by the SMBus protocol),
 * but (much) shorter values will usually also work.
 *
 * In the future, a timeout will be enabled by default, so if you require the timeout to be disabled, it is
 * recommended you disable it by default using `setWireTimeoutUs(0)`, even though that is currently
 * the default.
 *
 * @param timeout a timeout value in microseconds, if zero then timeout checking is disabled
 * @param reset_with_timeout if true then TWI interface will be automatically reset on timeout
 *                           if false then TWI interface will not be reset on timeout

 */
void TwoWire::setWireTimeout(uint32_t timeout, bool reset_with_timeout){
  //twi_setTimeoutInMicros(timeout, reset_with_timeout);
}

/***
 * Returns the TWI timeout flag.
 *
 * @return true if timeout has occurred since the flag was last cleared.
 */
bool TwoWire::getWireTimeoutFlag(void){
  //return(twi_manageTimeoutFlag(false));
  return false;
}

/***
 * Clears the TWI timeout flag.
 */
void TwoWire::clearWireTimeoutFlag(void){
  //twi_manageTimeoutFlag(true);
}

/**********************************************************************
   I2C Read Transaction
 <Start> <i2c port addr [w]>  <iaddress[w]> <Stop> <Start> <i2 port addr[r]> <rbuf[r]> <Stop>
***********************************************************************/

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint32_t iaddress, uint8_t isize, uint8_t sendStop)
{
  if (isize > 0) {
  // send internal address; this mode allows sending a repeated start to access
  // some devices' internal registers. This function is executed by the hardware
  // TWI module on other processors (for example Due's TWI_IADR and TWI_MMR registers)

  //  <Start> <i2c port addr [w]>
  beginTransmission(address,false);

  // the maximum size of internal address is 3 bytes
  if (isize > 3){
    isize = 3;
  }

  // write internal register address - most significant byte first
  //  <iaddress[w]> <Stop>
  // write internal register address - most significant byte first
  while (isize-- > 0)
    write((uint8_t)(iaddress >> (isize*8)));
  endTransmission(true);
  }

  // clamp to buffer length
  if(quantity > BUFFER_LENGTH){
    quantity = BUFFER_LENGTH;
  }
  // perform blocking read into buffer
  uint8_t read = readFrom(address, rxBuffer, quantity, sendStop);
  // set rx buffer iterator vars
  rxBufferIndex = 0;
  rxBufferLength = read;

  return read;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
	return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

//----------------------------------
// bWrRd: 0 - Write, 1 -> Read
//----------------------------------

void TwoWire::beginTransmission(uint8_t address,bool bWrRd)
{
  volatile int32_t x;
  // indicate that we are transmitting
  transmitting = 1;
  // set address of targeted slave
  txAddress = address;
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  status  = WIRE_SUCESS;
  
  WIRE_REG(WIRE_REG_WDATA) = (address << 1) | bWrRd;
  WIRE_REG(WIRE_REG_CMD)   = WIRE_CMD_WR(1) | WIRE_CMD_START(1) ;
  // WaitWIRE_STAT_RACK for Transmission completion
  x =WIRE_REG(WIRE_REG_STATUS);
  while ((x =WIRE_REG(WIRE_REG_STATUS)) & WIRE_STAT_TIP(1)) ;

  if(x & WIRE_STAT_RACK(1)) status = WIRE_ADDR_NACK;

}

void TwoWire::beginTransmission(uint8_t address)
{
  beginTransmission(address,(bool) false);
}

void TwoWire::beginTransmission(int address)
{
  beginTransmission((uint8_t)address,(bool) false);
}

//
//	Originally, 'endTransmission' was an f(void) function.
//	It has been modified to take one parameter indicating
//	whether or not a STOP should be performed on the bus.
//	Calling endTransmission(false) allows a sketch to 
//	perform a repeated start. 
//
//	WARNING: Nothing in the library keeps track of whether
//	the bus tenure has been properly ended with a STOP. It
//	is very possible to leave the bus in a hung state if
//	no call to endTransmission(true) is made. Some I2C
//	devices will behave oddly if they do not see a STOP.
//
uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
  // transmit buffer (blocking)
  uint8_t ret = writeTo(txAddress, txBuffer, txBufferLength, 1, sendStop);
  // reset tx buffer iterator vars
  txBufferIndex = 0;
  txBufferLength = 0;
  // indicate that we are done transmitting
  transmitting = 0;
  return ret;
}

//	This provides backwards compatibility with the original
//	definition, and expected behaviour, of endTransmission
//
uint8_t TwoWire::endTransmission(void)
{
  return endTransmission(true);
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
  // in master transmitter mode
    // don't bother if buffer is full
    if(txBufferLength >= BUFFER_LENGTH){
      setWriteError();
      return 0;
    }
    // put byte in tx buffer
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    // update amount in buffer   
    txBufferLength = txBufferIndex;
  return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
  // in master transmitter mode
    for(size_t i = 0; i < quantity; ++i){
      write(data[i]);
    }
  return quantity;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::read(void)
{
  int value = -1;
  
  // get each successive byte on each call
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }

  return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
  int value = -1;
  
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }

  return value;
}

void TwoWire::flush(void)
{
  // XXX: to be implemented.
}

// behind the scenes function that is called when data is received
void TwoWire::onReceiveService(uint8_t* inBytes, int numBytes)
{
  // don't bother if user hasn't registered a callback
  if(!user_onReceive){
    return;
  }
  // don't bother if rx buffer is in use by a master requestFrom() op
  // i know this drops data, but it allows for slight stupidity
  // meaning, they may not have read all the master requestFrom() data yet
  if(rxBufferIndex < rxBufferLength){
    return;
  }
  // copy twi rx buffer into local read buffer
  // this enables new reads to happen in parallel
  for(uint8_t i = 0; i < numBytes; ++i){
    rxBuffer[i] = inBytes[i];    
  }
  // set rx iterator vars
  rxBufferIndex = 0;
  rxBufferLength = numBytes;
  // alert user program
  user_onReceive(numBytes);
}

// behind the scenes function that is called when data is requested
void TwoWire::onRequestService(void)
{
  // don't bother if user hasn't registered a callback
  if(!user_onRequest){
    return;
  }
  // reset tx buffer iterator vars
  // !!! this will kill any pending pre-master sendTo() activity
  txBufferIndex = 0;
  txBufferLength = 0;
  // alert user program
  user_onRequest();
}

// sets function called on slave write
void TwoWire::onReceive( void (*function)(int) )
{
  user_onReceive = function;
}

// sets function called on slave read
void TwoWire::onRequest( void (*function)(void) )
{
  user_onRequest = function;
}

uint8_t TwoWire:: readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
  uint8_t i;

  //  <Start> <i2c port addr [R]>
  beginTransmission(address,true);
  
  // copy twi buffer to data
  for(i = 0; i < length; ++i){
    if(i == length-1) {
        data[i] = readByte(true); // Send Last Tranaction with Nack
    } else {
        data[i] = readByte(false); // Send Read with ack
    }
  }
  endTransmission(true);
	
  return length;
}


//---------------------------------------------------------
// Normally Last Read transaction should exit with Nack=1
//---------------------------------------------------------
uint8_t TwoWire:: readByte(bool bNack) {

    uint8_t data;
    volatile int32_t x;

    WIRE_REG(WIRE_REG_CMD)   = WIRE_CMD_RD(1) | WIRE_CMD_ACK(bNack);
    x =WIRE_REG(WIRE_REG_STATUS);
    while ((x =WIRE_REG(WIRE_REG_STATUS)) & WIRE_STAT_TIP(1)) ;
    data= WIRE_REG(WIRE_REG_RDATA) ;
    return data;

}

uint8_t TwoWire:: readByte() {
     return readByte((bool) false);
}

/* 
 * Function twi_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 *          sendStop: boolean indicating whether or not to send a stop at the end
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 *          5 .. timeout
 */
uint8_t TwoWire::writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
  volatile int32_t x;
  uint8_t i;


  
  // Send the Data Byte
  for(i = 0; i < length; ++i){
    WIRE_REG(WIRE_REG_WDATA) = data[i];
    WIRE_REG(WIRE_REG_CMD)   = WIRE_CMD_WR(1) ;
    // Wait for Transmission completion
    x =WIRE_REG(WIRE_REG_STATUS);
    while ((x =WIRE_REG(WIRE_REG_STATUS)) & WIRE_STAT_TIP(1)) ;
    if(x == WIRE_STAT_RACK(1)) {
         status = WIRE_DATA_NACK;
         return status;
    }
  }
  
  if( sendStop) {
     // Send Stop
     WIRE_REG(WIRE_REG_CMD)   = WIRE_CMD_STOP(sendStop) ;
     x =WIRE_REG(WIRE_REG_STATUS);
     while ((x =WIRE_REG(WIRE_REG_STATUS)) & WIRE_STAT_FSM_BUSY(1)) ;
  }

  return status;	// success
  //if (error == 0xFF)
  //  return 0;	// success
  //else if (error == TW_MT_SLA_NACK)
  //  return 2;	// error: address send, nack received
  //else if (error == TW_MT_DATA_NACK)
  //  return 3;	// error: data send, nack received
  //else
  //  return 4;	// other twi error
}


// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();

