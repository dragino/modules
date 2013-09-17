//////////////////////////////////////////////////////////////////
//  This program is designed to read the temperature from Dallas sensors. It is originally designed for 
//  Dragino MT-DS temperature processing module, it can be used by other avr by re-define the ONE_WIRE_BUS[] array 
//  and NUMBER_OF_BUS
//  
//  This program will read the temperature from Dallas sensors and send the ID;value pair to Dragino MS12 via UART port. 
//  The code supports below Dallas sensors: 
//     DS18B20
//     DS1822
//     DS1820
//  This code doesn't consider the Parasite power mode on Dallas Temp. 
//
//  more info about MT-DS module please refer http://wiki.dragino.com
//
//   created Sep.28 2012
//   by Edwin Chen <edwin@dragino.com>.
//   Shenzhen, China
//
////////////////////////////////////////////////////////////////// 

#include <OneWire.h>
#include <DallasTemperature.h>

// Define the bus to connect to sensors and temperature precision. in MT-DS, we use 8 buses to connect to Dallas Sensors. 
const int ONE_WIRE_BUS[]={2,3,4,5,6,7,8,9};
#define TEMPERATURE_PRECISION 9
#define NUMBER_OF_BUS 8
#define POLL_TIME 10000    //how long the avr will poll every sensors 

// Setup oneWire instances to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire *oneWire[NUMBER_OF_BUS];

// Pass onewire reference to Dallas temperatures. 
DallasTemperature *sensors[NUMBER_OF_BUS];

// arrays to hold device addresses
DeviceAddress tempDeviceAddress[8];

int numberOfDevices[NUMBER_OF_BUS];

void setup(void)
{
  // start serial port
  Serial.begin(9600);

  delay(5000);   // avoid the serial interupt the booting of Dragino. 
  Serial.println(""); // add this so the first sending line can be shown in Dragino. 
  
  //check how many sensors are connect to MT-DS, normally the result won't showed in Dragion 
  //because Dragino has a longer boot time compare to avr, the code wil be useful when debug. 
  for(int i=0;i<NUMBER_OF_BUS; i++)   //search each bus one by one
  {
    oneWire[i] = new OneWire(ONE_WIRE_BUS[i]);
    sensors[i] = new DallasTemperature(oneWire[i]);
    sensors[i]->begin();
    numberOfDevices[i] = sensors[i]->getDeviceCount();
    Serial.print("Locating devices...");
    Serial.print("Found ");
    Serial.print(numberOfDevices[i], DEC);
    Serial.print(" devices on port ");
    Serial.println(ONE_WIRE_BUS[i],DEC);
    for(int j=0;j<numberOfDevices[i]; j++)
    {
      // Search the wire for address
      if(sensors[i]->getAddress(tempDeviceAddress[j], j))
	{
		Serial.print("Found device ");
		Serial.print(j, DEC);
		Serial.print(" with address: ");
		printAddress(tempDeviceAddress[j]);
		Serial.print("");
		
		// set the resolution to TEMPERATURE_PRECISION bit (Each Dallas/Maxim device is capable of several different resolutions)
		sensors[i]->setResolution(tempDeviceAddress[j], TEMPERATURE_PRECISION);
	}else{
		Serial.print("Found ghost device at ");
		Serial.print(j, DEC);
		Serial.print(" but could not detect address. Check power and cabling");
	}
    }
      delay(500);
  }
  
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop(void)
{ 
  for(int i=0;i<NUMBER_OF_BUS; i++)   // poll every bus
  {
     sensors[i]->begin();
     numberOfDevices[i] = sensors[i]->getDeviceCount();
     sensors[i]->requestTemperatures();
      // print the device information
    for(int j=0;j<numberOfDevices[i]; j++)    // poll devices connect to the bus
    {
      sensors[i]->getAddress(tempDeviceAddress[j], j);
      printAddress(tempDeviceAddress[j]);      //print ID
      Serial.print(";"); 
      float tempC = sensors[i]->getTempC(tempDeviceAddress[j]);
      Serial.print(tempC);                    //print temperature
      Serial.print(";");
    }
  }
  Serial.println();  
  delay(POLL_TIME);
}

