#include <stdio.h>
#include "Gui.h"
#include "Sim.h"

int main()
{
    printf("Wybór trybu:\nb - wstawianie blokady\ns - wstawianie zrodla\nl - ustawianie dlugosci fali\ndel - usuwanie\n\n");
    printf("wybór kształtu:\nr - prostokat\nc - kolo\n");

    Sim* sim = new Sim();
    Gui* gui = new Gui();

    for(int y=10; y<300; y+=40)
    {
        auto block = new Rect_object(200, y, 220, y+20, 0.0, 0, 0, 0);
        block->set_color(255, 0, 255); //styling demo - more options available
        sim -> add_object(block);
    }

    sim -> add_object(new Circle_object(60, 60, 4, 0.1, 100, sim->lambda_to_omega(20), 0));

    while(gui->running())
    {
        for(int i = 0; i < 4; i++)
        {
            sim->tick();
        }

        gui->refresh(sim);
    }
}
