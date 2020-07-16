#include <Servo.h>

#include <Wire.h>

#include <LSM303.h>

 

LSM303 compass;

Servo myservo;

 

int servoPin = 3;       // servomotor pin 

int solenoidPin = 2;    // mosfet pin

int switchPin = 4;      // switch pim

int pos = 0;            // variable to store the servo position

int switchState;        // variable that stores the Reed switch state

int servoDir = 0;       // variable that stores the direction the motor is turning in the demo program

int solenoidState = LOW;  // variable that stores if solenoid is on or off        

unsigned long previousMillis = 0;        // store last time solenoid was updated

const long interval = 1000;           // interval at which to turn solenoid on and off (milliseconds)

int headingdifference = 0;

int rolls = 0;

int lastState = 0;

int stage = 0;

int startingcase = 1;

int i = 0;

int t = 0;

int timeelaped = 0;

void setup() {

  myservo.attach(servoPin);               // attaches the servo on pin 3 to the servo object

  pinMode(solenoidPin, OUTPUT);           //Sets the pin as an output

  pinMode(switchPin, INPUT_PULLUP);       //Sets the pin as an input_pullup

  Serial.begin(9600);                     // starts serial communication @ 9600 bps

  Wire.begin();

  compass.init();

  compass.enableDefault();

  compass.writeReg(0x24, 0x74);           // updates at a higher rate on the magentometer
   

  /*

  Calibration values; the default values of +/-32767 for each axis

  lead to an assumed magnetometer bias of 0. Use the Calibrate example

  program to determine appropriate values for your particular unit.

  */

  compass.m_min = (LSM303::vector<int16_t>){-1200, -1273, +1359};

  compass.m_max = (LSM303::vector<int16_t>){+1204, +718, +1829};

}

 

 

 

void loop() {

if (t == 0)

  {

    delay(15000);

    t = 1;

    timeelaped = millis();

  }

////////////// SOLENOID VALVE ///////////////////////////////////////////////////

unsigned long currentMillis = millis(); //piston on

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

 

  if (switchState == 1 && lastState != 1) //read switch

    {

      rolls = rolls + 1;

    }

  lastState = switchState;

  Serial.println("Num of rolls is: " + String(rolls));

  Serial.println("Distance traveled: " + String(rolls*3.14*3.937) + "inches" );

 

  if (rolls >= 10)

  {

    Serial.println("Start of Stage 2");

    if (startingcase == 1)

    {

      myservo.write(130);

      delay(5000);

    }

    else{

     myservo.write(50);

     delay(5000);

    }

    i=0;

    while (i < 10)

    {

      if (currentMillis - previousMillis >= interval)

     {

     previousMillis = currentMillis;

      if (solenoidState == LOW) {

      solenoidState = HIGH;

      } else {

      solenoidState = LOW;

      }

       digitalWrite(solenoidPin, solenoidState);    //Switch Solenoid ON/oFF

       }

     i = i + 1;

     }

    stage = 2;

    Serial.println("Stage 3");

 

    while (millis >= 70*1000)

    {

     

    }

   

  }

 

  while ((rolls >= 10) && (stage == 2))  //STAGE 3 CLSOED FEEDBACK LOOP

  {

  compass.read();

   float heading = compass.heading(); // reads compass

   headingdifference = 246 - heading; // gets angle difference of direction with 246 as north

   Serial.println(heading);

 

     if(headingdifference <= pos) { //change servo position

     pos--;

     Serial.println("turn left");

    }

     else

    {

     pos++;

     Serial.println("turn right");

    }

 

  myservo.write(pos);              // tell servo to go to position in variable 'pos'

 

  Serial.println(heading);

  Serial.println((headingdifference)); // print to serial of diff

 

  delay(10);

   if (currentMillis - previousMillis >= interval)

    {

    previousMillis = currentMillis;

      if (solenoidState == LOW) {

      solenoidState = HIGH;

    } else {

      solenoidState = LOW;

    }

    digitalWrite(solenoidPin, solenoidState);    //Switch Solenoid ON/oFF

    }

 

   Serial.print(" ");

   Serial.println(heading);

     while (millis >= 70*1000)

    {

     

    }

 

  

   delay(10);

  }

}
