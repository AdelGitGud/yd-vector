#ifndef VECTOR_H
#define VECTOR_H
#include <stdlib.h>
#include <iostream>

//#define CHECK_OUT_OF_RANGE_ERR

// Default malloc size
#define DEFAULT_ALLOC 5

// Returns bigger of two arguments.
#define MAX(A, B) ((A) > (B) ? (A) : (B))

// Reserve this amount of extra memory with push_back.
// Always at least one.
#define ALLOC_LEAP(CURRSIZE) (MAX(CURRSIZE/2, 1))

namespace yd{

#ifdef CHECK_OUT_OF_RANGE_ERR
	class Vector_Index_Out_of_Range : public std::exception {
	};

	inline void VECOUTOFRANGE() {
		throw Vector_Index_Out_of_Range();
	}
#endif

	// Returned from find
	template <typename T, typename T2>
	struct Pair{
		T first;
		T2 second;
		Pair(const T& firstElement, const T2& secondElement) : first(firstElement), second(secondElement) {};
		Pair() {};
	};


template<typename T>
class Vector
{
public:
	Vector<T>();
	
	// Array of elements as an argument
	template<typename T2, size_t SIZE>
	Vector(T2(&arr)[SIZE]);
	// Copy constructor
	Vector<T>(const Vector<T>& vec);
	Vector<T>(std::initializer_list<T> list);
	Vector<T>(size_t vecSize);
	~Vector<T>();
	// Copy operator =
	Vector<T>& operator=(const Vector<T>& vec);
	// pushes newItem to Vector
	void push_back(const T& newItem);
	// reserves the vector to have a total of newSize elements
	// only reserves memory, does not create extra elements.
	// if newSize < size, the extra elements are freed
	void reserve(size_t newSize);
	// resizes the vector to have a  total of newSize elements
	// fills new elements with either initValue or default initialization value
	// if newSize < size, the extra elements are freed
	void resize(size_t newSize);
	void resize(size_t newSize, const T& initValue);
	// erases members between start and end, if end is null then erase start
	// moves all the elements after the removed element
	// so that there are no discontinuities inside the vector
	T* erase(T* start, T* end = 0);

	T* insert(T* pos, const T& value);
	// Erase last element, if empty, do nothing, return false
	bool pop_back();
	// Access elements with []. Const + non-const
	T& operator[](size_t idx);
	const T& operator[](size_t idx) const;
	// The beginning of the Vector, const or not
	T* begin();
	const T* begin() const;
	// The end of the vector - non-inclusive, const or not
	T* end();
	const T* end() const;
	// The number of elements in the vector
	size_t size() const;
	// The amount of memory allocated
	size_t capacity() const;
	// Is this vector empty?
	bool empty() const;
	// Find element targetExample ptr, end if not
	T* find(const T& targetExample);
	// Const version
	const T* find(const T& targetExample) const;


	// We want to compare two vectors
	bool operator==(const Vector<T>& otherVec) const;
	// Empty the vector
	void clear();
	// Remove duplicate data. If something was done,
	// returns true.
	bool removeDuplicates();
	// Gets a smaller vector from inside the vector
	Vector<T> subVector(const T* begin, size_t vecSize) const;
	// Max value: 1. idx, 2. value ptr
	const Pair<size_t, const T*> max() const;
	Pair<size_t, T*> max();
	// Min value: 1. idx, 2. value ptr
	const Pair<size_t, const T*> min() const;
	Pair<size_t, T*> min();

	// counts the number of &ref
	const int count(const T& reference) const;

	void removeElements(const Vector<T>& vec);
	void removeElements(const T& ref);

	// adds a vec to the end of this one
	void cat(const Vector<T>& vec);

	// Shuffle with rand()
	void shuffle();

	// Swap two elements
	void swap(T* first, T* second);

	// first matching element
	T* firstMatch(const Vector<T>& vec);

	// no more size - 1
	T& last();
	const T& last() const;
	// but actually it is back in std::vector
	T& back();
	const T& back() const;

