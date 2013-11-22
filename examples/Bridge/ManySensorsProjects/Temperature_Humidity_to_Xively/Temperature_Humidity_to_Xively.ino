// Example testing sketch for various DHT humidity/temperature sensors
// and update to xively server

#include "DHT.h"
#include <Console.h>
#include <Process.h>

//define xively connect info
#define APIKEY        "xxxxxx"   // replace your xively api key here
#define FEEDID        "xxxxxx"                   // replace your xively feed ID

#define DHTPIN A0 // which signal pin the sensor is connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 // DHT 11
//#define DHTTYPE DHT22 // DHT 22 (AM2302)
//#define DHTTYPE DHT21 // DHT 21 (AM2301)

//define sensor object
DHT dht(DHTPIN, DHTTYPE);

// set up net client info:
const unsigned long postingInterval = 60000;  //delay between updates to xively.com
unsigned long lastRequest = 0;      // when you last made a request
String dataString = "";

static char tbuffer[6];
static char hbuffer[6];

void setup()
{
    Bridge.begin();
    Console.begin(); 

  while (!Console) {
    ; // wait for Console port to connect.
  }

    dht.begin();
}

void updateData() {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(t) || isnan(h))
    {
        Console.println("Failed to read from DHT");
    }
    else
    {
            // print the sensor data in serial monitor
        Console.print("Humidity: ");
        Console.print(h);
        Console.print(" %\t");
        Console.print("Temperature: ");
        Console.print(t);
        Console.println(" *C");

       //convert the sensor data from float to string
    dtostrf(t,5, 2, tbuffer);
    dtostrf(h,5, 2, hbuffer);
      
      // convert the readings to a String to send it:
        dataString = "Temperature,";
        
        dataString += tbuffer;
      //add Humidity:
        dataString += "\nHumidity,";
        dataString += hbuffer;
    }
}

// this method makes a HTTP connection to the server:
void sendData() {
  // form the string for the API header parameter:
  String apiString = "X-ApiKey: ";
  apiString += APIKEY;

  // form the string for the URL parameter:
  String url = "https://api.xively.com/v2/feeds/";
  url += FEEDID;
  url += ".csv";

  // Send the HTTP PUT request

  // Call a command in MS14.
  // Is better to declare the Process here, so when the
  // sendData function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process xively;
  Console.print("Sending data... ");
  xively.begin("curl");
  xively.addParameter("-k");
  xively.addParameter("--request");
  xively.addParameter("PUT");
  xively.addParameter("--data");
  xively.addParameter(dataString);
  xively.addParameter("--header");
  xively.addParameter(apiString); 
  xively.addParameter(url);
  xively.run();
  Console.println("done!\n\n");

  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (xively.available()>0) {
    char c = xively.read();
    Console.write(c);
  }

}

void loop()
{
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
