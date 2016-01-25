/* Damped pendulum, with shifting centre of mass to simulate robot
 */

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>

const int columnWidth = 20;	// Width of output columns
const int precision = 10;	// Decimal places to output

const double m = 5.2;		// Mass of robot
const double mp = 20;		// Mass of pendulum
const double g = 9.81;		// g
const double l = 2.5;		// Length of pendulum
const double wr = std::sqrt(g / l);		// Angular frequency of robot movement
const double b = 0.05;		// Damping coefficient
const double range = 0.045;	// Half range of robot movement in m
const double height = 0.34;	// Height of CoM of robot from feet in m
const int dimension = 2;	// Dimension of system 

int PrimeFuncGSL(double t, const double y[], double dydt[], void* params);	// The derivative of theta and w

int main()
{
	double epsAbs = 1e-12;		// Maximum desired absolute error
	double epsRel = 0;		// Maximum desired relative error

	double y[dimension] = {0, 0};	// Array of [theta, w]
	double t = 0;			// Time
	double tFinal = 240;		// Time to end simulation
	double stepSize = 0.1;		// Initial step size for RK4

	double maxTheta = 0;		// Maximum angle reached

	gsl_odeiv_system GSLsystem = {PrimeFuncGSL, NULL, dimension, NULL};			// The GSL system
	gsl_odeiv_step* StepFunc = gsl_odeiv_step_alloc(gsl_odeiv_step_rkf45, dimension);	// The GSL stepping function
	gsl_odeiv_control* control = gsl_odeiv_control_standard_new(epsAbs, epsRel, 1, 0);	// The control function for adaptive step sizing
	gsl_odeiv_evolve* evolve = gsl_odeiv_evolve_alloc(dimension);				// The workspace to evolve the system with adaptive step sizing
	
	// Main loop
	while (t < tFinal)
	{
		// Check for Runge-Kutta success
		if (gsl_odeiv_evolve_apply(evolve, control, StepFunc, &GSLsystem, &t, tFinal, &stepSize, y) != GSL_SUCCESS)
		{
			break;
		}
	
		// Record maximum angle reached
		if (y[0] > maxTheta)
		{
			maxTheta = y[0];
		}

		// Output data
		std::cout << std::setprecision(precision)
			  << std::setw(columnWidth) << t
			  << std::setw(columnWidth) << y[0] 
			  << std::setw(columnWidth) << evolve->yerr[0] 
			  << std::setw(columnWidth) << y[1] 
			  << std::setw(columnWidth) << evolve->yerr[1] 
			  << std::endl;
	}
	std::cerr << maxTheta << std::endl;
}

// Derivatives of theta and w
int PrimeFuncGSL(double t, const double y[], double dydt[], void* params)
{
	double theta = (mp * y[0] + m * (y[0] + (range / (l - height)) * cos(wr * t))) / (mp + m);	// Theta of centre of mass
	double f = - (m * m * wr * wr * range / ((l - height) * (mp + m))) * cos(wr * t);		// Approximate force exered by robot on swing

	dydt[0] = y[1];
	dydt[1] = - (g / l) * sin(theta) + (f - b * y[1]) / (m * l * l); 
	return GSL_SUCCESS;
}
