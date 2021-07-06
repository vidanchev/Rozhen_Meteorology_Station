/* Blinking a LED continuously */

int led_pin = 9; /* pin to blink */
int dt = 20; /* time step between two levels [ms] */
int step_light = 10; /* light step between two levels [ 0 to 255 ] */ 
int light_val = 0; /* current value of the light */

void setup() {
  // put your setup code here, to run once:
  pinMode( led_pin , OUTPUT ); /* we make led_pin an output */
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //analogWrite( led_pin , led_level ); /* write into led_pin the led_level value */
  
  light_val += step_light; /* we increment the light value with step_light */

/* In case we reached max value (255), reverse the step */
  if( light_val >= 255 ){
    light_val = 255;
    step_light *= -1;
  }
/* In case we reached the min value (0), reverse the step */
  if( light_val <= 0 ){
    light_val = 0;
    step_light *= -1;
  }

  analogWrite( led_pin , light_val ); /* Write the value we obtained */
  delay( dt ); /* Delay so we can see the effect */
}
