/*

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

  http://www.simpleavr.com/msp430-projects/compact-audio-spectrum-analyzer


  Parts / Bill of Materials

  . MSP430G2452/G2553 mcu, or 20 pin dip device w/ 2k+ flash will do
  . Custom printed circuit board
  . MSGEQ7 ic
  . 1 x 47k pull up resistor for msp430
  . 1 x 200k, 2 x 22k resistors
  . 2 x 0.1uF, 1 x 33pF capacitors
  . 1 x 3.5mm audio socket
  . A few header pins (1x3, 1x3, 1x2)
  . optional 0.1uF bypass caps for both ICs
  . optional 100-1000uF cap for power source

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

  Construction

  The project could be built on perf board, or in my case a custom PCB. I used Fritzing to
  design my PCB as it's simple to use. You can find the design from my github link below.

  The PCB does not have all components clearly marked. If you are using my PCB design,
  please follow the schematic and the photo below for component placement

  . The 20 pin msp430 is to be mounted at the back side of the PCB, head face south
  . 8 pin msgeq7 mounts on the front side, head face east
  . Resistor closest to msgeq7 is a 200k
  . Blue caps shown in photo are 0.1uFs
  . The Beige cap is a 33pF
  . Two standing resistors close to the 3.5mm audio socket are 22ks
  . A 47k msp430 reset pull-up is needed just avobe the 22ks
  * Additional bypass, power filter capacitor can be used (bug soldered) as needed
  * If your input source is mono, you can omit the audio in merging resistors
  * You will also need a programming device that does SBW, I used a LaunchPad

  Software / Firmware

  I had used mspgcc to build the firmware but it should also works w/ CCS.
  The compile / link command is shown below, there is only one single c file for simplicity.
  You may need to adjust a bit based on your toolchain setup

  msp430-gcc -Os -Wall -ffunction-sections -fdata-sections -fno-inline-small-functions -Wl,-Map=msp430_msgeq7.map,--cref -I/cygdrive/c/mspgcc-20120406-p20120502/bin/../msp430/include -mmcu=msp430g2553 -o msp430_msgeq7.elf msp430_msgeq7.c
  msp430-objdump -DS msp430_msgeq7.elf
  msp430-strip msp430_msgeq7.elf
  msp430-size --totals msp430_msgeq7.elf

  There are two arrays / maps inside the code that you should adjust based on your usage.

  comps[] should contain the 'noise floor' of your build. These should be your adc values from
  msgeq7 when there is no sound input (but connected to your source).

  lvls[] is a psuedo logarithm scale mapping to translate adc values to the 12 vu led levels.
  You can adjust these values based on your input amplitude and/or taste.

  There are also defines that can affect the visuals. 
  . MODE_DOT is for a single dot bounce show
  . MODE_PEAK allows showing 'peak' signals
  . MODE_2DOT bounces two dots instead of one dot
  . For bar-graph, just do not define MODE_DOT and MODE_2DOT

  The SBW (spy-by-wire) programming is done thru a launchPad. There is a 2 pin header at the back of the
  PCB for this purpose. You do not need to solder the header pins as the hole diameter is tight and
  friction is enough to make good temporary contact (the same is true for the 3 pin power headers)

  You can follow the photo below on how it's done. Just remember to remove any mcu on the launchpad
  when you do this.


  You will also find that there are unpopulated components on the PCB. They are an unfinished attempt
  to bring together 3 PCBs to form a 20 band spectrum analyzer. I am not going to describe it and
  if you are interested, you can explore yourself. 

  The basic idea is 
  (a) have 2 io pins toggle 2 additional registors to 'alter' the msgeq7 RC tank so that it 
  gives us another 2 set of 7 frequency bands.
  (b) implements a one wire communication (via 3rd pin on 3 pin power header) to send vu values to
  the other 2 PCB.
  (c) ther other 2 PCBs will not have msgeq7s on board but they have 8 columns of leds, while the
  master PCB drives 6 columns plus doing the msgeq7.

  I had temporary / permanently abandon this cascading PCB idea for now so if you try, you are on your own.

  code provided as is, no warranty
  This is a hobby project and does not guarantee to do anything. Use as-is.

  You cannot use code for commercial purpose w/o my permission

  Chirs Chung (www.simpleavr.com)
  Nov 2012, Jul 2013 published

*/


