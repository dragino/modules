/*
  Yeelink sensor client with Strings 
 
 This sketch connects an analog sensor to Yeelink,
 using a ms14 with Yún + MRFM12B. 
 
 This sketch also shows how to call a linux script and pass parameters to this script. 
 It will be easy to maintain if the complicate things are handle in Linux side by a script
 and the AVR just do the very basic feature, collect data and send. 
 
 To use this Sketch, user require to upload the yeelink.lua script and put it into the /root/ directory
 of MS14
 
 created 15 March 2010
 updated 27 May 2013
 by Tom Igoe
   
 updated 16, Oct, 2013 to support Yeelink
 by Edwin Chen
   
 */

// include all Libraries needed:
#include <Process.h>
#include <Console.h>


// set up net client info:
const unsigned long postingInterval = 60000;  //delay between updates to emoncms.com
unsigned long lastRequest = 0;      // when you last made a request
String dataStringTemp = "";


void setup() {
  // start serial port:
  Bridge.begin();
  Console.begin();

  while(!Console);    // wait for Network Serial to open
  Console.println("Yeelink client");

  // Do a first update immediately
  updateData();
  sendData();
  lastRequest = millis();
}

void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // if the sending interval has passed since your
  // last connection, then connect again and send data:
  if (now - lastRequest >= postingInterval) {
    updateData();
    sendData();
    lastRequest = now;
  }
}

void updateData() {
  // convert the readings to a String to send it:
  dataStringTemp = "{\"value\":";
  dataStringTemp += random(10) + 20;
  dataStringTemp += "}";
}

// this method makes a HTTP connection to the server:
void sendData() {

  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process yeelink;
  yeelink.begin("lua");
  yeelink.addParameter("/root/yeelink.lua");
  yeelink.addParameter(dataStringTemp);
  yeelink.run();
  Console.println("done!");

  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (yeelink.available()>0) {
    char c = yeelink.read();
    Console.write(c);
  }

}




