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
#define OUTOFBOUNDS 8

point2DArray p2DArray;
point2DArray p2ClippedArray;
pointXYArray pXYArray;

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

void Miniboi::draw_poly(uint8_t n, point2DArray& pnts, char c, char fc){

    mb14 xmax, xmin; // extremes in 18.14 fixed point accuracy
    int temp;
    float t2;
    uint8_t x1, x2;
    uint8_t xminPoint = 0, xmaxPoint = 0; // extremes
    uint8_t p1, p2;
    uint8_t i;

    if (n < 3) return; // not a polygon !

    if (fc != -1) { // if fillcolor is set

    xmax = xmin = pnts[0].x; // initialize to point 0 of polygon
    temp = mb2int(xmax);
    t2 = mb2float(xmax);

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
    x1 = (uint8_t)round2Scanline(xmin);
    x2 = (uint8_t)round2Scanline(xmax);

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
            draw_line(round2Scanline(pnts[i].x), round2Scanline(pnts[i].y),round2Scanline(pnts[i+1].x), round2Scanline(pnts[i+1].y),c);
         }
         // draw last line to p0
         draw_line(round2Scanline(pnts[0].x), round2Scanline(pnts[0].y),round2Scanline(pnts[n-1].x), round2Scanline(pnts[n-1].y),c);
    }
};

