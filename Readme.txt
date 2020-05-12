Metal Detector using ATmega328p
Timothy Nguyen 
80663560
March 24, 2020

Video: https://www.youtube.com/watch?v=LRn1SkCzUSs

INTRODUCTION:
The Colpitts oscillator circuit outputs waveforms of different frequencies. The frequency at the output of the CMOS inverter increases 
when metal is placed closer to the inductor of the circuit. I used this observation to create a simple metal detector using an ATmega328p 
microcontroller.


CIRCUIT DESIGN:
I used two 0.1 uF capacitors with 5% error, a 100mH inductor, a 1k ohm resistor, and a CMOS inverter using a NTD2955 P-MOSFET and a NTD3055L
N-MOSFET to output a square waveform. The predicted base frequency is 2300Hz but some error was to be expected. The output of this square waveform 
is connected to PB1 of the microcontroller as an output. To generate a sound when metal is detected, I connected a CEM-1302 magnetic buzzer to a 
NTD3055L N-MOSFET that is controlled by PC0 of the microcontroller. The P-MOSFET acts like a switch and can generate the required frequency to operate
the magnetic buzzer. I also connected an LED to PB0 to light up when metal is detected. A detailed circuit diagram is included in the zip file. 


PROGRAM:
The two main components I needed to program was a frequency generator for the magnetic buzzer and a frequency meter for the Colpitts 
oscillator. My instructor, Dr. Calvino-Fraga, was kind of enough to lend myself and my colleagues working base code to help us 
accomplish this. The frequency generator was implemented using timer interrupts and the frequency meter was implemented using a 
timer/counter. 
- Frequency Generator
The CEM-1302 magnetic buzzer required a 2048Hz signal so I simply changed the frequency constant in the freq_gen.c example file to 
match that and set the PC0 pin as the output. 
- Frequency Meter
I set PB1 pin as an input for the frequency meter. I used the Period.c example program to measure the base frequency and validated the result
using a Fluke 101 multimeter. The base frequency was around 2900Hz. After placing multiple coins close to the inductor, the minimum frequency 
that was generated was around 3100 Hz. Using this information, I programmed conditional statements to implement a metal detector. 
When metal was detected, I enabled global interrupts to turn on the frequency generator in order to emit sound and turned off PB0 in order
to emit light from the LED. Likewise, I disabled global interrupts and turned on PB0 when metal is not detected.


OPERATION:
When metal is placed approximately 1 cm away or closer from the inductor , a sound is emitted and an LED is turned on. The live frequency measurement
can also be viewed on Putty.


REFERENCES:
https://wiki.analog.com/university/courses/electronics/comms-lab-colpitts-osc 
https://courseware.ee.calpoly.edu/~dbraun/courses/ee307/F02/02_Shelley/Section2_BasilShelley.htm
https://www.cuidevices.com/product/resource/cem-1203-42-.pdf
 





