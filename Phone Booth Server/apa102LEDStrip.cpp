#include "apa102LEDStrip.h"
#include "math.h"

apa102LEDStrip::apa102LEDStrip()
{  
}

void apa102LEDStrip::init(unsigned short int numLEDs, uint8_t bytesPerLED, uint8_t globalBrightness)
{  
  _numLEDs = numLEDs;
  _bytesPerLED = bytesPerLED;
  _endFrameLength = round( (numLEDs/2)/8 );
  _frameLength = (2+numLEDs+_endFrameLength)*bytesPerLED;
  _globalBrightness = globalBrightness;
  _fullByte=255;
  _emptyByte=0;
  LEDs = new uint8_t*[_frameLength];
  SPIFrame = new uint8_t[_frameLength];
}

void apa102LEDStrip::renderFrame()
{
	for(_index=0; _index<_frameLength; _index++)
	{
		SPIFrame[_index] = *LEDs[_index];
	}
}

void apa102LEDStrip::setGlobalBrightness(uint8_t globalBrightness)
{
  _globalBrightness = globalBrightness;
  for(_index=_bytesPerLED*2; _index<(_frameLength-(_endFrameLength*_bytesPerLED)); _index+=_bytesPerLED)
  {
    LEDs[_index] = &_globalBrightness;
  }  
}

void  apa102LEDStrip::setGlobalBrightness(unsigned short int pixelIndex, uint8_t pixelBrightness)
{
  LEDs[ (_bytesPerLED*2)+(_bytesPerLED*pixelIndex) ] = &pixelBrightness;
}
