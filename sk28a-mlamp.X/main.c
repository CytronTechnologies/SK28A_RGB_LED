/************************************************************
* SK28A SIMPLE RGB LED MOOD LAMP PROJECT                    *
* BY NG YONG HAN                                            *
*************************************************************/

#include <pic.h>

#define _XTAL_FREQ 1000000

__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & VCAPEN_OFF & PLLEN_OFF & STVREN_OFF & BORV_LO & LVP_OFF);

/*******************************************************************************
* INTERRUPT FUNCTION: timers_int()
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Handles PWM and Timer interrupts.
*
*******************************************************************************/
void interrupt timers_int()
{
    if(PIR1bits.TMR2IF == 1)
        PIR1bits.TMR2IF = 0;
    
    if(PIR3bits.TMR4IF == 1)
        PIR3bits.TMR4IF = 0;

    if(PIR3bits.TMR6IF == 1)
        PIR3bits.TMR6IF = 0;

}

/*******************************************************************************
* PUBLIC FUNCTION: init()
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the peripherals in SK28A.
*
*******************************************************************************/
void init()
{
    APFCONbits.CCP3SEL = 1;     // RB5 pin selected as CCP3 pin
    ADCON0bits.ADON = 0;        // Shut off ADC module
    ANSELA = 0x00;              // All pins are digital
    ANSELB = 0x00;              //
    LCDCONbits.LCDEN = 0;       // Shut off LCD driver
    OSCCONbits.IRCF3 = 1;       // 1MHz internal clock
    OSCCONbits.IRCF2 = 0;       //
    OSCCONbits.IRCF1 = 1;       //
    OSCCONbits.IRCF1 = 1;       //
    OSCCONbits.SCS1 = 1;        // internal oscillator
    OSCCONbits.SCS0 = 1;        //
    TRISA = 0x00;               // clear TRISA
    TRISB = 0x00;               // clear TRISB
    TRISC = 0x00;               // clear TRISC
    PORTA = 0x00;               // clear PORTA
    PORTB = 0x00;               // clear PORTB
}

/*******************************************************************************
* PUBLIC FUNCTION: init_pwm()
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize PWM modules. All Timers have 1x prescalers only.
*
*******************************************************************************/
void init_pwm()
{
    TRISAbits.TRISA4 = 1;       // CCP5 pin disabled
    TRISBbits.TRISB5 = 1;       // CCP3 pin disabled
    TRISBbits.TRISB0 = 1;       // CCP4 pin disabled

    CCP4CONbits.CCP4M3 = 1;     // enable PWM on CCP4
    CCP4CONbits.CCP4M2 = 1;     //
    CCP4CONbits.CCP4M1 = 0;     //
    CCP4CONbits.CCP4M0 = 0;     //

    CCP5CONbits.CCP5M3 = 1;     // enable PWM on CCP5
    CCP5CONbits.CCP5M2 = 1;     //
    CCP5CONbits.CCP5M1 = 0;     //
    CCP5CONbits.CCP5M0 = 0;     //

    CCP3CONbits.P3M1 = 0;       // P3A, P3B as port pins
    CCP3CONbits.P3M0 = 0;       //

    CCP3CONbits.CCP3M3 = 1;     // enable PWM on CCP3
    CCP3CONbits.CCP3M2 = 1;     //
    CCP3CONbits.CCP3M1 = 0;     //
    CCP3CONbits.CCP3M0 = 0;     //

    CCPTMRS0bits.C3TSEL1 = 0;   // CCP3 is on Timer2
    CCPTMRS0bits.C3TSEL0 = 0;   //

    CCPTMRS0bits.C4TSEL1 = 0;   // CCP4 is on Timer4
    CCPTMRS0bits.C4TSEL0 = 1;   //

    CCPTMRS1bits.C5TSEL1 = 1;   // CCP5 is on Timer6
    CCPTMRS1bits.C5TSEL0 = 0;   //

    PR2 = 250;                  // Timer 2 PR2
    T2CON = 0x00;               // clear T2CON, no prescalers or postscalers
    TMR2 = 0x00;                // clear Timer 2 register
    PIR1bits.TMR2IF = 0;        // clear Timer 2 interrupt flag
    PIE1bits.TMR2IE = 1;        // Enable Timer 2 interrupt
    T2CONbits.TMR2ON = 1;       // Switches on Timer 2

    PR4 = 250;                  // Timer 4 PR4
    T4CON = 0x00;               // clear T4CON, no prescalers or postscalers
    TMR4 = 0x00;                // clear Timer 4 register
    PIR3bits.TMR4IF = 0;        // clear Timer 4 interrupt flag
    PIE3bits.TMR4IE = 1;        // Enable Timer 4 interrupt
    T4CONbits.TMR4ON = 1;       // Switches on Timer 4

    PR6 = 250;                  // Timer 6 PR6
    T6CON = 0x00;               // clear T6CON, no prescalers or postscalers
    TMR6 = 0x00;                // clear Timer 6 register
    PIR3bits.TMR6IF = 0;        // clear Timer 6 interrupt flag
    PIE3bits.TMR6IE = 1;        // Enable Timer 6 interrupt
    T6CONbits.TMR6ON = 1;       // Switches on Timer 6

}

