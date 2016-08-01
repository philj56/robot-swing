/**
 * \file priority_queue.h
 *
 * \brief File containing crsc::priority_queue implementation.
 *
 * Use this priority queue rather than the version found in PriorityQueue.h, this is
 * a much improved structure which is more efficient, robust and supports any arbitrary
 * comparator type allowing use of any strict weak ordering function-object of the user's choice. 
 *
 * The rest of the code in this machine learning directory is not currently compatible with this
 * version of the priority queue however it will be trivial to refactor...
 *
 * Wherever a `PriorityQueue<int, double>` appears (representing action-qvalue pairs) replace it
 * with a `crsc::priority_queue<std::pair<int,double>,...>`, when searching for a specific action in
 * a `crsc::priority_queue` use `pq.find(action_search)` where `pq` is the priority queue instance
 * and `action_search` is a functor or function-object (or lambda if >=C++11 is available) determining
 * the action to search for. When altering the Q-Value of an action in `pq` use `pq.alter(newQ, action_search)`
 * where `newQ` is new Q-Value to alter to and `action_search` is the action corresponding to Q-Value to alter.
 *
 * Note also that if you want to order the priority queue using the Q-Value as priorities in descending order, as
 * is the case with current code, then you must declare the priority queue as follows (for example):
 *
 * \code{.cpp}
 *  template<typename _Ty1, typename _Ty2>
 * 	struct qvalue_less {
 *	    bool operator()(const std::pair<_Ty1,_Ty2>& lhs, const std::pair<_Ty1,_Ty2>& rhs) const {
 *	 	    return lhs.second < rhs.second;
 *	    }
 *	};
 *  crsc::priority_queue<std::pair<int,double>, qvalue_less<int,double> > pq; 
 * \endcode
 *
 * ALTERNATIVELY: Use the aqv_priority_queue class in file "aqv_priority_queue.h" which is a template specialisation
 *                of crsc::priority_queue specifically for an action-qvalue queue with ordering based on maximum 
 *                qvalue (i.e. constant time lookup for maximum q-value action-qvalue pair in the structure).
 *
 * --------------------------------------------------------------------------------------------------------------------------
 * IMPORTANT: IF >=C++11 COMPILER IS BEING USED THEN DO NOT USE THIS VERSION OF `crsc::priority_queue`, INSTEAD USE THE
 *            VERSION AT `https://github.com/SJR276/crescent_library/blob/master/crescent_library/priority_queue.h`. This
 *            is important as the version in the link above is compliant with non-copyable types such as `std::unique_ptr`
 *            whereas this version is not, additionally the version in the above link is slightly more robust due to explicit
 *            const-correctness and noexcept specifications allowed by C++11 onwards. Note, however, that the version in the
 *            link above is NOT compliant with C++03 or earlier - if you can use a modern C++ compiler (linked priority_queue
 *            was tested in GCC 6.1.0 and MSVC 2015) then you should definitely do so.
 *
 */
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H
#include <algorithm>
#include <ostream>
#include <set>
#include <vector>
#include <utility>

