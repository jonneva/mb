#include <SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream> // for file writing
#include <fstream> // for file writing

#include "Miniboi.h"
#include "Miniboi_emu.h"
#include "Miniboi3D.h"
#include "mb88.h"



Miniboi_emu EMU;

using namespace sf;
using namespace std;
using namespace Miniboi2D;

ofstream myfile;

// START OF ARDUINO CODE

Miniboi MB;
uint8_t scrbuffer[504]; // the screen buffer
Vect2D V1(0,0);
Vect2D V2(0,10);
Transform2D TF;
Transform2D TFNeg;
Poly2D P1;
Poly2D Star;
int counter = 0;
float scale = 0.01f;


void setup() {
    float rotangle = -(float)PI/18;
    mb88 testangle; mb14 testangle2;
    myfile << "Rads from 1 degrees to 90 \n";

    for (float m = 0; m<91 ;m++) {
        rotangle =(float)PI/2 * (m/90);
        testangle2 = float2mb(rotangle);
        myfile << mb2float(testangle2) << "\n";
    }

    rotangle = (float)PI*2/360*5; // rotate by 5 degrees
    testangle = rotangle;
    myfile << "Rotation angle in radians: ";
    myfile << mb882float(testangle) << "\n";
    MB.begin(&scrbuffer[0]); // pointer to screen buffer
    TF.rotateAngleXY(float2mb(-rotangle));
    TF.setLocation(Vect2D(float2mb88(0.0f),float2mb88(0.0f)));
    P1 = Poly2D(
            Vect2D(-35, 10 ),
            Vect2D(35, 10),
            Vect2D(35, -10),
            Vect2D(-35, -10));
    Vec2DArray SP;
    Star = Poly2D(SP);
    P1.fillColor = HATCH2;P1.lineColor = 1;
    P1.scale(scale);
    P1.add(TF);
}

void loop() {
    P1.addRotation(TF);
    MB.draw_poly(P1,P1.lineColor,P1.fillColor);
    MB.delay(10);
    P1.scale(1.01f);
    MB.clear_screen();
}


// END OF ARDUINO CODE

int main()
{
    myfile.open ("debug.txt");
    myfile << "Debug file \n";
    EMU.start();
    setup();

while (emuWindow.isOpen())
    {
	loop();
	//EMU.refresh(&scrbuffer[0]); // pointer to buffer
    }
    myfile.close();
	return 0;
}

