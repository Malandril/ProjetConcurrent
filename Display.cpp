//
// Created by user on 20/10/17.
//
#include <SDL2/SDL.h>
#include <iostream>
#include <caca_conio.h>

#include "Display.h"
#include "main.h"

bool canDisplay = true;

void *display(void *arg) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        return nullptr;
    }
    vector<vector<Cell *>> &terrain = (vector<vector<Cell *>> &) (*(vector<vector<Cell *>> *) arg);

    /* Création de la fenêtre */
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    int w = 1024;
    int h = 256;
    SDL_CreateWindowAndRenderer(w, h, SDL_WINDOW_RESIZABLE, &window, &renderer);
    bool done = false;
    double xp = 2;
    double yp = xp;
    if (window) {
        while (!done) {
            SDL_Event event;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            for (int x = 0; x < terrain.size(); ++x) {
                for (int y = 0; y < terrain[0].size(); ++y) {
                    Cell *cell = terrain[x][y];
                    if (cell != nullptr) {
                        int value = cell->readValue();
                        int c = 0;
                        ThreadPart *part = cell->owner;
                        if (value == 1 || value == OBSTACLE || part != nullptr) {
                            if (part != nullptr) {
                                c = 60;

                                Uint8 i = static_cast<Uint8>(part->getPos().x + part->getPos().y);
                                SDL_SetRenderDrawColor(renderer, i, c, i, SDL_ALPHA_OPAQUE);


                            } else if (value == OBSTACLE) {
                                SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                            }
                            if (value == 1) {
                                SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
                            }
                            SDL_Rect rect = {static_cast<int>(xp * x), static_cast<int>(yp * y), static_cast<int>(xp),
                                             static_cast<int>(yp)};
                            SDL_RenderFillRect(renderer, &rect);
                        }
                    } else {
                        return nullptr;
                    }
                }
            }

            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    done = SDL_TRUE;
                }
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_KP_PLUS:
                            w *= 2;
                            h *= 2;
                            SDL_SetWindowSize(window, w, h);
                            xp *= 2;
                            yp *= 2;
                            break;
                        case SDLK_KP_MINUS:
                            w /= 2;
                            h /= 2;
                            SDL_SetWindowSize(window, w, h);
                            xp /= 2;
                            yp /= 2;
                            break;

                    }

                }

            }
        }
    } else {
        fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
    }


    SDL_Quit();
}

void waitDisplayRefresh() {
    timespec t = {0, 10000000L};
    nanosleep(&t, nullptr);
}






