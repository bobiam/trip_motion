/*
Trip motion is an art project by Bob Eells for Flipside 2015, mostly 3d printed, with a strand of WS2811 LEDs
It borrows heavily from the Adafruit_Neopixel demo code and uses their library.

LICENSE - 
This code makes lights blink, and should not be used to run heart monitors or keep planes in the air.
If you actually need a real software license for this, I'm curious what you're doing and will happily 
release it under the very open license of your choosing if you reach out and ask for it.  
You can pretty much assume MIT, GPL, CC style terms.  
I'm not gonna sue you over blinky light code I wrote for fun.
*/

#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define pirr 2
#define pirg 3
#define pirb 8

#define numLEDs 54

//strip data pin
#define PIN 6

#define intensity_multiplier .85

int rs[numLEDs];
int gs[numLEDs];
int bs[numLEDs];

//frequently used named colors
uint32_t orangered = Color(255,69,0);
uint32_t orange = Color(255,165,0);
uint32_t black = Color(0,0,0);
uint32_t red = Color(255,0,0);
uint32_t green = Color(0,255,0);
uint32_t blue = Color(0,0,255);
uint32_t yellow = Color(255,255,0);
uint32_t purple = Color(255,0,255);
uint32_t seagreen = Color(84,255,159);
uint32_t indigo = Color(75,0,130);
uint32_t violet = Color(238,130,238);
uint32_t white = Color(255,255,255);
uint32_t darkgreen = Color(0,255,0);
uint32_t forestgreen = Color(34,255,34);
uint32_t darkslategray = Color(47,255,79);
uint32_t darkolivegreen = Color(85,255,47);
uint32_t shipcove = Color(115,138,189);
uint32_t poloblue = Color(118,149,200);
uint32_t deepblue = Color(0,11,76);
//doyoubelieveinmagic?
uint32_t dybim = white+1;

bool debug = false;

// Set the first variable to the NUMBER of pixels. 25 = 25 pixels in a row
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, PIN, NEO_RGB + NEO_KHZ800);

void setup() {
    
  strip.begin();
  Serial.begin(9600);
  pinMode(pirr, INPUT);
  pinMode(pirg, INPUT);
  pinMode(pirb, INPUT);  
  
  //initialize trip_motion array
  for(int i = 0; i<numLEDs; i++)
  {
    rs[i] = 0;
    gs[i] = 0;
    bs[i] = 0;
  }
  // Update LED contents, to start they are all 'off'
  strip.show();
  if(!debug)
  {
    all(white);
    delay(2000);
  }  
}

void bob_debug() {
  
}

void loop() {
  if(debug)
  {
    bob_debug();
  }else{
    int polled = poll();
    if(polled == 999 && !debug)
    {
      trip_motion();
    }else{
      no_one_close();
    }
  }
}

int no_one_close()
{
  int polled;
  
  polled = fades(1,10);
  if(polled == 999)
    return 999;
  
  polled =  cross_the_streams(black,blue,red,10,10);
  if(polled == 999)
    return 999;
    
  polled = bouncy(black,dybim,10,30);
  if(polled == 999)
    return 999;

  polled = earthRand(200);
  if(polled == 999)
    return 999;

  polled = airRand(150);
  if(polled == 999)
    return 999;

  polled = fireRand(100);
  if(polled == 999)
    return 999;
    
  polled = waterRand(50);
  if(polled == 999)
    return 999;
    
  polled = nchase(red,green,6,50);
  if(polled == 999)
    return 999;

  polled = nchase(green,blue,6,25);
  if(polled == 999)
    return 999;

  polled = nchase(blue,red,6,12);
  if(polled == 999)
    return 999;  

  polled = bouncy(black,blue,10,50);
  if(polled == 999)
    return 999;

  polled = rainbow(20);
  if(polled == 999)
    return 999;

  polled = rainbowCycle(20);
  if(polled == 999)
    return 999;

  polled = crossfader(white,black,20,true);
  if(polled == 999)
    return 999;

  polled = crossfader(orangered,deepblue,20,false);
  if(polled == 999)
    return 999;

  polled = fract(red,green,500);  
  if(polled == 999)
    return 999;  
        
  polled = omgp();
  if(polled == 999)
    return 999;
    
  polled = ants(blue, yellow,250,500);  
  if(polled == 999)
    return 999;
    
  polled = accel_chase(darkgreen,black,30);  
  if(polled == 999)
    return 999;  
    
  polled = randommy();
  if(polled == 999)
    return 999;   

  polled = chase(blue,red,20);
  if(polled == 999)
    return 999;  
    
  polled = theaterChaseRainbow(50);  
  if(polled == 999)
    return 999;  
    
  return 1;
}

