//This sketch is an echo test for the ms14+m32w+ Bluetooth Bee HC-06
//This test also requires: 
// 1) ms14+m32w+ Bluetooth Bee HC-06
// 2) Android Phone with BlueTerm App installed (https://play.google.com/store/apps/details?id=es.pymasde.blueterm)
// 
//How to test?
//1/ Open BlueTerm in your Android phone and pair it with HC-06. 
//2/ Upload this sketch to MS14 and open Serial Monitor
//3/ In the BlueTerm console type a character and you will see the character is shown in the Arduino Serial Monitor. 
//The Android phone will get a "OK" string from HC-06 and shown in the console. 
//
//2014-04-16 Dragino Technology Co., Limited 

#include <Console.h>

#include <SoftwareSerial.h>
#define rxPin 10  // RX Pin on the Xbee socket
#define txPin 9   // TX Pin on the Xbee socket
SoftwareSerial bluetoothbee =  SoftwareSerial(rxPin, txPin);

void setup(){
  //Initialize Console and wait for port to open:
  Bridge.begin();
  Console.begin(); 
  bluetoothbee.begin(9600);
  
  while (!Console) {
    ; // wait for Console port to connect.
  } 
  
  // prints title with ending line break 
  Console.println("Waiting for bluetooth data");   
}


void loop(){
  if (bluetoothbee.available() > 0){
   char c = bluetoothbee.read();  // Get the Android data
   bluetoothbee.println("OK"); //Send back an OK to Android
   Console.println(c);  // Print this char to the Serial Monitor
  }
  delay(100);
}
