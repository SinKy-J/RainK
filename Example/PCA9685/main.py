import time
from machine import I2C,Pin
from servo import Servos
i2c=I2C(sda=Pin(22),scl=Pin(23),freq=10000)
servos=Servos(i2c,address=0x40)


while True:
    for i in range(2, 170):  #我不想把舵机转到最尽0或180度，个人喜好；当然我这里是每次转1°，如果你想调快点
                             #你可以【for i in range(2,170, 2)】,后面的2是步伐相当于c++里面的2++ 
        servos.position(7,i)  #0是表示pca9685的舵机接口，我接的是0号排针；i表示舵机位置
        time.sleep_ms(10)     
    for i in range(170, 2, -1):
        servos.position(0,i)
        time.sleep_ms(10)