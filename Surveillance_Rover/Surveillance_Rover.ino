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
int varFwd = 100;
int varTurn = 255;

//Servo definition
Servo fwd;
Servo turn;  
Servo lidarServo;

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
  //Set servo to pin A4 and A5
  fwd.attach(8, 1000, 2000);
  turn.attach(9, 1000, 2000);

  // Serial output
  Serial.begin(115200);
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

void calculateDirection(int distance, int pos, int *pFwd, int *pTurn)
{
  //The speed when turning needs to be > 100 in order to keep spinning
  //Fwd < 90 -> Rear
  //Fwd = 90 -> Stops
  //Fwd > 90 -> Forward
  //Turn < 90 -> Left
  //Turn = 90 -> Front
  //Turn > 90 -> Right
  if(pos < 90 && distance <= 120)
  {
    *pFwd = 90;
    *pTurn = 90;
  }
  else if(pos >= 90 && distance <= 120)
  {
    *pFwd = 90;
    *pTurn = 90;
  }
  else
  {
    *pFwd = 90;
    *pTurn = 90;
  }
}

void spinWheels(int varFwd, int varTurn)
{
  fwd.write(varFwd);
  
  turn.write(varTurn);
}

void loop() 
{
  for(pos = 30; pos <= 150; pos += 6)
  {
    lidarServo.write(pos);
    distance = lidarGetRange();
    calculateDirection(distance, pos, &varFwd, &varTurn);
    Serial.print("\n");
    Serial.print(distance);
    Serial.print("\t");
    Serial.print(pos);
    if (pos % 6 == 0)
      spinWheels(varFwd, varTurn);
  }
  for(pos = 150; pos>=30; pos -= 6)
  {
    lidarServo.write(pos);
    distance = lidarGetRange();
    calculateDirection(distance, pos, &varFwd, &varTurn);
    Serial.print("\n");
    Serial.print(distance);
    Serial.print("\t");
    Serial.print(pos);
    if (pos % 6 == 0)
      spinWheels(varFwd, varTurn);
  }
}
