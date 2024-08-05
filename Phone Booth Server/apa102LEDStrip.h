#ifndef apa102LEDStrip_h
#define apa102LEDStrip_h
#include <stdint.h>
#include <cstring>

class apa102LEDStrip
{
  public:
    apa102LEDStrip();
	void init(unsigned short int numLEDs, uint8_t bytesPerLED, uint8_t globalBrightness);
    void renderFrame();
    void setGlobalBrightness(uint8_t globalBrightness);
    void setGlobalBrightness(unsigned short int pixelIndex, uint8_t pixelBrightness);
	
    uint8_t **LEDs;
    uint8_t* SPIFrame;
	
    short int _frameLength;
    short int _endFrameLength;
    short int _numLEDs;
    uint8_t _bytesPerLED;
    short int _counter;
    uint8_t _fullByte;
    uint8_t _emptyByte;
    uint8_t _globalBrightness;
    
  private:
	unsigned short int _index;
};

#endif
