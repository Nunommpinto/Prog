#ifndef _PROGRAM
#define _PROGRAM

#include<iostream>
#include <string>
#include "Date.h"

using namespace std;

class Program
{
	string name;
	string type;
	string channel;
	bool recorded;
	int duration;  
	Date exhibitionDate;

public:
	Program(string name, string type, string channel, int duration, string day, int hour, int minutes);
	Program();
	inline string getDay() const;
	inline string getType() const;
	inline string getName() const;
	inline string getChannel() const;
	inline int getDuration() const;
	inline Date getDate() const;
	bool operator<(const Program& program2) const;
	bool overlaps(const Program& program2) const;
	bool operator==(const Program& p2);
	inline Program& setRecorded();
	inline Program& clearRecorded();
	inline bool isRecorded() const;
	inline Program& setname(const string& name);
	inline Program& settype(const string& type);
	inline Program& setduration(int duration);
	inline Program& setdate(const Date& date);
};
inline string Program::getDay() const
{
	return (this->exhibitionDate).getDay();
}
inline string Program::getType() const
{
	return this->type;
}
inline string Program::getName() const
{
	return this->name;
}
inline string Program::getChannel() const
{
	return this->channel;
}
inline int Program::getDuration() const
{
	return this->duration;
}
inline Date Program::getDate() const
{
	return this->exhibitionDate;
}
inline Program& Program::setRecorded()
{
	this->recorded = true;
	return *this;
}
inline Program& Program::clearRecorded()
{
	this->recorded = false;
	return *this;
}
inline bool Program::isRecorded() const
{
	return this->recorded;
}
inline Program& Program::setname(const string& name)
{
	this->name = name;
	return *this;
}
inline Program& Program::settype(const string& type)
{
	this->type = type;
	return *this;
}
inline Program& Program::setduration(int duration)
{
	this->duration = duration;
	return *this;
}
inline Program& Program::setdate(const Date& date)
{
	this->exhibitionDate = date;
	return *this;
}
#endif
