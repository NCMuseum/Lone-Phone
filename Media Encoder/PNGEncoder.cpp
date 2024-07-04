//g++ -o PNGEncoder PNGEncoder.cpp upng.cpp
//	./PNGEncoder/PNGEncoder /home/administrator/Video2/ Video2 1481
//	mv /home/administrator/Video2/Video2.STREAM /home/administrator/lcdScreenApp/
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <cmath>
#include <cstdio>
#include "upng.h"
#include <stdio.h>
#include <stdlib.h>

//Bitmaping Data
const uint8_t rows=64, cols=192;    //rows = HEIGHT(Y)  cols = WIDTH(X)
char integerCharString[] = "00000";

void compileStream(char* animationName, char* animationDirectory, unsigned short int frameCount);
void fileNameIncremeneter(unsigned short int fileNameIndex, char* filePathString);

int main(int argc, char **argv)
{
	char* animationName;
	char* animationDirectory;
	unsigned short int animationNameLength = 0;
	unsigned short int argCounter=0, frameCount=0;
	
	if(argc<3)
	{
		printf("\r\nUsage\r\n\r\n\tPNGEncoder directory[/] animation_name frame_count\r\n\r\n");
		return 1;
	}
	//Animation directory with tailing "/"
	animationDirectory = new char[strlen(argv[1])+1];
	memcpy(animationDirectory, argv[1], strlen(argv[1]));
	animationDirectory[strlen(argv[1])]=0;
	
	//Animation Name
	animationNameLength = strlen(argv[2])+1;
	animationName = new char[animationNameLength];
	memcpy(animationName, argv[2], strlen(argv[2]));
	animationName[animationNameLength-1]=0;
	
	//frame count
	frameCount = atoi(argv[3]);
	
	printf("\r\nAnimationName\t[%s]\tFrames[%d]\tDir[%s]\r\n", animationName, frameCount, animationDirectory);
	compileStream(animationName, animationDirectory, frameCount);
	return 0;
}


void compileStream(char* animationName, char* animationDirectory, unsigned short int frameCount)
{
	char outputStreamName[] = ".STREAM";
	char pngTerminationString[] = "_00000.png";
	const unsigned short int outputStreamLocation_Length = strlen(animationDirectory)+strlen(animationName)+strlen(outputStreamName)+1;
	const unsigned short int inputStreamLocation_Length = strlen(animationDirectory)+strlen(animationName)+strlen(pngTerminationString)+1;
	char* outputStreamLocation = new char[outputStreamLocation_Length];
	char* inputStreamLocation = new char[inputStreamLocation_Length];
	unsigned short int postFrameCounter=0;
	unsigned short int bytesWriten=0;
	upng_t* upng;
	FILE* outputStream;	
	
	//Compile output stream name
	memcpy(outputStreamLocation, animationDirectory, strlen(animationDirectory));
	memcpy(outputStreamLocation+strlen(animationDirectory), animationName, strlen(animationName));
	memcpy(outputStreamLocation+strlen(animationDirectory)+strlen(animationName), outputStreamName, strlen(outputStreamName));
	outputStreamLocation[outputStreamLocation_Length-1]=0;
	
	//compile input steream name with 1st .png file
	memcpy(inputStreamLocation, animationDirectory, strlen(animationDirectory));
	memcpy(inputStreamLocation+strlen(animationDirectory), animationName, strlen(animationName));
	memcpy(inputStreamLocation+strlen(animationDirectory)+strlen(animationName), pngTerminationString, strlen(pngTerminationString));
	inputStreamLocation[inputStreamLocation_Length-1]=0;

	printf("\r\noutput stream locacation[%s]\r\n", outputStreamLocation);
	outputStream = fopen(outputStreamLocation, "wb");
	
	if(outputStream==NULL)
	{
		printf("\r\n\tERROR - Can not open stream file for writing....\r\n");
		return;
	}
	else
	{
		//build the stream file
		for(postFrameCounter=0; postFrameCounter<frameCount; postFrameCounter++)
		{
		  fileNameIncremeneter(postFrameCounter+1, inputStreamLocation);
		  upng = upng_new_from_file(inputStreamLocation);
		  if (upng != NULL)
		  {
			upng_decode(upng);
			printf("\r\nFrame\t%d\t%d\tBytes\t", postFrameCounter, upng_get_size(upng));
			//bytesWriten = outputStream.write(upng_get_buffer(upng), );
			bytesWriten = fwrite(upng_get_buffer(upng) , 1 , upng_get_size(upng) , outputStream );
			upng_free(upng);
			printf("Written\t%d bytes", bytesWriten);
		  }
		}
		fclose(outputStream);
	}
	
}

void fileNameIncremeneter(unsigned short int fileNameIndex, char* filePathString)
{
  char renameAtPointer=0, renameCounter=0;
  char namePlacer = 95; //95="_"

  //find the "_" in the file name fileNamePointer
  for(renameCounter=0; renameCounter<strlen(filePathString); renameCounter++)
  {
    if(filePathString[renameCounter]==namePlacer)
    {
      renameAtPointer = renameCounter+1;
      break;
    }
  }
  //itoa(fileNameIndex, integerCharString, 10);  
  sprintf(integerCharString, "%d", fileNameIndex);
  if(fileNameIndex<10)
  {
    filePathString[renameAtPointer] = 48;
    filePathString[renameAtPointer+1] = 48;
    filePathString[renameAtPointer+2] = 48;
    filePathString[renameAtPointer+3] = 48;
    filePathString[renameAtPointer+4] = integerCharString[0];
  }
  else if(fileNameIndex<100)
  {
    filePathString[renameAtPointer] = 48;
    filePathString[renameAtPointer+1] = 48;
    filePathString[renameAtPointer+2] = 48;
    filePathString[renameAtPointer+3] = integerCharString[0];
    filePathString[renameAtPointer+4] = integerCharString[1];
  }
  else if(fileNameIndex<1000)
  {
    filePathString[renameAtPointer] = 48;
    filePathString[renameAtPointer+1] = 48;
    filePathString[renameAtPointer+2] = integerCharString[0];
    filePathString[renameAtPointer+3] = integerCharString[1];
    filePathString[renameAtPointer+4] = integerCharString[2];
  }
  else if(fileNameIndex<10000)
  {
    filePathString[renameAtPointer] = 48;
    filePathString[renameAtPointer+1] = integerCharString[0];
    filePathString[renameAtPointer+2] = integerCharString[1];
    filePathString[renameAtPointer+3] = integerCharString[2];
    filePathString[renameAtPointer+4] = integerCharString[3];
  }
  else if(fileNameIndex<100000)
  {
    filePathString[renameAtPointer] = integerCharString[0];
    filePathString[renameAtPointer+1] = integerCharString[1];
    filePathString[renameAtPointer+2] = integerCharString[2];
    filePathString[renameAtPointer+3] = integerCharString[3];
    filePathString[renameAtPointer+4] = integerCharString[4];
  }
}