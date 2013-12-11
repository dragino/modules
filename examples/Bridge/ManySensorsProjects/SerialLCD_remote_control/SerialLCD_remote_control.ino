/*
  Arduino Yun Bridge example
 
 This example for the Arduino Yun shows how to use the 
 Bridge library to receive text through REST calls /url and 
 show the text in a SerialLCD 
 you can create your own API when using REST style 
 calls through the browser.
 
 use below commands to send string via REST calls

 * "/arduino/YOUR_TEXT"     
 
 */

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>

// include the library code:
#include <SerialLCD.h>
#include <SoftwareSerial.h>

// initialize the LCD library
SerialLCD slcd(9,10);//this is a must, assign soft serial pins

// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;

void setup() {
  // Bridge startup

  pinMode(13,OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();

// Start LCD Display  
  slcd.begin();
  // Print a message to the LCD.
  slcd.print("Wait for text....");
  
}

void loop() {
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
  // read the text from REST calls
  String text = client.readStringUntil('\r');
  
  //Convert the string to char array
  char textbuf[text.length()+1];
  text.toCharArray(textbuf,text.length()+1);
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  slcd.setCursor(0, 1);
  // show the text in LCD. 
  slcd.print(textbuf);

}



