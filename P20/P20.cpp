#include "P20.h"
#include <utility>
#include "common/DataBuffer2D.h"


namespace
{

void BasicBufferTest()
{
	std::vector<std::uint32_t> buffer {1, 2, 3, 4, 5, 6};
	auto bufferPtr = CreateBuffer(std::move(buffer), 2, 3);

	// bufferPtr->SetElement(1, 0, 44);
	
	std::cout << *bufferPtr << std::endl;
}


}	// unnamed

namespace P20
{

/*
**
*/
void Process()
{
	BasicBufferTest();
}

} // namespace P18
