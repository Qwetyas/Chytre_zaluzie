#define A 22
#define B 24
#define C 26
#define D 28
#define RX 50
#define TX 51
#define pinLED 13
#include <SoftwareSerial.h>
 
#define NUMBER_OF_STEPS_PER_REV 512
#include <Stepper.h>
SoftwareSerial bluetooth(TX, RX);
int one_rotation = 515;
int full_motion = 3090;
float current_state = 0;

void setup(){
  bluetooth.begin(9600);
  bluetooth.println("Arduino zapnuto, test Bluetooth..");
  // nastavení pinu s LED diodou jako výstup
  pinMode(pinLED, OUTPUT);
  
pinMode(A,OUTPUT);
pinMode(B,OUTPUT);
pinMode(C,OUTPUT);
pinMode(D,OUTPUT);
int rychlost=0.5;
}

void write(int a,int b,int c,int d){
digitalWrite(A,a);
digitalWrite(B,b);
digitalWrite(C,c);
digitalWrite(D,d);
}

void nahoru(int otocek){
  for(int i = 0; i < otocek; i++){
    write(10,0,0,0);
    delay(1);
    write(10,10,0,0);
    delay(1);
    write(0,10,0,0);
    delay(1);
    write(0,10,10,0);
    delay(1);
    write(0,0,10,0);
    delay(1);
    write(0,0,10,10);
    delay(1);
    write(0,0,0,10);
    delay(1);
    write(10,0,0,10);
    delay(1);
  }
}

void dolu(int otocek){
  for(int i = 0; i < otocek; i++){
    write(0,0,0,1);
    delay(1);
    write(0,0,1,1);
    delay(1);
    write(0,0,1,0);
    delay(1);
    write(0,1,1,0);
    delay(1);
    write(0,1,0,0);
    delay(1);
    write(1,1,0,0);
    delay(1);
    write(1,0,0,0);
    delay(1);
    write(1,0,0,1);
    delay(1);
  }
}

void loop() {
  byte BluetoothData;
  if (bluetooth.available() > 0) {
      BluetoothData = bluetooth.read();
    
     
    switch (BluetoothData) {
      case '0': //zatemni
       nahoru(full_motion * current_state);
        current_state = 0;
        bluetooth.println(current_state);
        break;
      case '1': //odtemni
       dolu(full_motion * (1 - current_state));
        current_state = 1;
        bluetooth.println(current_state);
        break;
      case '2': //zatemni o 50%
        if ( current_state > 0.5 ) {
          //z vetsi casti zatemnenejsi
          nahoru(full_motion * (current_state - 0.5 ));          
        } else if ( current_state < 0.5 ) {
          //z vetsi casti odtemnenejsi
          dolu(full_motion * (0.5 - current_state));          
        } else {
          //current_state uz je na 0.5         
          }
          current_state = 0.5;
          bluetooth.println(current_state);
        break;
        case '3': //zatemni o 25%
        if ( current_state > 0.25 ) {
          //z vetsi casti zatemnenejsi
          nahoru(full_motion * (current_state - 0.25 ));
          
        } else if ( current_state < 0.25 ) {
          //z vetsi casti odtemnenejsi
          dolu(full_motion * (0.25 - current_state));        
        } else {
          //current_state uz je na 0.25        
          }
          current_state = 0.25;
          bluetooth.println(current_state);
        break;
       case '4': //zatemni o 75%
        if ( current_state > 0.75 ) {
          //z vetsi casti zatemnenejsi
          nahoru(full_motion * (current_state - 0.75 ));          
        } else if ( current_state < 0.75 ) {
          //z vetsi casti odtemnenejsi
          dolu(full_motion * (0.75 - current_state));         
        } else {
          //current_state uz je na 0.75         
          }
          current_state = 0.75;
          bluetooth.println(current_state);
        break;     
    }
  }
  delay(100);
  //tohle je testik
}
