/*
   16 button remote for testing UDP message conversionf or lone phone booth

*/
#include <ESP8266WiFi.h>
#include "Arduino.h"
#include <WiFiUdp.h>

//Networking
const char *ssid = "";
const char *password = "";
IPAddress remoteAddress = IPAddress(192,168,15,102);
const unsigned short int txPort = 5555;
WiFiUDP udp;

const byte numOfPins = 2, numberOfInputs=4, numOfButtons=16;
const byte rowMultiplexer[numOfPins] = {D1, D2};
const byte colMultiplexer[numOfPins] = {D4, D3};
const byte inputPin = D0;
const byte addressArray[numberOfInputs][numOfPins] = {{0,0},{0,1},{1,0},{1,1}};

byte buttonArray[numOfButtons] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
byte previousButtonArray[numOfButtons] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

unsigned short int cnt=0, inCounter=0, innerCounter=0, inputCounter=0, inputResult=0;
unsigned long timers[2];

//Scene strings to send
const byte numberOfScenes = 10;
const char* sceneStrings[numberOfScenes] = {"state1","state2","state3","state4","state5","state6","state7","state8","state9","state10"};


void setup() 
{
  byte sceneCounter=0;
  
  Serial.begin(115200);
  Serial.println("\r\n\r\n\r\n");
  
  //Eable WIFI
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
        delay(100);
        Serial.print(".");
  }
  Serial.printf("\r\nCurrent Ip Address:\t");
  Serial.print(WiFi.localIP());
  Serial.print("\r\n");
  
  
  //set up Multiplexer Address Pins
  for(cnt=0; cnt<numOfPins; cnt++)
  {
    pinMode(rowMultiplexer[cnt], OUTPUT);
    pinMode(colMultiplexer[cnt], OUTPUT);
    digitalWrite(rowMultiplexer[cnt], LOW);
    digitalWrite(colMultiplexer[cnt], LOW);
  }
  //Set up Input Pin
  pinMode(inputPin, INPUT);

  for(sceneCounter=0; sceneCounter<numberOfScenes; sceneCounter++)
  {
    Serial.printf("\r\nState\t%d\t[%s]\tLength\t[%d]", sceneCounter, sceneStrings[sceneCounter], strlen(sceneStrings[sceneCounter]));
  }
}

void loop()
{
  if( readButtons() == 1)
  {
    //Button pushed start timer
    timers[0] = micros();
    while( readButtons() == 1)
    {
      //Wait untill buttnos have been released      
      memcpy( &previousButtonArray, &buttonArray, numOfButtons );
      yield();
    }
    //Button Released stop timer
    timers[1] = micros();
    if( timers[1]-timers[0] < 10000 )
    {
       //Serial.printf("Bounce - NOTHING SENT....\r\n");
    }
    else
    {
      for(cnt=0; cnt<numberOfScenes; cnt++)
      {
        if(previousButtonArray[cnt]==1)
        {
          udp.beginPacket(remoteAddress, txPort);
          udp.write(sceneStrings[cnt], strlen(sceneStrings[cnt]));
          udp.endPacket();
        }
      }
      
      for(cnt=0; cnt<numOfButtons; cnt++)
      {
        Serial.printf("%d\t", previousButtonArray[cnt]);
      }
      
      Serial.printf("\r\n");
    }
  }
  yield();  
 
}

byte readButtons()
{
  inputResult=0;
  inputCounter=0;
  
  for(inCounter=0; inCounter<numberOfInputs; inCounter++)
  {
      //Select Row
      setRowAddress(inCounter);
      for(innerCounter=0; innerCounter<numberOfInputs; innerCounter++)
      {
        //Select Column and Read
        setColAddress(innerCounter);
        buttonArray[inputCounter] = digitalRead(inputPin);
        if(buttonArray[inputCounter]==1){inputResult++;}
        inputCounter++;
      }
  }
  if(inputResult)
  {
    return 1;
  }
  return 2;
}

void setRowAddress(byte multiplexerAddress)
{
  for(cnt=0; cnt<numOfPins; cnt++)
  {
    digitalWrite(rowMultiplexer[cnt], addressArray[multiplexerAddress][cnt]);
  }

}

void setColAddress(byte multiplexerAddress)
{
  for(cnt=0; cnt<numOfPins; cnt++)
  {
    digitalWrite(colMultiplexer[cnt], addressArray[multiplexerAddress][cnt]);
  }

}