int fades(int wait, int loops)
{
  for(int j=0;j<loops;j++)
  {
    for(int i=0;i<255;i++)
    {
        all(Color(i,0,0));
        delay(wait);
        if(poll() == 999)
          return 999;
    }
    for(int i=255;i>0;i--)
    {
        all(Color(i,0,0));
        delay(wait);
        if(poll() == 999)
          return 999;
    }    
    for(int i=0;i<255;i++)
    {
        all(Color(0,0,i));
        delay(wait);
        if(poll() == 999)
          return 999;
    }
    for(int i=255;i>0;i--)
    {
        all(Color(0,0,i));
        delay(wait);
        if(poll() == 999)
          return 999;
    }    
    for(int i=0;i<255;i++)
    {
        all(Color(0,i,0));
        delay(wait);
        if(poll() == 999)
          return 999;
    }
    for(int i=255;i>0;i--)
    {
        all(Color(0,i,0));
        delay(wait);
        if(poll() == 999)
          return 999;
    }        
  }
  return 1;
}

//BE - earth color palette
int airRand(int loops)
{
  uint32_t colors[] = {shipcove,white,deepblue,poloblue};
  int maxWait = 150;
  int poll = paletteRand(colors,sizeof(colors)/sizeof(uint32_t),maxWait,loops);
  if(poll == 999)
    return 999;    
  return 1;
}

//BE - earth color palette
int earthRand(int loops)
{
  uint32_t colors[] = {darkolivegreen,darkslategray,green,black};
  int maxWait = 150;
  int poll = paletteRand(colors,sizeof(colors)/sizeof(uint32_t),maxWait,loops);
  if(poll == 999)
    return 999;  
  return 1;
}

//BE - fire color palette
int fireRand(int loops)
{
  uint32_t colors[] = {black,red,orangered,yellow};
  int maxWait = 150;
  int poll = paletteRand(colors,sizeof(colors)/sizeof(uint32_t),maxWait,loops);
  if(poll == 999)
    return 999;  
  return 1;
}

//BE - water color palette
int waterRand(int loops)
{
  uint32_t colors[] = {black,blue,seagreen};
  int maxWait = 150;
  int poll = paletteRand(colors,sizeof(colors)/sizeof(uint32_t),maxWait,loops);
  if(poll == 999)
    return 999;  
  return 1;
}

int paletteRand(uint32_t colors[], int colorCount,int maxWait, int loops)
{
  for(int j = 0; j < loops; j++)
  {
    for(int i =0; i < strip.numPixels();i++)
    {
      strip.setPixelColor(i,colors[random(0,colorCount)]);
      if(poll() == 999)
        return 999;
    }
    strip.show();
    delay(maxWait);    
  }
  return 1;
}

//cross the streams
//sets ring to bc, runs a pod from each end (fgc1, fgc2), they overwrite each other as they cross.
//duration = loops * wait * numLEDs
int cross_the_streams(uint32_t bc, uint32_t fgc1, uint32_t fgc2, int wait, int loops)
{
  all(bc);
  for(int j=0;j<loops;j++)
  {
    for(int i=0;i<numLEDs+1;i++)
    {
      strip.setPixelColor(i,fgc1);
      strip.setPixelColor(numLEDs-i,fgc2);
      strip.show();
      delay(wait);
      if(poll() == 999)
        return 999;        
    }
  }
  return 1;
}

//bouncy
//sets ring to c1, then takes c2 off the two ends at a decelerating rate
//duration = init_wait * 
int bouncy(uint32_t c1, uint32_t c2, int init_wait, int loops)
{
  int wait;
  bool dybim_flag;
  int strip_divisor = 2;
  dybim_flag = false;
  for(int i=0;i<loops;i++)
  {
    if(c2 == dybim || dybim_flag )
    {
      dybim_flag = true;
      c2 = Wheel(i);
    }
    all(c2);
    wait = init_wait;
    for(int j=0;j<numLEDs/strip_divisor;j++)
    {
      strip.setPixelColor(j,c1);
      strip.setPixelColor(numLEDs-1-j,c1);
      strip.show();
      wait = 1.05 * wait;
      Serial.println(wait);
      delay(wait);
      if(poll() == 999)
        return 999;        
    }
    for(int j=numLEDs/strip_divisor;j>-1;j--)
    {
      strip.setPixelColor(j,c2);
      strip.setPixelColor(numLEDs-1-j,c2);
      strip.show();
      delay(wait);
      if(poll() == 999)
        return 999;        
      wait = .1 * wait;
    }    
  }
  return 1;
}

