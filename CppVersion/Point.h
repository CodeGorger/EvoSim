#ifndef _POINT_H_
#define _POINT_H_

#include <cmath>

template<typename T>
class Point
{
private:
	T _x;
	T _y;
public:
	Point(T inX, T inY)
	{
		_x = inX;
		_y = inY;
	}
	T GetX();
	T GetY();
	void SetX(T inX);
	void SetY(T inY);
	double GetDistance(Point<T> rhs);

	bool Equals(Point<T> rhs);
};


template<typename T>
bool Point<T>::Equals(Point<T> rhs)
{
	return ((rhs.GetX() == GetX()) && (rhs.GetY() == GetY()));
}

template<typename T>
T Point<T>::GetX()
{
	return _x;
}


template<typename T>
T Point<T>::GetY()
{
	return _y;
}


template<typename T>
void Point<T>::SetX(T inX)
{
	_x=inX;
}


template<typename T>
void Point<T>::SetY(T inY)
{
	_y=inY;
}


template<typename T>
double Point<T>::GetDistance(Point<T> rhs)
{
	double dx = ((double)GetX()) - ((double)rhs.GetX());
	double dy = ((double)GetY()) - ((double)rhs.GetY());
	return sqrt(dx*dx+dy*dy);
}


#endif