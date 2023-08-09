

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

int ch_width_1 = 0;
int ch_width_2 = 0;
int ch_width_3 = 0;
int ch_width_4 = 0;
int ch_width_5 = 0;
int ch_width_6 = 0;
int ch_width_7 = 0;
int ch_width_8 = 0;

Servo ch1;
Servo ch2;
Servo ch3;
Servo ch4;
Servo ch5;
Servo ch6;
Servo ch7;
Servo ch8;

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

const uint64_t pipeIn = 000322;
RF24 radio(9, 10); 

void ResetData()
{

data.throttle = 0;
data.roll = 127;
data.pitch = 127;
data.yaw = 127;
data.aux1 = 0;                                              
data.aux2 = 0;
data.aux3 = 0;
data.aux4 = 0;                                                     
}

void setup()
{

  ch1.attach(0);
  ch2.attach(2);
  ch3.attach(3);
  ch4.attach(4);
  ch5.attach(5);
  ch6.attach(6);
  ch7.attach(7);
  ch8.attach(8);
                                                           
  ResetData();                                             
  radio.begin();
  radio.openReadingPipe(1,pipeIn);
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);                          
  radio.setPALevel(RF24_PA_MAX);                            
  radio.startListening();                                   

}

unsigned long lastRecvTime = 0;

void recvData()
{
while ( radio.available() ) {
radio.read(&data, sizeof(Signal));
lastRecvTime = millis();                                    
}
}

void loop()
{
recvData();
unsigned long now = millis();
if ( now - lastRecvTime > 1000 ) {
ResetData();                                                
}

ch_width_1 = map(data.roll, 0, 255, 1000, 2000);
ch_width_2 = map(data.pitch, 0, 255, 1000, 2000); 
ch_width_3 = map(data.throttle, 0, 255, 1000, 2000); 
ch_width_4 = map(data.yaw, 0, 255, 1000, 2000); 
ch_width_5 = map(data.aux1, 0, 255, 1000, 2000); 
ch_width_6 = map(data.aux2, 0, 255, 1000, 2000); 
ch_width_7 = map(data.aux3, 0, 1, 1000, 2000); 
ch_width_8 = map(data.aux4, 0, 1, 1000, 2000); 


ch1.writeMicroseconds(ch_width_1);                          
ch2.writeMicroseconds(ch_width_2);
ch3.writeMicroseconds(ch_width_3);
ch4.writeMicroseconds(ch_width_4);
ch5.writeMicroseconds(ch_width_5);
ch6.writeMicroseconds(ch_width_6);                          
ch7.writeMicroseconds(ch_width_7);
ch8.writeMicroseconds(ch_width_8);


}