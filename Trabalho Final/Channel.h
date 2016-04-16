#ifndef _CHANNEL
#define _CHANNEL

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
 
#include "Program.h"

using namespace std;

class Channel
{
	friend class Box;
private:
  string name;
  vector<Program> programs;
 public:
  Channel(const string& name);
  inline string getName() const;
  bool setName(const string& name);
  inline const vector <Program>& getPrograms()const;
  int findProgram(const string& title) const;
  void addProgram(const Program& p);
  inline void setRecorded(int i);
  inline bool operator<(const Channel& c2) const;
  inline bool operator== (const Channel& c2) const;
  bool removeProgram(const Program& p);
};
inline string Channel::getName() const
{
	return name;
}
inline const vector <Program>& Channel::getPrograms() const
{
	return this->programs;
}
inline void Channel::setRecorded(int i)
{
	programs[i].setRecorded();
}
inline bool Channel::operator<(const Channel& c2) const
{
	return (this->name < c2.name);
}
inline bool Channel::operator== (const Channel& c2) const
{
	return(this->name == c2.name);
}

#endif
