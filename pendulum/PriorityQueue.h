#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <vector>
#include <string>
#include <stdexcept>
#include <utility>

/**
* @enum HeapType
*
* @brief Type of binary heap structure for a PriorityQueue implementation, contains the
*		 constants MIN and MAX representing minimum and maximum binary heaps respectively.
*/
enum HeapType {

	MIN,
	MAX

};

/**
* @class PriorityQueue
*
* @brief Template class representing a priority queue type data structure where each item (of type T) stored in the
*		  queue has a priority level (of type PT) associated with it.
*
* This priority queue structure has been implemented with a Min/Max Binary Heap (MBH) of dynamic size based on
* a vector of data with corresponding priorities. Both minimum and maximum heap structures can be chosen when
* creating a priority queue instance, whereby a minimum binary heap will store data items in order of ascending
* priority (such that the minimum priority element is dequeued first) and a maximum binary heap will store data
* items in order of descending priority (such that the maximum priority element is dequeued first).
*
* Due to this underlying structure, the priority queue has efficient operation times, below is a list of worst case
* time complexities for operations in this queue (where n is the number of entries in the priority queue):
*
* <strong>Time Complexities of Operations</strong>
* \verbatim
Enqueue [single] = O(log n)
Dequeue [single] = O(log n)
PeekFront = O(1)
Search [single] = O(n)
ChangePriority [single] = O(n log n)
Clear = O(1) if type T has trivial destructor
\endverbatim
*
* <strong>Important Notes on Instantiation Requirements</strong>
*
* The priority type PT should, generally, be a primitive such as int, double, float - however any types/classes which have assignment, greater than,
* less than and equality operators are supported (though generally not recommended).
*
* The type T that a priority queue is instantiated with MUST have overloaded assignment and equality operators as well as a defined toString() method.
* Additionally, the instantiated type T must have an "overloaded" definition of to_string(const T&) - this is required for Argument-Dependent-Lookup (ADL)
* to work; to do this, in the namespace where class T is defined one must define the following:
*
* \code{.cpp}
*	std::string to_string(const T& tInst) {
*		return tInst.toString();
*	}
* \endcode
*
* Note that if one is using priority queues with only primitive data types as the template typename arguments, then the following function should
* be defined in the namespace where the priority queues are instantiated (do NOT do this if you are also using queues with non-primitive types
* as well, in which case you should have to_string methods defined for any classes you use as mentioned above):
*
* \code{.cpp}
*	template<typename T> std::string to_string(T x) {
*		return static_cast<std::ostringstream&>((std::ostringstream() << std::dec << x)).str();
*	}
* \endcode
*
* <strong>To-Do List for Priority Queue class</strong>
*
* @todo [MEDIUM PRIORITY] Generic code cleaning - avoid any warnings and make sure pass-by types and return types are consistent and make sense.
*
* @todo [LOW PRIORITY] Implement a fixed size priority queue, would require a separate constructor, max capacity field variable,
*		isFixedCapacity field variable flag and extra conditions in enqueuing methods.
*
* <strong>Example Instantiations of Priority Queue</strong>
*
* Declare an empty priority queue of integers and integer priority with minimum heap structure:
*
* \code{.cpp}
*	PriorityQueue<int, int> priorityQueue(MIN);
* \endcode
*
* Declare an empty priority queue of doubles with unsigned int priorities with maximum heap structure using pointers:
*
* \code{.cpp}
*	PriorityQueue<double, unsigned int>* priorityQueue = new PriorityQueue<double, unsigned int>(MAX);
* \endcode
*
* Declare a priority queue of integers and double priorities with maximum heap structure from a vector of pairs:
*
* \code{.cpp}
*	std::vector< std::pair<int, double> > dataVector;
*	dataVector.push_back(std::make_pair<int,double>(1,1.0));
*	// ...
*	PriorityQueue<int, double> priorityQueue(dataVector, MAX);
* \endcode
*
* Declare a priority queue from an already existing priority queue (copy constructing):
*
* \code{.cpp}
*	PriorityQueue<int, int> priorityQueueOne(MIN);
*	PriorityQueue<int, int> priorityQueueTwo(priorityQueueOne);
* \endcode
*
* @author Samuel Rowlinson
* @date February, 2016
*/
template<typename T, typename PT> class PriorityQueue {

private:

	/*********************************************************************/
	/**********************	PRIVATE FIELD VARIABLES **********************/
	/*********************************************************************/

	std::vector< std::pair<T, PT> > dataWithPriorityVec;
	HeapType heapType;

	/**********************************************************************/
	/**********************	PRIVATE MEMBER FUNCTIONS **********************/
	/**********************************************************************/

	/**
	* @brief Bubble down the MBH, performing swaps if any priorities are out of order.
	*
	* @param position Position in MBH to perform bubbling down from
	*/
	void bubbleDownHeap(size_t position) {

		size_t sizeVec = dataWithPriorityVec.size();

		// position of left node of heap in terms of vector storage
		size_t leftNodePos = 2 * position + 1;

		// position of right node of heap in terms of vector storage
		size_t rightNodePos = 2 * position + 2;

		// if position of left node is >= vector size then this is a leaf node (do nothing)
		if (leftNodePos >= sizeVec) {
			return;
		}

		size_t minPos = position;

		switch (heapType) {

			// if type of heap is MIN, check child node priorities for smaller values than current node priority
		case MIN:
			// set minimum position in heap to left child node
			if (dataWithPriorityVec.at(position).second > dataWithPriorityVec.at(leftNodePos).second) {
				minPos = leftNodePos;
			}

			// set minimum position in heap to right child node
			if (rightNodePos < sizeVec && dataWithPriorityVec.at(minPos).second > dataWithPriorityVec.at(rightNodePos).second) {
				minPos = rightNodePos;
			}
			break;

			// if type of heap is MAX, check child node priorities for greater values than current node priority
		case MAX:
			// set minimum position in heap to left child node
			if (dataWithPriorityVec.at(position).second < dataWithPriorityVec.at(leftNodePos).second) {
				minPos = leftNodePos;
			}

			// set minimum position in heap to right child node
			if (rightNodePos < sizeVec && dataWithPriorityVec.at(minPos).second < dataWithPriorityVec.at(rightNodePos).second) {
				minPos = rightNodePos;
			}
			break;

		}

		// if the minimum position variable has changed, then a swap is required
		if (minPos != position) {

			// set temporary data to dataVec at index [position] (saved for swap)
			T tempData = dataWithPriorityVec.at(position).first;
			PT tempPriority = dataWithPriorityVec.at(position).second;

			// perform the swaps
			dataWithPriorityVec.at(position) = std::make_pair(dataWithPriorityVec.at(minPos).first, dataWithPriorityVec.at(minPos).second);
			dataWithPriorityVec.at(minPos) = std::make_pair(tempData, tempPriority);

			// recursively call bubbleDownHeap with minPos as position of data vector
			bubbleDownHeap(minPos);

		}

	}

	/**
	* @brief Bubble up the MBH, performing swaps if any priorities are out of order.
	*
	* @param position Position in MBH to perform bubbling up from
	*/
	void bubbleUpHeap(size_t position) {

		// if position is zero, the node is root node - do nothing
		if (position == 0) {
			return;
		}

		// position of parent in vector in terms of heap storage
		size_t parentPos = (position - 1) / 2;

		bool swapRequired = false;

		switch (heapType) {

			// if minimum binary heap, check if parent node priority is greater than 
			// current node priority and set swapRequired flag to true if necessary
		case MIN:
			if (dataWithPriorityVec.at(parentPos).second > dataWithPriorityVec.at(position).second)
				swapRequired = true;
			break;

			// if maximum binary heap, check if parent node priority is less than
			// current node priority and set swapRequired flag to true if necessary
		case MAX:
			if (dataWithPriorityVec.at(parentPos).second < dataWithPriorityVec.at(position).second)
				swapRequired = true;
			break;

		}

		// if swapRequired flag is true, perform swap of nodes and recursively call bubbleUpHeap with
		// position of parent node of current node
		if (swapRequired) {

			// set temporary data to dataVec at index [position] (saved for swap)
			T tempData = dataWithPriorityVec.at(position).first;
			PT tempPriority = dataWithPriorityVec.at(position).second;

			// perform the swaps
			dataWithPriorityVec.at(position) = std::make_pair(dataWithPriorityVec.at(parentPos).first, dataWithPriorityVec.at(parentPos).second);
			dataWithPriorityVec.at(parentPos) = std::make_pair(tempData, tempPriority);

			// recursively call bubbleUpHeap with parentPos as position of data vector
			bubbleUpHeap(parentPos);
		}

	}

	/**
	* @brief Removes the top item of the heap, i.e. elements with highest/lowest priority for MAX/MIN queue.
	*/
	void removeTopOfHeap() {

		size_t sizeVec = dataWithPriorityVec.size();

		// if vector is empty, do nothing
		if (sizeVec == 0) {
			return;
		}

		// shift items and remove top heap item
		dataWithPriorityVec.at(0) = std::make_pair(dataWithPriorityVec.at(sizeVec - 1).first, dataWithPriorityVec.at(sizeVec - 1).second);
		dataWithPriorityVec.pop_back();

		// bubble down from the root of the heap
		bubbleDownHeap(0);

	}

	/**
	* @brief Performs heapification of the MBH, sorting all data into correct positions
	*
	* @todo Fix overflow behaviour of size_t counters - don't want to use int's, possible data loss.
	*
	* Heapifies the underlying binary heap structure, bubbling down the heap from every position. This method
	* checks that each node of the underlying heap has the properties of a Min/Max binary heap and bubbles down
	* at the node if this property is violated.
	*/
	void heapification() {

		// start at the end of the priority queue bubbling down the MBH from each entry (iterating
		// up the heap) to preserve MBH and priority queue behaviour of the data structure
		for (size_t i = (int)(getSize() - 1); (int)i >= 0; (int)i--) {
			bubbleDownHeap(i);
		}

	}

	/**
	* @brief Copies the parameterised queue to this queue
	*
	* @param copyTarget Queue to copy
	*/
	void copy(const PriorityQueue<T, PT>& copyTarget) {
		heapType = copyTarget.heapType;
		dataWithPriorityVec = copyTarget.dataWithPriorityVec;
	}

protected:

	/**
	* @brief Computes the end priority in the queue.
	*
	* Gives the largest value of priority for a MAX heap queue and the smallest
	* value of priority for a MIN heap queue.
	*
	* @return Final/End value of priority in this queue
	*/
	PT lastPriority() const {

		PT endPr = dataWithPriorityVec.at(0).second;

		// iterate over queue
		for (const_iterator iter = begin(); iter < end(); ++iter) {

			switch (heapType) {

				// find maximum priority value
			case MIN:
				if (iter.operator*().second > endPr)
					endPr = iter.operator*().second;
				break;

				// find minimum priority value
			case MAX:
				if (iter.operator*().second < endPr)
					endPr = iter.operator*().second;

				break;

			}

		}

		return endPr;

	}

public:

	typedef typename std::vector< std::pair<T, PT> >::const_iterator const_iterator;

	/************************************************************************/
	/**********************	CONSTRUCTORS / DESTRUCTORS **********************/
	/************************************************************************/

	/**
	* @brief Constructor for empty priority queue.
	*
	* @param _heapType Type of underlying heap structure, can be HeapType::MIN or HeapType::MAX
	*/
	PriorityQueue(HeapType _heapType) {
		heapType = _heapType;
	}

	/**
	* @brief Constructor for priority queue to pass data with priorities initially via a vector of pairs.
	*
	* @param _dataWithPriorityVec Vector of pairs containing data objects and corresponding priorities
	* @param _heapType Type of underlying heap structure, can be HeapType::MIN or HeapType::MAX
	*/
	PriorityQueue(const std::vector< std::pair<T, PT> >& _dataWithPriorityVec, HeapType _heapType) : dataWithPriorityVec(_dataWithPriorityVec), heapType(_heapType) {
		// heapify the priority queue instance to preserve 
		// the PQ and MBH behaviour of the data structure
		heapification();
	}

	/**
	* @brief Constructor for priority queue to pass data with priorities initially via arrays.
	*
	* @param dataArr Array of data objects
	* @param priorityArr Arrays of priorities of data objects
	* @param size Size of arrays
	* @param _heapType Type of underlying heap structure, can be HeapType::MIN or HeapType::MAX
	*/
	PriorityQueue(T* dataArr, PT* priorityArr, size_t size, HeapType _heapType) : heapType(_heapType) {
		// loop over data and priorities arrays inserting the data
		// with corresponding priority to the dataVector
		for (size_t i = 0; i < size; ++i) {
			dataWithPriorityVec.push_back(std::make_pair(dataArr[i], priorityArr[i]));
		}

		// heapify the priority queue instance to preserve 
		// the PQ and MBH behaviour of the data structure
		heapification();
	}

	/**
	* @brief Copy constructor for priority queue.
	*
	* @param copyQueue Copy target priority queue instance
	*/
	PriorityQueue(const PriorityQueue<T, PT>& copyQueue) {
		copy(copyQueue);
	}

	/**
	* @brief Destructor for priority queue.
	*
	* @remark Note that this destructor is not virtual indicating that priority queue should not be extended.
	*/
	~PriorityQueue() {
		clear();
	}

	/**************************************************************************/
	/**********************	GETTERS / SETTERS / TOSTRING **********************/
	/**************************************************************************/

	/**
	* @brief Gets the pair at a given index in the underlying vector
	*        structure in the queue NOT IN TERMS OF HEAP STORAGE ORDER!
	*
	* This method should only be used in routines to randomly access elements
	* in the priority queue (i.e. an algorithm where order is not important).
	*
	* @remark Rather than using this operator to iterate over a priority queue use
	* 	   the PriorityQueue::const_iterator and begin(), end() methods
	* @warning THIS OPERATOR DOES NOT RETURN THE PAIR AT AN ORDERED INDEX IN THE QUEUE
	* @return Pair of queue at given index of underlying vector
	*/
	const std::pair<T, PT>& at(const size_t index) const {
		return dataWithPriorityVec.at(index);
	}

	/**
	* @brief Getter for the size of the priority queue
	*
	* @return The current filled size of the priority queue
	*/
	size_t getSize() const {
		return dataWithPriorityVec.size();
	}

	/**
	* @brief Determines whether the priority queue is empty or not
	*
	* @return True if the queue is empty, false otherwise
	*/
	bool isEmpty() const {
		return dataWithPriorityVec.empty();
	}

	/**
	* @brief Peeks the top pair of the queue without dequeuing it.
	*
	* @warning Undefined behaviour if PQ is empty
	* @return Pair at top of PQ without removing it from the structure
	*/
	const std::pair<T, PT>& peekFront() const {
		return dataWithPriorityVec.at(0);
	}

	/**
	* @brief Gets the type of heap structure of the queue
	*
	* @return Type of heap, either MIN or MAX
	*/
	HeapType getHeapType() const {
		return heapType;
	}

	/**
	* @brief Sets the type of the underlying binary heap structure
	*
	* @param heap Type of underlying heap structure, can be HeapType::MIN or HeapType::MAX
	*/
	void setHeapType(HeapType heap) {

		// if heap type to be set is the same as this->heapType, do nothing
		if (heapType == heap)
			return;

		heapType = heap;

		// heapify structure based on updated type of binary heap
		heapification();

	}

	/**
	* @brief Gives a string representation of the PQ, printed in order of priority based on heap type.
	*
	* @remark This method requires that the type T of the priority queue has
	*		  a defined "overloaded" to_string(const T&) method for ADL to work.
	* @return The PQ in a string data format
	*/
	std::string toString() {

		// copy this queue to a temporary "streamed queue"
		PriorityQueue<T, PT> streamedQueue(*this);

		std::string retString = "Data\tPriority\n";

		// loop over streamed queue appending data to return string
		while (streamedQueue.getSize()) {
			std::pair<T, PT> dataPair = streamedQueue.dequeue();
			retString += to_string(dataPair.first) + "\t" + to_string(dataPair.second) + "\n";

		}

		return retString;

	}

	/**
	* @brief Saves a vector of pairs representation of the priority queue
	*
	* Returns a const reference to a vector of pairs representing the priority queue in
	* order of priorities based upon the underlying heap type of the structure.
	*
	* @warning Potentially slow if used often for large queues
	* @return std::vector of std::pair's containing ordered queue data
	*/
	std::vector< std::pair<T, PT> > saveOrderedQueueAsVector() {

		// copy this queue to a temporary queue
		PriorityQueue<T, PT> savedQueue(*this);

		std::vector< std::pair<T, PT> > vectorStore;

		// loop over queue pushing dequeued pairs back to vectorStore
		while (savedQueue.getSize()) {
			vectorStore.push_back(savedQueue.dequeue());
		}

		return vectorStore;

	}

	/**************************************************************/
	/**********************	QUEUE OPERATIONS **********************/
	/**************************************************************/

	/**
	* @brief Enqueue a data item in the PQ with "lowest" level of priority
	*
	* Inserts an item of data into the queue at end of queue (highest value of priority
	* for MIN heap queue, lowest value of priority for MAX heap queue)
	*
	* @remark If the priority queue is empty this method does nothing.
	* @param data Data item to insert into PQ
	*/
	void enqueue(const T& data) {

		if (isEmpty()) {
			return;
		}

		dataWithPriorityVec.push_back(std::make_pair(data, lastPriority()));

	}

	/**
	* @brief Enqueue a data item in the PQ with given priority.
	*
	* @param data Data item to insert into PQ
	* @param priority Priority level of item
	*/
	void enqueueWithPriority(const T& data, const PT priority) {

		size_t sizeVec = dataWithPriorityVec.size();

		// push the pair of data parameters to the end of the data vector
		dataWithPriorityVec.push_back(std::make_pair(data, priority));

		// call bubble up to sort data priorities
		bubbleUpHeap(sizeVec);

	}

	/**
	* @brief Enqueue an array of data with corresponding priorities into the PQ
	*
	* @warning The parameter arraySize must equal the size of the parameter arrays data and priority,
	*			this method does not defend against cases where this is violated - undefined behaviour may occur.
	* @param data Array of data objects to enqueue
	* @param priority Array of priorities corresponding the data array items
	* @param arraySize Size of data and priority arrays
	*/
	void enqueueWithPriority(T* data, PT* priority, size_t arraySize) {

		for (size_t i = 0; i < arraySize; ++i) {
			enqueueWithPriority(data[i], priority[i]);
		}

	}

	/**
	* @brief Enqueue a vector of data with corresponding priorities into the PQ
	*
	* @param data Vector of pairs containing data items with corresponding priorities
	*/
	void enqueueWithPriority(const std::vector< std::pair<T, PT> >& data) {

		for (typename std::vector< std::pair<T, PT> >::const_iterator iter = data.begin(); iter < data.end(); ++iter) {
			enqueueWithPriority(iter.operator*().first, iter.operator*().second);
		}

	}

	/**
	* @brief Dequeues the data item with highest priority for a MAX queue and lowest priority for a MIN queue.
	*
	* @return The dequeued item, i.e. the pair containing the data entry and its corresponding priority
	* @throw Throws out_of_range exception if queue is empty
	*/
	std::pair<T, PT> dequeue() {

		if (isEmpty())
			throw std::out_of_range("Priority queue is already empty, cannot dequeue.");

		// save a copy of the item to be dequeued to be returned
		std::pair<T, PT> dequeuedItem = dataWithPriorityVec.at(0);

		// remove the item from the heap
		removeTopOfHeap();

		return dequeuedItem;

	}

	/**
	* @brief Clears the priority queue
	*/
	void clear() {
		dataWithPriorityVec.clear();
	}

	/**
	* @brief Searches for an item in the PQ and returns the item with corresponding priority
	*
	* @todo Consider implementing a concurrent data structure with PQ (such as a set) to improve search time complexity.
	* @param item Object to search for in the priority queue
	* @return A std::pair containing the object and its corresponding priority
	* @throw Throws invalid_argument exception if item does not exist within queue
	*/
	std::pair<T, PT> search(const T& item) {

		// iterate over queue
		for (const_iterator iter = begin(); iter < end(); ++iter) {

			// data at current node equals search item, return current node
			if (iter.operator*().first == item) {
				return std::make_pair(iter.operator*().first, iter.operator*().second);
			}

		}

		// if item could not be found within queue, throw an exception
		throw std::invalid_argument("Item does not exist within priority queue.");

	}

	/**
	* @brief Searches for an item in the PQ and returns a vector of all occurrences of the item
	*		  with their corresponding priorities.
	*
	* @warning If the item does not exist within the queue then this method will return an empty vector.
	* @param item Object to search for in the priority queue
	* @return A std::vector of std::pair instances containing the objects and corresponding priorities
	*/
	std::vector< std::pair<T, PT> > searchAll(const T& item) {

		// container to store occurrences of data pairs in the queue
		std::vector< std::pair<T, PT> > occurrencesVec;

		// iterate over queue
		for (const_iterator iter = begin(); iter < end(); ++iter) {

			// data at current node equals search item, push current node to occurrences vector
			if (iter.operator*().first == item) {
				occurrencesVec.push_back(std::make_pair(iter.operator*().first, iter.operator*().second));
			}

		}

		return occurrencesVec;

	}

	/**
	* @brief Searches for a priority in the PQ and returns a pair with the first instance of an item
	*		  in the queue with the given priority.
	*
	* @param priority Priority to search for in the queue
	* @return A std::pair of data and associated priority containing the first instance where priority occurs
	* @throw Throws invalid_argument exception if priority does not exist in the queue
	*/
	std::pair<T, PT> searchByPriority(const PT priority) {

		// iterate over queue
		for (const_iterator iter = begin(); iter < end(); ++iter) {
			if (iter.operator*().second == priority)
				return std::make_pair(iter.operator*().first, iter.operator*().second);
		}

		// if priority was not found in queue, throw invalid argument exception
		throw std::invalid_argument("There is no item with the given priority in the queue.");

	}

	/**
	* @brief Searches for a priority in the PQ and returns a vector containing all pairs of data
	*		  where the priority occurred.
	*
	* @param priority Priority to search for in the queue
	* @return A std::vector of std::pair's containing all instances of data where the priority occurs
	*/
	std::vector< std::pair<T, PT> > searchByPriorityAll(const PT priority) {

		// container to store occurrences of data pairs in the queue
		std::vector< std::pair<T, PT> > occurrencesVec;

		// iterate over queue inserting data pairs to the occurrences vector where priority occurs
		for (const_iterator iter = begin(); iter < end(); ++iter) {
			if (iter.operator*().second == priority)
				occurrencesVec.push_back(std::make_pair(iter.operator*().first, iter.operator*().second));
		}

		return occurrencesVec;

	}

	/**
	* @brief Changes the priority of the first occurrence of an item in the PQ
	*
	* @param item Data item to change priority of
	* @param updatedPriority Updated priority of item
	*/
	void changePriority(const T& item, const PT updatedPriority) {

		size_t i = 0;
		bool itemFound = false;

		// loop over whole PQ
		while (i < dataWithPriorityVec.size()) {

			// if data at PQ position [i] equals parameterised item
			// update its priority and break from loop
			if (dataWithPriorityVec.at(i).first == item) {
				dataWithPriorityVec.at(i).second = updatedPriority;
				itemFound = true;
				break;
			}

			i++;

		}

		// bubble up the heap from the position of changed priority
		if (itemFound)
			bubbleUpHeap(i);

	}

	/**
	* @brief Changes the priorities of all occurrences of an item in the PQ
	*
	* @param item Data item to change priority of
	* @param updatedPriority Updated priority of item
	*/
	void changePriorityAll(const T& item, const PT updatedPriority) {

		size_t i = 0;

		// container to store positions in queue of the
		// indexes where the priorities were altered
		std::vector<size_t> changedPositions;

		// loop over whole PQ
		while (i < dataWithPriorityVec.size()) {

			// if data at PQ position [i] equals parameterised item
			// update its priority and push its position to the
			// changedPositions container ready for bubbling up
			if (dataWithPriorityVec.at(i).first == item) {
				dataWithPriorityVec.at(i).second = updatedPriority;
				changedPositions.push_back(i);
			}

			i++;

		}

		// loop over all changedPositions and bubble up the heap from each position
		for (size_t j = 0; j < changedPositions.size(); j++) {

			bubbleUpHeap(changedPositions.at(j));

		}

	}

	/**
	* @brief Exchanges the content of this container by the content of the parameterised container.
	*
	* Swaps the contents of each container - this queue becomes a "copy" of parameterised queue and
	* the parameterised queue becomes a "copy" of this queue before the swap was completed.
	*
	* @param priorityQueue A priority queue container of the same type as this queue.
	*/
	void swap(PriorityQueue<T, PT>& priorityQueue) {

		// swap underlying vector containers
		dataWithPriorityVec.swap(priorityQueue.dataWithPriorityVec);

		// swap heap types
		HeapType tempHeapType = heapType;
		setHeapType(priorityQueue.heapType);
		priorityQueue.setHeapType(tempHeapType);

	}

	/**
	* @brief Merges the content of this container with the content of parameterised container
	*		 such that the new state of this container is the sum of the original state and parameter.
	*
	* @param thatQueue A priority queue container to merge with this queue
	* @throw Throws std::invalid_argument exception if this queue and thatQueue are of different heapType
	*/
	void merge(const PriorityQueue<T, PT>& thatQueue) {

		if (heapType != thatQueue.heapType)
			throw std::invalid_argument("Cannot merge queues of different heap types.");

		// merging the same queue onto itself
		if (this == &thatQueue) {
			// save instanced copy of addPQ vec container to avoid non-incrementable
			// iterator compiler error when enqueueing contents onto this
			std::vector< std::pair<T, PT> > instancedVecCopy = thatQueue.dataWithPriorityVec;
			enqueueWithPriority(instancedVecCopy);
		}

		// else merge directly using thatQueue vec container
		else {
			enqueueWithPriority(thatQueue.dataWithPriorityVec);
		}

	}

	/****************************************************************************/
	/**********************	PUBLIC STATIC MEMBER FUNCTIONS **********************/
	/****************************************************************************/

	/**
	* @brief Merges the content of the first parameterised container with the content of
	*        of the second parameterised container and returns the merged container.
	*
	* @param firstQueue A priority queue container instance
	* @param secondQueue A priority queue container instance of same type as firstQueue
	* @return Merged priority queue containers
	* @throw Throws std::invalid_argument exception if firstQueue and secondQueue have different heap types
	*/
	static PriorityQueue<T, PT> merge(const PriorityQueue<T, PT>& firstQueue, const PriorityQueue<T, PT>& secondQueue) {

		if (firstQueue.heapType != secondQueue.heapType)
			throw std::invalid_argument("Cannot merge queues of different heap types.");

		PriorityQueue<T, PT> mergedQueue(firstQueue);

		mergedQueue.enqueueWithPriority(secondQueue.dataWithPriorityVec);

		return mergedQueue;

	}

	/******************************************************************/
	/**********************	OVERLOADED OPERATORS **********************/
	/******************************************************************/

	/**
	* @brief Gets the pair at a given index in the underlying vector
	*        structure in the queue NOT IN TERMS OF HEAP STORAGE ORDER!
	*
	* This method should only be used in routines to randomly access elements
	* in the priority queue (i.e. an algorithm where order is not important).
	*
	* @remark Rather than using this operator to iterate over a priority queue use
	* 	   the PriorityQueue::const_iterator and begin(), end() methods
	* @warning THIS OPERATOR DOES NOT RETURN THE PAIR AT AN ORDERED INDEX IN THE QUEUE
	* @return Pair of queue at given index of underlying vector
	*/
	const std::pair<T, PT>& operator[](const size_t index) const {
		return dataWithPriorityVec.at(index);
	}

	/**
	* @brief Overloaded addition operator.
	*
	* @param addPQ Addition target queue
	* @return Instance of priority queue as this queue plus addPQ
	* @throw Throws invalid_argument exception if this queue and addPQ are of different heapType
	*/
	PriorityQueue<T, PT> operator+(const PriorityQueue<T, PT>& addPQ) const {

		if (heapType != addPQ.heapType) {
			throw std::invalid_argument("Cannot add queues of different heap types.");
		}

		// create priority queue object instantiated with this queue
		PriorityQueue<T, PT> sumQueue(*this);

		// enqueue data vector of addPQ into sumQueue
		sumQueue.enqueueWithPriority(addPQ.dataWithPriorityVec);

		return sumQueue;

	}

	/**
	* @brief Overloaded assignment operator.
	*
	* @param assignPQ Assigment target queue
	* @return Instance of priority queue which is a copy of pQ
	*/
	const PriorityQueue<T, PT>& operator=(const PriorityQueue<T, PT>& assignPQ) {
		if (this != &assignPQ)
			copy(assignPQ);

		return *this;
	}

	/**
	* @brief Overloaded addition-assignment operator.
	*
	* @param addPQ Addition target queue
	* @return Instance of priority queue added and assigned to this queue
	* @throw Throws invalid_argument exception if this queue and addPQ are of different heapType
	*/
	PriorityQueue<T, PT>& operator+=(const PriorityQueue<T, PT>& addPQ) {

		if (heapType != addPQ.heapType) {
			throw std::invalid_argument("Cannot add queues of different heap types.");
		}

		// merging the same queue onto this queue
		if (this == &addPQ) {
			// save instanced copy of addPQ vec container to avoid non-incrementable
			// iterator compiler error when enqueueing contents onto this
			std::vector< std::pair<T, PT> > instancedVecCopy = addPQ.dataWithPriorityVec;
			enqueueWithPriority(instancedVecCopy);
		}

		// else merge directly using addPQ container
		else {
			enqueueWithPriority(addPQ.dataWithPriorityVec);
		}

		return *this;

	}

	/**
	* @brief Overloaded equivalence operator.
	*
	* @param chkPQ Check target queue
	* @return true if this queue and chkPQ are equivalent, false otherwise
	* @bug Differing queues sometimes give true equivalency evaluation - seems to occur when neighbouring nodes are different
	*	   but not when nodes several positions away from each other are different.
	*/
	bool operator==(const PriorityQueue<T, PT>& chkPQ) const {

		// pointer to same queue, return true
		if (this == &chkPQ)
			return true;

		// if queue sizes or heap types are different, return false
		if (getSize() != chkPQ.getSize() || heapType != chkPQ.heapType) {
			return false;
		}

		// iterate over queues
		for (const_iterator iter = begin(), iterChkPQ = chkPQ.begin(); iter < end(); ++iter, ++iterChkPQ) {

			// if any nodes of the two queues differ, return false
			if (iter.operator*().first != iterChkPQ.operator*().first || iter.operator*().second != iterChkPQ.operator*().second) {
				return false;
			}

		}

		return true;

	}

	/**
	* @brief Overloaded not-equivalent operator.
	*
	* @param chkPQ Check target queue
	* @return true if the queues are not equal, false otherwise
	*/
	bool operator!=(const PriorityQueue<T, PT>& chkPQ) const {

		return !(*this == chkPQ);

	}

	/*****************************************************************/
	/**********************	ITERATOR OPERATIONS **********************/
	/*****************************************************************/

	/**
	* @brief Begin iterator
	*
	* @return Constant iterator to beginning of queue
	*/
	const_iterator begin() const {
		return dataWithPriorityVec.begin();
	}

	/**
	* @brief End iterator
	*
	* @return Constant iterator to end of queue
	*/
	const_iterator end() const {
		return dataWithPriorityVec.end();
	}

};

