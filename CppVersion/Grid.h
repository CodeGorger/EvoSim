#ifndef _GRID_H_
#define _GRID_H_

#include <vector>
#include <iostream>
#include "Point.h"

// A class that will hold a 2D 'array' of type T
template<typename T>
class Grid
{
public:
	// Will create the 2D array with size width and height
	// All entries will initially be set to initVal
	Grid(size_t width, size_t height, T initVal);

	// Will return a reference of the object at inPosition
	T & Get(Point<size_t> inPosition);

	// Will return a reference of the object at inX,inY
	T & Get(size_t inX, size_t inY);

	// Will set the object at inPosition to inVal
	void Set(Point<size_t> inPosition, T inVal);
	// Will set the object at inX,inY to inVal
	void Set(size_t inX, size_t inY, T inVal);

	size_t GetWidth();
	size_t GetHeight();

	// Will check if a location is still inside the map
	// (so will not cause array out of bound...)
	bool IsLocationWithin(Point<size_t> spawn);

private:
	std::vector<T> _data;
	size_t _width;
	size_t _height;
};

template<typename T>
size_t Grid<T>::GetWidth()
{
	return _width;
}

template<typename T>
size_t Grid<T>::GetHeight()
{
	return _height;
}

template<typename T>
Grid<T>::Grid(size_t inWidth, size_t inHeight, T initVal)
{
	_width = inWidth;
	_height = inHeight;
	_data = std::vector<T>(_width*_height, initVal);
}


template<typename T>
T & Grid<T>::Get(Point<size_t> inPosition)
{
	static T failed = T();
	if (inPosition.GetX() >= _width)
	{
		std::cout << "Fail! Grid<T>::Get: X >= _width" << std::endl;
		return failed;
	}
	if (inPosition.GetY() >= _height)
	{
		std::cout << "Fail! Grid<T>::Get: Y >= _height" << std::endl;
		return failed;
	}
	return _data[inPosition.GetX() + inPosition.GetY() * _width];
}


template<typename T>
T & Grid<T>::Get(size_t inX, size_t inY)
{
	static T failed = T();
	if (inX >= _width)
	{
		std::cout << "Fail! Grid<T>::Get: inX >= _width" << std::endl;
		return failed;
	}
	if (inY >= _height)
	{
		std::cout << "Fail! Grid<T>::Get: inY >= _height" << std::endl;
		return failed;
	}
	return _data[inX + inY * _width];
}

template<typename T>
void Grid<T>::Set(Point<size_t> inPosition, T inVal)
{
	if (inPosition.GetX() >= _width)
	{
		std::cout << "Fail! Grid<T>::Set: inX >= _width" << std::endl;
		return;
	}
	if (inPosition.GetY() >= _height)
	{
		std::cout << "Fail! Grid<T>::Set: inY >= _height" << std::endl;
		return;
	}
	_data[inPosition.GetX() + inPosition.GetY() * _width] = inVal;
}

template<typename T>
void Grid<T>::Set(size_t inX, size_t inY, T inVal)
{
	if (inX >= _width)
	{
		std::cout << "Fail! Grid<T>::Set: inX >= _width" << std::endl;
		return;
	}
	if (inY >= _height)
	{
		std::cout << "Fail! Grid<T>::Set: inY >= _height" << std::endl;
		return;
	}
	_data[inX + inY * _width] = inVal;
}


template<typename T>
bool Grid<T>::IsLocationWithin(Point<size_t> spawn)
{
	return (spawn.GetX()<_width)&&(spawn.GetY() < _height);
}

#endif