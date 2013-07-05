msp430_msgeq7
=============

Compact audio spectrum analyzer based on MSP430 and MSGEQ7

http://www.simpleavr.com/msp430-projects/compact-audio-spectrum-analyzer

7 band compact audio sprectrum analyzer based on msgeq7 and msp430 mcu

Credits

This project is based on RobG's code to drive the msgeq7 w/ a msp430 mcu
http://www.43oh.com/2011/09/launchpad-audio-spectrum-analyzer-with-the-msgeq7/

Description

There is not much of a schematic as one can easily google 'msgeq7' to get a typical
hookup diagram. The msp430 mcu interfaces w/ the msgeq7 via 3 pin.
Reset and Strobe pins are used to control msgeq7 and they are 'low' at rest. Pulling
Reset 'high' for 100ns will signal the msgeq7 that we need readings. After that each
successive 'high' pulse on the Strobe pin will advance / cycle the frequency band
for reading, during which we use the msp430 adc10 channel to read the amplitude of
each of the 7 frequency bands.

If you are using my circuit board design, there are a few connections not being used,
they were there because I was experimenting the idea of cascading three boards and
expanding the frequency band to 21 (instead of 7). The 8th column being one of these
experiments and you don't need to populate them.

Features

. msp430 value line 20 pin w/ adc10 can be used, I used msp430g2452
. Direct drive 7 x 12 led array w/ multiplexing row and columns and polarity
. Compensation for 'noise floor' of circuit built via a pre-fixed table array
. Logarithm mapping of adc values to volume unit dot levels
. Choice of one-dot, two-dot and bar type of display (decide at build time)
. Peak dot indicator avaliable (decide at build time)

  Schematic

         c0    c1    c2    c3    c4    c5    c6    NC
     R0  +-----+-----+-----+-----+-----+-----+-----+
         |\ \  |     |     |     |     |     |     |
         | v ^ |     |     |     |     |     |     |
         |  \ \|     |     |     |     |     |     |
     R1  +-----+-----+-----+-----+-----+-----+-----+

	 . a pair of leds are connected to each column + row node
	 . they are arranged in opposing cathode / anode polarities
     . there are 7 columns x 12 rows of leds
	 . the 12 rows are arrange in 2 banks of opposing polarity
	 . total io pin used is than 7 + 6 = 13 pins

                MSP430x2xx
             -----------------
         /|\|                 |              /|\  MSGEQ7
          | |                 |               |  ------------
          --|RST              |               --|(1)Vdd      | .1uF
            |                 |                 |    Audio(5)|--||--o--[ 22k ]--o L in
            |             P1.0|<-- ADC_IN ------|(3)Out      |      |
            |             P1.1|--- RESET ------>|(7)Reset    |      |
            |             P1.2|--- STROBE ----->|(4)Strobe   |      o--[ 22k ]--o R in
            |             P2.6|--> Col6         |            |
            |             P2.7|--> Col5         |            |             /|\
            |                 |                 |            |              |
            |             P2.3|--> Col0         |    Clkin(8)|--[ 200k]-----o
            |             P2.4|--> Col1         |       CG(6)|-------o      |
            |             P2.5|--> Col2         |            |       |      = 33pF
            |             P1.6|--> Col3         |      Gnd(2)|--o    = .1uF |
            |             P1.7|--> Col4         |            |  |    |      | 
            |                 |                                ---  ---    ---
            |             P2.2|--> Row0/6                      ///  ///    ///
            |             P2.1|--> Row1/7
            |             P2.0|--> Row2/8       0.1uF by-pass capacitor for both
            |             P1.5|--> Row3/9
            |             P1.4|--> Row4/A
            |             P1.3|--> Row5/B
            |                 |


  code provided as is, no warranty
  This is a hobby project and does not guarantee to do anything. Use as-is.

  You cannot use code for commercial purpose w/o my permission

  Chirs Chung (www.simpleavr.com)
  Nov 2012, Jul 2013 published
