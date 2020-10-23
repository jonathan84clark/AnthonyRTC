/********************************************************************
* RTC V2
* DESC: This is the second iteration of Anthony's RTC. This one being more advanced,
* with a WiFi interface. This device is able to automatically update for daylight savings
* time. It is also capable of providing a web interface for setting wakeup times. 
* Author: Jonathan L Clark
* Date: 10/22/2020
********************************************************************/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>

struct TimeFrame
{
   int startHour;
   int startMinute;
   int endHour;
   int endMinute;
   int greenMinutes;
};

#ifndef STASSID
#define STASSID "JLC1"
#define STAPSK  "1990clark$fam1984"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;
bool offlineMode = false;

TimeFrame sleepWakeupTime;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
ESP8266WebServer server(80);

const int led = LED_BUILTIN;

const String postForms = "<html>\
  <head>\
    <title>Anthony Real-Time Clock</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Add Time</h1><br>\
    <form method=\"post\" enctype=\"text/plain\" action=\"/postTime/\">\
      <p>Start Red</p>\
      <select name=\"startTime\" id=\"startRedTime\">\
         <option value=\"1800\">6:00 PM</option>\
         <option value=\"1900\">7:00 PM</option>\
         <option value=\"1915\">7:15 PM</option>\
         <option value=\"1930\">7:30 PM</option>\
      </select>\
      <p>End Red</p>\
      <select name=\"endTime\" id=\"startGreenTime\">\
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
         <option value=\"10\">10 min</option>\
         <option value=\"20\">20 min</option>\
         <option value=\"30\">30 min</option>\
         <option value=\"40\">40 min</option>\
         <option value=\"50\">50 min</option>\
         <option value=\"60\">60 min</option>\
      </select>\
      <input type=\"submit\" value=\"Set\">\
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
   /*
   Serial.print("Start: ");
   Serial.print(sleepWakeupTime.startHour);
   Serial.print(" : ");
   Serial.print(sleepWakeupTime.startMinute);
   Serial.print("End: ");
   Serial.print(sleepWakeupTime.endHour);
   Serial.print(" : ");
   Serial.println(sleepWakeupTime.endMinute);
   Serial.print("End minutes: ");
   Serial.println(sleepWakeupTime.greenMinutes);
   Serial.println("Times read from EEProm");
   */
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/html", postForms);
  digitalWrite(led, 0);
}

/*******************************************************
* HANDLES A PLAIN POST
*******************************************************/
void handlePlain() 
{
   if (server.method() != HTTP_POST) 
   {
      digitalWrite(led, 1);
      //server.send(405, "text/plain", "Method Not Allowed");
      digitalWrite(led, 0);
   } 
   else 
   {
      digitalWrite(led, 1);
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
              if (key == "startTime")
              {
                 sleepWakeupTime.startHour = hours;
                 sleepWakeupTime.startMinute = minutes;
              }
              else if (key == "endTime")
              {
                 sleepWakeupTime.endHour = hours;
                 sleepWakeupTime.endMinute = minutes;
              }
              else
              {
                 sleepWakeupTime.greenMinutes = intVal;
              }
              saveTime = true;
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
      digitalWrite(led, 0);
   }
}

/******************************************************
* HANDLES THE PAGE NOT BEING FOUND
******************************************************/
void handleNotFound() 
{
   digitalWrite(led, 1);
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
   digitalWrite(led, 0);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  EEPROM.begin(128);
  ReadTimes();
  WiFi.begin(ssid, password);
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
        offlineIndex++;
        if (offlineIndex >= 10)
        {
           break;
        }
    }
    Serial.println(WiFi.status());
  }
  if (!offlineMode)
  {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      // Initialize a NTPClient to get time
      timeClient.begin();
      // Set offset time in seconds to adjust for your timezone, for example:
      // GMT +1 = 3600
      // GMT +8 = 28800
      // GMT -1 = -3600
      // GMT -7 = -25200
      // GMT 0 = 0
      timeClient.setTimeOffset(-25200);
      timeClient.update();
      unsigned long epochTime = timeClient.getEpochTime();
      Serial.print("Hour: ");
      Serial.print(timeClient.getHours());
      Serial.print(":");
      Serial.print(timeClient.getMinutes());
      Serial.print(":");
      Serial.println(timeClient.getSeconds());

      if (MDNS.begin("esp8266")) {
          Serial.println("MDNS responder started");
      }

      server.on("/", handleRoot);

      server.on("/postTime/", handlePlain);

      server.onNotFound(handleNotFound);

      server.begin();
      Serial.println("HTTP server started");
   }
   else
   {
     Serial.println("System starting in offline mode");
   }
}

void loop(void) 
{
   if (!offlineMode)
   {
      server.handleClient();
   }
   
}
