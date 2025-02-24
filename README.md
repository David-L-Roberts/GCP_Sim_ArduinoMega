# GCP_Sim_ArduinoMega
 Arduino Mega code for GCP4000 simulator system.


## TO DO

Action codes
- in manual mode, allow setting to specific state number
- allow manual setting of relays when in `manual` mode
- in decrease or increase EZ mode, send completion signal over serial when switching has finished
  - calling next state on end state should return an error.

Serial Coms
- allow reading of current state number
  - allow reading of current state output list (binary state >> 110011001)

Check data type of action codes from uint8_t to uint16_t.

Process set_state action code, then listen for state number

force system change to state number