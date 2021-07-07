/*
 Simple Weather Station Firmware - based on Nathan Seidle and Joel Bartlett Weather Shield Example but reduced and prepared for future SD card logging

 Logging pressure, relative humidity and temperature (light sensor voltage as well which can be skipped for the SD logging)
 
 Nathan S. Code: https://learn.sparkfun.com/tutorials/arduino-weather-shield-hookup-guide-v12?_ga=2.195135980.118929541.1625170515-2032379188.1623359155

 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 */

#include <Wire.h> /* I2C library for the sensors */
#include "SparkFunMPL3115A2.h" /* "SparkFun MPL3115" library */
#include "SparkFun_Si7021_Breakout_Library.h" /* "SparkFun Si7021" library */

MPL3115A2 p_sensor; /* Pressure sensor object */
Weather hum_sensor; /* Humidity sensor object */

/* Pin Definitions */
/* ######################################## */
const byte led_blue = 7;
const byte led_green = 8;
const byte ref_3v3 = A3;
const byte light = A1;
/* ######################################## */

/* Global Variables */
long time_tot; /* Count the total passing miliseconds */
int time_step = 1000; /* Set the time step at which data is logged */

void setup()
{
  
  /* Serial monitor used just for debugging */
  Serial.begin( 9600 );
  Serial.println( "Weather Shield Starting ..." );

  pinMode( led_blue , OUTPUT ); /* Blue status LED */
  pinMode( led_green , OUTPUT ); /* Green status LED */
  
  pinMode( ref_3v3 , INPUT ); /* Reference 3.3V comparison */
  pinMode( light , INPUT ); /* light measurement voltage */

  /* Set-up pressure sensor */
  p_sensor.begin( ); /* Initialize Sensor */
  delay( 250 ); /* Initialization delay */
  p_sensor.setModeBarometer( ); /* Set mode to measuring in [Pa] from 20 to 110 [kPa] */
  p_sensor.setOversampleRate( 7 ); /* Set Oversample to the recommended 128 */
  p_sensor.enableEventFlags(); /* Enable all three pressure and temp event flags */

  /* Set-up humidity sensor */
  hum_sensor.begin( ); /* Initialize Sensor */
  delay( 250 ); /* Initialization delay */  

  time_tot = millis( ); /* Read number of miliseconds since board started this program (~50 days before overflow) */

  Serial.println( "Weather Shield online!" );
    
}

void loop()
{
  /* Log readings every time_step number of miliseconds */
  if( millis( ) - time_tot >= time_step ){
    
    digitalWrite( led_blue , HIGH ); /* Start LED blink sequence */

    time_tot += time_step; /* Increment miliseconds by step */

    /* Check Humidity Sensor */
    float hum_val = hum_sensor.getRH( ); /* Measure humidity */

    if( hum_val == 998 ) //Humidty sensor failed to respond if this value was obtained
    {
      Serial.println( "I2C communication to sensors is not working. Check solder connections." ); /* Only good for debugging purposes over USB */

      //Try re-initializing the I2C comm and the sensors
      p_sensor.begin( );
      delay( 250 ); 
      p_sensor.setModeBarometer( );
      p_sensor.setOversampleRate( 7 );
      p_sensor.enableEventFlags( );
      hum_sensor.begin( );
      delay( 250 );
    }
    else
    {

      /* OPTION 1 - Serial Monitor -> uncomment for debugging  */
      /* ##################################################### */
      Serial.print( "At Time = " );
      Serial.print( time_tot );
      Serial.print( " ms, Humidity = " );
      Serial.print( hum_val );
      Serial.print(" [%], Temp = " );
      float temp_val = hum_sensor.getTemp( ); /* Measure temperature */
      Serial.print( temp_val , 2 );
      Serial.print( " [C], Pressure = ");
      float pres_val = p_sensor.readPressure( ); /* Measure pressure */
      Serial.print( pres_val );
      Serial.print(" [Pa], ");
      float light_lvl = get_light_level(); /* Measure light sensor voltage */
      Serial.print( "light sensor val = " );
      Serial.print( light_lvl );
      Serial.println( "[V] " );
      
      /* ##################################################### */

      /* OPTION 2 - SD Card to be added -> uncomment for live action :) */
      /* ##################################################### */

      /* ##################################################### */

    }

    digitalWrite( led_blue , LOW ); /* Finish LED blink sequence */
  }

  delay( 100 ); /* General delay for readings */
  
}

//Returns the voltage of the light sensor based on the 3.3V rail
//This allows us to ignore what VCC might be (an Arduino plugged into USB has VCC of 4.5 to 5.2V)
float get_light_level( ){
  
  float opVoltage = analogRead( ref_3v3 );

  float lightSensor = analogRead( light );

  opVoltage = 3.3/opVoltage; //The reference voltage is 3.3V

  lightSensor = opVoltage*lightSensor;

  return( lightSensor );
}
