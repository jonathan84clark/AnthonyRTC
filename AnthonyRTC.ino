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
#include <EEPROM.h>

int incomingByte = 0; // for incoming serial data

int commandSet[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int commandIndex = 0;
unsigned long msTicks = 0;
int numFlashes = 0;
int flashColor = 0;
unsigned long nextFlash = 0;
unsigned long nextTick = 0;
unsigned long flashRate = 100;
boolean flashOn = false;

// Defines for Blue toy
//#define GREEN_LED 3
//#define RED_LED 4

// Defines for white toy
#define GREEN_LED 6
#define RED_LED 5
#define NUM_TIME_EVENTS 10
// On the Arduino Nano SDA=A4, SCL=A5
RTClib RTC;
DS3231 Clock;

struct TimeEvent {
    int active;
    int address;
    byte daysOfWeek;
    byte color;
    int startHour;
    int startMinute;
    int endHour;
    int endMinute;
};

struct TimeEvent timeEvents[NUM_TIME_EVENTS];

void setup() {
  // Start the serial port
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  ReadStoredTimes();

  // Start the I2C interface
  Wire.begin();
}

void ReadStoredTimes()
{
    int eeprom_addr = 0;
    for (int i = 0; i < NUM_TIME_EVENTS; i++)
    {
        timeEvents[i].address = eeprom_addr;
        timeEvents[i].active = EEPROM.read(eeprom_addr++);
        timeEvents[i].daysOfWeek = EEPROM.read(eeprom_addr++);
        timeEvents[i].color = EEPROM.read(eeprom_addr++);
        timeEvents[i].startHour = EEPROM.read(eeprom_addr++);
        timeEvents[i].startMinute = EEPROM.read(eeprom_addr++);
        timeEvents[i].endHour = EEPROM.read(eeprom_addr++);
        timeEvents[i].endMinute = EEPROM.read(eeprom_addr++);
    }
}

void SaveTime(int index)
{
   int eeprom_addr = timeEvents[index].address;
   EEPROM.write(eeprom_addr++, timeEvents[index].active);
   EEPROM.write(eeprom_addr++, timeEvents[index].daysOfWeek);
   EEPROM.write(eeprom_addr++, timeEvents[index].color);
   EEPROM.write(eeprom_addr++, timeEvents[index].startHour);
   EEPROM.write(eeprom_addr++, timeEvents[index].startMinute);
   EEPROM.write(eeprom_addr++, timeEvents[index].endHour);
   EEPROM.write(eeprom_addr++, timeEvents[index].endMinute);
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
               flashColor = 1;
               flashOn = false;
               flashRate = 100;
               nextFlash = msTicks + flashRate;
            }
            else if (commandSet[2] == 2) // Add a new time condition
            {
                int clean_index = -1;
                // Find the index of an unused time slot
                for (int i = 0; i < NUM_TIME_EVENTS; i++)
                {
                    if (timeEvents[i].active == 0)
                    {
                        clean_index = i;
                        break;
                    }
                }
                // No room left on device, so flash 4 times
                if (clean_index == -1)
                {
                    numFlashes = 4;
                    flashColor = 2;
                    flashOn = false;
                    flashRate = 200;
                    nextFlash = msTicks + flashRate;
                }
                else
                {
                    timeEvents[clean_index].active = 1;
                    timeEvents[clean_index].daysOfWeek = commandSet[3];
                    timeEvents[clean_index].color = commandSet[4];
                    timeEvents[clean_index].startHour = commandSet[5];
                    timeEvents[clean_index].startMinute = commandSet[6];
                    timeEvents[clean_index].endHour = commandSet[7];
                    timeEvents[clean_index].endMinute = commandSet[8];
                    numFlashes = 10;
                    flashColor = 1;
                    flashOn = false;
                    flashRate = 100;
                    nextFlash = msTicks + flashRate;
                    SaveTime(clean_index);
                }
            }
            else if (commandSet[2] == 3)
            {
                for (int i = 0 ; i < EEPROM.length() ; i++)
                {
                    EEPROM.write(i, 0);
                }
                ReadStoredTimes();
                numFlashes = 10;
                flashOn = false;
                flashRate = 100;
                nextFlash = msTicks + flashRate;
            }
        }

        commandIndex = 0;
    }
  }
  if (numFlashes > 0 && nextFlash < msTicks)
  {
      if (flashOn)
      {
         if ((flashColor & 0x02) == 0x02)
         {
             digitalWrite(RED_LED, LOW);
         }
         if ((flashColor & 0x01) == 0x01)
         {
             digitalWrite(GREEN_LED, LOW);
         }
         flashOn = false;
      }
      else
      {
         if ((flashColor & 0x02) == 0x02)
         {
            digitalWrite(RED_LED, HIGH);
         }
         if ((flashColor & 0x01) == 0x01)
         {
            digitalWrite(GREEN_LED, HIGH);
         }
         flashOn = true;
      }
      nextFlash = msTicks + flashRate;
      numFlashes--;
  }
  if (nextTick < msTicks)
  {
      DateTime now = RTC.now();
      SerialPrintTime(now);
      byte ledsActive = 0;
      for (int i = 0; i < NUM_TIME_EVENTS; i++)
      {
         if (timeEvents[i].active == 1)
         {
             if (timeEvents[i].startHour <= now.hour() && timeEvents[i].startMinute <= now.minute() &&
                 timeEvents[i].endHour >= now.hour() && timeEvents[i].endMinute >= now.minute())
             {
                ledsActive |= timeEvents[i].color;      
             }
         }
      }
      if ((ledsActive & 0x01) == 0x01)
      {
         digitalWrite(GREEN_LED, HIGH);
      }
      else
      {
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(RED_LED, HIGH);
      }
      /*
      if ((ledsActive & 0x02) == 0x02)
      {
         digitalWrite(RED_LED, HIGH);
      }
      else
      {
         digitalWrite(RED_LED, LOW);
      }
      */
      nextTick = msTicks + 1000;
  }
}
