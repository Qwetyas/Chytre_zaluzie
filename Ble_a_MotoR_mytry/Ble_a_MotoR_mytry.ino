#define A 22
#define B 24
#define C 26
#define D 28
#define RX 50
#define TX 51
#define pinLED 13
#define NUMBER_OF_STEPS_PER_REV 512
#include <Stepper.h>

#include <SoftwareSerial.h>
const int LIGHT_PIN = A0; // Pin connected to voltage divider output
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 4660.0; // Measured resistance of 3.3k resistor
const float SERO = 14400.0;
int one_rotation = 515;
int full_motion = 3090;
float current_state = 0;
boolean wasDay;
//-----------------ESP-----------------------
#define DEBUG true
String mySSID = "UPC-Jiri-Kara";       // WiFi SSID
String myPWD = "W8hdyubpNssf"; // WiFi Password
String myAPI = "G4YEFVCJUSCBSXJV";   // API Key
String myHOST = "184.106.153.149";
String myPORT = "80";
String myFIELD = "field1";
String myFIELD2 = "field2"; 
String odesilanaHodnota = "1";
String odesilanaHodnota2 = "2";
//-------------------------------------------------

SoftwareSerial bluetooth(TX, RX);

void setup(){
   Serial.begin(9600);
  bluetooth.begin(9600);
  bluetooth.println("Arduino zapnuto, test Bluetooth.."); 
  pinMode(LIGHT_PIN, INPUT);
  pinMode(A,OUTPUT);
  pinMode(B,OUTPUT);
  pinMode(C,OUTPUT);
  pinMode(D,OUTPUT);
  Serial1.begin(115200);
  espData("AT+RST", 1000, DEBUG);                      //Reset the ESP8266 module
  espData("AT+CWMODE=1", 1000, DEBUG);                 //Set the ESP mode as station mode
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   //Connect to WiFi network
  espData("AT+CIFSR", 1000, DEBUG);
  //delay(1000);
}
void funkce1(){
  
  String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+float(current_state);
  espData("AT+CIPMUX=0", 1000, DEBUG); 
  espData("AT+CIPSTART=\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
  //espData("AT+CIPSEND=0," +String(sendData.length()),1000,DEBUG); 
  Serial1.println("AT+CIPSEND=51");
  Serial1.find(">"); 
  Serial1.println(sendData);
  espData("AT+CIPCLOSE=0",1000,DEBUG);
}
String espData(String command, const int timeout, boolean debug){
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  Serial1.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (Serial1.available())
    {
      char c = Serial1.read();
      response += c;
    }
  }

    Serial.print(response);

  return response;
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
 funkce1();
  
  if (bluetooth.available() > 0) {
      BluetoothData = bluetooth.read();
     bluetooth.println(BluetoothData);
    switch (BluetoothData) {
      case '0': //0%
       nahoru(full_motion * current_state);
        current_state = 0;
        bluetooth.println(current_state);
        break;
      case '1': //100%
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
         case '5': // automatic
      boolean ahoj = true;
      boolean was = true;
        while(ahoj){
          funkce1();     
          if (BluetoothData==55)ahoj=false;     
          int lightADC = analogRead(LIGHT_PIN);       
          if (lightADC < 0)continue;   
          float lightV = lightADC * VCC / 1023.0;
          float lightR = R_DIV * (VCC / lightV - 1.0);
          Serial.println("Voltage: " + String(lightV) + " V");
          Serial.println("Resistance: " + String(lightR) + " ohms");
          boolean isDay = lightR <= SERO;
          if(was){
            wasDay=!isDay;
            was = false;
            continue;
          }
          if (isDay && !wasDay){
            nahoru(full_motion * current_state);
            bluetooth.println("vytahuju");
            current_state = 0;
            wasDay = isDay;
            continue;
          }          
          if(!isDay && wasDay){
            dolu(full_motion * (1 - current_state));
             bluetooth.println("zatahuju");
             current_state = 1;
             wasDay = isDay;
            continue;
          }           
          delay(15000); 
          BluetoothData = bluetooth.read();            
      }
      break;
    }
  }
  delay(100); 
}
