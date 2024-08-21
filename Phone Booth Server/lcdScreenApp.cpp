/*
g++ -o lcdScreenApp lcdScreenApp.cpp KS0108X3_RPI_Driver.cpp osc.cpp apa102LEDStrip.cpp colourObject.cpp -VBitmap.cpp -lgpiod -fpermissive -pthread

Launch "./lcdScreenApp IP_ADDRESS configFile"

autostart 
	sudo nano /etc/rc.local
*/

#include "KS0108X3_RPI_Driver.h"
#include <math.h>
#include <stdint.h>
#include <string.h>
#include <sys/time.h>
#include <cmath>
#include <cstdio>
#include "osc.h"
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <threads.h>
#include <stdlib.h>
#include <getopt.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include "colourObject.h"
#include "apa102LEDStrip.h"
#include "VBitmap.h"


//LCD Screen
KS0108X3_RPI_Driver lcdScreen;
uint8_t WIDTH = 192, HEIGHT = 64;

//OSC Stuff
osc oscObject;

//OSC to Video mapping array
unsigned long configSize=0;
typedef struct contentMapping{ 
  char* controllName;
  char* contentName;
  unsigned short int numberOfFrames;
}contentMapping;
contentMapping* contentIndex;
unsigned short int currentContentIndex=0, contentFlag=0;

//network stuff
typedef struct networkSystem{
	const unsigned short int maxBufferSize=1024;
	char *buf = new char[maxBufferSize];
    struct sockaddr_in si_me;
	struct sockaddr_in si_other;
    int s, i;
	socklen_t slen = sizeof(sockaddr_in);
	in_addr_t localAddress;
	struct timeval read_timeout;
	int currentPacketLength=0;
}networkSystem;
networkSystem localNetwork;
unsigned short int udpRXPort = 2222;
unsigned short int oscTXPort = 5556;
sockaddr_in broadcastOSCAddress;
int broadcastEnable=1;

//colour system
const char maxValue = 255;
char colourList[7*3]={maxValue,0,0, maxValue,maxValue,0, 0,maxValue,0, 0,maxValue,maxValue, 0,0,maxValue, maxValue,0,maxValue, maxValue,maxValue,maxValue};
colourObject primaryCO(128, 7, colourList);
uint8_t tmpColour [3] = {0,0,0};

