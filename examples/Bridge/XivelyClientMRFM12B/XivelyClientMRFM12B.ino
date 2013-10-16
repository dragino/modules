/*
  Xively sensor client with Strings 
 
 This sketch connects an analog sensor to Xively,
 using an Arduino Yún. 
 
 This sketch also shows how to call a linux script and pass parameters to this script. 
 It will be easy to maintain if the complicate things are handle in Linux side by a script
 and the AVR just do the very basic feature, collect data and send. 
 
 To use this Sketch, user require to upload the xively.lua script and put it into the /root/ directory
 of MS14
 
 created 15 March 2010
 updated 27 May 2013
 by Tom Igoe
 
 updated 16, Oct, 2013 to support mega168
 by Edwin Chen
 */


// include all Libraries needed:
#include <Process.h>



// set up net client info:
const unsigned long postingInterval = 60000;  //delay between updates to xively.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

void setup() {
  // start serial port: 
  Bridge.begin();
  Console.begin();

  while(!Console);    // wait for Network Serial to open
  Console.println("Xively client");

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
  dataString = "Temperature,";
  dataString += random(10) + 20;
  // add pressure:
  dataString += "\nPressure,";
  dataString += random(5) + 100;
}

// this method makes a HTTP connection to the server:
void sendData() {

  // Send the HTTP PUT request

  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process xively;
  Console.print("\n\nSending data... ");
  xively.begin("lua");
  xively.addParameter("/root/xively.lua");
  xively.addParameter(dataString);
  xively.run();
  Console.println("done!");

  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (xively.available()>0) {
    char c = xively.read();
    Console.write(c);
  }

}




