#include "Miniboi.h"
#include "Miniboi_emu.h"

#define swapWT(type, a, b)    \
{               \
    type _t_;   \
    _t_ = a;    \
    a = b;      \
    b = _t_;    \
}

#define P0CLIPPED   1
#define P1CLIPPED   2
#define PSWAPPED    4
#define OUTOFBOUNDS 8 // out of bounds
#define SECTOR9 8
#define SECTOR12 9
#define SECTOR36 10
#define SECTOR33 12
    /*
        __|_1_|_2_|_4_| <-xbit
        8_| 9 | 10| 12|
        16| 17| 18| 20|
        32| 33| 34| 36|_y=ymax
    */

point2DArray p2DArray;
point2DArray p2ClippedArray;
pointXYArray pXYArray;
pointXYArray pXYClipped;

// PUBLIC
char Miniboi::begin(uint8_t* buf) {
    buffer = buf;
	return 1;
} // end of begin

unsigned long Miniboi::millis() {
    return myClock.getElapsedTime().asMilliseconds();
};

void Miniboi::delay(unsigned int x) {
	unsigned long time = millis() + x;
	while(millis() < time) {
        	EMU.refresh(&buffer[0]);
	}
} // end of delay

void Miniboi::fill(uint8_t color) {
	switch(color) {
		case BLACK:
			for (int i = 0; i < 504; i++)
				buffer[i] = 0;
			break;
		case WHITE:
			for (int i = 0; i < 504; i++)
				buffer[i] = 0xFF;
			break;
        case HATCH:
			for (int i = 0; i < 504; i++)
				buffer[i] = 0xAA;
			break;
		case INVERT:
			for (int i = 0; i < 504; i++)
				buffer[i] = ~buffer[i];
			break;
	}
} // end of fill

void Miniboi::set_pixel(uint8_t x, uint8_t y, char c) {
	if (x >= 84 || y >= 48) return;
	if (c==WHITE) {buffer[(y >> 3) * 84 + x] |= (0x80 >> (y & 7)); return;}
	buffer[(y >> 3) * 84 + x] &= ~(0x80 >> (y & 7));
}// end of set_pixel

void Miniboi::set_pixel(Miniboi2D::Vect2D vect, char c) {
	int x, y;
	x = convertFromViewXToScreenX(vect.x);
    y = convertFromViewYToScreenY(vect.y);
	if (x >= 84 || y >= 48) return;
	if (c==WHITE) {buffer[(y >> 3) * 84 + x] |= (0x80 >> (y & 7)); return;}
	buffer[(y >> 3) * 84 + x] &= ~(0x80 >> (y & 7));
}// end of set_pixel

void Miniboi::draw_line(int8_t x0, int8_t y0, int8_t x1, int8_t y1, char c) {

if ((uint8_t)x0 > XMAX || (uint8_t)y0 > YMAX || (uint8_t)x1 > XMAX || (uint8_t)y1 > YMAX ) {
    if (clipLine (&x0,&y0,&x1,&y1)==0) return; // line out of bounds
}

	if (x0 == x1)
		draw_column(x0,y0,y1,c);
	else if (y0 == y1)
		draw_row(y0,x0,x1,c);
	else {
		int e;
		signed int dx,dy,j, temp;
		signed char s1,s2, xchange;
		signed int x,y;

		x = x0;
		y = y0;

		//take absolute value
		if (x1 < x0) {
			dx = x0 - x1;
			s1 = -1;
		}
		else if (x1 == x0) {
			dx = 0;
			s1 = 0;
		}
		else {
			dx = x1 - x0;
			s1 = 1;
		}

		if (y1 < y0) {
			dy = y0 - y1;
			s2 = -1;
		}
		else if (y1 == y0) {
			dy = 0;
			s2 = 0;
		}
		else {
			dy = y1 - y0;
			s2 = 1;
		}

		xchange = 0;

		if (dy>dx) {
			temp = dx;
			dx = dy;
			dy = temp;
			xchange = 1;
		}

		e = ((int)dy<<1) - dx;

		for (j=0; j<=dx; j++) {
			sp(x,y,c);

			if (e>=0) {
				if (xchange==1) x = x + s1;
				else y = y + s2;
				e = e - ((int)dx<<1);
			}
			if (xchange==1)
				y = y + s2;
			else
				x = x + s1;
			e = e + ((int)dy<<1);
		}
	}
}

