#include "mbed.h"
#include "MMA8452.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "Speaker.h"

#define ASTEROID_HEIGHT 12
#define ASTEROID_WIDTH 15
#define SPRITE_MAX 15
#define EARTH_WIDTH 10
#define EARTH_HEIGHT 10
#define EXPLOSION1_WIDTH 20
#define SCREEN_MAX 125
#define SCREEN_MIN 1
#define NUM_ASTEROIDS 4
#define Q 0x808000 //OLIVE 
#define I 0x008000 //GREEN 
#define S 0xC0C0C0 //SILVER 
#define C 0x17202A //UFO GLASS 
#define D 0x797D7F //DARK GREY 
#define L 0x00FF00 //LIME 
#define P 0xFF00FF //PINK 
#define R 0xF1C40F //YELLOW 
#define O 0xF39C12 //ORANGE 
#define G 0xAAB7B8 //GREY 
#define _ 0x000000 //BLACK 
#define X 0xFFFFFF //WHITE 
#define B 0x0000FF //BLUE 
#define r 0xFF0000 //RED

int asteroid_sprite_1[ASTEROID_HEIGHT * ASTEROID_WIDTH] = {
    _,_,_,_,_,_,_,X,_,_,_,_,_,_,_,
    _,_,_,_,_,_,X,_,X,_,_,_,_,_,_,
    _,_,_,_,_,X,_,_,_,X,_,_,_,_,_,
    _,_,_,_,_,X,X,X,X,X,_,_,_,_,_,
    _,_,_,_,_,X,_,_,_,X,_,_,_,_,_,
    _,_,_,_,_,X,_,_,_,X,_,_,_,_,_,
    _,_,_,_,_,X,_,_,_,X,_,_,_,_,_,
    _,_,_,_,_,X,_,_,_,X,_,_,_,_,_,
    _,_,_,X,X,_,_,_,_,_,X,X,_,_,_,
    _,_,X,_,_,_,_,X,_,_,_,_,X,_,_,
    _,_,X,_,_,_,X,_,X,_,_,_,X,_,_,
    _,_,_,X,X,X,_,_,_,X,X,X,_,_,_,
};

int asteroid_sprite_2[ASTEROID_HEIGHT * ASTEROID_WIDTH] = {
    _,_,_,_,X,X,X,X,X,X,_,_,_,_,_,
    _,_,_,X,_,_,_,_,_,X,_,_,_,_,_,
    _,_,X,_,_,_,_,_,_,X,_,_,_,_,_,
    _,X,_,_,_,_,_,_,X,_,_,_,_,_,_,
    X,X,X,X,_,_,_,_,X,X,X,X,X,X,_,
    _,_,_,X,_,_,_,_,_,_,_,_,_,X,_,
    _,_,X,_,_,_,_,_,_,_,_,_,_,X,_,
    _,X,_,_,_,_,_,X,_,_,_,_,_,X,_,
    X,_,_,_,_,_,X,X,_,_,_,_,X,_,_,
    _,X,_,_,_,X,_,X,_,_,_,_,X,_,_,
    _,_,X,_,X,_,_,X,_,_,_,X,_,_,_,
    _,_,_,X,_,_,_,X,X,X,X,_,_,_,_
};

int asteroid_sprite_3[ASTEROID_HEIGHT * ASTEROID_WIDTH] = {
    _,_,_,_,X,X,X,X,X,X,X,X,_,_,_,
    _,_,_,X,_,_,_,_,_,_,_,_,X,_,_,
    _,_,X,_,_,_,_,_,_,_,_,_,X,_,_,
    _,X,_,_,_,_,_,_,_,_,_,_,X,_,_,
    X,X,X,X,_,_,_,_,_,_,_,_,_,X,_,
    _,_,_,X,_,_,_,_,_,_,_,_,_,X,_,
    _,_,X,_,_,_,_,_,_,_,_,_,_,X,_,
    _,_,X,X,X,_,_,X,_,_,_,_,_,X,_,
    _,_,_,_,X,_,_,X,_,_,_,_,X,_,_,
    _,_,_,_,X,_,_,X,_,_,_,_,X,_,_,
    _,_,_,_,X,_,_,X,_,_,_,X,_,_,_,
    _,_,_,_,X,X,X,X,X,X,X,_,_,_,_,
};

