/* A simple file logger that logs the miliseconds elapsed since the Arduino was powered on */

/* Based on example code from https://learn.sparkfun.com/tutorials/microsd-breakout-with-level-shifter-hookup-guide?_ga=2.195636972.118929541.1625170515-2032379188.1623359155 */

#include <SPI.h>
#include <SD.h>

File fd;

char fileName[] = "demo_1.txt"; /* SD library only supports up to 8.3 names -> up to 8 symbols and no spaces */

const uint8_t chipSelect = 5;
const uint8_t cardDetect = 9;

bool alreadyBegan = false;  // SD.begin() misbehaves if not first call
long timeNow; /* Counter for miliseconds -> long can keep up to ~50 days */

void setup()
{
  Serial.begin( 9600 );

  // Note: To satisfy the AVR SPI gods the SD library takes care of setting
  // SS_PIN as an output. We don't need to.
  pinMode( cardDetect , INPUT );

  initializeCard( );

  timeNow = millis( ); /* Count the milisecs since we started the program */
}

void loop()
{
  // Make sure the card is still present
  if ( !digitalRead( cardDetect ) ){
    initializeCard( );
  }

  /* Write the msec elapsed since the Arduino came on */
  fd = SD.open( fileName , FILE_WRITE );

  if( fd ){

    timeNow = millis( );
    fd.print( "Time Elapsed since Arduino came on is: " );
    fd.print( timeNow );
    fd.println( " [miliseconds]" );
    
    fd.close( );
  }
  else{
    Serial.print( "Error opening " );
    Serial.println( fileName );
  }
  
  delay( 500 );
  
}

/* Detect and initialize the SD Card */
void initializeCard( void ){
  
  Serial.print( F( "Initializing SD card..." ) );

  // Is there even a card?
  if ( !digitalRead( cardDetect ) ){
    
    Serial.println( F( "No card detected. Waiting for card." ) );
    while ( !digitalRead( cardDetect ) );
      delay( 250 ); // 'Debounce insertion'
  }

  // Card seems to exist.  begin() returns failure
  // even if it worked if it's not the first call.
  if( !SD.begin( chipSelect ) && !alreadyBegan ){
    Serial.println( F( "Initialization failed!" ) );
    initializeCard( ); // Possible infinite retry loop is as valid as anything
  }
  else{
    alreadyBegan = true;
  }
  Serial.println( F( "Initialization done." ) );

  Serial.print( fileName );
  if( SD.exists( fileName ) ){
    Serial.println( F( " exists." ) );
  }
  else{
    Serial.println( F( " doesn't exist. Creating." ) );
  }

  Serial.print( "Opening file: " );
  Serial.println( fileName );

}
