#ifndef _DATE
#define _DATE

#include <iostream>
#include <string>

using namespace std;

class Date
{
  string weekDay;
  unsigned hour;
  unsigned minutes;
 public:
  Date(const string& day, unsigned hour, unsigned minutes);
  Date();
  bool operator<(const Date& date2) const;
  bool operator== (const Date& date2) const;
  inline int operator+ (int num)const;
  inline int operator- (int num)const;
  inline string getDay() const;
  inline int getHour() const;
  inline int getMinutes() const;
  int toInt() const;

};
inline string Date::getDay() const
{
	return this->weekDay;
}
inline int Date::getHour() const
{
	return this->hour;
}
inline int Date::getMinutes() const
{ 
	return this->minutes;
}
inline int Date::operator+ (int num) const
{
	return this->toInt() + num;
}
inline int Date::operator- (int num)const
{
	return this->toInt() - num;
}
#endif
