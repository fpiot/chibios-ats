#include "share/atspre_staload.hats"

%{^
#include "ch.h"
#include "hal.h"
#include "test.h"
%}

%{
static WORKING_AREA(waThread1, 32);

msg_t thread1_ats(void *);
void c_entry(void) {
	/* Starts the LED blinker thread. */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, thread1_ats, NULL);
}
%}

staload UN = "prelude/SATS/unsafe.sats"

#define THREAD_SLEEP_MS   1000U
#define PORTB_LEDON       int2char0 0xff
#define PORTB_LEDOFF      int2char0 0x00

abst@ype SerialDriver = $extype"SerialDriver"
abst@ype SerialConfig = $extype"SerialConfig"
typedef msg_t = $extype"msg_t"
macdef SD1_PTR  = $extval(cPtr0(SerialDriver), "(&SD1)")
macdef PORTB_PTR = $extval(ptr, "(0x05 + 0x20)")         (* Only for Arduino Mega 2560 *)

extern fun halInit (): void = "mac#"
extern fun chSysInit (): void = "mac#"
extern fun sdStart (s: cPtr0(SerialDriver), c: ptr): void = "mac#"
extern fun TestThread (p: cPtr0(SerialDriver)): void = "mac#"
extern fun chThdSleepMilliseconds (ms: uint): void = "mac#"
extern fun c_entry (): void = "mac#"

extern fun thread1 (arg: ptr): int = "ext#thread1_ats"
implement thread1 (arg) = 0 where {
  fun loop () = {
    val () = $UN.ptr0_set<char> (PORTB_PTR, PORTB_LEDON)
    val () = chThdSleepMilliseconds THREAD_SLEEP_MS
    val () = $UN.ptr0_set<char> (PORTB_PTR, PORTB_LEDOFF)
    val () = chThdSleepMilliseconds THREAD_SLEEP_MS
    val () = loop ()
  }
  val () = loop ()
}

implement main0 () = {
  fun loop () = {
    val () = chThdSleepMilliseconds THREAD_SLEEP_MS
    val () = loop ()
  }
  (*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   *)
  val () = halInit ()
  val () = chSysInit ()
  (* Activates the serial driver 1 using the driver default configuration. *)
  val () = sdStart (SD1_PTR, the_null_ptr)
  val () = c_entry () // xxx Should be snatched...
  val () = TestThread SD1_PTR
  val () = loop ()
}
