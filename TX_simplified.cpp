
  #include <SPI.h>
  #include <nRF24L01.h>
  #include <RF24.h>
  const uint64_t pipeOut = 000322;         
  RF24 radio(9, 10);                       

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
  data.throttle = 0;
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
    

  data.roll = Border_Map( analogRead(A3), 0, 512, 1023, true );        
  data.pitch = Border_Map( analogRead(A2), 0, 512, 1023, true );      
  data.throttle = Border_Map( analogRead(A1),570, 800, 1023, false );  
  // data.throttle = Border_Map( analogRead(A1),0, 512, 1023, false ); 
  data.yaw = Border_Map( analogRead(A0), 0, 512, 1023, true );        
  data.aux1 = Border_Map( analogRead(A4), 0, 512, 1023, true );        
  data.aux2 = Border_Map( analogRead(A5), 0, 512, 1023, true );        
  data.aux3 = digitalRead(7);
  data.aux4 = digitalRead(8);

  radio.write(&data, sizeof(Signal));  
}