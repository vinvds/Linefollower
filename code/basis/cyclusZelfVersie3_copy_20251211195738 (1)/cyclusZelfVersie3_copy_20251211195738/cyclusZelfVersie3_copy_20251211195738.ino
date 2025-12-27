#include "SerialCommand.h"
#include "EEPROMAnything.h"
#include <Arduino.h>

// Definieer Bluetooth of Serial poort
#define SerialPort Serial
#define Baudrate 115200
SoftwareSerial BTSerial(7, 4); // RX | TX wijzigen
//pinnen nog wijzigen 
#define MotorLeftFoward PA8
#define MotorLeftBackward PA9
#define MotorRightFoward PA10
#define MotorRightBackward PB6

SerialCommand sCmd(SerialPort);
bool debug;
bool run; 
unsigned long previous, calculationTime;

const int sensor[] = {A0, A1, A2, A3, A4, A5, A6, A7};

struct param_t
{
  unsigned long cycleTime;
  int black[8];
  int white[8];
  int power;
  float diff;
  flaot kp;
  /* andere parameters die in het eeprom geheugen moeten opgeslagen worden voeg je hier toe ... */
} params;

int normalised[8];
float debugPosition;

void onUnknownCommand(char *command);
void onSet();
void onDebug();
void onCalibrate();
void onRun();


void setup()
{
  SerialPort.begin(Baudrate);

  sCmd.addCommand("set", onSet);
  sCmd.addCommand("debug", onDebug);
  sCmd.addCommand("calibratie", onCalibrate);
  sCmd.addCommand("run", onRun);
  sCmd.setDefaultHandler(onUnknownCommand);

  EEPROM_readAnything(0, params);

  pinMode(13, OUTPUT);
  SerialPort.println("ready");
}

void loop()
{
  digitalWrite(8, HIGH);
  sCmd.readSerial();
 
  unsigned long current = micros();
  if (current - previous >= params.cycleTime)
  {
    previous = current;
    

   // SerialPort.print("normalised values: ");
    
    for (int i = 0; i < 8; i++)
    {
      
      normalised[i] = map(analogRead(sensor[i]), params.black[i], params.white[i], 0, 4096);
      //SerialPort.print(normalised[i]);
      //SerialPort.print(" ");
    }
    //SerialPort.println();

    float position = 0; 
    int index = 0;
    for (int i = 1; i < 8; i++) if (normalised[i] < normalised[index]) index = i;
   
   if (normalised[index] > 3000) run = false;
    
    if (index == 0) position = -50;
    else if (index == 7) position = 50;  
    else{
      long sNul = normalised[index];
    long sMinusEen = normalised[index-1];
    long sPlusEen = normalised[index+1];


    float b =  sPlusEen - sMinusEen;
    
    b = b/2;
    float a = sPlusEen - b - sNul;
     position = -b / (2 * a);  
    position += index;
    position -= 3.5;   
    position *= 15;  //sensor distance in mm

   

    }
    debugPosition = position; 
    //SerialPort.print("positie: ");
    //SerialPort.println(position);
    float error = -position; 
    float output = error * params.kp; 

    output = constrain(output, -510, 510);

    int powerLeft = 0;
    int powerRight = 0; 

    if (run) (output >= 0)
      {
        powerLeft = constrain(params.power + params.diff * output, -255, 255);
        powerRight = constrain(powerLeft - output, -255, 255);
        powerLeft = powerRight + output;
      }
    else
    {
      powerRight = constrain(params.power - params.diff * output, -255, 255);
      powerLeft = constrain(powerRight + output, -255, 255);
      powerRight = powerLeft - output;
    }
    analogWrite(MotorLeftFoward,  powerLeft  > 0 ?  powerLeft  : 0);
    analogWrite(MotorLeftBackward, powerLeft < 0 ? -powerLeft : 0);

    analogWrite(MotorRightFoward,  powerRight > 0 ?  powerRight : 0);
    analogWrite(MotorRightBackward, powerRight < 0 ? -powerRight : 0);

    
  }

  unsigned long difference = micros() - current;
  if (difference > calculationTime) calculationTime = difference;
}

void onUnknownCommand(char *command)
{
  SerialPort.print("unknown command: \"");
  SerialPort.print(command);
  SerialPort.println("\"");
}

void onSet()
{
  char* param = sCmd.next();
  char* value = sCmd.next();  
  
  if (strcmp(param, "cycle") == 0) params.cycleTime = atol(value);
  else if (strcmp(param, "power") == 0)  params.power = atol(value);
  else if (strcmp(param, "diff")  == 0)  params.diff  = atof(value);
  else if (strcmp(param, "kp")    == 0)  params.kp    = atof(value);

  /* parameters een nieuwe waarde geven via het set commando doe je hier ... */
  
  EEPROM_writeAnything(0, params);
}

void onDebug()
{
  SerialPort.print("cycle time: ");
  SerialPort.println(params.cycleTime);
  
  /* parameters weergeven met behulp van het debug commando doe je hier ... */
  SerialPort.print("black: ");
  for (int i = 0; i < 8; i++)
{
  SerialPort.print(params.black[i]);
  SerialPort.print(" ");
}
SerialPort.println(" ");


 SerialPort.print("white: ");
  for (int i = 0; i < 8; i++)
{
  SerialPort.print(params.white[i]);
  SerialPort.print(" ");
}
SerialPort.println(" ");


  SerialPort.print("normalised: "):
  for (int i = 0; i < 8; i++){
    SerialPort.print((normalised[i]));
    SerialPort.print(" ");
  }
  SerialPort.println(" ");

  SerialPort.print("position: ");
  SerialPort.println(debugPosition);

  SerialPort.print("power: ");
  SerialPort.println(params.power);
  SerialPort.print("diff: ");
  SerialPort.println(params.diff);
  SerialPort.print("kp: ");
  SerialPort.println(params.kp);



  SerialPort.print("calculation time: ");
  SerialPort.println(calculationTime);
  calculationTime = 0;
}

void onCalibrate()
{
  char* param = sCmd.next();

  if (strcmp(param, "black") == 0)
  {
    SerialPort.print("start calibrating black... ");
    for (int i = 0; i < 8; i++) params.black[i]=analogRead(sensor[i]);
    SerialPort.println("done");
  }
  else if (strcmp(param, "white") == 0)
  {
    SerialPort.print("start calibrating white... ");    
    for (int i = 0; i < 8; i++) params.white[i]=analogRead(sensor[i]);  
    SerialPort.println("done");      
  }

  EEPROM_writeAnything(0, params);
}

void onRun()
{
    //wijzigen
    run = true;
  SerialPort.println("Run started");
}

