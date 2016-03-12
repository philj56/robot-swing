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
{
	//initialise SDL
	SDL_Init(SDL_INIT_VIDEO);
	
	//create the window and the window surface
	window = SDL_CreateWindow( "Inverted Pendulum", 0,0,130,130, SDL_WINDOW_SHOWN );
	window_surface=SDL_GetWindowSurface(window);
	
	//get the images of the three states as SDL_Surface*s
	point=SDL_LoadBMP("point.bmp");
	
	//position rectangles for rendering purposes
	centre_position.x=65;
	centre_position.y=65;
	centre_position.w=1;
	centre_position.h=1;
	pendulum_position.x=65;
	pendulum_position.y=115;
	pendulum_position.w=1;
	pendulum_position.h=1;
}

Environment::~Environment()
{
	SDL_FreeSurface(window_surface);
	SDL_FreeSurface(point);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

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
	
	print();
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

void Environment::print()
{
	//fill the window with white
	SDL_FillRect(window_surface,NULL,SDL_MapRGB(window_surface->format,0xFF,0xFF,0xFF));
	
	//blit the centre
	SDL_BlitSurface(point,NULL,window_surface,&centre_position);
	
	//set the pendulum position and blit the end point
	pendulum_position.x=std::round(centre_position.x+50*std::sin(theta));
	pendulum_position.y=std::round(centre_position.y+50*std::cos(theta));
	SDL_BlitSurface(point,NULL,window_surface,&pendulum_position);
	
	//update the window
	SDL_UpdateWindowSurface(window);
	
	//pause for smoothness
	SDL_Delay(dt*1000);
}
