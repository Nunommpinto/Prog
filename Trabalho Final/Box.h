#ifndef _BOX
 #define _BOX

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

#include "Date.h"
#include "Channel.h"
#include "Program.h"
#include "Movie.h"

using namespace std;

class Box
{
  string password;
  Date currentDate;
  vector<Channel> channels;
  vector<Movie> movieClub;
  vector<Movie> seenMovies;
  vector<Program> toBeRecorded;
  bool authorized;
  float total_money_spent;

 public:
  Box(Date currentDate);
  bool load();
  void save();

  inline Date getDate()const;
  inline void setDate(const Date d);

  vector<string> getTypes() const;
  int getTypeFrequency(const string& type) const;

  vector<Program> listByDay(string day) const;
  vector<Program> listByChannel(const string& channel, const string& day) const;
  vector<Program> listByType(const string&  type, const string& day) const;
  vector<Program> listByAvailable(const string& variant) const;
  inline const vector<Program>& listRecordings() const;

  inline const vector<Channel>& getChannels() const;
  inline const vector<Movie>& getmovieClub() const;
  inline const vector<Movie>& getseenmovies() const;

  const Movie& getmovie(const string& title) const;

  void recordprogram(const Program& program);
  void cancelrecording(const Program& program);

  bool issettoberecorded(const Program& program);
  bool rentMovies(const string& title);

  float moneySpent() const;
  float getCost(const string& title) const;
  int timesWhatched(const string& title) const;

  inline bool isAuthorized() const;
  bool authorize(const string& s);
  bool changePassword(const string& s);
  // Channel CRUD
  bool createdChannel(const string& name);
  bool removeChannel(const Channel& channel);
  bool updateChannel(const string& name, const Channel& channel);
  // Program CRUD
  bool createdProgram(const string& title, const Channel& channel);
  bool removeProgram(const Channel& channel, const Program& program);
  bool updateProgram(const string& name, const string& type, int duration, const Date& date, const Program& program);
  // Movie CRUD
  bool createdMovie(const string& title);
  bool removeMovie(const string& title);
  bool updateMovie(const string& title, const Movie& movie, float cost);
};
inline Date Box::getDate()const//
{
	return this->currentDate;
}
inline void Box::setDate(const Date d)//
{
	this->currentDate = d;
}
inline const vector<Program>& Box::listRecordings() const
{
	return this->toBeRecorded;
}
inline const vector<Channel>& Box::getChannels() const
{
	return (this->channels);
}
inline const vector<Movie>& Box::getmovieClub() const 
{
	return this->movieClub;
}
inline const vector<Movie>& Box::getseenmovies() const
{
	return this->seenMovies;
}
inline bool Box::isAuthorized() const//
{
	return this->authorized;
}
#endif