namespace crsc {
	/**
	 * \class priority_queue
	 *
	 * \brief A container adaptor (wrapping a `std::vector` as the heap storage) that provides constant
	 *        time lookup of the largest(by default) element, at the expense of logarithmic insertion
	 *        and extraction.
	 *
	 * The element priorities are compared by default using `std::less<_Ty>` such that the largest element is always
	 * at the top of the heap, this comparator can be altered as a template argument to any other `Compare` type such
	 * as `std::greater<_Ty>` which would define minimum binary heap behaviour such that the smallest element is
	 * always on the top of the heap (providing constant lookup for the smallest item).
	 *
	 * Unlike `std::priority_queue` this container only allows the wrapping of a `std::vector` as the underlying
	 * heap container. However, this version of a priority queue provides methods to search for items in the container,
	 * alter item values, clear the container and iterate over the container with `const_iterator`s which the 
	 * STL priority queue does not provide. In addition, similarly to `std::priority_queue`, every method is guaranteed
	 * to preserve the class invariant such that the heap is not invalidated at any point between method calls.
	 *
	 * \tparam _Ty The type of the elements.
	 * \tparam _Pr A `Compare` type providing a strict weak ordering, defaults to `std::less<_Ty>`.
	 * \invariant The heap (whose ordering/behaviour is defined by the comparator `_Pr`) shall never be invalidated
	 *            between method calls, and if any exceptions are thrown by a method the heap shall never be left in
	 *            a state which would invalidate the heap.
	 * \author Samuel Rowlinson
	 * \date July, 2016
	 */
	template<typename _Ty,
		class _Pr = std::less<_Ty>
	> class priority_queue {
	public:
		// PUBLIC API TYPE DEFINITIONS
		typedef _Ty value_type;
		typedef _Ty& reference;
		typedef const _Ty& const_reference;
		typedef _Ty* pointer;
		typedef const _Ty* const_pointer;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;
		typedef typename std::vector<_Ty>::const_iterator const_iterator;
		typedef typename std::vector<_Ty>::const_reverse_iterator const_reverse_iterator;
		// CONSTRUCTION/ASSIGNMENT
		/**
		 * \brief Default constructor, initialises empty container with optional comparator argument.
		 *
		 * \param compare Comparator function-object to initialise underlying comparison functor.
		 * \complexity Complexity of construction of `compare` (typically constant).
		 * \exceptionsafety No-throw guarantee if `_Pr()` does not throw, otherwise dependent upon
		 *                  exception safety of `compare`.
		 */
		explicit priority_queue(const _Pr& compare = _Pr())
			: heap_vec(), comp(compare) {}
		/**
		 * \brief Constructs the container with contents of `_vec`.
		 *
		 * \param _vec Container to initialise contents with.
		 * \param compare Comparator function object to initialise underlying comparison functor.
		 * \complexity Linear in `_vec.size()` multiplied by logarithmic in `_vec.size()` plus
		 *             an additional linear in `_vec.size()` for vector copy.
		 * \exceptionsafety No-throw guarantee if `_Pr()` does not throw, otherwise dependent upon
		 *                  exception safety of `compare`.
		 */
		explicit priority_queue(const std::vector<value_type>& _vec, const _Pr& compare = _Pr())
			: heap_vec(_vec), comp(compare) { heapify(); }
		/**
		 * \brief Constructs the container with the contents of the range `[first, last)`.
		 *
		 * \param first Beginning of range to copy elements from.
		 * \param last End of range to copy elements from.
		 * \param compare Comparator function object to initialise underlying comparison functor.
		 * \complexity Linear in distance between `first` and `last` plus linear in this distance multiplied
		 *             logarithmic in this distance.
		 * \exceptionsafety No-throw guarantee if `_Pr()` does not throw, otherwise dependent upon
		 *                  exception safety of `compare`.
		 */
		template<class InputIt>
		priority_queue(InputIt first, InputIt last, const _Pr& compare = _Pr())
			: heap_vec(first, last), comp(compare) { heapify(); }
		/**
		 * \brief Constructs the container with a copy of the contents of `_other`.
		 *
		 * \param _other Container to use as data source to initialise this container with.
		 * \complexity Linear in the size of `_other`.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		priority_queue(const priority_queue& _other) 
			: heap_vec(_other.heap_vec), comp(_other.comp) {}
		/**
		 * \brief Destructs the container. The destructors of the elements are called
		 *        used storage is deallocated.
		 *
		 * \complexity Linear in the size of the container.
		 * \exceptionsafety No-throw guarantee, implicitly `noexcept` specification.
		 */
		~priority_queue() {}
		/**
		 * \brief Copy-assignment operator, replaces the contents of the container with
		 *        the a copy of the contents of `_other`.
		 *
		 * \param _other Another `priority_queue` container to be used as data source.
		 * \return `*this`.
		 * \complexity Linear in the size of the `_other`.
		 */
		priority_queue& operator=(const priority_queue& _other) {
			if (this != &_other)
				priority_queue(_other).swap(*this);
			return *this;
		}
		// CAPACITY
		/**
		 * \brief Checks whether the container is empty.
		 * 
		 * \return `true` if empty, `false` otherwise.
		 * \complexity Constant.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		bool empty() const {
			return heap_vec.empty();
		}
		/**
		 * \brief Returns the number of elements in the container.
		 *
		 * \return Number of elements in the container.
		 * \complexity Constant.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		size_type size() const {
			return heap_vec.size();
		}
		/**
		 * \brief Returns the maximum number of elements the container is able
		 *        to hold due to system or library implementation limitations.
		 *
		 * \return Maximum number of elements.
		 * \complexity Constant.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		size_type max_size() const {
			return heap_vec.max_size();
		}
		// ELEMENT ACCESS
		/**
		 * \brief Accesses the top element of the container without popping it.
		 *
		 * \return const_reference to top element of the container.
		 * \complexity Constant.
		 * \exceptionsafety If `!empty()` then no-throw guarantee, otherwise 
		 *                  undefined behaviour.
		 */
		const_reference top() const {
			return heap_vec[0];
		}
		/**
		 * \brief Finds the first instance of an element in the container.
		 *
		 * \param _val Value to search for in the container.
		 * \return const_iterator pointing to found element, `crsc::priority_queue::cend()` if not found.
		 * \complexity Linear in the size of the container.
		 * \exceptionsafety No-throw guarantee.
		 */
		const_iterator find(const value_type& _val) const {
			return std::find(heap_vec.begin(), heap_vec.end(), _val);
		}
		/**
		 * \brief Finds all instances of an element in the container.
		 *
		 * \param _val Value to search for in the container.
		 * \return `std::set<const_iterator>` containing `const_iterator`s to each item with value
		 *         `_val` in the container.
		 * \complexity Linear in the size of the container.
		 * \exceptionsafety Strong-guarantee, if an exception is thrown there are no changes
		 *                  in the container.
		 */
		std::set<const_iterator> find_all(const value_type& _val) const {
			std::set<const_iterator> it_set;
			for (const_iterator it = heap_vec.begin(); it < heap_vec.end(); ++it) {
				if (*it == _val) it_set.insert(it);
			}
			return it_set;
		}
		/**
		 * \brief Find the first instance of an element in the container based on
		 *        a unary predicate.
		 *
		 * \param _p Unary predicate which returns `true` for the required element.
		 * \return `const_iterator` pointing to found element, `crsc::priority_queue::cend()` if not found.
		 * \complexity Linear in the size of the container.
		 * \exceptionsafety No-throw guarantee.
		 */
		template<class UnaryPredicate>
		const_iterator find(UnaryPredicate _pred) const {
			return std::find_if(heap_vec.begin(), heap_vec.end(), _pred);
		}
		/**
		 * \brief Finds all instances of an element in the container based on
		 *        a unary predicate.
		 *
		 * \param _p Unary predicate which returns `true` for the required element.
		 * \return `std::set<const_iterator>` containing `const_iterator`s to each item in the
		 *         container which satisfies the predicate `_p`.
		 * \complexity Linear in the size of the container.
		 * \exceptionsafety Strong-guarantee, if an exception is thrown there are no changes
		 *                  in the container.
     	         */
		template<class UnaryPredicate>
		std::set<const_iterator> find_all(UnaryPredicate _pred) const {
			std::set<const_iterator> it_set;
			for (const_iterator it = heap_vec.begin(); it < heap_vec.end(); ++it) {
				if (_pred(*it)) it_set.insert(it);
			}
			return it_set;
		}
		/**
		 * \brief Writes the contents of the container to a `std::ostream` in heap-order.
		 *
		 * \param _os Instance of `std::ostream` to write to.
		 * \param _delim Delimiter for separation of container elements in stream.
		 * \return Reference to modified `_os`.
		 * \complexity Linear in the size of the container.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		std::ostream& write(std::ostream& _os, char _delim = ' ') const {
			priority_queue<value_type, _Pr> tmp(*this);
			while (!tmp.empty()) {
				_os << tmp.top() << _delim;
				tmp.dequeue();
			}
			return _os;
		}
		// MODIFIERS
		/**
		 * \brief Pushes an item into the container and sorts it.
		 *
		 * \param _val Value to enqueue into the container.
		 * \complexity Logarithmic in the size of the container plus amortized
		 *             constant for `std::vector::push_back` operation.
		 * \exceptionsafety Strong-guarantee, if an exception is thrown there are no changes
		 *                  in the container.
		 */
		void enqueue(const value_type& _val) {
			heap_vec.push_back(_val);
			bubble_up(heap_vec.size()-1);
		}
		/**
		 * \brief Pops the top item from the container.
		 * 
		 * \complexity Logarithmic in the size of the container.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification - even
		 *                  if container is `empty()` in which case `dequeue()`
		 *                  does nothing.
		 */
		void dequeue() {
			pop_top();
		}
		/**
		 * \brief Clears all items from the container.
		 *
		 * \complexity Linear in the size of the container.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		void clear() {
			heap_vec.clear();
		}
		/**
		 * \brief Alters the element value at the specified iterator position in the container
		 *        to the value `_alter_to_val` by copy-assignment.
		 *
		 * \param _pos `const_iterator` to element in container to alter.
		 * \param _alter_to_val Value to assign to element to alter.
		 * \return `const_iterator` to next valid position in container after altering.
		 * \complexity Logarithmic in the size of the container.
		 * \exceptionsafety Strong-guarantee, if an exception is thrown there are no changes
		 *                  in the container.
		 */
		const_iterator alter(const_iterator _pos, const value_type& _alter_to_val) {
			difference_type index = std::distance<const_iterator>(heap_vec.begin(), _pos);
			bool b_up = comp(heap_vec[index], _alter_to_val);
			heap_vec[index] = _alter_to_val;
			b_up ? bubble_up(index) : bubble_down(index);
			return _pos;
		}
		/**
		 * \brief Alters the first instance of the specified value `_val_find`
		 *        in the container to `_alter_to_val` by copy-assignment. If 
		 *        `_val_find` does not exist in the container, this method does
		 *        nothing.
		 *
		 * \param _val_find Value to search for and alter in the container.
		 * \param _alter_to_val Value to assign search item to.
		 * \complexity Linear in the size of the container plus logarithmic
		 *             in the size of the container.
		 * \exceptionsafety Strong-guarantee, if an exception is thrown there are no changes
		 *                  in the container.
		 */
		void alter(const std::pair<value_type, value_type>& _tgt_alt) {
			typename std::vector<value_type>::iterator it = std::find(heap_vec.begin(), heap_vec.end(), _tgt_alt.first);
			if (it != heap_vec.end()) {
				*it = _tgt_alt.second;
				difference_type index = std::distance(heap_vec.begin(), it); // index of changed element
				comp(_tgt_alt.first, _tgt_alt.second) ? bubble_up(index) : bubble_down(index);
			}
		}
		/**
		 * \brief Alters the first instance of an item in the container for
		 *        which the unary predicate `_p` is satisfied to `_alter_to_val`
		 *        by copy-assignment. If there are no such items in the container
		 *        which satisfy `_p` then this method does nothing.
		 *
		 * \param _alter_to_val Value to assign search item to.
		 * \param _p Unary predicate which returns `true` for the required element.
		 * \complexity Linear in the size of the container plus logarithmic
		 *             in the size of the container.
		 * \exceptionsafety Strong-guarantee, if an exception is thrown there are no changes
		 *                  in the container.
		 */
		template<class UnaryPredicate>
		void alter(const value_type& _alter_to_val, UnaryPredicate _pred) {
			typename std::vector<value_type>::iterator it = std::find_if(heap_vec.begin(), heap_vec.end(), _pred);
			if (it != heap_vec.end()) {
				bool b_up = comp(*it, _alter_to_val);
				*it = _alter_to_val;
				difference_type index = std::distance(heap_vec.begin(), it); // index of changed element
				b_up ? bubble_up(index) : bubble_down(index);
			}
		}
		/**
		 * \brief Alters all instances of the specified value `_val_find` in the container
		 *        to `_alter_to_val` by copy-assignment. If `_val_find` does not exist in
		 *        the container, this method does nothing.
		 *
		 * \param _val_find Value to search for and alter in the container.
		 * \param _alter_to_val Value to assign each instance of `_val_find` to.
		 * \complexity Linear in the size of the container plus the number of
		 *             occurrences of `_val_find` in the container multiplied by
		 *             logarithmic in the size of the container.
		 * \exceptionsafety Strong-guarantee, if an exception is thrown there are no changes
		 *                  in the container.
		 */
		void alter_all(const std::pair<value_type, value_type>& _target_alter) {
			for (typename std::vector<value_type>::iterator it = heap_vec.begin(); it < heap_vec.end(); ++it) {
				if (*it == _target_alter.first) *it = _target_alter.second; 
			}
			heapify();
		}
		/**
		 * \brief Alters all instances of items in the container for which the unary 
		 *        predicate `_p` is satisifed to `_alter_to_val` by copy-assignment. If
		 *        there are no such items in the container which satisfy `_p` then this
		 *        method does nothing.
		 *
		 * \param _alter_to_val Value to assign each `_p` satisfied item to.
		 * \param _p Unary predicate which returns `true` for the required elements.
		 * \complexity Linear in the size of the container plus the number of
		 *             items for which `_p` is satisfied in the container multiplied
		 *             by logarithmic in the size of the container.
		 * \exceptionsafety Strong-guarantee, if an exception is thrown there are no changes
		 *                  in the container.
		 */
		template<class UnaryPredicate>
		void alter_all(const value_type& _alter_to_val, UnaryPredicate _pred) {
			for (typename std::vector<value_type>::iterator it = heap_vec.begin(); it < heap_vec.end(); ++it) {
				if (_pred(*it)) *it = _alter_to_val;
			}
			heapify();
		}
		/** 
		 * \brief Exchanges the contents of the container with those of `_other`. Does not
		 *        cause references and iterators to associate with the other container.
		 *
		 * \param _other `priority_queue` container to swap with.
		 * \complexity Constant.
		 */
		void swap(priority_queue& _other) {
			heap_vec.swap(_other.heap_vec);
			std::swap(comp, _other.comp);
		}
		// ITERATORS
		/**
		 * \brief Returns a const_iterator the first element of the container.
		 *
		 * \remark If the container is empty, the return value will be equal to `cend()`.
		 * \return Constant iterator to the first element.
		 * \complexity Constant.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		const_iterator cbegin() const {
			return heap_vec.begin();
		}
		/**
		 * \brief Returns a const_iterator to the past-the-end element of the container.
		 *
		 * \return Constant iterator to the past-the-end element.
		 * \complexity Constant.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		const_iterator cend() const {
			return heap_vec.end();
		}
		/**
		 * \brief Returns a const_reverse_iterator to the first element of the reversed container. It
		 *        corresponds to the last element of the non-reversed container.
		 *
		 * \return Constant reverse iterator to the first element.
		 * \complexity Constant.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		const_reverse_iterator crbegin() const {
			return heap_vec.rbegin();
		}
		/**
		 * \brief Returns a const_reverse_iterator to the past-the-end element of the reversed container. It
		 *        corresponds to the element preceding the first element of the non-reversed container.
		 *
		 * \return Constance reverse iterator to the past-the-end element.
		 * \complexity Constant.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		const_reverse_iterator crend() const {
			return heap_vec.rend();
		}
	private:
		std::vector<value_type> heap_vec;	// underlying heap container
		_Pr comp; 	// comparator function-object, determines priorities
		/**
		 * \brief Bubbles down the heap from a given vector index, performing
		 *        swaps based on comparator conditions.
		 *
		 * \param _pos Index to perform bubbling down from.
		 * \complexity Logarithmic in the size of the container.
		 * \exceptionsafety If `_pos < size()` then no-throw guarantee, otherwise
		 *                  undefined behaviour.
		 */
		void bubble_down(size_type _pos) {
			size_type left_child = 2*_pos + 1; // index of left child node relative to parent
			size_type right_child = 2*_pos + 2; // index of right child node relative to parent
			if (left_child >= heap_vec.size()) return; 
			size_type min_pos = _pos;
			// perform comparison between values of heap at min_pos and left node pos
			// and set min_pos to position of left node if comparison yields true
			if (comp(heap_vec[_pos], heap_vec[left_child]))
				min_pos = left_child;
			// perform comparison between values of heap at min_pos and right node pos
			// and set min_pos to position of right node if comparison yields true
			if (right_child < heap_vec.size() && comp(heap_vec[min_pos], heap_vec[right_child]))
				min_pos = right_child;
			// if required, perform swap and bubble down from swapped min_pos
			if (min_pos != _pos) {
				std::swap(heap_vec[_pos], heap_vec[min_pos]);
				bubble_down(min_pos);
			}
		}
		/**
		 * \brief Bubbles up the heap from a given vector index, performing
		 *        swaps based on comparator conditions.
		 *
		 * \param _pos Index to perform bubbling up from.
		 * \complexity Logarithmic in the size of the container.
		 * \exceptionsafety If `_pos < size()` then no-throw guarantee, otherwise
		 *                  undefined behaviour.
		 */
		void bubble_up(size_type _pos) {
			// at top of heap, exit
			if (!_pos) return;
			// position of parent node relative to a child node in heap_vec
			size_type parent = (_pos - 1)/2;
			// perform comparison between heap values at parent and _pos
			// and do swap and bubble up from parent if true
			if (comp(heap_vec[parent], heap_vec[_pos])) {
				std::swap(heap_vec[_pos], heap_vec[parent]);
				bubble_up(parent);
			}
		}
		/**
		 * \brief Removes the top item from the heap and bubbles down from new top.
		 *
		 * \complexity Logarithmic in the size of the container.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		void pop_top() {
			size_type heap_size = heap_vec.size();
			if (!heap_size) return;
			// set top of heap to last heap element then remove last heap element
			heap_vec[0] = heap_vec[--heap_size];
			heap_vec.pop_back();
			// bubble down from top to get previously last heap element to correct index
			bubble_down(0);
		}
		/**
		 * \brief Performs heapification of entire binary heap, bubbling down from
		 *        each index such that binary heap invariant is guaranteed.
		 *
		 * \complexity Linear in the size of the container multiplied by
		 *             logarithmic in the size of the container.
		 * \exceptionsafety No-throw guarantee, `noexcept` specification.
		 */
		void heapify() {
			for (int i = heap_vec.size() - 1; i > -1; --i)
				bubble_down(i);
		}
	};
	/**
	 * \brief Stream insertion operator for `crsc::priority_queue`, uses
	 *        ' ' delimiter by default for element separation.
	 *
	 * \param _os Instance of `std::ostream` to write to.
	 * \param _pq `crsc::priority_queue` container to write to stream.
	 * \return Modified reference to `_os`.
	 * \complexity Linear in the size of `_pq`.
	 * \exceptionsafety No-throw guarantee, `noexcept` specification.
	 */
	template<typename _Ty,
		//class _Pr = std::less<_Ty>
		class _Pr
	> std::ostream& operator<<(std::ostream& _os, const priority_queue<_Ty, _Pr>& _pq) {
		return _pq.write(_os);
	}
}

#endif //!PRIORITY_QUEUE_H