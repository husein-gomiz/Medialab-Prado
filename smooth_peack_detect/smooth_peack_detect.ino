#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int inputPin  = 0;  // analog pin used to connect the potentiometer
int val,val2;    // variable to read the value from the analog pin

const int numReadings = 10;
int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
int off=1,on=0, peack=0,reading=0, over_threshold;
int thres_top=0, thres_bot=0;

void setup() {
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  thres_top=800;  //umbral superior
  thres_bot=600;  //umbral inferior

}

void loop() {

//Smoothing ( suaviza la señal para quitar ruido)
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

  
//Si el pico sube del umbral superior, se queda en espera de bajar del umbral inferior.
  if (average>800) {
    over_threshold=1;
  }

// en cuanto pasa el umbral inferior detecta un pico
  if (over_threshold ==1 && average < 600){
    peack=1;
    over_threshold=0;
   // delay(1500);
  }

// si ha habido un pico, si esta abierto => cerrar y viceversa.  
  if (peack==1) {
    if (off==1) {
      val2=170;
      on = 1;
      off = 0;
      peack = 0;
     }
    else{
      val2=20;
      off=1;
      on=0;
      peack=0;
    }
  }
  Serial.println(average);
  myservo.write(val2);                  // sets the servo position according to the  value
  delay(15);                           // waits for the servo to get there
}
