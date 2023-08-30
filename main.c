#include "lpc824.h"
#include "serial.h"

#define F_CPU 60000000L
#define SYSTEM_CLK 30000000L
#define DEFAULT_F 100000L // For a 10us servo resolution

volatile int ISR_pwm1=60, ISR_pwm2=130, ISR_cnt=0,ISR_cnt2=0, ISR_cnt3=0, ISR_cnt4=0, EMAG_cnt=0; coinadder=0; //isr inital wont affect rn
int SERVO_ON = 0;
int BOARDER = 0;
int count = 0;
void InitTimer(void)
{
	SCTIMER_CTRL |= BIT2; // halt SCTimer

    // Assign a pin to the timer. So we can check the frequency.  (It should be 100kHz)
    // Assign GPIO_14 to SCT_OUT0_O
	SWM_PINASSIGN7 &= 0x00ffffff;
	SWM_PINASSIGN7 |= (14 << 24); 
	
	SYSCON_SYSAHBCLKCTRL |= BIT8; // Turn on SCTimer 
	SYSCON_PRESETCTRL |=  BIT8; // Clear the reset SCT control
	
	SCTIMER_CONFIG |= BIT0; // Unified 32 bit counter
	SCTIMER_MATCH0 = SYSTEM_CLK/DEFAULT_F; // Set delay period 
	SCTIMER_MATCHREL0 = SYSTEM_CLK/DEFAULT_F;
	SCTIMER_EV0_STATE = BIT0;  // Event 0 pushes us into state 0
	// Event 0 configuration:
	// Event occurs on match of MATCH0, new state is 1	
	SCTIMER_EV0_CTRL =  BIT12 + BIT14 + BIT15;
	// State 1 configuration
	SCTIMER_EV1_STATE = BIT1;  // Event 1 pushes us into state 1
	// Event 1 configuration
	// Event occurs on MATCH0, new state is 0
	SCTIMER_EV1_CTRL =  BIT12 + BIT14;
	// OUT0 is set by event 0
	SCTIMER_OUT0_SET = BIT0;
	// OUT1 is cleared by event 1
	SCTIMER_OUT0_CLR = BIT1;
	// Processing events 0 and 1
	SCTIMER_LIMIT_L = BIT0 + BIT1;
	// Remove halt on SCTimer
	SCTIMER_CTRL &= ~BIT2;		
		
	SCTIMER_EVEN = 0x01; //Interrupt on event 0
	NVIC_ISER0|=BIT9; // Enable SCT interrupts in NVIC
}

void Reload_SCTIMER (unsigned long Dly)
{
	SCTIMER_CTRL |= BIT2; // halt SCTimer
	SCTIMER_MATCH0 = Dly; // Set delay period 
	SCTIMER_MATCHREL0 = Dly;
	SCTIMER_COUNT = 0;
	SCTIMER_CTRL &= ~BIT2;	// Remove halt on SCTimer	
}

