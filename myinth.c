#include "clib.h"

extern int KeyBuffer;

void resetHandler() {
    printString("resethandler\n");
    exit(0);
}

void keyHandler() {
    printString("keyhandler\n");
        // char key = (char) KeyBuffer;
        // int c = 0;
        // if(key == 'd') {
        //         printString("\nDELAY KEY PRESSED\n");
        //         while (c < 5000)
        //                 c++;
        //         printString("\nDELAY COMPLETE\n");

        // } else {
        //         printString("\nKEYPRESS (");
        //         printChar(key);
        //         printString(") IGNORED\n");
        // }

}

void tickHandler() {
        printString("tickhandler\n");
}