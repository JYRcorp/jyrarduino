// Source pour SPI: http://www.gammon.com.au/spi
//
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>
#include <unistd.h>

#define SWAP_2BYTES(x) (((x & 0xFFFF) >> 8) | ((x & 0xFF) << 8))

using namespace std;

int fd;
unsigned char data[16];
unsigned char tmpSpi;
int NBPINBYTES=16;  

int spi_value;

int spiTxRx(unsigned char txDat);
/**********************************************************
Main
  Setup SPI
    Open file spidev0.0 (chip enable 0) for read/write 
      access with the file descriptor "fd"
    Configure transfer speed (1MHz)
***********************************************************/

int main (void)
{


   fd = open("/dev/spidev0.0", O_RDWR);
   cout << "fd=" << fd << endl;

   unsigned int speed = 1000000;
   ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
   cout << "Done SPI_IOC_WR_MAX_SPEED_HZ" << endl;

   while (1)
   {
      // 8 entrées analogiques = 8 int = 16 bytes
      //
      // A very important point is that the slave cannot respond to an incoming byte at the same moment. 
      // The response has to be in the next byte. This is because the bits which are being sent, and the bits which are being received, are being sent simultaneously. 
      // On request 0 mais la réponse arrivera lors du request suivant
      for (int i=0; i<=NBPINBYTES; i++)
      {
         tmpSpi = spiTxRx( (unsigned char)  i    );
         if (i>0) {
          data[i-1]=tmpSpi;
         }
         //laisser du temps au slave
         usleep (1000); 
      }

      for (int i=0; i<NBPINBYTES; i++)
      {
         cout << "data[" << (i) << "]=" << (int)data[i] << endl;
      }

      //usleep (500000);
   }
}

/**********************************************************
spiTxRx
 Transmits one byte via the SPI device, and returns one byte
 as the result.

 Establishes a data structure, spi_ioc_transfer as defined
 by spidev.h and loads the various members to pass the data
 and configuration parameters to the SPI device via IOCTL

 Local variables txDat and rxDat are defined and passed by
 reference.  
***********************************************************/

int spiTxRx(unsigned char txDat)
{
 
  unsigned char rxDat;

  struct spi_ioc_transfer spi;

  memset (&spi, 0, sizeof (spi));

  spi.tx_buf        = (unsigned long)&txDat;
  spi.rx_buf        = (unsigned long)&rxDat;
  spi.len           = 1;

  ioctl (fd, SPI_IOC_MESSAGE(1), &spi);

  return rxDat;
}