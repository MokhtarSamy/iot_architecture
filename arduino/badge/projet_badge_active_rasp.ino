#include <SPI.h> 
#include <MFRC522.h>
#include <TimeLib.h> 
#include <SimpleSleep.h>

/* Je travaille avec une carte mega 2560 pour d'autre carte voir la description de celle si
 * MEGA 2560     UNO
 * RST 5         9
 * SDA(SS) 53    10
 * MOSI 51       11
 * MISO 50       12
 * SCK 52        13
 */
#define RST_PIN 9
#define SS_PIN 10
#define LIGHT_PIN 2


//communication bus
#define VALIDATE_INPUT 2
#define BUSINPUT  3
#define IsBUSINPuT 4

//raspberri input
#define RASPINPUT 6
#define RASP_IS_INPUT 7

int id [10] = {1,0,1,1,0,1,1,1,0,1};
int code [10]={1,0,1,1,0,1,1,0,1,1};

SimpleSleep Sleep;
byte nbPins = 0;
const byte nbPinsMax = 20;
byte specialpadge [] = {169, 255, 8, 186};
byte mypins[nbPinsMax][4];
int block=1;

/*you can get the unique port id for the arduino under TOOLS > Get Board Info my card is
 * VID: 1A86
 * PID: 7523
 */
byte data[16] = {"1A867523"};

char motsdepasse[6] = {'A','M','e','r','T','o'};
byte read_data[18];

MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;





void setup() {
  //printtoidlist(specialpadge);
  Serial.begin(9600);
  while (!Serial);
  printtoidlist(specialpadge);
  // hours minutes seconds days months years
  setTime(19, 15, 0, 8, 1, 2023);
  //printArray(mypins);  
  // put your setup code here, to run once:
  pinMode(LIGHT_PIN, OUTPUT);
  pinMode(VALIDATE_INPUT, INPUT);
  
  pinMode(RASPINPUT, INPUT);
  pinMode(RASP_IS_INPUT, INPUT);
  
  pinMode(BUSINPUT, OUTPUT);
  pinMode(IsBUSINPuT,OUTPUT);
  SPI.begin();      
  mfrc522.PCD_Init();
  delay(4);
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println("Bring the RFID Tag (Can be a card or keychain) to the RFID reader");
  Serial.println();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;  
  }
  
}

void printtoidlist(byte uid[]){
  writeatlist(nbPins,uid);
  nbPins+=1;
}
void writeatlist(byte pos, byte uid[4]){
  mypins[pos][0] = uid[0];
  mypins[pos][1] = uid[1];
  mypins[pos][2] = uid[2];
  mypins[pos][3] = uid[3];
}

void printArray( byte a[][4] ) {
   // loop through array's rows
   for ( int i = 0; i < nbPins; ++i ) {
      for ( int j = 0; j < 4; ++j ){
        Serial.print (a[ i ][ j ] );
      }
      Serial.println ( ) ;
   }
}
String timemodif(String string){
  if(string.length()==1){
    return "0"+string;
  }
  return string;
}


void givecode(){
  digitalWrite(BUSINPUT,HIGH);
  digitalWrite(IsBUSINPuT,HIGH);
  while(digitalRead(VALIDATE_INPUT)!=1){};
  digitalWrite(IsBUSINPuT,LOW);
  while(digitalRead(VALIDATE_INPUT)!=0){};
  for(int i = 0; i<10; i++){
    if(id[i]==1){
      digitalWrite(BUSINPUT,HIGH);
    }else{
      digitalWrite(BUSINPUT,LOW);
    }
    digitalWrite(IsBUSINPuT,HIGH);
    while(digitalRead(VALIDATE_INPUT)!=1){};
    digitalWrite(IsBUSINPuT,LOW);
    while(digitalRead(VALIDATE_INPUT)!=0){};
  }
  
}

boolean getcode() {
  Serial.println("Get input");
  boolean rightcode = true;
  int start = 1;
  for(int i = 0; i<10; i++){
    while(digitalRead(RASP_IS_INPUT)!= start){};
    if(start ==1) start = 0;
    else start = 1;
    if(code[i]!=digitalRead(RASPINPUT)){
      //Serial.print("Error ");
      rightcode=false;
    }
    //Serial.print(digitalRead(RASP_IS_INPUT));
    //Serial.println(digitalRead(RASPINPUT));
  }
  Serial.println(rightcode);
  if(rightcode==0){
    getcode();
  }
  return rightcode;
  
}


