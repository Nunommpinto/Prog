#include "Cursor.h"

Cursor::Cursor()
{
	this->pos = 0;
	this->pos_max = 256;
}
Cursor::Cursor(int pos_max)
{
	this->pos = 0;
	this->pos_max = pos_max;
}
bool Cursor::changeMax(int max)
{
	if (this->pos_max == max)
		return false;
	else
	{
		this->pos = 0;
		this->pos_max = max;
		return true;
	}
}
Cursor::Cursor(int pos, int pos_max)
{
	this->pos = pos;
	this->pos_max = pos_max;
}
bool Cursor::up()
{
	if (this->pos_max == 0)
		return false;
	else if (this->pos > 0)
		pos--;
	else this->pos = this->pos_max;
	return true;
}
bool Cursor::down()
{
	if (this->pos_max == 0)
		return false;
	if (this->pos < pos_max)
		pos++;
	else this->pos = 0;
	return true;
}
bool Cursor::setPosition(int position)
{
	if (position >= 0 && position <= pos_max)
	{
		this->pos = position;
		return true;
	}
	else return false;
}

