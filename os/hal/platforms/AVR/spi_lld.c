/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    templates/spi_lld.c
 * @brief   SPI Driver subsystem low level driver source template.
 *
 * @addtogroup SPI
 * @{
 */

#include "ch.h"
#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/
#if USE_AVR_SPI1 || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif
#if USE_AVR_SPI2 || defined(__DOXYGEN__)
SPIDriver SPID2;
#endif
/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
static void spi_start_transmission(SPIDriver *spip)
{
#if USE_AVR_SPI1 || defined(__DOXYGEN__)
    if(spip->untransmitted_bytes> 0)
    {
        if(spip->tx_buffer != NULL)
        {
            SPDR =  *spip->tx_buffer;
            spip->tx_buffer++;
        }
        else
        {
            volatile uint8_t tempSPDR = SPDR;
            SPDR = tempSPDR;
        }
        spip->untransmitted_bytes--;
    }

#endif


}

static void spi_setup_transmission(SPIDriver *spip,size_t n, const uint8_t *txbuf, uint8_t *rxbuf)
{
    spip->tx_buffer= txbuf;
    spip->rx_buffer= rxbuf;
    spip->untransmitted_bytes= n;

}
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/
#if USE_AVR_SPI1 || defined(__DOXYGEN_)
CH_IRQ_HANDLER(SPI_STC_vect)   //SPI1 interrupt
{

    CH_IRQ_PROLOGUE();

    if(SPCR & (1<<MSTR))
    {
        if(SPID1.rx_buffer != NULL)
        {
            *SPID1.rx_buffer=SPDR;
            SPID1.rx_buffer++;
        }
        if(SPID1.untransmitted_bytes> 0)
        {
            spi_start_transmission(&SPID1);
        }
        else
        {
            _spi_isr_code(&SPID1);

        }
    }
    else
    {
        if(SPID1.config->slave_cb == NULL)
            SPDR = SPID1.config->slave_cb(&SPID1,SPDR);
        SPCR |=(1<<MSTR);

    }

    CH_IRQ_EPILOGUE();
}
#endif
/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void)
{


#if USE_AVR_SPI1 || defined(__DOXYGEN__)
    spiObjectInit(&SPID1);

#endif



}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip)
{

    if (spip->state == SPI_STOP)
    {
        /* Clock activation.*/
#if USE_AVR_SPI1 || defined(__DOXYGEN__)
        if(spip == &SPID1)
        {
            SPCR = (1<<MSTR)|
                   (1<<SPIE)| //enable interrupt
                   (1<<SPR1)|(1<<SPR0)| //Clk/128
                   ((spip->config->spi_mode & 0x3)<<CPHA);
        }
#endif

    }
    /* Configuration.*/
#if USE_AVR_SPI1 || defined(__DOXYGEN__)
    if(spip == &SPID1)
    {
        /*mosi, sck and ss output*/
        PORT_SPI1 |= (1<<SPI1_SCK)|(1<<SPI1_MOSI)|(1<<SPI1_SS);
        PORT_SPI1 &= ~(1<<SPI1_MISO);
        SPCR |= (1<<SPE);
    }
#endif
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip)
{
#if USE_AVR_SPI1 || defined(__DOXYGEN__)
    if(spip == &SPID1)
    {
        /*all input*/
        PORT_SPI1 &= ~((1<<SPI1_MISO)|(1<<SPI1_SCK)|(1<<SPI1_MOSI)|(1<<SPI1_SS));
        SPCR &=~(1<<SPE);
    }
#endif
}

/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_select(SPIDriver *spip)
{

#if USE_AVR_SPI1 || defined(__DOXYGEN__)
    if(spip == &SPID1)
    {

        PORT_SPI1 &= ~_BV(SPI1_SS);
        DDR_SPI1  |= _BV(SPI1_SS);

    }
#endif
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_unselect(SPIDriver *spip)
{
#if USE_AVR_SPI1 || defined(__DOXYGEN__)
    if(spip == &SPID1)
    {
        DDR_SPI1  &= ~_BV(SPI1_SS);
        PORT_SPI1 |= _BV(SPI1_SS);


    }
#endif

}

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @notapi
 */
void spi_lld_ignore(SPIDriver *spip, size_t n)
{
    spi_setup_transmission(spip, n, NULL,NULL);
    spi_start_transmission(spip);
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf)
{
    spi_setup_transmission(spip, n, txbuf, rxbuf);
    spi_start_transmission(spip);
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf)
{
    spi_setup_transmission(spip, n, txbuf, NULL);
    spi_start_transmission(spip);
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf)
{
    spi_setup_transmission(spip, n, NULL, rxbuf);
    spi_start_transmission(spip);
}

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 */
uint8_t spi_lld_polled_exchange(SPIDriver *spip, uint8_t frame)
{
    while(spip->state !=SPI_READY)
        ;
#if USE_AVR_SPI1 || defined(__DOXYGEN__)
    if(spip == &SPID1)
    {
        SPCR &= ~(1<<SPIE);

        SPDR = frame;

        while(!(SPSR & (1<<SPIF)))
            ;
        uint8_t retval = SPDR; //this is needed to clear spif
        SPCR |= (1<<SPIE);
        return retval;

    }
#endif
}

#endif /* HAL_USE_SPI */

/** @} */
