//#define USE_SPECIAL             //check for custom drivers

#define WR_ACTIVE2  {WR_ACTIVE; WR_ACTIVE;}
#define WR_ACTIVE4  {WR_ACTIVE2; WR_ACTIVE2;}
#define WR_ACTIVE8  {WR_ACTIVE4; WR_ACTIVE4;}
#define RD_ACTIVE2  {RD_ACTIVE; RD_ACTIVE;}
#define RD_ACTIVE4  {RD_ACTIVE2; RD_ACTIVE2;}
#define RD_ACTIVE8  {RD_ACTIVE4; RD_ACTIVE4;}
#define RD_ACTIVE16 {RD_ACTIVE8; RD_ACTIVE8;}
#define WR_IDLE2  {WR_IDLE; WR_IDLE;}
#define WR_IDLE4  {WR_IDLE2; WR_IDLE2;}
#define RD_IDLE2  {RD_IDLE; RD_IDLE;}
#define RD_IDLE4  {RD_IDLE2; RD_IDLE2;}

//################################### UNO ##############################
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328PB__)   //regular UNO shield on UNO
   //LCD pins  |D7 |D6 |D5 |D4 |D3 |D2 |D1 |D0 | |RD |WR |RS |CS |RST|
   //AVR   pin |PD7|PD6|PD5|PD4|PD3|PD2|PB1|PB0| |PC0|PC1|PC2|PC3|PC4|
   //UNO pins  |7  |6  |5  |4  |3  |2  |9  |8  | |A0 |A1 |A2 |A3 |A4 |
   #define RD_PORT PORTC
   #define RD_PIN  0
   #define WR_PORT PORTC
   #define WR_PIN  1
   #define CD_PORT PORTC
   #define CD_PIN  2
   #define CS_PORT PORTC
   #define CS_PIN  3
   #define RESET_PORT PORTC
   #define RESET_PIN  4
   
   #define BMASK         0x03              //more intuitive style for mixed Ports
   #define DMASK         0xFC              //does exactly the same as previous
   #define write_8(x)    { PORTB = (PORTB & ~BMASK) | ((x) & BMASK); PORTD = (PORTD & ~DMASK) | ((x) & DMASK); }
   #define read_8()      ( (PINB & BMASK) | (PIND & DMASK) )
   #define setWriteDir() { DDRB |=  BMASK; DDRD |=  DMASK; }
   #define setReadDir()  { DDRB &= ~BMASK; DDRD &= ~DMASK; }
   #define write8(x)     { write_8(x); WR_STROBE; }
   #define write16(x)    { uint8_t h = (x)>>8, l = x; write8(h); write8(l); }
   #define READ_8(dst)   { RD_STROBE; dst = read_8(); RD_IDLE; }
   #define READ_16(dst)  { uint8_t hi; READ_8(hi); READ_8(dst); dst |= (hi << 8); }
   
   #define PIN_LOW(p, b)        (p) &= ~(1<<(b))
   #define PIN_HIGH(p, b)       (p) |= (1<<(b))
   #define PIN_OUTPUT(p, b)     *(&p-1) |= (1<<(b))

   #define RD_ACTIVE  PIN_LOW(RD_PORT, RD_PIN)
   #define RD_IDLE    PIN_HIGH(RD_PORT, RD_PIN)
   #define RD_OUTPUT  PIN_OUTPUT(RD_PORT, RD_PIN)
   #define WR_ACTIVE  PIN_LOW(WR_PORT, WR_PIN)
   #define WR_IDLE    PIN_HIGH(WR_PORT, WR_PIN)
   #define WR_OUTPUT  PIN_OUTPUT(WR_PORT, WR_PIN)
   #define CD_COMMAND PIN_LOW(CD_PORT, CD_PIN)
   #define CD_DATA    PIN_HIGH(CD_PORT, CD_PIN)
   #define CD_OUTPUT  PIN_OUTPUT(CD_PORT, CD_PIN)
   #define CS_ACTIVE  PIN_LOW(CS_PORT, CS_PIN)
   #define CS_IDLE    PIN_HIGH(CS_PORT, CS_PIN)
   #define CS_OUTPUT  PIN_OUTPUT(CS_PORT, CS_PIN)
   #define RESET_ACTIVE  PIN_LOW(RESET_PORT, RESET_PIN)
   #define RESET_IDLE    PIN_HIGH(RESET_PORT, RESET_PIN)
   #define RESET_OUTPUT  PIN_OUTPUT(RESET_PORT, RESET_PIN)
