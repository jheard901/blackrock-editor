//custom stack memory allocator

#include "custom_stack.h"

typedef unsigned int Marker;

//STACK ALLOCATOR CLASS

StackAllocator::StackAllocator(unsigned int stackSize_bytes)
{
	
}

//these two functions need to return something. I need to
//learn more about creating custom stacks to do this

/*
void* StackAllocator::alloc(unsigned int size_bytes)
{

}

Marker StackAllocator::getMarker()
{
	
}
*/

void StackAllocator::freeToMarker(Marker marker)
{

}

void StackAllocator::clear()
{

}


//DOUBLE BUFFERED ALLOCATOR CLASS

void DoubleBufferedAllocator::swapBuffers()
{
	m_curStack = (unsigned int)!m_curStack;
}

void DoubleBufferedAllocator::clearCurrentBuffer()
{
	m_stack[m_curStack].clear();
}

/*
void* DoubleBufferedAllocator::alloc(unsigned int nBytes)
{
	return m_stack[m_curStack].alloc(nBytes);
}
*/

/* Example usage of double buf allocator in program */

/*

//in the WinMain() function
//....

DoubleBufferedAllocator g_doubleBufAllocator;

//main game loop
while(true)
{
	//clear the single frame allocator every frame as before
	g_singleFrameAllocator.clear();

	//swap the active and inactive buffers of the double buffered allocator
	g_doubleBufAllocator.swapBuffers();

	//Now clear the newly active buffer, leaving the last frame's buffer intact
	g_doubleBufAllocator.clearCurrentBuffer();

	//....

	//allocate out of the current buffer, without disturbing last
	//frame's data. Only use this data this frame or next frame. Again,
	//this memory never needs to be freed
	void* p = g_doubleBufAllocator.alloc(nBytes);

	//....
}

*/