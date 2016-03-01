#ifndef ACTION_H
#define ACTION_H

//struct to store a basic action
//currently just stores an integer identifier - more will be added later

struct Action
{
	explicit Action(const int _action, void (*)() _execute);
	
	//numerical identifier for the action - PLZ DONT CHANGE
	int action;
	
	//function to call execute the associated action
	void (*)() execute;
	
	//comparison operator for use by priority queue
	bool operator==(const Action& target) const;
	
	/**
	 * @brief Overloaded inequivalent operator
	 * 
	 * @param target action object to compare
	 */
	bool operator!=(const Action& target) const;
	
	/**
	 * @brief Overloaded assignment operator
	 * 
	 * @param target action object to assign to this
	 */
	const Action& operator=(const Action& target);
};

#endif
