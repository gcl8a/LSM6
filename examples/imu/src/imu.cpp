// This example reads the raw values from the LSM6DS33
// accelerometer and gyro and prints those raw values to the
// serial monitor.

#include <FastGPIO.h>
#include <Romi32U4Buttons.h>
#include <Romi32U4Encoders.h>
#include <Romi32U4Motors.h>
#include <PCint.h>

#include <Wire.h> //not used in many codes, but platformio balks without it, since it scans the IMU library
#include <LSM6.h>

#include <PIDcontroller.h>

Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;
Romi32U4Motors motors;

LSM6 imu;

void setup()
{
  Wire.begin();

  if (!imu.init())
  {
    // Failed to detect the LSM6.
    while(1)
    {
      Serial.println(F("Failed to detect the LSM6."));
      delay(100);
    }
  }

  // Set the gyro full scale and data rate
  imu.setGyroDataOutputRate(LSM6::ODR104);

  // Set the accelerometer full scale and data rate
  imu.setAccDataOutputRate(LSM6::ODR104);
}

bool showAcc = true;
bool showGyro = false;

void loop()
{
  if(buttonA.isPressed()) motors.setEfforts(100, -100);
  else motors.setEfforts(0,0);

  if(buttonB.getSingleDebouncedPress()) showAcc = !showAcc;
  if(buttonC.getSingleDebouncedPress()) showGyro = !showGyro;

  if(imu.getStatus() & 0x01)
  {
    imu.read();

    // Serial.print(millis());
    // Serial.print('\t');

    if(showAcc)
    {
      Serial.print(imu.a.x);
      Serial.print(' ');
      Serial.print(imu.a.y);
      Serial.print(' ');
      Serial.print(imu.a.z);
      Serial.print(' ');

      // print out the pitch angle, in degrees
      Serial.print(180.0 * atan2(-imu.a.x, imu.a.z) / 3.1416);
      Serial.print(' ');
    }

    if(showGyro)
    {
      static float angle = 0;
      angle+=imu.g.y * 0.00962; //  = integrate the gyro to get position; assumes DR = 104Hz

      Serial.print(imu.g.x);
      Serial.print(' ');
      Serial.print(imu.g.y);
      Serial.print(' ');
      Serial.print(imu.g.z);
      Serial.print(' ');
      Serial.print(angle);
      Serial.print(' ');
    }

    Serial.print('\n');
  }
}
