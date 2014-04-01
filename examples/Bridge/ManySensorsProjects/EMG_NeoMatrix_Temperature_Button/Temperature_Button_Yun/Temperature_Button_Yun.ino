/*
   Temperature and Button Yun example

 This example for the Arduino Yun shows how to use the
 Bridge library to get the temperature and the button status 
 on the board through REST calls. 

 * "/arduino/get/x


 */

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
//include DHT11 temperature sensor lib
#include "DHT.h"
// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;


#define DHTPIN A6 // which signal pin the sensor is connected to
#define DHTTYPE DHT11 // DHT 11
#define btn_pin 3  // the button is connect to digital 3
//define sensor object
DHT dht(DHTPIN, DHTTYPE);
float temperature;

void setup() {
  // Bridge startup
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  dht.begin();
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  float t = dht.readTemperature();
  if (isnan(t))
  {
  }
  else
  {
    temperature = t;
  }
  
  // Get clients coming from server
  YunClient client = server.accept();
  
  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  delay(50); // Poll every 50ms
}

void process(YunClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "get" command, return temperature and button states
  if (command == "get") {
    client.print(temperature);
    client.print(F(","));
    client.println(digitalRead(btn_pin));    
  }
}


