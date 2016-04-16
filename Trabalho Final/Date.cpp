#include "Date.h"

string days[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };

Date::Date(const string& day, unsigned hour, unsigned minutes): weekDay(day)
{
	this->hour = hour;
	this->minutes = minutes;
}
Date::Date() : weekDay("Monday")
{
	this->hour = 0;
	this->minutes = 0;
}
bool Date::operator<(const Date& date2) const
 {
	if (this->toInt() < date2.toInt())
		return true;
	else return false;
}
bool Date::operator==(const Date& date2) const
{
	if (this->getDay() == date2.getDay() && this->getHour() == date2.getHour() && this->getMinutes() == date2.getMinutes())
		return true;
	else return false;
}
//converte uma data para inteiro
int Date::toInt() const
{
	int day;
	for (day = 0; day< 7; day++)
	if (this->weekDay == days[day])
		break;
	return day * 24 * 60 + this->hour * 60 + this->minutes;
}

