#include "encoder.h"

Encoder::Encoder():
cal(0),
end_thread(false),
handle(pmd_find_first())
{
    //thread_var=startThread(ReadAngle);
}

Encoder::~Encoder()
{
    end_thread=true;
    //thread_var.join();
}

float Encoder::GetAngle(){
    
    return actual_angle;
/*  
    unsigned int a;//, c;

    a = pmd_digin16(handle);
    raw_angle = a & (2047);
//  std::cout << raw_angle << "\t";
    actual_angle = raw_angle*(360.0/2048.0);
    
//  unsigned int a, b, c;
//  a = pmd_digin(handle, 0);
//  b = pmd_digin(handle, 1);
//  c = b << 8;
//  raw_angle = (a | c) & (2047);
//  actual_angle = raw_angle * (360.0/2048.0) - cal;
    
    return actual_angle - cal;
*/
}

float Encoder::GetVelocity()
{
    return velocity;
}

void Encoder::Calibrate(){
/*
    unsigned int a, b, c;
    a = pmd_digin(handle, 0);
    b = pmd_digin(handle, 1);
    c = b << 8;
    raw_angle = (a | c) & (2047);
    actual_angle = raw_angle * (360.0/2048.0) - cal;
*/
    cal = GetAngle();

}

void Encoder::ReadAngle()
{
    while(!end_thread)
    {
        raw_angle = pmd_digin16(handle) & (2047);
        
        actual_angle = raw_angle*(360.0/2048.0) - cal;
        
        //calculate velocity
        velocity=(actual_angle-old_angle)/0.01;
        
        //sleep for 10ms
        qi::os::msleep(10);
    }
}
