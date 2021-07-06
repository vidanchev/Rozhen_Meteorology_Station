/* Simple DHT11 reader and serial plotter example */

#include "DHT.h" /* Include DHT library */
#define dht_pin 4 /* Global definition of pin to read data from */
#define DHT_type DHT11 /* Define the name of the sensor */
float temp_C , rel_hum , temp_F; /* Variables to write in */
int dt = 100; /* Delay for loop */

DHT my_DHT( dht_pin , DHT_type ); /* Initialize DHT sensor object with pin and type as above */

void setup() {
  // put your setup code here, to run once:
  Serial.begin( 9600 ); /* Open serial channel through USB at 9600 bits per second */
  my_DHT.begin( );
  delay( 500 );
}

void loop() {
  // put your main code here, to run repeatedly:
  temp_C = my_DHT.readTemperature( ); /* Default is Celsius, put "true" for F */
  rel_hum = my_DHT.readHumidity( ); /* Read the humidity (output is in %) */
  temp_F = my_DHT.readTemperature( true ); /* Read the remperature in F */

  /* Print values in serial monitor */
  Serial.print( "Temp = " );
  Serial.print( temp_C );
  Serial.print( " [C], or " );
  Serial.print( temp_F );
  Serial.print( " [F], Humidity = " );
  Serial.print( rel_hum );
  Serial.println( " [%]" );


  /* Plotting for Serial Monitor */
  /*
  Serial.print( "Temp = " );
  Serial.println( temp_C );
  Serial.print( ", Hum = " );
  Serial.println( rel_hum );
  */

  delay( dt );
  
}
