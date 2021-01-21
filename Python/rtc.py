################################################################################
# ANTHONY RTC (ADVANCED)
# DESC: The Anthony RTC provides several functions.
# 1. Displays a timer that shows the countdown on when it's time to wake update
# 2. Displays a red color during sleep time, a green when its time to wake up and
#    a no color when wakeup time has passed
# 3. Works like a sound machine. 
# Author: Jonathan L Clark
# Date: 1/16/2021
################################################################################
import digitalio
import board
from PIL import Image, ImageDraw, ImageFont
import adafruit_rgb_display.ili9341 as ili9341
from threading import Thread
import time
from datetime import datetime, date
import os
from random import shuffle
from pygame import mixer
import RPi.GPIO as GPIO

FMT = '%H:%M:%S'
SLEEP_START_STR = "19:30:00"
BEGIN_WAKE_STR = "6:00:00"
DISPLAY_START_STR = "2:00:00"
SLEEP_END_STR = "6:30:00"
MUSIC_SWITCH_STR = "21:00:00"
MIDNIGHT_STR = "23:59:59"

# First define some constants to allow easy resizing of shapes.
BORDER = 20
FONTSIZE = 150
 
# Configuration for CS and DC pins (these are PiTFT defaults):
cs_pin = digitalio.DigitalInOut(board.CE0)
dc_pin = digitalio.DigitalInOut(board.D4)
reset_pin = digitalio.DigitalInOut(board.D24)
lcd_led = 16
RED_LED = 13
GREEN_LED = 12
WHITE_LEDS = 18

GPIO.setmode(GPIO.BCM)            # choose BCM or BOARD  
GPIO.setup(lcd_led, GPIO.OUT) # set a port/pin as an output   
 
# Config for display baudrate (default max is 24mhz):
BAUDRATE = 24000000
RED_BRIGHTNESS = 50.0
GREEN_BRIGHTNESS = 50.0
WHITE_BRIGHTNESS = 50.0

