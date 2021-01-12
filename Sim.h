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
#include <SDL2/SDL.h>

#define LAP_RADIUS 3
#define DAMP_BORDER 50
#define BORDER 3
#define walls
#define RASTER 1.0

class Object
{
protected:
    double object_c;
    double object_force_amplitude;
    double object_force_omega;
    double object_force_phase;
    bool visible, filled;
    int color_r, color_g, color_b;
public:
    Object(double object_c, double object_force_amplitude, double object_force_omega, double object_force_phase);
    virtual void impose(double (&local_c)[RES_X][RES_Y],
                        double (&force_amplitude)[RES_X][RES_Y],
                        double (&force_omega)[RES_X][RES_Y],
                        double (&force_phase)[RES_X][RES_Y]) = 0;
    virtual void display(SDL_Renderer* renderer) = 0;
    void set_visible(bool is_visible);
    void set_color(int r, int g, int b);
    virtual bool overlaps(int x, int y) = 0;
};

class Rect_object : public Object
{
    int x0, y0, x1, y1;
public:
    Rect_object(int x0, int y0, int x1, int y1, double object_c,
                double object_force_amplitude, double object_force_omega, double object_force_phase);
    void impose(double (&local_c)[RES_X][RES_Y],
                        double (&force_amplitude)[RES_X][RES_Y],
                        double (&force_omega)[RES_X][RES_Y],
                        double (&force_phase)[RES_X][RES_Y]) override;
    void display(SDL_Renderer* renderer) override;
    bool overlaps(int x, int y) override;
};

class Circle_object : public Object
{
    int x0, y0, r;
public:
    Circle_object(int x0, int y0, int r, double object_c,
                  double object_force_amplitude, double object_force_omega, double object_force_phase);
    void impose(double (&local_c)[RES_X][RES_Y],
                        double (&force_amplitude)[RES_X][RES_Y],
                        double (&force_omega)[RES_X][RES_Y],
                        double (&force_phase)[RES_X][RES_Y]) override;
    virtual void display(SDL_Renderer* renderer) override;
    bool overlaps(int x, int y) override;
};

class Sim
{
    double a0[RES_X][RES_Y];
    double a1[RES_X][RES_Y];
    double a2[RES_X][RES_Y];
    double E[RES_X][RES_Y];

    double force_omega[RES_X][RES_Y];
    double force_amplitude[RES_X][RES_Y];
    double force_phase[RES_X][RES_Y];
    double local_c[RES_X][RES_Y];

    double t;
    const double c;
    const double t_step;
    double max_amplitude;
    double max_energy;
    double laplacian_sum;
    //clock_t system_begin_time;
    std::vector <Object*> objects = std::vector<Object*>();

    void laplace_avg();
    double border_distance(int x, int y);
public:
    Sim();
    void reset_arrays();
    double get_amplitude(int x, int y);
    double get_energy(int x, int y);
    void tick();
    double get_max_amplitude();
    void add_object(Object* object);
    std::vector <Object*> get_objects();
    double f_to_lambda(double f);
    double f_to_omega(double f);
    double lambda_to_f(double lambda);
    double lambda_to_omega(double lambda);
    void remove_object_at(int x, int y);

};


#endif //WAVE_SIM_SIM_H
