/*
 Example: get value from light sensor and sound sensor and send to ms14 to construct a report.
 Ths ms14 side will archive the report and send as a daily report via mail.  
 
 */

#include <Console.h>
#include <FileIO.h>
#include <math.h>
#define LIGHT_SENSOR A3
#define SOUND_SENSOR A2

float RLightsensor; //Resistance of sensor in K

void setup() {
  // Initialize Console and file IO. 
  Bridge.begin();
  Console.begin(); 
  FileSystem.begin();
} 

void loop() {
  long now = millis();
  String dataString;
 
  //Sampling the sensor value. Sample the value every minisecond and 
  //get the average value on 10,000 sampling. to get a average sound strength
  long sensorSoundValue = 0;
  for(int i=0; i< 10000; i++)
  {
    sensorSoundValue += analogRead(SOUND_SENSOR);
    delay(1);
  }
  sensorSoundValue = sensorSoundValue/10000;

  //Get Light sensor value
  int sensorLightValue = analogRead(LIGHT_SENSOR);
  RLightsensor=(float)(1023-sensorLightValue)*10/sensorLightValue;
  
  //Construct the datastring;  
  dataString += getTimeStamp();
  dataString += " , ";
    
  dataString += String(RLightsensor);
  dataString += " , ";
    
  dataString += String(sensorSoundValue);
    
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = FileSystem.open("/var/test.csv", FILE_APPEND);
  
  // if the file is available, write the datastring to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    Console.println(dataString);
  } 
}

// This function return a string with the time stamp
// MS14 will call the Linux "data" command and get the time stamp
String getTimeStamp() {
  String result;
  Process time;
  // date is a command line utility to get the date and the time 
  // in different formats depending on the additional parameter 
  time.begin("date");
  time.addParameter("+%D-%T");  // parameters: D for the complete date mm/dd/yy
                                //             T for the time hh:mm:ss    
  time.run();  // run the command

  // read the output of the command
  while(time.available()>0) {
    char c = time.read();
    if(c != '\n')
      result += c;
  }
  
  return result;
}


