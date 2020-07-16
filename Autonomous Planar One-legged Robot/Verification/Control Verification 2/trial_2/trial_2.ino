#include <Servo.h>
#include <Wire.h>
#include <LSM303.h>

LSM303 compass;
Servo myservo;

int servoPin = 3;       // Pin that the servomotor is connected to
int solenoidPin = 2;    // Pin that the mosfet is conected to
int switchPin = 4;      // Pin that the switch is conected to
int pos = 0;            // variable to store the servo position
int switchState;        // variable that stores the Reed switch state
int servoDir = 0;       // variable that stores the direction the motor is turning in the demo program
int solenoidState = LOW;  // variable that stores if solenoid is on or off         
unsigned long previousMillis = 0;        // will store last time solenoid was updated
const long interval = 1000;           // interval at which to turn solenoid on and off (milliseconds)
int headingdifference = 0;            // angle diff between direction of north and servo
int distance = 0;          // distance travelled by the robot
int count = 0;          // number of times reed switch changes
int prevSwitchState = 1;              // previous state of the initialized to 1 (off)
double angle  = 0;
double preAngle = 0;
double error = 0;
double motorVelocity = 0;
double errorIntegral = 0;
double control Signal  = 0;
int PGain = 5;
int DGain = 200;
int IGain = 0.05;
int desiredPosition = 180;


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
  compass.m_min = (LSM303::vector<int16_t>){  -449,  -1294,   +337};
  compass.m_max = (LSM303::vector<int16_t>){  +1399,   +645,   +500};
}
  


void loop() {

////////////// SERVOMOTOR ///////////////////////////////////////////////////

  

////////////// MAGNETOMETER ///////////////////////////////////////////////////
  compass.read();
  float heading = compass.heading();
  headingdifference = desiredPosition - heading;      //sets the direction of the servo to north


  /*if(headingdifference >= pos) {    //checks if there's a change in current position with heading
    pos++;
  }
  else
  {
    pos--;
  }
  */
  

  myservo.write(pos);              // tell servo to go to position in variable 'pos'

  Serial.print("   Position:  ");
  Serial.print(pos);
  /*Serial.print("   HeadingDifference  ");
  Serial.println(headingdifference);
  */
  
  delay(10); 
  
////////////// REED SWITCH ///////////////////////////////////////////////////
  switchState = digitalRead(switchPin);
  if  (prevSwitchState != switchState){
     prevSwitchState = switchState;
    if (prevSwitchState == 0){
      count++;
    }
  }
  distance  = count*3.14*2.75591;          // count times the diameter of the wheel
  Serial.print("   Distance(inch)   ");
  Serial.println(distance);
  
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

////////////// Trial 1  ///////////////////////////////////////////////////
/*if(currentMillis - previousMillis > dt && count < 1) {
 previousMillis = currentMillis;
 count = count + 1;
 desiredPosition = 280;
 }
 else if(currentMillis - previousMillis > dt && count >= 1 && count < 7) {
 previousMillis = currentMillis;
 count = count + 1;
 if (valvestate == LOW)
 valvestate = HIGH;
 else
 valvestate = LOW;
 digitalWrite(valve, valvestate);
 }
 else if (count >= 7) {
 valvestate = HIGH;
 }
}
*/
////////////// Serial Print  ///////////////////////////////////////////////////

  /*
  Serial.print("   SolenoidState   ");
  Serial.print(solenoidState);
 */
  Serial.print("   Magnetometer   ");
  Serial.println(heading);
  /*
  Serial.print("Reed Switch:   ");
  Serial.print(switchState);
  */

  //delay(100);
}
