#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "Miniboi.h"
#include "Miniboi_emu.h"
#include "Miniboi3D.h"

Miniboi_emu EMU;

using namespace sf;
using namespace std;
using namespace Miniboi2D;

// START OF ARDUINO CODE

Miniboi MB;
uint8_t scrbuffer[504]; // the screen buffer
Vect2D V1(0,0);
Vect2D V2(0,10);
Transform2D TF;
Poly2D P1;



void setup() {
    MB.begin(&scrbuffer[0]); // pointer to screen buffer
    TF.rotateAngleXY(0.1f);
    P1 = Poly2D(
            Vect2D(-21, -21),
            Vect2D(21, -21),
            Vect2D(21, 21),
            Vect2D(-21, 21));
}

void loop() {
    MB.draw_line(-10,10,100,30,1);
    MB.set_pixel(0,0,1);
    MB.set_pixel(V1,1);
    V2.addRotation(TF);
    P1.addRotation(TF);
    for (int i=0; i<P1.getNumVertices()-1;i++) {
        //MB.set_pixel(MB.convertFromViewXToScreenX(P1[i].x), MB.convertFromViewYToScreenY(P1[i].y),1);
        MB.draw_line(P1[i],P1[i+1],1);
    }
    MB.draw_line(P1[0],P1[P1.getNumVertices()-1],1);
    //MB.set_pixel(V2,1);
    MB.delay(1);
    MB.clear_screen();

}


// END OF ARDUINO CODE

int main()
{
    EMU.start();
    setup();

while (emuWindow.isOpen())
    {
	loop();
	//EMU.refresh(&scrbuffer[0]); // pointer to buffer
    }
	return 0;
}

