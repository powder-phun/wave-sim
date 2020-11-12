//
// Created by jan on 10.11.2020.
//

#include "Sim.h"
#include <math.h>

Sim::Sim():
c(0.5), t_step(1.0), max_amplitude(1.0)
{

    for(int x = 0; x < RES_X; x++)
    {
        for(int y = 0; y < RES_Y; y++)
        {
            a0[x][y] = 0;
            a1[x][y] = 0;
            a2[x][y] = 0;
        }
    }
}

double Sim::get_amplitude(int x, int y)
{
    return a0[x][y];
}

void Sim::tick()
{
    laplace_avg();

    double radius = 5;
    int x0 = RES_X/2;
    int y0 = RES_Y/2;
    for(int x = 0; x < RES_X; x++)
    {
        for(int y = y0 - radius; y <= y0 + radius; y++)
        {
            if((x-x0)*(x-x0)+(y-y0)*(y-y0) <= radius * radius)
            {
                a2[x][y] += 0.1*sin(t * lambda_to_omega(30.0));
            }
        }
    }

    /*x0 = RES_X/2 - 20;
    y0 = RES_Y/2;
    for(int x = x0 - radius; x <= x0 + radius; x++)
    {
        for(int y = y0 - radius; y <= y0 + radius; y++)
        {
            if((x-x0)*(x-x0)+(y-y0)*(y-y0) <= radius * radius)
            {
                a2[x][y] += 0.1*sin(3*t);
            }
        }
    }*/

    for(int x0 = BORDER; x0 < RES_X - BORDER; x0++) //velocity
    {
        for (int y0 = BORDER; y0 < RES_Y - BORDER; y0++)
        {
            a1[x0][y0] += t_step * a2[x0][y0];
        }
    }

    for(int x0 = BORDER; x0 < RES_X - BORDER; x0++) //amplitude
    {
        for (int y0 = BORDER; y0 < RES_Y - BORDER; y0++)
        {
            a0[x0][y0] += t_step * a1[x0][y0];
            //a0[x0][y0] *= (1.0 - 1.0E-2);
            if(a0[x0][y0] > max_amplitude)
            {
                max_amplitude = a0[x0][y0];
            }
        }
    }

    #ifndef walls

    for(int x0 = 0; x0 < BORDER; x0++) //left wall
    {
        for (int y0 = BORDER; y0 < RES_Y - BORDER; y0++)
        {
            a0[x0][y0] = a0[BORDER][y0];
        }
    }

    for(int x0 = RES_X - BORDER; x0 < RES_X; x0++) //right wall
    {
        for (int y0 = BORDER; y0 < RES_Y - BORDER; y0++)
        {
            a0[x0][y0] = a0[RES_X - BORDER - 1][y0];
        }
    }

    for(int x0 = 0; x0 < RES_X; x0++) //top wall
    {
        for (int y0 = 0; y0 < BORDER; y0++)
        {
            a0[x0][y0] = a0[x0][BORDER];
        }
    }

    for(int x0 = 0; x0 < RES_X; x0++) //bottom wall
    {
        for (int y0 = RES_Y - BORDER; y0 < RES_Y; y0++)
        {
            a0[x0][y0] = a0[x0][RES_Y - BORDER - 1];
        }
    }
    #endif

    y0 = RES_Y/2 - 30;

    for(int x = 0; x < RES_X/2 - 30; x++)
    {
        for(int y = y0 - radius; y <= y0 + radius; y++)
        {
            //if((x-x0)*(x-x0)+(y-y0)*(y-y0) <= radius * radius)
            {
                a0[x][y] = 0;
            }
        }
    }

    for(int x = RES_X/2 + 30; x < RES_X; x++)
    {
        for(int y = y0 - radius; y <= y0 + radius; y++)
        {
            //if((x-x0)*(x-x0)+(y-y0)*(y-y0) <= radius * radius)
            {
                a0[x][y] = 0;
            }
        }
    }

    for(int x = RES_X/2 - 10; x < RES_X/2 + 10; x++)
    {
        for(int y = y0 - radius; y <= y0 + radius; y++)
        {
            //if((x-x0)*(x-x0)+(y-y0)*(y-y0) <= radius * radius)
            {
                a0[x][y] = 0;
            }
        }
    }


    t+= t_step;
}

void Sim::laplace_avg()
{
    for(int x0 = LAP_RADIUS; x0 < RES_X - LAP_RADIUS; x0++)
    {
        for(int y0 = LAP_RADIUS; y0 < RES_Y - LAP_RADIUS; y0++)
        {
            double sum = 0.0;
            double count = 0;

            for(int x = x0 - LAP_RADIUS; x <= x0 + LAP_RADIUS; x++)
            {
                for(int y = y0 - LAP_RADIUS; y <= y0 + LAP_RADIUS; y++)
                {
                    if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= LAP_RADIUS * LAP_RADIUS) {
                        count += 1.0;
                        sum += a0[x][y];
                    }
                }
            }

            a2[x0][y0] = (sum/count-a0[x0][y0])*8/(RASTER*RASTER*LAP_RADIUS*LAP_RADIUS)*c*c;
        }
    }
}

double Sim::get_max_amplitude()
{
    return max_amplitude;
}

double Sim::f_to_lambda(double f)
{
    return c/f;
}

double Sim::f_to_omega(double f)
{
    return 2.0*M_PI*f;
}

double Sim::lambda_to_f(double lambda)
{
    return c/lambda;
}

double Sim::lambda_to_omega(double lambda)
{
    return f_to_omega(lambda_to_f(lambda));
}
