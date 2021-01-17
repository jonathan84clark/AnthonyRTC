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
import adafruit_rgb_display.st7789 as st7789  # pylint: disable=unused-import
import adafruit_rgb_display.hx8357 as hx8357  # pylint: disable=unused-import
import adafruit_rgb_display.st7735 as st7735  # pylint: disable=unused-import
import adafruit_rgb_display.ssd1351 as ssd1351  # pylint: disable=unused-import
import adafruit_rgb_display.ssd1331 as ssd1331  # pylint: disable=unused-import
import time
from datetime import datetime
 

# First define some constants to allow easy resizing of shapes.
BORDER = 20
FONTSIZE = 150
 
# Configuration for CS and DC pins (these are PiTFT defaults):
cs_pin = digitalio.DigitalInOut(board.CE0)
dc_pin = digitalio.DigitalInOut(board.D4)
reset_pin = digitalio.DigitalInOut(board.D24)
 
# Config for display baudrate (default max is 24mhz):
BAUDRATE = 24000000

def DisplayCount():
     
    # Setup SPI bus using hardware SPI:
    spi = board.SPI()
     
    disp = ili9341.ILI9341(
        spi,
        rotation=90,  # 2.2", 2.4", 2.8", 3.2" ILI9341
        cs=cs_pin,
        dc=dc_pin,
        rst=reset_pin,
        baudrate=BAUDRATE,
    )
    # pylint: enable=line-too-long
     
    # Create blank image for drawing.
    # Make sure to create image with mode 'RGB' for full color.
    if disp.rotation % 180 == 90:
        height = disp.width  # we swap height/width to rotate it to landscape!
        width = disp.height
    else:
        width = disp.width  # we swap height/width to rotate it to landscape!
        height = disp.height
     
    image = Image.new("RGB", (width, height))
     
    # Get drawing object to draw on image.
    draw = ImageDraw.Draw(image)
     
    # Draw a green filled box as the background
    draw.rectangle((0, 0, width, height), fill=(0, 0, 0))
    disp.image(image)
     
    # Load a TTF Font
    font = ImageFont.truetype("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", FONTSIZE)
     
    # Display image.
    disp.image(image)

    time.sleep(1)
    value = 500
    for x in range(0, 100):
        # Draw Some Text
        text = str(value)
        (font_width, font_height) = font.getsize(text)
        draw.text(
            (width // 2 - font_width // 2, height // 2 - font_height // 2),
            text,
            font=font,
            fill=(0, 0, 0),
        )
        # Display image.
        disp.image(image)
        value-=1
        # Draw Some Text
        text = str(value)
        (font_width, font_height) = font.getsize(text)
        draw.text(
            (width // 2 - font_width // 2, height // 2 - font_height // 2),
            text,
            font=font,
            fill=(255, 255, 255),
        )
        # Display image.
        disp.image(image)
        time.sleep(1)
     
#DisplayCount()
FMT = '%H:%M:%S'
red_start_time = "19:00:00"
green_time_str = "6:00:00"
display_time_str = "2:00:00"
mid_night = datetime.strptime("23:59:59", FMT)
red_time = datetime.strptime(red_start_time, FMT)
green_time = datetime.strptime(green_time_str, FMT)
green_end_time = datetime.strptime("6:30:00", FMT)
display_time = datetime.strptime(display_time_str, FMT)
now = datetime.now()
now = datetime.strptime("2:31:00", FMT)


if ((now > red_time and now < mid_night) or now < green_time):
    print("Red:")

elif (now > green_time and now < green_end_time):
    print("Green")
else:
    print("Off")
    
if (now > display_time and now < green_time):
    print("Show Display")
    tdelta = green_time - now
    if tdelta.seconds < 60:
        print(int(tdelta.seconds))
    else:
        print(int(tdelta.seconds / 60.0))
        
else:
    print("Display off")
#s1 = '10:33:26'
#s2 = '11:15:49' # for example

#tdelta = datetime.strptime(s2, FMT) - datetime.strptime(s1, FMT)
#print(tdelta.seconds)

