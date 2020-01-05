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

int incomingByte = 0; // for incoming serial data

int commandSet[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int commandIndex = 0;
unsigned long msTicks = 0;
int numFlashes = 0;
unsigned long nextFlash = 0;
unsigned long nextTick = 0;
boolean flashOn = false;

// Defines for Blue toy
//#define GREEN_LED 3
//#define RED_LED 4

// Defines for white toy
#define GREEN_LED 6
#define RED_LED 5
// On the Arduino Nano SDA=A4, SCL=A5
RTClib RTC;
DS3231 Clock;

void setup() {
  // Start the serial port
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Start the I2C interface
  Wire.begin();
  //SetClock();
}

void SetClock()
{
   Clock.setYear(20);
   Clock.setMonth(1);
   Clock.setDate(2);
   Clock.setDoW(4);
   Clock.setHour(19);
   Clock.setMinute(37);
   Clock.setSecond(0);
}

void SerialPrintTime(DateTime now)
{
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
}

void loop() {
  msTicks = millis();
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();
    commandSet[commandIndex] = incomingByte;
    commandIndex++;
    // At this point we recieved 16 bytes, now we process the message
    if (commandIndex == 16)
    {
        // First Check the password
        if (commandSet[0] == 145 && commandSet[1] == 198)
        {
            if (commandSet[2] == 0) // Test mode
            {
                if (commandSet[3] == 1)
                {
                   digitalWrite(GREEN_LED, HIGH);
                }
                else if (commandSet[3] == 2)
                {
                   digitalWrite(GREEN_LED, LOW);
                }
                if (commandSet[4] == 1)
                {
                    digitalWrite(RED_LED, HIGH);
                }
                else if (commandSet[4] == 2)
                {
                    digitalWrite(RED_LED, LOW);
                }
            }
            else if (commandSet[2] == 1) // Set time mode
            {
               Clock.setYear(commandSet[3]);
               Clock.setMonth(commandSet[4]);
               Clock.setDate(commandSet[5]);
               Clock.setDoW(commandSet[6]);
               Clock.setHour(commandSet[7]);
               Clock.setMinute(commandSet[8]);
               Clock.setSecond(commandSet[9]);
               numFlashes = 8;
               flashOn = false;
               nextFlash = msTicks + 100;
            }
            else if (commandSet[2] == 2) // Add a new time condition
            {
                // Time condition format
                // Byte 0-1 (Stored in eeprom as indicator this is set
                // Byte 0: 1=Red, 0=Green, 2=Off
                // Byte 1: Days of week (bits)
                // Byte 1: Start Hour
                // Byte 2: Start Minute
                // Byte 3: End Hour
                // Byte 4: End Minute
            }
        }

        commandIndex = 0;
    }
  }
  if (numFlashes > 0 && nextFlash < msTicks)
  {
      if (flashOn)
      {
         digitalWrite(RED_LED, LOW);
         digitalWrite(GREEN_LED, LOW);
         flashOn = false;
      }
      else
      {
         digitalWrite(RED_LED, HIGH);
         digitalWrite(GREEN_LED, HIGH);
         flashOn = true;
      }
      nextFlash = msTicks + 100;
      numFlashes--;
  }
  if (nextTick < msTicks)
  {
      DateTime now = RTC.now();
      SerialPrintTime(now);
      nextTick = msTicks + 1000;
  }
  /*
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }
    delay(1000);
    DateTime now = RTC.now();
    // For Anthony's bedroom light
    if (now.hour() >= 5 && now.minute() >= 30 && now.hour() <= 8)
    {
       digitalWrite(GREEN_LED, HIGH);
       digitalWrite(RED_LED, LOW);
    }
    else if (now.hour() > 1 && now.hour() < 5 && now.minute() < 30)
    {
       digitalWrite(GREEN_LED, LOW);
       digitalWrite(RED_LED, HIGH);
    }
    else
    {
       digitalWrite(GREEN_LED, LOW);
       digitalWrite(RED_LED, LOW);
    }

    /*
    if (now.hour() >= 3 && now.hour() < 7 || now.hour() == 7 && now.minute() < 30)
    {
        digitalWrite(RED_LED, HIGH);
    }
    else if (now.hour() == 7 && now.minute() >= 30 && now.minute() < 35)
    {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
    }
    else if (now.hour() >= 9 && now.minute() >= 35)
    {
       digitalWrite(RED_LED, LOW);
       digitalWrite(GREEN_LED, LOW);
    }
    */
    
}
