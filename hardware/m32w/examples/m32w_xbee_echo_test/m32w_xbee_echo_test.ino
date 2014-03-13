//This sketch is an echo test for the ms14+m32w+XBee
//This test use two XBee module, XBee-A is connected to PC and control by X-CTU tool. XBee-B is loaded in the m32w. 
//Both XBee are pre-configure to work at the same network. 
//User can put chareters in X-CTU so the chars will be sent from XBee-A to XBee-B then to m32w, m32w will return the same to XBee-A so to finished
//a simple echo test. 
//2014-03-13 Dragino Technology Co., Limited 

#include <SoftwareSerial.h>
#define rxPin 10  // RX Pin on the Xbee socket
#define txPin 9   // TX Pin on the Xbee socket
SoftwareSerial xbee =  SoftwareSerial(rxPin, txPin);

void setup(){
  xbee.begin(9600);
}


void loop(){
  if (xbee.available() > 0){
   xbee.write(xbee.read());
}
