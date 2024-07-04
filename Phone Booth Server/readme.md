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

- Open the boot config file and add the following to enable SPI1

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

- Compile with below or the included make file

```
TBD or use make
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