void Miniboi::draw_poly(Miniboi2D::Poly2D poly, char c, char fc){
    uint8_t numpoints = 0;
    if (poly.getNumVertices() > 4) fc = -1; //only fill triangles & quads
    //p2DArray.clear();
    // this needs clipping !!!
    // clipping strategy...
    // convert all points to screen coords
    // initialize vertex number counter = 0
    // loop thru edges:
    //  store old endpoints
    //  send edge to clipedge function
    //  see what the return is and copy to new array accordingly
    //  do not copy invalid points
    // send clipped pointarray to drawpoly

    // CLIP BEFORE THIS//
    p2DArray.resize(poly.getNumVertices());

    // Convert to screen space

    for (char i=0; i< poly.getNumVertices(); i++) {
        point2D p;
        p.x = int2mb(convertFromViewXToScreenX(poly[i].x));
        p.y = int2mb(convertFromViewYToScreenY(poly[i].y));
        p2DArray[i]=p;
        numpoints++;
    }

    // Clip edges
    int8_t index, validpoints=0;
    p2ClippedArray.clear();
    p2ClippedArray.resize(numpoints+5);
    for (char i=0; i<numpoints;i++) {
        point2D P0,P1;
        char retval;
        index=i+1;
        if (index == numpoints) index=0; // loop back to start
        P0 = p2DArray[i];P1=p2DArray[index];
        retval = clipLine(&P0,&P1);
        // if retval is set, something has changed
        if (retval & OUTOFBOUNDS) continue; // jump over points
        if (retval) {
            if (!(retval & PSWAPPED)) {
                // if points were not swapped
                if (retval & P0CLIPPED) {
                    // if P0 was clipped, push in clipped p0
                    p2ClippedArray.push_back(P0);
                    validpoints++;
                    }
                else {
                    // else push in the original point
                    p2ClippedArray.push_back(p2DArray[i]);
                    validpoints++;
                    }
                if (retval & P1CLIPPED) {
                    // if P1 was clipped push in clipped P1
                    p2ClippedArray.push_back(P1);
                    validpoints++;
                    }
                else {
                    // else push in original P1
                    p2ClippedArray[i] = p2DArray[index];
                    validpoints++;
                    }
            } else {
                // points were swapped in the clip function
                // so correct order must be restored
                if (retval & P0CLIPPED) {
                    // if P0 was clipped it means P1 was clipped
                    // so push in P1 first !!
                    p2ClippedArray.push_back(P1);
                    validpoints++;
                    }
                else {
                    // P0 was NOT clipped, push in original P0
                    // remember, P2Darray is still in the original order !
                    p2ClippedArray.push_back(p2DArray[i]);
                    validpoints++;
                    }

                if (retval & P1CLIPPED) {
                    // P1 was clipped and swapped, push in P0
                    p2ClippedArray.push_back(P0);
                    validpoints++;
                    }
                else {
                    // P1 was not clipped push original P1
                    p2ClippedArray.push_back(p2DArray[index]);
                    validpoints++;
                }
            }
        } else {
        // retval was returned zero, no clipping or swapping occurred
        // push in originals
        p2ClippedArray.push_back(p2DArray[i]);
        p2ClippedArray.push_back(p2DArray[index]);
        validpoints +=2;
        }
    }
    if (validpoints <3) return; //its no longer a polygon !
    draw_poly(validpoints,p2DArray,c,fc); //draw the polygon
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

// walk edge horizontally, storing edge y's along the way
void Miniboi::walkEdge(uint8_t *edgeTable, point2D *p1, point2D *p2)
{
    mb14 y, dy;
    int dx, x1, x2,temp;

    // make sure p1 of edge is leftmost
    if (p2->x < p1->x) swapWT(point2D *, p1, p2);

    x1 = round2Scanline(p1->x);
    x2 = round2Scanline(p2->x);
    dx = x2 - x1;

    if (dx == 0) return; // avoid divide by zero

    // top left included, bottom right excluded
    dx = (((2) > (dx-1)) ? (2) : (dx-1));

    y = p1->y;  // starting y for walk
    dy = mbDiv((p2->y - p1->y), int2mb(dx)); // y increment for walk

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

char Miniboi::clipLine(point2D *p0, point2D *p1)
{
    // Clipline (point2D,point2D, newpoint, newpoint)
    // clips mb14 values against window boundaries
    // returns 0 if points are within bounds
    // bit 1 (1) set if p1 was clipped
    // bit 2 (2) set if p2 was clipped
    // bit 3 (4) set if points were swapped
    // bit 4 (8) set if whole line is out of bounds

    char clipval = 0; // default to out of bounds

    // Check X bounds

    if (p0->x >= 0 && p0->x <= XMAX2D && p0->y >= 0 && p0->y <= YMAX2D && p1->x >= 0 && p1->x <= XMAX2D && p1->y >= 0 && p1->y <= YMAX2D)
    return clipval; // is within window, no need to clip

	if (p1->x < p0->x) {
        std::swap (*p1,*p0); // swap so that we dont have to check x1 also
        clipval ^= PSWAPPED; // points have been swapped
	}

	if (p0->x>XMAX2D) return OUTOFBOUNDS; // whole line is out of bounds

    // calculate gradient
    mb14 dx = mbSub(p1->x,p0->x);
    mb14 dy = mbSub(p1->y,p0->y);
    mb14 m = mbDiv(dy,dx);

	// Clip against X0 = 0
	if (p0->x < 0) {
        if ( p1->x < 0) return OUTOFBOUNDS; // nothing visible
        p0->y = mbAdd(p0->y,mbMul(m,-p0->x)); // get y0 at boundary
        p0->x = 0;
        clipval |= P0CLIPPED;
	}

	// Clip against x1 = 83
	if (p1->x > XMAX2D) {
        p1->y = mbAdd(p1->y,mbMul(m,mbSub(p1->x,XMAX2D)));
        p1->x = XMAX2D;
        clipval |= P1CLIPPED;
	}

    // Check Y bounds
	if (p1->y<p0->y) {
        std::swap (*p1,*p0); // swap so that we dont have to check x1 also
        clipval ^= PSWAPPED;
	}

	if (p0->y>YMAX2D) return OUTOFBOUNDS; // whole line is out of bounds

    dx = mbSub(p1->x,p0->x);
    dy = mbSub(p1->y,p0->y);
    m = mbDiv(dx,dy);

    if (p0->y < 0) {
        if ( p1->y < 0) return OUTOFBOUNDS; // nothing visible
        p0->x = mbAdd(p0->x,mbMul(m,-p0->y)); // get y0 at boundary
        p0->y = 0;
        clipval |= P0CLIPPED;
	}

    // Clip against y1 = 47
	if (p1->y > YMAX2D) {
        p1->y = YMAX2D;
        clipval |= P1CLIPPED;
	}
	return clipval; // clipped succesfully
}

// clip polygon to window boundaries
char Miniboi::clipPoly(char n, point2DArray& pnts)
{
    // clipPoly clips polygon edges with window edges
    for (char i = 0; i<n; i++) {

    }
}