int asteroid_sprite_4[ASTEROID_HEIGHT * ASTEROID_WIDTH] = {
    _,_,_,_,X,X,X,X,X,X,X,X,_,_,_,
    _,_,_,X,X,X,X,X,X,X,X,X,X,_,_,
    _,_,X,X,X,X,X,X,X,X,X,X,X,_,_,
    _,X,X,X,X,X,X,X,X,_,_,X,X,_,_,
    X,X,X,X,X,X,X,X,X,_,_,X,X,X,_,
    _,_,_,X,X,X,X,X,X,X,X,X,X,X,_,
    _,_,X,X,X,X,X,X,X,X,X,X,X,X,_,
    _,X,X,X,X,X,X,X,X,X,X,X,X,X,_,
    X,X,X,X,X,X,X,X,X,X,X,X,X,X,_,
    _,X,X,X,X,X,X,X,X,X,X,X,X,_,_,
    _,_,X,X,X,X,X,X,X,X,X,X,_,_,_,
    _,_,_,X,X,X,X,X,X,X,X,_,_,_,_
};

int spaceship_earth1[EARTH_WIDTH *EARTH_HEIGHT] = {
    _,_,S,S,S,S,S,S,_,_,
    _,S,I,I,I,I,I,I,S,_,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,I,S,
    S,I,I,I,I,I,I,I,S,_,
    _,S,S,S,S,S,S,S,_,_,
};
Serial pc(USBTX,USBRX); //not used in this program
uLCD_4DGL uLCD(p9, p10, p11); // create a global uLCD object
MMA8452 acc(p28, p27, 40000);  //instantiate an acc object!


bool earthAlive = true;
int offsetx = 63;
int offsety = 63;
double factor = 52;


double x = 0, y = 0, z = 0;
PinDetect pb1(p15);

int count = 1;

//START CLASSES
class ScreenObject
{
protected:
    int xpos;
    int ypos;

public:
    virtual void update() = 0;
    virtual void draw() = 0;
    int getX() {
        return xpos;
    }

    int getY() {
        return ypos;
    }
};

class Earth : public ScreenObject
{
public:
    Earth() {
        xpos = 63;
        ypos = 63;

    }
    int oldX,oldY;
    void draw() {
        uLCD.BLIT(xpos, ypos, EARTH_WIDTH, EARTH_HEIGHT, spaceship_earth1);
    }

    void update() {
        uLCD.filled_rectangle(xpos, ypos, xpos + EARTH_WIDTH, ypos + EARTH_HEIGHT, BLACK);
        if(!acc.isXYZReady()) {
            wait(0.01);
        } else {
            acc.readXYZGravity(&x,&y,&z); //notice this is passed by reference use pointers
            if (x>1)
                x = 1;
            if (x<-1)
                x = -1;
            if (y>1)
                y = 1;
            if (y < -1)
                y = -1;
            ypos = -1 * x *factor+offsety;
            xpos = -1* y *factor+offsetx;
        }
    }
};


class AbstractAsteroid : public ScreenObject
{
protected:
    int deltaX, deltaY;
};


class ConAsteroid1 : public AbstractAsteroid
{
public:

