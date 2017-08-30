//
// Lecture analogique de A0, A1, A2, A4
// + renvoi des données en I2C
//
#include <Wire.h>
// I2C address : choisir de 0x03 à 0x77 
#define SLAVE_ADDRESS 0x12
int readData=0;
char data[12];         // 6 lectures analogiques possibles dans 6 int = 12 bytes
byte NBPINBYTES=12;  
// LED
int boucle=0;
int MAXBOUCLE=180;
int MIDBOUCLE=160;

void setup() {
    // initialize digital pin LED_BUILTIN as an output.
    pinMode(LED_BUILTIN, OUTPUT);    
    // pins analogiques
    pinMode(A0, INPUT );      // ensure button pin is an input
    digitalWrite(A0, LOW );   // ensure pullup is off on button pin
    pinMode(A1, INPUT );      // ensure button pin is an input
    digitalWrite(A1, LOW );   // ensure pullup is off on button pin
    pinMode(A2, INPUT );      // ensure button pin is an input
    digitalWrite(A2, LOW );   // ensure pullup is off on button pin
    pinMode(A4, INPUT );      // ensure button pin is an input
    digitalWrite(A4, LOW );   // ensure pullup is off on button pin
    // I2C
    Wire.begin(SLAVE_ADDRESS); // join i2c bus (address optional for master)
    Wire.onReceive(receiveData); // register event
    Wire.onRequest(sendData);  
    // init tableau
    for(int i=0;i<NBPINBYTES;i++){
      data[i]=0;
    }      
}

void loop() {
    boucle++;
    if(boucle==MIDBOUCLE){
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    }   
    if(boucle==MAXBOUCLE){
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      boucle=0;
    }  

    int tmpval = analogRead(A0);
    data[0] = (byte) (tmpval >> 8);    // high order byte
    data[1] = (byte) (tmpval & 0xFF);  // low-order byte     
        
    tmpval = analogRead(A1);
    data[2] = (byte) (tmpval >> 8);    // high order byte
    data[3] = (byte) (tmpval & 0xFF);  // low-order byte   
    
    tmpval = analogRead(A2);
    data[4] = (byte) (tmpval >> 8);    // high order byte
    data[5] = (byte) (tmpval & 0xFF);  // low-order byte   
    
    tmpval = analogRead(A4);
    data[6] = (byte) (tmpval >> 8);    // high order byte
    data[7] = (byte) (tmpval & 0xFF);  // low-order byte   
    
    delay(10);
}

void receiveData(int byteCount){
  // Reception de l'indice du PIN demandé (A0 byte1 =0; A0 byte2 =1...  A1 ... A2 ...  ya pas A3  mais A4 )
    while(Wire.available()) {
      readData = Wire.read();
    }
}

void sendData(){
  // Renvoi des 2 bytes de la valeur demandée par receiveData
    Wire.write( data[readData] );
    Wire.write( data[readData+1] );
}



