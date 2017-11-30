/*
Autonomous Surveillance Rover

By :
- Émile Bélanger
- Marc-André Bilodeau
- Gabriel Lapointe
- Stanislas Benajmin Nkawe
- Pierre-Olivier Otis
- Gabriel Tremblay
*/

/*
TO DO :
- Change servo type to Analog Type
- Do analogWrite between 0-5V (0-255)
- Change Sabertooth switch setting to : all 1 except for switch 4 which is off
  (note that the default setting given is 1 on, everything else off)
*/

//Include libraries
#include <Servo.h>
#include <Wire.h>

//LIDAR-Lite I2C comm definition
#define    LIDARLite_ADDRESS   0x62          // Default I2C Address of LIDAR-Lite.
#define    RegisterMeasure     0x00          // Register to write to initiate ranging.
#define    MeasureValue        0x04          // Value to initiate ranging.
#define    RegisterHighLowB    0x8f          // Register to get both High and Low bytes in 1 call.

//Variable definition
int yAxis = A1;
int xAxis = A2;
int yVal;
int xVal;
int i = 0;
int varFwd = 100;
int varTurn = 255;
int defaultSpeed = 110;
int waypoint;
int maxVal = 0;
int curVal = 0;
int index;
int count = 0;
int pushbutton = 0;

//Activate test mode
bool test = false;
bool lasttest = false;

double angle;
double object;

int ranges[12];

//Servo definition
Servo fwd;
Servo turn;  
Servo lidarServo;

//Pi definition
const double pi = 3.14159265359;

//LIDAR-Lite inital definitions
int pos = 75;         // Position of the servo (degress, [0, 180])
int distance = 0;    // Distance measured

//Boot song definition
int bootSong[] = { 294,349,587 };
const int noteCount = 3;
int noteLengts[] = { 125,125,500 };

//Main setup area
void setup() 
{
  //Set pushbutton to pin 8 as input
  pinMode(8, INPUT);
  
  //Set servo to pin A4 and A5
  fwd.attach(8, 1000, 2000);
  turn.attach(9, 1000, 2000);

  // Serial output
  Serial.begin(9600);
  Serial.println("< START >");
  
  //Set LIDAR servo control to pin 7
  lidarServo.attach(7);
  
  //LIDAR control
  Wire.begin(); // join i2c bus
  
  //BootSong
  for (int thisNote = 0; thisNote < noteCount; thisNote++)
  {
    int noteLengt = noteLengts[thisNote];
    tone(5, bootSong[thisNote],noteLengt);
    int pause = noteLengt;
    delay(pause);
    noTone(5);
  }
}

// Get a measurement from the LIDAR Lite
int lidarGetRange(void)
{
  int val = -1;
  
  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterMeasure); // sets register pointer to  (0x00)  
  Wire.write((int)MeasureValue); // sets register pointer to  (0x00)  
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit

  Wire.beginTransmission((int)LIDARLite_ADDRESS); // transmit to LIDAR-Lite
  Wire.write((int)RegisterHighLowB); // sets register pointer to (0x8f)
  Wire.endTransmission(); // stop transmitting

  delay(20); // Wait 20ms for transmit
  
  Wire.requestFrom((int)LIDARLite_ADDRESS, 2); // request 2 bytes from LIDAR-Lite

  if(2 <= Wire.available()) // if two bytes were received
  {
    val = Wire.read(); // receive high byte (overwrites previous reading)
    val = val << 8; // shift high byte to be high 8 bits
    val |= Wire.read(); // receive low byte as lower 8 bits
  }
  
  return val;
} 

