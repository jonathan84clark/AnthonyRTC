/*****************************************************
* ANTHONY RTC
* DESC: The Anthony RTC is a clock designed to turn a light
* on at different times to indicate wakeup time and breakfast
* time.
* Author: Jonathan L Clark
* Date: 12/12/2019
*****************************************************/

#include "DS3231.h"
#include <Wire.h>

// On the Arduino Nano SDA=A4, SCL=A5
RTClib RTC;
//DS3231 Clock;

void setup() {
  // Start the serial port
  Serial.begin(9600);

  // Start the I2C interface
  Wire.begin();
  //SetClock();
}

void SetClock()
{
  /*
   Clock.setYear(19);
   Clock.setMonth(12);
   Clock.setDate(12);
   Clock.setDoW(5);
   Clock.setHour(21);
   Clock.setMinute(31);
   Clock.setSecond(0);
   */
}
void loop() {
    delay(1000);
  
    DateTime now = RTC.now();
    
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    /*
    Serial.print(" since midnight 1/1/1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");
    */

}
