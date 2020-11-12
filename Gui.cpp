#include "Gui.h"

//
// Created by jan on 10.11.2020.
//
Gui::Gui():
offset(0), running_status(false)
{
    bool success = false;
    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        if (SDL_CreateWindowAndRenderer(RES_X * SCALE_FACTOR, RES_Y * SCALE_FACTOR, 0, &window, &renderer) == 0)
        {
            success = true;
        }
    }

    if(!success)
    {
        fprintf(stderr, "%s", "Failed to initialise\n");
    }
    else
    {
        running_status = true;
    }
}

void Gui::refresh(Sim& sim)
{
    max_amplitude = sim.get_max_amplitude();

    for (int x = 0; x < RES_X * SCALE_FACTOR; x++)
    {
        for (int y = 0; y < RES_Y * SCALE_FACTOR; y++)
        {
            SDL_SetRenderDrawColor(renderer, r(sim.get_amplitude(x / SCALE_FACTOR, y / SCALE_FACTOR)),
                                   g(sim.get_amplitude(x / SCALE_FACTOR, y / SCALE_FACTOR)),
                                   b(sim.get_amplitude(x / SCALE_FACTOR, y / SCALE_FACTOR)), SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    offset++;
    printf("%i %f\n", offset, max_amplitude);

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(window);
    //SDL_Delay(1000);

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            running_status = false;
            quit();
        }
    }
}

void Gui::quit()
{
    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

bool Gui::running()
{
    return running_status;
}

Uint8 Gui::r(double a)
{
    if(a < 0.0)
    {
        return -250.0*a/max_amplitude;
    }
    else
    {
        return 0;
    }
}

Uint8 Gui::g(double a)
{
    if(a > 0.0)
    {
        return 250.0*a/max_amplitude;
    }
    else
    {
        return 0;
    }
}

Uint8 Gui::b(double a)
{
    return 0;
}

