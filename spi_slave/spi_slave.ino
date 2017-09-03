#include <SPI.h>

char data[16];         // 8 lectures analogiques possibles dans 8 int = 16 bytes
byte NBPINBYTES=16;  
int temp;

void setup (void)
{
  Serial.begin (115200);   // debugging
  // turn on SPI in slave mode
  SPCR |= bit (SPE);
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  
  // init tableau
  for(int i=0;i<NBPINBYTES;i++){
    data[i]=0;
  }      

  // now turn on SPI interrupt
  SPI.attachInterrupt();
}


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  // grab byte from SPI Data Register
  byte c = SPDR;  
  
  // renvoie +1 à la valeur
  temp = (int) c;
  temp++;
  SPDR = (byte) temp;

}

// main loop - wait for flag set in interrupt routine
void loop (void)
{
    
} 
