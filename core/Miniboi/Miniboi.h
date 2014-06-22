#ifndef MINIBOI_H
#define MINIBOI_H

#include <stdint.h>
#include <stdlib.h>
#include "Miniboi_math.h" // needed for polygon filling
#include "Vect2D.h"
#include "Poly2D.h"
#include "Transform2D.h"

//#define MINIBOI_TRIG // if fixed point trigonometric needed

#define WHITE   1
#define BLACK   0
#define HATCH   2
#define HATCH2  3
#define HATCH3  4
#define INVERT  5

#define HATCH1EVEN  0xAA
#define HATCH1ODD   0x55
#define HATCH2EVEN  0x88
#define HATCH2ODD   0x22
#define HATCH3EVEN  0xDD
#define HATCH3ODD   0x77

#define XMAX    83
#define YMAX    47
#define XMAX2D  1359872 // 83<<14
#define YMAX2D  770048  // 47<<14


typedef struct
{
    int16_t x, y; // screen xy coordinates, 8 bits is not enough
} pointXY;

typedef struct
{
    mb14 x, y;
} point2D;

typedef std::vector<point2D> point2DArray;
typedef std::vector<pointXY> pointXYArray; // display clipped xy's

#define clear_screen()	fill(0)

using namespace Miniboi2D;

class Miniboi {
public:
    uint8_t* buffer; // Teh screen buffer
    uint8_t edgeTable1[84]; // edge tables for polygon filling
    uint8_t edgeTable2[84]; // ToDo: make optional if not needed
	char begin(uint8_t*);
	void fill(uint8_t);
	unsigned long millis();
	void delay(unsigned int);
	void set_pixel(uint8_t,uint8_t,char);
    void set_pixel(Vect2D,char);
	void draw_line(int8_t,int8_t, int8_t, int8_t, char);
	void draw_line(Vect2D,Vect2D,char);
    void draw_column(uint8_t, uint16_t, uint16_t, uint8_t);
    void draw_row(uint8_t, uint16_t, uint16_t, uint8_t);
    void draw_rect(uint8_t,uint8_t,uint8_t,uint8_t,char,char);
    void draw_poly(uint8_t, pointXYArray&, char, char);
    void draw_poly(Poly2D, char, char);
    int convertFromViewXToScreenX(mb88 x) { return (int) (round2Scanline(x)<<1) + XMAX/2; } // permanent zoom x 2 due to 8.8 range limitations
    int convertFromViewYToScreenY(mb88 y) { return (int) (round2Scanline(-y)<<1) + YMAX/2; } // was round2Scanline(-y) + YMAX/2;

private:
    void sp(uint8_t,uint8_t,char);
    int  round2Scanline(mb14);
    int  round2Scanline(mb88);
    void walkEdge(uint8_t *, pointXY *, pointXY *);
    char clipLine(int8_t*, int8_t*, int8_t*, int8_t*);
    char clipLine(pointXY*, pointXY*);
    char clipPoly(char, point2DArray&);
};

extern Miniboi MB; // until i get rid of Quokka 3d heritage

#endif
