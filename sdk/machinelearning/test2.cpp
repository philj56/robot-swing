#include "PriorityQueue.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <utility>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <boost/bind.hpp>

double temperature(unsigned long t) {
	return 100.0*std::exp((-8.0*t*t) / (2600.0*2600.0)) + 0.1;//0.1 is an offset
	//	^
	//	|
	//make this large
}

int selectAction(PriorityQueue<int, double>& a_queue, unsigned long iterations) {
	
	typedef std::vector<std::pair<int, double> > VecPair ; 
	
	//turn priority queue into a vector of pairs
	VecPair vec = a_queue.saveOrderedQueueAsVector();

    //sum for partition function 
	double sum = 0.0;

	// calculate partition function by iterating over action-values
	for (VecPair::iterator iter = vec.begin(), end = vec.end(); iter < end; ++iter) {
		sum += std::exp((iter->second) / temperature(iterations));
	}

	// compute Boltzmann factors for action-values and enqueue to vec
	for (VecPair::iterator iter = vec.begin(); iter < vec.end(); ++iter) {
		iter->second = std::exp(iter->second / temperature(iterations)) / sum;
	}

	// calculate cumulative probability distribution
	for (VecPair::iterator iter = vec.begin()++, end = vec.end(); iter < end; ++iter) {
        //second member of pair becomes addition of its current value
        //and that of the index before it
		iter->second += (iter-1)->second;
	}

	//generate RN between 0 and 1
	double rand_num = static_cast<double>(rand()) / RAND_MAX;

	// choose action based on random number relation to priorities within action queue
	for (VecPair::iterator iter = vec.begin(), end = vec.end(); iter < end; ++iter) {
		if (rand_num < iter->second)
			return iter->first;
	}
	
	return -1; //note that this line should never be reached	
}
int main(){
    std::vector< std::pair<int, double> > vec;
    std::pair<int, double> pear;
    //size of vector
    int size = 10;
    std::vector< int > vec2(size, 0);

    //make priority queue full of zeroes 
    //except for one optimal action 
    for(int i = 0; i < size; i++)
    {   
        pear = std::make_pair(i, 0.0);
        //optimal action
        if(i ==3){pear = std::make_pair(i, 100);}
        vec.push_back(pear);
    } 
    
    unsigned long i = 0;
    int chosen_action;
    
    //coinstruct PQ from vector of pairs
    PriorityQueue<int, double> a_queue(vec, MAX);
    
    //choses action many times
	for(int j=0; j < 1000; j++){
	    //finds action
		chosen_action = selectAction(a_queue, i);
		//increases value of vec2 in bin corresponding 
		//to chosen action
		vec2[chosen_action] ++;
		i++;
	}
	std::cout<< "action" << "\tNumber of times selected" << std::endl;
	//prints contents of vector
	int j = 0;
	for(std::vector< int >::iterator it = vec2.begin(); it < vec2.end(); it++)
	{
	    std::cout << j << "\t" << (*it) << std::endl;
	    j++;
	}

    return 0;
}