void Miniboi::draw_line(Miniboi2D::Vect2D v0, Miniboi2D::Vect2D v1, char c) {
    int x0,y0,x1,y1;
    x0 = convertFromViewXToScreenX(v0.x);
    y0 = convertFromViewYToScreenY(v0.y);
    x1 = convertFromViewXToScreenX(v1.x);
    y1 = convertFromViewYToScreenY(v1.y);
    draw_line (x0,y0,x1,y1,c);
}

void Miniboi::draw_row(uint8_t y, uint16_t x0, uint16_t x1, uint8_t c) {
	uint8_t hbit;

    if (x0 > x1) {
			hbit = x0;
			x0 = x1;
			x1 = hbit;
		} // swap the xs correct way round

	if (x0 == x1)
		sp(x0,y,c);
	else {

	hbit = 0x80 >> (y&7); //find the bit to be set in that byte
                          // remainder = number & ( divisor - 1 )

    x0 = (y>>3)*84 + x0 ;         // x0 now points to first buffer byte
    x1 = (y>>3)*84 + x1 ;         // x1 now points to last buffer byte

    // Drawing loops

    if (c == WHITE) {
			while (x0 < x1)
				buffer[x0++] |= hbit;
		}
		else if (c == BLACK) {
			while (x0 < x1)
				buffer[x0++] &= ~hbit;
		}
		else if (c == INVERT) {
			while (x0 < x1)
				buffer[x0++] ^= hbit;
		}
		else if (c == HATCH) {
			while (x0 < x1)
				if ((y+x0)&1) {
                    // odd bits set
                    buffer[x0++] |= hbit;
				} else {
                    // even bits turn off
                    buffer[x0++] &= ~hbit;
				}
		}
	}
} // end of draw_row

void Miniboi::draw_column(uint8_t x, uint16_t y0, uint16_t y1, uint8_t c) {

	unsigned char topbits, bottombits, hatchbit=0;

    if (y0 > y1) {
			topbits = y0;
			y0 = y1;
			y1 = topbits;
		} // swap the ys correct way round

    if (y0 == y1)
		set_pixel(x,y0,c); // draw pixel if length = 1

    switch (c) {
    case HATCH:
        if (x&1) hatchbit = HATCH1ODD;
        else hatchbit = HATCH1EVEN;
        break;
    case HATCH2:
        if (x&1) hatchbit = HATCH2ODD;
        else hatchbit = HATCH2EVEN;
        break;
    case HATCH3:
        if (x&1) hatchbit = HATCH3ODD;
        else hatchbit = HATCH3EVEN;
        break;
    }

    topbits = 0xFF >> (y0&7); // mask for top byte
    bottombits = 0xFF << (7-(y1&7)); // mask for bottom byte

    y0 = (y0>>3)*84 + x; // y0 now points to topmost byte
    y1 = (y1>>3)*84 + x; // y1 now points to last bottom byte

    if (y0==y1) {
        if (topbits && bottombits) {
            topbits &= bottombits;
            if (topbits) bottombits = topbits;
        }
    } // same byte !!
    // Drawing loop

    if (c == WHITE) {

            buffer[y0] |= topbits; // topmost byte
            y0 += 84;           // increment, if several bytes
			while ( y0 < y1) {
				buffer[y0] = 0xFF; // its a whole byte
				y0 += 84;           // increment, if several bytes
			}
			buffer[y1] |= bottombits; // last byte
		}
		else if (c == BLACK) {

            buffer[y0] &= ~topbits; // topmost byte
            y0 += 84;           // increment, if several bytes
            while ( y0 < y1) {
				buffer[y0] = 0x00; // its a whole byte
				y0 += 84;           // increment, if several bytes
			}
			buffer[y1] &= ~bottombits; // last byte
		}
		else if (c == INVERT) {

            buffer[y0] ^= topbits; // topmost byte
            y0 += 84;           // increment, if several bytes
            while ( y0 < y1) {
				buffer[y0] ^= 0xFF; // its a whole byte
				y0 += 84;           // increment, if several bytes
			}
			buffer[y1] ^= bottombits; // last byte
		}
        else if (hatchbit) {
            buffer[y0] &= ~topbits; // switch off affected bytes
            buffer[y0] |= hatchbit & topbits; // topmost byte
            y0 += 84;           // increment, if several bytes
            while ( y0 < y1) {
				buffer[y0] = hatchbit; // its a whole byte
				y0 += 84;           // increment, if several bytes
			}
			buffer[y1] &= ~bottombits; // switch off affected bytes
			buffer[y1] |= hatchbit & bottombits; // last byte
		}
}

