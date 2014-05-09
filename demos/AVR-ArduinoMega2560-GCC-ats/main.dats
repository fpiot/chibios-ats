%{^
#include "ch.h"
#include "hal.h"
#include "test.h"
%}

%{
SerialDriver *c_SD1_p(void) {
	return &SD1;
}

void c_clear_led1(void) {
	palClearPad(IOPORT2, PORTB_LED1);
}

void c_toggle_led1(void) {
	palTogglePad(IOPORT2, PORTB_LED1);
}

static WORKING_AREA(waThread1, 32);
static msg_t Thread1(void *arg) {
	return thread1_ats();
}

void c_entry(void) {
	/* Starts the LED blinker thread. */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
}
%}

#define THREAD_SLEEP_MS   1000U

abst@ype SerialDriver = $extype"SerialDriver"
abst@ype SerialConfig = $extype"SerialConfig"
typedef msg_t = $extype"msg_t"

extern fun halInit (): void = "mac#"
extern fun chSysInit (): void = "mac#"
extern fun sdStart (s: cPtr0(SerialDriver), c: ptr): void = "mac#"
extern fun c_toggle_led1 (): void = "mac#"
extern fun c_clear_led1 (): void = "mac#"
extern fun c_SD1_p (): cPtr0(SerialDriver) = "mac#"
extern fun TestThread (p: cPtr0(SerialDriver)): void = "mac#"
extern fun chThdSleepMilliseconds (ms: uint): void = "mac#"
extern fun c_entry (): void = "mac#"

extern fun thread1 (arg: ptr): int = "ext#thread1_ats"
implement thread1 (arg) = begin
  loop ();
  0;
end where {
  fun loop () = begin
    c_toggle_led1 ();
    chThdSleepMilliseconds THREAD_SLEEP_MS;
    loop ();
  end
}

implement main0 () = begin
  (*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   *)
  halInit ();
  chSysInit ();
  (* Activates the serial driver 1 using the driver default configuration. *)
  sdStart (SD1_p, the_null_ptr);
  c_clear_led1 ();
  c_entry (); // xxx Should be snatched...
  TestThread SD1_p;
  loop ();
end where {
  fun loop () = begin
    chThdSleepMilliseconds THREAD_SLEEP_MS;
    loop ();
  end
  val SD1_p = c_SD1_p ()
}
