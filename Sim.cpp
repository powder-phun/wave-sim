//
// Created by jan on 10.11.2020.
//

#include "Sim.h"


Sim::Sim():
c(0.1), t_step(1.0), max_amplitude(0), max0(0), max1(0)
{
    for(int x = 0; x < RES_X; x++)
    {
        for(int y = 0; y < RES_Y; y++)
        {
            a0[x][y] = 0;
            a1[x][y] = 0;
            a2[x][y] = 0;
            E[x][y] = 0.0;
            local_c[x][y] = c;
            force_omega[x][y] = 0;
            force_amplitude[x][y] = 0;
            force_phase[x][y] = 0;

        }
    }
    for(int y = 145; y<= 155; y++)
    {
        for(int x = 140; x <= 170; x++)
        {
            force_omega[x][y] = lambda_to_omega(40);
            force_amplitude[x][y] = 100.0;
            //force_phase[x][y] = (80.0-x)/40.0*2.0*M_PI;
        }
    }

    objects.push_back(new Rect_object(300, 130, 303, 170, 0));
    objects.push_back(new Rect_object(300, 0, 303, 130-30, 0));

    for(int i = 0; i < objects.size(); i++)
    {
        //objects[i]->impose(local_c);
    }

    const clock_t begin_time = clock();
}

double Sim::get_amplitude(int x, int y)
{
    return a0[x][y];
}

double Sim::get_energy(int x, int y)
{
    return E[x][y];//a0[x][y]*a0[x][y]+a1[x][y]*a1[x][y]*277.8;
}

void Sim::tick()
{
    laplace_avg(); //propagate wave

    for(int x = 0; x < RES_X; x++) //apply force
    {
        for(int y = 0; y <= RES_Y; y++)
        {
            a2[x][y] += force_amplitude[x][y] * sin(t * force_omega[x][y] + force_phase[x][y]);
        }
    }

    for(int x = BORDER; x < RES_X - BORDER; x++) // calculate velocity
    {
        for (int y = BORDER; y < RES_Y - BORDER; y++)
        {
            if(border_distance(x, y) <= DAMP_BORDER) //border dampening
            {
                a2[x][y] -= 0.001 * a1[x][y] * (DAMP_BORDER - border_distance(x, y));
            }
            a1[x][y] += t_step * a2[x][y];
        }
    }

    for(int x = BORDER; x < RES_X - BORDER; x++) //calculate displacement
    {
        for (int y = BORDER; y < RES_Y - BORDER; y++)
        {
            a0[x][y] += t_step * a1[x][y];

            if(a0[x][y] * a0[x][y] + a1[x][y] * a1[x][y] * 277.8 > max_energy)
            {
                max_energy = a0[x][y] * a0[x][y] + a1[x][y] * a1[x][y] * 277.8;
            }

            if(abs(a0[x][y]) > max_amplitude)
            {
                max_amplitude = abs(a0[x][y]);
            }
        }
    }


    #ifndef walls

    for(int x = 0; x < BORDER; x++) //left wall
    {
        for (int y = BORDER; y < RES_Y - BORDER; y++)
        {
            a0[x][y] = a0[BORDER][y];
        }
    }

    for(int x = RES_X - BORDER; x < RES_X; x++) //right wall
    {
        for (int y = BORDER; y < RES_Y - BORDER; y++)
        {
            a0[x][y] = a0[RES_X - BORDER - 1][y];
        }
    }

    for(int x = 0; x < RES_X; x++) //top wall
    {
        for (int y = 0; y < BORDER; y++)
        {
            a0[x][y] = a0[x][BORDER];
        }
    }

    for(int x = 0; x < RES_X; x++) //bottom wall
    {
        for (int y = RES_Y - BORDER; y < RES_Y; y++)
        {
            a0[x][y] = a0[x][RES_Y - BORDER - 1];
        }
    }
    #endif
    if(count%100 == 0)
        printf("%i, %f, %f\n",
               count,
               float( clock () - system_begin_time ) /  CLOCKS_PER_SEC,
               count / (float( clock () - system_begin_time ) /  CLOCKS_PER_SEC));

    count++;

    t+= t_step;
}

#define classic

