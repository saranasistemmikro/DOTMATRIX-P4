// Author: Phil Kaziewicz 19th July 2014
//
// Thanks! - Serial input added and multi mode display formats - Graham Marshall Sept 2016
// see http://forum.arduino.cc/index.php?topic=222334.45
// ESP8266 driver and other notes are at -
// and http://www.sunspot.co.uk/Projects/displays/LEDmatrix16x64/LEDmatrix16x64.html
// curl commands to send data to the ESP8266 come from terminal in linux or OSX box
//
// 64x32 LED display matrix test code!@£
// based upon original code from canton-electonics
// Arduino 1.0.5 UNO R3
//#include <pgmspace.h>

// Connections to board
//const byte latchPin=8;
//const byte clockPin=12;
//const byte data_R1=10;
//const byte data_R2=11;
//const byte en_74138=2;
//const byte la_74138=3;
//const byte lb_74138=4;
//const byte lc_74138=5;
//const byte ld_74138=6;

const byte latchPin=30;
const byte clockPin=29;
const byte data_R1=28;
//const byte data_R2=11;

const byte en_74138=31;//2;

const byte la_74138=24;//3;
const byte lb_74138=25;//4;
const byte lc_74138=26;//5;
const byte ld_74138=27;//6;

int count=1;

char inData[40]    = "SSarana Sistem Mikro      >"; // Allocate some space for serial data received
//char inData[100] = "0-Fixed- Scrolling message >"; //
//  char inData[40]    = "SScroll 16x8 >"; // Allocate some space for serial data received
char inChar=-1; // Where to store the character read
byte index = 0; // Index into array; where to store the character

byte ScanRow = 0;
unsigned long counter;
int n = 0;