void STC_IRQ_Handler(void)
{	
	//if (frequency is high enough): do this. Else, skip everything
	SCTIMER_EVFLAG = 0x01; // Clear interrupt flag
	ISR_cnt++;
	
	if(ISR_cnt==ISR_pwm1) // CHECK ISR DELAY TIME AND TURN OFF PULSE (to set the pulse width)
	{
		GPIO_B1=0; // PIN12 // BOT SERVO XYPLANE
	}
	if(ISR_cnt==ISR_pwm2) // CHECK ISR DELAY TIME AND TURN OFF PULSE (to set the pulse width)
	{
		GPIO_B9=0; // PIN13 // TOP SERVO EMAG
	}
	if(ISR_cnt>=10000) // ENTER THIS LOOP TO SET PULSE TO HIGH
	{
		ISR_cnt=0; // 2000 * 10us=20ms
		GPIO_B1=1; // PIN12
		GPIO_B9=1; // PIN13
		
		if (SERVO_ON) {  // CHANGE ON/OFF so that ISR counters do not increment for position change conditions
		ISR_cnt2++;
		//ISR_cnt3++;
		}
		
		if (BOARDER){
		
		ISR_cnt4++;
		
		}
		
		// ISR delay time changing
		
		// WHEELS for boarder
		
		if (ISR_cnt4 >= 1 ){ // MOVE WHEELS BACKWARD
					GPIO_B3=1; // PIN 7
					GPIO_B2=0; // PIN 8
					
					GPIO_B11=1; // PIN 9
					GPIO_B10=0; // PIN 10 
		}
		
		// rotation 
		
		if (ISR_cnt4 >= 5 ){ // rotate 
			GPIO_B3=0; // PIN 7
			GPIO_B2=1; // PIN 8
			
			GPIO_B11=1; // PIN 9
			GPIO_B10=0; // PIN 10 				--- > 20 units = 360 degrees, 4 units = 72 degrees
		}
		
		if (ISR_cnt4 >= 13 ){ // STOP WHEELS
			GPIO_B3=0; // PIN 7
			GPIO_B2=0; // PIN 8
			
			GPIO_B11=0; // PIN 9
			GPIO_B10=0; // PIN 10 
		}
		
		if (ISR_cnt4 >= 14){ // Reset ISR counters to reset position
		
		ISR_cnt4 = 0;
		
		}
		
		if (ISR_cnt4 < 1){ // set ISR to RESET POS	

		BOARDER = 0; // TURN OFF BOARDER FLAG
		
		GPIO_B3=0; // PIN 7
		GPIO_B2=1; // PIN 8
		
		GPIO_B11=0; // PIN 9
		GPIO_B10=1; // PIN 10 
		
		}
		
		
		// WHEELS END 
		

		if (ISR_cnt2 >= 1){ // Set new ISR width for new position // ROTATION IN EMAG ROTATION PICKUP READY	
		ISR_pwm1 = 110;
				
				// WHEELS
		
				if (ISR_cnt2 >= 2 ){ // MOVE WHEELS BACKWARD
					GPIO_B3=1; // PIN 7
					GPIO_B2=0; // PIN 8
					
					GPIO_B11=1; // PIN 9
					GPIO_B10=0; // PIN 10 
				}
				
				
				if (ISR_cnt2 >= 6 ){ // STOP WHEELS
					GPIO_B3=0; // PIN 7
					GPIO_B2=0; // PIN 8
					
					GPIO_B11=0; // PIN 9
					GPIO_B10=0; // PIN 10 
				}
				
				// WHEELS END 
				
		
				if (ISR_cnt2 >= 10 ){ // EMAG ROTATION TO GROUND
				ISR_pwm2 = 260;
				}
				
				// TURN ON EMAG
				//eputs("---- EMAG ON ----\r\n");
				
				
		}	
		if (ISR_cnt2 >= 15){ // SWEEP START
		ISR_pwm1 = 200;
		}
		
		if (ISR_cnt2 >= 20){ // SWEEP BACK
		ISR_pwm1 = 120;
		}
		if (ISR_cnt2 >= 25 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 250;
		}
		if (ISR_cnt2 >= 26 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 240;
		}
		if (ISR_cnt2 >= 27 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 230;
		}
		if (ISR_cnt2 >= 28 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 220;
		}
		if (ISR_cnt2 >= 29 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 210;
		}
		
		if (ISR_cnt2 >= 30 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 200;
		}
		if (ISR_cnt2 >= 31 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 190;
		}
		if (ISR_cnt2 >= 32 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 180;
		}
		if (ISR_cnt2 >= 33 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 170;
		}
		if (ISR_cnt2 >= 34 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 160;
		}
		if (ISR_cnt2 >= 35 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 150;
		}
		if (ISR_cnt2 >= 36 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 140;
		}
		if (ISR_cnt2 >= 37 ){ // EMAG ROTATION BACK UP
		ISR_pwm2 = 130;
		coinadder++;
		}
		
		
		
		
		if (ISR_cnt2 >= 40 ){ // SWEEP START
		ISR_pwm1 = 65;
		}
		
	
		
		if (ISR_cnt2 >= 46){ // Reset ISR counters to reset position
		
		ISR_cnt2 = 0;
		//ISR_cnt3 = 0;
		
		}
		
		if (ISR_cnt2 < 1){ // set ISR to RESET POS	
		ISR_pwm1 = 60;
		ISR_pwm2 = 130;
		SERVO_ON = 0; // TURN OFF SERVO MOVEMENT

		
		// TURN OFF EMAG
		//eputs("---- EMAG OFF ----\r\n");
		}		
	}	
}


