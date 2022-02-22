#define INTERRUPT_PIN 19
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif
MPU6050 mpu;

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint8_t fifoBuffer[64]; // FIFO storage buffer

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

unsigned long timer;
void dmpDataReady() {
  mpuInterrupt = true;
}



struct Orientation prevOrientation;

void initializeIMU() {
  Wire.begin();
  Wire.setClock(400000);

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);
  
  
  devStatus = mpu.dmpInitialize();

  mpu.setXAccelOffset(-1748);
  mpu.setYAccelOffset(765);
  mpu.setZAccelOffset(1346);
  mpu.setXGyroOffset(93);
  mpu.setYGyroOffset(79);
  mpu.setZGyroOffset(-1);

  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print(F("DMP Initialization failed (code "));
  } 
}

struct Orientation getIMUOrientation() {
  
  if (!dmpReady || !mpu.testConnection()) {
    struct Orientation o;
    o.Yaw = 0;
    o.Pitch = 0;
    o.Roll = 0;
    o.Error = true;
    return o;
  }

  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer)) {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    struct Orientation o;
    o.Yaw = ypr[0];
    o.Pitch = ypr[2];
    o.Roll = ypr[1];
    o.Error = false;

    prevOrientation.Yaw = o.Yaw;
    prevOrientation.Pitch = o.Pitch;
    prevOrientation.Roll = o.Roll;

    return o;
  } else {
    return prevOrientation;
  }
}
