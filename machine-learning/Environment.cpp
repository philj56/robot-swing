/*
* environment.cpp
* Robotics 2016
*
*/

#include "Environment.h"
#include <iostream>

Environment::Environment(double _theta, double _thetadot, double _torque, double _maxtorque, double _time, double _deltatime, double _mass, double _length, double _gamma):
	maxtorque(_maxtorque),
	dt(_deltatime),
	mass(_mass),
	l(_length),
	gamma(_gamma),
	theta(_theta),
	thetadot(_thetadot),
	torque(_torque),
	time(_time)
{}

void Environment::propagate() // Calculate successive values of theta and thetadot
{
	
	std::cout << "Progagating" << std::endl;
	
	/*
	double N = 100;		// Number of steps the RK4 method will use
	double h = dt / N;		// Calculates RK4 step size for N steps over the time dt

	for (int i = 0; i < N; ++i) // Calculate theta and thetadot at time t+dt
	{
		theta = rk4theta(h); 			// Calculate the value of theta at time t+h
		thetadot = rk4thetadot(h);		// Calculate the value of thetadot at time t+h
	}

	time += dt;	 // Propogate time
	*/
	
	theta+=thetadot*dt;
	thetadot+=(torque-l*9.81*sin(theta))*dt/mass;
	time+=dt;
	
	std::cout << "theta: " << theta << ", thetadot: " <<thetadot << "\n";
}

void Environment::setTorque(double _T)
{
	std::cout << "Setting torque to " << _T << std::endl;
	
	// Absolute value of the torque must be less than or equal to the maximum
	if (_T < maxtorque || _T == maxtorque) torque = _T;
}

void Environment::resetPendulum()
{
	theta = 0;
	thetadot = 0;
	torque = 0;
	time = 0;
}

double Environment::rk4theta(double h)
{
	double k1 = h * thetadot;
	double k2 = h * (thetadot + 0.5 * k1);
	double k3 = h * (thetadot + 0.5 * k2);
	double k4 = h * (thetadot + k3);

	theta += k1 / 6 + k2 / 3 + k3 / 3 + k4 / 6;

	return theta;
}
double Environment::rk4thetadot(double h)
{
	double g = 9.81;
	double k1 = h * (torque - gamma * thetadot - mass * g * l * sin(theta)) / (mass * l * l);
	double k2 = h * (torque - gamma * (thetadot + 0.5 * k1) - mass * g * l * sin(theta)) / (mass * l * l);
	double k3 = h * (torque - gamma * (thetadot + 0.5 * k2) - mass * g * l * sin(theta)) / (mass * l * l);
	double k4 = h * (torque - gamma * (thetadot + k3) - mass * g * l * sin(theta)) / (mass * l * l);

	thetadot += k1 / 6 + k2 / 3 + k3 / 3 + k4 / 6;

	return thetadot;
}
