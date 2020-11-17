/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO

#include <stdio.h>
#include "Gui.h"
#include "Sim.h"

int main()
{
    static Sim sim;
    static Gui gui;


    if(true)
    {
        printf("Init complete\n");
        while(gui.running())
        {
            for(int i = 0; i < 10; i++)
                sim.tick();
            gui.refresh(sim);
        }
    }
}
