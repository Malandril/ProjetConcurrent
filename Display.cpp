//
// Created by user on 20/10/17.
//
#include <SDL.h>
#include <iostream>

#include "Display.h"

void display(int max_x, int max_y, int &counter, int terrain[]) {
    std::cout << terrain[0];
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
                SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, 0, 0, static_cast<int>(max_x * xp), static_cast<int>(max_y * yp));
                SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, 0, 0, max_x, max_y);
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
                for (int x = 0; x < max_x; ++x) {
                    for (int y = 0; y < max_y; ++y) {
                        if (terrain[x * max_y + y] == 1) {
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

template<size_t size_x, size_t size_y>
void lol(int (&terrain)[size_x][size_y]) {
    printf("loiloool");
}






