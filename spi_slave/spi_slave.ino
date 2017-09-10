// Source pour SPI: http://www.gammon.com.au/spi
//
// 8 lectures analogiques possibles dans 8 int = 16 bytes
byte data[16];         // data= les données courantes
int NBPINBYTES=16;  
int indice;
int tmpval;
//char str[100]; //debug

void setup (void)
{
    //Serial.begin (115200);   // debugging
    
    // init tableau
    for(int i=0;i<NBPINBYTES;i++){
      data[i]=0;
    }      
    // entrées analogiques de A0 à A7
    for (int pin=A0; pin<=A7; pin++){
      pinMode(pin, INPUT );      // pin is an input
      digitalWrite(pin, LOW );   // pullup is off 
    }
  
    // turn on SPI in slave mode
    SPCR |= bit (SPE);
    // have to send on master in, *slave out*
    pinMode(MISO, OUTPUT);
    // now turn on SPI interrupt
    SPCR |= _BV(SPIE);
}


// SPI interrupt routine
ISR (SPI_STC_vect)
{
    // grab byte from SPI Data Register
    byte c = SPDR;  
    
    // A very important point is that the slave cannot respond to an incoming byte at the same moment. 
    // The response has to be in the next byte. This is because the bits which are being sent, and the bits which are being received, are being sent simultaneously. 
    // On request c mais la réponse arrivera au master lors de sa request suivante
    // renvoie la valeur du tableau data à l'indice demandé 
    indice = (int) c;
    if (indice<NBPINBYTES) {
        //sprintf(str, "data[%d]=%d ", indice, data[indice]); //debug
        //Serial.println(str);    //debug
        SPDR = data[indice];                 
    }
    else {
      SPDR = 0;
    }

}

void loop (void)
{
    // lectures des entrées analogiques A0 à A7 et mise en tableau des bytes
    for (int pin=A0; pin<=A3; pin++){
      tmpval = analogRead(pin);
      //sprintf(str, "A%d=%d - ", (pin-A0), tmpval); //debug
      //Serial.print(str); //debug
      data[ (pin-A0)*2   ] = (byte) (tmpval >> 8);    // high order byte
      data[((pin-A0)*2+1)] = (byte) (tmpval & 0xFF);  // low-order byte   
      //sprintf(str, "data[%d]=%d - data[%d]=%d - ", (pin-A0)*2, (tmpval >> 8), ((pin-A0)*2+1), (tmpval & 0xFF)); //debug
      //Serial.println(str);  //debug
    }
    //Serial.println(" ");
    delay(10);
} 
