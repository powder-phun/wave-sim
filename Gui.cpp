#include "Gui.h"

//
// Created by jan on 10.11.2020.
//
Gui::Gui():
offset(0), running_status(false), current_point_chosen(0), shape_placed('r'), placement_mode('b'), new_lambda(20)
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

void Gui::refresh(Sim* sim)
{
    max_amplitude = sim->get_max_amplitude();

    for (int x = 0; x < RES_X * SCALE_FACTOR; x++)
    {
        for (int y = 0; y < RES_Y * SCALE_FACTOR; y++)
        {
            SDL_SetRenderDrawColor(renderer, r(sim->get_amplitude(x / SCALE_FACTOR, y / SCALE_FACTOR)),
                                   g(sim->get_amplitude(x / SCALE_FACTOR, y / SCALE_FACTOR)),
                                   b(sim->get_amplitude(x / SCALE_FACTOR, y / SCALE_FACTOR)), SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    offset++;
    //printf("%i %f\n", offset, max_amplitude);
    std::vector <Object*> objects = sim->get_objects();
    for(int i = 0; i < objects.size(); i++)
    {
        objects[i]->display(renderer);
    }
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

        if(event.type == SDL_MOUSEBUTTONDOWN)
        {
            new_x[current_point_chosen] = event.button.x;
            new_y[current_point_chosen] = event.button.y;
            current_point_chosen++;

            if(current_point_chosen >= 2 && placement_mode == 'b')
            {
                if(shape_placed == 'r')
                {
                    if(new_x[0] > new_x[1])
                    {
                        std::swap(new_x[0], new_x[1]);
                    }

                    if(new_y[0] > new_y[1])
                    {
                        std::swap(new_y[0], new_y[1]);
                    }
                    sim->add_object(new Rect_object(new_x[0], new_y[0], new_x[1], new_y[1], 0, 0, 0, 0));
                }

                if(shape_placed == 'c')
                {
                    int r = sqrt((new_x[0]-new_x[1])*(new_x[0]-new_x[1])+(new_y[0]-new_y[1])*(new_y[0]-new_y[1]));
                    sim -> add_object(new Circle_object(new_x[0], new_y[0], r, 0, 0, 0, 0));
                }

                current_point_chosen = 0;
            }

            if(current_point_chosen >= 2 && placement_mode == 's')
            {
                if(shape_placed == 'r')
                {
                    if(new_x[0] > new_x[1])
                    {
                        std::swap(new_x[0], new_x[1]);
                    }

                    if(new_y[0] > new_y[1])
                    {
                        std::swap(new_y[0], new_y[1]);
                    }

                    sim->add_object(new Rect_object(new_x[0], new_y[0], new_x[1], new_y[1], 0.1, 1, sim->lambda_to_omega(new_lambda), 0));
                }

                if(shape_placed == 'c')
                {
                    int r = sqrt((new_x[0]-new_x[1])*(new_x[0]-new_x[1])+(new_y[0]-new_y[1])*(new_y[0]-new_y[1]));
                    sim -> add_object(new Circle_object(new_x[0], new_y[0], r, 0.1, 1, sim->lambda_to_omega(new_lambda), 0));
                }

                current_point_chosen = 0;
            }

            if(current_point_chosen >= 1 && placement_mode == 'd')
            {
                sim->remove_object_at(new_x[0], new_y[0]);
                current_point_chosen = 0;
            }

            if(current_point_chosen >= 2 && placement_mode == 'l')
            {
                new_lambda = sqrt((new_x[0]-new_x[1])*(new_x[0]-new_x[1])+(new_y[0]-new_y[1])*(new_y[0]-new_y[1]));
                printf("lambda: %d\n", new_lambda);
                current_point_chosen = 0;
            }
        }



        if(event.type == SDL_KEYDOWN)
        {
            printf("%d\n", event.key.keysym.scancode);
            if(event.key.keysym.scancode == 21)
            {
                shape_placed = 'r';
            }

            if(event.key.keysym.scancode == 6)
            {
                shape_placed = 'c';
            }

            if(event.key.keysym.scancode == 5)
            {
                placement_mode = 'b';
            }

            if(event.key.keysym.scancode == 22)
            {
                placement_mode = 's';
            }

            if(event.key.keysym.scancode == 76) //del
            {
                placement_mode = 'd';
            }

            if(event.key.keysym.scancode == 15)
            {
                placement_mode = 'l';
            }

            current_point_chosen = 0;
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
        return -a/max_amplitude*250;
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
        return a/max_amplitude*250;
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



