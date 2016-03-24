/**
 * @file Encoder.h
 *
 * @brief Interface file for Encoder class.
 *
 * @author Coding and Machine Learning Teams 2015-2016
 * @date March, 2016
 */

#ifndef ENCODER_H
#define ENCODER_H

#include "pmd1208fs.h"
#include <iostream>
#include <pthread.h>

/**
 * @class Encoder
 *
 * @brief Class to hold instanced data of encoder at top of robot-swing system.
 *
 * Holds angle data for encoder component of robot-swing system at any given stage, used
 * to find and update angles and (indirectly) velocities of the robot on the swing.
 * 
 * @author Coding and Machine Learning Teams 2015-2016
 * @date March, 2016
 */
class Encoder {
public:

	/**
	 * @brief Default constructor.
	 */
	explicit Encoder();
	
	/**
	 * @brief Destructor.
	 */
	~Encoder();

	/**
	 * @brief Gets the current encoder angle value.
	 *
	 * @return Current value of the encoder angle in degrees.
	 */
	float GetAngle();

	/**
	 * @brief Gets the current robot velocity, interpolated (indirect - may be inaccurate).
	 *
	 * @return Current value of the robot velocity, in radians/sec.
	 */
	float GetVelocity();

	/**
	 * @brief Calibrates the encoder angle.
	 */
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