	//yup
	typedef size_t size_type;

	// does this contain all the elements of the
	// referenced vector?
	// is it faster to sort or to not sort?
	// Who knows.
	bool matchAll(const Vector<T>& refVec) const;

	// sums all of the vector's contents together,
	// left to right
	T sum() const;

private:
	// The initial malloc
	void initMemory(size_t initSize = DEFAULT_ALLOC * sizeof(T));
	// Frees the memory used by the vector
	void freeMemory();
	// Copies data to vector from first to last
	// Malloc has to be done already
	// by default copies to the beginning of the vector
	void uninitializedCopy(const T *first, const T *last, T* begin = 0);
	// Calls destructors between these two points
	// Doesn't call free(), though
	void destroyBetween(T* first, T* last);
	// All the data
	T* beginPtr;
	// Actual contents size
	size_t size_;
	// Reserved memory
	size_t capacity_;
};

template<typename T>
T* Vector<T>::insert(T* pos, const T& value){
#ifdef CHECK_OUT_OF_RANGE_ERR
	if (!(pos >= beginPtr && pos < beginPtr + size_)) {
		VECOUTOFRANGE();
	}
#endif
	if (size_ == capacity_) {
		reserve(capacity_ + ALLOC_LEAP(capacity_));
	}
	T* iterPtr = beginPtr + size_;
	new (iterPtr) T(*(beginPtr + size_ - 1));
	iterPtr--;

	while (iterPtr > pos) {
		*iterPtr = *(iterPtr - 1);
		iterPtr--;
	}
	*pos = value;
	size_++;
	return pos;
}

template<typename T>
T& Vector<T>::back(){
	return *(beginPtr + size_ - 1);
}

template<typename T>
const T& Vector<T>::back() const {
	return *(beginPtr + size_ - 1);
}


template<typename T>
bool Vector<T>::matchAll(const Vector<T>& refVec) const{
	const T* iterPtr = refVec.begin();
	while (iterPtr != refVec.end()) {
		if (find(*iterPtr) == end())
			return false;
		iterPtr++;
	}
	return true;
}

template<typename T>
inline T Vector<T>::sum() const{
	T sum;
	const T* iterPtr = beginPtr;
	while (iterPtr < beginPtr + size_) {
		sum += *iterPtr;
		iterPtr++;
	}
	return sum;
}

template<typename T>
T& Vector<T>::last(){
	return *(beginPtr + size_ - 1);
}

template<typename T>
const T& Vector<T>::last() const{
	return *(beginPtr + size_ - 1);
}


template<typename T>
T* Vector<T>::firstMatch(const Vector<T>& vec){
	T* iterPtr = beginPtr;
	while (iterPtr < beginPtr + size_) {
		if (vec.find(*iterPtr) != vec.end()) {
			return iterPtr;
		}
		else {
			iterPtr++;
		}
	}
	return beginPtr + size_;
}

template<typename T>
void yd::Vector<T>::removeElements(const T& ref){
	T* iterPtr = beginPtr;
	while (iterPtr < beginPtr + size_) {
		if (*iterPtr == ref) {
			erase(iterPtr);
		}
		else {
			iterPtr++;
		}
	}
}

template<typename T>
void yd::Vector<T>::swap(T* first, T* second){
	T firstCopy = *first;
	*first = *second;
	*second = firstCopy;
}

template<typename T>
void Vector<T>::shuffle(){
	size_t counter = size_;
	while (counter > 0) {
		size_t randIdx = rand() % counter;
		counter--;
		swap(beginPtr + counter, beginPtr + randIdx);
	}
}

template<typename T>
void Vector<T>::removeElements(const Vector<T>& vec){
	T* iterPtr = beginPtr;
	while (iterPtr < beginPtr + size_) {
		if (vec.find(*iterPtr) != vec.end()) {
			erase(iterPtr);
		}
		else {
			iterPtr++;
		}
	}
}

template<typename T>
void Vector<T>::cat(const Vector<T>& vec){
	size_t oldSize = size_;
	if(size_ + vec.size_ > capacity_)
		reserve(size_ + vec.size_);
	uninitializedCopy(vec.beginPtr, vec.beginPtr + vec.size_, beginPtr + oldSize);
	size_ = oldSize + vec.size_;
}

template<typename T>
const int Vector<T>::count(const T& reference) const{
	const T* iterPtr = beginPtr;
	int counting = 0;
	while (iterPtr < beginPtr + size_) {
		if (*iterPtr == reference)
			counting++;
		iterPtr++;
	}
	return counting;
}

template<typename T>
const Pair<size_t, const T*> Vector<T>::max() const{
	T* biggest = beginPtr;
	T* iterPtr = beginPtr + 1;
	while (iterPtr < beginPtr + size_) {
		biggest = *biggest < *iterPtr ? iterPtr : biggest;
		iterPtr++;
	}
	return Pair<size_t, T*>{ biggest - beginPtr, biggest };
}

template<typename T>
Pair<size_t, T*> Vector<T>::max(){
	T* biggest = beginPtr;
	T* iterPtr = beginPtr + 1;
	while (iterPtr < beginPtr + size_) {
		biggest = *biggest < *iterPtr ? iterPtr : biggest;
		iterPtr++;
	}
	return Pair<size_t, T*>{ size_t(biggest - beginPtr), biggest };
}

template<typename T>
const Pair<size_t, const T*> Vector<T>::min() const {
	T* smallest = beginPtr;
	T* iterPtr = beginPtr + 1;
	while (iterPtr < beginPtr + size_) {
		smallest = *smallest > *iterPtr ? iterPtr : smallest;
		iterPtr++;
	}
	return Pair<size_t, T*>{ smallest - beginPtr, smallest };
}

template<typename T>
Pair<size_t, T*> Vector<T>::min() {
	T* smallest = beginPtr;
	T* iterPtr = beginPtr + 1;
	while (iterPtr < beginPtr + size_) {
		smallest = *smallest > *iterPtr ? iterPtr : smallest;
		iterPtr++;
	}
	return Pair<size_t, T*>{ size_t(smallest - beginPtr), smallest };
}


// For all of our cout needs, extracts our vector to ostream.
// Done as an operator so we can easily print vectors inside vectors
template<typename T>
std::ostream& operator<< (std::ostream& os, const Vector<T>& vec) {
	const T* iterPtr = vec.begin();
	os << "[";
	while (iterPtr != vec.end()) {
		os << *iterPtr;
		if (iterPtr != vec.end() - 1)
			os << " ";
		iterPtr++;
	}
	return os << "]" << std::endl;
}

template<typename T>
Vector<T> Vector<T>::subVector(const T* begin, size_t vecSize) const{
	Vector<T> newVec;
#ifdef CHECK_OUT_OF_RANGE_ERR
	if (!(begin >= beginPtr && begin < beginPtr + size_)) {
		VECOUTOFRANGE();
	}
#endif

	newVec.reserve(vecSize);
	newVec.uninitializedCopy(begin, begin + vecSize);
	newVec.size_ = vecSize;
	return newVec;
}

template<typename T>
void Vector<T>::initMemory(size_t initSize) {
	void *tempBuffer = malloc(initSize * sizeof(T));
	if (tempBuffer == NULL) {
		// This could also throw an error.
		throw;
		return;
	}
	beginPtr = (T*)tempBuffer;

	size_ = 0;
	capacity_ = initSize;
}

template<typename T>
void Vector<T>::freeMemory() {
	if (beginPtr) {
		//We have to call the destructors ourselves because of
		//Placement new.
		destroyBetween(beginPtr, beginPtr + size_);
		free(beginPtr);
		beginPtr = 0;
	}
}

template<typename T>
void Vector<T>::uninitializedCopy(const T *first, const T *last, T* begin) {
	T* iterPtr = begin ? begin : beginPtr;

#ifdef CHECK_OUT_OF_RANGE_ERR
	if (!(iterPtr >= beginPtr && iterPtr < beginPtr + size_)) {
		VECOUTOFRANGE();
	}
#endif

	try {
		// Loop through first - last, placement new copies
		// with the copy constructor
		while (first != last) {
			new (static_cast<void*>(iterPtr)) T(*first);
			first++;
			(void)iterPtr++;
		}
	}
	catch (...) {
		// If this fails, destroy the ones we managed to create
		destroyBetween(beginPtr, iterPtr);
		throw;
		// Might want to throw here.
	}
}
template<typename T>
void Vector<T>::destroyBetween(T *first, T *last) {
	while (first != last) {
		first->~T();
		first++;
	}
}

template<typename T>
bool Vector<T>::pop_back() {
	if (empty())
		return false;
	size_--;
	(beginPtr + size_)->~T();
	return true;
}

template<typename T>
bool Vector<T>::empty() const {
	return size() == 0;
}

template<typename T>
T* Vector<T>::find(const T& targetExample) {
	T* ptr = beginPtr;
	while (ptr != beginPtr + size_) {
		if (*ptr == targetExample) {
			break;
		}
		ptr++;
	}
	return ptr;
}

template<typename T>
const T* Vector<T>::find(const T& targetExample) const {
	T* ptr = beginPtr;
	while (ptr != beginPtr + size_) {
		if (*ptr == targetExample) {
			break;
		}
		ptr++;
	}
	return ptr;
}

template<typename T>
bool Vector<T>::operator==(const Vector<T>& otherVec) const {
	if (size() == otherVec.size()) {
		size_t idx = 0;
		while (idx != size()) {
			if (!(beginPtr[idx] == otherVec.begin()[idx])) {
				return false;
			}
			idx++;
		}
		return true;
	}
	return false;
}

template<typename T>
void Vector<T>::clear() {
	destroyBetween(beginPtr, beginPtr + size_);
	size_ = 0;
	reserve(DEFAULT_ALLOC);
}

template<typename T>
bool Vector<T>::removeDuplicates() {
	T* iterPtr = beginPtr;
	bool removed = false;
	while (iterPtr != beginPtr + size_) {
		T* iterPtr2 = iterPtr + 1;
		while (iterPtr2 != beginPtr + size_) {
			if (*iterPtr2 == *iterPtr) {
				erase(iterPtr2);
				removed = true;
			}
			else {
				iterPtr2++;
			}
		}
		iterPtr++;
	}
	return removed;
}

template<typename T>
T* Vector<T>::erase(T* start, T* end /*= 0*/) {
	// We erase end - start elements, except if end not specified
	size_t eraseCount = end ? end - start : 1;

#ifdef CHECK_OUT_OF_RANGE_ERR
	if (!(start >= beginPtr && start + eraseCount < beginPtr + size_)) {
		VECOUTOFRANGE();
	}
#endif

	if (eraseCount > 0) {
		T* iterPointer = start;
		while (iterPointer < beginPtr + size_) {
			if (iterPointer + eraseCount < beginPtr + size_) {
				*iterPointer = *(iterPointer + eraseCount);
			}
			else {
				iterPointer->~T();
			}
			iterPointer++;
		}
		size_ = size_ - eraseCount;
		return start + 1;
	}
	return start;
}

template<typename T>
void Vector<T>::resize(size_t newSize, const T& initValue) {
	reserve(newSize);
	while (size_ < capacity_) {
		new(beginPtr + size_) T(initValue);
		size_++;
	}
}

template<typename T>
void Vector<T>::resize(size_t newSize) {
	reserve(newSize);
	while (size_ < capacity_) {
		new(beginPtr + size_) T();
		size_++;
	}
}

template<typename T>
void Vector<T>::reserve(size_t newSize) {

	size_t oldSize = size();
	newSize = newSize ? newSize : 1;
	if (oldSize != newSize) {

		// Destroy things that will be completely out of vector range
		if (oldSize > newSize) {
			destroyBetween(beginPtr + newSize, beginPtr + oldSize);
		}

		void* tempBuffer = realloc(beginPtr, sizeof(T)*(newSize));
		if (tempBuffer == NULL) {
			// This could also throw an error.
			return;
		}
		beginPtr = (T*)tempBuffer;

		capacity_ = newSize;
		if (oldSize > newSize) {
			size_ = capacity_;
		}
	}
}

template<typename T>
size_t Vector<T>::size() const {
	return size_;
}

template<typename T>
size_t Vector<T>::capacity() const {
	return capacity_;
}

template<typename T>
T* Vector<T>::end() {
	return beginPtr + size_;
}

template<typename T>
inline const T * Vector<T>::end() const {
	return beginPtr + size_;
}

template<typename T>
T* Vector<T>::begin() {
	return beginPtr;
}

template<typename T>
inline const T * Vector<T>::begin() const {
	return beginPtr;
}

template<typename T>
const T& Vector<T>::operator[](size_t idx) const {

#ifdef CHECK_OUT_OF_RANGE_ERR
	if (idx >= size_) {
		VECOUTOFRANGE();
	}
#endif

	return *(beginPtr + idx);
}

template<typename T>
T& Vector<T>::operator[](size_t idx) {

#ifdef CHECK_OUT_OF_RANGE_ERR
	if (idx >= size_) {
		VECOUTOFRANGE();
	}
#endif

	return *(beginPtr + idx);
}

template<typename T>
inline Vector<T>::Vector() : beginPtr(0), size_(0), capacity_(0) {
	initMemory();
}

template<typename T>
template<typename T2, size_t SIZE>
inline Vector<T>::Vector(T2(&arr)[SIZE]) {
	T* initListIter = &arr[0];
	size_t arrSize = SIZE;
	initMemory();
	reserve(arrSize);
	for (size_t i = 0; i < arrSize; i++) {
		push_back(*(initListIter));
		initListIter++;
	}
}

template<typename T>
inline Vector<T>::~Vector() {
	freeMemory();
}

template<typename T>
inline Vector<T> &Vector<T>::operator=(const Vector<T> &vec) {
	if (beginPtr) {
		freeMemory();
	}
	size_t mallocSize = (vec.capacity_);
	initMemory(mallocSize);
	uninitializedCopy(vec.beginPtr, vec.beginPtr + vec.size_);

	size_ = vec.size();
	capacity_ = vec.capacity_;
	return *this;
}

template<typename T>
inline Vector<T>::Vector(const Vector<T>& vec) {
	size_t mallocSize = (vec.capacity_);
	initMemory(mallocSize);
	uninitializedCopy(vec.beginPtr, vec.beginPtr + vec.size_);

	size_ = vec.size();
	capacity_ = vec.capacity_;
}

template<typename T>
inline Vector<T>::Vector(std::initializer_list<T> list){
	size_t mallocSize = (list.size());
	initMemory(mallocSize);
	uninitializedCopy(list.begin(), list.end());

	size_ = list.size();
	capacity_ = list.size();
}

template<typename T>
inline Vector<T>::Vector(size_t vecSize){
	initMemory(vecSize);
	resize(vecSize);
	capacity_ = vecSize;
}

template<typename T>
void Vector<T>::push_back(const T& newItem) {

	// We still have allocated, unused space
	if (capacity_ > size_) {
		// Placement news, because we want to call the constructors when needed
		// Unused variable, if I refer to this...
		new(beginPtr + size_) T(newItem);
		size_++;
	}
	// Nawh, time to reallocate
	else {
		// add allocLeap number of mem slots to the beginPtr
		size_t oldSize = size_;
		size_t newSize = oldSize + ALLOC_LEAP(oldSize);
		void* tempBuffer = realloc(beginPtr, sizeof(T)*(newSize));
		if (tempBuffer == NULL) {
			// This could also throw an error.
			return;
		}
		beginPtr = (T*)tempBuffer;
		capacity_ = newSize;

		size_++;
		new (beginPtr + oldSize) T(newItem);
	}

}
}
#endif
