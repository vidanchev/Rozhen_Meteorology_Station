# Rozhen Meteorology Station

This repo contains some code preparation for an Arduino Meteorological station which I was a an advisor for during the [first summer school on space research, technology and applications at Rozhen Observatory, Bulgaria](https://bulgarianspace.online/space-schoolbg-2021-programme/).

All the code is written during practical sessions of the school with purely educational purposes in mind. It is intended for people who have little to no knowledge in Arduino and embedded programming to get some basic knowledge and make a simple meteorological station with COTS sensors.

## Code Examples:
- 1_Blink_LED: Example of blinking a LED - Arduino Hello World program to get started :)
- 2_Continuous_LED: Blinking a LED continuously - using PWM to make the blinking more smooth.
- 3_Reading_Temp: Connecting the [DHT11](https://learn.adafruit.com/dht) sensor, reading temperature and humidity through the DHT.h library.
- 4_Example_FW: Connecting the [Sparkfun Weather Shield](https://learn.sparkfun.com/tutorials/arduino-weather-shield-hookup-guide-v12?_ga=2.195135980.118929541.1625170515-2032379188.1623359155) and writing example firmware based on Nathan Seidle and Joel Bartlett's version (Beerware license).
- 5_Example_SD_Card: Connecting the [SD Card](https://learn.sparkfun.com/tutorials/microsd-breakout-with-level-shifter-hookup-guide?_ga=2.195636972.118929541.1625170515-2032379188.1623359155) and writing simple date (miliseconds elapsed) since the Arduino is online. 