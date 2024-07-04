#include "VBitmap.h"
#include "math.h"

VBitmap::VBitmap()
{  
	
}

VBitmap::VBitmap(unsigned short int frameWidth, unsigned short int frameHeight, uint8_t bytesPerPixel)
{  
	rows = frameHeight;
  cols = frameWidth;
  if(bytesPerPixel>3)
  {
	  bytesPerLed=3;
  }
  else  
 {
  bytesPerLed = bytesPerPixel;
 }
	//Init The Main frame
	
	bitmap = new uint8_t**[frameHeight];
	for(_yCount=0; _yCount<frameHeight; _yCount++)
	{
		bitmap[_yCount] = new uint8_t*[frameWidth];
		for(_xCount=0; _xCount<frameWidth; _xCount++)
		{
			bitmap[_yCount][_xCount] = new uint8_t[bytesPerLed];
			memcpy(bitmap[_yCount][_xCount], _blankColourBlock, bytesPerLed);
		}
	}
 
 //Init the Vertical Buffer
 
 verBuffer = new uint8_t*[frameWidth];
 for(_xCount=0; _xCount<frameWidth; _xCount++)
 {
    verBuffer[_xCount] = new uint8_t[bytesPerLed];
    memcpy(verBuffer[_xCount], _blankColourBlock, bytesPerLed);
 }
 
 //Init the Horizontal Buffer
 
 horBuffer = new uint8_t*[frameHeight];
 for(_yCount=0; _yCount<frameHeight; _yCount++)
 {
    horBuffer[_yCount] = new uint8_t[bytesPerLed];
    memcpy(horBuffer[_yCount], _blankColourBlock, bytesPerLed);
 }
 
}


void VBitmap::init(unsigned short int frameWidth, unsigned short int frameHeight, uint8_t bytesPerPixel)
{  
  rows = frameHeight;
  cols = frameWidth;
  if(bytesPerPixel>3)
  {
	  bytesPerLed=3;
  }
  else  
 {
  bytesPerLed = bytesPerPixel;
 }
  bitmap = new uint8_t**[frameHeight];
  for(_yCount=0; _yCount<frameHeight; _yCount++)
  {
    bitmap[_yCount] = new uint8_t*[frameWidth];
    for(_xCount=0; _xCount<frameWidth; _xCount++)
    {
      bitmap[_yCount][_xCount] = new uint8_t[bytesPerLed];
      memcpy(bitmap[_yCount][_xCount], _blankColourBlock, bytesPerLed);
    }
  }
  //Init the Vertical Buffer
  verBuffer = new uint8_t*[frameWidth];
  for(_xCount=0; _xCount<frameWidth; _xCount++)
  {
    verBuffer[_xCount] = new uint8_t[bytesPerLed];
    memcpy(verBuffer[_xCount], _blankColourBlock, bytesPerLed);
  }
  //Init the Horizontal Buffer
  horBuffer = new uint8_t*[frameHeight];
  for(_yCount=0; _yCount<frameHeight; _yCount++)
  {
    horBuffer[_yCount] = new uint8_t[bytesPerLed];
    memcpy(horBuffer[_yCount], _blankColourBlock, bytesPerLed);
  }

}

void VBitmap::clear()
{  
  for(_yCount=0; _yCount<rows; _yCount++)
  {
    for(_xCount=0; _xCount<cols; _xCount++)
    {
      memcpy(bitmap[_yCount][_xCount], _blankColourBlock, bytesPerLed);
    }
  }
}

void VBitmap::setGlobalBrightness(uint8_t gBValue)
{
  _globalBrightness = gBValue;
}
 
int VBitmap::getWave(float sinCounter, float minWave, float maxWave)
{
  return abs( (sin(sinCounter)*(maxWave+1-minWave)))+minWave ;
}


void VBitmap::drawPixel(unsigned short int pixX, unsigned short int pixY, uint8_t *pixelColour)
{
  if(pixX>=0 && pixX<cols && pixY>=0 && pixY<rows)
  {
    memcpy(bitmap[pixY][pixX], pixelColour, 3);
  }
}

void VBitmap::getPixel(unsigned short int pixX, unsigned short int pixY, uint8_t *pixelColour)
{
  if(pixX>=0 && pixX<cols && pixY>=0 && pixY<rows)
  {
    memcpy(pixelColour, bitmap[pixY][pixX], 3);
  }
}

