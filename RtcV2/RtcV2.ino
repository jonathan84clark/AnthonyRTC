/********************************************************************
* RTC V2
* DESC: This is the second iteration of Anthony's RTC. This one being more advanced,
* with a WiFi interface. This device is able to automatically update for daylight savings
* time. It is also capable of providing a web interface for setting wakeup times. 
* Install an NTP client from
* https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
* Author: Jonathan L Clark
* Date: 10/22/2020
********************************************************************/
#include <DHT.h>
#include <DHT_U.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "DS3231.h"
#include <Wire.h>

#define DHTTYPE DHT11   // DHT 11
#define DHTPIN 13     // Digital pin connected to the DHT sensor

#define BRIGHT_GREEN_LEDS 12
#define BRIGHT_RED_LEDS   14
#define DIM_GREEN         10
#define DIM_YELLOW         9
#define DIM_RED           15

#define RED_BRIGHTNESSS 170
#define GREEN_BRIGHTNESSS 180

struct TimeFrame
{
   int startHour;
   int startMinute;
   int endHour;
   int endMinute;
   int greenMinutes;
};

struct TimeCache
{
   int year;
   int month;
   int day;
   int hour;
   int minute;
   int second;
};

float temperature;
float humidity;

#ifndef STASSID
#define STASSID "JLC1"
#define STAPSK  "1990clark$fam1984"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;
bool offlineMode = false;
unsigned long msTicks = 0;
unsigned long nextTick = 0;
long set_rtc_time = -1;

TimeCache cachedTime;
RTClib RTC;
DS3231 Clock;
TimeFrame sleepWakeupTime;
DHT dht(DHTPIN, DHTTYPE);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
ESP8266WebServer server(80);

