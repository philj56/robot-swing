#ifndef ENCODER_H
#define ENCODER_H

#include "pmd1208fs.h"
#include <iostream>
class Encoder
{
public:
    Encoder();


    float actual_angle, raw_angle, temp;

    libusb_device_handle * handle;


    inline float GetAngle();
    inline void Calibrate();

    float cal;
};

float Encoder::GetAngle(){
   unsigned int a;//, c;

   a = pmd_digin16(handle);
   raw_angle = a & (2047);
   //std::cout << raw_angle << "\t";
   actual_angle = raw_angle*(360.0/2048.0);
/*
    unsigned int a, b, c;
    a = pmd_digin(handle, 0);
    b = pmd_digin(handle, 1);

    c = b << 8;

    raw_angle = (a | c) & (2047);

    actual_angle = raw_angle * (360.0/2048.0) - cal;
*/
    return actual_angle;
}

void Encoder::Calibrate(){
    unsigned int a, b, c;
    a = pmd_digin(handle, 0);
    b = pmd_digin(handle, 1);

    c = b << 8;

    raw_angle = (a | c) & (2047);

    actual_angle = raw_angle * (360.0/2048.0) - cal;

    cal = actual_angle;

}

#endif // ENCODER_H
