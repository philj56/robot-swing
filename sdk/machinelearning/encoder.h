#ifndef ENCODER_H
#define ENCODER_H

#include "pmd1208fs.h"
#include <iostream>

class Encoder
{
    public:
        explicit Encoder();
        ~Encoder();
        
        inline float GetAngle();
        inline float GetVelocity();
        inline void Calibrate();
        
    private:
        void ReadAngle();
        
        float cal;
        float raw_angle;
        float actual_angle;
        float old_angle;
        float velocity;
        
        bool end_thread=false;
        
        libusb_device_handle * handle;
};

#endif // ENCODER_H
