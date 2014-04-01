/*
  EMG Detector Demo

 A EMG detector example to change the LED matrix according to EMG value. 
 It also get temperature value and button state from another dragino.
 The LED color will change as the temperature change. 
 While press the button, the LED matrix will show Rainbow color. 

 The circuit:  
 1) MS14S + M32
 2) NeoPixel 8x8. with DIN connected to MS14-S D4 (pin 3+)
 3) EMG Detector with VOUT connected to MS14-S A0 (pin 4+)
 4) The other MS14 has the ip 192.168.240.1 and running the approxiate sketch. 
 

 created 2014
 by Dragino Technology Co., Limited
 
 Reference Link of this example:

 */
#include <Adafruit_NeoPixel.h>
#include <Bridge.h>
#include <Console.h>

#define PIN 4

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(64, PIN, NEO_GRB + NEO_KHZ800);
int max_analog_dta      = 300;              // max analog data
int min_analog_dta      = 100;              // min analog data
int static_analog_dta   = 0;                // static analog data

//Check the button status and temeprature from another MS14-S on every 5 seconds. 
const unsigned long pollingInterval = 5000;
int btn_status=0;
float temperature;
unsigned long lastPoll = 0;      // when you last poll

// get analog value from EMG detector
int getAnalog(int pin)
{
    long sum = 0;
    
    for(int i=0; i<32; i++)
    {
        sum += analogRead(pin);
    }
    
    int dta = sum>>5;
    
    max_analog_dta = dta>max_analog_dta ? dta : max_analog_dta;         // if max data
    min_analog_dta = min_analog_dta>dta ? dta : min_analog_dta;         // if min data
    
    return sum>>5;
}

void setup() {
  //Initialize Console and wait for port to open:
  Bridge.begin();  
  Console.begin();
  strip.begin();
  strip.show();
  
  //Initia static_analog_date from EMG dector
  long sum = 0;
  for(int i=0; i<=8; i++)
  {
    for(int j=0; j<100; j++)
    {
      sum += getAnalog(A0);
      delay(1);
    }
  }
    sum /= 900; 
    static_analog_dta = sum;
}

int level       = 16;
int level_buf   = 16;
int color       = 127;

void loop() {
  int val = getAnalog(A0);
  int level2;
    
  if(val>static_analog_dta)                   // larger than static_analog_dta
  {
    level2 = 16 + map(val, static_analog_dta, max_analog_dta, 0, 16);
  }
  else
  {
    level2 = 16 - map(val, min_analog_dta, static_analog_dta, 0, 16);
  }
  
    // to smooth the change of led bar
   if(level2 > level)
   {
        level++;
   }
   else if(level2 < level)
   {
        level--;
   }  
  
    if(level != level_buf)
    {
        level_buf = level;
        showPixels(Wheel(color),level*2); // Show status on NeoPixel
    }
  delay(10);

  // Get Temperature and btn  
  long now = millis();  
  if (now - lastPoll >= pollingInterval) {
    updateColor();
    lastPoll = now;
  }  
  if(btn_status == '1') rainbow(40);  
}

//Get temperature and 
void updateColor(){
      // Get Temperature and btn_status.
    char tem[5]; 
    int i=0; 
    Process getStatus;
    getStatus.begin("curl");
    // ip address of second MS14 which monitor the temperature and button state
    getStatus.addParameter("http://192.168.240.1/arduino/get/0");
    getStatus.run();
    while (getStatus.available() > 0) {
      char c = getStatus.read();
      if( i<5 ) tem[i] = c; 
      if( i == 6) btn_status = c;
      i++;
      Console.print(c);
    }
    Console.println("");
    int temperature = (int)(atof(tem)*10);
    //map the temperature into color. temperature range from 15 degree to 40 degree
    color = map(temperature,150,400,0,255);
}

//turn off all LEDs in counts. 
void showPixels(uint32_t c,uint8_t count) {
    //Console.println(count);
    for(uint16_t i=0; i<(strip.numPixels()-count); i++) {
      strip.setPixelColor(i, 0);
    } 
    for(uint16_t i=(strip.numPixels()-count); i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
    } 
    strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