//LEDs
unsigned short int numLeds = (4*20)+(4*31);
apa102LEDStrip leds;
const unsigned short int pixelWidth = 104, pixelHeight = 4;
short int pixelMap[pixelHeight][pixelWidth] = {{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,-1},{-1,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};
VBitmap virtualPixels(pixelWidth, pixelHeight, 3);

//SPI for LEDS
int SPI1;
char *SPI1Device = "/dev/spidev1.0";
uint8_t spiMode;
uint8_t bits = 8;
uint32_t speed = 2000000;


void playStream(char* streamFile, unsigned short int totalFrames);
short int findNeedleCount(char* haystack, unsigned short int hayStackLength, char needle, unsigned short needleCount);
short int countNeedles(char* haystack, unsigned short int hayStackLength, char needle);
uint8_t loadConfiguration(char* configFilePath);
void *handleUDP(void *args_);
void *ledOutput(void *args_);
void initSPI(char* deviceName);	
void tx_via_spidev(char* ledData, unsigned long int blockSize);

int main(int argc, char **argv)
{
	unsigned short int yIndex=0, xIndex=0;
	unsigned short int contentCounter=0;
	unsigned short int rc=0;
	thrd_t udpServerThread;
	thrd_t LEDOutputThread;
	
	//load osc to content mappigs from config file
	loadConfiguration(argv[2]);
	
	if(lcdScreen.online()!=0)
	{
		printf("\r\n\t\tError!\r\n");
		return 1;
	}
	//Init the secondary SPIdev bus
	initSPI(SPI1Device);
	//Init the LEDs
	leds.init(numLeds, 4, 255);
	//Pixel Map to the VideoFrame
	virtualPixels.maskMap = new short int*[pixelHeight];
	for(yIndex=0; yIndex<pixelHeight; yIndex++)
	{
		virtualPixels.maskMap[yIndex] = new short int[pixelWidth];
		for(xIndex=0; xIndex<pixelWidth; xIndex++)
		{
			virtualPixels.maskMap[yIndex][xIndex] = pixelMap[yIndex][xIndex];
		}
	}
	virtualPixels.assignMapToLEDArray(virtualPixels.maskMap, &leds);
	virtualPixels.clear();
	
	//init network
	localNetwork.localAddress = inet_addr(argv[1]);
	localNetwork.read_timeout.tv_sec = 0;
	localNetwork.read_timeout.tv_usec = 10;
	localNetwork.s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//Enable Broadcasting on this socket
	setsockopt(localNetwork.s, SOL_SOCKET, SO_BROADCAST, &broadcastEnable, sizeof(broadcastEnable));
	if(localNetwork.s==-1)
	{
      printf("socket error\r\n");
	  return 1;
	}
	else
	{
		printf("\r\nSocket[%d]", localNetwork.s);
	}
    localNetwork.si_me.sin_family = AF_INET;
    localNetwork.si_me.sin_port = htons(udpRXPort);
    localNetwork.si_me.sin_addr.s_addr = localNetwork.localAddress;
	//Bind to IP address
	rc = bind(localNetwork.s, (struct sockaddr*)&localNetwork.si_me, sizeof(localNetwork.si_me));
	if(rc==-1)
	{
        printf("\r\nBind issue\r\n");
		return 1;
	}
	else
	{
		printf("\r\nNetwork is up at [%s][%d]", argv[1], rc);
		ioctl(localNetwork.s,FIONBIO,true);
	}
	
	//set up the broadcast address for OSC retransmition
	broadcastOSCAddress.sin_family = AF_INET;
	broadcastOSCAddress.sin_port = htons(oscTXPort);
	broadcastOSCAddress.sin_addr.s_addr = inet_addr("192.168.15.255");
	
	//create the UDP Server Thread to handle incoming OSC data
	thrd_create(&udpServerThread, (thrd_start_t)handleUDP, (void *)contentCounter);
	
	//Create the LED driver Thread
	thrd_create(&LEDOutputThread, (thrd_start_t)ledOutput, (void *)contentCounter);
	
	while(1)
	{
		contentFlag=0;
		playStream(contentIndex[currentContentIndex].contentName, contentIndex[currentContentIndex].numberOfFrames);
	}	
	
	close(localNetwork.s);
	return 0;
}

void *ledOutput(void *args_) 
{
	unsigned short int cIndex=0, pixCounter=0;
	while(1)
	{
		switch(currentContentIndex)
		{
			case	0:		cIndex=0;
							break;
			case	1:		cIndex=maxValue;
							break;
			case	2:		cIndex=maxValue+10;
							break;
			case	3:		cIndex=maxValue+20;
							break;
			case	4:		cIndex=maxValue+30;
							break;	
			case	5:		cIndex=maxValue+40;
							break;	
			case	6:		cIndex=maxValue+50;
							break;	
			case	7:		cIndex=maxValue+60;
							break;	
			case	8:		cIndex=maxValue+70;
							break;	
			case	9:		cIndex=maxValue+80;
							break;	
		}
		//Grab Colour
		primaryCO.getColour(cIndex%primaryCO._bandWidth, (char*)tmpColour);
		//Draw Vertical Line
		virtualPixels.drawVLine(pixCounter%pixelWidth, 0, pixelHeight, tmpColour);
		//Render pixelmap
		leds.renderFrame();
		tx_via_spidev((char*)leds.SPIFrame, leds._frameLength);
		//Fade video frame
		virtualPixels.subtractiveFade(10);
		pixCounter++;
		usleep(5000);
	}
}

void *handleUDP(void *args_) 
{
    unsigned short int contentCounter=0;
	short int oscTXCounter=0;
	const uint8_t maxInputPacketLength = 100;
	char* inputData = new char[maxInputPacketLength];
	char* controlName = new char[maxInputPacketLength];
	
	memset(inputData, 0, maxInputPacketLength);
	memset(controlName, 0, maxInputPacketLength);
	
	while(1)
	{
		//handle UDP reception here
		ioctl(localNetwork.s,FIONREAD,&localNetwork.currentPacketLength);
		if(localNetwork.currentPacketLength>0)
		{
			//clear reception buffer
			memset(localNetwork.buf, 0, localNetwork.maxBufferSize);
			//Receieve data
			recvfrom(localNetwork.s, localNetwork.buf, localNetwork.currentPacketLength, 0, (struct sockaddr*)&localNetwork.si_me, &localNetwork.slen);
			//check that data rcecievd is smaller tan the max data buffer size
			if(localNetwork.currentPacketLength<maxInputPacketLength)
			{
				//store in local buffer
				memset(inputData, 0, maxInputPacketLength);
				memcpy(inputData, localNetwork.buf, localNetwork.currentPacketLength);
				//check if this packet matches a configured OSC control label
				for(contentCounter=0; contentCounter<configSize; contentCounter++)
				{
					//Store this indexes OSC control name into local buffer withthout the '/'
					memset(controlName, 0, maxInputPacketLength);
					memcpy(controlName, contentIndex[contentCounter].controllName+1, strlen(contentIndex[contentCounter].controllName));
					if(strcmp(inputData, controlName)==0)
					{
						printf("\r\nCurrent control[%s][%s]\tIndex[%d]\tMATCH[%d]\n", contentIndex[contentCounter].controllName, controlName, contentCounter, strcmp(inputData, controlName) );
						//Controll matches retransmit as OSC
						oscObject.setValue(contentIndex[contentCounter].controllName, 1);
						oscObject.generateOSCPacket(contentIndex[contentCounter].controllName);
						oscTXCounter = sendto(localNetwork.s, oscObject.txPacketBuffer, oscObject.txPacketBufferLength, 0, (struct sockaddr*)&broadcastOSCAddress, sizeof(broadcastOSCAddress));
						printf("\r\nSent OSC\t[%d]\n", oscTXCounter);
						oscObject.setValue(contentIndex[contentCounter].controllName, 0);
						//Set global content index
						currentContentIndex = contentCounter;
						contentFlag=1;
						break;
					}
				}
			}
		}
	}
}

uint8_t loadConfiguration(char* configFilePath)
{
	FILE* configurationFileStream = fopen(configFilePath,"r");
	char* inputBuffer;
	unsigned long fileSize=0, contentCounter=0;
	unsigned short int startSeek=0, endSeek=0, seekIndex=0, seekLine=0, seekSize=0;
	char* tempFrameCounter;
	
	if(configurationFileStream == NULL)
	{
		printf("\r\nCan not open config file...\r\n\r\n");
		return 0;
	}
	else
	{
		//Get file size
		fseek(configurationFileStream, 0L, SEEK_END);
		fileSize = ftell(configurationFileStream);
		rewind(configurationFileStream);
		printf("\r\nConfig file loaded its size is\t[%d]", fileSize);
		inputBuffer = new char[fileSize];
		fread(inputBuffer, (size_t)fileSize, 1, configurationFileStream);
		//count number of entries in the config file by counting the ';'
		configSize = countNeedles(inputBuffer, fileSize, ';');
		printf("\r\n\tFound\t[%d]\tconfiguration lines..", configSize);
		
		//Set up the global index arrays
		contentIndex = new contentMapping[configSize];
		//read config file into index
		startSeek=0;
		seekIndex=1;
		seekLine=1;
		for(contentCounter=0; contentCounter<configSize; contentCounter++)
		{
			//OSC Control Name
			endSeek = findNeedleCount(inputBuffer, fileSize, ',', seekIndex);
			//work out the size of the OSC control name
			seekSize = (endSeek-startSeek)+1;		//always +1 for char termintaion string 
			//create new char string in index
			contentIndex[contentCounter].controllName = new char[seekSize];
			memcpy(contentIndex[contentCounter].controllName, inputBuffer+startSeek, seekSize-1);
			contentIndex[contentCounter].controllName[seekSize-1]=0;
			startSeek=endSeek+1;
			seekIndex++;
			
			//Content name/location
			endSeek = findNeedleCount(inputBuffer, fileSize, ',', seekIndex);
			//work out the size of the content name
			seekSize = (endSeek-startSeek)+1;		//always +1 for char termintaion string 
			//create new char string in index
			contentIndex[contentCounter].contentName = new char[seekSize];
			memcpy(contentIndex[contentCounter].contentName, inputBuffer+startSeek, seekSize-1);
			contentIndex[contentCounter].contentName[seekSize-1]=0;
			startSeek=endSeek+1;
			seekIndex++;
			
			//Number of frames for this content file
			endSeek = findNeedleCount(inputBuffer, fileSize, ';', seekLine);
			//work out the size of the content name
			seekSize = (endSeek-startSeek)+1;		//always +1 for char termintaion string 
			//create new char string in index
			tempFrameCounter = new char[seekSize];
			memcpy(tempFrameCounter, inputBuffer+startSeek, seekSize-1);
			tempFrameCounter[seekSize-1]=0;
			contentIndex[contentCounter].numberOfFrames=atoi(tempFrameCounter);
			delete tempFrameCounter;
			startSeek=findNeedleCount(inputBuffer, fileSize, '\n', seekLine)+1;
			seekLine++;
		}
		
		for(contentCounter=0; contentCounter<configSize; contentCounter++)
		{
			printf("\r\nItem\t[%d][%s][%s][%d]", contentCounter, contentIndex[contentCounter].controllName, contentIndex[contentCounter].contentName, contentIndex[contentCounter].numberOfFrames);
		}
		printf("\r\n");
		
		//set up OSC Mappings
		for(contentCounter=0; contentCounter<configSize; contentCounter++)
		{
			oscObject.addControll(contentIndex[contentCounter].controllName,1,'f', 10000);
			oscObject.timeOutControll(contentIndex[contentCounter].controllName);
		}
		
		fclose(configurationFileStream);
		return 1;
	}
}

void playStream(char* streamFile, unsigned short int totalFrames)
{
	unsigned short int frameIndex=0, bufferIndex=0;
	char* frameBuffer = new char[HEIGHT*WIDTH*3];
	unsigned short int pixelValue=0;
	unsigned short int colourLevelFilter = 200;
	unsigned short int xCount, yCount;
	uint8_t tempColour[3] = {0,0,0};
	FILE* outputStream;

    outputStream = fopen(streamFile, "r");
	for(frameIndex=0; frameIndex<totalFrames; frameIndex++)
    {
      fread(frameBuffer, 1, HEIGHT*WIDTH*3, outputStream);
	  for(yCount=0; yCount<HEIGHT; yCount++)
      {
        for(xCount=0; xCount<WIDTH; xCount++)
        {
          memcpy(tempColour, frameBuffer+bufferIndex, 3);
          pixelValue = tempColour[0]+tempColour[1]+tempColour[2];
          if(pixelValue>colourLevelFilter)
          {
			lcdScreen.drawPixel(xCount, yCount, 1);
          }
          else
          {
            lcdScreen.drawPixel(xCount, yCount, 0);
          }
          bufferIndex+=3;
        }
      }
	  if(contentFlag==1)
	  {
		  break;
	  }
      bufferIndex=0;
      lcdScreen.writeToDisplay();
	  lcdScreen.clearDisplay();
    }
	fclose(outputStream);
	delete frameBuffer;
}

short int findNeedleCount(char* haystack, unsigned short int hayStackLength, char needle, unsigned short needleCount)
{
  unsigned short int hayCount=0, nCount=0;
  for(hayCount=0; hayCount<hayStackLength; hayCount++)
  {
    if(haystack[hayCount]==needle)
    {
      nCount++;
      if(nCount==needleCount)
      {
        return hayCount;
      }
    }
  }
  return -1;
}
short int countNeedles(char* haystack, unsigned short int hayStackLength, char needle)
{
  unsigned short int found=0;
  unsigned short int hayCount=0;
  
  for(hayCount=0; hayCount<hayStackLength; hayCount++)
  {
    if(haystack[hayCount]==needle)
    {
      found++;
    }
  }
  return found;
}

void initSPI(char* deviceName)
{
	SPI1 = open(deviceName, O_RDWR);
	if(SPI1<0){printf("\r\nSPI DEVICE FAILED TO OPEN\r\n");}
	spiMode = SPI_MODE_0;
	//Set SPI Mode 
	ioctl(SPI1, SPI_IOC_WR_MODE, &spiMode);
	if( ioctl(SPI1, SPI_IOC_RD_MODE, &spiMode)==-1 ){printf("\r\nMODE NOT SET\r\n");}
	//set Bits Per Word
	ioctl(SPI1, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if( ioctl(SPI1, SPI_IOC_RD_BITS_PER_WORD, &bits)==-1 ){printf("\r\nBITS NOT SET\r\n");}
	//Set SPI Speed
	ioctl(SPI1, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if( ioctl(SPI1, SPI_IOC_RD_MAX_SPEED_HZ, &speed)==-1 ){printf("\r\nSPEED NOT SET\r\n");}
}

void tx_via_spidev(char* ledData, unsigned long int blockSize)
 {
	struct spi_ioc_transfer spiDataTransferBlock;
	uint8_t dataBlock[blockSize];
	uint8_t returnBuffer;
	
	memcpy(dataBlock, ledData, blockSize);
	memset(&spiDataTransferBlock, 0, sizeof(spiDataTransferBlock));
	spiDataTransferBlock.tx_buf = (unsigned long)dataBlock;
	spiDataTransferBlock.rx_buf = (unsigned long)returnBuffer;
	spiDataTransferBlock.len = blockSize;
	spiDataTransferBlock.delay_usecs = 0;
	spiDataTransferBlock.speed_hz = speed;
	spiDataTransferBlock.bits_per_word = bits;
	
	ioctl(SPI1, SPI_IOC_MESSAGE(1), &spiDataTransferBlock);
	
 }