void wait_1ms(void)
{
	// For SysTick info check the LPC824 manual page 317 in chapter 20.
	SYST_RVR = (F_CPU/1000L) - 1;  // set reload register, counter rolls over from zero, hence -1
	SYST_CVR = 0; // load the SysTick counter
	SYST_CSR = 0x05; // Bit 0: ENABLE, BIT 1: TICKINT, BIT 2:CLKSOURCE
	while((SYST_CSR & BIT16)==0); // Bit 16 is the COUNTFLAG.  True when counter rolls over from zero.
	SYST_CSR = 0x00; // Disable Systick counter
}

void waitms(int len)
{
	while(len--) wait_1ms();
}

#define PIN_PERIOD (GPIO_B13) // Read period from PIO0_13 (pin 3)

// GetPeriod() seems to work fine for frequencies between 400Hz and 400kHz.
long int GetPeriod (int n)
{
	int i;
	unsigned int saved_TCNT1a, saved_TCNT1b;
	
	SYST_RVR = 0xffffff;  // 24-bit counter set to check for signal present
	SYST_CVR = 0xffffff; // load the SysTick counter
	SYST_CSR = 0x05; // Bit 0: ENABLE, BIT 1: TICKINT, BIT 2:CLKSOURCE
	while (PIN_PERIOD!=0) // Wait for square wave to be 0
	{
		if(SYST_CSR & BIT16) return 0;
	}
	SYST_CSR = 0x00; // Disable Systick counter

	SYST_RVR = 0xffffff;  // 24-bit counter set to check for signal present
	SYST_CVR = 0xffffff; // load the SysTick counter
	SYST_CSR = 0x05; // Bit 0: ENABLE, BIT 1: TICKINT, BIT 2:CLKSOURCE
	while (PIN_PERIOD==0) // Wait for square wave to be 1
	{
		if(SYST_CSR & BIT16) return 0;
	}
	SYST_CSR = 0x00; // Disable Systick counter
	
	SYST_RVR = 0xffffff;  // 24-bit counter reset
	SYST_CVR = 0xffffff; // load the SysTick counter to initial value
	SYST_CSR = 0x05; // Bit 0: ENABLE, BIT 1: TICKINT, BIT 2:CLKSOURCE
	for(i=0; i<n; i++) // Measure the time of 'n' periods
	{
		while (PIN_PERIOD!=0) // Wait for square wave to be 0
		{
			if(SYST_CSR & BIT16) return 0;
		}
		while (PIN_PERIOD==0) // Wait for square wave to be 1
		{
			if(SYST_CSR & BIT16) return 0;
		}
	}
	SYST_CSR = 0x00; // Disable Systick counter

	return 0xffffff-SYST_CVR;
}

void ConfigPins(void)
{
	GPIO_DIR0 &= ~(BIT13);  // Configure PIO0_13 as input (pin 2).
	GPIO_DIR0 |= BIT14;    // Configure PIO0_14 as output (pin 20).
	
	
	SWM_PINENABLE0 |= BIT4; // Disable SWCLK on pin PIO0_3
	GPIO_DIR0 |= BIT3;    // Configure PIO0_3  as output (pin 7).
	
	SWM_PINENABLE0 |= BIT5; // Disable SWIO on pin PIO0_2
	GPIO_DIR0 |= BIT2;    // Configure PIO0_2  as output (pin 8).
	
	// Warning: PIO0_11 and PIO0_10 need external pull-up resistors to 3.3V (1k seems to work)
	// Check page 90 of the user manual, section: 8.4.3 Pin mode
	GPIO_DIR0 |= BIT11;   // Configure PIO0_11 as output (pin 9).
	GPIO_DIR0 |= BIT10;   // Configure PIO0_10 as output (pin 10).
	GPIO_DIR0 |= BIT15;   // Configure PIO0_15 as output (pin 11).
	GPIO_DIR0 |= BIT1;    // Configure PIO0_1  as output (pin 12).
	GPIO_DIR0 |= BIT9;    // Configure PIO0_9  as output (pin 13).
}

