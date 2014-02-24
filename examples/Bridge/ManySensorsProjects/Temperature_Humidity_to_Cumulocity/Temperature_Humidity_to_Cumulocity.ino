/*
 Cumulocity client demo.  
 
 This sketch do below task:
 1) Check if the device is registed in Cumulocity, if not, create a device;if yes, get the device ID from Cumulocity
 and store it in MS14
 2) Set up measurement types in Cumulocity.
 3) Update measurements from temp and huminity sensors. 
 
 Below firmware/hardware are required to run this sketch:
 1) hardware MS14-S and M32
 2) firmware on MS14-S: Yun firmware ver RC1.2
 3) DHT humidity/temperature sensors
  
 Created 21, Feb, 2014
 by Dragino Technology Co., Limited
 
 Reference Link: http://wiki.dragino.com/index.php?title=Cumulocity#Example_2:_Set_and_get_an_alarm
 
 */

// include all Libraries needed:
#include "DHT.h"
#include <Process.h>

#define DHTPIN A0 // which signal pin the sensor is connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302)
//#define DHTTYPE DHT21 // DHT 21 (AM2301)

//define sensor object
DHT dht(DHTPIN, DHTTYPE);

static char tbuffer[6];
static char hbuffer[6];


// set up net client info:
const unsigned long postingInterval = 60000;  //delay between updates to cumulocity.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

void setup() {
  // start serial port: 
  Bridge.begin();
  Console.begin();

  while(!Console);    // wait for Network Serial to open
  Console.println("Cumulocity client");


  registerDevice();
  setMeasurementTypes();
  
  dht.begin();
  lastRequest = millis();
}

void loop() {
  // get a timestamp so you can calculate reading and sending intervals:
  long now = millis();

  // if the sending interval has passed since your
  // last connection, then connect again and send data:
  if (now - lastRequest >= postingInterval) {
    updateData();
    lastRequest = now;
  }
}


//Post measurements to Cumulocity
void updateData() {
  // Send this command to MS14
  // lua /usr/lib/lua/dragino/iot/cumulocity.lua -p string
    //Single sensor example :  -p '"Customize Measurement": { "Point": { "value": 200, "unit": "ml" }}'
    //Multi sensor example : -p '"Customize Measurement": { "Point": { "value": 200, "unit": "ml" }},"TemperatureSensor": { "T": { "value": 35, "unit": "C" }}'

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // Set Alarm if temperature > 40
  if (t > 40) setTempAlarm();

  //convert the sensor data from float to string
  dtostrf(t,5, 2, tbuffer);
  dtostrf(h,5, 2, hbuffer);
  
  // convert the readings to a String to send it:
  dataString = "\"Temperature Sensor\":{\"T\":{\"value\":";
  dataString += t;
  dataString += ",\"unit\":\"C\"}},";
  // add pressure:
  dataString += "\"Humidity Sensor\":{\"H\":{\"value\":";
  dataString += h;
  dataString += ",\"unit\":\"%%\"}}";
  
  Process cumulocity;
  Console.print("\n\nPost Measurements... ");
  cumulocity.begin("lua");
  cumulocity.addParameter("/usr/lib/lua/dragino/iot/cumulocity.lua");
  cumulocity.addParameter("-p");
  cumulocity.addParameter(dataString);
  cumulocity.run();

  // Get return info
  while (cumulocity.available()>0) {
    char c = cumulocity.read();
    Console.write(c);
  }
}

// Registered this device on Cumulocity
void registerDevice() {

  // Send this command to MS14
  // lua /usr/lib/lua/dragino/iot/cumulocity.lua -g
  Process cumulocity;
  Console.print("\n\nRegister Device... ");
  cumulocity.begin("lua");
  cumulocity.addParameter("/usr/lib/lua/dragino/iot/cumulocity.lua");
  cumulocity.addParameter("-g");
  cumulocity.run();

  // Get return info
  while (cumulocity.available()>0) {
    char c = cumulocity.read();
    Console.write(c);
  }

}

// Setup measurement types
void setMeasurementTypes() {

  // Send this command to MS14
  // lua /usr/lib/lua/dragino/iot/cumulocity.lua -u "Temperature Sensor,Pressure Sensor"
  Process cumulocity;
  Console.print("\n\nSetup measurement types... ");
  cumulocity.begin("lua");
  cumulocity.addParameter("/usr/lib/lua/dragino/iot/cumulocity.lua");
  cumulocity.addParameter("-u");
  cumulocity.addParameter("\"Temperature Sensor,Humidity Sensor\"");
  cumulocity.run();

  // Get return info
  while (cumulocity.available()>0) {
    char c = cumulocity.read();
    Console.write(c);
  }

}

// Send Temperature Alarm
void setTempAlarm() {

  // Send this command to MS14
  // lua /usr/lib/lua/dragino/iot/cumulocity.lua -sa 0 "Overheated"
  Process cumulocity;
  Console.print("\n\nSend Alarm Cumulocity ");
  cumulocity.begin("lua");
  cumulocity.addParameter("/usr/lib/lua/dragino/iot/cumulocity.lua");
  cumulocity.addParameter("-sa");
  cumulocity.addParameter("0");
  cumulocity.addParameter("\"Overheated\"");
  cumulocity.run();

  // Get return info
  while (cumulocity.available()>0) {
    char c = cumulocity.read();
    Console.write(c);
  }

}




