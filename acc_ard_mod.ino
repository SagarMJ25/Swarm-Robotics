#include <Wire.h>

long accelX, accelY, accelZ;
float gForceX, gForceY, gForceZ;

boolean printOnce = true;  // Flag to print only once
float threshold = 0.1;     // Set a threshold for change detection

// Variables to store previous acceleration values
float prevGForceX = 0;
float prevGForceY = 0;
float prevGForceZ = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  setUpMPU();
}

void loop() {
  readAndProcessAccelData();

  // Check for significant change in acceleration
  if (abs(gForceX - prevGForceX) > threshold ||
      abs(gForceY - prevGForceY) > threshold ||
      abs(gForceZ - prevGForceZ) > threshold) {
    
    if (printOnce) {
      Serial.print(" Accel (g)");
      printData();
      printOnce = false;  // Reset the flag to prevent continuous printing
    }
  } else {
    printOnce = true;  // Reset the flag when there is no significant change
  }

  // Store previous acceleration values for comparison
  prevGForceX = gForceX;
  prevGForceY = gForceY;
  prevGForceZ = gForceZ;

  delay(100);
}

void setUpMPU() {
  // power management
  Wire.beginTransmission(0b1101000);
  Wire.write(0x6B);
  Wire.write(0b00000000);
  Wire.endTransmission();

  // configure accelerometer
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C);
  Wire.write(0b00000000);
  Wire.endTransmission();  
}

void readAndProcessAccelData() {
  Wire.beginTransmission(0b1101000); 
  Wire.write(0x3B); 
  Wire.endTransmission();
  Wire.requestFrom(0b1101000,6); 
  while(Wire.available() < 6);
  accelX = Wire.read()<<8|Wire.read(); 
  accelY = Wire.read()<<8|Wire.read(); 
  accelZ = Wire.read()<<8|Wire.read(); 
  processAccelData();
}

void processAccelData() {
  gForceX = accelX/16384.0;
  gForceY = accelY/16384.0; 
  gForceZ = accelZ/16384.0;
}

void printData() {
  Serial.println("Acceleration (g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.println(gForceZ);
}
