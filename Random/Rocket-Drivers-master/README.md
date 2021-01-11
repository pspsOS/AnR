# Rocket Drivers

<p align="center"><img src="resources/RRaPLogo.png" width="900" height="300"></p>

This project contains files written to control the avionics platform for Reusable Rocketry at Purdue's gimbal based Atlas platform. The drivers are also intended for use in the Purdue Space Program's solid rocket program.

> ALL CODE IS PROVIDED AS IS. PLEASE READ THE LICENSE FOR MORE INFORMATION

## Timeline

- [x] Create generic SPI bus functions
- [X] Extract pressure, temperature, and altitude from onboard barometer
- [X] Extract (but not parse) NMEA strings from GPS
- [X] Extract linear accelerometer data
- [ ] Extract 9 axis IMU data

## Intended Hardware

These drivers are intended to be used in conjunction with 4 sensors. These sensors are:

- [MS5607-02BA03
Barometric Pressure Sensor](https://www.te.com/commerce/DocumentDelivery/DDEController?Action=srchrtrv&DocNm=MS5607-02BA03&DocType=Data+Sheet&DocLang=English)
- [FGPMMOPA6H Standalone GPS Module](https://cdn-shop.adafruit.com/datasheets/GlobalTop-FGPMMOPA6H-Datasheet-V0A.pdf)
- [MMA655x Single Axis Inertial Sensor](https://www.nxp.com/docs/en/data-sheet/MMA655x.pdf)
- [ICM-20948 IMU](http://www.invensense.com/wp-content/uploads/2016/06/DS-000189-ICM-20948-v1.3.pdf)

## Notes

The struct `rocket_t` is my default data container. It should be changed to match whatever the intended plan for the rocket is. Doing this is simply easier from my end, so it isn't really a finalized solution. The calculations for the barometer are already included in the code. However, these would be worth going over again and starting to understand what they do and how they work. As for the GPS, the code will only return the NMEA strings GLL, GPRMC, and GPVTG (no sattelite count) unless something else is required. The command to change this frequency is 'PMTK_API_SET_NMEA_OUTPUT' and can be found [here](https://cdn-shop.adafruit.com/datasheets/PMTK_A11.pdf). The code will need to be changed to reflect the changes in output stings, but it shouldn't be too hard with the given code.

> `HandleHALError()` could be a very powerful tool for detecting issues. Since the target platform is unknown, it has been left blank. Please make sure to update this!
