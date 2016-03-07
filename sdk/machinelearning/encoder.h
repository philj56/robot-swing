#ifndef ENCODER_H
#define ENCODER_H

#include "pmd1208fs.h"
#include <iostream>
#include <pthread.h>

class Encoder
{
    public:
        explicit Encoder();
//      ~Encoder();
        
        float GetAngle();
        float GetVelocity();
        void Calibrate();
        
    private:
//      void* ReadAngle();
        
        float cal;
        float raw_angle;
        float actual_angle;
//      float old_angle;
//      float velocity;
        
        libusb_device_handle * handle;
        
//      pthread_t thread;
//      pthread_mutex_t mut;
};

#endif // ENCODER_H