/**
* @brief Overloaded stream extraction operator.
*
* @param outStream Reference to output stream
* @param targetQueue Instance of priority queue to write to output stream
* @return Reference to output stream containing target queue data
*/
template<typename Type, typename PriorityType> std::ostream& operator<<(std::ostream& outStream, const PriorityQueue<Type, PriorityType>& targetQueue) {

	PriorityQueue<Type, PriorityType> streamedQueue(targetQueue);

	// loop over queue sending queue data to output stream
	while (streamedQueue.getSize()) {
		std::pair<Type, PriorityType> dataPair = streamedQueue.dequeue();
		outStream << to_string(dataPair.first) << "\t" << to_string(dataPair.second) << "\n";
	}

	return outStream;

}

/**
* @brief Overloaded stream insertion operator.
*
* @warning Untested, use at your own risk!
* @param inStream Reference to input stream
* @param targetQueue Instance of priority queue to insert to input stream
* @return Reference to input stream containing target queue data
*/
template<typename Type, typename PriorityType> std::istream& operator>>(std::istream& inStream, const PriorityQueue<Type, PriorityType>& targetQueue) {

	PriorityQueue<Type, PriorityType> streamedQueue(targetQueue);

	while (streamedQueue.getSize()) {
		std::pair<Type, PriorityType> dataPair = streamedQueue.dequeue();
		inStream >> to_string(dataPair.first) >> "\t" >> to_string(dataPair.second) >> "\n";
	}

	return inStream;

}

#endif




