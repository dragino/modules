/*
 Cumulocity client demo.  
 
 This sketch do below task:
 1) Check if there is alarm for specify device in Cumulocity, if yes, display it in OLED display
 2) Press the touch button will send a command to Cumulocity to clear the alarm. 
 
 Below firmware/hardware are required to run this sketch:
 1) hardware MS14-S and M32
 2) firmware on MS14-S: Yun firmware ver RC1.2
 3) Grove OLED, touch sensor
  
 Created 22, Feb, 2014
 by Dragino Technology Co., Limited
 
 Reference Link: http://wiki.dragino.com/index.php?title=Cumulocity#Example_2:_Set_and_get_an_alarm
 
 */

#include <Process.h>
#include <SeeedOLED.h>
#include <Wire.h>

#define RESET_BUTTON 6

// set up net client info:
const unsigned long postingInterval = 60000;  //check alarm status every 1 minute
unsigned long lastRequest = 0;      // when you last made a request to check alarm
unsigned long lastPress = 0;      // when you last press the reset button
String AlarmID="";

boolean hasAlarm = false;


void setup() {
  
  // Bridge startup
  Bridge.begin();
  Console.begin();

  pinMode(RESET_BUTTON,INPUT);

  // OLED Startup
  Wire.begin();	
  SeeedOled.init();  //initialze SEEED OLED display
  DDRB|=0x21;         
  PORTB |= 0x21;

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();           //Set addressing mode to Page Mode
  SeeedOled.setTextXY(0,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Yeelink Demo"); //Print the String
}

void loop() {

  long now = millis();
  if ((now - lastPress >= 5000) && (hasAlarm == true)){
    if (digitalRead(RESET_BUTTON) == HIGH) {
      Console.println("reset alarm");
      resetAlarm();   // Reset overheated alarm
      lastPress = now;
    }    
  }
  
  if (now - lastRequest >= postingInterval) {
    isOverheated();  // check overheated alarm status
    showStatus();    // show alarm status on OLED
    lastRequest = now;
  }

}

//check if there is Overheated alarm on specify device
void isOverheated() {

  int PosStart = 0,PosEnd = 0;
  // Send this command to MS14
  // lua /usr/lib/lua/dragino/iot/cumulocity.lua -ga gid "Overheated"
  // change gid to the Global ID you want to monitor
  AlarmID = "";
  char str1[255];  
  Process cumulocity;
  Console.println("\n\nGet Alarm..... ");
  cumulocity.begin("lua");
  cumulocity.addParameter("/usr/lib/lua/dragino/iot/cumulocity.lua");
  cumulocity.addParameter("-ga");
  cumulocity.addParameter("14200"); // Change this number to the monitored Global ID 
  cumulocity.addParameter("Overheated");
  cumulocity.run();

  // Get return info
  int i=0,v;
  while (cumulocity.available() > 0) {
    str1[i] = cumulocity.read();
    if (str1[i] == 10 ) {
      if (PosEnd != 0) PosStart = PosEnd + 1;
      PosEnd = i;
    }
    Console.write(str1[i]);
    i++;
  }

  for(int k=PosStart;k<PosEnd;k++){
     AlarmID+=(str1[k]);
  }
  
  // There is Overheated alarm
  if (AlarmID.length() > 1) 
    hasAlarm = true;
  else
    hasAlarm = false;
}

void resetAlarm() {
  // Send this command to MS14
  // lua /usr/lib/lua/dragino/iot/cumulocity.lua -ca alarmID
  Process cumulocity;
  Console.println("\n\nReset Alarm..... ");
  cumulocity.begin("lua");
  cumulocity.addParameter("/usr/lib/lua/dragino/iot/cumulocity.lua");
  cumulocity.addParameter("-ca");
  cumulocity.addParameter(AlarmID); 
  cumulocity.run();
  
  while (cumulocity.available() > 0) {
    char c = cumulocity.read();
    Console.write(c);
  }
}


void showStatus() {
  SeeedOled.setTextXY(1,0);          //Set the cursor to Xth Page, Yth Column  
  SeeedOled.putString("Overheated?");     //Print the String

  SeeedOled.setTextXY(2,0);          //Set the cursor to Xth Page, Yth Column  
  if (hasAlarm == true) 
    SeeedOled.putString("True!!!");     // Alarm Detected.
  else 
    SeeedOled.putString("False");
}


