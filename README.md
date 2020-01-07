# Anthony RTC
Anthony RTC is a small device used to turn on lights at different times to indicate important times to young children.

## Theory Of Operation    
The Arduino is connected to an RTC module. The RTC provides the current time. In the EEProm are stored a series of start and end times as well as associated LED colors. Every second the times are checked. If the current time is between the start and end time the associated light turns on.  The entire EEProm is not used as it is unnecessary, however a #define at the top of the AnthonyRTC sketch indicates how many times we can store. If the device runs out of available time slots it will flash red. There is currently no way to remove or replace an existing time slot.  

## Interfacing  
The Anthony RTC uses a 16-byte serial interface to communicate with a PC for programming purposes. The included Visual Studio project is designed to provide a basic UI for configuring the device. If a programming action succeeds the LED will flash green, if it fails it will flash red.  

## Compilation  
The AnthonyRTC firmware can be compiled with the Arduino sketch IDE. The interface application is written in C# and can be compiled with MS Visual Studio.  

## Status  
As of 1/6/2020 this project is considered complete and has entered support mode.  