void VBitmap::getCircularPoints(unsigned short int circleX,unsigned short int circleY,unsigned short int circleR,unsigned short int angleFromTopLeftoRight, short int *points)
{
    points[0] = round( circleX + (sin(angleFromTopLeftoRight)*circleR) );
    points[1] = round( circleY - (cos(angleFromTopLeftoRight)*circleR) );
}

void VBitmap::getAdvancedCircularPoints(unsigned short int circleX, unsigned short int circleY, unsigned short int circleR, unsigned short int angleFromTopLeftoRight, short int xFactor, short int yFactor, short int xxFactor, short int yyFactor, short int *points)
{
  points[0] = round( circleX + sin(angleFromTopLeftoRight*xFactor*(PI / 180))*circleR*xxFactor );
  points[1] = round( circleY - cos(angleFromTopLeftoRight*yFactor*(PI / 180))*circleR*yyFactor );
}

void VBitmap::fillArea(unsigned short int xStart, unsigned short int xEnd, unsigned short int yStart, unsigned short int yEnd, uint8_t *pixelColour)
{
  if(xEnd==xStart || xEnd<xStart || yStart==yEnd || yEnd<yStart){return;}
  for(yStart; yStart<yEnd; yStart++)
  {
    drawHLine(xStart, yStart, xEnd-xStart, pixelColour);
  }
}

void VBitmap::drawHLine(unsigned short int x, unsigned short int y, unsigned short int lineLength, uint8_t *pixelColour)
{
  unsigned short int hLineXCnt=0;
  
  if(x>=0 && x<cols && y>=0 && y<rows && (x+lineLength-1)<cols)
  {
    for(hLineXCnt=x; hLineXCnt<(x+lineLength); hLineXCnt++)
    {
        drawPixel(hLineXCnt, y, pixelColour);   
    }  
  }    
}

void VBitmap::drawVLine(unsigned short int x, unsigned short int y, unsigned short int lineLength, uint8_t *pixelColour)
{
  unsigned short int hLineYCnt=0;
  
  if(x>=0 && x<cols && y>=0 && y<rows && (y+lineLength-1)<rows)
  {
    for(hLineYCnt=y; hLineYCnt<(y+lineLength); hLineYCnt++)
    {
        drawPixel(x, hLineYCnt, pixelColour);   
    }  
  }    
}

void VBitmap::drawLine(short int x0, short int y0, short int x1, short int y1, uint8_t *pixelColour)
{
  short int  dx = abs(x1-x0);
  short int  dy = abs(y1-y0);
  short int  sx = (x0 < x1) ? 1 : -1;
  short int  sy = (y0 < y1) ? 1 : -1;
  short int  err = dx-dy;
  short int e2 =0;
  
  while(true)
  {
    drawPixel(x0, y0, pixelColour);
    if ((x0==x1) && (y0==y1)) break;
    e2 = 2*err;
    if (e2 >-dy){ err -= dy; x0  += sx; }
    if (e2 < dx){ err += dx; y0  += sy; }
  }
}

//poly line draw
void VBitmap::drawPolly(short int cX, short int cY, unsigned short int radius, uint8_t *pixColour, unsigned short int rotationAngle, unsigned short int points)
{
  if(points<3){return;}
  short int firstPoint[2]={0,0}, secondPoint[2]={0,0}, tempPoint[2]={0,0};
  short int pollyCount=0, pollyAngle=(360/points);
  
  //generate 1st polly point
  getAdvancedCircularPoints(cX, cY, radius, rotationAngle, 1, 1, 1, 1, firstPoint);
  //Generate all other points and connect
  for(pollyCount=1; pollyCount<points; pollyCount++)
  {
    if(pollyCount==1)
    {
      getAdvancedCircularPoints(cX, cY, radius, (pollyCount*pollyAngle)+rotationAngle, 1, 1, 1, 1, secondPoint);
      drawLine(firstPoint[0],firstPoint[1],secondPoint[0],secondPoint[1],pixColour);
    }
    else if(pollyCount+1<=points)
    {
      getAdvancedCircularPoints(cX, cY, radius, (pollyCount*pollyAngle)+rotationAngle, 1, 1, 1, 1, tempPoint);
      drawLine(secondPoint[0],secondPoint[1],tempPoint[0],tempPoint[1],pixColour);    
      secondPoint[0] = tempPoint[0];
      secondPoint[1] = tempPoint[1];
    }
  }
  drawLine(tempPoint[0],tempPoint[1],firstPoint[0],firstPoint[1],pixColour);    
}

