
  #include <SPI.h>
  #include <nRF24L01.h>
  #include <RF24.h>
  #include <EEPROM.h>
    
  const uint64_t pipeOut = 000322;        
  RF24 radio(9, 10);                      
  
 #define trimbut_1 1                      
 #define trimbut_2 2                      
 #define trimbut_3 3                      
 #define trimbut_4 4                      
 #define trimbut_5 5                      
 #define trimbut_6 6                      
 
 int tvalue1 = EEPROM.read(1) * 4;        
 int tvalue2 = EEPROM.read(3) * 4;        
 int tvalue3 = EEPROM.read(5) * 4;        

  struct Signal {
  byte throttle;
  byte pitch;
  byte roll;
  byte yaw;
  byte aux1;
  byte aux2;
  byte aux3;
  byte aux4;  
};

  Signal data;
  void ResetData() 
{
  data.throttle = 512;                      
  data.pitch = 127;
  data.roll = 127;
  data.yaw = 127;
  data.aux1 = 0;                         
  data.aux2 = 0;
  data.aux3 = 0;
  data.aux4 = 0;
}
  void setup()
{

  radio.begin();
  radio.openWritingPipe(pipeOut);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);       
  radio.setPALevel(RF24_PA_MAX);         
  radio.stopListening();                 
  ResetData();
 
  pinMode(trimbut_1, INPUT_PULLUP); 
  pinMode(trimbut_2, INPUT_PULLUP);
  pinMode(trimbut_3, INPUT_PULLUP); 
  pinMode(trimbut_4, INPUT_PULLUP);
  pinMode(trimbut_5, INPUT_PULLUP); 
  pinMode(trimbut_6, INPUT_PULLUP);

  tvalue1= EEPROM.read(1) * 4;
  tvalue2= EEPROM.read(3) * 4;
  tvalue3= EEPROM.read(5) * 4;
}


  int Border_Map(int val, int lower, int middle, int upper, bool reverse)
{
  val = constrain(val, lower, upper);
  if ( val < middle )
  val = map(val, lower, middle, 0, 128);
  else
  val = map(val, middle, upper, 128, 255);
  return ( reverse ? 255 - val : val );
}
  void loop()
{



  if(digitalRead(trimbut_1)==LOW and tvalue1 < 630) {
    tvalue1=tvalue1+15;
    EEPROM.write(1,tvalue1/4); 
    delay (130);
  }   
  if(digitalRead(trimbut_2)==LOW and tvalue1 > 280){
    tvalue1=tvalue1-15;
    EEPROM.write(1,tvalue1/4);
    delay (130);
  }
 
  if(digitalRead(trimbut_3)==LOW and tvalue2 < 630) {
    tvalue2=tvalue2+15;
    EEPROM.write(3,tvalue2/4);
    delay (130);
  }   
  if(digitalRead(trimbut_4)==LOW and tvalue2 > 280){
    tvalue2=tvalue2-15;
    EEPROM.write(3,tvalue2/4);
    delay (130);
  }  
    
    if(digitalRead(trimbut_5)==LOW and tvalue3 < 630) {
     tvalue3=tvalue3+15;
     EEPROM.write(5,tvalue3/4);
     delay (130);
  }   
  if(digitalRead(trimbut_6)==LOW and tvalue3 > 280){
    tvalue3=tvalue3-15;
    EEPROM.write(5,tvalue3/4);
    delay (130);
  }
  


  data.roll = Border_Map( analogRead(A3), 0, tvalue1, 1023, true );       
  data.pitch = Border_Map( analogRead(A2), 0, tvalue2, 1023, true );            
  data.throttle = Border_Map( analogRead(A1),570, 800, 1023, false );    
  
  data.yaw = Border_Map( analogRead(A0), 0, tvalue3, 1023, true );              
  data.aux1 = Border_Map( analogRead(A4), 0, 512, 1023, true );                 
  data.aux2 = Border_Map( analogRead(A5), 0, 512, 1023, true );                 
  data.aux3 = digitalRead(7);
  data.aux4 = digitalRead(8);
  radio.write(&data, sizeof(Signal)); 
}