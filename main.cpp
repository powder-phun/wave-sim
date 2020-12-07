/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL and standard IO

#include <stdio.h>
#include "Gui.h"
#include "Sim.h"

int main()
{
    Sim sim = Sim();
    Gui gui = Gui();
    Rect_object klocek(300, 170+30, 303, RES_Y, 0);

    sim.add_object(&klocek);


    if(true)
    {
        printf("Init complete\n");
        while(gui.running())
        {
            for(int i = 0; i < 25; i++)
                sim.tick();
            gui.refresh(sim);
        }
    }
}