void VBitmap::drawCCircle(unsigned short int x0,unsigned short int y0,unsigned short int radius, uint8_t *pixelColour)
{
  short int degCounter = 0;
  short int tempCPoints[2];
  
  for(degCounter=0; degCounter<360; degCounter++)
  {
    getCircularPoints(x0, y0, radius, degCounter, tempCPoints);
    drawPixel(tempCPoints[0], tempCPoints[1], pixelColour);
  }
}

void VBitmap::singleSubtractiveFade(unsigned short int fadeLevel, uint8_t *pixelColour)
{
  if(fadeLevel<pixelColour[0]){pixelColour[0]-=fadeLevel;}
  else{pixelColour[0]=0;}
  if(fadeLevel<pixelColour[1]){pixelColour[1]-=fadeLevel;}
  else{pixelColour[1]=0;}
  if(fadeLevel<pixelColour[2]){pixelColour[2]-=fadeLevel;}
  else{pixelColour[2]=0;}
}

void VBitmap::subtractiveFade(unsigned short int fadeLevel)
{
  for(_yCount=0; _yCount<rows; _yCount++)
  {
    for(_xCount=0; _xCount<cols; _xCount++)
    {
      if( ((unsigned short int)bitmap[_yCount][_xCount][0] + (unsigned short int)bitmap[_yCount][_xCount][1] + (unsigned short int)bitmap[_yCount][_xCount][2])!=0 )
      {
        if(fadeLevel<bitmap[_yCount][_xCount][0]){bitmap[_yCount][_xCount][0]-=fadeLevel;}
        else{bitmap[_yCount][_xCount][0]=0;}
        if(fadeLevel<bitmap[_yCount][_xCount][1]){bitmap[_yCount][_xCount][1]-=fadeLevel;}
        else{bitmap[_yCount][_xCount][1]=0;}
        if(fadeLevel<bitmap[_yCount][_xCount][2]){bitmap[_yCount][_xCount][2]-=fadeLevel;}
        else{bitmap[_yCount][_xCount][2]=0;}
      }
    }
  }  
}

void VBitmap::rangedSubtractiveFade(unsigned short int fadeLevel, unsigned short int leftBound, unsigned short int rightBound, unsigned short int topBound, unsigned short int bottomBound)
{  
  for(_yCount=topBound; _yCount<bottomBound; _yCount++)
  {
    for(_xCount=leftBound; _xCount<rightBound; _xCount++)
    {
      if( ((unsigned short int)bitmap[_yCount][_xCount][0] + (unsigned short int)bitmap[_yCount][_xCount][1] + (unsigned short int)bitmap[_yCount][_xCount][2])!=0 )
      {
        if(fadeLevel<bitmap[_yCount][_xCount][0]){bitmap[_yCount][_xCount][0]-=fadeLevel;}
        else{bitmap[_yCount][_xCount][0]=0;}
        if(fadeLevel<bitmap[_yCount][_xCount][1]){bitmap[_yCount][_xCount][1]-=fadeLevel;}
        else{bitmap[_yCount][_xCount][1]=0;}
        if(fadeLevel<bitmap[_yCount][_xCount][2]){bitmap[_yCount][_xCount][2]-=fadeLevel;}
        else{bitmap[_yCount][_xCount][2]=0;}
      }
    }
  }  
}

void VBitmap::gradiatedSubtractiveFade(uint8_t gradientDirection, uint8_t fadeStep,  uint8_t fadeValue)
{
  uint8_t gradYCounter=0;
  
  //0 right 1 down 2 left 3 up
  if(gradientDirection==0)
  {
    
  }
  else if(gradientDirection==1)
  {
    
  }
  else if(gradientDirection==2)
  {
    
  }
  else if(gradientDirection==3)
  {
    for(gradYCounter=rows-1; gradYCounter>0; gradYCounter--)
    {
      rangedSubtractiveFade(fadeValue, 0, cols, gradYCounter, gradYCounter+1);
      if(fadeStep>fadeValue)
      {
        return;
      }
      else
      {
        fadeValue-=fadeStep;
      }
    }
  }
}

