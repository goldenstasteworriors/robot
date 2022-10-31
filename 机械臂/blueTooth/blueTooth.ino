char serialData;
 
void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT); //11引脚连接演示用LED
 
}
 
void loop(){
  if( Serial.available()>0 ){ 
    
    serialData =  Serial.read();   
    
    if (serialData == '1' ) {  //接收到点亮LED指令
      Serial.print("Got command: ");  Serial.println(serialData); 
      Serial.println("LED-ON");
      
      digitalWrite(3, HIGH);  //点亮LED指令
    } else {    //接收到熄灭LED指令
      Serial.print("Got command: ");  
      Serial.println(serialData); 
      Serial.println("LED-OFF");
 
      digitalWrite(3, LOW);      //熄灭LED指令  
    }      
  }
}
