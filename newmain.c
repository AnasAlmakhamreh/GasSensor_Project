#define _XTAL_FREQ 4000000

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

// CONFIG (standard)
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit
#pragma config BOREN = OFF      // Brown-out Reset Enable bit
#pragma config LVP = OFF        // Low-Voltage (single-supply) In-Circuit Serial Programming
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

// LCD pins
#define RS RD0
#define EN RD1
#define D4 RD2
#define D5 RD3
#define D6 RD4
#define D7 RD5

// Outputs
#define YELLOW_LED RB1
#define RED_LED    RB2
#define BUZZER     RB3

// Function prototypes
void ADC_Init();
unsigned int ADC_Read(unsigned char channel);
void Lcd_Init();
void Lcd_Command(unsigned char);
void Lcd_Char(unsigned char);
void Lcd_String(const char*);
void Lcd_Set_Cursor(char, char);
void Lcd_Clear();

void main() {
    unsigned int gas_level = 0;
    char buffer[16];

    // Setup
    TRISA = 0xFF;  // RA0 as input (ADC)
    TRISB = 0x00;  // LEDs and buzzer
    TRISD = 0x00;  // LCD pins

    ADC_Init();
    Lcd_Init();
    Lcd_Clear();

    while (1) {
        gas_level = ADC_Read(0); // Read from AN0

        // LCD Display
        Lcd_Set_Cursor(1, 1);
        Lcd_String("Gas Detected");
        Lcd_Set_Cursor(2, 1);
        sprintf(buffer, "Gas Level: %d", gas_level);
        Lcd_String(buffer);

        // LED & Buzzer Logic
        YELLOW_LED = (gas_level >= 400) ? 1 : 0;
        RED_LED    = (gas_level >= 700) ? 1 : 0;
        BUZZER     = (gas_level >= 900) ? 1 : 0;

        __delay_ms(500);
        Lcd_Clear();
    }
}

void ADC_Init() {
    ADCON1 = 0x80; // Right justify, Fosc/32
    ADCON0 = 0x41; // Turn on ADC, select AN0
}

unsigned int ADC_Read(unsigned char channel) {
    if (channel > 7) return 0;
    ADCON0 &= 0x11000101;             // Clear channel select bits
    ADCON0 |= (channel << 3);         // Set new channel
    __delay_ms(2);                    // Acquisition time
    GO_nDONE = 1;
    while (GO_nDONE);
    return ((ADRESH << 8) + ADRESL);  // Return result
}

// ==== LCD FUNCTIONS BELOW ====

void Lcd_Command(unsigned char cmd) {
    RS = 0;
    D4 = (cmd >> 4) & 1;
    D5 = (cmd >> 5) & 1;
    D6 = (cmd >> 6) & 1;
    D7 = (cmd >> 7) & 1;
    EN = 1; __delay_ms(1); EN = 0;

    D4 = cmd & 1;
    D5 = (cmd >> 1) & 1;
    D6 = (cmd >> 2) & 1;
    D7 = (cmd >> 3) & 1;
    EN = 1; __delay_ms(1); EN = 0;
}

void Lcd_Char(unsigned char data) {
    RS = 1;
    D4 = (data >> 4) & 1;
    D5 = (data >> 5) & 1;
    D6 = (data >> 6) & 1;
    D7 = (data >> 7) & 1;
    EN = 1; __delay_ms(1); EN = 0;

    D4 = data & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    EN = 1; __delay_ms(1); EN = 0;
}

void Lcd_Init() {
    Lcd_Command(0x02); // 4-bit mode
    Lcd_Command(0x28); // 2 lines, 5x7
    Lcd_Command(0x0C); // Display on, cursor off
    Lcd_Command(0x06); // Auto increment
    Lcd_Command(0x01); // Clear display
    __delay_ms(2);
}

void Lcd_String(const char* str) {
    while (*str) Lcd_Char(*str++);
}

void Lcd_Set_Cursor(char row, char col) {
    char pos = (row == 1) ? 0x80 + (col - 1) : 0xC0 + (col - 1);
    Lcd_Command(pos);
}

void Lcd_Clear() {
    Lcd_Command(0x01);
    __delay_ms(2);
}
