// modified to use in M328W module and Arduino Yun Bridge Class
// info@dragino.com

#include <Console.h>
#include <RFM69.h>
#include <SPI.h>

#define NODEID      99
#define NETWORKID   100
#define GATEWAYID   1
#define FREQUENCY   RF69_868MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define KEY         "thisIsEncryptKey" //has to be same 16 characters/bytes on all nodes, not more not less!
#define LED         8 // Control the LED on MS14
#define SERIAL_BAUD 115200
#define ACK_TIME    100  // # of ms to wait for an ack

int TRANSMITPERIOD = 300; //transmit a packet to gateway so often (in ms)
byte sendSize=0;
boolean requestACK = false;
RFM69 radio;

typedef struct {		
  int           nodeId; //store this nodeId
  unsigned long uptime; //uptime in ms
  float         temp;   //temperature maybe?
} Payload;
Payload theData;

void setup() {
  Bridge.begin();
  Console.begin();
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  //radio.setHighPower(); //uncomment only for RFM69HW!
  radio.encrypt(KEY);
  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Console.println(buff);
}

long lastPeriod = -1;
void loop() {
  //process any serial input
  if (Console.available() > 0)
  {
    char input = Console.read();
    if (input >= 48 && input <= 57) //[0,9]
    {
      TRANSMITPERIOD = 100 * (input-48);
      if (TRANSMITPERIOD == 0) TRANSMITPERIOD = 1000;
      Console.print("\nChanging delay to ");
      Console.print(TRANSMITPERIOD);
      Console.println("ms\n");
    }
    
    if (input == 'r') //d=dump register values
      radio.readAllRegs();
    if (input == 'E') //E=enable encryption
      radio.encrypt(KEY);
    if (input == 'e') //e=disable encryption
      radio.encrypt(null);
  }

  //check for any received packets
  if (radio.receiveDone())
  {
    Console.print('[');Console.print(radio.SENDERID, DEC);Console.print("] ");
    for (byte i = 0; i < radio.DATALEN; i++)
      Console.print((char)radio.DATA[i]);
    Console.print("   [RX_RSSI:");Console.print(radio.readRSSI());Console.print("]");

    if (radio.ACKRequested())
    {
      radio.sendACK();
      Console.print(" - ACK sent");
      delay(10);
    }
    Blink(LED,5);
    Console.println();
  }
  
  int currPeriod = millis()/TRANSMITPERIOD;
  if (currPeriod != lastPeriod)
  {
    //fill in the struct with new values
    theData.nodeId = NODEID;
    theData.uptime = millis();
    theData.temp = 91.23; //it's hot!
    
    Console.print("Sending struct (");
    Console.print(sizeof(theData));
    Console.print(" bytes) ... ");
    if (radio.sendWithRetry(GATEWAYID, (const void*)(&theData), sizeof(theData)))
      Console.print(" ok!");
    else Console.print(" nothing...");
    Console.println();
    Blink(LED,3);
    lastPeriod=currPeriod;
  }
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}