void Miniboi::draw_rect(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, char c, char fc) {

	if (fc != -1) {
		for (unsigned char i = x0; i < x0+w; i++)
			draw_column(i,y0,y0+h,fc);
	}
	draw_line(x0,y0,x0+w,y0,c);
	draw_line(x0,y0,x0,y0+h,c);
	draw_line(x0+w,y0,x0+w,y0+h,c);
	draw_line(x0,y0+h,x0+w,y0+h,c);
} // end of draw_rect

void Miniboi::draw_poly(uint8_t n, pointXYArray& pnts, char c, char fc){

    int16_t xmax, xmin; // extremes
    uint8_t x1, x2;
    uint8_t xminPoint = 0, xmaxPoint = 0; // extremes
    uint8_t p1, p2;
    uint8_t i;

    if (n < 3) return; // not a polygon !

    if (fc != -1) { // if fillcolor is set

    xmax = xmin = pnts[0].x; // initialize to point 0 of polygon

    // FIND EXTREMES
    for (i = 1; i < n; i++)   // iterate through points of polygon
    {
        if (pnts[i].x > xmax)   // if current point is > current xmax
        {
            xmax = pnts[i].x;   // new xmax is at current point
            xmaxPoint = i;      // store this information
        }

        if (pnts[i].x < xmin)   // if current point is < current xmin
        {
            xmin = pnts[i].x;   // new xmin is at current point
            xminPoint = i;      // store this information
        }
    }

    // ROUND THE FOUND EXTREMES TO SCANLINES
    //x1 = (uint8_t)round2Scanline(xmin);
    //x2 = (uint8_t)round2Scanline(xmax);
    // disabled because already working with integer numbers
    x1 = (xmin);
    x2 = (xmax);
    if (x1 == x2) return; // polygon is 100% vertical = DO NOT DRAW !

    /* START WALKING LEFT TO RIGHT */
    p1 = xminPoint;     // begins here
    p2 = xminPoint + 1; // towards next point
    if (p2 >= n) p2 = 0;  // if p2 > number of points, wrap to p0

    do {
        walkEdge(&edgeTable1[0], &pnts[p1], &pnts[p2]);
        // then, ready for walking the next edge
        p1 = p2;        // last right point becomes new left point
        p2 = p2 + 1;    // next point please !
        if (p2 >= n) p2 = 0; // again, wrap to p0 if needed
    } while (p1 != xmaxPoint); //... until we arrive at right


    /* START WALKING RIGHT TO LEFT */
    p1 = xmaxPoint;     // begins here
    p2 = xmaxPoint + 1; // towards next point
    if (p2 >= n) p2 = 0;  // if p2 > number of points, wrap to p0

    do {
        walkEdge(&edgeTable2[0], &pnts[p1], &pnts[p2]);
        // then, ready for walking the next edge
        p1 = p2;        // last right point becomes new left point
        p2 = p2 + 1;    // next point please !
        if (p2 >= n) p2 = 0; // again, wrap to p0 if needed
    } while (p1 != xminPoint); //... until we arrive back to left

    /* NOW... DRAW THE COLUMNS TO FILL IN ! */
    do
    {
        // x = location to draw AND index to edge table
        draw_column(x1, edgeTable1[x1], edgeTable2[x1], fc);
        x1++;
    } while (x1 < x2);

    } // end of if fill colour c != -1

    /* DRAW EDGES IF EDGE COLOR IS SET */

    if (c!=-1) {
         for (i = 0; i < n-1; i++)   // iterate through points of polygon
         {
            if ((pnts[i].x == pnts[i+1].x) || (pnts[i].y == pnts[i+1].y ))
            {
                if (pnts[i].x == 0 || pnts[i].x == XMAX || pnts[i].y == 0 || pnts[i].y == YMAX)
                {
                    continue;
                }
            }
            draw_line(pnts[i].x, pnts[i].y,pnts[i+1].x,pnts[i+1].y,c);
         }
         // draw last line to p0
         if ((pnts[0].x == pnts[n-1].x) || (pnts[0].y == pnts[n-1].y ))
            {
                if (pnts[0].x == 0 || pnts[0].x == XMAX || pnts[n-1].y == 0 || pnts[n-1].y == YMAX)
                {
                    return;
                }
            }
         draw_line(pnts[0].x, pnts[0].y,pnts[n-1].x, pnts[n-1].y,c);
    }
};

