#pragma once

class StackAllocator
{
private:

public:
	//stack marker: represents the current top of the stack;
	//you can only rollback to a marker within the stack
	typedef unsigned int Marker;

	//constructs a stack allocator with the given total size
	explicit StackAllocator(unsigned int stackSize_bytes);

	//allocates a new block of the given size from stack top
	void* alloc(unsigned int size_bytes);

	//returns a marker to the crrent stack top
	Marker getMarker();

	//rolls the stack back to a previous marker
	void freeToMarker(Marker marker);

	//clears the entire stacks (rolls it back to zero)
	void clear();

};

//this class allows a block of memory allocated on
//frame i to be used on frame i + 1 . This is done
//by creating 2 single-frame stack allocators of
//equal size and then ping-pong between them every frame

class DoubleBufferedAllocator
{
	unsigned int m_curStack;
	StackAllocator m_stack[2];
public:
	void swapBuffers();
	void clearCurrentBuffer();
	void* alloc(unsigned int nBytes);
};