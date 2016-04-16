#ifndef _CURSOR
#define _CURSOR
#include <iostream>
#include <string>

class Cursor
{
	int pos;
	int pos_max;
public:
	Cursor();
	Cursor(int pos_max);
	Cursor(int pos, int pos_max);
	int getPosition() const;
	bool up();
	bool down();
	bool setPosition(int position);
	bool changeMax(int max);
};
inline int Cursor::getPosition() const
{
	return this->pos;
}
#endif