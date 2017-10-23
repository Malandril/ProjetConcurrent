//
// Created by user on 20/10/17.
//
#include <SDL.h>
#include <iostream>

#include "Display.h"

void display(int max_x, int max_y, int &counter, int terrain[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        return;
    }

    {
        /* Création de la fenêtre */
        SDL_Window *window = nullptr;
        SDL_Renderer *renderer = nullptr;
        SDL_CreateWindowAndRenderer(640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer);
        bool done = false;
        int w;
        int h;
        SDL_GetWindowSize(window, &w, &h);
        double xp = std::min(((double) w) / max_x, ((double) h) / max_y);
        double yp = xp;
        if (window) {
            while (!done) {
                SDL_Event event;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);
                for (int x = 0; x < max_x; ++x) {
                    for (int y = 0; y < max_y; ++y) {
                        if (terrain[x * max_y + y] == 1 || terrain[x * max_y + y] == 8) {
                            if (terrain[x * max_y + y] == 8) {
                                SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                            } else {
                                SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
                            }
                            SDL_Rect rect = {static_cast<int>(xp * x), static_cast<int>(yp * y), static_cast<int>(xp),
                                             static_cast<int>(yp)};
                            SDL_RenderFillRect(renderer, &rect);
                        }
                    }
                }
                SDL_RenderPresent(renderer);
                if (counter == 0) {
                    break;
                }
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        done = SDL_TRUE;
                    } else if (event.type == SDL_WINDOWEVENT) {
                        if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                            SDL_GetWindowSize(window, &w, &h);
                            double xp = std::min(((double) w) / max_x, ((double) h) / max_y);
                            double yp = xp;
                        }
                    }

                }
            }
        } else {
            fprintf(stderr, "Erreur de création de la fenêtre: %s\n", SDL_GetError());
        }
    }

    SDL_Quit();
}

void displayWaitRefresh() {
    timespec t = {0, 10000000L};
    nanosleep(&t, nullptr);
}

bool hasTrueDisplay() {
    return true;
}






