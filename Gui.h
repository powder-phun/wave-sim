//
// Created by jan on 10.11.2020.
//

#ifndef WAVE_SIM_GUI_H
#define WAVE_SIM_GUI_H

#include <stdio.h>
#include <SDL2/SDL.h>
#include "consts.h"
#include "Sim.h"


class Gui
{
    int offset;
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running_status;
    double max_amplitude;

    Uint8 r(double a);
    Uint8 g(double a);
    Uint8 b(double a);

    //drawing:
    int new_x[3], new_y[3];
    int current_point_chosen, new_lambda;
    char shape_placed; //r - rectangle, c - circle
    char placement_mode; //b - block, s - source, d - delete, l - set lambda


public:
    Gui();
    void refresh(Sim* sim);
    void quit();
    bool running();
};


#endif //WAVE_SIM_GUI_H