int crossfader(uint32_t c1, uint32_t c2, int wait, bool accel)
{
  byte colors1[3];
  byte colors2[3];
  int pixel;
  decodeColor(c1,colors1);
  decodeColor(c2,colors2);
  float delta_r, delta_g, delta_b, red, green, blue;

  delta_r = colors2[0] - colors1[0];
  delta_g = colors2[1] - colors1[1];
  delta_b = colors2[2] - colors1[2];
  
  delta_r = delta_r / numLEDs;
  delta_g = delta_g / numLEDs;
  delta_b = delta_b / numLEDs;
  for(int j =0;j<1000;j++)
  {
    for(int i=0;i<numLEDs;i++)
    {
      red = colors1[0]+(i*delta_r);
      green = colors1[1]+(i*delta_g);
      blue = colors1[2]+(i*delta_b);
      pixel = (i + j) % numLEDs;
      strip.setPixelColor(pixel,Color(red,green,blue));
    }
    strip.show();    
    delay(wait);
    if(accel)
      wait = wait * 0.99;
    if(poll() == 999)
      return 999;  
  } 
  return 1;
}

int nchase(uint32_t bg,uint32_t fg,int n,int wait)
{
  int p;
  all(bg);
  for(int j=0;j<(numLEDs/n);j++)
  {
    for(int i=0;i<numLEDs/n;i++)
    {
      p = (n*i)+j;
      if(p>numLEDs)
        p = p-numLEDs;
      strip.setPixelColor(p,fg);
      strip.show();
      delay(wait);
      if(poll()==999)
        return 999;
    }
  }
  return 1;
}

int randommy()
{
  for(int i=0;i<random(150,500);i++)
  {
    strip.setPixelColor(random(0,strip.numPixels()),randomColor());
    delay(random(0,200));  
    strip.show();
    if(poll()==999)
      return 999;    
  }
  return 1;
}

int omgp()
{
  int i,j,k;
  int pos; //cheap/easy rollover

  for(k=0;k<numLEDs;k++)
  {
    for(i=0;i<(numLEDs/3);i++)
    {
      for(j=0;j<3;j++)
      {
        pos = i * 3 * k;
        while(pos > 255)
        {
          pos = pos - 255;
        }
        strip.setPixelColor(i*3+j,Wheel(pos));
      }
    }
    strip.show();
    if(poll()==999)
      return 999;    
    delay(500);
  }
  return 1;
} 

int chase(uint32_t fg, uint32_t bg, int wait)
{
  int i,j;
  for(j=0;j<100;j++)
  {
    all(bg);
    for(i=0;i<numLEDs;i++)
    {
      strip.setPixelColor(i,fg);
      strip.setPixelColor(i-1,bg);
      strip.show();
      delay(wait);        
      if(poll()==999)
        return 999;      
    }
    for(i=numLEDs;i>-1;i--)
    {
      all(bg);
      strip.setPixelColor(i,fg);
      strip.show();
      delay(wait);  
      if(poll()==999)
        return 999;      
    }
  }    
  return 1;
}

int accel_chase(uint32_t fg, uint32_t bg, int init_wait)
{
  int i,j, wait;
  wait = init_wait;
  for(j=0;j<22;j++)
  {
    all(bg);
    for(i=0;i<numLEDs;i++)
    {
      strip.setPixelColor(i,fg);
      strip.setPixelColor(i-1,bg);
      strip.show();
      delay(wait);        
      if(poll()==999)
        return 999;      
    }
    for(i=numLEDs;i>-1;i--)
    {
      strip.setPixelColor(i,fg);
      strip.setPixelColor(i+1,bg);
      strip.show();
      delay(wait);        
      if(poll()==999)
        return 999;      
    }
    wait = .90 * wait;
  }    
  return 1;
}

