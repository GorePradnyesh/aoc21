/*
**
**
*/

#include <iostream>
#include <memory>
#include <sstream>

template <typename T>
struct Node
{
public:
	Node(const T& inData)
	:mData(inData)
	{}
	
	Node(
		const std::shared_ptr<Node<T>> inLeftNode,
		const std::shared_ptr<Node<T>> inRightNode,
		const T& inData)
	:mLeft(inLeftNode),
	mRight(inRightNode),
	mData(inData)
	{}
	
	
	T mData;
	std::shared_ptr<Node<T>> mLeft;
	std::shared_ptr<Node<T>> mRight;
	
	// ?
	std::shared_ptr<Node<T>> mParent;
};

template <typename T>
using NodePtr = std::shared_ptr<Node<T>>;

template <typename T>
bool AddLeft(const NodePtr<T>& node, const NodePtr<T>& inOther)
{
	if(node->mLeft)
	{
		std::cout << "Bad Op. Node already contains left \n";
		return false;
	}
	node->mLeft = inOther;
	return false;
}


template <typename T>
bool AddRight(const NodePtr<T>& node, const NodePtr<T>& inOther)
{
	if(node->mRight)
	{
		std::cout << "Bad Op. Node already contains left \n";
		return false;
	}
	node->mRight = inOther;
	return false;
}

/*
**
*/
template <typename T>
std::ostream& operator<<(std::ostream& os, const Node<T>& inNode)
{
	os << inNode.mData ;
	return os;
}

/*
**
*/
template<typename T>
void GetNodeStream(const NodePtr<T>& inNode, std::stringstream& ss)
{
	if(inNode->mData)
	{
		ss << inNode->mData;
	}
	else
	{
		ss << "[";
		if(inNode->mLeft)
		{
			GetNodeStream(inNode->mLeft, ss);
			ss << ",";
		}
		if(inNode->mRight)
		{
			GetNodeStream(inNode->mRight, ss);
		}
		ss << "]";
	}
}

/*
**
*/
template<typename T>
void PrintNode(const NodePtr<T>& inNode)
{
	std::stringstream ss;
	GetNodeStream(inNode, ss);
	std::cout << ss.str() << "\n";
}

