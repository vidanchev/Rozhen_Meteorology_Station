/* Blinking a LED - Arduino Hello World :) */

int led_pin = 8; /* pin to blink */
int dt = 1000; /* delay in [ms] */

void setup() {
  // put your setup code here, to run once:
  pinMode( led_pin , OUTPUT ); /* we make led_pin an output */
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite( led_pin , HIGH ); /* write into led_pin the "HIGH" value */
  delay( dt ); /* Delay between HIGH and LOW */
  digitalWrite( led_pin , LOW ); /* write into led_pin the "LOW" value */
  delay( dt ); /* Delay between LOW and HIGH */
}
