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
Poly2D P1;
int counter = 0;


void setup() {
    float rotangle = (float)PI/18;
    mb88 testangle; mb14 testangle2;
    myfile << "Rads from 1 degrees to 90 \n";

    for (float m = 0; m<91 ;m++) {
        rotangle =(float)PI/2 * (m/90);
        testangle2 = float2mb(rotangle);
        myfile << mb2float(testangle2) << "\n";
    }

    rotangle = (float)PI*2/360*10; // rotate by 5 degrees
    testangle = rotangle;
    myfile << "Rotation angle in radians: ";
    myfile << mb882float(testangle) << "\n";
    MB.begin(&scrbuffer[0]); // pointer to screen buffer
    TF.rotateAngleXY(float2mb(rotangle));
    //myfile << "Rotation angle: " << rotangle << "  \n";
    P1 = Poly2D(
            Vect2D(-10, -10),
            Vect2D(10, -10),
            Vect2D(10, 10),
            Vect2D(-10, 10));
    mb88 test1, test2,test3; float result;
    test1=103.5f;
    test2=12.7f;
    test3=10.0f;
    test3=sqrt(test3);
    result = mb882float(test3);
    test3+=1;
    myfile << "X vector turning \n";
}

void loop() {
    MB.draw_line(V1,V2,1);
    //MB.set_pixel(0,0,1);
    //MB.set_pixel(V2,1);
    float length;
    length = mb882float(V2.length());
    if (counter < 40) {
        myfile << mb882float(V2.x) << " at " << counter <<" rotations \n";
        V2.addRotation(TF);
        counter ++;
    }
    P1.addRotation(TF);
    for (int i=0; i<P1.getNumVertices()-1;i++) {
        MB.set_pixel(MB.convertFromViewXToScreenX(P1[i].x), MB.convertFromViewYToScreenY(P1[i].y),1);
        MB.draw_line(P1[i],P1[i+1],1);
    }
    MB.draw_line(P1[0],P1[P1.getNumVertices()-1],1);
    //MB.set_pixel(V2,1);
    //MB.draw_poly(P1,1,0);
    MB.delay(100);
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

