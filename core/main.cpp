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


void setup() {
    MB.begin(&scrbuffer[0]); // pointer to screen buffer
    TF.rotateAngleXY(0.5f);
}

void loop() {
    MB.set_pixel(0,0,1);
    MB.set_pixel(V1,1);
    V2.addRotation(TF);
    MB.set_pixel(V2,1);
    MB.delay(100);
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
	EMU.refresh(&scrbuffer[0]); // pointer to buffer
    }
	return 0;
}