void calculateDirection(int distance, int pos, int *pFwd, int *pTurn, int waypoint)
{
  //The speed when turning needs to be > 100 in order to keep spinning
  //Fwd < 90 -> Rear
  //Fwd = 90 -> Stops
  //Fwd > 90 -> Forward
  //Turn < 90 -> Left
  //Turn = 90 -> Front
  //Turn > 90 -> Right
  //Waypoint directions :
  //0 -> Forward
  //1 -> Left
  //2 -> Right

  angle = pos * (pi / 180);
  object = distance * sin(angle);
  
  if(waypoint == 0)
  {
    *pFwd = defaultSpeed;
    *pTurn = 90;
  }
  else if(waypoint == 1)
  {
    if(object <= 15)
    {
      *pFwd = 50;
      *pTurn = 180;
    }
    if(object > 15 && object <= 30)
    {
      *pFwd = defaultSpeed;
      *pTurn = 180;
    }
    else if(object > 30 && object <= 40)
    {
      *pFwd = defaultSpeed;
      *pTurn = 160;
    }
    else if(object > 40 && object <= 50)
    {
      *pFwd = defaultSpeed;
      *pTurn = 140;
    }
    else if(object > 50 && object <= 60)
    {
      *pFwd = defaultSpeed;
      *pTurn = 120;
    }
    else if(object > 60 && object <= 70)
    {
      *pFwd = defaultSpeed;
      *pTurn = 100;
    }
  }
  else if(waypoint == 2)
  {
    if(object <= 15)
    {
      *pFwd = 50;
      *pTurn = 0;
    }
    else if(object > 15 && object <= 30)
    {
      *pFwd = defaultSpeed;
      *pTurn = 0;
    }
    else if(object > 30 && object <= 40)
    {
      *pFwd = defaultSpeed;
      *pTurn = 20;
    }
    else if(object > 40 && object <= 50)
    {
      *pFwd = defaultSpeed;
      *pTurn = 40;
    }
    else if(object > 50 && object <= 60)
    {
      *pFwd = defaultSpeed;
      *pTurn = 60;
    }
    else if(object > 60 && object <= 70)
    {
      *pFwd = defaultSpeed;
      *pTurn = 80;
    }
  }
  else
  {
    *pFwd = defaultSpeed;
    *pTurn = 90;
  }
}

void spinWheels(int varFwd, int varTurn)
{
  if(test == true)
  {
    fwd.write(90);
    turn.write(90);
  }
  else
  {
    fwd.write(varFwd);
    turn.write(varTurn);
  }
  
}

void loop() 
{
  i = 0;

  pushbutton = digitalRead(8);
  if(pushbutton == HIGH) {
    if(lasttest == false) {
      test = true;
      lasttest = true;
    }
    else {
      test = false;
      lasttest = false;
    }
  }
  
  count = 0;
  for(pos = 30; pos <= 150; pos += 10)
  {
    lidarServo.write(pos);
    distance = lidarGetRange();
    ranges[i] = distance;
    i++;
    Serial.print("\n");
    Serial.print(distance);
    Serial.print("\t");
    Serial.print(pos);
    if(waypoint == 1 || waypoint == 2)
    {
      count++;
      if(count < 5)
      {
        spinWheels(varFwd, varTurn);
      }
      else
      {
        spinWheels(varFwd, 90);
      }
    }
    else
    {
      spinWheels(varFwd, varTurn);
    }
  }
  
  maxVal = 0;
  curVal = 0;
  
  for(i = 0; i < 20; i++)
  {
    curVal = ranges[i];
    
    if(curVal > maxVal)
    {
      maxVal = curVal;
      index = i;
    }
  }

  if(index >= 0 && index < 8)
  {
    waypoint = 1;
  }
  if(index >=8 && index <= 11)
  {
    waypoint = 0;
  }
  if(index > 11 && index <= 20)
  {
    waypoint = 2;
  }
  
  calculateDirection(distance, pos, &varFwd, &varTurn, waypoint);
  i = 0;
  count = 0;
  for(pos = 150; pos>=30; pos -= 10)
  {
    lidarServo.write(pos);
    distance = lidarGetRange();
    ranges[i] = distance;
    i++;
    Serial.print("\n");
    Serial.print(distance);
    Serial.print("\t");
    Serial.print(pos);
    if(waypoint == 1 || waypoint == 2)
    {
      count++;
      if(count < 5)
      {
        spinWheels(varFwd, varTurn);
      }
      else
      {
        spinWheels(varFwd, 90);
      }
    }
    else
    {
      spinWheels(varFwd, varTurn);
    }
  }

  maxVal = 0;
  curVal = 0;
  
  for(i = 0; i < 20; i++)
  {
    curVal = ranges[i];
    
    if(curVal > maxVal)
    {
      maxVal = curVal;
      index = i;
    }
  }

  if(index >= 0 && index < 8)
  {
    waypoint = 2;
  }
  if(index >=8 && index <= 11)
  {
    waypoint = 0;
  }
  if(index > 11 && index <= 20)
  {
    waypoint = 1;
  }

  calculateDirection(distance, pos, &varFwd, &varTurn, waypoint);
}
