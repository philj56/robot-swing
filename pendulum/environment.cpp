/* 
 * environment.cpp
 * Robotics 2016
 *
 */
 
 #include "environment.h"
 #include <cmath>
 
 environment::environment(double _theta, double _thetadot, double _torque, double _deltatorque, double _maxtorque, double _time, double _deltatime, double _mass, double _length, double _gamma)
 : theta(_theta), thetadot(_thetadot), torque(_torque), deltatorque(_deltatorque), maxtorque(_maxtorque), time(_time), deltatime(_deltatime), mass(_mass), length(_length), gamma(_gamma)
 {
	 initialtorque = torque;
 }
 
 void environment::propogate() // Calculate successive values of theta and thetadot
 {
	double N = 100;			// Number of steps the RK4 method will use
	double h = dt/N;		// Calculates RK4 step size for N steps over the time dt
	
	for (i = 0; i < N; i++) // Calculate theta and thetadot at time t+dt
	{
		theta = rk4theta(h); 			// Calculate the value of theta at time t+h
		thetadot = rk4thetadot(h);		// Calculate the value of thetadot at time t+h
	}
	
	time += deltatime;	// Propogate time
	// Output t, theta, thetadot, and torque to a file either here or in the main
 }
 
void environment::setTorque(double _T)
{
	// Absolute value of the torque must be less than or equal to the maximum
	if (_T < maxtorque || _T == maxtorque) torque = _T;
}


double environment::rk4theta(double h)
{
	double k1 = h * thetadot;
	double k2 = h * (thetadot + 0.5 * k1);
	double k3 = h * (thetadot + 0.5 * k2);
	double k4 = h * (thetadot + k3);
	
	double theta = theta + k1/6 + k2/3 + k3/3 + k4/6;
	
	return theta;
}
double environment::rk4thetadot(double h)
{
	double k1 = h * (torquen - gamma * thetadot - m * g * l * sin(thetan))/(m * l * l);
	double k2 = h * (torquen - gamma * (thetadot + 0.5 * k1) - m * g * l * sin(thetan))/(m * l * l);
	double k3 = h * (torquen - gamma * (thetadot + 0.5 * k2) - m * g * l * sin(thetan))/(m * l * l);
	double k4 = h * (torquen - gamma * (thetadot + k3) - m * g * l * sin(thetan))/(m * l * l);
	
	thetadot = thetadot + k1/6 + k2/3 + k3/3 + k4/6;
	
	return thetadot;
}
