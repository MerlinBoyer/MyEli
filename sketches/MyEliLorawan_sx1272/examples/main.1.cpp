// https://github.com/mcci-catena/arduino-lmic#installing
// https://www.disk91.com/2019/technology/lora/hoperf-rfm95-and-arduino-a-low-cost-lorawan-solution/

#include <Arduino.h>
#include <SPI.h>
#include <lmic.h>

#define RUNNING_LED A0
#define DEBUG_LED A1



static void running_led(int state) {
  digitalWrite(RUNNING_LED, state);
}
static void debug_led(int state){
  digitalWrite(DEBUG_LED, state);
}


static u1_t ledstate = 0;
static void blinkfunc (osjob_t* j) {
  // toggle LED
  ledstate = !ledstate;
  debug_led(ledstate);
  // reschedule blink job
  os_setTimedCallback(j, os_getTime()+ms2osticks(100), blinkfunc);
}


//////////////// functions 

// log text to USART and toggle LED
// counter
static int running_led_state = 0;
static void running_routine (osjob_t* job) {
  // say hello

  Serial.println("Hello World!\r ");
  // log counter
  Serial.println("state = " + running_led_state);

  running_led_state = !running_led_state;
  running_led( running_led_state );
  // reschedule job every second
  os_setTimedCallback(job, os_getTime()+sec2osticks(500), running_routine);
}


static osjob_t blinkjob;
void onEvent (ev_t ev) {
  // debug_event(ev);  <--- ????
  switch(ev) {
    // starting to join network
    case EV_JOINING:
      // start blinking
      blinkfunc(&blinkjob);
      break;
    // network joined, session established
    case EV_JOINED:
      // cancel blink job
      os_clearCallback(&blinkjob);
      // switch on LED
      debug_led(1);
      // (don't schedule any new actions)
    break;
  }
}





///////////////////// //////////////// main

void setup() {
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  
  // LMIC_reset();
  // initialize run-time env
  os_init();
  // setup running led
  osjob_t initjob;
  running_routine( &initjob );
}

void loop() {
  delay(500);
}