/*
  Simulate UART TX Data 
 
 This sketch simulate Temperature and Humidity data to UART. 
 
 To test the pass through feature for different IoT service 
 
 created 25 Apr 2014
 by Dragino Technology Co., Limited
 
 Reference:
 http://wiki.dragino.com/index.php?title=Xively#Upload_data_to_Xively_use_Pass_Through_Mode
 
 */
String dataString = "";

void setup() {
  Serial1.begin(115200);
}

void loop() {
  dataString = "temp:";
  dataString += random(10) + 20;
  Serial1.println(dataString);  // upload Temperature data
  delay(20000);
  dataString = "humidity:";
  dataString += random(5) + 70;  // upload humidity data
  Serial1.println(dataString);  
  delay(20000);
}





