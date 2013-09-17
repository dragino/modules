//This sketech is a modified version from jeelabs pingPong.pde, original code can be found at https://github.com/jcw/ !!
//<edwin@dragino.com> 

// Demo of a sketch which sends and receives packets
// 2010-05-17 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: pingPong.pde 7763 2011-12-11 01:28:16Z jcw $

// with thanks to Peter G for creating a test sketch and pointing out the issue
// see http://news.jeelabs.org/2010/05/20/a-subtle-rf12-detail/

// Use Dragino RF12 lib for Dragino's module
#include <DR_RF12.h>
#include <Ports.h>

MilliTimer sendTimer;
char payload[] = "Hello!";
byte needToSend;
int led=13;

static void sendLed (byte on) {
    digitalWrite(led,on);
}

static void receiveLed (byte on) {
    digitalWrite(led,!on); // inverse, because LED is tied to VCC
}

void setup () {
    delay(8000);  // Wait for 8s until dragino enter autoboot 
    Serial.begin(9600);
    Serial.println(9600);
    Serial.println("Send and Receive");
    pinMode(led,OUTPUT);
    rf12_initialize(1, RF12_433MHZ, 33);
}

void loop () {
    if (rf12_recvDone() && rf12_crc == 0) {
        receiveLed(1);
        Serial.print("OK,We Received:   ");
        for (byte i = 0; i < rf12_len; ++i)
            Serial.write(rf12_data[i]);
        Serial.println();
        delay(100); // otherwise led blinking isn't visible
        receiveLed(0);
    }
    
    if (sendTimer.poll(3000))
        needToSend = 1;

    if (needToSend && rf12_canSend()) {
        needToSend = 0;
        
        sendLed(1);
        rf12_sendStart(0, payload, sizeof payload);
        delay(100); // otherwise led blinking isn't visible
        sendLed(0);
    }
}
