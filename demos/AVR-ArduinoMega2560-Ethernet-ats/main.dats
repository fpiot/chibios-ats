#include "share/atspre_staload.hats"

%{^
#include "ch.h"
#include "hal.h"
%}

%{
static WORKING_AREA(waThread1, 32);
%}

staload UN = "prelude/SATS/unsafe.sats"

#define THREAD_SLEEP_MS   1000U
#define PORTB_LEDON       int2char0 0xff
#define PORTB_LEDOFF      int2char0 0x00

abst@ype SerialDriver = $extype"SerialDriver"
abst@ype SerialConfig = $extype"SerialConfig"
abst@ype Thread       = $extype"Thread"
abst@ype tprio_t      = uint
typedef msg_t         = int
typedef tfunc_t       = (ptr) -> msg_t
macdef SD1_PTR        = $extval(cPtr0(SerialDriver), "(&SD1)")
macdef waThread1_PTR  = $extval(ptr, "waThread1")
macdef waThread1_SIZE = $extval(size_t, "sizeof(waThread1)")
macdef NORMALPRIO     = $extval(tprio_t, "NORMALPRIO")
macdef PORTB_PTR      = $extval(ptr, "(0x05 + 0x20)")   (* Only for Arduino Mega 2560 *)

extern fun halInit: () -> void = "mac#"
extern fun chSysInit: () -> void = "mac#"
extern fun sdStart: (cPtr0(SerialDriver), ptr) -> void = "mac#"
extern fun chThdCreateStatic: (ptr, size_t, tprio_t, tfunc_t, ptr) -> cPtr0(Thread) = "mac#"
extern fun chThdSleepMilliseconds: {p:pos} (uint p) -> void = "mac#"

extern fun thread1: tfunc_t
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
  (* Starts the LED blinker thread. *)
  val tp = chThdCreateStatic (waThread1_PTR, waThread1_SIZE, NORMALPRIO, thread1, the_null_ptr)
  val () = loop ()
}
