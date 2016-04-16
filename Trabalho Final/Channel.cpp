#include "Channel.h"

Channel::Channel(const string& name) : name(name) {}
bool Channel::setName(const string& name)
{
	if (this->name == name)
		return false;
	else
	{
		this->name = name;
		return true;
	}
}
void Channel::addProgram(const Program& p)
{
	
	bool found = false;
	int i = 0;
	for (; i < programs.size(); i++)
	if (p < programs[i])
	{
		found = true;
		break;
	}
	if (found)
	{
		programs.resize(programs.size() + 1);
		for (int j = programs.size() - 1; j > i; j--)
		{
			programs[j] = programs[j - 1];
		}
		programs[i] = p;
	}
	else programs.push_back(p);
}
int Channel::findProgram(const string& title) const
{
	int i = 0;
	for (; i < this->programs.size();i++)
	if (programs[i].getName() == title)
		return i;
	return -1;
}
bool Channel::removeProgram(const Program& p)
{
	int i = 0;
	bool found = false;
	for (; i < this->programs.size();i++)
	if (programs[i]== p)
	{
		found = true;
		break;
	}
	if (found)
	{
	programs[i] = programs[programs.size() - 1];
	programs.pop_back();
	sort(programs.begin(), programs.end());
	return true;
	}
	else return false;
}