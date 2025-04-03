# Finite State Machine
 Embedded system Programming:Using a finite state machine to model Embedded Code
 
Use of a finite state machine (FSM)
• Used to encapsulate code in discrete blocks
• Each state will have a mechanism to either enter a state
or to move from one state to another
• You will never reach a dead end, because there may
never be an undefined state*
• *If FSM is not implemented correctly in embedded code
this condition can be violated.
• Many different conventions / representations for FSM
• Simple FSM’s
• SDL FSM’s
• Extended FSM’s
• Mealy machine FSM
• Moore machine FSM

Putting it all together in practice
• On paper
• Interpret requirements of system
• Define states, inputs, outputs, variables
• Draw finite state machine
• Can create state transition table – More on this later.
• Code
• Define states as an enumeration “enum”
• Create a skeleton for the states using a switch statement
• Remember the default state
• use it as a debug point, you should not end up in the default state
• if you do, find out why
• Revise on drawn FSM, code, re-test.
• Always make a config / start-up state
• Move all configurations / setup in an config / start-up state, instead of doing
it prior to the while(1) as we have done thus far – can recover if something
died without warning.

Languages used- C
Hardware- PIC24FJ64GA002
Description-The PIC24FJ64GA002 microcontroller has a 16-bit CPU and can operate at a maximum speed of 16 MHz. It includes 64KB of flash memory for program storage. The device comes in a 28-pin SPDIP package.

Technical Specifications

The PIC24FJ64GA002 has 21 I/O pins and requires THT mounting. It operates within a temperature range of -40C to +85C.
