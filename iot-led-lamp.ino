#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

const int NUM_PIXELS = 60;
const int MAX_NUM_OF_PARAMS = 5;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRBW + NEO_KHZ800);

//setPixelColor:0-10;255;255;255;255;

void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif
  // End of trinket special code

  Serial.begin(9600);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.println("strip initialized");
  
//  Serial.println("");
//  Serial.println("");
}

void loop() {  
//  Serial.println("checking for commands");
  
  String cmd = checkForCommand();
  int c = 0;
  
  while (cmd.length() > 0) {
    c++;
    Serial.print("input found: ");
    Serial.println(cmd);

    String method = getStringToSep(cmd, ':', 0);
    int methodLen = method.length();
    if (methodLen > 0) {
      cmd = cmd.substring(methodLen + 1);
      
      Serial.print("performing method: ");
      Serial.println(method);

      String params[MAX_NUM_OF_PARAMS] = {};
      
      String param = getStringToSep(cmd, ';', 0);
      int paramLen = param.length();
      bool hasParams = false;
      
      for (int i = 0; paramLen > 0 && i < MAX_NUM_OF_PARAMS; i++) {
          cmd = cmd.substring(paramLen + 1);
          params[i] = param;
          param = getStringToSep(cmd, ';', 0);
          paramLen = param.length();
          hasParams = true;
      }
      
      if (hasParams) {
        Serial.println("parsed params: ");
        for (int i = 0; i < MAX_NUM_OF_PARAMS; i++) {
          Serial.print(i);
          Serial.print(": ");
          Serial.println(params[i]);
        }
      }
      else {
        Serial.println("no params found");
      }

      if (method == "setPixelColor") {
        int arrSepIndex = params[0].indexOf('-');
        int first = 0;
        int last = 0;
        if (arrSepIndex > 0) {
          first = params[0].substring(0, arrSepIndex).toInt();
          last = params[0].substring(arrSepIndex + 1).toInt();
        }
        else {
          first = params[0].toInt();
          last = params[0].toInt();
        }
        for (int i = first; i <= last && i < NUM_PIXELS; i++) {
          setPixel(i, params[1].toInt(), params[2].toInt(), params[3].toInt(), params[4].toInt());
        }
      }
    }
  }
  
//  Serial.print("commands processed: ");
//  Serial.println(c);

  if (c > 0) {
    Serial.println("updating strip");
    strip.show();
  }
  
  c = 0;

//  Serial.println("waiting 5 seconds");
  delay(1);
  
//  Serial.println("");
//  Serial.println("");
}

void setPixel(int pixel, int r, int g, int b, int w) {
  strip.setPixelColor(pixel, strip.Color(r, g, b, w));
}

String getStringToSep(String str, char sep, int from) {
  int sepIndex = str.indexOf(sep);
  if (sepIndex > 0) {
    return str.substring(from, sepIndex);
  }
  return "";
}

String checkForCommand() {
  return Serial.readStringUntil('\n');
}

//void loop() {
//
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
////    strip.setPixelColor(random(strip.numPixels()), strip.Color(0, 0, 0, random(127)));
//    strip.setPixelColor(i, strip.Color(0, 0, 0, 255));
//    strip.show();
//  }
//  delay(1);
//  
//////  // Some example procedures showing how to display to the pixels:
////  colorWipe(strip.Color(255, 0, 0), 50); // Red
////  colorWipe(strip.Color(0, 255, 0), 50); // Green
////  colorWipe(strip.Color(0, 0, 255), 50); // Blue
////  colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
//////  // Send a theater pixel chase in...
////  theaterChase(strip.Color(127, 127, 127), 50); // White
////  theaterChase(strip.Color(127, 0, 0), 50); // Red
////  theaterChase(strip.Color(72, 0, 175), 50); // Blue
////
////  rainbow(20);
////  rainbowCycle(5);
////  theaterChaseRainbow(50);
//}
//
//// Fill the dots one after the other with a color
//void colorWipe(uint32_t c, uint8_t wait) {
//  for(uint16_t i=0; i<strip.numPixels(); i++) {
//    strip.setPixelColor(i, c);
//    strip.show();
//    delay(wait);
//  }
//}
//
//void rainbow(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256; j++) {
//    for(i=0; i<strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel((i+j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//  uint16_t i, j;
//
//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
//    for(i=0; i< strip.numPixels(); i++) {
//      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//    }
//    strip.show();
//    delay(wait);
//  }
//}
//
////Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, c);    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
//    for (int q=0; q < 3; q++) {
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
//      }
//      strip.show();
//
//      delay(wait);
//
//      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
//        strip.setPixelColor(i+q, 0);        //turn every third pixel off
//      }
//    }
//  }
//}
//
//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
//uint32_t Wheel(byte WheelPos) {
//  WheelPos = 255 - WheelPos;
//  if(WheelPos < 85) {
//    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
//  }
//  if(WheelPos < 170) {
//    WheelPos -= 85;
//    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
//  }
//  WheelPos -= 170;
//  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
//}
