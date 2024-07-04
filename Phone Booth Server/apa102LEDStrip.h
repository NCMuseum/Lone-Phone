#ifndef apa102LEDStrip_h
#define apa102LEDStrip_h

class apa102LEDStrip
{
  public:
	apa102LEDStrip();
	apa102LEDStrip(short int numLEDs, char bytesPerLED, char globalBrightness);
    void setGlobalBrightness(char brightnessValue);
    void setPixel(short int pixelIndex, char *pixelColour);
    void getPixel(short int pixelIndex, char *pixelColour);

	char* LEDs;
	
    char _globalBrightness;
    short int _rainbowSize;
    int _frameLength;
    short int _endFrameLength;
    short int _numLEDs;
    char _bytesPerLED;
    short int _maxValue;
    short int _counter;
	
  private:
};

#endif
