%{^
#include "ch.h"
#include "hal.h"
#include "test.h"
%}

%{
static WORKING_AREA(waThread1, 32);
static msg_t Thread1(void *arg) {

  while (TRUE) {
    palTogglePad(IOPORT2, PORTB_LED1);
    chThdSleepMilliseconds(1000);
  }
  return 0;
}

/*
 * Application entry point.
 */
void c_entry(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 1 using the driver default configuration.
   */
  palClearPad(IOPORT2, PORTB_LED1);
  sdStart(&SD1, NULL);

  /*
   * Starts the LED blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  TestThread(&SD1);
}
%}

#define THREAD_SLEEP_MS   i2u 1000

extern fun chThdSleepMilliseconds (ms: uint): void = "mac#"
extern fun c_entry (): void = "mac#"

implement main0 () = begin
  let fun loopsleep () = (chThdSleepMilliseconds THREAD_SLEEP_MS; loopsleep ())
  in (c_entry (); loopsleep ())
  end
end
