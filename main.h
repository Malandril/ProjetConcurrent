//
// Created by user on 24/10/17.
//

#ifndef PROJETCONCURRENT_MAIN_H
#define PROJETCONCURRENT_MAIN_H

#include <array>

#define OBSTACLE 7
#define MAX_X 512
#define MAX_Y 128
#define DEST_X 0
#define DEST_Y 0
#define SEED 2501
#define REPEAT 5

void wakeAllThreadParts();

extern std::array<ThreadPart *, 4> threadParts;


#endif //PROJETCONCURRENT_MAIN_H
