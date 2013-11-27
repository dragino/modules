// This example is to show how to use switch to control a remote relay in another 
// ms14 via REST calls over internet

#include <Console.h>
#include <Process.h>

//define remote ms14 info
#define IPADDR        "10.130.1.216"   // replace another ms14 ip here
#define USER        "root"                   // replace another ms14 username for log in
#define PASSWORD        "arduino"                   // replace another ms14 password for log in
#define RELAYPIN        "4"                   // what pin does the remote relay connect to

const int switchPin = 6;     // the number of the switch pin

// variables will change:
int switchState;         // variable for reading the switch status
int switchStateOld;      //previous switch status

void setup()
{
  // initialize the switch pin as an input:
    pinMode(switchPin, INPUT);  
    
    Bridge.begin();
    Console.begin(); 

  while (!Console) {
    ; // wait for Console port to connect.
  }
  switchStateOld = switchState;
}

// this method will notify the other ms14 to set the relay
void switchOnChange(int state) {
  // form the logon string:
  String login = USER;
  login += ":";
  login += PASSWORD;
  
  // form the control url 
  String url = "http://";
  url += IPADDR;
  url += "/arduino/digital/";
  url += RELAYPIN;
  url += "/";
  url += state;      //update relay state
  
  // Send the HTTP request
  
  // Call a command in ms14.
  // Is better to declare the Process here, so when the
  // switchOnChange function finishes the resources are immediately
  // released. Declaring it global works too, BTW.
  Process changeRelay;
  Console.print("change relay state ");
  changeRelay.begin("curl");
  changeRelay.addParameter("--user");
  changeRelay.addParameter(login); 
  changeRelay.addParameter(url);
  changeRelay.run();
  Console.println("done!\n\n");

  // If there's incoming data from the net connection,
  // send it out the Serial:
  while (changeRelay.available()>0) {
    char c = changeRelay.read();
    Console.write(c);
  }

}

void loop()
{
  switchState = digitalRead(switchPin);
  
  if (switchState != switchStateOld)
  {
    switchStateOld = switchState;
    switchOnChange(switchState);
  }

}
