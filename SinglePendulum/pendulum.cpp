#include <iomanip>
#include <iostream>
#include <fstream>
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>

const int columnWidth = 20;	// Width of output columns
const int precision = 10;	// Decimal places to output

const double m = 5.2;		// Mass of pendulum
const double g = -9.81;		// g
const double l = 2;		// Length of pendulum
const double A = 0.5;		// Amplitude of driving force
const double wd = 1;		// Angular frequency of driving force
const double b = 0.5;		// Damping coefficient
const int dimension = 2;	// Dimension of system 

int PrimeFuncGSL(double t, const double y[], double dydt[], void* params);	// The derivative of theta and w

int main()
{
	double epsAbs = 1e-12;		// Maximum desired absolute error
	double epsRel = 0;		// Maximum desired relative error

	double y[dimension] = {1, 0};	// Array of [theta, w]
	double t = 0;			// Time
	double tFinal = 60;		// Time to end simulation
	double stepSize = 0.1;		// Initial step size for RK4

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

		// Output data
		std::cout << std::setprecision(precision)
			  << std::setw(columnWidth) << t
			  << std::setw(columnWidth) << y[0] 
			  << std::setw(columnWidth) << evolve->yerr[0] 
			  << std::endl;
	}
}

// Derivatives of theta and w
int PrimeFuncGSL(double t, const double y[], double dydt[], void* params)
{
	dydt[0] = y[1];
	dydt[1] = -(g / l) * sin(y[0]) + (A * cos(wd * t) - b * y[1]) / (m * l * l);
	return GSL_SUCCESS;
}
