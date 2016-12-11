#include <stdio.h>
#include "cursor.h"

float scr_x, scr_y;
float prj_x, prj_y;

void cursor_setpos(float x, float y){
     scr_x = x;
     scr_y = y;
};

void cursor_draw() {
    for (int i=0; i<10; i++) {
        printf("%d", i); 
    }
};