//Split Shifter uses above shifters
void VBitmap::splitShift(uint8_t direction, uint8_t numberOfStripes, uint8_t wrap)
{
  //  0 = up/down splits
  //  1 = left/right splits
  short int splitCnt;
  short int cords[2];
  
  if(direction==0)
  {
    for(splitCnt=0; splitCnt<numberOfStripes; splitCnt++)
    {
      cords[0] = (short int)round(((float)cols/(float)numberOfStripes)*(float)splitCnt);
      cords[1] = (int)round(((float)cols/(float)numberOfStripes)*(float)(splitCnt+1));
      if(splitCnt==numberOfStripes-1)
      {
        cords[1]--;
      }
      if(splitCnt%2==0)
      {
        shiftUp(wrap, cords[0] , cords[1] , 0, rows-1);
      }
      else
      {
        shiftDown(wrap, cords[0] , cords[1] , 0, rows-1);
      }
    }
  }
  else if(direction==1)
  {
    for(splitCnt=0; splitCnt<numberOfStripes; splitCnt++)
    {
      cords[0] = (short int)round(((float)rows/(float)numberOfStripes)*(float)splitCnt);
      cords[1] = (int)round(((float)rows/(float)numberOfStripes)*(float)(splitCnt+1));
      if(splitCnt==numberOfStripes-1)
      {
        cords[1]--;
      }
      if(splitCnt%2==0)
      {
        shiftLeft(wrap, 0, cols-1, cords[0] , cords[1] );
      }
      else
      {
        shiftRight(wrap, 0, cols-1, cords[0] , cords[1] );
      }
    }
  }
}

void VBitmap::shiftDown(bool wrap, unsigned short int leftBound, unsigned short int rightBound, unsigned short int topBound, unsigned short int bottomBound)
{
  unsigned short int shiftxCnt=0, shiftyCnt=0;
  //if wrap is true save the Line at "bottomBound"
  if(wrap)
   {
     for(shiftxCnt=0; shiftxCnt<cols; shiftxCnt++)
     {
        memcpy(verBuffer[shiftxCnt], bitmap[bottomBound][shiftxCnt], bytesPerLed);
     }
   }
   //shift down for "bottomBound-topBound" from "topBound" to "bottomBound"
   for(shiftyCnt=bottomBound; shiftyCnt>topBound; shiftyCnt--)
   {
     for(shiftxCnt=leftBound; shiftxCnt<rightBound; shiftxCnt++)
     {
       memcpy(bitmap[shiftyCnt][shiftxCnt], bitmap[shiftyCnt-1][shiftxCnt], bytesPerLed);
     }
   }
   if(wrap)
   {
     for(shiftxCnt=leftBound; shiftxCnt<rightBound; shiftxCnt++)
     {
       memcpy(bitmap[topBound][shiftxCnt], verBuffer[shiftxCnt], bytesPerLed);
     }
   }  
}
void VBitmap::shiftUp(bool wrap, unsigned short int leftBound, unsigned short int rightBound, unsigned short int topBound, unsigned short int bottomBound)
{
  unsigned short int shiftxCnt=0, shiftyCnt=0;
  //if wrap is true save the Line at "topBound"
  if(wrap)
   {
     for(shiftxCnt=0; shiftxCnt<cols; shiftxCnt++)
     {
        memcpy(verBuffer[shiftxCnt], bitmap[topBound][shiftxCnt], bytesPerLed);
     }
   }
   //shift up for "bottomBound-topBound" from "bottomBound" to "topBound"
   for(shiftyCnt=topBound; shiftyCnt<bottomBound; shiftyCnt++)
   {
     for(shiftxCnt=leftBound; shiftxCnt<rightBound; shiftxCnt++)
     {
       memcpy(bitmap[shiftyCnt][shiftxCnt], bitmap[shiftyCnt+1][shiftxCnt], bytesPerLed);
     }
   }
   if(wrap)
   {
     for(shiftxCnt=leftBound; shiftxCnt<=rightBound; shiftxCnt++)
     {
       memcpy(bitmap[bottomBound][shiftxCnt], verBuffer[shiftxCnt], bytesPerLed);
     }
   }  
}