// Set your Static IP address
IPAddress local_IP(192, 168, 1, 190);
// Set your Gateway IP address
IPAddress gateway(192, 168, 1, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   //optional
IPAddress secondaryDNS(8, 8, 4, 4); //optional

const int led = LED_BUILTIN;

const String postForms = "<html>\
  <head>\
    <title>Anthony Real-Time Clock</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Set Time</h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postTime/\">\
      <p>Start Red</p>\
      <select name=\"startTime\" id=\"startRedTime\">\
         <option value=\"-1\">----</option>\
         <option value=\"1800\">6:00 PM</option>\
         <option value=\"1900\">7:00 PM</option>\
         <option value=\"1915\">7:15 PM</option>\
         <option value=\"1930\">7:30 PM</option>\
      </select>\
      <p>End Red</p>\
      <select name=\"endTime\" id=\"startGreenTime\">\
         <option value=\"-1\">----</option>\
         <option value=\"500\">5:00 AM</option>\
         <option value=\"530\">5:30 AM</option>\
         <option value=\"600\">6:00 AM</option>\
         <option value=\"630\">6:30 AM</option>\
         <option value=\"700\">7:00 AM</option>\
         <option value=\"730\">7:30 AM</option>\
         <option value=\"800\">8:00 AM</option>\
         <option value=\"830\">8:30 AM</option>\
         <option value=\"900\">9:00 AM</option>\
      </select>\
      <p>Green Time</p>\
      <select name=\"greenTime\" id=\"greenTime\">\
         <option value=\"-1\">---</option>\
         <option value=\"10\">10 min</option>\
         <option value=\"20\">20 min</option>\
         <option value=\"30\">30 min</option>\
         <option value=\"40\">40 min</option>\
         <option value=\"50\">50 min</option>\
         <option value=\"60\">60 min</option>\
      </select>\
      <br><input type=\"submit\" value=\"Set\">\
    </form>\
    <h1>Update RTC Time</h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/updateRtc/\">\
      <input type=\"submit\" value=\"Update RTC\">\
    </form>\
  </body>\
</html>";

/*******************************************************
* SAVES THE START AND END TIMES
*******************************************************/
void SaveTimes()
{
   int address = 0;
   EEPROM.write(address++, sleepWakeupTime.startHour); 
   EEPROM.write(address++, sleepWakeupTime.startMinute);
   EEPROM.write(address++, sleepWakeupTime.endHour);
   EEPROM.write(address++, sleepWakeupTime.endMinute);
   EEPROM.write(address++, sleepWakeupTime.greenMinutes);
   EEPROM.commit();
   Serial.println("Times saved to EEProm");
}

/*******************************************************
* READS THE TIMEFRAMES FROM THE EEPROM
*******************************************************/
void ReadTimes()
{
   int eeprom_addr = 0;
   sleepWakeupTime.startHour = EEPROM.read(eeprom_addr++);
   sleepWakeupTime.startMinute = EEPROM.read(eeprom_addr++);
   sleepWakeupTime.endHour = EEPROM.read(eeprom_addr++);
   sleepWakeupTime.endMinute = EEPROM.read(eeprom_addr++);
   sleepWakeupTime.greenMinutes = EEPROM.read(eeprom_addr++);
}

void handleRoot() {
  //digitalWrite(led, 1);
  server.send(200, "text/html", postForms);
  //digitalWrite(led, 0);
}

/*******************************************************
* LEADING ZERO
* DESC: Produces a string with a leading zero based on an int
* if that int is less than 10.
*******************************************************/
String leadingZero(int input)
{
   String output = String(input);
   if (input < 10)
   {
      output = "0" + output;
   }
   return output;
}

/*******************************************************
* GET DATA
* DESC: Produces system data.
*******************************************************/
void updateRTC()
{
   server.send(405, "text/plain", "OK");
   set_rtc_time = millis() + 1000; // Do this in a second
}

/*******************************************************
* GET DATA
* DESC: Produces system data.
*******************************************************/
void getData()
{
   String timeString = String(cachedTime.month) + "/";
   timeString += String(cachedTime.day) + "/";
   timeString += String((cachedTime.year % 100) + 2000) + " "; // Year only stored as 2-digit number, this wont work in the year 2100
   timeString += leadingZero(cachedTime.hour) + ":";
   timeString += leadingZero(cachedTime.minute) + ":";
   timeString += leadingZero(cachedTime.second);
   String wakeupString = String(sleepWakeupTime.endHour * 100 + sleepWakeupTime.endMinute);
   String sleepString = String(sleepWakeupTime.startHour * 100 + sleepWakeupTime.startMinute);
   String dataString = "{\"rtc_date_time\" : \"" + timeString + "\"}";
   dataString += ",{\"wakeup_time\" : " + wakeupString + "}";
   dataString += ",{\"sleep_time\"  : " + sleepString + "}";
   dataString += ",{\"stay_green_min\"  : " + String(sleepWakeupTime.greenMinutes) + "}";
   dataString += ",{\"temperature\" : " + String(temperature) + "}";
   dataString += ",{\"humidity\" : " + String(humidity) + "}";
   server.send(200, "text/plain", dataString);
}

/*******************************************************
* HANDLES A PLAIN POST
*******************************************************/
void handlePlain() 
{
   if (server.method() != HTTP_POST) 
   {
      server.send(405, "text/plain", "Method Not Allowed");
   } 
   else 
   {
      server.send(200, "text/plain", "POST body was:\n" + server.arg("plain"));
      String parsedArgs = server.arg("plain");
      String value = "";
      String key = "";
      String tempString = "";
      char nextDelim = '=';
      bool saveTime = false;
      for (int i = 0; i < parsedArgs.length(); i++)
      {
        if (parsedArgs.charAt(i) == nextDelim)
        {
           // Just collected the key, now we colect the value
           if (nextDelim == '=')
           {
              key = tempString;
              nextDelim = '\n';
              tempString = "";
           }
           else
           {
              value = tempString;
              int intVal = tempString.toInt();
              int hours = intVal / 100;
              int minutes = intVal % 100;
              if (key == "startTime" && intVal != -1)
              {
                 sleepWakeupTime.startHour = hours;
                 sleepWakeupTime.startMinute = minutes;
                 saveTime = true;
              }
              else if (key == "endTime" && intVal != -1)
              {
                 sleepWakeupTime.endHour = hours;
                 sleepWakeupTime.endMinute = minutes;
                 saveTime = true;
              }
              else if (intVal != -1)
              {
                 sleepWakeupTime.greenMinutes = intVal;
                 saveTime = true;
              }
              else
              {
                 Serial.println("Will not set key: " + key);
              }
              nextDelim = '=';
           }
           tempString = "";
        }
        else
        {
           tempString += parsedArgs.charAt(i);
        }
      }
      if (saveTime)
      {
         SaveTimes();
      }
      Serial.println(parsedArgs);
   }
}

/******************************************************
* HANDLES THE PAGE NOT BEING FOUND
******************************************************/
void handleNotFound() 
{
   String message = "File Not Found\n\n";
   message += "URI: ";
   message += server.uri();
   message += "\nMethod: ";
   message += (server.method() == HTTP_GET) ? "GET" : "POST";
   message += "\nArguments: ";
   message += server.args();
   message += "\n";
   for (uint8_t i = 0; i < server.args(); i++) 
   {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
   }
   server.send(404, "text/plain", message);
}

/******************************************************
* UPDATES THE RTC WITH THE NTP TIME
******************************************************/
void UpdateRtc()
{
    timeClient.update();
    unsigned long epochTime = timeClient.getEpochTime();
    struct tm *ptm = gmtime ((time_t *)&epochTime); 
    int hour = timeClient.getHours();
    int numeric_month = ptm->tm_mon+1;
    // Account for daylight savings time (assume update time is not midnight)
    if (ptm->tm_mon+1 >= 11 || numeric_month < 3 || numeric_month == 3 && ptm->tm_mday < 8)
    {
       hour -= 1;
    }
    // First lets see if the RTC needs an update
    DateTime now = RTC.now();
    if (now.month() != numeric_month || now.hour() != hour ||
        now.minute() != timeClient.getMinutes() || now.year() % 100 != (ptm->tm_year+1900) % 100)
    {
       Serial.println("Updating RTC...");
       Clock.setYear((ptm->tm_year+1900) % 100);
       Clock.setMonth(numeric_month);
       Clock.setDate(ptm->tm_mday);
       Clock.setDoW(timeClient.getDay());
       Clock.setHour(hour);
       Clock.setMinute(timeClient.getMinutes());
       Clock.setSecond(timeClient.getSeconds());
    }
    else
    {
       Serial.println("RTC time valid");
    }
}

void setup(void) {
  pinMode(led, OUTPUT);
  pinMode(BRIGHT_GREEN_LEDS, OUTPUT);
  pinMode(BRIGHT_RED_LEDS, OUTPUT);
  pinMode(DIM_GREEN, OUTPUT);
  pinMode(DIM_YELLOW, OUTPUT);
  pinMode(DIM_RED, OUTPUT);
  digitalWrite(BRIGHT_GREEN_LEDS, LOW);
  digitalWrite(BRIGHT_RED_LEDS, LOW);
  digitalWrite(DIM_GREEN, LOW);
  digitalWrite(DIM_YELLOW, LOW);
  digitalWrite(DIM_RED, HIGH);
  digitalWrite(led, 0);
  Serial.begin(115200);
  // Start the I2C interface
  Wire.begin();
  dht.begin();
  EEPROM.begin(128);
  ReadTimes();
  WiFi.begin(ssid, password);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
     Serial.println("STA Failed to configure");
  }
  Serial.println("");
  int offlineIndex = 0;

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (WiFi.status() == WL_NO_SSID_AVAIL)
    {
        Serial.println("Internet not available!");
        offlineMode = true;
        digitalWrite(DIM_GREEN, LOW);
        digitalWrite(DIM_YELLOW, HIGH);
        digitalWrite(DIM_RED, LOW);
        offlineIndex++;
        if (offlineIndex >= 10)
        {
           break;
        }
    }
    else if (WiFi.status() == WL_CONNECT_FAILED)
    {
        Serial.println("Internet connection failed!");
        offlineMode = true;
        digitalWrite(DIM_GREEN, LOW);
        digitalWrite(DIM_YELLOW, LOW);
        digitalWrite(DIM_RED, HIGH);
        offlineIndex++;
        if (offlineIndex >= 10)
        {
           break;
        }
    }
  }
  if (!offlineMode)
  {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      digitalWrite(DIM_GREEN, HIGH);
      digitalWrite(DIM_YELLOW, LOW);
      digitalWrite(DIM_RED, LOW);

      // Initialize a NTPClient to get time
      timeClient.begin();
      // Set offset time in seconds to adjust for your timezone, for example:
      // GMT +1 = 3600
      // GMT +8 = 28800
      // GMT -1 = -3600
      // GMT -7 = -25200
      // GMT 0 = 0
      timeClient.setTimeOffset(-25200);
      UpdateRtc();
      

      if (MDNS.begin("esp8266")) {
          Serial.println("MDNS responder started");
      }

      server.on("/", handleRoot);

      server.on("/postTime/", handlePlain);

      server.on("/data", getData);

      server.on("/updateRtc/", updateRTC);

      server.onNotFound(handleNotFound);

      server.begin();
      Serial.println("HTTP server started");
   }
   else
   {
     Serial.println("System starting in offline mode");
   }
   //SetupLEDStates();
}

