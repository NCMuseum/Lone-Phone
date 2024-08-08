# Set up for the RAspberry Pi 5

- Install the latest Raspi OS 32BIT!
- Run sudo raspi-config

```
- enable spi
- enable serial
- disable i2c
- disable 1wire
- disable camera
```

- Open the boot config file and add the following to enable SPI1 to enable the LED Driver

```
sudo nano /boot/firmware/config.txt
add:
dtoverlay=spi1-1cs
save and restart
```

- Update the system

```
sudo apt-get update
sudo apt-get upgrade
```

- Install FTPD

```
sudo apt-get install pure-ftpd
```

- Install the GPIOD Lib Includes

```
sudo apt-get install libgpiod-dev
```

- Compile via make file

```
make
```

# Pin configuration for the RPi5

- SPI CS pin = GPIO-7
- SPI MOSI Pin = GPIO-10
- SPI CLK Pin = GPIO-11
- PIN_DI = GPIO-14
- PIN_RW = GPIO-15
- PIN_E = GPIO-25
- PIN_CS1 = GPIO-23
- PIN_CS2 = GPIO-24
- PIN_RES = GPIO-1

# Hardware Connectivity

![Interface Board](https://github.com/NCMuseum/Lone-Phone/blob/main/Phone%20Booth%20Server/RPI_to_KS0108_InterfaceBoard.png)

# Prerequsites

-  A folder contaning all the PNG Video stream files creating using the PNG Ecnoder tool in this repo. 
-  The file "configFile.txt" tells the app what UDP Message/OSC control is mapped to what Video File and example of one config file entry
```
/state1,/home/user/video_streams/Video1.STREAM,1649;
```
The UDP Message "state1" mapped to OSC Control "/state1" will plau video stream "/home/user/video_streams/Video1.STREAM" which has "1649" frames

# Launching the app manualy or via auto run

TO manualy launch the app
```
./lcdScreenApp IP_ADDRESS_OF_DEVICE CONFIG_FILE_NAME
```
eg
```
./lcdScreenApp 192.168.100.123 configFile.txt
```

# Networking

Server recieves UDP messages (via unicast or broadcast) on port 2222 and re-transmits OSC mesages (via broadcast) on port 5555

# Pixel Layout on the TOP of the Booth

![Phone Booth Top](https://github.com/NCMuseum/Lone-Phone/blob/main/Phone%20Booth%20Server/docs/PhoneBoothTop.png)