void Miniboi::draw_poly(Miniboi2D::Poly2D poly, char c, char fc){
    uint8_t numpoints = 0; int16_t debugval;
    uint8_t edgecandidate = 0;
    if (poly.getNumVertices() > 4) fc = -1; //only fill triangles & quads

    pXYArray.clear();

    // Convert to screen space

    for (char i=0; i< poly.getNumVertices(); i++) {
        pointXY p;
        // poly is an array of vect2d's in mb88 format
        p.x = convertFromViewXToScreenX(poly[i].x);
        p.y = convertFromViewYToScreenY(poly[i].y);
        pXYArray.push_back(p);
        numpoints++;
    }

    debugval = pXYArray.size();

    // Clip edges
    int8_t index, validpoints=0;
    pXYClipped.clear();
    pointXY TopRight, TopLeft, BottomRight, BottomLeft;
    TopRight.x = BottomRight.x = XMAX;
    TopLeft.x = BottomLeft.x = 0;
    TopRight.y = TopLeft.y = 0;
    BottomLeft.y = BottomRight.y = YMAX;

    for (char i=0; i<numpoints;i++) {
        pointXY P0,P1;
        char retval;
        index=i+1;
        if (index == numpoints) index=0; // loop back to start
        P0 = pXYArray[i];P1=pXYArray[index];
        retval = clipLine(&P0,&P1);
        // if retval is set, something has changed
        /*
        __|_1_|_2_|_4_| <-xbit
        8_| 9 | 10| 12|
        16| 17| 18| 20|
        32| 33| 34| 36|_y=ymax
        */
        if (retval & OUTOFBOUNDS) {
            switch (retval) {
            case SECTOR9:
                edgecandidate = 9;
                break;
            case SECTOR12:
                edgecandidate = 12;
                break;
            case SECTOR36:
                edgecandidate = 36;
                break;
            case SECTOR33:
                edgecandidate = 33;
                break;
            default:
                edgecandidate = 0; // something weird must have happened
            }
            continue; // jump over points, do not store anything yet
        }
        if (retval) {
                if (retval & P0CLIPPED) {
                    // if P0 was clipped, push in clipped p0
                    // check if there is a corner candidate waiting
                    // to be pushed in first
                    // this assumes polygon is defined clockwise
                    if (edgecandidate) {
                        if (P0.x == XMAX && edgecandidate == 12) {
                           pXYClipped.push_back(TopRight);
                           validpoints++;
                        }
                        if (P0.x == 0 && edgecandidate == 33) {
                           pXYClipped.push_back(BottomLeft);
                           validpoints++;
                        }
                        if (P0.y == YMAX && edgecandidate == 36) {
                           pXYClipped.push_back(BottomRight);
                           validpoints++;
                        }
                        if (P0.y == 0 && edgecandidate == 9) {
                           pXYClipped.push_back(TopLeft);
                           validpoints++;
                        }
                        edgecandidate = 0;
                    }
                    pXYClipped.push_back(P0);
                    validpoints++;
                    }
                else {
                    // else push in the original point
                    pXYClipped.push_back(pXYArray[i]);
                    validpoints++;
                    }
                if (retval & P1CLIPPED) {
                    // if P1 was clipped also push in clipped P1
                    pXYClipped.push_back(P1);
                    validpoints++;
                    }

        } else {
        // retval was returned zero, no clipping or swapping occurred
        // push in point 0, we know its good
        pXYClipped.push_back(pXYArray[i]);
        validpoints ++;
        }
    }
    debugval = pXYClipped.size();
    if (validpoints <3) return; //its no longer a polygon !
    if (edgecandidate) {
        // there is still one corner point remaining !
        if (pXYClipped[0].x == XMAX && edgecandidate == 12) {
            pXYClipped.push_back(TopRight);
            validpoints++;
        }
        if (pXYClipped[0].x == 0 && edgecandidate == 33) {
            pXYClipped.push_back(BottomLeft);
            validpoints++;
        }
        if (pXYClipped[0].y == YMAX && edgecandidate == 36) {
            pXYClipped.push_back(BottomRight);
            validpoints++;
        }
        if (pXYClipped[0].y == 0 && edgecandidate == 9) {
            pXYClipped.push_back(TopLeft);
            validpoints++;
        }
    }
    draw_poly(validpoints,pXYClipped,c,fc); //draw the polygon
}

