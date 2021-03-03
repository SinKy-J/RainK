import time
import mpu6050
import machine
from machine import I2C,Pin

i2c = I2C(scl = Pin(22), sda = Pin(21))

accelerometer = mpu6050.accel(i2c)
ints = accelerometer.get_ints()

#初始化传感器
sum_val = 0
for i in range(400):
  acc_data = accelerometer.get_values()
  sum_val+=acc_data['AcX']/16384
error_x = sum_val/400


#平滑滤波器
class avg_fiter():
    def __init__(self, data_list):
        self.data_sum=sum(data_list)
        self.data_list=data_list
    def fit(self, data, len):
        #data是传入的数据,len是平滑的长度
        self.data_sum = self.data_sum - self.data_list[0] + data
        self.data_list.pop(0)
        self.data_list.append(data)
        data = self.data_sum/len
        return data
    
data_list=[0,0,0,0,0]

avgfiter = avg_fiter(data_list)
    
while True:
    acc_data = accelerometer.get_values()
    temp = avgfiter.fit(acc_data['AcX']/16384,5)
    #现在这里面的数字是由加速度算出来的-90~+90度，未经过滤波
    print(round(acc_data['AcX']/16384*90,2),round(acc_data['AcY']/16384*90,2),round(acc_data['AcZ']/16384*90,2))
    time.sleep_ms(30)