/*******************************************************************************
* INTERRUPT FUNCTION: enable_interrupt()
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Enable interrupts for the whole microcontroller. Hi-Tech has a macro for this one
* but here is written for the sake of clarity.
*******************************************************************************/
void enable_interrupt()
{
    INTCONbits.GIE = 1;         // Enable global (all) interrupts
    INTCONbits.PEIE = 1;        // Enable peripheral interrupts

    __delay_ms(1);

    TRISAbits.TRISA4 = 0;       // Reenable CCP5 pin as output
    TRISBbits.TRISB5 = 0;       // Reenable CCP3 pin as output
    TRISBbits.TRISB0 = 0;       // Reenable CCP4 pin as output

}

/*******************************************************************************
* MAIN FUNCTION: main()
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Main body of the program. "count1" is a unsigned char variable and it is manipulated
* in the for loops to control the intensities of the RGB LED. Prior to starting the program
* all the channels are 0 in duty cycles first.
*
*******************************************************************************/
void main()
{
    unsigned char count1 = 0x00;
    __delay_ms(100);
    init();
    __delay_ms(100);
    init_pwm();
    __delay_ms(100);
    enable_interrupt();

    CCPR3L = 0;                 // Red Channel
    CCPR4L = 0;                 // Green Channel
    CCPR5L = 0;                 // Blue Channel

    while(1)
    {
        for(count1 = 0; count1 < 250; count1++) // ascending red
        {
            CCPR3L = count1;
            CCPR4L = 0;
            CCPR5L = 0;
            __delay_ms(10);
        }
        for(count1 = 0; count1 < 250; count1++) // descending red - ascending green
        {
            CCPR3L = 250-count1;
            CCPR4L = count1;
            CCPR5L = 0;
             __delay_ms(10);
        }
        for(count1 = 0; count1 < 250; count1++) // descending green - ascending blue
        {
            CCPR3L = 0;
            CCPR4L = 250-count1;
            CCPR5L = count1;
            __delay_ms(10);
        }
        for(count1 = 0; count1 < 250; count1++) // descending blue - ascending red-green
        {
            CCPR3L = count1;
            CCPR4L = count1;
            CCPR5L = 250-count1;
             __delay_ms(10);
        }
        for(count1 = 0; count1 < 250; count1++) // descending red-green - ascending blue-green
        {
            CCPR3L = 250-count1;
            CCPR4L = 250;
            CCPR5L = count1;
            __delay_ms(10);
        }
        for(count1 = 0; count1 < 250; count1++) // descending blue-green - ascending blue-red
        {
            CCPR3L = count1;
            CCPR4L = 250-count1;
            CCPR5L = 250;
             __delay_ms(10);
        }
        for(count1 = 0; count1 < 250; count1++) // descending blue-red - ascending red
        {
            CCPR3L = 250;
            CCPR4L = 0;
            CCPR5L = 250-count1;
             __delay_ms(10);
        }
        for(count1 = 0; count1 < 250; count1++) // descending red
        {
            CCPR3L = 250-count1;
            CCPR4L = 0;
            CCPR5L = 0;
             __delay_ms(10);
        }        
    }
}