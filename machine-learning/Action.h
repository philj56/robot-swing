//enum to identify actions
enum actions{FORWARDS,BACKWARDS};

//struct to store a basic action
//currently just stores an integer identifier - more will be added later

struct Action
{
	explicit Action(const int _action);
	
	const int action;
	
	//function to call execute the associated action
	void exectute();
	
	//needs function pointer to actual action
}
