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



void setup() {
    float rotangle = -(float)PI/18;
    myfile << "Rads from 1 degrees to 179";
    for (float m = 1; m<180 ;m++) {
        rotangle =-(float)PI/(180-m);
        myfile << rotangle << "\n";
    }
    rotangle = -(float)PI/18;
    MB.begin(&scrbuffer[0]); // pointer to screen buffer
    TF.rotateAngleXY(rotangle); // 10 rad
    myfile << "Rotation angle: " << rotangle << "  \n";
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
}

void loop() {
    MB.draw_line(V1,V2,1);
    //MB.set_pixel(0,0,1);
    //MB.set_pixel(V2,1);
    float length;
    length = mb882float(V2.length());
    V2.addRotation(TF);
    //P1.addRotation(TF);
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