// PRIVATE

void Miniboi::sp(uint8_t x, uint8_t y, char c) {
    if (c==WHITE) {buffer[(y >> 3) * 84 + x] |= (0x80 >> (y & 7)); return;}
    if (c==HATCH) {
        if ((x+y) & 1) {
                // odd pixels on
                buffer[(y >> 3) * 84 + x] |= (0x80 >> (y & 7));
                } else {
                // even pixels off
                buffer[(y >> 3) * 84 + x] &= ~(0x80 >> (y & 7));
                }
        return;
        }
	buffer[(y >> 3) * 84 + x] &= ~(0x80 >> (y & 7));
}

// find the closest scanline, include top&left, discard bottom&right
// return an integer value
int Miniboi::round2Scanline (mb14 n) {
    if (mbFract(n) == mbHalf) n++;
    return mb2int(n + mbHalf);
};

int Miniboi::round2Scanline (mb88 n) {
    if (mb88fract(n) == mb88Half) n+=1;
    return mb882int(n + mbHalf);
};

// walk edge horizontally, storing edge y's along the way
void Miniboi::walkEdge(uint8_t *edgeTable, pointXY *p1, pointXY *p2)
{
    mb14 y, dy;
    int dx, x1, x2,temp;

    // make sure p1 of edge is leftmost
    if (p2->x < p1->x) swapWT(pointXY *, p1, p2);

    //x1 = round2Scanline(p1->x);
    //x2 = round2Scanline(p2->x);
    x1 = (p1->x);
    x2 = (p2->x);
    dx = x2 - x1;

    if (dx == 0) return; // avoid divide by zero

    // top left included, bottom right excluded
    dx = (((2) > (dx-1)) ? (2) : (dx-1));

    y = int2mb(p1->y);  // starting y for walk
    dy = mbDiv(int2mb((p2->y - p1->y)), int2mb(dx)); // y increment for walk

    do {
        temp = mb2int(y);
        if (temp < 0) temp = 0; //check bounds
        else if (temp > YMAX) temp =YMAX; //check bounds
        edgeTable[x1] = mb2int(y);   // store current edge y at index x in table
        y += dy;        // increment y by defined step
        x1++;           // step rightward
    } while(x1 < x2);  // until rightmost point of edge is reached
}

