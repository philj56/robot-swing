/* Simple damped-driven pendulum solver
 */

#include <iomanip>
#include <iostream>
#include <fstream>
#include <cmath>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_odeiv.h>

extern const gsl_odeiv_step_type* gsl_odeiv_step_rk4;

const int columnWidth = 20;	// Width of output columns
const int precision = 10;	// Decimal places to output

const double m = 2;		// Mass of pendulum
const double M = 5;		// Mass of flywheel
const double I = 2;		// Inertia of flywheel
const double g = -9.81;		// g
const double l = 2;		// Length of pendulum
const int dimension = 2;	// Dimension of system 

int PrimeFuncGSL(double t, const double y[], double dydt[], void* params);	// The derivative of theta and w
double phi(double t, double w);
double dphi(double t, double w);
double ddphi(double t, double w);
double h(double t, double w);
double dh(double t, double w);

struct GSLParams
{
	double w;
	double b;
};

int main()
{
	struct GSLParams par;
	double wStep = 0.01;
	par.w = 0;	// Omega
	par.b = 0.2;
	double y[dimension] = {5.0 * M_PI / 4.0, 0};	// Array of [theta, w]
	double yerr[dimension] = {0, 0};	// Array of [theta, w]
	double dyin[dimension] = {0, 0};	// Array of [theta, w]
	double dyout[dimension] = {0, 0};	// Array of [theta, w]
	double t = 0;			// Time
	double tFinal = 1000;		// Time to end simulation
	double jMax = 450;		// Time to end simulation
	double jMin = 450;		// Time to end simulation
	double stepSize = 0.01;		// Initial step size for RK4
	double maxAmp = 0;

	gsl_odeiv_system GSLsystem = {PrimeFuncGSL, NULL, dimension, &par};			// The GSL system
	gsl_odeiv_step* StepFunc = gsl_odeiv_step_alloc(gsl_odeiv_step_rk4, dimension);	// The GSL stepping function
        GSL_ODEIV_FN_EVAL(&GSLsystem, 0, y, dyin);

	// Main loop
	for (int i = 5; i < 6; i++)
	{
		par.b = i * 0.1 + 0.05;
		for (int j = jMax; j >= jMin; j--)
		{
			t = 0;
			par.w = j * wStep;
			maxAmp = 0;
			y[0] = 5.0 * M_PI / 4.0;
			y[1] = 0;
			while (t < tFinal)
			{
				// Check for Runge-Kutta success
				if (gsl_odeiv_step_apply(StepFunc, t, stepSize, y, yerr, dyin, dyout, &GSLsystem) != GSL_SUCCESS)
				{
					break;
				}
		
				for (int i = 0; i < dimension; i++)
				{
					dyin[i] = dyout[i];
				}
		
				t += stepSize;
				if (t > 0.95*tFinal)
				{
					maxAmp = std::abs(fmod(std::abs(y[0]), 2.0 * M_PI) - M_PI) >  maxAmp ?  std::abs(fmod(std::abs(y[0]), 2.0 * M_PI) - M_PI) : maxAmp;
				}
				std::cout << std::setprecision(precision)
					  << std::setw(columnWidth) << t
					  << std::setw(columnWidth) << y[0] - M_PI
					  << std::endl;
			}

			gsl_odeiv_step_reset(StepFunc);
		}
		std::cout << std::endl;
	}
}

// Derivatives of theta and w
int PrimeFuncGSL(double t, const double y[], double dydt[], void* params)
{
	GSLParams* par = static_cast<GSLParams*>(params);
	double w = par->w;
	double b = par->b;
	dydt[0] = y[1];
	dydt[1] = (2.0 * M * y[1] * dh(t, w) * (l - h(t, w)) - I * ddphi(t, w) - g * sin(y[0]) * (m * l + M * (l - h(t, w)))) 
		/ (m * l * l + M * pow((l - h(t, w)), 2.0) + I) - b * y[1];
	return GSL_SUCCESS;
}

double phi(double t, double w)
{
	return 0;
}

double dphi(double t, double w)
{
	return 0;
}

double ddphi(double t, double w)
{
	return 0;
}

double h(double t, double w)
{
	return (l / 4.0) * (1.0 + sin(w * t));
}

double dh(double t, double w)
{
	return (l * w / 4.0) * cos(w * t);
}
