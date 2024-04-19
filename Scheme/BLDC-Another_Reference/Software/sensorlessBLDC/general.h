#include <xc.h>


// PWM frequency
#define	FPWM	20000

// Number of motor poles (not pole-pairs)
#define N_OF_MOTOR_POLES    14

// Motor maximum speed in RPM
#define MAX_MOTOR_SPEED     8000.0    // rpm

// Speed in RPM at which the motor starts to spin
#define START_MOTOR_VALUE	2200	// rpm

// Maximum length in seconde of the command pulse
#define MAX_PULSE_WIDTH     0.015   // sec


#define SPEED_REFERENCE_SCALE_FACTOR     MAX_MOTOR_SPEED/(MAX_PULSE_WIDTH*FCY/TMR2_PRESCALER)

/********************** Clock configuration ***************************/
// processor frequency using external 7.3728 MHz crystal with 16x PLL
#define	FCY		29491200UL

// processor frequency using external 7.3728 MHz crystal with 8x PLL
//#define	FCY		14745600UL

// processor frequency using internal 7.37 MHz FRC 16x PLL
//#define	FCY		29480000UL

/********************** Various configuration ***************************/
// FULL_DUTY the equivalent of 100% duty cycle
#define	FULL_DUTY (2*FCY/FPWM)

// Timer 1 and 2 prescaler
#define TMR1_PRESCALER 8UL
#define TMR2_PRESCALER 8UL

// Specify the maximum number of degrees of phase advance
#define MAX_PHASE_ADVANCE 30

/********************** Filter configuration ***************************/
// Filter phase delay is the delay in the filtered signal compared to the actual signal
// FILTER_PHASE_DELAY is equal to the Group Delay (as a function of filtered signal frequency)
//  multiplied by Fcy divided by the Timer 1 prescaler.
// Assuming a maximum speed of 100000 electrical revs per minute the frequency we are concerned 
//  with is 1666 revs per second.  This is the frequency of the filtered signal this speed
// Refering to the group delay plot in the dsPIC Filter design tool the delay at 1333 Hz
//  is approx 90 us.  At a frequency of 0 the phase delay is 85us.  We can ignore the 5us
//  and just account for the worst case scenerio
// FILTER_PHASE_DELAY = 90us*FCY/TMR1_PRESCALER
#define INVERSE_GROUP_DELAY 11111 // 1/(90us)  
#define FILTER_PHASE_DELAY FCY/TMR1_PRESCALER/INVERSE_GROUP_DELAY

// time through ADC interrupt divided by prescaler
#define PROCESSING_DELAY 410/TMR1_PRESCALER

// time through ADC interrupt divided by prescaler for high speed mode
#define PROCESSING_DELAY_HS 270/TMR1_PRESCALER

/********************** ADC configuration ***************************/
//ADC Configuration Values for low and high speed modes
#define ADCON2_LOW_SPEED 0x0410     // channel scan for CH0, MUX A only, 5 conversions per interrupt, Vrefs are AVdd and AVss
#define ADCON2_HIGH_SPEED 0x0408    // channel scan for CH0, MUX A only, 3 conversions per interrupt, Vrefs are AVdd and AVss
#define ADCSSL_LOW_SPEED 0x003B     // scan AN0, AN2, AN3, AN4 and AN5
#define ADCSSL_HIGH_SPEED 0x000B    // scan AN0, AN2, AN3
// Define the ADC result buffers for the analog pins
#define VBUSBUF     ADCBUF0		//AN0
#define VCURRENTBUF	ADCBUF1     //AN1
#define VPHABUF     ADCBUF2		//AN3
#define VPHBBUF     ADCBUF3		//AN4
#define VPHCBUF     ADCBUF4		//AN5


// Number of PWM interrupt needed to fire Medium event every 1ms
#define MEDIUM_EVENT_COUNTER_FIRE  (int)(0.001*(float)FPWM)