//############################# END OF BLOCKS ############################
#elif defined(RISCDUINO_UNO)      //regular UNO shield on UNO
   //LCD pins  |D7 |D6 |D5 |D4 |D3 |D2 |D1 |D0 | |RD |WR |RS |CS |RST|
   //AVR   pin |PD7|PD6|PD5|PD4|PD3|PD2|PB1|PB0| |PC0|PC1|PC2|PC3|PC4|
   //UNO pins  |7  |6  |5  |4  |3  |2  |9  |8  | |A0 |A1 |A2 |A3 |A4 |
   #define RD_PORT PORTC
   #define RD_PIN  0
   #define WR_PORT PORTC
   #define WR_PIN  1
   #define CD_PORT PORTC
   #define CD_PIN  2
   #define CS_PORT PORTC
   #define CS_PIN  3
   #define RESET_PORT PORTC
   #define RESET_PIN  4
   
   #define BMASK         0x03              //more intuitive style for mixed Ports
   #define DMASK         0xFC              //does exactly the same as previous
   #define write_8(x)    { PORTB = (PORTB & ~BMASK) | ((x) & BMASK); PORTD = (PORTD & ~DMASK) | ((x) & DMASK); }
   #define read_8()      ( (PINB & BMASK) | (PIND & DMASK) )
   #define setWriteDir() { DDRB |=  BMASK; DDRD |=  DMASK; }
   #define setReadDir()  { DDRB &= ~BMASK; DDRD &= ~DMASK; }
   #define write8(x)     { write_8(x); WR_STROBE; }
   #define write16(x)    { uint8_t h = (x)>>8, l = x; write8(h); write8(l); }
   #define READ_8(dst)   { RD_STROBE; dst = read_8(); RD_IDLE; }
   #define READ_16(dst)  { uint8_t hi; READ_8(hi); READ_8(dst); dst |= (hi << 8); }
   
   #define PIN_LOW(p, b)        (p) &= ~(1<<(b))
   #define PIN_HIGH(p, b)       (p) |= (1<<(b))
   #define PIN_OUTPUT(p, b)     (p) |= (1<<(b))

   #define RD_ACTIVE     PIN_LOW(RD_PORT, RD_PIN)
   #define RD_IDLE       PIN_HIGH(RD_PORT, RD_PIN)
   #define RD_OUTPUT     PIN_OUTPUT(DDRC, RD_PIN)
   #define WR_ACTIVE     PIN_LOW(WR_PORT, WR_PIN)
   #define WR_IDLE       PIN_HIGH(WR_PORT, WR_PIN)
   #define WR_OUTPUT     PIN_OUTPUT(DDRC, WR_PIN)
   #define CD_COMMAND    PIN_LOW(CD_PORT, CD_PIN)
   #define CD_DATA       PIN_HIGH(CD_PORT, CD_PIN)
   #define CD_OUTPUT     PIN_OUTPUT(DDRC, CD_PIN)
   #define CS_ACTIVE     PIN_LOW(CS_PORT, CS_PIN)
   #define CS_IDLE       PIN_HIGH(CS_PORT, CS_PIN)
   #define CS_OUTPUT     PIN_OUTPUT(DDRC, CS_PIN)
   #define RESET_ACTIVE  PIN_LOW(RESET_PORT, RESET_PIN)
   #define RESET_IDLE    PIN_HIGH(RESET_PORT, RESET_PIN)
   #define RESET_OUTPUT  PIN_OUTPUT(DDRC, RESET_PIN)
#else
#error MCU unsupported
#endif                          // regular UNO shields on Arduino boards



 // General macros.   IOCLR registers are 1 cycle when optimised.
#define WR_STROBE { WR_ACTIVE; WR_IDLE; }       //PWLW=TWRL=50ns
#define RD_STROBE RD_IDLE, RD_ACTIVE, RD_ACTIVE, RD_ACTIVE      //PWLR=TRDL=150ns, tDDR=100ns

#define CTL_INIT()   { RD_OUTPUT; WR_OUTPUT; CD_OUTPUT; CS_OUTPUT; RESET_OUTPUT; }
#define WriteCmd(x)  { CD_COMMAND; write16(x); CD_DATA; }
#define WriteData(x) { write16(x); }