void loop(void) 
{
   msTicks = millis();
   if (!offlineMode)
   {
      server.handleClient();
   }
   if (set_rtc_time > -1 && set_rtc_time < msTicks)
   {
      UpdateRtc();
      set_rtc_time = -1;
   }
   if (nextTick < msTicks)
   {
      DateTime now = RTC.now();
      long nowMin = now.hour() * 60 + now.minute();

      byte ledsActive = 0;
      long startTimeMin = sleepWakeupTime.startHour * 60 + sleepWakeupTime.startMinute;
      long endTimeMin = sleepWakeupTime.endHour * 60 + sleepWakeupTime.endMinute;
      long endGreenState = endTimeMin + sleepWakeupTime.greenMinutes;
      temperature = dht.readTemperature(true);
      humidity = dht.readHumidity();
      // Cache the time for the server
      cachedTime.year = now.year();
      cachedTime.month = now.month();
      cachedTime.day = now.day();
      cachedTime.hour = now.hour();
      cachedTime.minute = now.minute();
      cachedTime.second = now.second();
      //Serial.print("Start time: ");
      //Serial.print(startTimeMin);
      //Serial.print("Now: ");
      //Serial.println(nowMin);
      if (nowMin >= endTimeMin && nowMin < endGreenState)
      {
         analogWrite(BRIGHT_GREEN_LEDS, GREEN_BRIGHTNESSS);  
         digitalWrite(BRIGHT_RED_LEDS, LOW);  
      }
      else if (nowMin >= endGreenState && nowMin < startTimeMin)
      {
         digitalWrite(BRIGHT_GREEN_LEDS, LOW);  
         digitalWrite(BRIGHT_RED_LEDS, LOW);
      }
      else if (nowMin >= startTimeMin || nowMin < endTimeMin)
      {
         digitalWrite(BRIGHT_GREEN_LEDS, LOW);  
         analogWrite(BRIGHT_RED_LEDS, RED_BRIGHTNESSS);  
      }
      nextTick = msTicks + 1000;
   }
   
}
