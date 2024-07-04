#include "apa102LEDStrip.h"
#include <math.h>

apa102LEDStrip::apa102LEDStrip()
{
}

apa102LEDStrip::apa102LEDStrip(short int numLEDs, char bytesPerLED, char globalBrightness)
{  
  _numLEDs = numLEDs;
  _bytesPerLED = bytesPerLED;
  _endFrameLength = 1;//round( (numLEDs/2)/8 );
  _frameLength = (1+1+numLEDs+_endFrameLength)*bytesPerLED;
  LEDs = new char[_frameLength];
  _globalBrightness = globalBrightness;

  //Start Frame
  LEDs[0] = 0;
  LEDs[1] = 0;
  LEDs[2] = 0;
  LEDs[3] = 0;
  //Driver frame+PIXEL frames
  for(_counter=_bytesPerLED; _counter<_frameLength-(_endFrameLength*_bytesPerLED); _counter+=_bytesPerLED)
  {
    LEDs[_counter] = _globalBrightness;
    LEDs[_counter+1] = 0;
    LEDs[_counter+2] = 0;
    LEDs[_counter+3] = 0;
  }
  //END frames
  for(_counter=_frameLength-(_endFrameLength*_bytesPerLED); _counter<_frameLength; _counter+=_bytesPerLED)
  {
    LEDs[_counter] = 255;
    LEDs[_counter+1] = 255;
    LEDs[_counter+2] = 255;
    LEDs[_counter+3] = 255;
  }
}

void apa102LEDStrip::setGlobalBrightness(char brightnessValue)
{
  for(_counter=_bytesPerLED; _counter<_frameLength-(_endFrameLength*_bytesPerLED); _counter+=_bytesPerLED)
  {
    LEDs[_counter] = brightnessValue;
  }  
}

void apa102LEDStrip::setPixel(short int pixelIndex, char *pixelColour)
{
  _counter = 4*(pixelIndex+1);
  LEDs[ _counter + 1 ] = pixelColour[2];
  LEDs[ _counter + 2 ] = pixelColour[1];
  LEDs[ _counter + 3 ] = pixelColour[0];
}
void apa102LEDStrip::getPixel(short int pixelIndex, char *pixelColour)
{
  _counter = 4*(pixelIndex+1);
  pixelColour[2] = LEDs[ _counter + 1 ];
  pixelColour[1] = LEDs[ _counter + 2 ];
  pixelColour[0] = LEDs[ _counter + 3 ];
}