/*
**
**
*/
#include <ostream>

template <typename T>
struct Node
{
public:
	Node(const T& inData)
	:mData(inData)
	{}
	
	T mData;
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const Node<T> inNode)
{
	os << inNode.mData;
	return os;
}

