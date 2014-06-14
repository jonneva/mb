#include "ferrari.h"

unsigned char ferrari_fwd[] = {
  35,19,
0x00,0x0C,0x06,0x00,0x00,
0x00,0x3F,0xFF,0x80,0x00,
0x0E,0x5E,0x4F,0x4E,0x00,
0x0F,0x92,0x0F,0x3E,0x00,
0x01,0x0C,0x1F,0x90,0x00,
0x03,0x3F,0xFF,0xD8,0x00,
0x0E,0x00,0x00,0x0E,0x00,
0x3F,0xFF,0xFF,0xFF,0x80,
0x7F,0xFF,0xFF,0xFF,0xC0,
0x7F,0xFF,0xFF,0xFF,0xC0,
0x3B,0x00,0xE0,0x1B,0x80,
0x00,0x00,0xE0,0x00,0x00,
0x7F,0xFF,0xFF,0xFF,0xC0,
0x7F,0xFF,0xFF,0xFF,0xC0,
0x7F,0xF7,0xFD,0xFF,0xC0,
0x7E,0x03,0xF8,0x0F,0xC0,
0x3F,0xAA,0xAA,0xBF,0x80,
0x2A,0xFF,0xFF,0xEA,0x80,
0x2A,0x00,0x00,0x0A,0x80
};

unsigned char ferrari_left[] = {
  35,19,
  0x00,0x30,0x18,0x00,0x00,
0x01,0xFF,0xFE,0x00,0x00,
0x72,0x79,0x3D,0x1C,0x00,
0x7C,0x48,0x3C,0xFC,0x00,
0x0C,0x30,0x7E,0x20,0x00,
0x19,0xFF,0xFF,0xB0,0x00,
0x38,0x00,0x00,0x1C,0x00,
0x7F,0xFF,0xFF,0xFF,0x00,
0xFF,0xFF,0xFF,0xFF,0x80,
0xFF,0xFF,0xFF,0xFF,0x80,
0x3B,0x00,0xE0,0x1B,0x80,
0x00,0x00,0xE0,0x00,0x00,
0xFF,0xFF,0xFF,0xFF,0x80,
0xFF,0xFF,0xFF,0xFF,0x80,
0xFF,0xF7,0xFD,0xFF,0x80,
0xFC,0x07,0xF0,0x1F,0x80,
0x7F,0x55,0x55,0x7F,0x00,
0x55,0xFF,0xFF,0xD5,0x00,
0x54,0x00,0x00,0x15,0x00
};

unsigned char ferrari_right[] = {
  35,19,
0x00,0x03,0x01,0x80,0x00,
0x00,0x0F,0xFF,0xF0,0x00,
0x07,0x17,0x93,0xC9,0xC0,
0x07,0xE4,0x83,0xC7,0xC0,
0x00,0x83,0x07,0xE6,0x00,
0x01,0xBF,0xFF,0xF3,0x00,
0x07,0x00,0x00,0x03,0x80,
0x1F,0xFF,0xFF,0xFF,0xC0,
0x3F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,
0x3B,0x00,0xE0,0x1B,0x80,
0x00,0x00,0xE0,0x00,0x00,
0x3F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xF7,0xFD,0xFF,0xE0,
0x3F,0x07,0xFC,0x07,0xE0,
0x1F,0xD5,0x55,0x5F,0xC0,
0x15,0x7F,0xFF,0xF5,0x40,
0x15,0x00,0x00,0x05,0x40
};

unsigned char ferrari_alpha[] = {
35, 19,
0x00,0x3F,0xFF,0x80,0x00,
0x0E,0x7F,0xFF,0xCE,0x00,
0x1F,0xDE,0x4F,0x7F,0x00,
0x1F,0x9E,0x0F,0x3F,0x00,
0x0F,0x0C,0x1F,0x9E,0x00,
0x0F,0x3F,0xFF,0xDE,0x00,
0x3F,0xFF,0xFF,0xFF,0x80,
0x7F,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xFF,0xE0,
0xFF,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xC0,
0x7F,0xFF,0xFF,0xFF,0xC0,
0x7F,0xFF,0xFF,0xFF,0xC0,
};

unsigned char ferrari_alpha_left[] = {
35, 19,
0x01,0xFF,0xFE,0x00,0x00,
0x73,0xFF,0xFF,0x1C,0x00,
0xFE,0x79,0x3D,0xFE,0x00,
0xFC,0x78,0x3C,0xFE,0x00,
0x7C,0x30,0x7E,0x3C,0x00,
0x39,0xFF,0xFF,0xBC,0x00,
0x7F,0xFF,0xFF,0xFF,0x00,
0xFF,0xFF,0xFF,0xFF,0x80,
0xFF,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0xC0,
0xFF,0xFF,0xFF,0xFF,0x80,
0xFF,0xFF,0xFF,0xFF,0x80,
0xFF,0xFF,0xFF,0xFF,0x80,

};

unsigned char ferrari_alpha_right[] = {
35, 19,
0x00,0x0F,0xFF,0xF0,0x00,
0x07,0x1F,0xFF,0xFD,0xC0,
0x0F,0xF7,0x93,0xCF,0xE0,
0x0F,0xE7,0x83,0xC7,0xE0,
0x07,0x83,0x07,0xE7,0xC0,
0x07,0xFF,0xFF,0xFF,0x80,
0x1F,0xFF,0xFF,0xFF,0xC0,
0x3F,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xE0,
0x7F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,
0x3F,0xFF,0xFF,0xFF,0xE0,
};

unsigned char wheels1[] = {
33,2,
0x11,0xFF,0xFF,0xC4,0x00,
0x44,0x00,0x00,0x11,0x00
};

unsigned char wheels2[] = {
33,2,
0x55,0xFF,0xFF,0xD5,0x00,
0x54,0x00,0x00,0x15,0x00
};

unsigned char wheels3[] = {
33,2,
0x45,0xFF,0xFF,0xD1,0x00,
0x10,0x00,0x00,0x04,0x00,
};

unsigned char fumes1[] = {
60,5,0x00,0x03,0x80,0x00,0x00,0x0E,0x00,0x00,
0x00,0x06,0xF0,0x00,0x00,0x7B,0x00,0x00,
0x00,0x01,0x58,0x00,0x00,0xD4,0x00,0x00,
0x00,0x14,0x87,0xFF,0xFF,0x79,0x40,0x00,
0x00,0x2B,0x54,0x00,0x01,0xD6,0xA0,0x00,
};

unsigned char fumes2[] = {
60,5,0x01,0x63,0x80,0x00,0x00,0x06,0x34,0x00,
0x02,0xD3,0xF0,0x00,0x00,0x7E,0x5A,0x00,
0x01,0xA7,0x5C,0x00,0x01,0xD7,0x2C,0x00,
0x0A,0xDD,0x17,0xFF,0xFF,0x45,0xDA,0x80,
0x1D,0xFF,0x70,0x00,0x00,0x77,0xFD,0xC0
};

unsigned char fumes3[] = {
60,5,0x0B,0x03,0xC0,0x00,0x00,0x0E,0x0D,0x00,
0x16,0x87,0xF0,0x00,0x00,0x7E,0x16,0x80,
0x0D,0x32,0xFC,0x00,0x01,0xF4,0xCB,0x00,
0x56,0xA1,0x57,0xFF,0xFF,0x50,0x76,0xA0,
0xEF,0x7B,0x50,0x00,0x00,0x5D,0xFF,0x70
};

