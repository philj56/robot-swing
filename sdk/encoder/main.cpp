#include "encoder.h"
#include <iostream>
#include <fstream>
#include <sys/time.h>


int main()
{
    Encoder enc;

    //enc.Calibrate();

    std::ofstream myfile;
    myfile.open("encoderdata.txt");

    double elapsedTime;

    timeval t;


    //for (int i = 0; i < 100; i++){

    timeval to;
    gettimeofday(&to, NULL);

    while (elapsedTime < 20000.0){
        float temp = enc.GetAngle();
        gettimeofday(&t, NULL);

        elapsedTime = (t.tv_sec - to.tv_sec) * 1000;
        elapsedTime += (t.tv_usec - to.tv_usec) / 1000;
        myfile << elapsedTime << "\t" << temp << std::endl;
        std::cout << elapsedTime << "\t" << temp << std::endl;
     }


    return 0;
}
