#include <TimeLib.h> 
#define ARDUINO_POWER 23

#define VALIDATE_INPUT 44
#define BUSINPUT  46
#define IsBUSINPuT 48

#define signalUservalidated 25


int id [10] = {1,0,1,1,0,1,1,1,0,1};
boolean isUservalidated = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  pinMode(signalUservalidated, OUTPUT);
  digitalWrite(signalUservalidated, LOW);
  pinMode(ARDUINO_POWER, OUTPUT);
  digitalWrite(ARDUINO_POWER,LOW);
  pinMode(VALIDATE_INPUT, OUTPUT);
  digitalWrite(VALIDATE_INPUT,LOW);
  pinMode(BUSINPUT,INPUT);
  pinMode(IsBUSINPuT,INPUT);
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  userinput();
  allumecomand();
}

void userinput(){
  if(Serial.available() == 0)return;
  int arduinopowerdecision = Serial.parseInt();
  if(arduinopowerdecision==1){
    Serial.println("Voiture started ");
    digitalWrite(ARDUINO_POWER,HIGH);
  }else if(arduinopowerdecision==2){
    Serial.println("Voiture Stoped");
    digitalWrite(ARDUINO_POWER,LOW);
    digitalWrite(signalUservalidated, LOW);
  }
}

void allumecomand(){
  if(digitalRead(IsBUSINPuT)!=0){
    int val;
    Serial.print("Get id : ");
    if(digitalRead(BUSINPUT)==1){
      Serial.println("firs number = 1");
      digitalWrite(VALIDATE_INPUT,HIGH);
      boolean correctid = true;
      int bitinput;
      for(int i = 0; i<10; i++){
        bitinput = getinput();
        Serial.print(bitinput);
        if(bitinput != id[i]){
          Serial.print("wrong id nb at " + i);
          correctid = false;
        }
      }
      while(digitalRead(IsBUSINPuT)!=0){};
      digitalWrite(VALIDATE_INPUT,LOW);
      if(correctid){
        digitalWrite(signalUservalidated, HIGH);
        Serial.println("id validated");
      }
    }
    
  }
}

int getinput(){
  int val;
  while(digitalRead(IsBUSINPuT)!=0){};
  digitalWrite(VALIDATE_INPUT,LOW);
  while(digitalRead(IsBUSINPuT)==0){};
  val = digitalRead(BUSINPUT);
  digitalWrite(VALIDATE_INPUT,HIGH);
  return val;
}