#include <msp430.h>
#include <stdint.h>

const uint8_t row_col[] = { 
	//  row     col
	((0+3)<<4) | (8+3),
	((0+4)<<4) | (8+4),
	((0+5)<<4) | (8+5),
	((8+0)<<4) | (0+6),
	((8+1)<<4) | (0+7),
	((8+2)<<4) | (8+7),
	((0+0)<<4) | (8+6),
	((0+0)<<4) | (0+2),
};

const uint8_t p1rows = BIT3|BIT4|BIT5;
const uint8_t p2rows = BIT0|BIT1|BIT2;

const uint8_t p1cols = BIT6|BIT7;
const uint8_t p2cols = BIT3|BIT4|BIT5|BIT6|BIT7;

volatile uint8_t ticks=0, clicks=0;
volatile uint8_t col=0;

uint8_t output[8*2*2];		// 8 columns of p1+p2 values times 2 sets
uint8_t levels[] = { 0, 0, 0, 0, 0, 0, 0, 0, };	// 0..11
uint8_t sCurrent=0;
uint8_t samples[4][8];

uint8_t peaks[]  = { 0, 0, 0, 0, 0, 0, 0, 0, };
uint8_t peak_out[]  = { 0, 0, 0, 0, 0, 0, 0, 0, };
uint8_t retreat[]  = { 0, 0, 0, 0, 0, 0, 0, 0, };

#define EQ_DC_IN_PIN BIT0
#define EQ_RESET_PIN BIT1
#define EQ_STROBE_PIN BIT2

uint8_t mode = 0;

#define MODE_DOT  	BIT0
#define MODE_PEAK 	BIT1
#define MODE_2DOT  	BIT2
#define MODE_SAMPLE BIT7

//________________________________________________________________________________
void load() {
	uint8_t c=0, r=0;
	for (c=0;c<8;c++) {
		uint8_t c_val = row_col[c] & 0x0f;
		uint8_t *pout = output + (c<<1);

		*pout = *(pout+1) = 0;
		if (c_val & 0x08)
			*(pout+1) = 1<<(c_val&0x07);
		else
			*pout = 1<<c_val;
		*(pout+16) = *pout;
		*(pout+17) = *(pout+1);

		if (levels[c] > 12)
			levels[c] = 12;

		r = levels[c];

		uint8_t drop_to = 0;
		if (mode & MODE_PEAK) {
			if (peaks[c] > r) {
				if (peak_out[c]) {
					peak_out[c]--;
					drop_to = r;
					r = peaks[c];
				}//if
				else {
					peaks[c]--;
					peak_out[c] = 12;
				}//else
			}//if
			else {
				peaks[c] = r;
				peak_out[c] = 12;
			}//else
		}//if

		uint8_t dots = 12;
		if (mode & MODE_DOT) dots = 1;
		if (mode & MODE_2DOT) dots = 2;

		while (r) {
			r--;
			uint8_t r_val = row_col[r%6] >> 4;
			uint8_t r_alt = (r/6) * 16;
			if (r_val&0x08)
				*(pout+r_alt+1) |= 1<<(r_val&0x07);
			else
				*(pout+r_alt) |= 1<<r_val;
			if (drop_to) {
				r = drop_to;
				drop_to = 0;
			}//if
			else {
				dots--;
				if (!dots) break;
			}//else
		}//while
	}//for
}

//________________________________________________________________________________
void scan() {

	uint8_t *pout = output + (col<<1);
	uint8_t p1=*pout, p2=*(pout+1);

	col++;
	col &= 0x0f;

	P1DIR = P2DIR = 0;
	P1OUT = P2OUT = 0;
	if (!col) return;
	P1DIR = p1|EQ_RESET_PIN|EQ_STROBE_PIN;
	P2DIR = p2;
	p1 &= p1rows;
	p2 &= p2rows;
	if (!(col&0x08)) {
		p1 ^= 0xf8;
		p2 ^= 0xff;
	}//if
	p1 &= ~0x07;
	P1OUT = p1;
	P2OUT = p2;

}

