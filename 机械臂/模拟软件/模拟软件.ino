#include<SoftwareSerial.h>
SoftwareSerial BTserial(2,3);
void setup() {
  Serial.begin(9600);
  BTserial.begin(9600);
  Serial.print("HC-06 DEMO/TEST ");
  BTserial.print("AT");
  pinMode(4,OUTPUT);

}

void loop() {
  if(BTserial.available()>0){
    char BTserialData=BTserial.read();
    Serial.print(BTserialData);
    if(BTserialData='1'){
      digitalWrite(4,HIGH);
    }
    else if(BTserialData=='0'){
      digitalWrite(4,LOW);
    }
  }
  if(Serial.available()>0){
    char serialData=Serial.read();
    BTserial.print(serialData);
  }
}
