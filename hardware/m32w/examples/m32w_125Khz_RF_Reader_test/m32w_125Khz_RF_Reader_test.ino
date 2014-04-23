//This sketch shows how to use RFM6300 to read a EM4100 RFID card
//This test requires: 
// 1) ms14+m32w+ 125Khz EM4100 RF Reader (RDM6300)
// 
//How to test?
//1/ Connect the wires between m32w and RDM6300 according to (http://wiki.dragino.com/index.php?title=M32W#RDM6300_--------_125Khz_RF_Card_Reader) 
//2/ Upload this sketch to MS14 and open Serial Monitor
//3/ Then put a card close to the coil. The card ID will be read and shown in the Arduino Serial Monitor via WiFi.  
//
//2014-04-16 Dragino Technology Co., Limited 

#include <Console.h>

#include <SoftwareSerial.h>
#define rxPin 10  // RX Pin to RF reader socket
#define txPin 9   // TX Pin to RF reader
SoftwareSerial RF_Reader =  SoftwareSerial(rxPin, txPin);
int data = 0;

void setup(){
  //Initialize Console and wait for port to open:
  Bridge.begin();
  Console.begin(); 
  RF_Reader.begin(9600);
  
  while (!Console) {
    ; // wait for Console port to connect.
  } 
  
  // prints title with ending line break 
  Console.println("Waiting for RF Reader data");   
}


void loop(){
  if (RF_Reader.available() > 0){
   // read the incoming number on serial RX
    delay(100);  // Needed to allow time for the data to come in from the serial buffer.
    for (int z=0; z<14; z++) // read the rest of the tag
    {
      data=RF_Reader.read();
      Console.print(data,DEC); // print the data on Arduino Serial Monito
    }
    RF_Reader.flush(); // stops multiple reads  
    Console.println();
  }
}
