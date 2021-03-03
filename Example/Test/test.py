import time
import ssd1306
import neopixel
import machine


number = 0
i2c = machine.I2C(scl = machine.Pin(22), sda = machine.Pin(21), freq = 10000000)
oled = ssd1306.SSD1306_I2C(128,32,i2c)
rgb = neopixel.NeoPixel(machine.Pin(19), 2, timing = True)
rgb[0] = (0, 5, 5)
rgb[1] = (5, 0, 5)
rgb.write()
while True:
    oled.text('Hello RainK', 0, 0)
    oled.text(str(number), 0, 16)
    oled.show()
    time.sleep_ms(1000)
    number = number + 1
    oled.show_fill(0)