class AnthonyRTC:
    def __init__(self):
        GPIO.setup(RED_LED, GPIO.OUT)
        GPIO.setup(GREEN_LED, GPIO.OUT)
        GPIO.setup(WHITE_LEDS, GPIO.OUT)
        
        self.red_led = GPIO.PWM(RED_LED, RED_BRIGHTNESS)
        self.green_led = GPIO.PWM(GREEN_LED, GREEN_BRIGHTNESS)
        
        self.red_led.start(0)
        self.green_led.start(0)

        self.green_led.ChangeDutyCycle(0)
        self.red_led.ChangeDutyCycle(0)
        
        GPIO.output(lcd_led, 0)       # set port/pin value to 1/GPIO.HIGH/True  
        
        # Setup SPI bus using hardware SPI:
        spi = board.SPI()
         
        self.disp = ili9341.ILI9341(
            spi,
            rotation=90,  # 2.2", 2.4", 2.8", 3.2" ILI9341
            cs=cs_pin,
            dc=dc_pin,
            rst=reset_pin,
            baudrate=BAUDRATE,
        )

        if self.disp.rotation % 180 == 90:
            self.height = self.disp.width  # we swap height/width to rotate it to landscape!
            self.width = self.disp.height
        else:
            self.width = self.disp.width  # we swap height/width to rotate it to landscape!
            self.height = self.disp.height
         
        self.image = Image.new("RGB", (self.width, self.height))
         
        # Get drawing object to draw on image.
        self.draw = ImageDraw.Draw(self.image)
         
        # Draw a green filled box as the background
        self.draw.rectangle((0, 0, self.width, self.height), fill=(0, 0, 0))
        self.disp.image(self.image)
        self.last_value = None
         
        # Load a TTF Font
        self.font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", FONTSIZE)
        
        self.music_change_tm = datetime.strptime(MUSIC_SWITCH_STR, FMT).time()
        self.sleep_end_tm = datetime.strptime(SLEEP_END_STR, FMT).time()
        self.sleep_start_tm = datetime.strptime(SLEEP_START_STR, FMT).time()
        self.begin_wake_tm = datetime.strptime(BEGIN_WAKE_STR, FMT).time()
        self.display_start_tm = datetime.strptime(DISPLAY_START_STR, FMT).time()
        self.midnight_end_tm = datetime.strptime(MIDNIGHT_STR, FMT).time()

        sound_thread = Thread(target = self.SoundMachine)
        sound_thread.daemon = True
        sound_thread.start()

        sleep_thread = Thread(target = self.SleepLight)
        sleep_thread.daemon = True
        sleep_thread.start()

    # Manage the sleep light
    def SleepLight(self):
        mid_night = datetime.strptime("23:59:59", FMT).time()

        while (True):
            now = datetime.now().time()
            if ((now > self.sleep_start_tm and now < mid_night) or now < self.begin_wake_tm):
                #print("Red")
                self.red_led.ChangeDutyCycle(RED_BRIGHTNESS)
                #self.white_led.ChangeDutyCycle(WHITE_BRIGHTNESS)
                GPIO.output(WHITE_LEDS, GPIO.HIGH)
                self.green_led.ChangeDutyCycle(0)

            elif (now > self.begin_wake_tm and now < self.sleep_end_tm):
                print("Green")
                self.green_led.ChangeDutyCycle(GREEN_BRIGHTNESS)
                #self.white_led.ChangeDutyCycle(WHITE_BRIGHTNESS)
                GPIO.output(WHITE_LEDS, GPIO.HIGH)
                self.red_led.ChangeDutyCycle(0)
            else:
                #print("Off")
                self.green_led.ChangeDutyCycle(0)
                self.red_led.ChangeDutyCycle(0)
                #self.white_led.ChangeDutyCycle(0)
                GPIO.output(WHITE_LEDS, GPIO.LOW)

            if (now > self.display_start_tm and now < self.begin_wake_tm):
                GPIO.output(lcd_led, 1)  
                tdelta = datetime.combine(date.today(), self.begin_wake_tm) - datetime.combine(date.today(), now)
                if tdelta.seconds < 60:
                    self.DisplayCount(int(tdelta.seconds))
                else:
                    self.DisplayCount(int(tdelta.seconds / 60.0))
                    
            else:
                GPIO.output(lcd_led, 0)       # set port/pin value to 1/GPIO.HIGH/True  
            time.sleep(1)

    # Function to operate the sound machine
    def SoundMachine(self):
        # Anthony
        sound_directory = "/home/pi/Music/KidsSoundMachineSleep/"
        white_sound = "/home/pi/Music/KidsSoundMachineSleep/FlowingWater.mp3"
        white_sound2 = "/home/pi/Music/KidsSoundMachineSleep/FlowingCreek.mp3"
        white_sound3 = "/home/pi/Music/KidsSoundMachineSleep/BabblingBrook.mp3"
        water_sounds = [white_sound, white_sound2]
        mixer.init()
        
        while (True): # Main loop will run forever and ever and ever
            music_files = []
            for file in os.listdir(sound_directory):
                if file.endswith(".mp3"):
                    file_name = os.path.join(sound_directory, file)
                    if not file_name == white_sound and not file_name == white_sound2 and not file_name == white_sound3:
                        music_files.append(file_name)
                    #print(os.path.join(sound_directory, file))

            shuffle(music_files) # Make sure we have random music each time

            now = datetime.now().time()
            print("Waiting for sleep start time...")
            while (now < self.sleep_start_tm and now > self.sleep_end_tm):
                now = datetime.now().time()
                time.sleep(0.5)

            now = datetime.now().time()
            sound_index = 0
            # Play primary music between these two times
            while (now <= self.music_change_tm and now > self.sleep_start_tm):
                if not mixer.music.get_busy():
                    mixer.music.stop()
                    mixer.music.load(music_files[sound_index])
                    mixer.music.play()
                    print("Song switched to: " + music_files[sound_index])
                    sound_index += 1
                    # Reached the end of our music list, restart at the beginning
                    if sound_index >= len(music_files):
                        sound_index = 0

                now = datetime.now().time()
                time.sleep(0.5)
           
            # Play sound machine between 9:00 and wakup time 6:00
            sound_index = 0
            mixer.music.stop()
            now = datetime.now().time()
            while (now >= self.music_change_tm or now < self.sleep_end_tm):
                if not mixer.music.get_busy():
                    print("Starting sound machine sound")
                    mixer.music.stop()
                    mixer.music.load(water_sounds[sound_index])
                    mixer.music.play()
                    sound_index+=1
                    if sound_index >= len(water_sounds):
                        sound_index = 0
                now = datetime.now().time()
                time.sleep(0.5)
                
            mixer.music.stop()
            
    # Display the current count on the screen
    def DisplayCount(self, value):
        if self.last_value == value:
            return # Do nothing, the value on the screen is the same

        self.last_value = value
        self.draw.rectangle((0, 0, self.width, self.height), fill=(0, 0, 0))
        self.disp.image(self.image)
        # Draw Some Text
        text = str(value)
        (font_width, font_height) = self.font.getsize(text)
        self.draw.text(
            (self.width // 2 - font_width // 2, self.height // 2 - font_height // 2),
            text,
            font=self.font,
            fill=(255, 255, 255),
        )
        # Display image.
        self.disp.image(self.image)
     
if __name__ == '__main__':
    sound_machine = AnthonyRTC()

    while (True):
        time.sleep(1)

