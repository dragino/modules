// modified to use in M328W module and Arduino Yun Bridge Class
// info@dragino.co
#include <Console.h>
#include <RFM69.h>
#include <SPI.h>

#define NODEID      1
#define NETWORKID   100
#define FREQUENCY   RF69_868MHZ //Match this with the version of your Moteino! (others: RF69_433MHZ, RF69_868MHZ)
#define KEY         "thisIsEncryptKey" //has to be same 16 characters/bytes on all nodes, not more not less!
#define LED         8 // Control the LED of MS14
#define ACK_TIME    100  // # of ms to wait for an ack

RFM69 radio;
bool promiscuousMode = false; //set to 'true' to sniff all packets on the same network

typedef struct {		
  int           nodeId; //store this nodeId
  unsigned long uptime; //uptime in ms
  float         temp;   //temperature maybe?
} Payload;
Payload theData;

void setup() {
  Bridge.begin();
  Console.begin();
  delay(10);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  //radio.setHighPower(); //uncomment only for RFM69HW!
  radio.encrypt(KEY);
  radio.promiscuous(promiscuousMode);
  char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Console.println(buff);
}

byte ackCount=0;
void loop() {
  //process any serial input
  if (Console.available() > 0)
  {
    char input = Console.read();
    if (input == 'r') //d=dump all register values
      radio.readAllRegs();
    if (input == 'E') //E=enable encryption
      radio.encrypt(KEY);
    if (input == 'e') //e=disable encryption
      radio.encrypt(null);
    if (input == 'p')
    {
      promiscuousMode = !promiscuousMode;
      radio.promiscuous(promiscuousMode);
      Console.print("Promiscuous mode ");Console.println(promiscuousMode ? "on" : "off");
    }
   
  }

  if (radio.receiveDone())
  {
    Console.print('[');Console.print(radio.SENDERID, DEC);Console.print("] ");
    Console.print(" [RX_RSSI:");Console.print(radio.readRSSI());Console.print("]");
    if (promiscuousMode)
	{
      Console.print("to [");Console.print(radio.TARGETID, DEC);Console.print("] ");
    }
	
    if (radio.DATALEN != sizeof(Payload))
      Console.print("Invalid payload received, not matching Payload struct!");
    else
    {
      theData = *(Payload*)radio.DATA; //assume radio.DATA actually contains our struct and not something else
      Console.print(" nodeId=");
      Console.print(theData.nodeId);
      Console.print(" uptime=");
      Console.print(theData.uptime);
      Console.print(" temp=");
      Console.print(theData.temp);
    }
    
    if (radio.ACKRequested())
    {
      byte theNodeID = radio.SENDERID;
      radio.sendACK();
      Console.print(" - ACK sent.");

      // When a node requests an ACK, respond to the ACK
      // and also send a packet requesting an ACK (every 3rd one only)
      // This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
      if (ackCount++%3==0)
      {
        Console.print(" Pinging node ");
        Console.print(theNodeID);
        Console.print(" - ACK...");
        delay(3); //need this when sending right after reception .. ?
        if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 0))  // 0 = only 1 attempt, no retries
          Console.print("ok!");
        else Console.print("nothing");
      }
    }
    Console.println();
    Blink(LED,3);
  }
}

void Blink(byte PIN, int DELAY_MS)
{
  pinMode(PIN, OUTPUT);
  digitalWrite(PIN,HIGH);
  delay(DELAY_MS);
  digitalWrite(PIN,LOW);
}
