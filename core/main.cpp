#include <SFML/Graphics.hpp>
#include <stdio.h>

#include "Miniboi.h"
#include "Miniboi_emu.h"
#include "Miniboi3D.h"

Miniboi_emu EMU;

using namespace sf;
using namespace std;
//using namespace Miniboi3D;

// START OF ARDUINO CODE

Miniboi MB;
uint8_t scrbuffer[504]; // the screen buffer
Miniboi2D::Vect2D V1(0,0);


void setup() {
    MB.begin(&scrbuffer[0]); // pointer to screen buffer
}

void loop() {
    MB.set_pixel(0,0,1);
    MB.set_pixel(V1,1);
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

