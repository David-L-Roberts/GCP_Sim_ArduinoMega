# GCP_Sim_ArduinoMega
 Arduino Mega code for GCP4000 simulator system.


## TO DO
Timer
- Accept switching time from serial monitor
- change switching time to be based off timer interrupts instead of sleep wait time
- As switching period decrease, being jumping over every 2nd state (or more etc... depending on min switching time)

Action codes
- allow manual setting of relays when in `manual` mode
- in decrease or increase EZ mode, send completion signal over serial when switching has finished
  - calling next state on end state should return an error.

Serial Coms
- allow reading of current state number
  - allow reading of current state output list (binary state >> 110011001)