byte buffer[256] = { // Display buffer (which is scanned by the interrupt timer) of 8x32 bytes
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
// ---------------------------------------------------------------------------------------------------
// FONTS sourced from various locations on the internet
// full ASCII character set (8x16) (1520 bytes)
const uint8_t font8x16_basic[] PROGMEM = {
/* (32) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* ! (33) */ 0x00, 0x00, 0x00, 0x18, 0x3C, 0x3C, 0x3C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
/* " (34) */ 0x00, 0x00, 0x66, 0x66, 0x66, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* # (35) */ 0x00, 0x00, 0x00, 0x00, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x6C, 0xFE, 0x6C, 0x6C, 0x00, 0x00, 0x00,
/* $ (36) */ 0x00, 0x18, 0x18, 0x7C, 0xC6, 0xC2, 0xC0, 0x7C, 0x06, 0x06, 0x86, 0xC6, 0x7C, 0x18, 0x18, 0x00,
/* % (37) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0xC2, 0xC6, 0x0C, 0x18, 0x30, 0x60, 0xC6, 0x86, 0x00, 0x00, 0x00,
/* & (38) */ 0x00, 0x00, 0x00, 0x38, 0x6C, 0x6C, 0x38, 0x76, 0xDC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00,
/* ' (39) */ 0x00, 0x00, 0x30, 0x30, 0x30, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* ( (40) */ 0x00, 0x00, 0x00, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0C, 0x00, 0x00, 0x00,
/* ) (41) */ 0x00, 0x00, 0x00, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18, 0x30, 0x00, 0x00, 0x00,
/* * (42) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, 0x00, 0x00, 0x00,
/* + (43) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x7E, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00,
/* , (44) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x30, 0x00, 0x00,
/* - (45) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* . (46) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
/* / (47) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0x80, 0x00, 0x00, 0x00,
/* 0 (48) */ 0x00, 0x00, 0x00, 0x3C, 0x66, 0xC3, 0xC3, 0xDB, 0xDB, 0xC3, 0xC3, 0x66, 0x3C, 0x00, 0x00, 0x00,
/* 1 (49) */ 0x00, 0x00, 0x00, 0x18, 0x38, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x7E, 0x00, 0x00, 0x00,
/* 2 (50) */ 0x00, 0x00, 0x00, 0x7C, 0xC6, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xC0, 0xC6, 0xFE, 0x00, 0x00, 0x00,
/* 3 (51) */ 0x00, 0x00, 0x00, 0x7C, 0xC6, 0x06, 0x06, 0x3C, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* 4 (52) */ 0x00, 0x00, 0x00, 0x0C, 0x1C, 0x3C, 0x6C, 0xCC, 0xFE, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, 0x00, 0x00,
/* 5 (53) */ 0x00, 0x00, 0x00, 0xFE, 0xC0, 0xC0, 0xC0, 0xFC, 0x06, 0x06, 0x06, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* 6 (54) */ 0x00, 0x00, 0x00, 0x38, 0x60, 0xC0, 0xC0, 0xFC, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* 7 (55) */ 0x00, 0x00, 0x00, 0xFE, 0xC6, 0x06, 0x06, 0x0C, 0x18, 0x30, 0x30, 0x30, 0x30, 0x00, 0x00, 0x00,
/* 8 (56) */ 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* 9 (57) */ 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x06, 0x06, 0x0C, 0x78, 0x00, 0x00, 0x00,
/* : (58) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x00,
/* ; (59) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00, 0x18, 0x18, 0x30, 0x00, 0x00, 0x00,
/* < (60) */ 0x00, 0x00, 0x00, 0x00, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x00, 0x00, 0x00,
/* = (61) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x7E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* > (62) */ 0x00, 0x00, 0x00, 0x00, 0x60, 0x30, 0x18, 0x0C, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x00, 0x00, 0x00,
/* ? (63) */ 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0x0C, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x00, 0x00, 0x00,
/* @ (64) */ 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xDE, 0xDE, 0xDE, 0xDC, 0xC0, 0x7C, 0x00, 0x00, 0x00,
/* A (65) */ 0x00, 0x00, 0x00, 0x10, 0x38, 0x6C, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00,
/* B (66) */ 0x00, 0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x66, 0x66, 0x66, 0x66, 0xFC, 0x00, 0x00, 0x00,
/* C (67) */ 0x00, 0x00, 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xC0, 0xC0, 0xC2, 0x66, 0x3C, 0x00, 0x00, 0x00,
/* D (68) */ 0x00, 0x00, 0x00, 0xF8, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6C, 0xF8, 0x00, 0x00, 0x00,
/* E (69) */ 0x00, 0x00, 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00,
/* F (70) */ 0x00, 0x00, 0x00, 0xFE, 0x66, 0x62, 0x68, 0x78, 0x68, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00,
/* G (71) */ 0x00, 0x00, 0x00, 0x3C, 0x66, 0xC2, 0xC0, 0xC0, 0xDE, 0xC6, 0xC6, 0x66, 0x3A, 0x00, 0x00, 0x00,
/* H (72) */ 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xFE, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00,
/* I (73) */ 0x00, 0x00, 0x00, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
/* J (74) */ 0x00, 0x00, 0x00, 0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0xCC, 0xCC, 0xCC, 0x78, 0x00, 0x00, 0x00,
/* K (75) */ 0x00, 0x00, 0x00, 0xE6, 0x66, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00,
/* L (76) */ 0x00, 0x00, 0x00, 0xF0, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x62, 0x66, 0xFE, 0x00, 0x00, 0x00,
/* M (77) */ 0x00, 0x00, 0x00, 0xC3, 0xE7, 0xFF, 0xFF, 0xDB, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x00, 0x00, 0x00,
/* N (78) */ 0x00, 0x00, 0x00, 0xC6, 0xE6, 0xF6, 0xFE, 0xDE, 0xCE, 0xC6, 0xC6, 0xC6, 0xC6, 0x00, 0x00, 0x00,
/* O (79) */ 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* P (80) */ 0x00, 0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00,
/* Q (81) */ 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xD6, 0xDE, 0x7C, 0x0C, 0x0E, 0x00,
/* R (82) */ 0x00, 0x00, 0x00, 0xFC, 0x66, 0x66, 0x66, 0x7C, 0x6C, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00,
/* S (83) */ 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0x60, 0x38, 0x0C, 0x06, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* T (84) */ 0x00, 0x00, 0x00, 0xFF, 0xDB, 0x99, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
/* U (85) */ 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* V (86) */ 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00,
/* W (87) */ 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xDB, 0xDB, 0xFF, 0x66, 0x66, 0x00, 0x00, 0x00,
/* X (88) */ 0x00, 0x00, 0x00, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x18, 0x3C, 0x66, 0xC3, 0xC3, 0x00, 0x00, 0x00,
/* Y (89) */ 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
/* Z (90) */ 0x00, 0x00, 0x00, 0xFF, 0xC3, 0x86, 0x0C, 0x18, 0x30, 0x60, 0xC1, 0xC3, 0xFF, 0x00, 0x00, 0x00,
/* [ (91) */ 0x00, 0x00, 0x00, 0x3C, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x3C, 0x00, 0x00, 0x00,
/* \ (92) */ 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0x70, 0x38, 0x1C, 0x0E, 0x06, 0x02, 0x00, 0x00, 0x00,
/* ] (93) */ 0x00, 0x00, 0x00, 0x3C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x3C, 0x00, 0x00, 0x00,
/* ^ (94) */ 0x00, 0x10, 0x38, 0x6C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* _ (95) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x00,
/* ` (96) */ 0x00, 0x30, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* a (97) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0C, 0x7C, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00,
/* b (98) */ 0x00, 0x00, 0x00, 0xE0, 0x60, 0x60, 0x78, 0x6C, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x00, 0x00, 0x00,
/* c (99) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC0, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* d (100) */ 0x00, 0x00, 0x00, 0x1C, 0x0C, 0x0C, 0x3C, 0x6C, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00,
/* e (101) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xFE, 0xC0, 0xC0, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* f (102) */ 0x00, 0x00, 0x00, 0x38, 0x6C, 0x64, 0x60, 0xF0, 0x60, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00,
/* g (103) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0xCC, 0x78,
/* h (104) */ 0x00, 0x00, 0x00, 0xE0, 0x60, 0x60, 0x6C, 0x76, 0x66, 0x66, 0x66, 0x66, 0xE6, 0x00, 0x00, 0x00,
/* i (105) */ 0x00, 0x00, 0x00, 0x18, 0x18, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
/* j (106) */ 0x00, 0x00, 0x00, 0x06, 0x06, 0x00, 0x0E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x66, 0x66, 0x3C,
/* k (107) */ 0x00, 0x00, 0x00, 0xE0, 0x60, 0x60, 0x66, 0x6C, 0x78, 0x78, 0x6C, 0x66, 0xE6, 0x00, 0x00, 0x00,
/* l (108) */ 0x00, 0x00, 0x00, 0x38, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C, 0x00, 0x00, 0x00,
/* m (109) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6, 0xFF, 0xDB, 0xDB, 0xDB, 0xDB, 0xDB, 0x00, 0x00, 0x00,
/* n (110) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x66, 0x00, 0x00, 0x00,
/* o (111) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* p (112) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x66, 0x66, 0x66, 0x66, 0x66, 0x7C, 0x60, 0x60, 0xF0,
/* q (113) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x76, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x7C, 0x0C, 0x0C, 0x1E,
/* r (114) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDC, 0x76, 0x66, 0x60, 0x60, 0x60, 0xF0, 0x00, 0x00, 0x00,
/* s (115) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0xC6, 0x60, 0x38, 0x0C, 0xC6, 0x7C, 0x00, 0x00, 0x00,
/* t (116) */ 0x00, 0x00, 0x00, 0x10, 0x30, 0x30, 0xFC, 0x30, 0x30, 0x30, 0x30, 0x36, 0x1C, 0x00, 0x00, 0x00,
/* u (117) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0x76, 0x00, 0x00, 0x00,
/* v (118) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xC3, 0x66, 0x3C, 0x18, 0x00, 0x00, 0x00,
/* w (119) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0xC3, 0xC3, 0xDB, 0xDB, 0xFF, 0x66, 0x00, 0x00, 0x00,
/* x (120) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC3, 0x66, 0x3C, 0x18, 0x3C, 0x66, 0xC3, 0x00, 0x00, 0x00,
/* y (121) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0xC6, 0x7E, 0x06, 0x0C, 0xF8,
/* z (122) */ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0xCC, 0x18, 0x30, 0x60, 0xC6, 0xFE, 0x00, 0x00, 0x00,
/* { (123) */ 0x00, 0x00, 0x00, 0x0E, 0x18, 0x18, 0x18, 0x70, 0x18, 0x18, 0x18, 0x18, 0x0E, 0x00, 0x00, 0x00,
/* | (124) */ 0x00, 0x00, 0x00, 0x18, 0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00, 0x00, 0x00,
/* } (125) */ 0x00, 0x00, 0x00, 0x70, 0x18, 0x18, 0x18, 0x0E, 0x18, 0x18, 0x18, 0x18, 0x70, 0x00, 0x00, 0x00,
/* ~ (126) */ 0x00, 0x00, 0x00, 0x76, 0xDC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/* BLK(127)*/ 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};
// full ASCII character set (8x8) (760 bytes)
const uint8_t font8x8_basic[] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // U+0020 (space)
0x18, 0x3C, 0x3C, 0x18, 0x18, 0x00, 0x18, 0x00, // U+0021 (!)
0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // U+0022 (")
0x36, 0x36, 0x7F, 0x36, 0x7F, 0x36, 0x36, 0x00, // U+0023 (#)
0x0C, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x0C, 0x00, // U+0024 ($)
0x00, 0x63, 0x33, 0x18, 0x0C, 0x66, 0x63, 0x00, // U+0025 (%)
0x1C, 0x36, 0x1C, 0x6E, 0x3B, 0x33, 0x6E, 0x00, // U+0026 (&)
0x06, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, // U+0027 (')
0x18, 0x0C, 0x06, 0x06, 0x06, 0x0C, 0x18, 0x00, // U+0028 (()
0x06, 0x0C, 0x18, 0x18, 0x18, 0x0C, 0x06, 0x00, // U+0029 ())
0x00, 0x66, 0x3C, 0xFF, 0x3C, 0x66, 0x00, 0x00, // U+002A (*)
0x00, 0x0C, 0x0C, 0x3F, 0x0C, 0x0C, 0x00, 0x00, // U+002B (+)
0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x06, // U+002C (,)
0x00, 0x00, 0x00, 0x3F, 0x00, 0x00, 0x00, 0x00, // U+002D (-)
0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x0C, 0x00, // U+002E (.)
0x60, 0x30, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x00, // U+002F (/)
0x3E, 0x63, 0x73, 0x7B, 0x6F, 0x67, 0x3E, 0x00, // U+0030 (0)
0x0C, 0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x3F, 0x00, // U+0031 (1)
0x1E, 0x33, 0x30, 0x1C, 0x06, 0x33, 0x3F, 0x00, // U+0032 (2)
0x1E, 0x33, 0x30, 0x1C, 0x30, 0x33, 0x1E, 0x00, // U+0033 (3)
0x38, 0x3C, 0x36, 0x33, 0x7F, 0x30, 0x78, 0x00, // U+0034 (4)
0x3F, 0x03, 0x1F, 0x30, 0x30, 0x33, 0x1E, 0x00, // U+0035 (5)
0x1C, 0x06, 0x03, 0x1F, 0x33, 0x33, 0x1E, 0x00, // U+0036 (6)
0x3F, 0x33, 0x30, 0x18, 0x0C, 0x0C, 0x0C, 0x00, // U+0037 (7)
0x1E, 0x33, 0x33, 0x1E, 0x33, 0x33, 0x1E, 0x00, // U+0038 (8)
0x1E, 0x33, 0x33, 0x3E, 0x30, 0x18, 0x0E, 0x00, // U+0039 (9)
0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x00, // U+003A (:)
0x00, 0x0C, 0x0C, 0x00, 0x00, 0x0C, 0x0C, 0x06, // U+003B (//)
0x18, 0x0C, 0x06, 0x03, 0x06, 0x0C, 0x18, 0x00, // U+003C (<)
0x00, 0x00, 0x3F, 0x00, 0x00, 0x3F, 0x00, 0x00, // U+003D (=)
0x06, 0x0C, 0x18, 0x30, 0x18, 0x0C, 0x06, 0x00, // U+003E (>)
0x1E, 0x33, 0x30, 0x18, 0x0C, 0x00, 0x0C, 0x00, // U+003F (?)
0x3E, 0x63, 0x7B, 0x7B, 0x7B, 0x03, 0x1E, 0x00, // U+0040 (@)
0x0C, 0x1E, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x00, // U+0041 (A)
0x3F, 0x66, 0x66, 0x3E, 0x66, 0x66, 0x3F, 0x00, // U+0042 (B)
0x3C, 0x66, 0x03, 0x03, 0x03, 0x66, 0x3C, 0x00, // U+0043 (C)
0x1F, 0x36, 0x66, 0x66, 0x66, 0x36, 0x1F, 0x00, // U+0044 (D)
0x7F, 0x46, 0x16, 0x1E, 0x16, 0x46, 0x7F, 0x00, // U+0045 (E)
0x7F, 0x46, 0x16, 0x1E, 0x16, 0x06, 0x0F, 0x00, // U+0046 (F)
0x3C, 0x66, 0x03, 0x03, 0x73, 0x66, 0x7C, 0x00, // U+0047 (G)
0x33, 0x33, 0x33, 0x3F, 0x33, 0x33, 0x33, 0x00, // U+0048 (H)
0x1E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, // U+0049 (I)
0x78, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, 0x00, // U+004A (J)
0x67, 0x66, 0x36, 0x1E, 0x36, 0x66, 0x67, 0x00, // U+004B (K)
0x0F, 0x06, 0x06, 0x06, 0x46, 0x66, 0x7F, 0x00, // U+004C (L)
0x63, 0x77, 0x7F, 0x7F, 0x6B, 0x63, 0x63, 0x00, // U+004D (M)
0x63, 0x67, 0x6F, 0x7B, 0x73, 0x63, 0x63, 0x00, // U+004E (N)
0x1C, 0x36, 0x63, 0x63, 0x63, 0x36, 0x1C, 0x00, // U+004F (O)
0x3F, 0x66, 0x66, 0x3E, 0x06, 0x06, 0x0F, 0x00, // U+0050 (P)
0x1E, 0x33, 0x33, 0x33, 0x3B, 0x1E, 0x38, 0x00, // U+0051 (Q)
0x3F, 0x66, 0x66, 0x3E, 0x36, 0x66, 0x67, 0x00, // U+0052 (R)
0x1E, 0x33, 0x07, 0x0E, 0x38, 0x33, 0x1E, 0x00, // U+0053 (S)
0x3F, 0x2D, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, // U+0054 (T)
0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3F, 0x00, // U+0055 (U)
0x33, 0x33, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00, // U+0056 (V)
0x63, 0x63, 0x63, 0x6B, 0x7F, 0x77, 0x63, 0x00, // U+0057 (W)
0x63, 0x63, 0x36, 0x1C, 0x1C, 0x36, 0x63, 0x00, // U+0058 (X)
0x33, 0x33, 0x33, 0x1E, 0x0C, 0x0C, 0x1E, 0x00, // U+0059 (Y)
0x7F, 0x63, 0x31, 0x18, 0x4C, 0x66, 0x7F, 0x00, // U+005A (Z)
0x1E, 0x06, 0x06, 0x06, 0x06, 0x06, 0x1E, 0x00, // U+005B ([)
0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x40, 0x00, // U+005C (\)
0x1E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1E, 0x00, // U+005D (])
0x08, 0x1C, 0x36, 0x63, 0x00, 0x00, 0x00, 0x00, // U+005E (^)
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, // U+005F (_)
0x0C, 0x0C, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, // U+0060 (`)
0x00, 0x00, 0x1E, 0x30, 0x3E, 0x33, 0x6E, 0x00, // U+0061 (a)
0x07, 0x06, 0x06, 0x3E, 0x66, 0x66, 0x3B, 0x00, // U+0062 (b)
0x00, 0x00, 0x1E, 0x33, 0x03, 0x33, 0x1E, 0x00, // U+0063 (c)
0x38, 0x30, 0x30, 0x3e, 0x33, 0x33, 0x6E, 0x00, // U+0064 (d)
0x00, 0x00, 0x1E, 0x33, 0x3f, 0x03, 0x1E, 0x00, // U+0065 (e)
0x1C, 0x36, 0x06, 0x0f, 0x06, 0x06, 0x0F, 0x00, // U+0066 (f)
0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x1F, // U+0067 (g)
0x07, 0x06, 0x36, 0x6E, 0x66, 0x66, 0x67, 0x00, // U+0068 (h)
0x0C, 0x00, 0x0E, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, // U+0069 (i)
0x30, 0x00, 0x30, 0x30, 0x30, 0x33, 0x33, 0x1E, // U+006A (j)
0x07, 0x06, 0x66, 0x36, 0x1E, 0x36, 0x67, 0x00, // U+006B (k)
0x0E, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x1E, 0x00, // U+006C (l)
0x00, 0x00, 0x33, 0x7F, 0x7F, 0x6B, 0x63, 0x00, // U+006D (m)
0x00, 0x00, 0x1F, 0x33, 0x33, 0x33, 0x33, 0x00, // U+006E (n)
0x00, 0x00, 0x1E, 0x33, 0x33, 0x33, 0x1E, 0x00, // U+006F (o)
0x00, 0x00, 0x3B, 0x66, 0x66, 0x3E, 0x06, 0x0F, // U+0070 (p)
0x00, 0x00, 0x6E, 0x33, 0x33, 0x3E, 0x30, 0x78, // U+0071 (q)
0x00, 0x00, 0x3B, 0x6E, 0x66, 0x06, 0x0F, 0x00, // U+0072 (r)
0x00, 0x00, 0x3E, 0x03, 0x1E, 0x30, 0x1F, 0x00, // U+0073 (s)
0x08, 0x0C, 0x3E, 0x0C, 0x0C, 0x2C, 0x18, 0x00, // U+0074 (t)
0x00, 0x00, 0x33, 0x33, 0x33, 0x33, 0x6E, 0x00, // U+0075 (u)
0x00, 0x00, 0x33, 0x33, 0x33, 0x1E, 0x0C, 0x00, // U+0076 (v)
0x00, 0x00, 0x63, 0x6B, 0x7F, 0x7F, 0x36, 0x00, // U+0077 (w)
0x00, 0x00, 0x63, 0x36, 0x1C, 0x36, 0x63, 0x00, // U+0078 (x)
0x00, 0x00, 0x33, 0x33, 0x33, 0x3E, 0x30, 0x1F, // U+0079 (y)
0x00, 0x00, 0x3F, 0x19, 0x0C, 0x26, 0x3F, 0x00, // U+007A (z)
0x38, 0x0C, 0x0C, 0x07, 0x0C, 0x0C, 0x38, 0x00, // U+007B ({)
0x18, 0x18, 0x18, 0x00, 0x18, 0x18, 0x18, 0x00, // U+007C (|)
0x07, 0x0C, 0x0C, 0x38, 0x0C, 0x0C, 0x07, 0x00, // U+007D (})
0x6E, 0x3B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // U+007E (~)
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // U+007F (block)
};

// Routine to print a single character in one of 8 columns
// Inputs:
// x is one of (0,16,24,32,40,48,56),
// y (0 to 16 or 24 depending upon font size),
// n is either (0 to 9) or an ascii value,
// font=2 for big font, 0 for small font,
// inverse is true for an inverted character (black with red background)
void drawChar(uint16_t x, uint16_t y, byte n, byte font, boolean inverse) {
byte charbytes[16], fontrows, xover8 = x>>3;
int index;
if (0 != (x % 8)) return; // x not a multiple of 8
if ((n>9) && (n<32)) return; // invalid character
if (font==2) fontrows=16; else fontrows=8;
if ((n>=0) && (n<=9)) index=(n+16)*fontrows; else index=(n-32)*fontrows; // go to the right code for this character

// addressing start at buffer and add y (rows) * (WIDTH is 64 so WIDTH/8) is 8 plus (x / 8) is 0 to 7
byte *pDst = buffer + (y << 3) + xover8;
for (byte i=0;i<fontrows;i++) { // fill up the charbytes array with the right bits
if (font==0) charbytes[i] = pgm_read_byte(&(font8x8_basic[index+i]));
// if (font==1) charbytes[i] = pgm_read_byte(&(font8x8_extended[index+i]));
if (font==2) charbytes[i] = pgm_read_byte(&(font8x16_basic[index+i]));
// reverse bit order for fonts 0 and 1
if (font != 2) {
charbytes[i] = (charbytes[i] & 0xF0) >> 4 | (charbytes[i] & 0x0F) << 4;
charbytes[i] = (charbytes[i] & 0xCC) >> 2 | (charbytes[i] & 0x33) << 2;
charbytes[i] = (charbytes[i] & 0xAA) >> 1 | (charbytes[i] & 0x55) << 1;
};
if (inverse) charbytes[i] = ~charbytes[i];
};
const byte *pSrc = charbytes; // point at the first set of 8 pixels
for (byte i=0; i<fontrows; i++) {
*pDst = *pSrc; // populate the destination byte
pDst += 8; // go to next row on buffer
pSrc++; // go to next set of 8 pixels in character
}
};

void moveLeft(byte pixels, byte rowstart, byte rowstop) { // routine to move certain rows on the screen "pixels" pixels to the left
byte row, column;
short unsigned int address;
for (column=0;column<8;column++) {
for (row=rowstart;row<rowstop;row++) {
address = (row<<3) + column; /// right here!
if (column==7)
buffer[address] = buffer[address]<<pixels; // shuffle pixels left on last column and fill with a blank
else { // shuffle pixels left and add leftmost pixels from next column
byte incomingchar = buffer[address+1];
buffer[address] = buffer[address]<<pixels;
for (byte x=0;x<pixels;x++) { buffer[address] += ((incomingchar & (128>>x)) >> (7-x))<<(pixels-x-1); };
}
}
}
};

// set a single pixel on or off
void setPixel(byte x, byte y, byte colour) { bitWrite(buffer[(y<<3)+(x>>3)],7-(x&7),colour); };

void drawRect(byte x1, byte y1, byte x2, byte y2, byte colour) {
for (byte x=x1;x<=x2;x++) { setPixel(x,y1,colour); setPixel(x,y2,colour); };
for (byte y=y1;y<=y2;y++) { setPixel(x1,y,colour); setPixel(x2,y,colour); };
};

void shiftOut(byte row) { // fast routine to shove out 8 columns into two rows via board's shift registers
for(byte column=0;column<8;column++) {
byte index = column + (row<<3);
for(byte i=0;i<8;i++) {
PORTA &= ~(1 << 4);     // PORTK &= ~(1 << 4);     //PORTB &=~(3<<(data_R1-8)); // data_R2 is LOW; data_R1 is LOW;
PORTA &= ~(1 << 5);     //PORTK &= ~(1 << 5);     //PORTB &=~(1<<(clockPin-8)); // digitalWrite(clockPin,LOW);
PORTA |= ((buffer[index]>>(7-i)) & 0x01) << (4); // PORTK |= ((buffer[index]>>(7-i)) & 0x01) << (4); // top set of rows  //  PORTB |= ((buffer[index]>>(7-i)) & 0x01) << (data_R1-8); // top set of rows
//PORTB |= !((buffer[index+128]>>(7-i)) & 0x01) << (data_R2-8); // bottom set of rows

PORTA |= 1<<(5); // PORTK |= 1<<(5); // digitalWrite(clockPin,HIGH); //PORTB |= 1<<(clockPin-8); // digitalWrite(clockPin,HIGH);

};
};
};

// Scan a pair of rows on to the display from "buffer" via the interrupt
ISR(TIMER2_COMPA_vect){
cli();
digitalWrite(31, HIGH); // digitalWrite(en_74138, HIGH); // Turn off display
shiftOut(ScanRow); // Shift out 8 columns
digitalWrite(30, LOW); //digitalWrite(latchPin, LOW);
digitalWrite(30, HIGH); //digitalWrite(latchPin, HIGH);
PORTA = ScanRow | (PORTA & 0xf0); // PORTD = (ScanRow << 3) | (PORTD & 0X87); // Highlight row: pins 3 4 5 6 (la_74138 lb_74138 lc_74138 ld_74138)
digitalWrite(en_74138, LOW); // Turn on display
ScanRow++; // Do the next pair of rows next time this routine is called
if (ScanRow==16) ScanRow=0;
sei();
};

void setup() {

 

Serial.begin(115200); // opens serial port, sets data rate to 9600 bps


// Set up Timer2 as the scanning interrupt timer
cli(); // clear interrupts
TCCR2A = 0; TCCR2B = 0; TCNT2 = 0;
TCCR2B |= (1 << CS12) | (1 << CS10); // Set 1024 prescaler
// 160Hz scan rate = 10 frames/second (16 pairs of rows)
OCR2A = 97; // 97 = (16,000,000 / (1024*160)) - 1
TCCR2A |= (1 << WGM21); TIMSK2 |= (1 << OCIE2A);

pinMode(latchPin,OUTPUT); 
pinMode(clockPin,OUTPUT);
pinMode(data_R1,OUTPUT); 
//pinMode(data_R2,OUTPUT);

pinMode(en_74138,OUTPUT);
pinMode(la_74138,OUTPUT); 
pinMode(lb_74138,OUTPUT);
pinMode(lc_74138,OUTPUT); 
pinMode(ld_74138,OUTPUT);

digitalWrite(en_74138, LOW);
digitalWrite(data_R1, HIGH); 
//digitalWrite(data_R2, HIGH);
counter = millis();
sei(); //allow interrupts
};
//-------------------------------------------------------------------------------------------------------LOOP
// Note that there's no need to do anything with the screen in the main loop.
// Whatever's in "buffer" is constantly scanned out.
void loop() {

while(Serial.available()) {
inChar = Serial.read(); // Read a character
inData[index] = inChar; // Store it
index++; // Increment where to write next
}

index = 0;

Serial.write(inData[0]);

 

//---------------------------------------------------------------------------display 8 8x16 characters fixed----------2
// display fixed data - full display height use 16x64 for a line of 8 characters
// send a 2 then 8 characters
// command like curl "http://192.168.0.101/212345678" command 2 - followed by 8 16 pixel tall characters

if (inData[0] == 50) // 50 is DEC ASCII for 2 - use the 8x16 tall font

{
drawChar(0,0,inData[1],2,false);
delay(50);
drawChar(8,0,inData[2],2,false);
delay(50);
drawChar(16,0,inData[3],2,false);
delay(50);
drawChar(24,0,inData[4],2,false);
delay(50);
drawChar(32,0,inData[5],2,false);
delay(50);
drawChar(40,0,inData[6],2,false);
delay(50);
drawChar(48,0,inData[7],2,false);
delay(50);
drawChar(56,0,inData[8],2,false);
delay(50);
}

//----------------------------------------------------------------8 fixed 8x8 top half---and bottom half--------0
// display fixed data - top and bottom lines of 8 characters
// send a 0 then 16 characters to fill the display
// or scroll 8x16 characters
// command like curl "http://192.168.0.101/012345678asdfghjk>" command 0 - followed by 8 characters for top line then 8 for bottom line

if (inData[0] == 48) // 58 is DEC ASCII for 0 - use the 8x8 small font
{
drawChar(0,0,inData[1],0,false);
delay(50);
drawChar(8,0,inData[2],0,false);
delay(50);
drawChar(16,0,inData[3],0,false);
delay(50);
drawChar(24,0,inData[4],0,false);
delay(50);
drawChar(32,0,inData[5],0,false);
delay(50);
drawChar(40,0,inData[6],0,false);
delay(50);
drawChar(48,0,inData[7],0,false);
delay(50);
drawChar(56,0,inData[8],0,false);
delay(50);

// bottom line

drawChar(0,8,inData[9],0,false);
delay(50);
drawChar(8,8,inData[10],0,false);
delay(50);
drawChar(16,8,inData[11],0,false);
delay(50);
drawChar(24,8,inData[12],0,false);
delay(50);
drawChar(32,8,inData[13],0,false);
delay(50);
drawChar(40,8,inData[14],0,false);
delay(50);
drawChar(48,8,inData[15],0,false);
delay(50);
drawChar(56,8,inData[16],0,false);
delay(50);
}

//---------------------------------------------------------------------------------8 fixed 16x8------------------T
// display fixed data - 8 characters 16x8 across the screen
// send a T then 16 characters to fill the display
// command like curl "http://192.168.0.101/T12345678>" command T (for Tall!)
// - followed by 8 characters for top line then 8 for bottom line

if (inData[0] == 84) // 84 is DEC ASCII for T
{
drawChar(0,0,inData[1],2,false);
delay(50);
drawChar(8,0,inData[2],2,false);
delay(50);
drawChar(16,0,inData[3],2,false);
delay(50);
drawChar(24,0,inData[4],2,false);
delay(50);
drawChar(32,0,inData[5],2,false);
delay(50);
drawChar(40,0,inData[6],2,false);
delay(50);
drawChar(48,0,inData[7],2,false);
delay(50);
drawChar(56,0,inData[8],2,false);
delay(50);

}

//----------------------------Mixed mode-----------8 8x8 characters fixed top line + 8x8 scrolling on bottom line---M

// if the received data starts with 0 display 8 fixed characters on the top line (8x64)
// followed by a scrolling message on the bottom line
// command like curl "http://192.168.0.101/M12345678scrolling message >" command 0
//- followed by 8 characters for top line then
// the string looks like M12345678scrolling message>
// NB! then add spaces up to 100 characters to wipe earlier data
if (inData[0] == 77) // 7 is DEC ASCII for M
{
// display top line as static
drawChar(0,0,inData[1],0,false);
drawChar(8,0,inData[2],0,false);
drawChar(16,0,inData[3],0,false);
drawChar(24,0,inData[4],0,false);
drawChar(32,0,inData[5],0,false);
drawChar(40,0,inData[6],0,false);
drawChar(48,0,inData[7],0,false);
drawChar(56,0,inData[8],0,false);

// display bottom line as a scrolled message
drawChar(56,8,inData[count],0,false); // start at the bottom right 8x8
count = (count + 1);
// move the text 8 pixels to the left
for (byte i=0; i<8; i++)
{
delay(50); // scroll speed
moveLeft(1,8,16); //odd? 15 fails 16 OK
};
Serial.print("count = "); Serial.println(count); // for de-bug

// repeat the scrolling data that starts at character 9 after the data for the fixed top line
if (inData[count]==62) // character > (ascii 62) denotes end of message
{
count = 9;
}
}

//-----------------------------------------------------------------------------------------------SCROLL BIG FONT----S
if (inData[0] == 83) // 83 is DEC ASCII for S - scroll the 8x16 font
{
// display a scrolled message of 16x8 characters
// command like curl "http://192.168.0.101/SHello World! >" command S - followed by text to scroll
drawChar(56,0,inData[count],2,false); // start at the bottom right 8x8
count = (count + 1);
// move the text 8 pixels to the left
for (byte i=0; i<8; i++)
{
delay(50); // scroll speed
moveLeft(1,0,16); // collumn of pixels moves 1 pixel to left starts at 0 (top row) and finishes at 15 (bottom row)
};
Serial.print("count = "); Serial.println(count); // for de-bug

// repeat the scrolling data that starts at character 1 after the data for the fixed top line
if (inData[count]==62) // character > (ascii 62) denotes end of message
{
count = 1;
}

}

//-----------------------------------------------------------------------------------------------LOOP END
};

 
