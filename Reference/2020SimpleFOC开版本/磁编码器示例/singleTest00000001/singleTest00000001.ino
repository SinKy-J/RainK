/******单路运行,串口输入值，电机运动到相应位置*******/
#include <SimpleFOC.h>

/****磁编码器传感器（传感器引脚,最小的预期读数,读取的最大值）****/
//MagneticSensorAnalog sensor = MagneticSensorAnalog(A1, 14, 1020);
MagneticSensorAnalog sensor = MagneticSensorAnalog(A0, 14, 1020);

// BLDC motor instance
//BLDCMotor motor = BLDCMotor(11, 10, 9, 7, 8);
//BLDCMotor motor = BLDCMotor(3, 5, 6, 7, 7);
/******如果不确定您的极对编号是多少，请查找所购电机手册或者用示例程序：find_pole_pairs.ino******/
/******电机 PWM引脚号（3,5,6）,电机极对数（7）,驱动器使能引脚（7）******/
BLDCMotor motor = BLDCMotor(3, 5, 6, 7, 7);

void setup() {

  /*******将编码器和驱动器连接至电动机，进行硬件初始化和现场定向控制的初始化***********/
  // initialise magnetic sensor hardware
  sensor.init();
  //sensor.init();
  // link the motor to the sensor
  motor.linkSensor(&sensor);
  //motor.linkSensor(&sensor0);
/********************配置 12V 电源电压，然后启动驱动器****/
  motor.voltage_power_supply = 12;
  // choose FOC modulation (optional)
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
  // set motion control loop to be used
  /*****设置回路控制方式：angle（角度）****/
  motor.controller = ControlType::angle;
  // contoller configuration 
  // default parameters in defaults.h
  /******配置速度PID控制器参数*************/
  // velocity PI controller parameters
  motor.PID_velocity.P = 0.12;
  motor.PID_velocity.I = 6;
  motor.PID_velocity.D = 0;
  // maximal voltage to be set to the motor
  motor.voltage_limit = 6;
  // velocity low pass filtering time constant
  // the lower the less filtered
  /*********配置低通滤波器的时间常数 Tf**********/
  motor.LPF_velocity.Tf = 0.01;

  /*********配置位置P控制器增益和速度极限变量*************/
  // angle P controller 
  motor.P_angle.P = 20;
  // maximal velocity of the position control
  motor.velocity_limit = 20;

  // use monitoring with serial 
  Serial.begin(115200);
  // comment out if not needed
  motor.useMonitoring(Serial);
  
  // initialize motor
  motor.init();
  // align sensor and start FOC
  motor.initFOC();


  //Serial.println("Motor ready.");
 // Serial.println("Set the target angle using serial terminal:");
  _delay(1000);
}

// angle set point variable
float target_angle = 0;
void loop() {

  // main FOC algorithm function
  // the faster you run this function the better
  // Arduino UNO loop  ~1kHz
  // Bluepill loop ~10kHz 
  motor.loopFOC();
  // Motion control function
  // velocity, position or voltage (defined in motor.controller)
  // this function can be run at much lower frequency than loopFOC() function
  // You can also use motor.move() and set the motor.target in the code
  motor.move(target_angle);

  // function intended to be used with serial plotter to monitor motor variables
  // significantly slowing the execution down!!!!
  // motor.monitor();
  
  // user communication
  serialReceiveUserCommand();
}

// utility function enabling serial communication with the user to set the target values
// this function can be implemented in serialEvent function as well
void serialReceiveUserCommand() {
  
  // a string to hold incoming data
  static String received_chars;
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the string buffer:
    received_chars += inChar;
    // end of user input
    if (inChar == '\n') {
      
      // change the motor target
      target_angle = received_chars.toFloat();
     // Serial.print("Target angle: ");
      //Serial.println(target_angle);
      
      // reset the command buffer 
      received_chars = "";
    }
  }
}
