/*
 Cumulocity client demo.  
 
 This sketch do below task:
 1) Check if the device is registed in Cumulocity, if not, create a device;if yes, get the device ID from Cumulocity
 and store it in MS14
 2) Set up measurement types in Cumulocity.
 3) Update measurements from different sensors. 
 
 Below firmware/hardware are required to run this sketch:
 1) hardware MS14-S and M32
 2) firmware on MS14-S: Yun firmware ver RC1.1
  
 Created 10, Feb, 2014
 by Dragino Technology Co., Limited
 
 Reference Link:
 
 */

// include all Libraries needed:
#include <Process.h>

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
  
  // convert the readings to a String to send it:
  dataString = "\"Temperature Sensor\":{\"T\":{\"value\":";
  dataString += random(10) + 20;
  dataString += ",\"unit\":\"C\"}},";
  // add pressure:
  dataString += "\"Pressure Sensor\":{\"P\":{\"value\":";
  dataString += random(5) + 100;
  dataString += ",\"unit\":\"Pa\"}}";
  
  Process cumulocity;
  Console.print("\n\nPost Measurements... ");
  cumulocity.begin("lua");
  cumulocity.addParameter("/usr/lib/lua/dragino/iot/cumulocity.lua");
  cumulocity.addParameter("-p");
  cumulocity.addParameter(dataString);
  cumulocity.run();
  Console.println("done!");

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
  Console.println("done!");

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
  cumulocity.addParameter("\"Temperature Sensor,Pressure Sensor\"");
  cumulocity.run();
  Console.println("   done!");

  // Get return info
  while (cumulocity.available()>0) {
    char c = cumulocity.read();
    Console.write(c);
  }

}




