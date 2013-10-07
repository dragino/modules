/*
  Emoncms sensor client with Strings 
 
 This sketch connects an analog sensor to Emoncms,
 using a ms14 with Yún + Lenonard. 
 
 created 15 March 2010
 updated 27 May 2013
 by Tom Igoe
   
 updated 6, Oct, 2013 to support Emoncms
 by Edwin Chen
   
 */


// include all Libraries needed:
#include <Process.h>
#include "passwords.h"      // contains my passwords, see below

/*
  NOTE: passwords.h is not included with this repo because it contains my passwords.
 You need to create it for your own version of this application.  To do so, make
 a new tab in Arduino, call it passwords.h, and include the following variables and constants:
 
 #define APIKEY        "foo"                  // replace your pachube api key here
 #define USERAGENT     "my-project"           // user agent is the project name
 */


// set up net client info:
const unsigned long postingInterval = 60000;  //delay between updates to emoncms.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

void setup() {
  // start serial port:
  Bridge.begin();
  Serial.begin(9600);

  while(!Serial);    // wait for Network Serial to open
  Serial.println("Emoncms client");

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
  dataString = "Temperature:";
  dataString += random(10) + 20;
  // add pressure:
  dataString += ",Pressure:";
  dataString += random(5) + 100;
}

// this method makes a HTTP connection to the server:
void sendData() {
  // form the string for the API header parameter:
  String apiString = "apikey=";
  apiString += APIKEY;

  // form the string for the URL parameter:
  String url = "http://emoncms.org/api/post?";
  url += "json={";
  url += dataString;
  url += "}&";
  url += apiString;
  // Send the HTTP PUT request

  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process emoncms;
  Serial.print("\n\nSending data... ");
  emoncms.begin("curl");
  emoncms.addParameter(url);
  emoncms.run();
  Serial.println("done!");

  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (emoncms.available()>0) {
    char c = emoncms.read();
    Serial.write(c);
  }

}




