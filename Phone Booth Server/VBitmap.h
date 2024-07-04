#ifndef VBitmap_h
#define VBitmap_h
#include "apa102LEDStrip.h"
#include <stdint.h>
#include <cstring>
#define PI 3.1415926535897932384626433832795

class VBitmap
{
  public:
    
  VBitmap();
	VBitmap(unsigned short int frameWidth, unsigned short int frameHeight, uint8_t bytesPerPixel);
	void init(unsigned short int frameWidth, unsigned short int frameHeight, uint8_t bytesPerPixel);
  void setGlobalBrightness(uint8_t gBValue);
  void clear();  
  void drawPixel(unsigned short int pixX, unsigned short int pixY, uint8_t *pixelColour);
  void getPixel(unsigned short int pixX, unsigned short int pixY, uint8_t *pixelColour);
  int getWave(float sinCounter, float minWave, float maxWave);
  void getCircularPoints(unsigned short int circleX, unsigned short int circleY, unsigned short int circleR, unsigned short int angleFromTopLeftoRight, short int *points);
  //--------------
  void getAdvancedCircularPoints(unsigned short int circleX, unsigned short int circleY, unsigned short int circleR, unsigned short int angleFromTopLeftoRight, short int xFactor, short int yFactor, short int xxFactor, short int yyFactor, short int *points);
  void fillArea(unsigned short int xStart, unsigned short int xEnd, unsigned short int yStart, unsigned short int yEnd, uint8_t *pixelColour);
  void drawHLine(unsigned short int x, unsigned short int y, unsigned short int lineLength, uint8_t *pixelColour);
  void drawVLine(unsigned short int x, unsigned short int y, unsigned short int lineLength, uint8_t *pixelColour);
  void drawLine(short int x0, short int y0, short int x1, short int y1, uint8_t *pixelColour);
  void drawPolly(short int cX, short int cY, unsigned short int radius, uint8_t *pixColour, unsigned short int rotationAngle, unsigned short int points);
  void drawCCircle(unsigned short int x0,unsigned short int y0,unsigned short int radius, uint8_t *pixelColour);
  
  void singleSubtractiveFade(unsigned short int fadeLevel, uint8_t *pixelColour);
  void subtractiveFade(unsigned short int fadeLevel);
  void rangedSubtractiveFade(unsigned short int fadeLevel, unsigned short int leftBound, unsigned short int rightBound, unsigned short int topBound, unsigned short int bottomBound);
  void gradiatedSubtractiveFade(uint8_t gradientDirection, uint8_t fadeStep,  uint8_t fadeValue);
  
  void shiftDown(bool wrap, unsigned short int  leftBound, unsigned short int  rightBound, unsigned short int  topBound, unsigned short int  bottomBound);
  void shiftUp(bool wrap, unsigned short int  leftBound, unsigned short int  rightBound, unsigned short int  topBound, unsigned short int  bottomBound);
  void shiftLeft(bool wrap, unsigned short int  leftBound, unsigned short int  rightBound, unsigned short int  topBound, unsigned short int  bottomBound);
  void shiftRight(bool wrap, unsigned short int  leftBound, unsigned short int  rightBound, unsigned short int  topBound, unsigned short int  bottomBound);
  void splitShift(uint8_t direction, uint8_t numberOfStripes, uint8_t wrap);
  
  void assignMapToLEDArray(short int **maskMap, apa102LEDStrip *leds);
  short int getRealAddress(unsigned short int xPos, unsigned short int yPos, short int** intMaskMap);
  
  short int **maskMap;
  uint8_t ***bitmap;
  uint8_t **verBuffer;
  uint8_t **horBuffer;
  
	uint8_t bytesPerLed;
	unsigned short int rows;
	unsigned short int cols;
  const short int deadValue = -1;
  uint8_t _blankColourBlock[3] = {0,0,0};
  uint8_t _globalBrightness = 255;
    
  private:
	unsigned short int _xCount;
	unsigned short int _yCount;
};

#endif
