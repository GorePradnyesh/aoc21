/*
**
**
*/

#include <ostream>
#include <memory>

template <typename T>
struct Node
{
public:
	Node(const T& inData)
	:mData(inData)
	{}
	
	T mData;
	std::shared_ptr<Node> mLeft;
	std::shared_ptr<Node> mRight;
	
	// ?
	std::shared_ptr<Node> mParent;
};


/*
**
*/
template <typename T>
std::ostream& operator<<(std::ostream& os, const Node<T>& inNode)
{
	os << inNode.mData ;
	return os;
}

