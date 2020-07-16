#include <Servo.h>
#include <Wire.h>
#include <LSM303.h>

LSM303 compass;
Servo myservo;

int servoPin = 3;       // Pin that the servomotor is connected to
int solenoidPin = 2;    // Pin that the mosfet is conected to
int switchPin = 4;      // Pin that the switch is conected tobbvg
int pos = 0;            // variable to store the servo position
int switchState;        // variable that stores the Reed switch state
int servoDir = 0;       // variable that stores the direction the motor is turning in the demo program
int solenoidState = LOW;  // variable that stores if solenoid is on or off         
unsigned long previousMillis = 0;        // will store last time solenoid was updated
const long interval = 1000;           // interval at which to turn solenoid on and off (milliseconds)


void setup() {
  myservo.attach(servoPin);               // attaches the servo on pin 9 to the servo object
  pinMode(solenoidPin, OUTPUT);           //Sets the pin as an output
  pinMode(switchPin, INPUT_PULLUP);       //Sets the pin as an input_pullup
  Serial.begin(9600);                     // starts serial communication @ 9600 bps
  Wire.begin();
  compass.init();
  compass.enableDefault();
  compass.writeReg(0x24, 0x74);           // sets a parameter on the magnetometer so it updates at a higher rate than the default rate of 7 Hz;  Has to occure after compass.enableDefault()

  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  compass.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  compass.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}
  


void loop() {

////////////// SERVOMOTOR ///////////////////////////////////////////////////
  compass.read();
  float heading = compass.heading();

  
   if(servoDir == 0)
  {
    pos++;
    if(pos >= 90)
    {
      servoDir = 1;
    }
  }
  else
  {
    pos--;
    if(pos <= 0)
    {
      servoDir = 0;
    }
  }
  

  myservo.write(pos);              // tell servo to go to position in variable 'pos'
  Serial.print("posistion:   ");
  Serial.print(pos);
  delay(10); 

////////////// MAGNETOMETER ///////////////////////////////////////////////////
  /*compass.read();
  float heading = compass.heading();
  */

////////////// SOLENOID VALVE ///////////////////////////////////////////////////
unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (solenoidState == LOW) {
      solenoidState = HIGH;
    } else {
      solenoidState = LOW;
    }
    digitalWrite(solenoidPin, solenoidState);    //Switch Solenoid ON/oFF
  }

////////////// REED SWITCH ///////////////////////////////////////////////////
  switchState = digitalRead(switchPin);

////////////// Serial Print  ///////////////////////////////////////////////////
  Serial.print("   Reed Switch: ");
  Serial.print(switchState);
  //Serial.print("   Magnetometer: ");
  //Serial.println(heading);

  //delay(100);
}
