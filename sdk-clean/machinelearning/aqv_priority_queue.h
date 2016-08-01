/**
 * \file aqv_priority_queue.h
 * 
 * \brief File containing aqv_priority_queue implementation.
 *
 * This is a convenient template-specialisation of `crsc::priority_queue` for use in the Machine
 * Learning library of this project. It provides constant time lookup of the maximum q-value action-qvalue
 * pair in the structure as well as methods for altering action-qvalue pairs based on values or unary
 * predicates. Constant iteration is supported via `aqv_priority_queue::const_iterator` and the reverse.
 *
 * \warning aqv_priority_queue is not as thoroughly tested as the generic `crsc::priority_queue` class from 
 *          which it is "composed" (using composition rather than inheritance), some methods may require minor
 *          tweaks.
 */
#ifndef AQV_PRIORITY_QUEUE_H
#define AQV_PRIORITY_QUEUE_H
#include "priority_queue.h"

/**
 * \struct qvalue_less 
 *
 * \brief Custom comparator for q-values, provides strict weak ordering in descending order.
 *
 * \tparam _TyA The type of actions.
 * \tparam _TyQ The type of q-values.
 */
template<typename _TyA,
	typename _TyQ
> struct qvalue_less {
	bool operator()(const std::pair<_TyA,_TyQ>& lhs, const std::pair<_TyA,_TyQ>& rhs) const {
		return lhs.second < rhs.second;
	}
};

/**
 * \class aqv_priority_queue
 *
 * \brief Template specialisation of `crsc::priority_queue` providing a convenient interface
 *        for manipulating a maximum binary heap of action-qvalue pairs to be used in a 
 *        Q-Learning machine learning library.
 *
 * \tparam _TyA The type of the actions.
 * \tparam _TyQ The type of the q-values.
 * \author Samuel Rowlinson
 * \date July 2016
 */
template<typename _TyA,
	typename _TyQ
