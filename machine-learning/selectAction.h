//function is fed with a priority queue of action-values 
//generates Boltzmann distribution of these action-values
//and selects an action based on probabilities 
Experience * selectAction(PriorityQueue<Experience *,double>& a_queue, double temp)
{	
	typedef PriorityQueue<Experience *,double> PQ;
	typedef std::vector< std::pair<Experience *, double> > Vec_Pair;
	typedef std::pair<Experience *, double> Pair;
	
	double sum= 0;
	int i = 0;
	
	int size = a_queue.getSize();
	Vec_Pair action_vec(size);
	Pair pear;
	
	//Calculate partition function by iterating over action-values
	for(PQ::constIter iter = a_queue.begin(),end=a_queue.end(); iter < end; ++iter)
	{
		sum += std::exp((iter->second)/temp);
	}
	//Calculate boltzmann factors for action-values
	for(Vec_Pair::iterator it = action_vec.begin(),end=action_vec.end(); it < end; ++it)
	{
	    it->first = a_queue[i].first;
	    it->second = std::exp(a_queue[i].second / temp) / sum;
	    ++i;
	}
	//calculate cumulative probability distribution    
	for(std::vector< std::pair<int, double> >::iterator it = action_vec.begin()+1,end=action_vec.end(); it < end; ++it)
	{
	    it->second += (it-1)->second;
	}
	
	//generate RN between 0 and 1
	double rand_num = (double)rand()/ (RAND_MAX);
	
	//select action based on probability 
	for(Vec_Pair::iterator it = action_vec.begin(),end=action_vec.end(); it < end; ++it)
	{
		//if RN falls within cumulative probability bin return the corresponding action
		if(rand_num < it->second)return it->first;
	}
}
