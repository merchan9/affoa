/*
  Blink without Delay

  Turns on and off a light emitting diode (LED) connected to a digital pin,
  without using the delay() function. This means that other code can run at the
  same time without being interrupted by the LED code.

  The circuit:
  - Use the onboard LED.
  - Note: Most Arduinos have an on-board LED you can control. On the UNO, MEGA
    and ZERO it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN
    is set to the correct LED pin independent of which board is used.
    If you want to know what pin the on-board LED is connected to on your
    Arduino model, check the Technical Specs of your board at:
    https://www.arduino.cc/en/Main/Products

  created 2005
  by David A. Mellis
  modified 8 Feb 2010
  by Paul Stoffregen
  modified 11 Nov 2013
  by Scott Fitzgerald
  modified 9 Jan 2017
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/BlinkWithoutDelay

  Edited by David Merchan 9/24/18
*/
#include <Wire.h> // Must include Wire library for I2C
#include <SparkFun_MMA8452Q.h> // Includes the SFE_MMA8452Q library

// Begin using the library by creating an instance of the MMA8452Q
//  class. We'll call it "accel". That's what we'll reference from
//  here on out.
MMA8452Q accel;

// constants won't change. Used here to set a pin number:
const int ledPin =  LED_BUILTIN;// the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

long relIntervals[ ] = {50,100,15,1200};  // relative intervals between toggles for one cycle

/* Other blinks:
 *  1/2 duty {1,1}
 *  1/10 duty {1,9}
 *  breathe {2,1,1}
 *  heartbeat {50,100,15,1200}
 */

int i = 0; // initializing index for intervals[ ]
int n = sizeof(relIntervals)/sizeof(relIntervals[0]);

// scales relative intervals to a desired frequency in Hz
void scaleIntervals (long interval[ ], float frequency){
  int total = 0;
  for (int j = 0; j < n; j++){
    total += interval[j];
  }
  for (int j = 0; j < n; j++){
    interval[j] = interval[j]*1000.0/(total*frequency);
  }
}

void setup() {
  Serial.begin(9600);
  accel.init();
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  scaleIntervals(relIntervals, 1);
}

void loop() {
  // check to see if it's time to blink the LED; that is, if the difference
  // between the current time and last time you blinked the LED is bigger than
  // the interval at which you want to blink the LED.
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= relIntervals[i]) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    
    // set the LED with the ledState:
    digitalWrite(ledPin, ledState);

    // toggle ledState:
    ledState = !ledState;

    // increment the index, up to the length of the array
    i = (i+1) % n;
  }

  if (accel.available()){
    accel.read();
    Serial.print(accel.cx, 3);
    Serial.print(";");
    Serial.print(accel.cy, 3);
    Serial.print(";");
    Serial.println(accel.cz, 3);
  }
}