// clip line to window boundaries
char Miniboi::clipLine(int8_t *x0, int8_t *y0, int8_t *x1, int8_t *y1)
{
    // Check X bounds
	if (*x1<*x0) {
        std::swap (*x1,*x0); // swap so that we dont have to check x1 also
        std::swap (*y1,*y0); // y needs to be swaaped also
	}

	if (*x0>XMAX) return 0; // whole line is out of bounds

	// Clip against X0 = 0
	if (*x0 < 0) {
        if ( *x1 < 0) return 0; // nothing visible
        int dx = (*x1 - *x0);
        int dy = ((*y1 - *y0) << 8); // 8.8 fixed point calculation trick
        int m = dy/dx;
        *y0 = *y0 + ((m*-*x0)>>8); // get y0 at boundary
        *x0 = 0;
	}

	// Clip against x1 = 83
	if (*x1 > XMAX) {
        int dx = (*x1 - *x0);
        int dy = ((*y1 - *y0) << 8); // 8.8 fixed point calculation trick
        int m = dy/dx;
        *y1 = *y1 + ((m*(*x1-XMAX))>>8); // get y0 at boundary
        *x1 = XMAX;
	}

    // Check Y bounds
	if (*y1<*y0) {
        std::swap (*x1,*x0); // swap so that we dont have to check x1 also
        std::swap (*y1,*y0); // y needs to be swaaped also
	}

	if (*y0>YMAX) return 0; // whole line is out of bounds

    if (*y0 < 0) {
        if ( *y1 < 0) return 0; // nothing visible
        int dx = (*x1 - *x0) << 8;
        int dy = (*y1 - *y0); // 8.8 fixed point calculation trick
        int m = dx/dy;
        *x0 = *x0 + ((m*-*y0)>>8); // get x0 at boundary
        *y0 = 0;
	}

    // Clip against x1 = 83
	if (*y1 > YMAX) {
        int dx = (*x1 - *x0) << 8;
        int dy = (*y1 - *y0); // 8.8 fixed point calculation trick
        int m = dx/dy;
        *x1 = *x1 + ((m*(YMAX-*y1))>>8); // get y0 at boundary
        *y1 = YMAX;
	}
	return 1; // clipped succesfully
}

