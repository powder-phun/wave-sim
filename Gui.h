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
    Uint8 r(double a);
    Uint8 g(double a);
    Uint8 b(double a);
    double max_amplitude;
public:
    Gui();
    bool init();
    void refresh(Sim& sim);
    void quit();
    bool running();



};


#endif //WAVE_SIM_GUI_H
