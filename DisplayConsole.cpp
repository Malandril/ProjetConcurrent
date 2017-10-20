//
// Created by user on 20/10/17.
//
#include <time.h>
#include <iostream>
#include "Display.h"

using std::cout;

void display(int max_x, int max_y, int &counter, int terrain[]) {
    timespec t = {};
    t.tv_sec = 1;
    while (counter > 0) {
        for (int i = 0; i < max_y / 6; ++i) {
            for (int j = 0; j < max_x / 6; ++j) {
                int value = terrain[j*max_y+i];
                if (value == 1) {
                    cout << "\033[31m1\033[0m";
                } else if (value == 8) {
                    cout << value;
                } else {
                    cout << " ";
                }
            }
            cout << "\n";
        }
        cout << std::endl;
        nanosleep(&t, NULL);
    }
}