#include <SparkFun_ADXL345.h>         // SparkFun ADXL345 Library
#include <Servo.h> // Servo library

// Create acceleratomer and servo objects
ADXL345 adxl = ADXL345();             // SparkFun: USE FOR I2C COMMUNICATION
Servo myservo1, myservo2;

// Intitialize x, y, and z values and angles and a counter for the smoothing array
int x = 0, y = 0, z = 0, count = 0, processingAngleX = 0, processingAngleY = 0;
int X[6], Y[6];

void setup() {
  // Set up serial communication
  Serial.begin(115200);

  // Power on and set sensitivity of acceleromter
  adxl.powerOn();                     // SparkFun: Power on the ADXL345
  adxl.setRangeSetting(2);            // SparkFun: Give the range settings
  // Accepted values are 2g, 4g, 8g or 16g
  // Attach servos to pins 9 and 10
  myservo1.attach(9);
  myservo2.attach(10);
}

void loop() {
  // Update acceleromter values
  adxl.readAccel(&X[count], &Y[count], &z);         // SparkFun: Read the accelerometer values and store them
  
  // Call average function to take an average of x and y arrays to smooth incoming data
  x = average(X);
  y = average(Y);

  // Map and constrain the accelerometer raw data to possible servo values
  x = map(x, -280, 260, 180, 0);
  x = constrain(x, 0, 180);

  y = map(y, -280, 260, 0, 180);
  y = constrain(y, 0, 180);

  // Set servo angle to the scaled angle from current averages
  myservo1.write(x);
  myservo2.write(y);

  // Move the position in the array over or set it to 0 if it is at the end so we overwrite the oldest data
  count++;
  if (count == 5) {
    count = 0;
  }

  // Map servo values to actual rotation of the board for processing
  processingAngleX = map(x, 0, 180, -5, 5);
  processingAngleY = map(y, 0, 180, -5, 5);

  // Print angles to serial monitor separated by a tab
  Serial.print(processingAngleX);
  Serial.print("\t");
  Serial.println(processingAngleY);

}

// Take an average of an integer array and return it as an int
int average(int arr[]) {
  int sum = 0;
  int length = (sizeof(arr) / sizeof(arr[0]));
  for (int i = 0; i < length; i++) {
    sum += arr[i];
  }
  int average = (float) sum / (float) length;
  return average;
}
