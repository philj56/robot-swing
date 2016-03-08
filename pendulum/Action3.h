#ifndef ACTION_H
#define ACTION_H

//struct to store a basic action
//currently just stores an integer identifier - more will be added later

struct Action
{
	explicit Action(const int _action, void (*_execute)(int), int _param);
	
	//numerical identifier for the action - PLZ DONT CHANGE
	int action;
	int param;
	
	//function to call execute the associated action
	void (*execute)(int);
	
	/**
	 * @brief Overloaded equivalent operator
	 * 
	 * @param target action object to compare
	 * @return true if target is equivalent to this 
	 */
	bool operator==(const Action& target) const;
	
	/**
	 * @brief Overloaded inequivalent operator
	 * 
	 * @param target action object to compare
	 * @return true is target is not-equivalent to this
	 */
	bool operator!=(const Action& target) const;
	
	/**
	 * @brief Overloaded assignment operator
	 * 
	 * @param target action object to assign to this
	 * @return const reference to assigned action object
	 */
	const Action& operator=(const Action& target);
};

#endif
