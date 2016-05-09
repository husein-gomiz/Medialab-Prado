/******************************************************
 * This program reads an analog input, smoothens it and detects peacks over a
 * threshold. When a peack is detected a servo motor is placed at min or max 
 * position depending on its previous state
 */


#include <Servo.h>

Servo myservo;  // create servo object to control a servo



//Smoothing variables
const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int inputPin  = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin
int open_servo=1,closed_servo=0, peack=0,reading=0, over_threshold;
int thres_max, thres_min;  //threshold limits
const int closed_val= 170;
const int open_val=20 ;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0; //initialize array
  }
  thres_max=800;  //upper threshold
  thres_min=600;  //bottom  threshold

}

void loop() {

//Smoothing 
//***********************************************************

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
  // calculate the average:

  average = total / numReadings;
  delay(1);        // delay in between reads for stability  
//****************************************************************

  
//if signal exceeds the top threshold it waits for it to pass the bottom threshold to declare a peack
  if (average>thres_max) {
    over_threshold=1;
  }
  
  if (over_threshold ==1 && average < thres_min){
    peack=1;
    over_threshold=0;
   // delay(1500);
  }

// when a peack occurs, close the servo if open and viceversa 
  if (peack==1) {
    if (open_servo==1) {
      val=closed_val;
      closed_servo = 1;
      open_servo = 0;
      peack = 0;
     }
    else{
      val=open_val;
      open_servo=1;
      closed_servo=0;
      peack=0;
    }
  }
  Serial.println(average);
  myservo.write(val);                  // sets the servo position according to the  value
  delay(15);                           // waits for the servo to get there
}
