
#include <avr/io.h> 
#ifndef ATMEGA_PINS_H
#define ATMEGA_PINS_H

  #if defined(__AVR_ATmega644__) || defined(__AVR_ATmega644P__)
      #define PINADC			PINA
      #define PORTADC			PORTA
      #define DDRADC			DDRA
      #define PORT_SPI1			PORTB
      #define SPI1_SS			4
      #define SPI1_SCK			7
      #define SPI1_MOSI			5
      #define SPI1_MISO			6
      #define PIN_SPI1			PINB
      #define DDR_SPI1			DDRB
  #elif defined(__AVR_ATmega328P__)
      #define PINADC			PINC
      #define PORTADC			PORTC
      #define DDRADC			DDRC
      #define PIN_SPI1                  PINB
      #define DDR_SPI1                  DDRB
      #define PORT_SPI1                 PORTB
      #define SPI1_SS                   2
      #define SPI1_SCK                  5
      #define SPI1_MOSI                 3
      #define SPI1_MISO                 4

  #elif defined(__AVR_ATmega2560__)
      #define PINADC			PINF
      #define PORTADC			PORTF
      #define DDRADC			DDRF
      #define PORT_SPI1			PORTB
      #define SPI1_SS			0
      #define SPI1_SCK			1
      #define SPI1_MOSI			2
      #define SPI1_MISO			3
      #define PIN_SPI1			PINB
      #define DDR_SPI1			DDRB
     
  #endif

#endif
