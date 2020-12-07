//
// Created by jan on 10.11.2020.
//

#ifndef WAVE_SIM_SIM_H
#define WAVE_SIM_SIM_H
#include "consts.h"
#include <math.h>
#include <stdio.h>
#include <algorithm>
#include <time.h>
#include <vector>

#define LAP_RADIUS 3
#define DAMP_BORDER 50
#define BORDER 3
#define walls
#define RASTER 1.0

class Object
{

public:
    virtual void impose(double (&local_c)[RES_X][RES_Y]);
    //virtual double display() = 0;
};

class Rect_object : public Object
{
    int x0, y0, x1, y1;
    double object_c;
public:
    Rect_object(int x0, int y0, int x1, int y1, double object_c);
    void impose(double (&local_c)[RES_X][RES_Y]) override;
};

class Sim
{
    double a0[RES_X][RES_Y];
    double a1[RES_X][RES_Y];
    double a2[RES_X][RES_Y];
    double E[RES_X][RES_Y];
    double t;
    const double c;
    const double t_step;
    void laplace_avg();
    double max_amplitude;
    double max_energy;
    double f_to_lambda(double f);
    double f_to_omega(double f);
    double lambda_to_f(double lambda);
    double lambda_to_omega(double lambda);
    double border_distance(int x, int y);
    double max0, max1;
    int count;
    double force_omega[RES_X][RES_Y];
    double force_amplitude[RES_X][RES_Y];
    double force_phase[RES_X][RES_Y];
    double local_c[RES_X][RES_Y];
    double laplacian_sum;
    clock_t system_begin_time;
    std::vector <Object*> objects;

public:
    Sim();
    double get_amplitude(int x, int y);
    double get_energy(int x, int y);
    void tick();
    double get_max_amplitude();
    double add_object(Object* object);
};


#endif //WAVE_SIM_SIM_H
