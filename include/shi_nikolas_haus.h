#pragma once

#include <freertos/FreeRTOS.h>

class point
{
    point(
        double i_coor_x = 0.0,
        double i_coor_y = 0.0);
    ~point();

private:
    double m_coor_x;
    double m_coor_y;
    double error = 1e-3;

public:
    void get_x(double &o_coor_x);
    void set_x(double &i_coor_x);

    void get_y(double &o_coor_y);
    void set_y(double &i_coor_y);

    void get_error(double &o_error);
    void set_error(double &i_error);

    void distance(point &i_point2, double &io_distance);
    bool coincidence(point &i_point2);
};

// class line : point
// {
//     line(
//     point point1,
//     point point2
//     );
//     ~line();

//     private:
//     point m_point1;
//     point m_point2;
//     double m_length;

//     public:
//     void get_point1(point& o_point1);
//     void set_point1(point& i_point1);

//     void get_point2(point& o_point2);
//     void set_point2(point& i_point2);

//     void get_error(double& o_error);
//     void set_error(double& i_error);

//     void length(double& i_coor_x, double& i_coor_y, double& io_distance);
//     bool coincidence(line& i_line2);
// };

class nikolas_haus : point
{
    nikolas_haus(
        point i_point_ap,
        point i_point_ne,
        point i_point_se,
        point i_point_sw,
        point i_point_nw);
    ~nikolas_haus();

private:
    point m_point_ap;
    point m_point_ne;
    point m_point_se;
    point m_point_sw;
    point m_point_nw;
    uint8_t config;

public:
    void configure(uint8_t i_config);
};