char Miniboi::clipLine(pointXY *p0, pointXY *p1)
{
    // clips int16_t values against window boundaries
    // returns 0 if points are within bounds
    // bit 1 (1) set if p1 was clipped
    // bit 2 (2) set if p2 was clipped
    // bit 3 (4) set if points were swapped
    // bit 4 (8) set if whole line is out of bounds

    char clipval = 0; // default to out of bounds

    // Check X bounds

    //if (p0->x >= 0 && p0->x <= XMAX && p0->y >= 0 && p0->y <= YMAX && p1->x >= 0 && p1->x <= XMAX && p1->y >= 0 && p1->y <= YMAX)
    //return clipval; // is within window, no need to clip

    /** Find the sector where the edge lies */
    uint8_t p0sector=9, p1sector =9;
    if (p0->x > 0) p0sector = 10;
    if (p1->x > 0) p1sector = 10;
    if (p0->x > XMAX) p0sector = 12;
    if (p1->x > XMAX) p1sector = 12;
    if (p0->y > 0) p0sector += 8;
    if (p1->y > 0) p1sector += 8;
    if (p0->y > YMAX) p0sector += 16;
    if (p1->y > YMAX) p1sector += 16;

    /** check if both within window (sector 18) */
    if (p0sector == 18 && p1sector == 18) return 0; // no need to clip

    /** check if both are in same non-corner sector */
    if (p0sector == p1sector) {
        if (p0sector == 10 || p0sector == 20 || p0sector == 34 || p0sector == 17)
        return 0; // both invisible and non-corner sector, no need to store
    }

    /** check if neither one is in screen */
    /** then check for a corner candidate */
    /** also check if midpoint is on screen */
    /*
        __|_1_|_2_|_4_| <-xbit
        8_| 9 | 10| 12|
        16| 17| 18| 20|
        32| 33| 34| 36|_y=ymax
    */
    if (!(p0sector == 18) && !(p1sector == 18) ) {
        uint8_t pSum = p0sector | p1sector;
        /* check if the vector goes through the screen */
        if (pSum )

        if (pSum & 2)
            {
            /* its somewhere on the right side of the screen */
            if (pSum & 16) return SECTOR30; // its down & right
            else return SECTOR6; // its up & right
            }
        else {
            /* its somewhere on the left side of the screen */
            if (pSum & 16) return SECTOR28; // its down & left
            else return SECTOR4; // its up & left
        }
    }

    /** We know now its a clippable edge, with */

	if (p1->x < p0->x) {
        std::swap (*p1,*p0); // swap so that we dont have to check x1 also
        clipval ^= PSWAPPED; // points have been swapped
	}

	if (p0->x>XMAX) {
        if (p1->y>YMAX) return OUTOFBOUNDSYMAX;
        return OUTOFBOUNDSXMAX; // whole line is out of bounds
    }


    // calculate gradient
    mb14 dx = mbSub(int2mb(p1->x),int2mb(p0->x));
    mb14 dy = mbSub(int2mb(p1->y),int2mb(p0->y));
    mb14 m = mbDiv(dy,dx);

	// Clip against X0 = 0
	if (p0->x < 0) {
        if (p1->x < 0) return OUTOFBOUNDSX0; // nothing visible

        p0->y = mbAdd(p0->y,mbMul(m,-p0->x)); // get y0 at boundary
        p0->x = 0;
        if (!(clipval & PSWAPPED)) clipval |= P0CLIPPED;
        else clipval |= P1CLIPPED;
	}

	// Clip against x1 = 83
	if (p1->x > XMAX) {
        p1->y = mbAdd(p1->y,mbMul(m,mbSub(XMAX,p1->x)));
        p1->x = XMAX;
        if (!(clipval & PSWAPPED)) clipval |= P1CLIPPED;
        else clipval |= P0CLIPPED;
	}

    // Check Y bounds
	if (p1->y<p0->y) {
        std::swap (*p1,*p0); // swap so that we dont have to check x1 also
        clipval ^= PSWAPPED;
	}

	if (p0->y>YMAX) return OUTOFBOUNDSYMAX; // whole line is out of bounds
    if (p1->x>XMAX) return OUTOFBOUNDSXMAX;

    dx = mbSub(int2mb(p1->x),int2mb(p0->x));
    dy = mbSub(int2mb(p1->y),int2mb(p0->y));
    m = mbDiv(dx,dy);

    if (p0->y < 0) {
        if ( p1->y < 0) return OUTOFBOUNDSY0; // nothing visible
        if (p0->x < 0) return OUTOFBOUNDSX0;
        p0->x = mbAdd(p0->x,mbMul(m,-p0->y)); // get y0 at boundary
        p0->y = 0;
        if (!(clipval & PSWAPPED)) clipval |= P0CLIPPED;
        else clipval |= P1CLIPPED;
	}

    // Clip against y1 = 47
	if (p1->y > YMAX) {
        p1->x = mbAdd(p1->x,mbMul(m,mbSub(YMAX,p1->y)));
        p1->y = YMAX;
        if (!(clipval & PSWAPPED)) clipval |= P1CLIPPED;
        else clipval |= P0CLIPPED;
	}

	if (clipval & PSWAPPED) {
        // for clarity's sake, swap them back in order
        std::swap (*p1,*p0);
	}
	return clipval; // clipped succesfully
}

// clip polygon to window boundaries
char Miniboi::clipPoly(char n, point2DArray& pnts)
{
    // clipPoly clips polygon edges with window edges
    for (char i = 0; i<n; i++) {

    }
    return 0;
}
