/*
 Simple Weather Station + SD Card Example firmware by Victor Danchev based on Nathan Seidle and Joel Bartlett Weather Shield Example

 Nathan S. Code: https://learn.sparkfun.com/tutorials/arduino-weather-shield-hookup-guide-v12?_ga=2.195135980.118929541.1625170515-2032379188.1623359155
 SD Card Code based on: https://learn.sparkfun.com/tutorials/microsd-breakout-with-level-shifter-hookup-guide?_ga=2.195636972.118929541.1625170515-2032379188.1623359155

 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 */

#include <Wire.h> /* I2C library for the sensors */
#include <SPI.h> /* SPI library for the SD card */
#include <SD.h> /* SD library for the SD card */
#include "SparkFunMPL3115A2.h" /* "SparkFun MPL3115" library */
#include "SparkFun_Si7021_Breakout_Library.h" /* "SparkFun Si7021" library */

File fd; /* Create file pointer */
MPL3115A2 p_sensor; /* Pressure sensor object */
Weather hum_sensor; /* Humidity sensor object */

/* Pin Definitions */
/* ######################################## */
const byte led_blue = 7;
const byte led_green = 8;
const byte chipSelect = 6;
const byte cardDetect = 9;
/* ######################################## */

/* Global Variables */
long time_tot; /* Count the total passing miliseconds */
int time_step = 1000; /* Set the time step at which data is logged */
char fileName[ ] = "log_1.csv"; /* SD library only supports up to 8.3 names -> no spaces and up to 8 chars */

bool alreadyBegan = false;  // SD.begin() misbehaves if not first call

void setup()
{
  
  /* Serial monitor used just for debugging */
  Serial.begin( 9600 );
  Serial.println( "Weather Shield Starting ..." );

  pinMode( led_blue , OUTPUT ); /* Blue status LED */
  pinMode( led_green , OUTPUT ); /* Green status LED */
  pinMode( cardDetect , INPUT ); /* Card detect initialization, we don't need to set the CS pin as output */

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
  
  initializeCard();
  
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
      Serial.println(" [Pa]");
      /* ##################################################### */

      /* OPTION 2 - SD Card -> uncomment for live action :) */
      /* ##################################################### */

      /* Check that the card is still present */
      if( !digitalRead( cardDetect ) ){
        initializeCard();
      }
      
      fd = SD.open( fileName , FILE_WRITE );
      if( fd ){
      
      fd.print( time_tot );
      fd.print( ", " );
      fd.print( hum_val );
      fd.print( ", " );
      fd.print( temp_val );
      fd.print( ", " );
      fd.println( pres_val );

      fd.close( );
      }
      else{
        Serial.println( "Error writing to SD Card!" );
      }
      /* ##################################################### */

    }

    digitalWrite( led_blue , LOW ); /* Finish LED blink sequence */
  }

  delay( 100 ); /* General delay for readings */
  
}


/* Detect the card and open the file */
void initializeCard( void ){

  /* Printout used when connected to USB only */
  Serial.print( F( "Initializing SD card..." ) );

  /* Check if there's a card */
  if( !digitalRead( cardDetect ) ){
    
    Serial.println( F( "No card detected. Waiting for card." ) );
    while ( !digitalRead( cardDetect ) );
    delay( 250 ); // 'Debounce insertion' delay
  }

  /* If we got here, card seems to be in place  
  begin() returns failure even if it worked if it's not the first call. */
  if( !SD.begin( chipSelect ) && !alreadyBegan ){  // begin uses half-speed...
    
    Serial.println( F( "Initialization failed!" ) );
    initializeCard(); // Possible infinite retry loop is as valid as anything
  }
  
  else{
    alreadyBegan = true;
  }
  
  Serial.println( F( "Initialization done." ) );

  Serial.print( fileName );
  if( SD.exists( fileName ) ){
    Serial.println(F( " exists." ) );
  }
  else{
    Serial.println( F( " doesn't exist. Creating." ) );
  }

  Serial.print( "Opening file: " );
  Serial.println( fileName );

  //Serial.println( F( "Enter text to be written to file. 'EOF' will terminate writing." ) );
}
