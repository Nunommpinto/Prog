#ifndef _MOVIE
 #define _MOVIE

#include <iostream>
#include <string>

using namespace std;


class Movie{
  string title;
  float cost;
  unsigned timesRented;
 public:
  Movie(const string& title, float cost);
  inline float getCost() const;
  inline unsigned gettimesRented() const;
  inline Movie& inctimesRented();
  inline Movie& settimesRented(unsigned num);
  inline string getTitle() const;
  inline bool operator<(const Movie& m2) const;
  inline bool operator==(const Movie& m2) const;
};
inline float Movie::getCost() const
{
	return this->cost;
}
inline unsigned Movie::gettimesRented() const
{
	return this->timesRented;
}
inline Movie& Movie::inctimesRented()
{
	this->timesRented++;
	return *this;
}
inline string Movie::getTitle() const
{
	return this->title;
}
inline Movie& Movie::settimesRented(unsigned num)
{
	this->timesRented = num;
	return *this;
}
inline bool Movie::operator<(const Movie& m2) const
{
	return (this->title < m2.title);
}
inline bool Movie::operator==(const Movie& m2) const
{
	if ((this->title == m2.title) && (this->cost == m2.cost))
		return true;
	else return false;
}
#endif