void Sim::laplace_avg()
{
    for(int y0 = LAP_RADIUS; y0 < RES_Y - LAP_RADIUS; y0++)
    {
        #ifdef window
        laplacian_sum = 0.0;
        double count = 0.0;

        for(int x = 0; x <= LAP_RADIUS + LAP_RADIUS; x++)
        {
            for(int y = y0 - LAP_RADIUS; y <= y0 + LAP_RADIUS; y++)
            {
                if ((x - LAP_RADIUS) * (x - LAP_RADIUS) + (y - y0) * (y - y0) <= LAP_RADIUS * LAP_RADIUS && (x != LAP_RADIUS || y0 != y))
                {
                    count += 1.0;
                    laplacian_sum += a0[x][y];
                }
            }
        }

        double laplacian = (laplacian_sum/count-a0[LAP_RADIUS][y0])*8/(RASTER*RASTER*LAP_RADIUS*LAP_RADIUS);
        a2[LAP_RADIUS][y0] = laplacian * local_c[LAP_RADIUS][y0] * local_c[LAP_RADIUS][y0];
        E[LAP_RADIUS][y0] = laplacian*laplacian*local_c[LAP_RADIUS][y0]*local_c[LAP_RADIUS][y0] + a1[LAP_RADIUS][y0]*a1[LAP_RADIUS][y0];


        for(int x0 = LAP_RADIUS + 1; x0 < RES_Y - LAP_RADIUS; x0++)
        {

            laplacian_sum -= a0[x0-4][y0];
            laplacian_sum -= a0[x0-3][y0-2];
            laplacian_sum -= a0[x0-3][y0-1];
            laplacian_sum -= a0[x0-3][y0+1];
            laplacian_sum -= a0[x0-3][y0+2];
            laplacian_sum -= a0[x0-1][y0-3];
            laplacian_sum -= a0[x0-1][y0+3];

            laplacian_sum += a0[x0+3][y0];
            laplacian_sum += a0[x0+2][y0-2];
            laplacian_sum += a0[x0+2][y0-1];
            laplacian_sum += a0[x0+2][y0+1];
            laplacian_sum += a0[x0+2][y0+2];
            laplacian_sum += a0[x0][y0-3];
            laplacian_sum += a0[x0][y0+3];

            double laplacian = (laplacian_sum/28-a0[x0][y0])*8/(RASTER*RASTER*LAP_RADIUS*LAP_RADIUS);
            a2[x0][y0] = laplacian * local_c[x0][y0] * local_c[x0][y0];
            E[x0][y0] = laplacian*laplacian*local_c[x0][y0]*local_c[x0][y0] + a1[x0][y0]*a1[x0][y0];
        }
        #endif

        #ifdef classic
        for(int x0 = LAP_RADIUS; x0 < RES_X - LAP_RADIUS; x0++)
        {
            double sum = 0.0;
            double count = 0;

            for(int x = x0 - LAP_RADIUS; x <= x0 + LAP_RADIUS; x++)
            {
                for(int y = y0 - LAP_RADIUS; y <= y0 + LAP_RADIUS; y++)
                {
                    if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= LAP_RADIUS * LAP_RADIUS && (x != x0 || y0 != y))
                    {
                        count += 1.0;
                        sum += a0[x][y];
                    }
                }
            }

            double laplacian = (sum/count-a0[x0][y0])*8/(RASTER*RASTER*LAP_RADIUS*LAP_RADIUS);
            a2[x0][y0] = laplacian * local_c[x0][y0] * local_c[x0][y0];
            E[x0][y0] = laplacian*laplacian*local_c[x0][y0]*local_c[x0][y0] + a1[x0][y0]*a1[x0][y0];
        }
        #endif
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

double Sim::border_distance(int x, int y)
{
    return std::min(std::min(RES_X - x, x), std::min(RES_Y - y, y));
}

double Sim::add_object(Object* object)
{
    //objects.push_back(object);
    //object->impose(local_c);
}

void Object::impose(double (&local_c)[RES_X][RES_Y])
{

}

Rect_object::Rect_object(int x0, int y0, int x1, int y1, double object_c):
x0(x0), y0(y0), x1(x1), y1(y1), object_c(object_c)
{

}

void Rect_object::impose(double (&local_c)[RES_X][RES_Y])
{
    for(int x = x0; x <= x1; x++)
    {
        for(int y = y0; y <= y1; y++)
        {
            local_c[x][y] = object_c;
        }
    }
}



