//
// Created by jan on 10.11.2020.
//

#ifndef WAVE_SIM_SIM_H
#define WAVE_SIM_SIM_H
#include "consts.h"

#define LAP_RADIUS 3
#define BORDER 3
#define walls
#define RASTER 1.0

class Sim
{
    double a0[RES_X][RES_Y];
    double a1[RES_X][RES_Y];
    double a2[RES_X][RES_Y];
    double t;
    const double c;
    const double t_step;
    void laplace_avg();
    double max_amplitude;
    double f_to_lambda(double f);
    double f_to_omega(double f);
    double lambda_to_f(double lambda);
    double lambda_to_omega(double lambda);

public:
    Sim();
    double get_amplitude(int x, int y);
    void tick();
    double get_max_amplitude();
};


#endif //WAVE_SIM_SIM_H