/* Start ADC calibration */
void ADC_Calibration(void)
{
	unsigned int saved_ADC_CTRL;

	// Follow the instructions from the user manual (21.3.4 Hardware self-calibration)
	
	//To calibrate the ADC follow these steps:
	
	//1. Save the current contents of the ADC CTRL register if different from default.	
	saved_ADC_CTRL=ADC_CTRL;
	// 2. In a single write to the ADC CTRL register, do the following to start the
	//    calibration:
	//    � Set the calibration mode bit CALMODE.
	//    � Write a divider value to the CLKDIV bit field that divides the system
	//      clock to yield an ADC clock of about 500 kHz.
	//    � Clear the LPWR bit.
	ADC_CTRL = BIT30 | ((300/5)-1); // BIT30=CALMODE, BIT10=LPWRMODE, BIT7:0=CLKDIV
	// 3. Poll the CALMODE bit until it is cleared.
	while(ADC_CTRL&BIT30);
	// Before launching a new A/D conversion, restore the contents of the CTRL
	// register or use the default values.
	ADC_CTRL=saved_ADC_CTRL;
}


void InitADC(void)
{
	// Will use pins 1 and 2 of TSSOP-20 package (PIO_23 and PIO_17) for ADC.
	// These correspond to ADC Channel 3 and 9.  Also connect the
	// VREFN pin (pin 17 of TSSOP-20) to GND, and VREFP the
	// pin (pin 17 of TSSOP-20) to VDD (3.3V).
	
	SYSCON_PDRUNCFG &= ~BIT4; // Power up the ADC
	SYSCON_SYSAHBCLKCTRL |= BIT24;// Start the ADC Clocks
	ADC_Calibration();
	ADC_SEQA_CTRL &= ~BIT31; // Ensure SEQA_ENA is disabled before making changes	
	
	ADC_CTRL =1;// Set the ADC Clock divisor
	SWM_PINENABLE0 &= ~BIT16; // Enable the ADC function on PIO_23 (ADC_3, pin 1 of TSSOP20)	
	SWM_PINENABLE0 &= ~BIT22; // Enable the ADC function on PIO_17 (ADC_9, pin 9 of TSSOP20)	
}

// WARNING: in order to use the ADC with other pins, the pins need to be configured in
// the function above.
int ReadADC(int channel)
{
	ADC_SEQA_CTRL &= ~BIT31; // Ensure SEQA_ENA is disabled before making changes
	ADC_SEQA_CTRL &= 0xfffff000; // Deselect all previously selected channels	
	ADC_SEQA_CTRL |= (1<<channel); // Select Channel	
	ADC_SEQA_CTRL |= BIT31 + BIT18; // Set SEQA and Trigger polarity bits
	ADC_SEQA_CTRL |= BIT26; // Start a conversion:
	while( (ADC_SEQA_GDAT & BIT31)==0); // Wait for data valid
	return ( (ADC_SEQA_GDAT >> 4) & 0xfff);
}

