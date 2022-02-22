#include <BleKeyboard.h>

BleKeyboard bleKeyboard;

struct Orientation {
  double Yaw;
  double Pitch;
  double Roll;
  bool Error;
};

//action system
int currentActionId = 0, exActionId = 0;
//const String actionNames[9] = {"BOS", "K", "KD", "D", "GD", "G", "GB", "B", "KB"};
float thresholds[6] = {0.3, 0.3, -0.3, -0.3, 0.25, -.25};
bool actionAllowed = true;
unsigned long actionStartTime;
unsigned int countdown;

//actions
const int countdowns[] = {0, 3, 7, 3, 3, 500, 3, 3, 3};
const int minCountdown = 300;
const int ON_ACTION = 16;
const int OFF_ACTION = 16;
const int GAS_ACTION = 1;
const int BRAKE_ACTION = 3;
const int TURN_RIGHT_ACTION = 5;
const int TURN_LEFT_ACTION = 6;
const int BT_1_ACTION = 2;
const int BT_2_ACTION = 4;

char carData = 'C';

struct Orientation o;

void setup() {
  initializeIMU();
  bleKeyboard.begin();
  Serial.begin(115200);
  Serial2.begin(9600);
}

void loop() {
  o = getIMUOrientation();
  checkActions();
  String a = "Error: " + String(o.Error) + ", ";
  a += "Yaw: " + String(o.Yaw) + ", ";
  a += "Pitch: " + String(o.Pitch) + ", ";
  a += "Roll: " + String(o.Roll) + ", ";
  a += "Action: " + String(currentActionId);
  sendToCar(carData);
  Serial.println(a);
  
}
void checkActions()
{
  //K-G
  if (-o.Pitch >= thresholds[0]) //K
  {
    currentActionId = 1;
  }
  else if (-o.Pitch < thresholds[2]) //G
  {
    currentActionId = 3;
  }
  //D-B
  else if (-o.Roll >= thresholds[1]) //D
  {
    currentActionId = 2;
  }
  else if (-o.Roll <= thresholds[3]) //B
  {
    currentActionId = 4;
  }
  else if(o.Yaw >= thresholds[4])
  {
    currentActionId = 5;
  }
  else if(o.Yaw < thresholds[5])
  {
    currentActionId = 6;
  }
  else
  {
    currentActionId = 0;
  } 
  if(exActionId != currentActionId)
    performAction(currentActionId);
  exActionId = currentActionId;
  /*
  if(currentActionId == exActionId)
    return;
  if (currentActionId == BRAKE_ACTION)
    brakeStart();
  if (exActionId != 0 && currentActionId == 0 && countdown != 0)
  {
    unsigned int timer = millis() - actionStartTime;
    if((minCountdown < timer) &&(timer <= countdown))
      performAction(currentActionId);
  }
  countdown = countdowns[currentActionId] * 1000;
  
  actionStartTime = millis();*/
}


void performAction(int i)
{

  if (i == 0)
  {
    carData = 'C';
  }
  else if (i == GAS_ACTION)
  {
    carData = 'G';
  }
  else if (i == BRAKE_ACTION)
  {
    carData = 'B';
  }
  else if (i == 5)
  {
    carData = 'R';
  }
  else if (i == 6)
  {
    carData = 'L';
  }
  else if (i == 2)
  {
    sendBt('1');
    //SEND BT SIGNAL
  }
  else if (i == 4)
  {
    sendBt('2');
    //SEND BT SIGNAL
  }
}

void sendToCar(char _data)
{
  Serial2.println(_data);
}

void sendBt(char _data)
{
  if (!bleKeyboard.isConnected())
    return; 
   bleKeyboard.print(_data);
  Serial.println(_data);
}
