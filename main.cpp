/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO

#include <stdio.h>
#include "Gui.h"
#include "Sim.h"

int main()
{
    Sim* sim = new Sim();
    Gui* gui = new Gui();


    sim->add_object(new Rect_object(300, 170+30, 303, RES_Y, 0, 0, 0, 0));
    sim->add_object(new Rect_object(300, 130, 303, 170, 0.05, 0, 0, 0));
    sim->add_object(new Rect_object(300, 0, 303, 130-30, 0, 0, 0, 0));

    auto soczewka = new Circle_object(120, 120, 20, 0.07, 0, 0, 0);
    soczewka->set_visible(false);
    sim->add_object(soczewka);

    auto source_1 = new Circle_object(100, 200, 5, 0.1, 100, sim->lambda_to_omega(30), 0);
    source_1->set_visible(false);
    sim->add_object(source_1);

    auto source_2 = new Circle_object(100, 100, 5, 0.1, 100, sim->lambda_to_omega(30), 3.1415);
    source_2->set_visible(false);
    sim->add_object(source_2);

    auto rozowy = new Rect_object(70, 70, 80, 90, 0, 0, 0, 0);
    rozowy->set_color(255, 0, 255);
    sim->add_object(rozowy);

    if(true)
    {
        printf("Init complete\n");
        while(gui->running())
        {
            for(int i = 0; i < 25; i++)
                sim->tick();
            gui->refresh(sim);
        }
    }
}