//does random stuff.  
int rand(int loops, int max_wait, int modus)
{
  int p;
  uint32_t c;
  for(int i=0;i<loops;i++)
  {
    p = random(0,numLEDs);
    switch(modus){
      case 0:
        c = Color(random(0,255),random(0,255),random(0,255));
        break;
      case 1:
        c = Color(random(0,255),0,0);
        break;
      case 2:
        c = Color(0,random(0,255),0);
        break;
      case 3:
        c = Color(0,0,random(0,255));        
        break;
    }
    strip.setPixelColor(p,c);
    strip.show();
    delay(random(0,max_wait));
    if(poll()==999)
      return 999;    
  }
  return 1;
}

int fract(uint32_t c1, uint32_t c2, int wait)
{
  if(fract_segments(c1,c2,25,wait) == 999)
    return 999;  
  if(fract_segments(c1,c2,12,wait) == 999)
    return 999;  
  if(fract_segments(c1,c2,6,wait) == 999)
    return 999;  
  if(fract_segments(c1,c2,3,wait) == 999)
    return 999;  
  if(fract_segments(c1,c2,1,wait) == 999)
    return 999;  
  return 1;
}

int fract_segments(uint32_t c1,uint32_t c2,int segment_size, int wait)
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
    if(poll()==999)
      return 999;    
  }
  return 1;
}


void trip_motion(){
  int t_loops = 0; 
  int delayer;
  delayer = 25;

  //we want this to play with people as long as they're around, at 25ms, it takes 40 loops to hit a second.  If we've been dark 60 seconds, kick back out to the patterns.
  while(t_loops<600){
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

    if(r || g || b)
    {
      //reset t_loops, something moved.
      t_loops = 0;
    }
    
    if(r)
    {
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
    strip.show();   // write all the pixels out  
    delay(delayer);
    t_loops++;
  }
}


//BE - alternate pods between two colors
int ants(uint32_t c1, uint32_t c2, uint8_t wait, int loops){
  int i,j;
  for(j=0;j<loops;j++)
  {
    for(i=0; i< strip.numPixels(); i++)
    {
      if(j%2)
      {     
        if(i % 2)
        {
          strip.setPixelColor(i,c1);
        }else{
          strip.setPixelColor(i,c2);
        }
      }else{
        if((i+1) % 2)
        {
          strip.setPixelColor(i,c1);
        }else{
          strip.setPixelColor(i,c2);
        }
      }
    }
    strip.show();
    delay(wait);
    if(poll() == 999)
      return 999;      
  }
  return 1;
}

int rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 256; j++) {     // 3 cycles of all 256 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 255));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
    if(poll() == 999)
      return 999;  
  }
  return 1;
}
// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
int rainbowCycle(uint8_t wait) {
  int i, j;
  
  for (j=0; j < 256 * 5; j++) {     // 5 cycles of all 25 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 96-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 96 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 256 / strip.numPixels()) + j) % 256) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
    if(poll() == 999)
      return 999;      
  }
  return 1;  
}

// BE
// fill the dots one after the other with said color
// good for testing purposes and color wipes inbetween patterns.
int colorWipe(uint32_t c, uint8_t wait) {
  int i;
  
  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
      if(poll() == 999)
        return 999;        
  }
  return 1;
}




/* Helper functions */

//BE - sometimes it's useful to get the RGB values back when passing colors to and fro.
void decodeColor(uint32_t c, byte ret[])
{
  int r,g,b;
  
  b = c & 255;
  g = c >>= 8;
  r = c >> 8;
  
  ret[0] = r;
  ret[1] = g;
  ret[2] = b;  
}

//BE
uint32_t randomColor()
{
  //generate a random color
  int r = random(0,256);
  int g = random(0,256);
  int b = random(0,256);
  return Color(r,g,b);
}

//BE 
void array_spin(byte arr[], int count)
{
  byte tmp = arr[0];
  for(int i=0;i<count-1;i++)
  {
    arr[i] = arr[i+1];
  }
  arr[count-1] = tmp;
}

void all(uint32_t c)
{
  for(int i=0;i<numLEDs;i++)
  {
    strip.setPixelColor(i,c);
  }
  strip.show();  
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
int theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < numLEDs; i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < numLEDs; i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }  
  return 1;
}

//poll the motion sensors, kick out a 999 if one trips, which kicks us to triple motion (trip_motion())
int poll()
{
  int r = digitalRead(pirr);
  int g = digitalRead(pirg);
  int b = digitalRead(pirb);
  if(r || g || b)
  {
    return 998;
  }
  return 1;
}