// In order to keep this as nimble as possible, avoid
// using floating point or printf on any of its forms!
void main(void)
{
	int i, j, u, v;
	long int count, f;
	unsigned char LED_toggle=0;

	//int coinadder = 0;
	
	ConfigPins();	
	initUART(115200);
	InitADC();
	InitTimer();
	enable_interrupts();
	
	waitms(500); // Give PuTTY time to start
	/*	
	eputs("\x1b[2J\x1b[1;1H"); // Clear screen using ANSI escape sequence.
	eputs("\r\nLPC824 multi I/O example.\r\n");
	eputs("Measures the voltage at channels 3 and 9 (pins 1 and 2 of TSSOP20 package)\r\n");
	eputs("Measures period on PIO0_13 (pin 3 of TSSOP20 package)\r\n");
	eputs("Toggles PIO0_3, PIO0_2, PIO0_11, PIO0_10, PIO0_15, PIO0_1, PIO0_9 (pins 7, 8, 9, 10, 11 of TSSOP20 package)\r\n");
	eputs("Generates servo PWMs on PIO0_1, PIO0_9 (pins 12, 13 of TSSOP20 package)\r\n");
	eputs("WARNING: PIO0_11 (pin 9) and PIO0_10 (pin 10) need external pull-up resistors to 3.3V (1k seems to work)\r\n");
	*/
	while(1)	
	{	
		
		j=ReadADC(3);
		i=ReadADC(9);
		
		// testing ADC value for input
		
		/*
		if (j >= 0x0ff0){
		//eputs("--- EMAG ON ---\r\n");
		SERVO_ON = 1; // TURN ON SERVO MOVEMENT
		//printf("%d", coinadder); //sends a signal of 1 to the live python script
		*/
		
		// ---
		
		v=(j*33000)/0xfff;
		
		eputs("ADC[3]=0x");
		PrintNumber(j, 16, 4);
		eputs(", ");
		PrintNumber(v/10000, 10, 1);
		eputc('.');
		PrintNumber(v%10000, 10, 4);
		eputs("V ");;
		
		

		i=ReadADC(9); //REMOVED TO HAVE ONLY ONE INPUT FOR SERVO MOTION
		

		// changed j's to i's and v's to u's to read ADC[9] instead of ADC[3]
		u=(i*33000)/0xfff;
		
		eputs("ADC[9]=0x");
		PrintNumber(i, 16, 4);
		eputs(", ");
		PrintNumber(u/10000, 10, 1);
		eputc('.');
		PrintNumber(u%10000, 10, 4);
		eputs("V ");;
		
		
		if (v/10000 >= 1 || u/10000 >= 1 /*|| v%10000 >= 1300 || u%10000 >= 1300*/){
		
		//eputs("--- XXXXXXX LINE DETECTED XXXXXXXX ---\r\n");
		
		BOARDER = 1;
		}
		

		GPIO_B14 = 1;	
		count=GetPeriod(35);
		GPIO_B14 = 0;	
		if(count>0)
		{
			f=(F_CPU*35L)/count;
			
			eputs("f=");
			PrintNumber(f, 10, 7);
			eputs("Hz, count=");
			PrintNumber(count, 10, 6);
			eputs("          \r");
			
		}
		
		else
		{
			eputs("NO SIGNAL                     \r");
		}
		
		
		// NEW EMAG ACTIVATION FROM COIN DETECTION
		
		if (count <= 27635){
		//eputs("--- EMAG ON ---\r\n");		
		//eputs("WINNER WINNER CHICKEN DINNER ");
		SERVO_ON = 1; // TURN ON SERVO MOVEMENT
		
		//coinadder++;

		}
		eputs("coinadder:\r");
		PrintNumber(coinadder, 10, 6);
		
		if (coinadder >= 30){
		
		//eputs("GAHDAYUMMMMMMMMMMMMMMMMMM\r");
		}
		// ---
		
		// 	SERVO EMAG DEBUG ---
		
		
		if (ISR_cnt2 >= 1){
		
		// TURN ON EMAG
		//eputs("--- EMAG ON ---\r\n");
		GPIO_B15=1;

		
		} 
		else {
		
		// TURN OFF EMAG
		//eputs("--- EMAG OFF ---\r\n");
		GPIO_B15=0;
		
		}
		
		while (coinadder >= 200){ // STOP WHEELS AFTER 20 COINS
		
			GPIO_B3=0; // PIN 7
			GPIO_B2=0; // PIN 8
			
			GPIO_B11=0; // PIN 9
			GPIO_B10=0; // PIN 10 
		
		}
		
		
		
		
		//GPIO_B15=1; // TESTING PIN 11 ON
		
		/*
		GPIO_B3=0; // PIN 7
		GPIO_B2=0; // PIN 8
		
		GPIO_B11=0; // PIN 9
		GPIO_B10=0; // PIN 10 
		*/
		
		/*
				 BACK  FORWARD
		PIN 7   | 1   | 0
		PIN 8   | 0   | 1
		
		PIN 9   | 1   | 0
		PIN 10  | 0   | 1
					
		
		*/
		
		
		
		// ----- 
		
		
		/*
		// Now toggle the pins on/off to see if they are working.
		// First turn all off:
		GPIO_B3=0;
		GPIO_B2=0;
		GPIO_B11=0;
		GPIO_B10=0;
		GPIO_B15=0;
		GPIO_B1=0;
		GPIO_B9=0;
		// Now turn on one of outputs per cycle to check
		switch (LED_toggle++)
		{
			case 0:
				GPIO_B3=1;
				break;
			case 1:
				GPIO_B2=1;
				break;
			case 2:
				GPIO_B11=1;
				break;
			case 3:
				GPIO_B10=1;
				break;
			case 4:
				GPIO_B15=1;
				break;
			default:
				break;
		}
		if(LED_toggle>4) LED_toggle=0;
		
		waitms(200);
		
		*/	
		
	}
}
