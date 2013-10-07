/*
  Yeelink sensor client with Strings 
 
 This sketch connects an analog sensor to Emoncms,
 using a ms14 with Yún + Lenonard. 
 
 created 15 March 2010
 updated 27 May 2013
 by Tom Igoe
   
 updated 6, Oct, 2013 to support Yeelink
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
 #define DEVICEID      ""                     // Device ID
 #define SENSORID_TEMPERATURE ""              // Sensor ID for Temperature
 #define USERAGENT     "my-project"           // user agent is the project name
 */


// set up net client info:
const unsigned long postingInterval = 60000;  //delay between updates to emoncms.com
unsigned long lastRequest = 0;      // when you last made a request
String dataStringTemp = "";


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
  dataStringTemp = "{\"value\":";
  dataStringTemp += random(10) + 20;
  dataStringTemp += "}";
}

// this method makes a HTTP connection to the server:
void sendData() {
  // form the string for the API header parameter:
  String apiString = "U-ApiKey: ";
  apiString += APIKEY;

  // form the string for the URL parameter:
  String url = "http://api.yeelink.net/v1.0/";
  url += "device/";
  url += DEVICEID;
  url += "/sensor/";
  url += SENSORID_TEMPERATURE;
  url += "/datapoints";
  // Send the HTTP PUT request

  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process yeelink;
  Serial.print("\n\nSending data... ");
  yeelink.begin("curl");
  yeelink.addParameter("--request");
  yeelink.addParameter("POST");
  yeelink.addParameter("--data-binary");
  yeelink.addParameter(dataStringTemp);
  yeelink.addParameter("--header");
  yeelink.addParameter(apiString);
  yeelink.addParameter(url);
  yeelink.run();
  Serial.println("done!");

  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (yeelink.available()>0) {
    char c = yeelink.read();
    Serial.write(c);
  }

}