    ConAsteroid1() {
        reset();
    }
    void reset() {
        do {
            deltaX = rand()%5 - 2;
        } while(deltaX == 0);
        do {
            deltaY = rand()%5 - 2;
        } while(deltaY == 0);
        int rannum = rand() % 4;
        //Left Edge
        if (rannum == 0) {
            xpos = 0;
            ypos = rand()%(125 - ASTEROID_HEIGHT);
        }
        //Top Edge
        if (rannum == 1) {
            xpos = rand()%(125 - ASTEROID_WIDTH);
            ypos = 0;
        }
        //Right Edge
        if (rannum == 2) {
            xpos = 125 - ASTEROID_WIDTH;
            ypos = rand()%(125 - ASTEROID_HEIGHT);
        }
        //Bottom Edge
        if (rannum == 3) {
            xpos = rand()%(125 - ASTEROID_WIDTH);
            ypos = 125 - ASTEROID_HEIGHT;
        }
    }
    void draw() {
        uLCD.BLIT(xpos, ypos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_1);
    }

    void update() {
        uLCD.filled_rectangle(xpos, ypos, xpos + ASTEROID_WIDTH, ypos + ASTEROID_HEIGHT, BLACK);
        xpos += deltaX;
        ypos += deltaY;
        if ((xpos < 0) || (ypos < 0) || (xpos > 125 - ASTEROID_WIDTH) || (ypos > 125 - ASTEROID_HEIGHT))
            reset();
    }

};

class ConAsteroid2 : public AbstractAsteroid
{
public:

    ConAsteroid2() {
        reset();
    }
    void reset() {
        do {
            deltaX = rand()%5 - 2;
        } while(deltaX == 0);
        do {
            deltaY = rand()%5 - 2;
        } while(deltaY == 0);
        int rannum = rand() % 4;
        //Left Edge
        if (rannum == 0) {
            xpos = 0;
            ypos = rand()%(125 - ASTEROID_HEIGHT);
        }
        //Top Edge
        if (rannum == 1) {
            xpos = rand()%(125 - ASTEROID_WIDTH);
            ypos = 0;
        }
        //Right Edge
        if (rannum == 2) {
            xpos = 125 - ASTEROID_WIDTH;
            ypos = rand()%(125 - ASTEROID_HEIGHT);
        }
        //Bottom Edge
        if (rannum == 3) {
            xpos = rand()%(125 - ASTEROID_WIDTH);
            ypos = 125 - ASTEROID_HEIGHT;
        }
    }
    void draw() {
        uLCD.BLIT(xpos, ypos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_2);
    }

    void update() {
        uLCD.filled_rectangle(xpos, ypos, xpos + ASTEROID_WIDTH, ypos + ASTEROID_HEIGHT, BLACK);
        xpos += deltaX;
        ypos += deltaY;
        if ((xpos < 0) || (ypos < 0) || (xpos > 125 - ASTEROID_WIDTH) || (ypos > 125 - ASTEROID_HEIGHT))
            reset();
    }

};

class ConAsteroid3 : public AbstractAsteroid
{
public:

    ConAsteroid3() {
        reset();
    }
    void reset() {
        do {
            deltaX = rand()%5 - 2;
        } while(deltaX == 0);
        do {
            deltaY = rand()%5 - 2;
        } while(deltaY == 0);
        int rannum = rand() % 4;
        //Left Edge
        if (rannum == 0) {
            xpos = 0;
            ypos = rand()%(125 - ASTEROID_HEIGHT);
        }
        //Top Edge
        if (rannum == 1) {
            xpos = rand()%(125 - ASTEROID_WIDTH);
            ypos = 0;
        }
        //Right Edge
        if (rannum == 2) {
            xpos = 125 - ASTEROID_WIDTH;
            ypos = rand()%(125 - ASTEROID_HEIGHT);
        }
        //Bottom Edge
        if (rannum == 3) {
            xpos = rand()%(125 - ASTEROID_WIDTH);
            ypos = 125 - ASTEROID_HEIGHT;
        }
    }
    void draw() {
        uLCD.BLIT(xpos, ypos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_3);
    }

    void update() {
        uLCD.filled_rectangle(xpos, ypos, xpos + ASTEROID_WIDTH, ypos + ASTEROID_HEIGHT, BLACK);
        xpos += deltaX;
        ypos += deltaY;
        if ((xpos < 0) || (ypos < 0) || (xpos > 125 - ASTEROID_WIDTH) || (ypos > 125 - ASTEROID_HEIGHT))
            reset();
    }

};

