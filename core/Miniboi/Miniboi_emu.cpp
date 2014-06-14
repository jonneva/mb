#include <SFML/Graphics.hpp>
#include "Miniboi_emu.h"

using namespace std;
using namespace sf;

sf::Texture emuTexture;
sf::Sprite emuSprite;
sf::RectangleShape emuPixel0, emuPixel1;
sf::Color color0(250, 250, 250); // background
sf::Color color1(50, 50, 50); // foreground
RenderWindow emuWindow{{EMUW, EMUH}, "Miniboi Emulator"};
sf::View emuScreen(sf::FloatRect(0, 0, 84, 48));
sf::Event emuEvent;
sf::Clock myClock;

int random(int a, int b) {
    int r; //Arduino random(a,b) EXCLUDES b !!!
    if (b < 1) return 0;
    if (a < 0) return 0;
    r = rand() % (b - a) + a ;
    return  r;
}


int Miniboi_emu::pollEsc() {
emuWindow.pollEvent(emuEvent);
    if(emuEvent.type == sf::Event::Closed || Keyboard::isKeyPressed(Keyboard::Key::Escape)) {
       emuWindow.close();
       return -1;
    }
return 0;
}

int Miniboi_emu::pollA()
{
emuWindow.pollEvent(emuEvent);
    if(Keyboard::isKeyPressed(Keyboard::Key::A)) return 1;
    return 0;
}

int Miniboi_emu::pollB()
{
emuWindow.pollEvent(emuEvent);
    if(Keyboard::isKeyPressed(Keyboard::Key::S)) return 1;
    return 0;
}

int Miniboi_emu::pollC()
{
emuWindow.pollEvent(emuEvent);
    if(Keyboard::isKeyPressed(Keyboard::Key::C)) return 1;
    return 0;
}

int Miniboi_emu::pollLeft()
{
emuWindow.pollEvent(emuEvent);
    if(Keyboard::isKeyPressed(Keyboard::Key::Left)) return 1;
    return 0;
}

int Miniboi_emu::pollRight()
{
emuWindow.pollEvent(emuEvent);
    if(Keyboard::isKeyPressed(Keyboard::Key::Right)) return 1;
    return 0;
}

int Miniboi_emu::pollUp()
{
emuWindow.pollEvent(emuEvent);
    if(Keyboard::isKeyPressed(Keyboard::Key::Up)) return 1;
    return 0;
}

int Miniboi_emu::pollDown()
{
emuWindow.pollEvent(emuEvent);
    if(Keyboard::isKeyPressed(Keyboard::Key::Down)) return 1;
    return 0;
}

void Miniboi_emu::start()
{
    emuWindow.setFramerateLimit(60);
    emuPixel0.setSize(sf::Vector2f(1,1));
    emuPixel0.setFillColor(color0);
    emuPixel1.setSize(sf::Vector2f(1,1));
    emuPixel1.setFillColor(color1);
    emuScreen.zoom(EMUZ);
    emuScreen.setViewport(sf::FloatRect(0, 0, 1, 1));
    emuWindow.clear(color0);
    emuWindow.setView(emuScreen);
}

void Miniboi_emu::refresh_old(uint8_t * screen){
    emuWindow.setView(emuScreen);
    emuWindow.clear(color0);

    int x=0, y=0;
    for (uint16_t byteptr=0; byteptr < 504; byteptr++) {
        for (uint8_t bitptr=0; bitptr < 8; bitptr ++) {
            if (screen[byteptr] & (0x80 >> (bitptr&7)))
                {
                    emuPixel1.setPosition(x,y);
                    emuWindow.draw(emuPixel1);
                } else {
                    emuPixel0.setPosition(x,y);
                    emuWindow.draw(emuPixel0);
                }
            x++;
            if (x==84) { x = 0; y++;}
            }
    }

    emuWindow.display();
    pollEsc();
}

void Miniboi_emu::refresh(uint8_t * screen){
    emuWindow.setView(emuScreen);
    emuWindow.clear(color0);

    int x=0, y=0;
    for (uint16_t byteptr=0; byteptr < 504; byteptr++) {
        for (int8_t bitptr=0; bitptr < 8; bitptr ++) {
            if (screen[byteptr] & (0x80 >> (bitptr&7)))
                {
                    emuPixel1.setPosition(x,y+bitptr);
                    emuWindow.draw(emuPixel1);
                } else {
                    //emuPixel0.setPosition(x,y+bitptr);
                    //emuWindow.draw(emuPixel0);
                }
            }
        x++;
        if (x==84) { y += 8; x=0;}
    }

    emuWindow.display();
    pollEsc();
}
