//This sketch is a PingPong test for the ms14+m32w+rfm12b (with Yun software)
//In this sketch, MS14 will keep sending data to 433Mhz network, 
//It will also receive data from 433Mhz wireless network sent by other devices, if the incoming data are in the same group, 
//it show be shown to the Arduino IDE serial monitor
//2014-02-21 Dragino Technology Co., Limited 

// Demo of a sketch which sends and receives packets
// 2010-05-17 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: pingPong.pde 7763 2011-12-11 01:28:16Z jcw $

// with thanks to Peter G for creating a test sketch and pointing out the issue
// see http://news.jeelabs.org/2010/05/20/a-subtle-rf12-detail/

#include <Console.h>
#include <RF12.h> // Protocol library for RFM12B, include in Jeelibs library
#include <Ports.h> // Library to use MilliTimer, include in Jeelibs library

MilliTimer sendTimer;

//Payload going to be sent in 433Mhz network
char payload[] = "Hello!";
byte needToSend;
int led=13;  

static void sendLed (byte on) {
    digitalWrite(led,on);
}

static void receiveLed (byte on) {
    digitalWrite(led,on); 
}

void setup () {
  
  //Initialize Console and wait for port to open:
    Bridge.begin();
    Console.begin(); 
    pinMode(led,OUTPUT);

//RFM12B works only after Arduino IDE use serial monitor to connect to MS14-S.
//Comment the while loop if want the RFM12B works after MS14-S boot.
  while (!Console) {
    ; // wait for Console port to connect.
  }
   Console.println("Send and Receive");

//Initiate RFM12B module 
    rf12_initialize(1, RF12_433MHZ, 33);
}

void loop () {
// See if anydata receive, it yes, print in the console
  if (rf12_recvDone() && rf12_crc == 0) {
        receiveLed(1);
        Console.print("OK,We Received:   ");
        for (byte i = 0; i < rf12_len; ++i)
            Console.write(rf12_data[i]);
        Console.println();
        delay(100); // otherwise led blinking isn't visible
        receiveLed(0);
    }
    
    if (sendTimer.poll(3000))
        needToSend = 1;

// Send data to 433Mhz wireless network.         
    if (needToSend && rf12_canSend()) {
        needToSend = 0;
        
        sendLed(1);
        rf12_sendStart(0, payload, sizeof payload);
        delay(100); // otherwise led blinking isn't visible
        sendLed(0);
    }
}
