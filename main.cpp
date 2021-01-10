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


    for(int y=10; y<200; y+=40)
    {
        //auto block = new Rect_object(200, y, 220, y+20, 0.05, 0, 0, 0);
        //sim -> add_object(block);
    }
    //sim -> add_object(new Rect_object(150, 0, 170, RES_Y, 0.05, 0, 0, 0));

    //sim -> add_object(new Circle_object(60, 60, 4, 0.1, 100, sim->lambda_to_omega(20), 0));
    //sim -> add_object(new Circle_object(300, 300, 44, 0.1, 100, sim->lambda_to_omega(20), 0));
    //sim -> add_object(new Circle_object(300, 300, 40, 0.1, 0, sim->lambda_to_omega(20), 0));

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
