#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define pirr 2
#define pirg 3
#define pirb 8

#define numLEDs 50
#define init_delay 20

//strip data pin
#define PIN 6

#define intensity_multiplier .85

int rs[numLEDs];
int gs[numLEDs];
int bs[numLEDs];

int delayer;

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
    
  strip.begin();
  Serial.begin(9600);
  
  pinMode(pirr, INPUT);
  pinMode(pirg, INPUT);
  pinMode(pirb, INPUT);  
  
  for(int i = 0; i<numLEDs; i++)
  {
    rs[i] = 0;
    gs[i] = 0;
    bs[i] = 0;
  }
  // Update LED contents, to start they are all 'off'
  strip.show();
}


void loop() {

  fract(Color(255,0,0),Color(0,0,255),500);
  for(int i=2;i<50;i=i*i)
  {
    theaterChaseRainbow(i);  
  }
  trip_motion();
  
}

void fract(uint32_t c1, uint32_t c2, int wait)
{
  fract_segments(c1,c2,25,wait);    
  fract_segments(c1,c2,12,wait);    
  fract_segments(c1,c2,6,wait);      
  fract_segments(c1,c2,3,wait);  
  fract_segments(c1,c2,1,wait);      
}

void fract_segments(uint32_t c1,uint32_t c2,int segment_size, int wait)
{
  for(int j=1;j<150;j++)
  {
    uint32_t c;
    int switcher_count = 0;
    for(int i=0;i<numLEDs;i++)
    {
      if(j%2)
      {
        strip.setPixelColor(i,c1);
      }else{
        strip.setPixelColor(i,c2);
      }
      switcher_count++;
      if(switcher_count == segment_size)
      {
        switcher_count = 0;
        c = c1;
        c1 = c2;
        c2 = c;
      }      
    }
    strip.show();
    delay(wait);  
    wait = wait * .95;        
  }
}


void trip_motion(){
  uint32_t loops = 0; 
  while(true){
    int r = digitalRead(pirr);
    int g = digitalRead(pirg);
    int b = digitalRead(pirb);

    for(int j = numLEDs-1; j>0; j--)
    {
      rs[j] = rs[j-1];
      gs[j] = gs[j-1];      
      bs[j] = bs[j-1];      
      strip.setPixelColor(j, Color(rs[j],gs[j],bs[j]));
    }
    
    delayer = init_delay;
        
    if(r)
    {
      Serial.print("Rs[0] is ");
      Serial.println(rs[0]);
      rs[0] = 255;
      //delayer = init_delay;
    }else{
      rs[0] = intensity_multiplier * rs[0];
    }
    
    if(g)
    {
      gs[0] = 255;
    }else{
      gs[0] = intensity_multiplier * gs[0];
    }
    
    if(b)
    {
      bs[0] = 255;   
    }else{
      bs[0] = intensity_multiplier * bs[0];
    }

    strip.setPixelColor(0, Color(rs[0], gs[0], bs[0]));
    Serial.print(rs[0]);
    Serial.print(", ");
    Serial.print(gs[0]);
    Serial.print(", ");
    Serial.println(bs[0]);
    
    strip.show();   // write all the pixels out  
    
    delay(delayer);
  }
}

// Create a 24 bit color value from R,G,B
uint32_t Color(byte r, byte g, byte b)
{
  uint32_t c;
  c = r;
  c <<= 8;
  c |= g;
  c <<= 8;
  c |= b;
  return c;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}
