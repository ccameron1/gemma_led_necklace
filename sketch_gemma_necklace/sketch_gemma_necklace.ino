// Low power NeoPixel earrings example.  Makes a nice blinky display
// with just a few LEDs on at any time...uses MUCH less juice than
// rainbow display!

#include <Adafruit_NeoPixel.h>

#define PIN       0
#define NUM_LEDS 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN);

uint8_t  mode   = 0,        // Current animation effect
         offset = 0;        // Position of spinner animation
uint32_t color  = 0xFF8866; // Starting color = white
uint32_t prevTime;          // Time of last animation mode switch
uint32_t wait = 50;
int dir = 1;
int bright = 50;


void setup() {
  pixels.begin();
  pixels.setBrightness(10); // ~1/3 brightness
  prevTime = millis();      // Starting time
}



void loop() {
  uint8_t  i;
  uint32_t t;

  switch(mode) {

   case 0: // Random sparkles - just one LED on at a time!
    i = random(NUM_LEDS);           // Choose a random pixel
    pixels.setPixelColor(i, color); // Set it to current color
    pixels.show();                  // Refresh LED states
    // Set same pixel to "off" color now but DON'T refresh...
    // it stays on for now...both this and the next random
    // pixel will be refreshed on the next pass.
    pixels.setPixelColor(i, 0);
    delay(10);                      // 10 millisecond delay
    break;
 
   case 1: // Spinny wheel (4 LEDs on at a time)
    for(i=0; i<NUM_LEDS; i++) {    // For each LED...
      uint32_t c = 0;              // Assume pixel will be "off" color
      if(((offset + i) & 7) < 2) { // For each 8 pixels, 2 will be...
        color = 0x00FF0F;
        c = color;                 // ...assigned the current color
      }
      pixels.setPixelColor(i, c);  // Set color of pixel 'i'
    }
    pixels.show();                 // Refresh LED states
    delay(50);                     // 50 millisecond delay
    offset++;                      // Shift animation by 1 pixel on next frame
    break;

   case 2: // rainbow
    uint16_t i, j;

    for(j=0; j<256*2; j++) { // 5 cycles of all colors on wheel
      for(i=0; i< pixels.numPixels(); i++) {
        pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
      }
    pixels.show();
    delay(wait);
    }
    break;

    // More animation modes could be added here!

    case 3:
      //Theatre-style crawling lights with rainbow effect
      for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
          for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
            pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
          }
          pixels.show();
    
          delay(wait);
    
          for (uint16_t i=0; i < pixels.numPixels(); i=i+3) {
            pixels.setPixelColor(i+q, 0);        //turn every third pixel off
          }
        }
      }
      break;

     case 4:
      colorWipe(pixels.Color(0,0,139), 50);
      colorWipe(pixels.Color(139,0,0), 50);
      colorWipe(pixels.Color(255,165,0), 50);
      colorWipe(pixels.Color(244,241,94), 50);
      colorWipe(pixels.Color(0,100,0), 50);
      
      break;

     case 5:
      fadeInOut(255, 0, 0); //red
      fadeInOut(255, 255, 255); //white
      fadeInOut(0, 0, 255); //blue
      break;

     case 6:
      for (uint16_t i = 0; i < 5; i++) {
        strobe(255, 0, 255);
      }
      break;
      
  }

  t = millis();                    // Current time in milliseconds
  if((t - prevTime) > 8000) {      // Every 8 seconds...
    mode++;
    // Advance to next animation mode
    if(mode > 6) {                 // End of modes?
      mode = 0;                    // Start over from beginning
      color >>= 8;                 // And change color
      if(!color) color = 0xFF8000; // preiodically reset to amber
    }
    pixels.clear();                // Set all pixels to 'off' state
    prevTime = t;                  // Record the time of the last mode change
  }
}

void colorWipe(uint32_t c, uint8_t waits) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
        pixels.setPixelColor(i, c);
        pixels.show();
        delay(waits);
      }
}


uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void fade() {

  uint32_t blueFade = pixels.ColorHSV(43650,255,bright); //set blueFade color to variable bright
  pixels.fill(blueFade);                                 //fill strip with color blueFade
  pixels.show();                                         //send data to strip
  bright = bright + dir;                                //change bright by +1 or -1
  if(bright > 75 || bright < 10) dir = dir * -1;        //set dir to +1 or -1
  delay(10);                                            //speed of fade faster with lower delay valu
}


// 0 to 255
void brighten() {
  uint16_t i, j;
//  uint16_t r, g, b;


  for (j = 45; j < 255; j++) {
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, 255,0,j);
    }
    pixels.show();
    delay(10);
  }
  //delay(1500);
}

// 255 to 0
void darken() {
  Serial.begin(9600);
  uint16_t i, j;

  for (j = 255; j > 45; j--) {
    for (i = 0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i, 0, j, j);
    }
    pixels.show();
    delay(10);
    Serial.println(j);
  }
  delay(1500);
}

void strobe(uint32_t red, uint32_t green, uint32_t blue) {
  uint8_t r, g, b;

  
  for (uint16_t k = 0; k < 256; k++) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    for (uint16_t i=0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i,r,g,b);
    }
    
    pixels.show();
  }

  for (uint16_t k = 255; k >= 10; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;

    for (uint16_t i=0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i,r,g,b);
    }
    pixels.show();
  }

  
}


void fadeInOut(uint32_t red, uint32_t green, uint32_t blue) {
  uint32_t r, g, b;

  for (uint16_t k = 0; k < 256; k++) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;
    for (uint16_t i=0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i,r,g,b);
    }
    
    pixels.show();
    delay(5);
  }

  for (uint16_t k = 255; k >= 10; k=k-2) {
    r = (k/256.0)*red;
    g = (k/256.0)*green;
    b = (k/256.0)*blue;

    for (uint16_t i=0; i < pixels.numPixels(); i++) {
      pixels.setPixelColor(i,r,g,b);
    }
    pixels.show();
    delay(5);
  }

  
}
