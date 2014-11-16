// Simple RFM12B sender program, with ACK and optional encryption
// It initializes the RFM12B radio with optional encryption and passes through any valid messages to the serial port
// felix@lowpowerlab.com
//
// modified to use in M328W module and Arduino Yun Bridge Class
// info@dragino.com

#include <Console.h>
#include <RFM12B.h>
#include <avr/sleep.h>

// You will need to initialize the radio by telling it what ID it has and what network it's on
// The NodeID takes values from 1-127, 0 is reserved for sending broadcast messages (send to all nodes)
// The Network ID takes values from 0-255
// By default the SPI-SS line used is D10 on Atmega328. You can change it by calling .SetCS(pin) where pin can be {8,9,10}
#define NODEID        2  //network ID used for this unit
#define NETWORKID    99  //the network ID we are on
#define GATEWAYID     1  //the node ID we're sending to
#define ACK_TIME     400  // # of ms to wait for an ack
#define SERIAL_BAUD  115200

//encryption is OPTIONAL
//to enable encryption you will need to:
// - provide a 16-byte encryption KEY (same on all nodes that talk encrypted)
// - to call .Encrypt(KEY) to start encrypting
// - to stop encrypting call .Encrypt(NULL)
uint8_t KEY[] = "ABCDABCDABCDABCD";

int interPacketDelay = 1000; //wait this many ms between sending packets
char input = 0;

// Need an instance of the Radio Module
RFM12B radio;
byte sendSize=0;
char payload[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*(){}[]`|<>?+=:;,.";
bool requestACK=false;

void setup()
{
  Bridge.begin();
  Console.begin();
  radio.Initialize(NODEID, RF12_433MHZ, NETWORKID);
  radio.Encrypt(KEY);
  radio.Sleep(); //sleep right away to save power
  Console.println("Transmitting...\n\n");
}

void loop()
{
  //serial input of [0-9] will change the transmit delay between 100-1000ms
  if (Console.available() > 0) {
    input = Console.read();
    if (input >= 48 && input <= 57) //[1..9] = {100..900}ms; [0]=1000ms
    {
      interPacketDelay = 100 * (input-48);
      if (interPacketDelay == 0) interPacketDelay = 1000;
      Console.print("\nChanging delay to ");
      Console.print(interPacketDelay);
      Console.println("ms\n");
    }
  }

  Console.print("Sending[");
  Console.print(sendSize+1);
  Console.print("]:");
  for(byte i = 0; i < sendSize+1; i++)
    Console.print((char)payload[i]);
  
  requestACK = !(sendSize % 3); //request ACK every 3rd xmission
  
  radio.Wakeup();
  radio.Send(GATEWAYID, payload, sendSize+1, requestACK);
  if (requestACK)
  {
    Console.print(" - waiting for ACK...");
    if (waitForAck()) Console.print("ok!");
    else Console.print("nothing...");
  }
  radio.Sleep();
  
  sendSize = (sendSize + 1) % 88;
  Console.println();
  delay(interPacketDelay);
}

// wait a few milliseconds for proper ACK, return true if received
static bool waitForAck() {
  long now = millis();
  while (millis() - now <= ACK_TIME)
    if (radio.ACKReceived(GATEWAYID))
      return true;
  return false;
}
