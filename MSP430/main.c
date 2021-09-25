#include <MSP430F2418.h>
#include <stdint.h>

void delay(unsigned int count)  // Function to provide delay Approx 1ms 
{
	 uint8_t i,j;
	for(i=0;i<count;i++)
	for(j=0;j<112;j++);
}

void LCD_Command (unsigned char cmd)	// command function
{
	P7OUT= cmd;
	P1OUT &= ~BIT6;		//Rs=0			
	P1OUT |= BIT7; 		//E=1
	delay(1);
	P1OUT &= ~BIT7;		//E=0
	delay(5);
}


void LCD_Data (unsigned char char_data)	// data write function
{
	P7OUT=char_data;
	P1OUT |= BIT6;		//Rs=1				
	P1OUT |= BIT7;   		//E=1			
	delay(1);	
	P1OUT &= ~BIT7;		//E=0
	delay(5);
}

void LCD_Init (void)		// LCD Initialize function 
{
	delay(20);		// LCD Power ON Initialization time >15ms 
	LCD_Command (0x38);	//Initialization of 16X2 LCD in 8bit mode 
	LCD_Command (0x0C);	// Display ON Cursor OFF 
	LCD_Command (0x01);	// Clear display 
        LCD_Command (0x06);		// Auto Increment cursor 
        LCD_Command (0x80);	
   
}
int cnt=0,dist=0;
int main (void)
 { 
    uint8_t i;
    uint8_t arr[14]="Distance:   cm";
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    BCSCTL1 = CALBC1_1MHZ;	//Set range to calibrated 1MHz
    DCOCTL = CALDCO_1MHZ;       //Set DCO step to calibrated 1MHz
    P7DIR = 0xFF;
    P7OUT = 0;
    P1DIR = 0xC1;
    P1SEL |= BIT1; 
    P1OUT = 0;
    
    TBCCTL0 = CCIE;  				        //enable interrupt
    TBCTL = TBSSEL_2 + MC_1 + ID_0 + TBCLR;  //Timer B used to create delay   
    TB0CCR0=50000;			//50 ms delay between each reading
  
    TACCTL0 = CM_3 + SCS + CCIS_0 + CAP + CCIE;  
//Capture both edges, sync, select signal to //capture, capture mode, enable interrupt
    TACTL = TASSEL_2 + MC_2 + ID_0 + TACLR;      //SMCLK, Continuous
     LCD_Init();
    
    for(i=0;i<14;i++)
    {
     LCD_Data(arr[i]);  
     }
     
   while (1)
   {  
        LCD_Command(0x89);
    
  if(dist >= 100)
      {
	 LCD_Data((dist/100)+'0');
         LCD_Data(((dist/10)%10)+'0');
         LCD_Data((dist%10)+'0');
      }
      else if (dist >= 10)
      {
	 LCD_Data((dist/10)+'0');
	 LCD_Data((dist%10)+'0');
      }
      else 
      { 
	 LCD_Data(dist+'0');	 
      }     
       __bis_SR_register(LPM4_bits | GIE);   //Wakes up from low power mode when Timer B (delay) is completed
    } 
   return 0;
 }   
 
 #pragma vector=TIMERA0_VECTOR
__interrupt void TimerA0(void)
{
	if (TACCTL0 & CCI)			// Raising edge
	{
	       cnt = TACCR0;		
	}
	else						// Falling edge
	{	
dist = ((TACCR0-cnt)/58)+1;		//Formula to Calculate distance in cm
	} 	
}

 #pragma vector=TIMERB0_VECTOR
__interrupt void TimerB0(void)
{
   //Send Trigger
     P1OUT |= BIT0; 	
     __delay_cycles(10); 				/10 micro seconds            
     P1OUT &= ~BIT0;	   
	 __bic_SR_register_on_exit( LPM4_bits);  
}