class ConAsteroid4 : public AbstractAsteroid
{
public:

    ConAsteroid4() {
        reset();
    }
    void reset() {
        do {
            deltaX = rand()%5 - 2;
        } while(deltaX == 0);
        do {
            deltaY = rand()%5 - 2;
        } while(deltaY == 0);
        int rannum = rand() % 4;
        //Left Edge
        if (rannum == 0) {
            xpos = 0;
            ypos = rand()%(125 - ASTEROID_HEIGHT);
        }
        //Top Edge
        if (rannum == 1) {
            xpos = rand()%(125 - ASTEROID_WIDTH);
            ypos = 0;
        }
        //Right Edge
        if (rannum == 2) {
            xpos = 125 - ASTEROID_WIDTH;
            ypos = rand()%(125 - ASTEROID_HEIGHT);
        }
        //Bottom Edge
        if (rannum == 3) {
            xpos = rand()%(125 - ASTEROID_WIDTH);
            ypos = 125 - ASTEROID_HEIGHT;
        }
    }
    void draw() {
        uLCD.BLIT(xpos, ypos, ASTEROID_WIDTH, ASTEROID_HEIGHT, asteroid_sprite_4);
    }

    void update() {
        uLCD.filled_rectangle(xpos, ypos, xpos + ASTEROID_WIDTH, ypos + ASTEROID_HEIGHT, BLACK);
        xpos += deltaX;
        ypos += deltaY;
        if ((xpos < 0) || (ypos < 0) || (xpos > 125 - ASTEROID_WIDTH) || (ypos > 125 - ASTEROID_HEIGHT))
            reset();
    }

};

bool overlap(ScreenObject & objectA, ScreenObject & objectB)
{
    int EX = objectA.getX();
    int EY = objectA.getY();
    int AX = objectB.getX();
    int AY = objectB.getY();

    int LCX = EX - ASTEROID_WIDTH;
    int LCY = EY - ASTEROID_HEIGHT;
    int RCX = EX + EARTH_WIDTH;
    int RCY = EY + EARTH_HEIGHT;

    if ((AX > LCX) && (AX < RCX) && (AY > LCY) && (AY < RCY)) {
        return true;
    }

    else {
        return false;
    }

}

int main()
{
    uLCD.baudrate(3000000);
    wait(0.1);

    //uLCD.display_control(PORTRAIT_R);
    set_time(0);
    pb1.mode(PullUp);
    time_t seconds = time(NULL);
    ConAsteroid1 Asteroid1;
    ConAsteroid2 Asteroid2;
    ConAsteroid3 Asteroid3;
    ConAsteroid4 Asteroid4;

    Earth theEarth;
    //set parameters -- use these and don't change
    acc.setBitDepth(MMA8452::BIT_DEPTH_12);
    acc.setDynamicRange(MMA8452::DYNAMIC_RANGE_4G);
    acc.setDataRate(MMA8452::RATE_100);

    while(seconds < 20) {
        seconds = time(NULL);


        theEarth.update();
        theEarth.draw();

        Asteroid1.update();
        Asteroid2.update();
        Asteroid3.update();
        Asteroid4.update();

        Asteroid1.draw();
        Asteroid2.draw();
        Asteroid3.draw();
        Asteroid4.draw();

        if (overlap(theEarth, Asteroid1)) {
            earthAlive = false;
            break;
        }

        if (overlap(theEarth, Asteroid2)) {
            earthAlive = false;
            break;
        }
        if (overlap(theEarth, Asteroid3)) {
            earthAlive = false;
            break;
        }
        if (overlap(theEarth, Asteroid4)) {
            earthAlive = false;
            break;
        }

    } //end infinite while loop

    if (earthAlive == true)
    {
        uLCD.cls();
        uLCD.printf("CONGRATS YOU WIN");
        }
        
        else
        {
            uLCD.cls();
            uLCD.printf("CONGRATS, YOU SUCK");
            }


} // end main