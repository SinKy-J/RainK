
#include <SimpleFOC.h>

//MagneticSensorAnalog sensor = MagneticSensorAnalog(A1, 14, 1020);
MagneticSensorAnalog sensor = MagneticSensorAnalog(A0, 14, 1020);
MagneticSensorAnalog sensor_master = MagneticSensorAnalog(A1, 14, 1020);
// BLDC motor instance
//BLDCMotor motor = BLDCMotor(11, 10, 9, 7, 8);
BLDCMotor motor = BLDCMotor(3, 5, 6, 7, 7);
// Stepper motor instance
//StepperMotor motor = StepperMotor(9, 5, 10, 6, 50, 8);

void setup() {
  // initialise magnetic sensor hardware
  sensor.init();
  sensor_master.init();
  //sensor.init();
  // link the motor to the sensor
  motor.linkSensor(&sensor);
  //motor.linkSensor(&sensor0);
  // power supply voltage
  // default 12V
  motor.voltage_power_supply = 12;
  // choose FOC modulation (optional)
  motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
  // set motion control loop to be used
  motor.controller = ControlType::angle;
  // contoller configuration 
  // default parameters in defaults.h

  // velocity PI controller parameters
  motor.PID_velocity.P = 0.09;
  motor.PID_velocity.I = 1.70;
  motor.PID_velocity.D = 0.0005;
  // maximal voltage to be set to the motor
  motor.voltage_limit = 6;
  // velocity low pass filtering time constant
  // the lower the less filtered
  motor.LPF_velocity.Tf = 0.04;

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

float val=0;
void loop() {
  val=sensor_master.getAngle();
  //Serial.print(target_angle);
 // Serial.println(val);
  // main FOC algorithm function
  // the faster you run this function the better
  // Arduino UNO loop  ~1kHz
  // Bluepill loop ~10kHz 
  motor.loopFOC();
  // Motion control function
  // velocity, position or voltage (defined in motor.controller)
  // this function can be run at much lower frequency than loopFOC() function
  // You can also use motor.move() and set the motor.target in the code
  motor.move(-val);

  // function intended to be used with serial plotter to monitor motor variables
  // significantly slowing the execution down!!!!
  // motor.monitor();
  // user communication
//  serialReceiveUserCommand();

  
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
    //  Serial.print("Target angle: ");
    //  Serial.println(target_angle);
      
      // reset the command buffer 
      received_chars = "";
    }
  }
}