void loop() {
   Sleep.idleFor(500);
   
  if(Serial.available() != 0){
    Serial.println("get input var");
    for(int i =0; i<6; i++){
      if(Serial.read()!=motsdepasse[i]){
        return;
      }
    }
    Serial.println("mots passe valider : comand add 1: del 2: read 3");
    int integerVariable = Serial.parseInt();
    if(integerVariable==1){
      Serial.println("Badge the uid to add");
      while(!mfrc522.PICC_IsNewCardPresent() or !mfrc522.PICC_ReadCardSerial()){ Sleep.idle(); };
      if(testallvaliduid(mfrc522.uid.uidByte)){
         Serial.println("Badge is already added to reader");
      }else{
        Serial.println("Add badge to list");
        printtoidlist(mfrc522.uid.uidByte); 
      }
      printArray(mypins);
    }else if(integerVariable==2){
        Serial.println("Badge the uid to remove");
        while(!mfrc522.PICC_IsNewCardPresent() or !mfrc522.PICC_ReadCardSerial()){ Sleep.idle(); };
        
        if(removeuid(mfrc522.uid.uidByte)){
          Serial.println("Badge removed");
        }else{
          Serial.println("Badge is removed already"); 
        }  
      printArray(mypins);
    }else if(integerVariable==3){
      while(!mfrc522.PICC_IsNewCardPresent() or !mfrc522.PICC_ReadCardSerial()){Sleep.idle();};
      readallblocks();
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
    delay(400);
  }
  digitalWrite(LIGHT_PIN,LOW);
  if (!mfrc522.PICC_IsNewCardPresent()){
    return;
  }
  if(!mfrc522.PICC_ReadCardSerial()){
    return;
  }
  Serial.println('testit');
  if(testallvaliduid(mfrc522.uid.uidByte)){
    
    Serial.println("valide PIN");
    
    readBlock(1, read_data);
    //String answer = String((char *)read_data);
    Serial.println("answer at block");
    int block = (int) strtol((char*) read_data, 0, 16);
    Serial.println(block);
    Serial.println("done");
    //printBlock();
    //block++;
    String mytime =  "1A867523"+timemodif(String(minute()))+ timemodif(String(hour())) + timemodif(String(day()))+ timemodif(String(month()));//+String(year());
    Serial.println(mytime);
    byte data2[18];
    mytime.getBytes(data2, 18);
    //Serial.println(data2);
    writeBlock(block,data2);
    block++;
    if((block+1)%4 == 0){
      block+=1;
    }
    if(block>10){
      block=2;
    }
    byte datablock[4];
    String blockstring = String(block, HEX);
    if(blockstring.length()==1){
      blockstring= "0"+blockstring;
    }
    blockstring.getBytes(datablock,4);
    writeBlock(1, datablock);    
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
    if(getcode()){
      givecode();
    }
    delay(1000);
  }
  
}


void readallblocks(){
    Serial.println("history");
    for(int i = 2;i<10;i++){
      if((i+1)%4 == 0){
        i+=1;
      }
      readBlock(i, read_data);
      Serial.print("data at block ");
      Serial.println(i);
      char * mydata = (char *) read_data;
      Serial.print ("your padged on voiture  id : ");
      for (int j=0 ; j<8 ; j++){
        Serial.write (read_data[j]);
      }
      Serial.println();
      Serial.print("At clock : ");
      Serial.write(read_data[8]);
      Serial.write(read_data[9]);
      Serial.print(" m |");
      Serial.write(read_data[10]);
      Serial.write(read_data[11]);
      Serial.print(" h |");
      Serial.write(read_data[12]);
      Serial.write(read_data[13]);
      Serial.print(" d |");
      Serial.write(read_data[14]);
      Serial.write(read_data[15]);
      Serial.println(" m ");
    }
}

boolean testallvaliduid(byte test []){
  for(byte i = 0; i<nbPins;i++){
    Serial.print(i);
    if(compare(mypins[i],test)){
      return true;
    }
  }
  return false;
}

boolean removeuid(byte arr []){
  for(byte i = 0; i<nbPins;i++){
    if(compare(mypins[i],arr)){
      Serial.print("Find i ");
      Serial.println(i);
      schifft(i);
      return true;
    }
    i++;
  }
  return false;
}
void schifft(byte i){
  if(i<nbPins){
    writeatlist(i,mypins[i+1]);
    schifft(i+1);
  }else{
    byte nullarray [4] = {0,0,0,0}; 
    writeatlist(i,nullarray); 
    nbPins-=1;   
  }
}

boolean compare(byte arr1 [], byte arr2 []){
 if(sizeof(arr1) / sizeof(byte) != sizeof(arr2) / sizeof(byte)){
    return false;
  }
  for (byte i = 0; i < sizeof(arr2) / sizeof(int); i++) {
    if(arr1[i] != arr2[i]){
      return false;    
    }
  }
  return true; 
}

void printuid(){
  Serial.print("UID tag :");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print("  ");
    Serial.print(mfrc522.uid.uidByte[i]);
    Serial.print("  ");
  }
  Serial.println();
}

int writeBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  if (blockNumber > 2 && (blockNumber+1)%4 == 0){
    Serial.print(blockNumber);
    Serial.println(" is a trailer block");
    return 2;
  }
  Serial.print(blockNumber);
  Serial.println(" is a data block");
  

  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed: ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;
  }
  
  status = mfrc522.MIFARE_Write(blockNumber, arrayAddress, 16);
  if (status != MFRC522::STATUS_OK) {
           Serial.print("MIFARE_Write() failed: ");
           Serial.println(mfrc522.GetStatusCodeName(status));
           return 4;
  }
  Serial.println("Block has been written!");
}



int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  
  byte status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed (read): ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;
  }

  byte buffersize = 18;
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(status));
          return 4;
  }
  //Serial.println("Block read successfully!");
}

void printBlock(){
    Serial.print("Reading block ");
    for (int j=0 ; j<18 ; j++)
    {
      Serial.write (read_data[j]);
    }
    Serial.println(" finished");
}
