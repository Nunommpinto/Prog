#include "Movie.h"

Movie::Movie(const string& title, float cost): title(title)
{
	this->cost = cost;
	this->timesRented = 0;
}
