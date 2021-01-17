################################################################################
# ANTHONY RTC (ADVANCED)
# DESC: The Anthony RTC provides several functions.
# 1. Display's a timer that shows the countdown on when it's time to wake update
# 2. Display's a red color during sleep time, a green when its time to wake up and
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
from datetime import datetime
import os
from random import shuffle
from pygame import mixer

FMT = '%H:%M:%S'

# First define some constants to allow easy resizing of shapes.
BORDER = 20
FONTSIZE = 150
 
# Configuration for CS and DC pins (these are PiTFT defaults):
cs_pin = digitalio.DigitalInOut(board.CE0)
dc_pin = digitalio.DigitalInOut(board.D4)
reset_pin = digitalio.DigitalInOut(board.D24)
 
# Config for display baudrate (default max is 24mhz):
BAUDRATE = 24000000

class AnthonyRTC:
    def __init__(self):
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

        sound_thread = Thread(target = self.SoundMachine)
        sound_thread.daemon = True
        sound_thread.start()

        sleep_thread = Thread(target = self.SleepLight)
        sleep_thread.daemon = True
        sleep_thread.start()

    # Manage the sleep light
    def SleepLight(self):
        red_start_time = "19:00:00"
        green_time_str = "10:00:00" #"6:00:00"
        display_time_str = "9:00:00" #"2:00:00"
        mid_night = datetime.strptime("23:59:59", FMT).time()
        red_time = datetime.strptime(red_start_time, FMT).time()
        green_time = datetime.strptime(green_time_str, FMT).time()
        green_end_time = datetime.strptime("6:30:00", FMT).time()
        display_time = datetime.strptime(display_time_str, FMT).time()
        #now = datetime.strptime("2:31:00", FMT)

        while (True):
            now = datetime.now().time()
            print(now)
            if ((now > red_time and now < mid_night) or now < green_time):
                print("Red:")

            elif (now > green_time and now < green_end_time):
                print("Green")
            else:
                print("Off")
                
            print(now)
            print(display_time)
            if (now > display_time and now < green_time):
                print(green_time.hour)
                #tdelta = green_time - now
                #if tdelta.seconds < 60:
                #    print(int(tdelta.seconds))
                #    self.DisplayCount(int(tdelta.seconds))
                #else:
                #    print(int(tdelta.seconds / 60.0))
                #    self.DisplayCount(int(tdelta.seconds / 60.0))
                    
            else:
                print("Display off")
            time.sleep(1)

    # Function to operate the sound machine
    def SoundMachine(self):
        sound_start_time_str = "8:00:00"
        sound_start_time = datetime.strptime(sound_start_time_str, FMT)
        
        nature_switch_time_str = "21:00:00"
        nature_switch_time = datetime.strptime(nature_switch_time_str, FMT)

        # Anthony
        sound_directory = "/home/pi/Music/KidsSoundMachineSleep/"
        white_sound = "/home/pi/Music/KidsSoundMachineSleep/FlowingWater.mp3"
        white_sound2 = "/home/pi/Music/KidsSoundMachineSleep/FlowingCreek.mp3"
        white_sound3 = "/home/pi/Music/KidsSoundMachineSleep/BabblingBrook.mp3"
        music_files = []
        for file in os.listdir(sound_directory):
            if file.endswith(".mp3"):
                file_name = os.path.join(sound_directory, file)
                if not file_name == white_sound and not file_name == white_sound2 and not file_name == white_sound3:
                    music_files.append(file_name)
                #print(os.path.join(sound_directory, file))

        shuffle(music_files) # Make sure we have random music each time
        mixer.init()
        sound_index = 0

        # First we play primary music
        while (True):
            if not mixer.music.get_busy():
                mixer.music.stop()
                mixer.music.load(music_files[sound_index])
                mixer.music.play()
                print("Song switched to: " + music_files[sound_index])
                sound_index += 1
                # Reached the end of our music list, restart at the beginning
                if sound_index >= len(music_files):
                    sound_index = 0

            # TODO: If our current time is greater than 9:00 but less than midnight we break
            time.sleep(0.5)
       
        # Now play white noise until that time is over
        mixer.music.stop()
        mixer.music.load(white_sound)
        mixer.music.play()

        while (True):
            if not mixer.music.get_busy():
                pygame.mixer.music.rewind()
            time.sleep(0.5)
         
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

