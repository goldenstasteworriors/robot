#include<Servo.h>
Servo base,fArm,rArm,claw;
int basepos=90;
int fArmpos=90;
int rArmpos=90;
int clawpos=105;
const int basemin=0;
const int basemax=180;
const int fArmmax=90;
const int fArmmin=30;
const int rArmmax=160;
const int rArmmin=20;
const int clawmax=105;
const int clawmin=0;
int frompos;
int topos;
int DSD=15;//舵机运行速度
bool mode=1;//mode=1:指令模式；mode=0:手柄模式
int moveStep = 3;//每次按键舵机移动量

 

void setup() {
  Serial.begin(9600);
 base.attach(11);
 delay(200);
 rArm.attach(10);
 delay(200);
 fArm.attach(9);
 delay(200);
claw.attach(5);
 delay(200);
 base.write(90);
 delay(10);
 fArm.write(90);
 delay(10);
rArm.write(90);
 delay(10);
claw.write(105);
 delay(10);
 }
void servoCmd(char servoName,int topos,int servoDelay);
void reportStatus();


void loop() {
if(Serial.available()>0){
  char serialCmd = Serial.read();
  if(mode==1)armDataCmd(serialCmd);//指令模式
  else armJoyCmd(serialCmd);//手柄模式
 }
 
}
void armDataCmd(char serialCmd){
  if(serialCmd=='w'||serialCmd=='a'||serialCmd=='s'||serialCmd=='d'||serialCmd=='8'||serialCmd=='5'||serialCmd=='4'||serialCmd=='6'){
    Serial.println("+Warning:Robot in Instruction Mode...");
    delay(100);
    while(Serial.available()>0)char wrongCommand=Serial.read();
    return;
  }
  if(serialCmd=='b'||serialCmd=='f'||serialCmd=='r'||serialCmd=='c'){
    int servoData=Serial.parseInt();
    servoCmd(serialCmd,servoData,DSD);}
    else{
      switch(serialCmd){
        case 'm':
        Serial.print("Command: Switch to Joy-Stick Mode");
        mode=0;
        break;
        case 'o':reportStatus();
        break;
        case 'i':
        armIniPos();
        break;
        default:
        Serial.println("Unkown Command");

      }
    }}
void servoCmd(char servoName,int topos,int servoDelay){
  Servo servo2go;
  Serial.println("");
  Serial.print("+Command:Servo ");
  Serial.print(servoName);
  Serial.print(" to ");
  Serial.print(topos);
  Serial.print(" at sevoDelay value ");
  Serial.print(servoDelay);
  Serial.print(".");
  Serial.println("");

  int frompos;
  switch(servoName){
    case 'b':
    if(topos<=basemax&&topos>=basemin){
      servo2go=base;
      frompos=base.read();
      break;
    }
    else{
      Serial.println("Warning: Base Servo Value Out Of Limit!");
      return;}

      case 'f':
    if(topos<=fArmmax&&topos>=fArmmin){
      servo2go=fArm;
      frompos=fArm.read();
      break;
    }
    else{
      Serial.println("Warning: fArm Servo Value Out Of Limit!");
      return;}

      case 'r':
    if(topos<=rArmmax&&topos>=rArmmin){
      servo2go=rArm;
      frompos=rArm.read();
      break;
    }
    else{
      Serial.println("Warning: rArm Servo Value Out Of Limit!");
      return;}

      case 'c':
    if(topos<=clawmax&&topos>=clawmin){
      servo2go=claw;
      frompos=claw.read();
      break;
    }
    else{
      Serial.println("Warning: Claw Servo Value Out Of Limit!");
      return;}
    }
    if(frompos<=topos){
      for(int i=frompos;i<=topos;i++){
        servo2go.write(i);
        delay(servoDelay);
      }}
    else{for(int i=frompos;i>=topos;i--)
    {
      servo2go.write(i);
      delay(servoDelay);
    }
        }
    }
void reportStatus(){
  Serial.println("");
  Serial.println("");
  Serial.println("+ Robot-Arm Status Report +");
  Serial.print("Claw Position: ");Serial.println(claw.read());
  Serial.print("Base Position: ");Serial.println(base.read());
  Serial.print("Rear Arm Position: ");Serial.println(rArm.read());
  Serial.print("Front Arm Position: ");Serial.println(fArm.read());
  Serial.println("+++++++++++++++++++++++++++");
  Serial.println("");

} 
void armIniPos(){
  Serial.println("+Command:Restore Initial Position");
  int robotIniPosArray[4][3]={
    {'b',90,DSD},
    {'f',90,DSD},
    {'r',90,DSD},
    {'c',105,DSD}
  };
  for(int i=0;i<4;i++)servoCmd(robotIniPosArray[i][0],robotIniPosArray[i][1],robotIniPosArray[i][2]);
}
void armJoyCmd(char serialCmd){
  if(serialCmd=='m'){
    Serial.println("+Command: Switch to Instruction Mode");
    mode=1;
    return;}
    else if(serialCmd=='b'||serialCmd=='c'||serialCmd=='f'||serialCmd=='r'){
    Serial.println("+Warning: Robot in Joy-Stick Mode");
    while(Serial.available()>0)int wrongData=Serial.read();
    return;}
    else if(serialCmd=='i'){armIniPos();
    return;}
    else if(serialCmd=='o'){reportStatus();
    return;}
  int baseJoyPos;
  int rArmJoyPos;
  int fArmJoyPos;
  int clawJoyPos;
  switch(serialCmd){
    case 'a'://base向右
    Serial.println("Received Command: Base Turn Left");
    baseJoyPos = base.read()-moveStep;
    servoCmd('b',baseJoyPos,DSD);
    break;
    case 'd'://base向左
    Serial.println("Received Command: Base Turn Right");
    baseJoyPos = base.read()+moveStep;
    servoCmd('b',baseJoyPos,DSD);
    break;
    case 's'://rear Arm向前
    Serial.println("Received Command: Rear Arm Forward ");
    rArmJoyPos = rArm.read()-moveStep;
    servoCmd('r',rArmJoyPos,DSD);
    break;
    case 'w'://rear Arm向前
    Serial.println("Received Command: Rear Arm Backward");
    rArmJoyPos = rArm.read()+moveStep;
    servoCmd('r',rArmJoyPos,DSD);
    break;
    case '8'://front Arm向上
    Serial.println("Received Command: Front Arm Up");
    fArmJoyPos = fArm.read()+moveStep;
    servoCmd('f',fArmJoyPos,DSD);
    break;
     case '5'://front Arm向下
    Serial.println("Received Command: Front Arm Down");
    fArmJoyPos = fArm.read()-moveStep;
    servoCmd('f',fArmJoyPos,DSD);
    break;
    case '4'://claw关闭
    Serial.println("Received Command: Claw Close");
    clawJoyPos = claw.read()+moveStep;
    servoCmd('c',clawJoyPos,DSD);
    break;
    case '6'://claw打开
    Serial.println("Received Command: Claw Open");
    clawJoyPos = claw.read()-moveStep;
    servoCmd('c',clawJoyPos,DSD);
    break;
    

  }







}
