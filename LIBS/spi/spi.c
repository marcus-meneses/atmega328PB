#include <spi/spi.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//SPI1 ----------------------------------------------------------------------

void spi1_init_master (void)
{
    // Set SS1 as OUTPUT and Pull it LOW BEFORE calling this function



    // DDRE=(1<<2);  //SS1  -> PE2
    // PORTE=(0<<2); //SS1  -> PE2
    // Set MOSI1, SCK1 as Output
    DDRC|=(1<<1); // SCK1 -> PC1
    DDRE|=(1<<3); // MOSI1 -> PE3
    // Set MISO1 as INPUT
    DDRC&=~(1<<0);  // MISO1 -> PC0
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16, Enable Interrupts
    //The MOSI, SCK pins are as per ATMega8

   SPCR1|=(1<<SPE1)|(1<<MSTR1)|(1<SPR10);
   SPSR1&=~(1<<SPI2X1);
 // Enable Global Interrupts  
 //sei();
}

// disable SPI Master Device;

void spi1_begin (void)
{
    DDRE|=(1<<2);  //SS1  -> PE2
    PORTE&=~(1<<2); //SS1  -> PE2
}


void spi1_end (void)
{
  //Set SS1 as OUTPUT and Pull it HIGH (might be input, but not sure about pullups
  //DDRE=(1<<2);  //SS1  -> PE2
  //PORTE=(1<<2); //SS1  -> PE2
  //disable SPI port 1 
    SPCR1&=~(1<<SPE1); // SPI disable, but master operation continued

    PORTE|=(1<<2); //SS1  -> PE2
}


//Function to send and receive data for both master and slave

uint8_t spi1_transmit (uint8_t data)
{
    // Load data into the buffer
    SPDR1 = data;
 
    //Wait until transmission complete
    while(!(SPSR1 & (1<<SPIF1)));   // Return received data

  return(SPDR1);
}

//---------------------------------------------------------------------------

//SPI0 ----------------------------------------------------------------------

void spi0_init_master (void)
{
     // Set SS0 as OUTPUT and Pull it HIGH BEFORE calling this function


    //DDRB=(1<<2);  //SS0  -> PB2
    //PORTB=(0<<2); //SS0  -> PB2
    // Set MOSI0, SCK0 as Output
    DDRB|=(1<<DDB5)|(1<<DDB3); // SCK0 -> PB5   MOSI0 -> PB3
    // Set MISO0 as INPUT
    DDRB&=~(1<<4);  // MISO0 -> PB4
    // Enable SPI, Set as Master
    // Prescaler: Fosc/16, Enable Interrupts
    //The MOSI, SCK pins are as per ATMega8

   SPCR0|=(1<<SPE)|(1<<MSTR)|(1<<SPR0);
   SPSR0&=~(1<<SPI2X);

 // Enable Global Interrupts  
 //sei();
}


void spi0_begin (void)
{
    DDRB|=(1<<2);  //SS0  -> PB2
    PORTB&=~(1<<2); //SS0  -> PB2
}


// disable SPI Master Device;

void spi0_end (void)
{
   // Set SS0 as OUTPUT and Pull it HIGH (might be input, but not sure about pullups
    //DDRB=(1<<2);  //SS1  -> PB2
    //PORTB=(1<<2); //SS1  -> PB2
  //disable SPI port 1 
    SPCR0&=~(1<<SPE); // SPI disable, but master operation continued
    PORTB|=(1<<2); //SS0  -> PB2
}


//Function to send and receive data for both master and slave

uint8_t spi0_transmit (uint8_t data)
{
    // Load data into the buffer
    SPDR0 = data;
 
    //Wait until transmission complete
    while(!(SPSR0 & (1<<SPIF)));   // Return received data

  return(SPDR0);
}

//---------------------------------------------------------------------------
