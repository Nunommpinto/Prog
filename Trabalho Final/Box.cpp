#include "Box.h"

Box::Box(Date date) :currentDate(date), password("")
{
	this->authorized = false;
}
bool Box::load()
{
	authorized = false;
	ifstream fin;
	fin.open("Box.txt");
	if (fin.fail())
		return false;
	int passlength;
	fin >> passlength;
	vector<int> encrypted;
	string passtemp;
	passtemp.resize(passlength);
	while (passlength > 0)
	{
		int temp;
		fin >> temp;
		encrypted.push_back(temp);
		passlength--;
	}
	int numchannels, nummovieclub, numseenmovies, numrecorded, key;
	fin >> numchannels >> nummovieclub >> numseenmovies >> numrecorded >> total_money_spent >> key;
	for (int i = 0; i < passtemp.length(); i++)
		passtemp[i] = encrypted[i] ^ key;
	this->password = passtemp;
	fin.ignore(100);
	fin.close();
	fin.open("Channels.txt");
	if (fin.fail())
		return false;
	for (; numchannels > 0; numchannels--)
	{
		string stemp;
		getline(fin, stemp);
		Channel ctemp(stemp);
		while (fin.peek() != '\n')
		{
			string name;
			getline(fin, name);
			string type;
			getline(fin, type);
			int duration;
			fin >> duration;
			fin.ignore();
			string day;
			getline(fin, day);
			int hour, minutes;
			fin >> hour >> minutes;
			fin.ignore();
			Program ptemp = Program(name, type, stemp, duration, day, hour, minutes);
			ctemp.addProgram(ptemp);
		}
		channels.push_back(ctemp);
		fin.ignore();
	}
	fin.close();
	sort(this->channels.begin(), this->channels.end());
	fin.open("MovieClub.txt");
	if (fin.fail())
		return false;
	for (; nummovieclub > 0; nummovieclub--)
	{
		string title;
		float cost;
		getline(fin, title);
		fin >> cost;
		fin.ignore();
		Movie mtemp = Movie(title, cost);
		movieClub.push_back(mtemp);
	}
	fin.close();
	sort(this->movieClub.begin(), this->movieClub.end());
	fin.open("SeenMovies.txt");
	if (fin.fail())
		return false;
	for (; numseenmovies > 0; numseenmovies--)
	{
		string title;
		float cost;
		unsigned timesrented;
		getline(fin, title);
		fin >> cost >> timesrented;
		fin.ignore();
		Movie mtemp = Movie(title, cost);
		mtemp.settimesRented(timesrented);
		seenMovies.push_back(mtemp);
	}
	fin.close();
	sort(this->seenMovies.begin(), this->seenMovies.end());
	fin.open("RecordedPrograms.txt");
	if (fin.fail())
		return false;
	vector<Program*> v;
	for (; numrecorded > 0; numrecorded--)
	{
		string channelname;
		string day;
		int hour, minutes;
		getline(fin, channelname);
		getline(fin, day);
		fin >> hour >> minutes;
		Date dtemp(day, hour, minutes);
		fin.ignore();
		for (int i = 0; i < channels.size(); i++)
		if (channelname == channels[i].getName())
		for (int j = 0; j < channels[i].programs.size(); j++)
		if (dtemp == channels[i].programs[j].getDate())
		{
			if (channels[i].programs[j].getDate() < this->currentDate)
				channels[i].setRecorded(j);
			toBeRecorded.push_back(channels[i].programs[j]);
		}
	}
	sort(toBeRecorded.begin(), toBeRecorded.end());
	fin.close();
	return true;
}
void Box::save()
{
	ofstream fout;
	fout.open("Box.txt");
	vector <int> encrypted;
	int key = rand() % 100 + 10;
	for (int i = 0; i < password.length(); i++)
	{
		encrypted.push_back(password[i] ^ key);
	}

	fout << encrypted.size()<< endl;
	for (int i = 0; i < encrypted.size(); i++)
	{
		fout << encrypted[i]<< " ";
	}
	fout << endl;
	fout << channels.size() << endl << movieClub.size() << endl << seenMovies.size() << endl << toBeRecorded.size() << endl << total_money_spent << endl<< key << endl;
	fout.close();
	fout.open("Channels.txt");
	for (int i = 0; i < channels.size(); i++)
	{
		fout << channels[i].getName() << endl;
		for (int j = 0; j < channels[i].programs.size(); j++)
		{
			fout << channels[i].programs[j].getName() << endl;
			fout << channels[i].programs[j].getType() << endl;
			fout << channels[i].programs[j].getDuration() << endl;
			fout << channels[i].programs[j].getDate().getDay() << endl;
			fout << channels[i].programs[j].getDate().getHour() << endl;
			fout << channels[i].programs[j].getDate().getMinutes() << endl;
		}
		fout << endl;
	}
	fout.close();
	fout.open("MovieClub.txt");
	for (int i = 0; i < movieClub.size(); i++)
	{
		fout << movieClub[i].getTitle() << endl;
		fout << movieClub[i].getCost() << endl;
	}
	fout.close();
	fout.open("SeenMovies.txt");
	for (int i = 0; i < seenMovies.size(); i++)
	{
		fout << seenMovies[i].getTitle() << endl;
		fout << seenMovies[i].getCost() << endl;
		fout << seenMovies[i].gettimesRented() << endl;
	}
	fout.close();
	fout.open("RecordedPrograms.txt");
	for (int i = 0; i < toBeRecorded.size(); i++)
	{
		fout << toBeRecorded[i].getChannel() << endl;
		fout << toBeRecorded[i].getDate().getDay() << endl;
		fout << toBeRecorded[i].getDate().getHour() << endl;
		fout << toBeRecorded[i].getDate().getMinutes() << endl;
	}
	fout.close();
}
vector<Program> Box::listByDay(string day) const
{
	vector<Program> v_out;
	if (day == "none")
	{
		day = this->currentDate.getDay();
		for (int i = 0; i < channels.size(); i++)
		for (int j = 0; j < (channels[i].programs).size(); j++)
		if ((channels[i].programs)[j].getDay() == day)
			v_out.push_back((channels[i].programs)[j]);

	}
	else
	{
		for (int i = 0; i < channels.size(); i++)
		for (int j = 0; j < (channels[i].programs).size(); j++)
		if ((channels[i].programs)[j].getDay() == day)
			v_out.push_back((channels[i].programs)[j]);
	}
	sort(v_out.begin(), v_out.end());
	return v_out;
}
vector<Program> Box::listByChannel(const string&  channel, const string& day) const
{
	int i = 0;
	vector<Program> v_out;
	for (; i < channels.size(); i++)
	if (channels[i].getName() == channel)
		break;
	if (day == "none")
		return channels[i].programs;
	else
	{
		for (int j = 0; j < channels[i].programs.size(); j++)
		if ((channels[i].programs)[j].getDay() == day)
			v_out.push_back((channels[i].programs)[j]);
	}
	sort(v_out.begin(), v_out.end());
	return v_out;
}
vector<Program> Box::listByType(const string&  type, const string& day) const
{
	vector<Program> v_out;
	if (day == "none")
	{
		for (int i = 0; i < channels.size(); i++)
		for (int j = 0; j < (channels[i].programs).size(); j++)
		if ((channels[i].programs)[j].getType() == type)
			v_out.push_back((channels[i].programs)[j]);
	}
	else
	{
		for (int i = 0; i < channels.size(); i++)
		for (int j = 0; j < (channels[i].programs).size(); j++)
		if ((channels[i].programs)[j].getDay() == day && (channels[i].programs)[j].getType() == type)
			v_out.push_back((channels[i].programs)[j]);

	}
	sort(v_out.begin(), v_out.end());
	return v_out;
}
vector<Program> Box::listByAvailable(const string& variant) const
{
	vector<Program> v_out;
	if (variant == "Live TV")
	{
		for (int i = 0; i < channels.size(); i++)
		for (int j = 0; j < (channels[i].programs).size(); j++)
		if (channels[i].programs[j].getDate() < this->getDate() && channels[i].programs[j].getDate() + channels[i].programs[j].getDuration() > this->currentDate + 0)
			v_out.push_back(channels[i].programs[j]);
	}
	else
	{
		for (int i = 0; i < toBeRecorded.size(); i++)
		if (toBeRecorded[i].isRecorded())
			v_out.push_back(toBeRecorded[i]);
	}
	sort(v_out.begin(), v_out.end());
	return v_out;
}
bool is_in_vector(const string& s, const vector<string>& v)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i] == s)
			return true;
	}
	return false;
}
vector<string> Box::getTypes() const
{
	vector<string> v_out;
	for (int j = 0; j < channels.size(); j++)
	for (int i = 0; i < channels[j].programs.size(); i++)
	{
		if (!is_in_vector(channels[j].programs[i].getType(), v_out))
			v_out.push_back(channels[j].programs[i].getType());
	}
	return v_out;
}
int Box::getTypeFrequency(const string& type) const
{
	int count = 0;
	for (int j = 0; j < channels.size(); j++)
	for (int i = 0; i < channels[j].programs.size(); i++)
	{
		if (channels[j].programs[i].getType() == type)
			count++;
	}
	return count;
}
const Movie& Box::getmovie(const string& title) const
{
	bool found = false;
	for (int i = 0; i < movieClub.size(); i++)
	if (movieClub[i].getTitle() == title)
		return movieClub[i];
	for (int i = 0; i < seenMovies.size(); i++)
	if (seenMovies[i].getTitle() == title)
		return seenMovies[i];
}
bool Box::rentMovies(const string& title)
{
	for (int i = 0; i < (this->movieClub).size(); i++)
	if (this->movieClub[i].getTitle() == title)
	{
		total_money_spent += this->movieClub[i].getCost();
		seenMovies.push_back(this->movieClub[i]);
		this->movieClub[i] = this->movieClub[this->movieClub.size() - 1];
		(this->movieClub).pop_back();
		this->seenMovies[this->seenMovies.size() - 1].inctimesRented();
		return true;
	}
	for (int i = 0; i < (this->seenMovies).size(); i++)
	if (this->seenMovies[i].getTitle() == title)
	{
		this->seenMovies[i].inctimesRented();
		total_money_spent += this->seenMovies[i].getCost();
		return true;
	}
	return false;
}
void Box::recordprogram(const Program &program)
{
	this->toBeRecorded.push_back(program);
	sort(toBeRecorded.begin(), toBeRecorded.end());
}
void Box::cancelrecording(const Program &program)
{
	for (int i = 0; i < toBeRecorded.size(); i++)
	if (toBeRecorded[i] == program)
	{
		toBeRecorded[i] = toBeRecorded[toBeRecorded.size() - 1];
		toBeRecorded.pop_back();
		sort(toBeRecorded.begin(), toBeRecorded.end());
	}
	if (program.isRecorded())
	{
		int i = 0;
		for (; i < channels.size(); i++)
		{
			if (program.getChannel() == channels[i].getName())
				break;
		}
		for (int j = 0; j < channels[i].programs.size(); j++)
		if (channels[i].programs[j] == program)
			channels[i].programs[j].clearRecorded();
	}
}
bool Box::issettoberecorded(const Program &program)
{
	bool found = false;
	for (int i = 0; i < toBeRecorded.size(); i++)
	if (toBeRecorded[i] == program)
		found = true;
	return found;
}
float Box::moneySpent() const
{
	return total_money_spent;
}
int Box::timesWhatched(const string& title) const
{
	for (int i = 0; i < seenMovies.size(); i++)
	if (seenMovies[i].getTitle() == title)
		return seenMovies[i].gettimesRented();
	for (int i = 0; i < movieClub.size(); i++)
	if (movieClub[i].getTitle() == title)
		return 0;
	return -1;
}
float Box::getCost(const string& title) const
{
	for (int i = 0; i < seenMovies.size(); i++)
	if (seenMovies[i].getTitle() == title)
		return seenMovies[i].getCost();
	for (int i = 0; i < movieClub.size(); i++)
	if (movieClub[i].getTitle() == title)
		return movieClub[i].getCost();
	return -1;
}
bool Box::authorize(const string& s)
{
	if (s == this->password)
		authorized = true;
	return authorized;
}
bool Box::changePassword(const string& s)
{
	if (s == password)
		return false;
	else
	{
		this->password = s;
		return true;
	}
}
bool Box::createdChannel(const string& name)
{
	bool found = false;
	for (int i = 0; i < channels.size(); i++)
	if (channels[i].getName() == name)
	{
		found = true;
		break;
	}
	if (found)
		return false;
	else
	{
	Channel ctemp = Channel(name);
	this->channels.push_back(ctemp);
	sort(channels.begin(), channels.end());
	return true;
	}

}
bool Box::removeChannel(const Channel& channel)
{
	int i = 0;
	bool found = false;
	for (; i < this->channels.size(); i++)
	{
		if (channel == channels[i])
		{
			found = true;
			break;
		}
	}
	if (found)
	{
		channels[i] = channels[channels.size() - 1];
		channels.pop_back();
		sort(channels.begin(), channels.end());
		for (int i = 0; i < toBeRecorded.size(); i++)
		if (toBeRecorded[i].getChannel() == channel.getName())
		{
			toBeRecorded[i] = toBeRecorded[toBeRecorded.size() - 1];
			toBeRecorded.pop_back();
		}
		sort(toBeRecorded.begin(), toBeRecorded.end());
		return true;
	}
	else return false;
}
bool Box::updateChannel(const string& name, const Channel& channel)
{
	bool found = false;
	for (int i = 0; i < this->channels.size(); i++)
	if (channels[i].getName() == name)
	{
		found = true;
		break;
	}
	if (found)
		return false;
	else
	{
		int i = 0;
		for (; i < this->channels.size(); i++)
		if (channels[i] == channel)
			break;
		if (channels[i].setName(name))
		{
			sort(channels.begin(), channels.end());
			return true;
		}
		else return false;
	}
}
bool Box::createdProgram(const string& title, const Channel& channel)
{
	if (channel.findProgram(title)!=-1)
		return false;
	else
	{
		int i = 0;
		for (; i < channels.size(); i++)
		if (channels[i] == channel)
			break;
		channels[i].addProgram(Program(title, "none", channel.getName(), 0, "Monday", 0, 0));
		return true;
	}
}
bool Box::removeProgram(const Channel& channel, const Program& program)
{
	int i = 0;
	bool found = false;
	for (; i < this->channels.size(); i++)
	{
		if (channel == channels[i])
		{
			found = true;
			break;
		}
	}
	if (!found)
		return false;
	else {
		for (int i = 0; i < toBeRecorded.size(); i++)
		if (toBeRecorded[i]== program)
		{
			toBeRecorded[i] = toBeRecorded[toBeRecorded.size() - 1];
			toBeRecorded.pop_back();
		}
		sort(toBeRecorded.begin(), toBeRecorded.end());
		return this->channels[i].removeProgram(program);
	}
}
bool Box::updateProgram(const string& name, const string& type, int duration, const Date& date, const Program& program)
{
	Program ptemp(name, type, program.getChannel(), duration, date.getDay(), date.getHour(), date.getMinutes());
	string channel = program.getChannel();
	int i = 0;
	for (; i < channels.size(); i++)
	{
	if (channel == channels[i].getName())
		break;
	}
	for (int j = 0; j < channels[i].programs.size(); j++)
	{
	if (ptemp.overlaps(channels[i].programs[j]) && !(channels[i].programs[j]== program))
		return false;
	}
	bool changedname = !(name == program.getName());
	if (changedname)
	for (int j = 0; j < channels[i].programs.size(); j++)
	if (channels[i].programs[j].getName() == name)
		return false;
	for (int j = 0; j < channels[i].programs.size(); j++)
		if (channels[i].programs[j] == program)
	{
		channels[i].programs[j].setname(name).settype(type).setduration(duration).setdate(date);
		break;
	}
	sort(channels[i].programs.begin(), channels[i].programs.end());
	for (int n = 0; n < toBeRecorded.size(); n++)
	{
		if (toBeRecorded[n] == program)
		{
			toBeRecorded[n].setname(name).settype(type).setduration(duration).setdate(date);
			sort(toBeRecorded.begin(), toBeRecorded.end());
			break;
		}
	}
	return true;
}
bool Box::createdMovie(const string& title)
{
	bool found = false;
	for (int i = 0; i < movieClub.size(); i++)
	if (movieClub[i].getTitle() == title)
	{
		found = true;
		break;
	}
	for (int i = 0; i < seenMovies.size(); i++)
	if (seenMovies[i].getTitle() == title)
	{
		found = true;
		break;
	}
	if (found)
		return false;
	else
	{
		Movie mtemp = Movie(title, 0.0);
		movieClub.push_back(mtemp);
		sort(movieClub.begin(), movieClub.end());
		return true;
	}

}
bool Box::removeMovie(const string& title)
{
	bool found = false;
	int i = 0;
	for (; i < this->movieClub.size(); i++)
	if (movieClub[i].getTitle() == title)
	{
		found = true;
		break;
	}
	if (found)
	{
		movieClub[i] = movieClub[movieClub.size() - 1];
		movieClub.pop_back();
		sort(this->movieClub.begin(), this->movieClub.end());
		return true;
	}
	int j = 0;
	for (; j < this->seenMovies.size(); j++)
	if (seenMovies[j].getTitle() == title)
	{
		found = true;
		break;
	}
	if (found)
	{
		seenMovies[i] = seenMovies[seenMovies.size() - 1];
		seenMovies.pop_back();
		sort(this->seenMovies.begin(), this->seenMovies.end());
		return true;
	}
	else return false;

}
bool Box::updateMovie(const string& title, const Movie& movie, float cost)
{
	if (title != movie.getTitle())
	{
		bool found1 = false;
		bool found2 = false;
		int pos;
		for (int i = 0; i < movieClub.size(); i++)
		if (movieClub[i].getTitle() == title)
			return false;
		else if (movie == movieClub[i])
		{
			found1 = true;
			pos = i;
		}
		for (int i = 0; i < seenMovies.size(); i++)
		if (seenMovies[i].getTitle() == title)
			return false;
		else if (movie == seenMovies[i])
		{
			found2 = true;
			pos = i;
		}
		if (found1)
		{
			movieClub[pos] = Movie(title, cost);
			sort(movieClub.begin(), movieClub.end());
		}
		else if (found2)
		{
			seenMovies[pos] = Movie(title, cost);
			sort(seenMovies.begin(), seenMovies.end());
		}
	}
	else
	{
		bool found1 = false;
		bool found2 = false;
		int pos;
		for (int i = 0; i < movieClub.size(); i++)
			 if (movie == movieClub[i])
		{
			found1 = true;
			pos = i;
		}
		for (int i = 0; i < seenMovies.size(); i++)
		if (movie == seenMovies[i])
		{
			found2 = true;
			pos = i;
		}
		if (found1)
		{
			movieClub[pos] = Movie(title, cost);
		}
		else if (found2)
		{
			seenMovies[pos] = Movie(title, cost);
		}
	}
	return true;
}

