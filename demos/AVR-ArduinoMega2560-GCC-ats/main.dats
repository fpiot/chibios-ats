#include "share/atspre_staload.hats"

%{^
#include "ch.h"
#include "hal.h"
#include "test.h"
%}

%{
void c_clear_led1(void) {
	palClearPad(IOPORT2, PORTB_LED1);
}

void c_toggle_led1(void) {
	palTogglePad(IOPORT2, PORTB_LED1);
}

static WORKING_AREA(waThread1, 32);

msg_t thread1_ats(void *);
void c_entry(void) {
	/* Starts the LED blinker thread. */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, thread1_ats, NULL);
}
%}

staload UN = "prelude/SATS/unsafe.sats"

#define THREAD_SLEEP_MS   1000U

abst@ype SerialDriver = $extype"SerialDriver"
abst@ype SerialConfig = $extype"SerialConfig"
typedef msg_t = $extype"msg_t"
macdef SD1_PTR  = $extval(cPtr0(SerialDriver), "(&SD1)")

extern fun halInit (): void = "mac#"
extern fun chSysInit (): void = "mac#"
extern fun sdStart (s: cPtr0(SerialDriver), c: ptr): void = "mac#"
extern fun c_toggle_led1 (): void = "mac#"
extern fun c_clear_led1 (): void = "mac#"
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
  sdStart (SD1_PTR, the_null_ptr);
  c_clear_led1 ();
  c_entry (); // xxx Should be snatched...
  TestThread SD1_PTR;
  loop ();
end where {
  fun loop () = begin
    chThdSleepMilliseconds THREAD_SLEEP_MS;
    loop ();
  end
}
