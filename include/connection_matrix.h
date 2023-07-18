#include <Arduino.h>
#include "Arduino_LED_Matrix.h"

#define OFF  0
#define GOOD 1
#define BAD  2

byte off_m[8][12] = { {0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0}};

byte good_m[8][12] = {  {0,0,0,0,0,0,0,0,0,0,1,1}, 
                        {0,0,0,0,0,0,0,0,0,1,1,0},
                        {0,0,0,0,0,0,0,0,1,1,0,0},
                        {0,0,0,0,0,0,0,1,1,0,0,0},
                        {1,1,0,0,0,0,1,1,0,0,0,0},
                        {0,1,1,0,0,1,1,0,0,0,0,0},
                        {0,0,1,1,1,1,0,0,0,0,0,0},
                        {0,0,0,1,1,0,0,0,0,0,0,0}};

byte bad_m[8][12] = {   {0,0,1,1,0,0,0,0,1,1,0,0},
                        {0,0,1,1,1,0,0,1,1,1,0,0},
                        {0,0,0,1,1,1,1,1,1,0,0,0},
                        {0,0,0,0,1,1,1,1,0,0,0,0},
                        {0,0,0,0,1,1,1,1,0,0,0,0},
                        {0,0,0,1,1,1,1,1,1,0,0,0},
                        {0,0,1,1,1,0,0,1,1,1,0,0},
                        {0,0,1,1,0,0,0,0,1,1,0,0}};

class ConnectionMatrix{

public:

    ArduinoLEDMatrix matrix;

    ConnectionMatrix(){}

    void begin(){
        matrix.begin();
    }

    void render(const int sel){
        switch (sel){
            case 0:
                matrix.renderBitmap(off_m, 8, 12);
                break;  
            case 1:
                matrix.renderBitmap(good_m, 8, 12);
                break; 
            case 2:
                matrix.renderBitmap(bad_m, 8, 12);
                break;
            default:
            matrix.renderBitmap(off_m, 8, 12);
                break;
        }
    }

    void off() {
        render(OFF);
    }    
    
    void good() {
        render(GOOD);
    }    
    
    void bad() {
        render(BAD);
    }
};