> class aqv_priority_queue {
public:
	// PUBLIC API TYPE DEFINITIONS
	typedef _TyA action_type;
	typedef _TyQ qvalue_type;
	typedef typename std::pair<_TyA, _TyQ> value_type;
	typedef typename crsc::priority_queue<value_type, qvalue_less<_TyA, _TyQ> > action_qvalue_priority_queue;
	typedef typename action_qvalue_priority_queue::const_reference const_reference;
	typedef typename action_qvalue_priority_queue::size_type size_type;
	typedef typename action_qvalue_priority_queue::const_iterator const_iterator;
	typedef typename action_qvalue_priority_queue::const_reverse_iterator const_reverse_iterator;
private:
	// FUNCTION-OBJECTS FOR FINDING ACTION/QVALUE
	/**
	 * \struct action_find
	 *
	 * \brief Function-object used to find a specific action within
	 *        an action-qvalue pair.
	 */
	struct action_find {
		action_find(const action_type& _action) : action(_action) {}
		bool operator()(const value_type& _val) const {
			return _val.first == action;
		}
	private:
		action_type action;
	};
	/**
	 * \struct qvalue_find
	 *
	 * \brief Function-object used to find a specific qvalue within
	 *        an action-qvalue pair.
	 */
	struct qvalue_find {
		qvalue_find(const qvalue_type& _qvalue) : qvalue(_qvalue) {}
		bool operator()(const value_type& _val) const {
			return _val.second == qvalue;
		}
	private:
		qvalue_type qvalue;
	};
public:
	// CONSTRUCTION/ASSIGNMENT
	/**
	 * \brief Default constructor, initialises empty `aqv_priority_queue`.
	 */
	aqv_priority_queue() : pq() {}
	/**
	 * \brief Constructs the container with contents of `_vec`.
	 *
	 * \param _vec Container to initialise contents with.
	 */
	explicit aqv_priority_queue(const std::vector<value_type>& _vec)
		: pq(_vec) {}
	/**
	 * \brief Constructs the container with the contents of the range `[first, last)`.
	 *
	 * \param first Beginning of the range to copy elements from.
	 * \param last End of range to copy element from.
	 */
	template<class InputIt>
	aqv_priority_queue(InputIt first, InputIt last)
		: pq(first, last) {}
	/**
	 * \brief Constructs the container with a copy of the contents of `_other`.
	 *
	 * \param _other Container to use as data source to initialise this container with.
	 */
	aqv_priority_queue(const aqv_priority_queue& _other)
		: pq(_other.pq) {}
	/**
	 * \brief Destructs the container. The destructors of the elements are called and
	 *        used storage is deallocated.
	 */
	~aqv_priority_queue() {}
	/**
	 * \brief Copy-assignment operator, replaces the contents of the container with a copy
	 *        of the contents of `_other`.
	 *
	 * \param _other Another `aqv_priority_queue` container to be used as data source.
	 */
	aqv_priority_queue& operator=(const aqv_priority_queue& _other) {
		if (this != &_other)
			aqv_priority_queue(_other).swap(*this);
		return *this;
	}
	// CAPACITY
	bool empty() const {
		return pq.empty();
	}
	size_type size() const {
		return pq.size();
	}
	size_type max_size() const {
		return pq.max_size();
	}
	// ELEMENT ACCESS
	const_reference top() const {
		return pq.top();
	}
	const_iterator find(const value_type& _val) const {
		return pq.find(_val);
	}
	std::set<const_iterator> find_all(const value_type& _val) const {
		return pq.find_all(_val);
	}
	template<class UnaryPredicate>
	const_iterator find(UnaryPredicate _pred) const {
		return pq.find(_pred);
	}
	template<class UnaryPredicate>
	std::set<const_iterator> find_all(UnaryPredicate _pred) const {
		return pq.find_all(_pred);
	}
	/**
	 * \brief Finds the first instance of an element in the container with
	 *        action value corresponding to `_action`.
	 *
	 * \param _action Action to search for in the container.
	 * \return `const_iterator` to found element.
	 */
	const_iterator find_by_action(const action_type& _action) const {
		return pq.find(action_find(_action));
	}
	/**
	 * \brief Finds all instances of elements in the container with action
	 *        values corresponding to `_action`.
	 *
	 * \param _action Action to search for in the container.
	 * \return `std::set` containing `const_iterator`s to all elements found.
	 */
	std::set<const_iterator> find_all_by_action(const action_type& _action) const {
		return pq.find_all(action_find(_action));
	}
	/**
	 * \brief Finds the first instance of an element in the container with 
	 *        qvalue corresponding to `_qvalue`.
	 *
	 * \param _qvalue Qvalue to search for in the container.
	 * \return `const_iterator` to found element.
	 */
	const_iterator find_by_qvalue(const qvalue_type& _qvalue) const {
		return pq.find(qvalue_find(_qvalue));
	}
	/**
	 * \brief Finds all instances of elements in the container with qvalues
	 *        corresponding to `_qvalue`.
	 *
	 * \param _qvalue Qvalue to search for in the container.
	 * \return `std::set` containing `const_iterator`s to all elements found.
	 */
	std::set<const_iterator> find_all_by_qvalue(const qvalue_type& _qvalue) const {
		return pq.find_all(qvalue_find(_qvalue));
	}
	std::ostream& write(std::ostream& _os, char _delim = '\n') const {
		aqv_priority_queue<action_type, qvalue_type> tmp(*this);
		while (!tmp.empty()) {
			_os << tmp.top().first << ' ' << tmp.top().second << _delim;
			tmp.dequeue();
		}
		return _os;
	}
	// MODIFIERS
	void enqueue(const value_type& _val) {
		pq.enqueue(_val);
	}
	void dequeue() {
		pq.dequeue();
	}
	void clear() {
		pq.clear();
	}
	/**
	 * \brief Alters the element value at the specified iterator position in
	 *        the container to the value `_alter_to_val` by copy-assignment.
	 *
	 * \param _pos `const_iterator` to element in container to alter.
	 * \param _alter_to_val Value to assign to element to alter.
	 * \return `const_iterator` to next valid position in container after altering.
	 */
	const_iterator alter(const_iterator _pos, const value_type& _alter_to_val) {
		return pq.alter(_pos, _alter_to_val);
	}
	void alter(const std::pair<value_type,value_type>& _tgt_alt) {
		pq.alter(_tgt_alt);
	}
	void alter_all(const std::pair<value_type,value_type>& _tgt_alt) {
		pq.alter_all(_tgt_alt);
	}
	template<class UnaryPredicate>
	void alter(const value_type& _alter_to_val, UnaryPredicate _pred) {
		pq.alter(_alter_to_val, _pred);
	}
	template<class UnaryPredicate>
	void alter_all(const value_type& _alter_to_val, UnaryPredicate _pred) {
		pq.alter_all(_alter_to_val, _pred);
	}
	/**
	 * \brief Alters the first instance of an element in the container with 
	 *        an action value `_action` to `_alter_to_val`.
	 *
	 * \param _action Action to search for.
	 * \param _alter_to_val Value to assign to element to alter.
	 */
	void alter_by_action(const action_type& _action, const value_type& _alter_to_val) {
		pq.alter(_alter_to_val, action_find(_action));
	}
	/**
	 * \brief Alters all instances of elements in the container with 
	 *        action values `_action` to `_alter_to_val`.
	 *
	 * \param _action Action to search for.
	 * \param _alter_to_val Value to assign to elements to alter.
	 */
	void alter_all_by_action(const action_type& _action, const value_type& _alter_to_val) {
		pq.alter_all(_alter_to_val, action_find(_action));
	}
	/**
	 * \brief Alters just the action of the first instance of an element in
	 *        the container with action value `_tgt_alt.first` to the second
	 *        field of `_tgt_alt`.
	 *
	 * \param _tgt_alt First field represents action to search for, second
	 *        gives value to change the action to.
	 */
	void alter_action(const std::pair<action_type,action_type>& _tgt_alt) {
		const_iterator search = find_by_action(_tgt_alt.first);
		pq.alter(search, std::make_pair(_tgt_alt.second, search->second));
	}
	/**
	 * \brief Alters the first instance of an element in the container with 
	 *        a q-value `_qvalue` to `_alter_to_val`.
	 *
	 * \param _action Qvalue to search for.
	 * \param _alter_to_val Value to assign to element to alter.
	 */
	void alter_by_qvalue(const qvalue_type& _qvalue, const value_type& _alter_to_val) {
		pq.alter(_alter_to_val, qvalue_find(_qvalue));
	}
	/**
	 * \brief Alters all instances of elements in the container with 
	 *        q-values `_qvalue` to `_alter_to_val`.
	 *
	 * \param _action Qvalue to search for.
	 * \param _alter_to_val Value to assign to elements to alter.
	 */
	void alter_all_by_qvalue(const qvalue_type& _qvalue, const value_type& _alter_to_val) {
		pq.alter_all(_alter_to_val, qvalue_find(_qvalue));
	}
	/**
	 * \brief Alters just the qvalue of the first instance of an element in
	 *        the container with q-value `_tgt_alt.first` to the second
	 *        field of `_tgt_alt`.
	 *
	 * \param _tgt_alt First field represents qvalue to search for, second
	 *        gives value to change the q-value to.
	 */
	void alter_qvalue(const std::pair<qvalue_type,qvalue_type>& _tgt_alt) {
		const_iterator search = find_by_qvalue(_tgt_alt.first);
		pq.alter(search, std::make_pair(search->first, _tgt_alt.second));
	}
	void swap(aqv_priority_queue& _other) {
		pq.swap(_other.pq);
	}
	// ITERATORS
	const_iterator cbegin() const {
		return pq.cbegin();
	}
	const_iterator cend() const {
		return pq.cend();
	}
	const_reverse_iterator crbegin() const {
		return pq.crbegin();
	}
	const_reverse_iterator crend() const {
		return pq.crend();
	}
private:
	action_qvalue_priority_queue pq;
};

template<typename _TyA,
	typename _TyQ
> std::ostream& operator<<(std::ostream& os, const aqv_priority_queue<_TyA, _TyQ>& pq) {
	pq.write(os);
	return os;
}


#endif //!AQV_PRIORITY_QUEUE_H