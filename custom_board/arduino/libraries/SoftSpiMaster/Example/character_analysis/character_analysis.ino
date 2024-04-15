/*
   Read the Serial Port Data and Transmit through SPI Master I/F

*/
#include "SoftSpiMaster.h"

SoftSpiMaster SpiM(2,3,4);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  SpiM.begin();
  SpiM.setBitOrder(SPI_MODE1);
  // send an intro:
  Serial.println("send any byte and I'll tell you everything I can about it");
  Serial.println();
}

void loop() {
  // get any incoming bytes:
  if (Serial.available() > 0) {
    int thisChar = Serial.read();

    // say what was sent:
    Serial.print("You sent me: \'");
    Serial.write(thisChar);
    Serial.print("\'  ASCII Value: ");
    Serial.println(thisChar);
    SpiM.transfer(thisChar);
    
    // add some space and ask for another byte:
    Serial.println();
    Serial.println("Give me another byte:");
    Serial.println();
  }
}
