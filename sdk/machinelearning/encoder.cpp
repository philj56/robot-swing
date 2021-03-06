/**
 * @file Encoder.cpp
 *
 * @brief Implementation file for Encoder class.
 *
 * @author Coding and Machine Learning Teams 2015-2016
 * @date March, 2016
 */

#include "encoder.h"

/**
 * Creates an encoder instance with cal field initialised to zero and the libusb_device_handle pointer
 * initialised with call to pmd_find_first. If multi-threading is enabled, a mutex is initialised 
 * and a POSIX thread is created here.
 */
Encoder::Encoder() :
	cal(0),
	handle(pmd_find_first()) {
	//  thread_mutex_init(&mut, NULL);
	//  pthread_create(&thread, NULL, ReadAngle, NULL);
}

/**
 * Destroys encoder object, if multi-threading is enabled the mutex field is destroyed here.
 */
Encoder::~Encoder()
{
//pthread_mutex_destroy(&mut);
}

float Encoder::GetAngle() {
	unsigned int a;//, c;

	a = pmd_digin16(handle);
	raw_angle = a & (2047);
	//  std::cout << raw_angle << "\t";
	actual_angle = raw_angle*(360.0 / 2048.0);

	//  unsigned int a, b, c;
	//  a = pmd_digin(handle, 0);
	//  b = pmd_digin(handle, 1);
	//  c = b << 8;
	//  raw_angle = (a | c) & (2047);
	//  actual_angle = raw_angle * (360.0/2048.0) - cal;

	return actual_angle - cal;
}

//float Encoder::GetAngle()
//{
//pthread_mutex_lock (&mut);
//return actual_angle;
//pthread_mutex_unlock (&mut);
//}

/*
float Encoder::GetVelocity()
{
pthread_mutex_lock (&mut);
return velocity;
pthread_mutex_unlock (&mut);
}
*/

void Encoder::Calibrate() {
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

/*void* Encoder::ReadAngle()
{
while(true)
{
raw_angle = pmd_digin16(handle) & (2047);

pthread_mutex_lock (&mut);

//calibrate angle
actual_angle = raw_angle*(360.0/2048.0) - cal;

//calculate velocity
velocity=(actual_angle-old_angle)/0.01;

pthread_mutex_unlock (&mut);

//sleep for 10ms
qi::os::msleep(10);

pthread_exit((void)* 0);
}
}*/
