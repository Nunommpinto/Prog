#include "Program.h"

Program::Program() : exhibitionDate("Monday", 0, 0), name("none"), type("none"), channel("none")
{
	this->recorded = false;
	this->duration = 0;
}
Program::Program(string name, string type, string channel, int duration, string day, int hour, int minutes) : exhibitionDate(day, hour, minutes), name(name), type(type), channel(channel)
{
	this->type = type;
	this->channel = channel;
	this->recorded = false;
	this->duration= duration;
}
bool Program::operator<(const Program& program2) const
{
	if (this->exhibitionDate < program2.exhibitionDate)
		return true;
	else return false;
}
bool Program::overlaps(const Program& program2) const
{
	if (*this < program2)
	{
		if ((program2.exhibitionDate.toInt() < (this->exhibitionDate.toInt() + this->duration)))
			return true;
		else return false;
	}
	else
	{
		if ((this->exhibitionDate.toInt() < (program2.exhibitionDate.toInt() + program2.duration)))
			return true;
		else return false;
	}
}
bool Program::operator==(const Program& p2)
{
	if (this->name == p2.name && this->type == p2.type && this->duration == p2.duration && this->exhibitionDate == p2.exhibitionDate)
		return true;
	else return false;
}