//________________________________________________________________________________
int main(void) {
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL  = CALDCO_8MHZ;

	P2SEL = P1SEL = 0;
	P1DIR = 0x00; P1OUT = 0x00;

	P1DIR = EQ_RESET_PIN|EQ_STROBE_PIN;
    TA0CCR0 = 576;                  // 576/72us
    TA0CTL = TASSEL_2 + MC_1;// SMCLK, continous

    ADC10CTL1 = INCH_0;
    ADC10CTL0 = ADC10SHT_2 + ADC10ON + ADC10IE;

	_BIS_SR(GIE);

	uint8_t ocnt=0;

	mode |= MODE_2DOT;
	//mode = MODE_DOT;
	//mode |= MODE_PEAK;

	load();
	while (1) {
		P1DIR = EQ_RESET_PIN|EQ_STROBE_PIN;
		P1OUT = 0x00;

		const uint8_t factor = 2;
		uint8_t i=0, j=0;
		for (sCurrent=0;sCurrent<factor;sCurrent++) {
			TA0CCTL0 = CCIE;
			__bis_SR_register(LPM0_bits);
			TA0CCTL0 = 0;
		}//for
		for (i=0;i<7;i++) {
			uint8_t use = 0, good = 1;
			for (j=0;j<factor;j++) {
				if (samples[j][i])
					use += samples[j][i];
				else
					good = 0;
			}//if
			if (!good) use = 0;
			if (use) {
				use /= factor;
			}//use
			if (levels[i]) levels[i]--;
			if (use > levels[i]) levels[i] = use;
			if (mode & MODE_PEAK) levels[i] = use;
		}//for

		load();

		//___________ delay before next take
		uint16_t cnt = 200 * 16;
		while (cnt--) scan();

		ocnt++;
	}//while



}

uint8_t timerCounter = 0;
uint8_t column = 0;
uint8_t txData = 0;
uint8_t txCounter = 0;
uint8_t eqCounter = 0;
uint8_t eqCtrl = 0;

uint8_t cnt2=0;
//______________________________________________________________________
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_iSR(void) {

	eqCtrl = (eqCounter >> 2) & 0x07;
	if (eqCtrl == 0) {
		if (eqCounter == 0) {
			P1OUT |= EQ_RESET_PIN;
			P1OUT &= ~EQ_RESET_PIN;
			eqCounter += 4;
		}//if
	} else if ((eqCounter & 0x03) == 3) {
		ADC10CTL0 |= ENC + ADC10SC;
	} else if (eqCounter & 0x01) {
		P1OUT |= EQ_STROBE_PIN;
	} else if (eqCounter & 0x02) {
		P1OUT &= ~EQ_STROBE_PIN;
	}//if
	eqCounter++;
	timerCounter++;

	if (!(timerCounter&0x1f))
		__bic_SR_register_on_exit(LPM0_bits);
}

//______________________________________________________________________
#pragma vector=ADC10_VECTOR
__interrupt void ADC10_ISR(void) {
	//_______ 7 band.. 63, 160, 400, 1000, 2500, 6250, 16000
	//_______ compensation for 'noise floor' of circuit built
	const uint8_t  comp[] = { 25, 30, 25, 35, 30, 50, 65, };	// id 0542
	//const uint8_t  comp[] = { 95, 85, 60, 70, 85, 90, 90, };
	//const uint8_t  comp[] = { 40, 40, 40, 40, 40, 40, 40, };
	//_______ logarithm mapping of adc values to vu dot levels, adjust for your input
	const uint16_t lvls[] = { 0, 
	  4, 6, 8, 11, 16, 22, 32, 45, 63, 89, 126, 178, };			// 256
	 //16, 22, 32, 45, 63, 90, 126, 179, 252, 356, 504, 711, };	// 1024
	 // 8, 11, 16, 22, 32, 45, 63, 89, 126, 178, 252, 356, };	// 512
	 // 6, 8, 12, 17, 24, 34, 47, 67, 95, 134, 189, 267, };		// 384


	// try scale from 1024 to 12

	uint16_t v = ADC10MEM;
	uint8_t i=12, c=eqCtrl-1;
	uint16_t ambience = comp[c];


	if (v > ambience)
		v -= ambience;
	else
		v = 0;

	v *= 2;

	do { 
		if (v >= lvls[i]) break;
	} while (i--);

	samples[sCurrent][c] = i;

}