void VBitmap::shiftRight(bool wrap, unsigned short int leftBound, unsigned short int rightBound, unsigned short int topBound, unsigned short int bottomBound)
{
  unsigned short int shiftxCnt=0, shiftyCnt=0;
   
   if(wrap)
   {
     //save the Vertical line at index rightBound
     for(shiftyCnt=topBound; shiftyCnt<=bottomBound; shiftyCnt++)
     {
        memcpy(horBuffer[shiftyCnt], bitmap[shiftyCnt][rightBound], bytesPerLed);
     }
   }
   //shift right from leftBound to rightBound from topBound to bottomBound
   for(shiftxCnt=rightBound; shiftxCnt>leftBound; shiftxCnt--)
   {
     for(shiftyCnt=topBound; shiftyCnt<bottomBound; shiftyCnt++)
     {
       memcpy(bitmap[shiftyCnt][shiftxCnt], bitmap[shiftyCnt][shiftxCnt-1], bytesPerLed);
     }
   }
   if(wrap)
   {
     for(shiftyCnt=topBound; shiftyCnt<=bottomBound; shiftyCnt++)
     {
       memcpy(bitmap[shiftyCnt][leftBound], horBuffer[shiftyCnt], bytesPerLed);
     }  
   }  
}

void VBitmap::shiftLeft(bool wrap, unsigned short int leftBound, unsigned short int rightBound, unsigned short int topBound, unsigned short int bottomBound)
{
  unsigned short int shiftxCnt=0, shiftyCnt=0;
   if(wrap)
   {
     //save the V line at "leftBound"
     for(shiftyCnt=topBound; shiftyCnt<=bottomBound; shiftyCnt++)
     {
        memcpy(horBuffer[shiftyCnt], bitmap[shiftyCnt][leftBound], bytesPerLed);
     }
   }
   for(shiftxCnt=leftBound; shiftxCnt<rightBound; shiftxCnt++)
   {
     for(shiftyCnt=topBound; shiftyCnt<bottomBound; shiftyCnt++)
     {
       memcpy(bitmap[shiftyCnt][shiftxCnt], bitmap[shiftyCnt][shiftxCnt+1], bytesPerLed);
     }
   }
   if(wrap)
   {
     for(shiftyCnt=topBound; shiftyCnt<=bottomBound; shiftyCnt++)
     {
       memcpy(bitmap[shiftyCnt][rightBound], horBuffer[shiftyCnt], bytesPerLed);
     }
   }
}

//for APA102  sk9822
void VBitmap::assignMapToLEDArray(short int **maskMap, apa102LEDStrip *leds)
{
  short int tempAddress=0, frameIndex=0;
  unsigned short int bitMapX=0, bitMapY=0;

  //init each pointer spot to SOMETHING
  //start frame
  leds->LEDs[0] = &leds->_emptyByte;
  leds->LEDs[1] = &leds->_emptyByte;
  leds->LEDs[2] = &leds->_emptyByte;
  leds->LEDs[3] = &leds->_emptyByte;
  //leds  
  for(_xCount=4; _xCount<leds->_frameLength-(leds->_endFrameLength*4); _xCount+=4)
  {
    leds->LEDs[_xCount] = &leds->_globalBrightness;
    leds->LEDs[_xCount+1] = &leds->_emptyByte;
    leds->LEDs[_xCount+2] = &leds->_emptyByte;
    leds->LEDs[_xCount+3] = &leds->_emptyByte;
  }
 //end frames
  for(_xCount; _xCount<leds->_frameLength; _xCount+=4)
  {
    leds->LEDs[_xCount] = &leds->_fullByte;
    leds->LEDs[_xCount+1] = &leds->_fullByte;
    leds->LEDs[_xCount+2] = &leds->_fullByte;
    leds->LEDs[_xCount+3] = &leds->_fullByte;
  }
  //attatch the mask map to each pointer
  for(bitMapY=0; bitMapY<rows; bitMapY++)
  {
    for(bitMapX=0; bitMapX<cols; bitMapX++)
    {
      tempAddress = getRealAddress(bitMapX, bitMapY, maskMap);
      if(tempAddress!=deadValue)
      {
        leds->LEDs[ (4*(tempAddress+1)) ] = &_globalBrightness;    //Pixels Global Brightness
        leds->LEDs[ (4*(tempAddress+1))+1 ] = &bitmap[bitMapY][bitMapX][2];
        leds->LEDs[ (4*(tempAddress+1))+2 ] = &bitmap[bitMapY][bitMapX][1];
        leds->LEDs[ (4*(tempAddress+1))+3 ] = &bitmap[bitMapY][bitMapX][0];
      } 
    }
  } 
}

short int VBitmap::getRealAddress(unsigned short int xPos, unsigned short int yPos, short int **intMaskMap)
{
   return  intMaskMap[yPos][xPos];
}
