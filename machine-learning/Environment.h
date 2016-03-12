#include <cmath>
#include "SDL2/SDL.h"

class Environment
{
	public:
	
		explicit Environment(double _theta, double _thetadot, double _torque, double _maxtorque, double _time, double _deltatime, double _mass, double _length, double _gamma);
		
		void propagate(); //Propagate the system through time
		
		double getTheta(){return std::acos(std::cos(theta));}
		double getThetadot() {return thetadot;}
		double getTorque() {return torque;}
		double getTime() {return time;}
		
		void setTorque(double _T); //Machine learning code sets the torque
		
		void resetPendulum();
		
	private:
		
		//Runge-Kutta 4 method for solving the equation of motion for a driven pendulum
		double rk4theta(double h); //RK4 solve for theta
		double rk4thetadot(double h); //RK4 solve for thetadot
		
		
		const double maxtorque; //Set a maximum torque in order to keep the pendulum 'undertorqued' (N m)
		
		
		const double dt; //The time interval the pendulum will be propogated over
		const double mass; //Mass of the pendulum
		const double l;	//Length of the pendulum
		const double gamma; //Damping factor (initially arbitrary value chosen)
		
		
		// Track the number of trials either here or in the main
		/*
		Approximate to the Phidget and stepper motor:
		mass = 0.5, l = 0.2, must find gamma
		*/
		
		double theta;
		double thetadot;
		double torque;
		double time;
		
};

#endif //ENVIRONMENT_H_ 
