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
 * @file    templates/gpt_lld.h
 * @brief   GPT Driver subsystem low level driver header template.
 *
 * @addtogroup GPT
 * @{
 */

#ifndef _GPT_LLD_H_
#define _GPT_LLD_H_

#include "atmega_timers.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/
/**
 * @brief   GPT3 driver enable switch.
 * @details If set to @p TRUE the support for GPT3 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(USE_AVR_GPT3)
#define USE_AVR_GPT3 FALSE
#endif
/**
 * @brief   GPT4 driver enable switch.
 * @details If set to @p TRUE the support for GPT4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(USE_AVR_GPT4)
#define USE_AVR_GPT4 FALSE
#endif
/**
 * @brief   GPT5 driver enable switch.
 * @details If set to @p TRUE the support for GPT5 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(USE_AVR_GPT5)
#define USE_AVR_GPT5 FALSE
#endif
/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   GPT frequency type.
 */
typedef uint32_t gptfreq_t;

/**
 * @brief   GPT counter type.
 */
typedef uint16_t gptcnt_t;

/**
 * @brief   Type of a structure representing a GPT driver.
 */
typedef struct GPTDriver GPTDriver;

/**
 * @brief   GPT notification callback type.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 */
typedef void (*gptcallback_t)(GPTDriver *gptp);

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Timer clock in Hz.
   * @note    The low level can use assertions in order to catch invalid
   *          frequency specifications.
   */
  gptfreq_t                 frequency;
  /**
   * @brief   Timer callback pointer.
   * @note    This callback is invoked on GPT counter events.
   */
  gptcallback_t             callback;
  /* End of the mandatory fields.*/
} GPTConfig;

/**
 * @brief   Structure representing a GPT driver.
 */
struct GPTDriver {
  /**
   * @brief Driver state.
   */
  volatile gptstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const GPTConfig           *config;
#if defined(GPT_DRIVER_EXT_FIELDS)
  GPT_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief input clock from prescaler
   */
  uint8_t clock_source;
  /**
   * @brief Lenght of the period in clock ticks
   */
  gptcnt_t period;
  /**
   * @brief Current clock tick.
   */
  gptcnt_t counter;
  /**
   * @brief Function called from the interrupt service routine
   */
  gptcallback_t callback;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if USE_AVR_GPT1 || defined(__DOXYGEN__)
extern GPTDriver GPTD1;
#endif
#if USE_AVR_GPT2 || defined(__DOXYGEN__)
extern GPTDriver GPTD2;
#endif
#if USE_AVR_GPT3 || defined(__DOXYGEN__)
extern GPTDriver GPTD3;
#endif
#if USE_AVR_GPT4 || defined(__DOXYGEN__)
extern GPTDriver GPTD4;
#endif
#if USE_AVR_GPT5 || defined(__DOXYGEN__)
extern GPTDriver GPTD5;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void gpt_lld_init(void);
  void gpt_lld_start(GPTDriver *gptp);
  void gpt_lld_stop(GPTDriver *gptp);
  void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t interval);
  void gpt_lld_stop_timer(GPTDriver *gptp);
  void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_GPT */

#endif /* _GPT_LLD_H_ */

/** @} */
