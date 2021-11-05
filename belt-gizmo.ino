#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define BUTTON1_PIN 2
#define BUTTON2_PIN 3
#define PIXEL_PIN 6  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 7  // Number of NeoPixels
int counter = 0;
int colorCounter = 0;
int buttonState;         // current state of the button
int colorState;
int lastButtonState;
int lastColorState;
int color;
int red = 0x8B;
int green = 0x0;
int blue = 0x0;

SoftwareSerial mySerial(10, 11); // RX, TX

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

boolean oldState = HIGH;

void setup() {
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  colorWipe(strip.Color(255,   0,   0), 200);
  colorWipe(strip.Color(  0,   0,   0), 200);
  Serial.begin(115200);
  mySerial.begin(9600);
  randomSeed(analogRead(0));
  Serial.print("Gizmo Start");
}

void loop() {

// read the pushbutton input pin:
  buttonState = digitalRead(BUTTON1_PIN);
  colorState = digitalRead(BUTTON2_PIN);

  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == LOW) {
      // if the current state is HIGH then the button
      counter++;
    } 
  }

  // compare the colorState to its previous state
  if (colorState != lastColorState) {
    // if the state has changed, increment the counter
    if (colorState == LOW) {
      // if the current state is HIGH then the button
      colorCounter++;
    } 
  }  

  // save the current state as the last state, 
  //for next time through the loop
  lastButtonState = buttonState;
  lastColorState = colorState;


  if (colorCounter == 0) {
    // Red
    red = 0x8B;
    green = 0x0;
    blue = 0x0;
  }
  
  else if (colorCounter == 1) {
    // Blue
    red = 0x0;
    green = 0x0;
    blue = 0x8B;
  }

  else if(colorCounter == 2) {
    // Green
    red = 0x0;
    green = 0x80;
    blue = 0x0;
  }
  
   else if (colorCounter == 3) {
    // Purple
    red = 0x8B;
    green = 0x0;
    blue = 0x8B;
  }
  
  else if (colorCounter == 4) {
    // Yellow
    red = 0xFF;
    green = 0xFF;
    blue = 0x0;
  } 

  else if (colorCounter == 5) {
    // White
    red = 0xFF;
    green = 0xFF;
    blue = 0xFF;
  } 
  
  else {
    // Red
    red = 0x8B;
    green = 0x0;
    blue = 0x0;
  }

 
  if (counter == 0) {
    Twinkle(red, green, blue, 10, 150, false);
  }
  
  else if (counter == 1) {
    RunningLights(red,green,blue, 150);
  }

  else if(counter == 2) {
    CylonBounce(red, green, blue, 1, 100, 100);
  }
  
   else if (counter == 3) {
    theaterChase(red,green,blue,200);
  }
  
  else if (counter == 4) {
    TwinkleRandom(7, 150, true);
  }

  else if (counter == 5) {
   theaterChaseRainbow(5);
  }

  else if (counter == 6) {
    long rand;
    int pause;
    
    rand = random(1, 7);  // returns a value 1-6
    switch (rand) {
      case 1:
      pause = 300;
      Twinkle(red, green, blue, 10, 150, false);
      break;
    
    case 2:
      pause = 300;
      RunningLights(red,green,blue, 150);
      break;
      
    case 3:
      pause = 300;
      CylonBounce(red, green, blue, 1, 100, 100);
      break;
    
    case 4:
      pause = 300;
      theaterChase(red,green,blue,200);
      break;
    
    case 5:
      pause = 300;
      TwinkleRandom(7, 150, true);
      break;
    
    case 6:
      pause = 300;
      theaterChaseRainbow(5);
      break; 
    
    default:
      Serial.print("I shouldn't be here, rand = ");
      Serial.println(rand);
      break;
   }
   
   delay(pause);   // There are better alternatives than delay()
  }
  
  else {
    counter = 0;
  }

long previousMillis = 0;
long interval = 5000;

//on the loop
unsigned long currentMillis = millis();
if(currentMillis - previousMillis > interval) {
  Serial.println("test");
  previousMillis = currentMillis;
  interval += 5000; 
}

}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void CylonBounce(byte red, byte green, byte blue, int EyeSize, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < PIXEL_COUNT-EyeSize-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }

  delay(ReturnDelay);

  for(int i = PIXEL_COUNT-EyeSize-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= EyeSize; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+EyeSize+1, red/10, green/10, blue/10);
    showStrip();
    delay(SpeedDelay);
  }
 
  delay(ReturnDelay);
}

void Twinkle(byte red, byte green, byte blue, int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
 
  for (int i=0; i<Count; i++) {
     setPixel(random(PIXEL_COUNT),red,green,blue);
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) {
       setAll(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}

void showStrip() {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.show();
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   FastLED.show();
 #endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
 #ifdef ADAFRUIT_NEOPIXEL_H
   // NeoPixel
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
 #endif
 #ifndef ADAFRUIT_NEOPIXEL_H
   // FastLED
   leds[Pixel].r = red;
   leds[Pixel].g = green;
   leds[Pixel].b = blue;
 #endif
}

void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < PIXEL_COUNT; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
}

byte * Wheel(byte WheelPos) {
  static byte c[3];
 
  if(WheelPos < 85) {
   c[0]=WheelPos * 3;
   c[1]=255 - WheelPos * 3;
   c[2]=0;
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   c[0]=255 - WheelPos * 3;
   c[1]=0;
   c[2]=WheelPos * 3;
  } else {
   WheelPos -= 170;
   c[0]=0;
   c[1]=WheelPos * 3;
   c[2]=255 - WheelPos * 3;
  }

  return c;
}

void theaterChase(byte red, byte green, byte blue, int SpeedDelay) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < PIXEL_COUNT; i=i+3) {
        setPixel(i+q, red, green, blue);    //turn every third pixel on
      }
      showStrip();
     
      delay(SpeedDelay);
     
      for (int i=0; i < PIXEL_COUNT; i=i+3) {
        setPixel(i+q, 0,0,0);        //turn every third pixel off
      }
    }
  }
}

void theaterChaseRainbow(int SpeedDelay) {
  byte *c;
 
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < PIXEL_COUNT; i=i+3) {
          c = Wheel( (i+j) % 255);
          setPixel(i+q, *c, *(c+1), *(c+2));    //turn every third pixel on
        }
        showStrip();
       
        delay(SpeedDelay);
       
        for (int i=0; i < PIXEL_COUNT; i=i+3) {
          setPixel(i+q, 0,0,0);        //turn every third pixel off
        }
    }
  }
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
 
  for(int j=0; j<PIXEL_COUNT*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<PIXEL_COUNT; i++) {
        // sine wave, 3 offset waves make a rainbow!
        //float level = sin(i+Position) * 127 + 128;
        //setPixel(i,level,0,0);
        //float level = sin(i+Position) * 127 + 128;
        setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
                   ((sin(i+Position) * 127 + 128)/255)*green,
                   ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      showStrip();
      delay(WaveDelay);
  }
}

void TwinkleRandom(int Count, int SpeedDelay, boolean OnlyOne) {
  setAll(0,0,0);
 
  for (int i=0; i<Count; i++) {
     setPixel(random(PIXEL_COUNT),random(0,255),random(0,255),random(0,255));
     showStrip();
     delay(SpeedDelay);
     if(OnlyOne) {
       setAll(0,0,0);
     }
   }
 
  delay(SpeedDelay);
}
