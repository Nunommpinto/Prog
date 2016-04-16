#include <iostream>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <iomanip>
#include <windows.h>
#include <sstream>
#include <chrono>
#include <thread>

#include "Date.h"
#include "Box.h"
#include "Cursor.h"


using namespace std;

//codigo de varias teclas
const int UP = 72;
const int DOWN = 80;
const int LEFT = 75;
const int RIGHT = 77;
const int ENTER = 13;
const int SPACE = 32;
const int BACKSPACE = 8;
const int ESCAPE = 27;

//largura do ecra
const int WIDTH = 80;

//usado para guardar o numero de '\n' ate ao fim do ecra atual
int end_of_screen = 24;
int current_color = 7; 

enum states
{
	EXIT = -1, ESCAPEMENU, MAINMENU, TVMENU, MOVIESMENU, ACTIONSMENU, WATCHMENU, LISTMENU, TRANSACTIONSMENU, RENTMENU, PASSWORDMENU, CRUDCHANNELMENU, CRUDPROGRAMMENU, CRUDMOVIEMENU, LIVEMENU, RECORDEDMENU, DAYMENU, CHANNELMENU, CHANNELMENU2, TYPEMENU, CREATECHANNEL, UPDATECHANNEL, DELETECHANNEL, CREATEPROGRAM, UPDATEPROGRAM, DELETEPROGRAM, CREATEMOVIE, UPDATEMOVIE, DELETEMOVIE
};

void ClearScreen() /* http://www.cplusplus.com/articles/4z18T05o/ */
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi)) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition(hStdOut, homeCoords);
}/* http://www.cplusplus.com/articles/4z18T05o/ */

//centra uma string
ostream& operator>>(ostream& out, string s)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	int num = (WIDTH - s.length()) / 2;
	for (int i = num; i > 0; i--)
	{
		out << ' ';
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), current_color);
	out << s;
	return out;
}
//usado em vez de endl de modo a contabilizar o numero de '\n' ate ao fim do ecra atual
ostream& el(ostream& out)
{
	end_of_screen--;
	return out << endl;
}
//move o cursor ate ao fim do ecra e restaura end_of_screen
void go_end_of_screen()
{
	while (end_of_screen > 0)
	{
		cout << el;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	end_of_screen = 24;
}
void changecolor(string color)
{
	if (color == "white")
		current_color = 7;
	else if (color == "black")
		current_color = 112;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), current_color);
}
///////////////////////////////////////
class Snake
{
	int size;
	vector<vector <int>> v;
	int dir;
public:
	Snake();
	bool endcond();
	void play();
	bool isinsnake(const vector<int>& va);

};
bool whithinbounds(const vector<int>& v1)
{
	if (v1[0] < 20 && v1[0] >= 0 && v1[1] < 20 && v1[1] >= 0)
		return true;
	else return false;
}
vector <int> gen_pellet()
{
	return{ rand() % 20, rand() % 20 };
}
Snake::Snake()
{
	(this->v).push_back({rand()%10+5,rand()%10+5});
	int op[] = { UP, DOWN, LEFT, RIGHT };
	this->dir = op[rand()%4];
	this->size = 1;
}
bool Snake::isinsnake(const vector<int>& va)
{
	for (int i = 0; i < v.size(); i++)
	if (va == v[i])
		return true;
	return false;
}
bool Snake::endcond()
{
	for (int i = 0; i < (this->v).size() - 1; i++)
	for (int j = i + 1; j < v.size(); j++)
	if (v[i] == v[j])
		return true;
	return false;
}
void Snake::play()
{
	ClearScreen();
	vector<int> pellet = gen_pellet();
	unsigned char input = 0;
	bool more = true;
	while (!(this->endcond()) && more)
	{
		
		if (input == 224)
			input = _getch();
		if (input == UP)
		{
			if (dir != DOWN)
				this->dir = UP;
		}
		else if (input == DOWN)
		{
			if (dir != UP)
				this->dir = DOWN;
		}
		else if (input == LEFT)
		{
			if (dir != RIGHT)
				this->dir = LEFT;
		}
		else if (input == RIGHT)
		{
			if (dir != LEFT)
				this->dir = RIGHT;
		}
		else if (input == ESCAPE)
		{
			more = false;
		}
		cout << el << el;
		for (int i = 0; i < 20; i++)
		{
			cout << "                    ";
			for (int j = 0; j < 20; j++)
			{
				vector <int> v_temp = { i, j };

				if (v_temp == pellet)
					cout << "@ ";
				else if (this->isinsnake(v_temp))
				{
					cout << "# ";
				}
				else cout << "- ";
			}
			cout << el;
		}
		vector<int> last = v[v.size() - 1];
		vector <int> previous = last;
		if (dir == UP)
		{
			last[0]--;
			if (whithinbounds(last))
				v.push_back(last);
			else more = false;
		}
		else if (dir == DOWN)
		{
			last[0]++;
			if (whithinbounds(last))
				v.push_back(last);
			else more = false;
		}
		else if (dir == LEFT)
		{
			last[1]--;
			if (whithinbounds(last))
				v.push_back(last);
			else more = false;
		}
		else if (dir == RIGHT)
		{
			last[1]++;
			if (whithinbounds(last))
				v.push_back(last);
			else more = false;
		}
		if (last == pellet)
		{
			size++;
			pellet = gen_pellet();
		}
		while (v.size() > size)
		{
			v.erase(v.begin(), v.begin() + 1);
		}
		int t1 = clock();
		while (true)
		{
			int t2 = clock();
			double speed = (this->size*(-.4 / 9) + .5);
			if (speed < .05)
				speed = .05;
			if ((t2 - t1) / double(CLOCKS_PER_SEC) > speed)
				break;
			if (_kbhit())
				input = _getch();
		}
		go_end_of_screen();
		ClearScreen();
	}
	cout << el << el;
	for (int i = 0; i < 20; i++)
	{
		cout << "                    ";
		for (int j = 0; j < 20; j++)
		{
			vector <int> v_temp = { i, j };

			if (v_temp == pellet)
				cout << "@ ";
			else if (this->isinsnake(v_temp))
				cout << "X ";
			else cout << "- ";
		}
		cout << el;
	}
	cout << el >> "Your score was " + to_string(size-1);
	go_end_of_screen();
	_getch();
}
/////////////////////////////////////
//separa uma string num vector em que cada elemento é uma linha da string original
vector<string> sep_string(string s_in)
{
	vector<string> v_out;
	string s;
	for (int i = 0; i < s_in.length(); i++)
	{
		if (s_in[i] == '\n')
		{
			v_out.push_back(s);
			s.clear();
		}
		else s += s_in[i];
	}
	return v_out;
}
vector<vector <string>> prep_intro(const vector<string>& v_in)
{
	vector<vector <string>> v_out;
	for (int i = 0; i < v_in.size(); i++)
	{
		vector<string> v = sep_string(v_in[i]);
		v_out.push_back(v);
	}
	return v_out;
}
void intro()
{

	string hello = " __  __          ___    ___           __     \n/\\ \\/\\ \\        /\\_ \\  /\\_ \\         /\\ \\    \n\\ \\ \\_\\ \\     __\\//\\ \\ \\//\\ \\     ___\\ \\ \\   \n \\ \\  _  \\  /'__`\\\\ \\ \\  \\ \\ \\   / __`\\ \\ \\  \n  \\ \\ \\ \\ \\/\\  __/ \\_\\ \\_ \\_\\ \\_/\\ \\L\\ \\ \\_\\ \n   \\ \\_\\ \\_\\ \\____\\/\\____\\/\\____\\ \\____/\\/\\_\\\n    \\/_/\\/_/\\/____/\\/____/\\/____/\\/___/  \\/_/\n";
	string welcome = " __      __          ___                                        \n/\\ \\  __/\\ \\        /\\_ \\                                       \n\\ \\ \\/\\ \\ \\ \\     __\\//\\ \\     ___    ___     ___ ___      __   \n \\ \\ \\ \\ \\ \\ \\  /'__`\\\\ \\ \\   /'___\\ / __`\\ /' __` __`\\  /'__`\\ \n  \\ \\ \\_/ \\_\\ \\/\\  __/ \\_\\ \\_/\\ \\__//\\ \\L\\ \\/\\ \\/\\ \\/\\ \\/\\  __/ \n   \\ `\\___ ___/\\ \\____\\/\\____\\ \\____\\ \\____/\\ \\_\\ \\_\\ \\_\\ \\____\\\n    '\\/__//__/  \\/____/\\/____/\\/____/\\/___/  \\/_/\\/_/\\/_/\\/____/\n\n";
	string tothe = " __               __    __                            \n/\\ \\__           /\\ \\__/\\ \\                           \n\\ \\ ,_\\   ___    \\ \\ ,_\\ \\ \\___      __               \n \\ \\ \\/  / __`\\   \\ \\ \\/\\ \\  _ `\\  /'__`\\             \n  \\ \\ \\_/\\ \\L\\ \\   \\ \\ \\_\\ \\ \\ \\ \\/\\  __/  __  __  __ \n   \\ \\__\\ \\____/    \\ \\__\\\\ \\_\\ \\_\\ \\____\\/\\_\\/\\_\\/\\_\\\n    \\/__/\\/___/      \\/__/ \\/_/\\/_/\\/____/\\/_/\\/_/\\/_/\n";
	//http://www.network-science.de/ascii/
	vector <string> out;
	vector <vector<string>> v = prep_intro({ hello});
	cout << el;
	bool skip = false;

	v = prep_intro({welcome, tothe });
	for (int i = 0; i < v.size(); i++)
	{
		ClearScreen();
		cout << el << el << el << el << el << el << el<<el;
		for (int j = 0; j < v[i].size(); j++)
		{
			if (_kbhit())
			{
				skip = true;
				break;
			}
			cout >> v[i][j] << el;
			this_thread::sleep_for(chrono::milliseconds(50));
		}
		if (_kbhit())
			break;
		if(i != v.size()-1)
			cout << el;
		this_thread::sleep_for(chrono::milliseconds(500));
		go_end_of_screen();
	}
	
	if (!skip)
	{
	int t;
	ClearScreen();
	string box = "                  ____.----.                                 \n        ____.----'          \\                                \n        \\                    \\                               \n         \\                    \\                              \n          \\                    \\                             \n           \\          ____.----'`--.__                       \n            \\___.----'          |     `--.____               \n           /`-._                |       __.-' \\              \n          /     `-._            ___.---'       \\             \n         /          `-.____.---'                \\            \n        /            / | \\                       \\           \n       /            /  |  \\                   _.--'          \n       `-.         /   |   \\            __.--'               \n          `-._    /    |    \\     __.--'     |               \n            | `-./     |     \\_.-'           |               \n            |          |                     |               \n            |          |                     |               \n     _______|          |                     |_______________\n            `-.        |                  _.-'               \n               `-.     |           __..--'                   \n                  `-.  |      __.-'                          \n                     `-|__.--'                               \n\n"; //http://www.chris.com/ascii/index.php?art=objects/boxes
	v = prep_intro({box});
	cout << el;
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v[i].size(); j++)
		{
			if (_kbhit())
			{
				skip = true;
				break;
			}
			cout >> v[i][j] << el;
			this_thread::sleep_for(chrono::milliseconds(50));
		}
		if (_kbhit())
		{
			skip = true;
			break;
		}
		if (i != v.size() - 1)
			cout << el;
		this_thread::sleep_for(chrono::milliseconds(300));
	}
	go_end_of_screen();
	t = time(NULL);
	while (true)
	{
		if (_kbhit())
			break;
		int t2 = time(NULL);
		if (t2 - t > 1)
			break;
	}
	}
	if (skip)
	{
		go_end_of_screen();
		_getch();
	}
	ClearScreen();
}
void outro()
{
	string bye = " , __                    \n/|/  \\            |  |  |\n | __/       _    |  |  |\n |   \\|   | |/    |  |  |\n |(__/ \\_/|/|__/  o  o  o\n         /|              \n         \\|              \n";
	vector <string> v = sep_string(bye);
	cout << el << el << el << el << el << el << el << el << el;
	for (int i = 0; i < v.size(); i++)
	{
		if (_kbhit())
			break;
		cout >> v[i] << el;
		this_thread::sleep_for(chrono::milliseconds(25));
	}
	go_end_of_screen();
	int t = time(NULL);
	while (true)
	{
		if (_kbhit())
			break;
		int t2 = time(NULL);
		if (t2 - t > 2)
			break;
	}
}
Date currentDate()
{
	string days[] = {"Monday","Tuesday","Wednesday","Thursday","Friday","Saturday","Sunday"};
	int hour = rand() % 24;
	int minute = rand() % 60;
	return Date(days[rand() % 7], hour, minute);
}
string f_to_string(float n)
{
	ostringstream outstr;
	outstr << n;
	return outstr.str();
}
string my_to_string(float n)
{
	string s_out = f_to_string(float(floor(n * 100 + .5) / 100));
	bool found = false;
	for (int i = s_out.length() - 1; i > 0; i--)
	if (s_out[i] == '.')
		found = true;
	if (!found)
		s_out += ".00";
	return s_out;
}
Date int_to_date(int num)
{
	string days[] = { "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	int minutes = num % 60;
	num = num - minutes;
	int hours = (num % (24 * 60)) / 60;
	num = num - hours *60;
	int day = (num / (24 * 60)) % 7;
	string d = days[day];
	return Date(d, hours, minutes);
}
string date_to_string(const Date& date)
{
	string minutes = to_string(date.getMinutes());
	string hour = to_string(date.getHour());
	if (minutes.length() == 1)
		minutes = "0" + minutes;
	if (hour.length() == 1)
		hour = "0" + hour;
	string time = date.getDay().substr(0, 3) + " " + hour + ":" + minutes;
	return time;
}
string normalize(const string& s1, const string& s2, int n)
{
	string s_out = s1;
	s_out.resize(n);
	int j = s_out.length() - 1;
	for (int i = s2.length() - 1; i >= 0; i--)
	{
		s_out[j] = s2[i];
		j--;
	}
	return s_out;
}
void genericmenu(int &state, Box &box, string name, string text, vector<string> options, vector<int> results)
{
	Cursor cursor1(options.size()-1);
	bool more = true;
	bool change = false;
	while (more)
	{
		change = false;
		cout << el >> name << el << el >> text << el << el << el << el;
		if(options.size() < 5 )
			cout << el;
		if (options.size() < 4)
			cout << el;
		if (options.size() < 3)
			cout << el << el;
		for (int i = 0; i < options.size(); i++)
		if (cursor1.getPosition() == i)
		{
			changecolor("black");
			cout >> "> " + options[i] << el << el << el;
			changecolor("white");
		}
		else cout >> options[i] << el << el << el;
		go_end_of_screen();
		cout >> date_to_string(box.getDate() )<< "                                   ";
		while (!change)
		{
			char input = _getch();
			if (input == UP || input == LEFT)
				change = cursor1.up();
			else if (input == DOWN || input == RIGHT)
				change = cursor1.down();
			else if (input == ESCAPE||input ==BACKSPACE)
			{
				change = true;
				more = false;
				state = results[results.size()-1];
			}
			else if (input == ENTER || input == SPACE)
				{
					change = true;
					more = false;
					state = results[cursor1.getPosition()];
				}
		}
	ClearScreen();
	}
}
void myinput(string &in, bool &change, bool &more,bool &ignore, int &state, int previousstate,Box &box, int &numtries)
{
	unsigned char input = _getch();
	if (input == 224)
	{
		ignore = true;
		change = false;
	}
	else
	if (input == ESCAPE)
	{
		state = previousstate;
		more = false;
		change = true;
	}
	else if (input == ENTER)
	{
		if (in == "snake")
		{
			Snake s1;
			s1.play();
		}
		if (box.authorize(in))
		{
			more = false;
			change = true;
		}
		else
		{
			if (in != "")
			{
				change = true;
				in.clear();
				numtries++;
			}
			else change = false;
		}
	}
	else if (input == BACKSPACE)
	{
		if (in == "")
			change = false;
		else
		{
			in = in.substr(0, in.length() - 1);
			change = true;
		}
	}
	else if (ignore)
	{
		ignore = false;
		change = false;
	}
	else
	{
		in += input;
		change = true;
	}
}
void insertpassword(Box &box, int &state)
{
	string in;
	bool more = true;
	int numtries = 0;
	bool ignore = false;
	while (more)
	{
		string message;
		if (numtries == 0)
			message = "Please insert password (or press ESC to go back):";
		else if(numtries < 10)
			message = "Please try again (or press ESC to go back):";
		else message = "Just press ESC";
		cout << el << el << el >> message << el << el << el << el << el << el << el << el << el;
		string hidden= in;
		fill(hidden.begin(), hidden.end(), '*');
		cout >> "> " + hidden;
		go_end_of_screen();
		bool change = false;
		bool ignore = false;
		while (!change)
		{
			myinput(in, change, more, ignore, state, MAINMENU, box, numtries);
		}
		ClearScreen();
	}
	ClearScreen();
}
void myinput(string &in, string &in2, bool &change, bool &more, bool &ignore, int &state, int previousstate, Box &box, int &numtries, int &phase)
{
	unsigned char input = _getch();
	if (input == 224)
	{
		ignore = true;
		change = false;
	}
	else
	if (input == ESCAPE)
	{
		if (phase == 1)
		{
		state = previousstate;
		more = false;
		change = true;
		}
		else if (phase == 2)
		{
			change = true;
			phase = 1;
			in2.clear();
		}
	}
	else if (input == ENTER)
	{
		if ((in == "" && phase == 1) || (in2 == "" && phase == 2))
			change = false;
		else if (phase == 1)
		{
			phase = 2;
			change = true;
		}
		else if (phase = 2)
		{
			if (in == "snake")
			{
				ClearScreen();
				more = false;
				change = true;
				cout << el << el << el << el << el << el << el << el << el << el << el >> "Error! Invalid password ;D";
				state = PASSWORDMENU;
				go_end_of_screen();
				_getch();
			}
			else if (in == in2)
			{
				bool success = box.changePassword(in);
				more = false;
				change = true;
				state = ACTIONSMENU;
				ClearScreen();
				if (success)
				cout << el << el << el << el << el << el << el << el << el << el << el >> "Password changed successfuly";
				else 
				{
					cout << el << el << el << el << el << el << el << el << el << el << el >> "New Password must differ from the old one";
					state = PASSWORDMENU;
				}
				go_end_of_screen();
				_getch();
			}
			else
			{
				change = true;
				numtries++;
				in.clear();
				in2.clear();
				phase = 1;
			}
		}
			
	}
	else if (input == BACKSPACE)
	{
		if (phase == 1)
		{
			if (in == "")
				change = false;
			else
			{
				in = in.substr(0, in.length() - 1);
				change = true;
			}
		}
		else if (phase == 2)
		{
			if (in2 == "")
				change = false;
			else
			{
				in2 = in2.substr(0, in2.length() - 1);
				change = true;
			}
		}
	}
	else if (ignore)
	{
		ignore = false;
		change = false;
	}
	else
	{
		if (phase == 1)
		in += input;
		else if (phase ==2)
			in2 += input;
		change = true;
	}
}
void changepassword(Box &box, int &state)
{
	string in;
	string in2;
	int phase = 1;
	bool more = true;
	int numtries = 0;
	while (more)
	{
		if (phase == 1)
		{
			string message;
			if (numtries == 0)
				message = "Please insert the new password (or press ESC to go back):";
			else message = "Passwords do not match, please try again (or press ESC to go back):";
			cout << el >> "Privileged Actions/Change Password" << el << el >> message  << el << el << el << el;
			string hidden = in;
			fill(hidden.begin(), hidden.end(), '*');
			cout >> "> " + hidden;
			go_end_of_screen();
			bool change = false;
			bool ignore = false;
			while (!change)
			{
				myinput(in,in2, change, more, ignore, state, ACTIONSMENU, box, numtries,phase);
			}
			ClearScreen();
		}
		else
		{
			string message;
			if (numtries == 0)
				message = "Please insert the new password (or press ESC to go back):";
			else message = "Passwords do not match, please try again (or press ESC to go back):";
			cout << el >> "Privileged Actions/Change Password" << el << el >> message << el << el << el << el;
			string hidden = in;
			fill(hidden.begin(), hidden.end(), '*');
			cout >> hidden;
			string message2 = "Please confirm the password";
			cout << el <<el << el << el >> message2 << el << el << el << el;
			string hidden2 = in2;
			fill(hidden2.begin(), hidden2.end(), '*');
			cout >> "> " + hidden2;
			go_end_of_screen();
			bool change = false;
			bool ignore = false;
			while (!change)
			{
				myinput(in, in2, change, more, ignore, state, ACTIONSMENU, box, numtries, phase);
			}
			ClearScreen();
		}
	}
	ClearScreen();
}
void transactionsmenu(Box &box, int &state)
{
	string in;
	bool more = true;
	int numtries = 0;
	while (more)
	{
		string message;
		if (numtries == 0)
			message = "Insert the title of a movie (ESC to go back):";
		else if (numtries == -1)
			message = "That movie does not exist in the database";
		else message = "You have not yet rented that movie";
		cout << el >> "Movies/Transactions" << el << el >> message << el << el >> "Total: " + my_to_string(box.moneySpent()) + " $" << el << el << el << el << el << el << el;
		cout >> "> " + in;
		unsigned char input;
		bool change = false;
		bool ignore = false;
		while (!change)
		{
		input = _getch();
		if (input == 224)
		{
			ignore = true;
			change = false;
		}
		else if (input == ESCAPE)
		{
			state = MOVIESMENU;
			change = true;
			more = false;
		}
		else if (input == ENTER)
		{
			if (in == "")
				change = false;
			else
			{
			int num = box.timesWhatched(in);
			if (num == -1)
			{
				numtries = -1;
				in.clear();
				change = true;
			}
			else if (num == 0)
			{
				numtries = 1;
				in.clear();
				change = true;
			}
			else
			{
				change = true;
				string s;
				if (num == 1)
					s = "once";
				else if (num == 2)
					s = "twice";
				else s = to_string(num) + " times";
				cout << el << el << el <<el<<el<<el >> in + " has been rented " + s;
				cout << el << el >> "At " + my_to_string(box.getCost(in)) + " $ per view that amounts to " + my_to_string(box.getCost(in)*num) +" $";
				go_end_of_screen();
				in.clear();
				_getch();
				ClearScreen();
				numtries = 0;
				
			}
			}
		}
		else if (input == BACKSPACE)
		{
			if (in != "")
			{
			in = in.substr(0, in.length() - 1);
			change = true;
			}
			else change = false;
		}
		else 
		if (ignore)
		{
			change = false;
			ignore = false;
		}
		else 
		{
			in += input;
			change = true;
		}
		}
		go_end_of_screen();
		ClearScreen();
	}
	ClearScreen();
}
string movie_to_string(const Movie& movie)
{
	int length = 40;
	string title = movie.getTitle();
	string cost = my_to_string(movie.getCost())+ " $";
	string num = to_string(movie.gettimesRented());
	string s_out;
	s_out.resize(40);
	for (int i = 0; i < title.length(); i++)
		s_out[i] = title[i];
	int j = s_out.length() - 1;
	for (int i = cost.length() - 1; i>=0; i--)
	{
		s_out[j] = cost[i];
		j--;
	}
	return s_out;
}
vector<vector<string>> prep_list(const vector<Movie>& movies)
{
	vector<vector<string>> v_out;
	int size = 7;
	vector<string> v;
	for (int i = 0; i < movies.size(); i++)
	{
		if (v.size() == size)
		{
			v_out.push_back(v);
			v.clear();
		}
		string s = movie_to_string(movies[i]);
		v.push_back(s);
	}
	if (v.size() != 0)
		v_out.push_back(v);
	return v_out;
}
string retrieve_title(string s)
{
	string s_out;
	for (int i = 0; s[i]!=0; i++)
		s_out += s[i];
	return s_out;
}
void watch(string program)
{
	ClearScreen();
	cout << el << el << el << el << el << el << el << el << el << el << el >> "Please pretend to be watching " + program <<el;
	cout >> "(Here's some help! :D)";
	go_end_of_screen();
	unsigned char c;
	do
	{
		c = _getch();
	} while (c==224);
	string horse[] = {
		"IIIII7IIII7IIIIII7I77I7I777777IIIII?:~?II7777III77III7777II7\nIIIIIIIIIIIIII7IIIIIIIIIII7III7IIII?,,?7I7IIIIIIIII7IIIIIIII\nIIIIIIIIIII7IIIIII7IIIIIIIIIII77I?:=,,7IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIII7IIIIIIII:~~~777IIIIIIIIII,IIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII::,:,=~7IIIIII?,,,,:,~IIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?~,,::~~~IIII++,,,:,,,,:+IIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIII77I:,.+,,:,::,,:,,::,IIIII\nIIIIIIIIIIIIIIIIII=:,:,,,.~.::~IIII,,,,,,,:,,::,~:,,,::,IIII\nIIIII=+?IIIIIII+,,:,,.,,,,,,.,,,,~III+~:==:,:?:+7777:=:~,III\nIIII=,,,,,,,,:I~,,,,,,,:::~~=~==.~,II==+~?~==~=7777777I=IIII\nIIII+::,,IIIIII:,,::,,~,,,,,,:::,==II==+=,:~,+7IIIII77I7IIII\nIIIII:=IIIIIIII~,,...,,~,..,::,,~:,.,,,::==:+7IIIIIIIIIIIIII\nIIIIIIIIIIIIIIIII:,....,.,,,,,.,:,....:,~=,77IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIII7I:,,,,,777:,,,,.,,:..,~:+77IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIII,,,=I,,?7I7I777?.:,,,.,,I7IIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIII+,:=7I,,=7IIIIIIIIII,::,,77IIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIII:,=.,IIIIIIIIIII,,~7+::IIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIII7+~:,,~IIIIIII..?7I77==IIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIII7~7II:,:III~,,IIIII,~IIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIII?+:,,:::.,II+,:~77IIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIII7II7IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\nIIIIII?IIIIIIIIIII??IIIIII??II?IIIIIIIIIIIIIIIIIIIIIIIII??I?\n~~=======++++===~==+++++?+?+?????++????+++++++++++++=+=~====\n",
		"IIIIIIII777II77777I7IIIIII7IIIII7III,::II7II7777IIIIIIIIII7I\nIIIIIIIII7I7777777IIIIIIIIIIIIIIII?~,,~IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII+,:+:77IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII+,,,~=,77IIIIIII+??,=IIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,,,,,,:IIIIIIII,,,,~,:~IIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII7+:77,:=I=,,:,,,,,,,,,,IIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII=III77I7:,,?~,,,,,,,,,,,,,IIIII\nIIIIIIIIIIIIIIIII?,,,:,,,,,,::,:IIII,,,,,.,,,,,,~:,,,,,,IIII\nIIIII~~????IIII::::,,,:::,,,,,,,.:III?:,,+~:,~:=7I77I~~~,~II\nIII+,,,,,,,::~+:::,,,,::::~~=:~=,=~II=:+:+:,=~:7II77I77:IIII\nIIII=,,:IIII?+I:,,,,,,,::,,,,,,,,~.II~~+::::,=7IIIIIIIIIIIII\nIIIII~IIIIIIIII:,,,,,,::,,,,,,,,:,,.~,,,:~+:?IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIII~,,,,,,,,,,,,,,:,,,,,,,:~:,IIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIII::,,,~7I,,,,,,,,:,,,,:~IIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIII,,:,,=IIII7I~,,,,,,,,IIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIII+::I7:,?IIIIIII7I,,,:,,,~IIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIII=:::+::+??IIIIII~,,I77III,,IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIII:,:+,+IIIII,,7IIIIII?.IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIII7I~,,,,III,:IIIIII,=IIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIII?II~,,:,,IIIII+,IIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII=IIIII+IIIIIIIIIIIIIIIIIIIII\n?IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIIIIIIII\n?IIIIIIIIIIIIIIIIIIIIIIIIII??IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n???I????????IIIIIIIIIIIIII?????IIIIIIIIIIIIIIIIII?IIIIIIIIII\n~~~~~===~~========+?+++++++???+???+??+??+???+?++++=?+???????\n",
		"IIIII777II7I777777777777777777777777,,,=77+77777777777777777\nIIIIIIIIIIIIIIIII7II7II7777777777II,:,,777?77777777777777777\nIIIIIIIIIIIIIIII7IIIII77777777777,:~~~I777?77777777777777777\nIIIIIIIIIIIIIIIIII7IIIIII777777.,,:~:77777I77777777777777777\nIIIIIIIIIIIIIIIIIII7II7IIII7I=,,,,,,,I7777III+,,,:::I7777777\nIIIIIIIIIIIIIIIII7I7IIIIII7I7??I:.7I,=?7:,,,..,,:::,~:777777\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII=~?III77,..+...:,,,,,:::~,77777\nII?III?IIIIIIIIIII=..,,.....:.,:??I+......,.,.,,.::::::,II7I\nIIIIII~+???IIIII,,,..,,,,,.......=?II=:~,=:..:~:777=,,:,,I77\nIII?..,.,,.,,,.,,:,,,,,,:,:~::~=,=,?I,,,=~:,~:,7777777:~:I77\nIIII~..,,?+?,,I,,,,,,,,,:,,,,::,,~,?I:==,:,::,777777777777I7\nII?II=:IIIIIIII,,,,,,,:,,,,,,,,:~:..:,,,~:+:.77777777777777I\nIII?IIIIIIIIIIII+,:.,.,,,:,,,,,:,,.,.,,,,::,7777777I7777I77I\nIIIIIIIIIIIIIIIII77,,,,,,=?,,,,.,,,.,,,,,,:77777777I7777777I\nII?IIIIIIIIIIIIIIII77?,,,~I7777,,,.,.....,:77I7777I7I777I7II\nIIIIIIIIIIIIIIIIIII7I:,,,.III77777I.,.:.,,::,:II777II7IIIIII\nIIIIIIIIIIIIIIIIIIIII:,.:,IIIII=IIII,.II77777I,7IIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIII77+,,..,,7?+III~,?I77777I=I7IIIII7IIIII\n?IIIIIIIIIIIIIIIIIIIIIIII:,III,I~,II7::7I7I7I:IIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIII777,,?I+I77I:.?IIIIII.IIIIIIIIIIIIII\nI?IIIIIIIIIIIIIIIIIIIIIIIIII7,,?IIIII77III+I~77IIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII7:,II7IIIIII+IIIIIIIIIIIIIIIII\n?I??IIIIIIIIIIIIIIIIIIIIIII+?II7I?IIIIIIII=IIIIIIIIIIIIIIIII\n+====+?IIIIIIIIII?IIIIIII++++?IIIIIIIIIII7=IIIIIIIIIIIIIIIII\n~~~~~~:~~=:~~~~~~~~~~=~~~====++===~=====~~~=++?++?=+=++==+==\n",
		"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,:?IIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?,,,IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII:,:,,~IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII:,.~=:IIIIIIIIIIII?IIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?,,:,.:~IIIIIIIII?~+:..+IIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?,,+,:~II+I:,,.,,.,:,,:IIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII??IIIII:~,,,:,.,:,:,,,,,..IIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIII:II=+III.,,:,:=~::,::,.,.,,,:IIII\nIIIIIIIIIIIIIIIII=.,,.,,:,....,.:=II++~:,:,,,:~,=II,,.,,~III\nIIIII.:~,,.,.,+.,,,.,:,::,:=~~~:,~=I?:+,==,::::7III?+,::,:II\nIIIII::::~:I:~I?,,.,..~~,,,,,~~~:=+II+++~,,,~,IIIIIIIII:+III\nIIIIII:=IIIIIII~:.,,.,.,.,.,,,::?~..=~,:~:~::IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIII,,,.,...,,.,..::~,,,,:,::::=~IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIII,,,,,.,,,.,.,,.,,,,,~:::::=IIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIII.,,,,.,III+,,,,,,,,,,,,,?IIIIIIII?IIIIII?\nIIIIIIIIIIIIIIIIIIIII,,.,:IIIIIIIII=,,,::,:::,,~IIIIIIIIIIII\n??????????????I?I?I?I?,,,:IIIIIIIIIIII~,,IIIIII,~III?III?I?I\nIIIIIIIIIIIIII?IIIIIII:,,,:.=IIIIII=~IIII,+IIIII,IIIIII???II\nIIIIIIII?IIIIII??I?I?I,,?III?=:,,~II,?II~,?IIIII+=II??I??I?I\nIIIII?III?III???I?I??I+,:IIIIIIII~=II:~IIIIIIIII?,IIIIIIIIII\nIIIIIII?III?IIIII????III,:IIIIIIIIIIIIIIIIIIIIII+?IIIIIIIIII\nIIIIII?I?????????I???IIII,.???IIII?IIIIIIIIIIIIII+IIIIIIIII?\nIIIIIIIII?III????I????IIII,.=II?III?IIIIIIIIIIIIIIIIIII??I??\nII?IIIII??II?I???????????=+?.~??II???IIIIIIII?IIIIIIIII????I\n,++=+==++===~~~~~~=~~~~+==+=+=~=~~=~+++=+++++==~===~~==~~~::\n",
		"IIIII7II7IIIIIIII7IIII77II77?IIIIIIII...IIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIIIII:...IIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIII.,:..IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII?II7...~:77IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII?I,,,...,,IIIIIIIIII+II,~IIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII?III,,.?:,.IIIII..........IIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII77I7,..=,.............IIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?~III?......,.............IIII\nIIIIIIIIIIIIIIIIII+,..,....,,:,..~?I~~,=..,.............:III\nIIIII~,,,,,,.:?I.,,,,.,,,:,,,:::,.:II,,,~:::.:,.II++I....III\nIIIII+,,,,:7~,7I,:,..,::::~,,,:~,??II,:~=,,,...7IIIIII7..:II\nIIIIII,,7I7IIIII:,..,,,:,,.,,,,:~=...,,..::.:.IIIIIIIII7IIII\nIIIIII7IIIIIIIII,.,,.,,..,.,.,,.,:..,,,,,,,,.IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIII,,.,..,,:.,.,,..:...~,::.,,,IIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIII=,,,,,,,,I7I~,,,,,,,...,,.,?IIIIIIIIIIIII?I\nIIIIIIIIIIIIIIIIII=,,,,=,,=IIIIIIIIII,,,,,.,:::IIIIIIIIIII?I\nIIIIIIIIIIIIIIIIIII::,II:,,IIIIIIIIIIIIIIII7=,,...?IIIIIII??\nIIIIIIIIIIIIIIIIII,::7III::?IIIIIIIIIIIIIIII=+III:.IIIIIII?I\nIIIIIIIIIIIIIIIII::~7IIIII:,,?IIIIIIIII++I?.?IIIIII.+IIIIIII\nIIIIIIIIIIIIIIIII::IIIIIIIIII~,,+IIIIIII??:IIIIIIIII.?IIIII?\nIIIIIIIIIIIIIIIIII:IIIIIIIIIIIII~,:IIIIIIIIIIIIIIIIII~?II???\nIIIIIIIIIIIIIIIIII=:IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII????\nIIIIIIIIIIIIIIIIIII~?IIIIII?+IIIIIIIIIIIIIIIIIIIIIIIIIII????\nIIIIIIIIIIIIIIIIIII+:,+?I++=~=?I????????IIIII?III?III???????\n+?+++=++++++++==+?+??????=+???+++++=+=++~======+=~~~~::,,::,\n",
		"IIIIIIIIIIIIIIIIIIIIIII77I7IIII7IIIII,,,?IIIIIIIIIIIIIIII7II\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,,,777IIIIIIII7IIIIIII7\nIIIIIIIIIIIIIIIIIIII7IIII7IIIIIIII..,,,77IIIIII7IIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII...,:,I7IIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII.,,,,,,:=7IIIIIIIII+?II+IIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?,,II,:,II?II?,,,,,,,..~IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII??I77777:.,,,...,,,,,,,.,,~III\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII7:?I7I,.,,,,.,:,:~:,,,,,,,,III\nIIIIIIIIIIIIIIIIIII:,,,.,...,~,.,:?I?~~:,,:~~,,::,,,,,,,,,II\nIIIII,.,,.,,,.~+...,,,,,,,,..,,:,,,II~:==~,~:,:~:7I~I=,,,,+I\nIIIIII.,,::+,.?7..,.,,::,.,~,.:::~?II,:::,~,,~,~7777777=:::?\nIIIIII,=7IIIIII+,...,,,,,,,,..,::=:..,,:,,,,,:?7II7I7777,+,I\nIIIIIIIIIIIIIII,....,,.,,,..,.,.,,,,.:.,::,::,7I7IIIIIII777I\nIIIIIIIIIIIIIII~...,.,..,:...,,.,.,,.~,,~~,,,=IIIIIIIIIII7II\nIIIIIIIIIIIIIII?,...,,,,,?777?:,:,,,,:,,,,,.:,=IIIIIIIIIIIII\nIIIIIIIIIIIIIIII,....~,,,II7I7777777777?:,,,,,...IIIIIIIIIII\nIIIIIIIIIIIIIII,.,,.77I:,+IIII7II7I77II777777I.,,IIIIIIIIIII\nIIIIIIIIIIIII~..,.777I7,,?IIII7IIII7III7I77777I,~.,IIIIIIIII\nIIIIIIIIIIII,,,:77777I7~,~IIIIIIIIIIIIIII7III?,7II7,:IIIIIII\nIIIIIIIIIIII~,IIIIIIIIII,,IIIIIIIIIIIIIIIIII=,77IIII?.~IIIII\nIIIIIIIIIIII:,IIIIIII?II7I,.IIIIIIIIIIIIIIII=IIIIIIIIII,=III\nIIIIIIIIIIII,:IIIIIIIIIII77I.,IIIIIIIIIIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIII:~IIIIIII?IIIIIII?..IIIIIIIIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIII:,~??III=???++===I:+IIIIIIIIIIIIIIIIIIIIIIIIIII\n~~~~=~==~==~++++==~~=++=+=++++=?+~=~~~~====~=~===~~==~~==~~~\n",
		"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII7,,,,77IIII7I77III7I7IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,:?777I7IIII7I7IIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII.,+,,7I7I7IIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII7,..:~=7IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,,,..,~?IIIIIIIIIII?IIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,..II:,,IIIIII:,........IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII7II7I,.......,,........III\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII:?III+.,.......,.:,.....,..+II\nIIIIIIIIIIIIIIIIIII+,.,~+?I,=?+:~?II~~=~.~:~..,,:...,.....II\nIIIII,,,....,,,,,,,,,,,,,:,......,III:=:++.,..,,.~~+7.....?I\nIIIII.,.:=:?,II,::,..,,.:,.,.....III?:.,,..:,,,~7IIII7I....I\nIIIII~~IIIIIII?.....,,...,.......,=I?,..:..,.,,IIIIIII7I...=\nIIIIIIIIIIIIII.......,,.,,...,......:,...,...,7IIIIIIII7777I\nIIIIIIIIIIIIII,,.....,,,....,..,...,:,...:...IIIIIIIIIIIIIII\nIIIIIIIIIIIII?.........,III7I~,,,,::+,:..........?IIIIIIIIII\nIIIIIIIIIIIII+,.,.,...,?IIIIIIIIIIII7I77I.:,.,~?,.+IIIIIIIII\nIIIIIIIIIIII..,,?II,..,IIIIIIIIIIIIIIIIIIIII7..II.IIIIIIIIII\nIIIIIIIIII.,,,IIIII.,.IIIIIIIIIIIIIIIIIIIIIIII....IIIIIIIIII\nIIIIIIIII,,,IIIIII..,IIIIIIIIIIIIIIIIIIIIIIIIIII..IIIIIIIIII\nIIIIIIII:,IIIIIII~,,IIIIIIIIIIIIIIIIIIIIIIIIIIII+,.IIIIIIIII\nIIIIIII,,IIIIIIIII+,IIIIIIIIIIIIIIIIIIIIIIIIIIII:II..III?III\nIIIIII,:IIIIIIIIIII+,IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,..III?I\nIIIIII.,IIIIIIIIIIIII.IIII?IIIIIIII?IIIIIIIIIIIIIIIIII:?IIII\nI??IIII+??++I?+??????~...?++?IIIIIIIIIIIIIIIIIIIIIIIIIIIIII?\n=++=+=+?++=~===~===+=+++=~=+=++===:~::~=~~~~=======~:~:::,::\n",
		"IIIIIIIIIIIIII7IIIIIIIII777777I7777I.,,I77777777777777777777\nIIIIIIIIIIIIIIIIIIIIIIIIII7777777III,,,777777777777777777777\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII7IIII,,:.I77I777777777777777777\nIIIIIIIIIIIIIIIIIIIIIIIIIIII7III..,:~7777I7I7777777777777777\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,,,,.,:?7II7II7I7IIII77777777\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?~,,,77:,,I7IIII:.,,,.,I::I777\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII7I7I777+,,,,,,,~~::~:,,,=777\nIIIIIII?IIIIIIIIIIIIIIIIIIIIII+IIII+.,,,,.,,,::~~:,,,,::,III\nIIII???=+???I?IIIIII?+?IIII+?II?+II=:~+=:~,~~::=~:,,,,::::II\nIIII:..............,,,,,.,.......+II+~~~==+::::::+II7~,,,:II\nIIIII,..:++IIII.,,,..,..,,.,:::::+II~:,,,,::=::?777I77I:::=I\nIIIII?IIIIIIII...........,,,..,,::II::,,~,,,:,:777777777::,I\nIIIIIIIIIIIII+....,,,...........,.,::,:,,:::::II777777777777\nIIIIIIIIIII??....,.,,,,...,....,,..,,,::,,:,:777II7777777777\nIIIIIIIII?II?....,,.,.77II77I+:,.,,:,,::,,,,:,,~II77IIII777I\nIIIIIIIIII??.....,.,,?IIIIIIIIIIIIII777777:::+=~~,,I7I77777I\nIIIIIIIII?...?I.,.,:7IIIIIIIIIIIIIIII7II777,:II7I77,IIIIIIII\nIIIIIIII,..II:.,,:IIIIIIIIIIIIIIIIIIIIIIIII7::7II7II=IIIII7I\nIIIIIII,.?II..,IIIIIIIIIIIIIIIIIIIIIIIIIIII77:+IIIII:IIIIIII\nIIIII?.:III?=,IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,IIIII?IIIIIII\n?I??,,II?I??I,IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,IIII=IIIIIII\n??,:III??I?II,=IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,IIIIIIIIIII\n?II??????????+,?I?III?IIIII??IIIIIIIIIIIIIIIIIII?,IIIIIIIIII\n=,=~~=++===+=+:,:=+=++=++~::~~++??????????????????~+??I??+++\n~~~~~~~:~:~~~~==~~:~~~~======+====~=====~=++====+=====~~~==~\n",
		"IIIIIIIIIIIIIIIIIIIIIIIII7IIIIIIIIII.,:IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?,.IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII.,,~IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII..::~IIIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII:.,,,.,:+IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII=....II...+III~........I.~IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,...:...,,,,:....IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII..........,,:,....,,.III\nIIII~,?III???IIIII?=:~?7IIIII7I+III=::+~~,,,:,,~:......,.?II\nIIII,,.......,?I,,,,,,:,.,..,.,..III?+===+?,,.:,,,III....,II\nIIIII,::7IIIIII,,,...,..,,.:~=~=,III:~,,,..,=:,:IIIIIII..,II\nIIIIIIIIIIIIII.,,..,,.....,,..,:~III,:,,=,....:IIIIIIIII,,.I\nIIIIIIIIIIIII+,,.,,......,....,,,.:,,:=..,,..,?IIIIIIIIIIIII\nIIIIIIIIIIIII,,,,::,,:.,:.,......,,.,,,:,.,.,:IIIIIIIIIIIIII\nIIIIIIIIIIII:,..,,,,+IIIIIIII=,..,~..,,:,......IIIIIIIIIIIII\nIIIIIIIIII=.,,,,,,,IIIIIIIIIIIIIIIIIIII.,.III.,..:IIIIIIIIII\nIIIIIIII.,..,,,,IIIIIIIIIIIIIIIIIIIIIII~.IIIIIIII=..IIIIIIII\nIIIIII..++..,IIIIIIIIIIIIIIIIIIIIIIIIII?:IIIIIIIIIII.IIIIIII\n?II+.,III,,IIIIIIIIIIIIIIIIIIIIIIIIIIIII=IIIIIIIIIIII+:IIIII\nII=~IIII,~IIIIIIIIIIIIIIIIIIIIIIIIIIIIII,IIIIIIIIIIIIII.IIII\nIIIIIII?.IIIIIIIIIIIIIIIIIIIIIIIIIIIIIII:IIIIIIIIIIIIIII~III\nIIIIIII.IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII~IIIIIIIIIIIIIIIIIII\nIIIIIII=?IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,IIIIIIIIIIIIIIIIII\nIIIIIIIII?I?I?II??II??++?~~?+===I?????++:I?III?++~::~~,,:?+?\n++=+=++??++?+?????~++???????????+??????????I?????????+?+++++\n",
		"7II7IIIIII7I7I7I7I777I7777777777777,,,=777777777777777777777\nIIIIIIIIII7IIIII7I7777777777777777I:,:7777777777777777777777\nIIIII7IIIIIIII77777II77777I777777~,~:I77I7777777777777777777\nIIII777IIIIIIIIIIII7III7777777I+,,~~+77777I77777777777777777\nIIIIIIIIIIIIIIIIIIIIIIII7I7777,,::,,:~+7I777I7II77I?I7777777\nIIIIIIIIIIIIIIIIIIII7IIIIIII7I+,,,+7=,,,?II+,,.,,,,,,I.=I777\nIIIIIIIIIIIIII7IIIIIIIIIIIIII?II777777+,,,~~.,,:,::~:,,,?777\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?II77I:,,:,,..,::::=::,,,~:,I77\nIIII..:+=:.....,,,,,,,.:+II,~+:=III~==?~:,:~:::~::,~,:::::7I\nIII?.,,...,.,?I::,,,,::,,,....,,III7?=+~++:,::~:~:?77:~::,I7\nIIII~==I7II7II,:,,,,:,,,:::~~===:III=~,:::~===::I77777=:::~I\nIIIII7IIIIIII.,,,,,,..,,.,,,..:~~II=~:,:~,:,~~=777777777~~,I\nIIIIIIII7IIII,,,,:,,,.,,,,,,,,::,::::::,,,:,,~77777777777777\nIIIIIIIIIIII=,:::,,,,,:,,,,,,,,,,:::::~:::,::,77777777777777\nIIIIIIIIIII?.,,,,,:77I77777I+,,,,~:,,::~~:::,:77777777777777\nIIIIIII,,,,.,.,+I77IIIIIIII77I777I7I:,:I77I::,:I77I7777I777I\nIIIII,..?:,,:7IIIIIIIIIIIIIIIII7IIII:,I7I7777I,,+III7III777I\nII+,,III,,,IIIIIIIIIIIIIIIIIIIIIIII:~7III777777?,.IIIIII7777\nI?:III+.,IIIIIIIIIIIIIIIIIIIIIIIII+:7IIII7II7I777=,,III7I777\nIIIII:.IIIIIIIIIIIIIIIIIIIIIIIIIII::IIIIIIIIII777777.=III777\nIII=,:IIIIIIIIIIIIIIIIIIIIIIIIIIII.IIIIIIIIIIIIIII7I7I,.:?II\nI,.,IIIIIIIIIIIIIIIIIIIIIIIIIIIII+=IIIIIIIIIIIIIIIIII7III+II\n~IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,IIIIIIIIIIIIIIIIIIIIIIIII\n+??==????????????????+?+::===+???II~???++?+=~++~=+IIIIIIIIII\n~~:~~~~=~~~~==~~~=~===~=~==~~~=====?=====~=~~~==~=~~~~===~~~\n",
		"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII=,,=IIIIIII777II7I77I77I7\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII=::IIIIIIIIIIIIIIIIII7III\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII+:=,?IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?,:~~=IIIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,:::,:~IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII??::,=7~,:,III?,=:,,,..+~,+IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIIIIII,::,II,.,:::::..:,IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIII=.,,,,...~=~=+~,.,,=,,III\nIIII~,~+?~......,.,:,:,...~,,~.:?II?~=+:,,:==~:~:,,:.,,~,+II\nIII?,,,.,,,,,=I~~:,,,::,:~:,,..,.=II?+=~?+~~::+=~:I77,,.,,II\nIIII,~??III?I?,,,,,,,,,.,.:=:~==,III:~,::::=:=:,?777I77:=:+I\nIIIIIIIIIIIII,.,,,,..,,..,..,,~~~?I?:~~:,,:~+:+77III7I7I+==I\nIIIIIIIIIIIII,,,,:,,.,,,,,,,,,::,,~,,~,,,,:,:?IIIIIIIIIIIIII\nIIIIIIIIIIII?,,:,,,,,,+?,,,,,,,,,,:,::,,,,,::?IIIIIIIIIIIIII\nIIIIIIII~,,,,,,,::,,IIIIIIIII?~,,::,,:~~,,,::IIIIIIIIIIIIIII\nIIIIIII+,,,,,:,=?IIIIIIIIIIIIIIIII?.,:III7:,:~IIIIIIIIIIIIII\nIIIIII+,,,,.IIIIIIIIIIIIIIIIIIIII:,,IIIIIIII:,+IIIIIIIIIIIII\n?IIII+.,+IIIIIIIIIIIIIIIIIIIIIII::IIIIIIIIIII,:IIIIIIIIIIIII\n=,,,.,IIIIIIIIIIIIIIIIIIIIIIIII.:IIIIIIIIIIIIII,+IIIIIIIIIII\nII,~IIIIIIIIIIIIIIIIIIIIIIIIII:~IIIIIIIIIIIIIIII,+IIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII.IIIIIIIIIIIIIIIIIII+,IIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII:IIIIIIIIIIIIIIIIIIIIII,IIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIIIIIIIIIIIIIIIIIIIII,?IIIIII\n=+?????????I????????+++++??+???????+?+?+??I?IIIIIIII?~?IIIII\n.====+==+=+=+=++==++=++++?++++++++????????????+?????????????\n",
		"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII=,,=IIIIIII777II7I77I77I7\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII=::IIIIIIIIIIIIIIIIII7III\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII+:=,?IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?,:~~=IIIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,,:::,:~IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII??::,=7~,:,III?,=:,,,..+~,+IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIIIIII,::,II,.,:::::..:,IIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIII=.,,,,...~=~=+~,.,,=,,III\nIIII~,~+?~......,.,:,:,...~,,~.:?II?~=+:,,:==~:~:,,:.,,~,+II\nIII?,,,.,,,,,=I~~:,,,::,:~:,,..,.=II?+=~?+~~::+=~:I77,,.,,II\nIIII,~??III?I?,,,,,,,,,.,.:=:~==,III:~,::::=:=:,?777I77:=:+I\nIIIIIIIIIIIII,.,,,,..,,..,..,,~~~?I?:~~:,,:~+:+77III7I7I+==I\nIIIIIIIIIIIII,,,,:,,.,,,,,,,,,::,,~,,~,,,,:,:?IIIIIIIIIIIIII\nIIIIIIIIIIII?,,:,,,,,,+?,,,,,,,,,,:,::,,,,,::?IIIIIIIIIIIIII\nIIIIIIII~,,,,,,,::,,IIIIIIIII?~,,::,,:~~,,,::IIIIIIIIIIIIIII\nIIIIIII+,,,,,:,=?IIIIIIIIIIIIIIIII?.,:III7:,:~IIIIIIIIIIIIII\nIIIIII+,,,,.IIIIIIIIIIIIIIIIIIIII:,,IIIIIIII:,+IIIIIIIIIIIII\n?IIII+.,+IIIIIIIIIIIIIIIIIIIIIII::IIIIIIIIIII,:IIIIIIIIIIIII\n=,,,.,IIIIIIIIIIIIIIIIIIIIIIIII.:IIIIIIIIIIIIII,+IIIIIIIIIII\nII,~IIIIIIIIIIIIIIIIIIIIIIIIII:~IIIIIIIIIIIIIIII,+IIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII.IIIIIIIIIIIIIIIIIII+,IIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII:IIIIIIIIIIIIIIIIIIIIII,IIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIIIIIIIIIIIIIIIIIIIII,?IIIIII\n=+?????????I????????+++++??+???????+?+?+??I?IIIIIIII?~?IIIII\n.====+==+=+=+=++==++=++++?++++++++????????????+?????????????\n",
		"IIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII+:~IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII:,.IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII+:~..IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII?,,=:IIIIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,:::.,=?IIIIIIIII?III,=IIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?:,,.?:,,~III?,:+,......?III???\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIIIII:,:+?,....,,.....:II??I\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII+?IIII?~..:I.:,=,:,,..,.,.=III?\nIIIII~IIIIIII?~:~:~~~=~,,:.,,~.:?II+?+~,,..,,,,.....,..,?I??\nIIIII~,:::,,:~I++=~,,~=,~=,,::::.~II?+==,,==,~==?IIII....?I?\nIIII~:~::,?I:I::::,,::,,,:~=~===,?II=~=~?~,,,::IIIIIIII,:?II\nIIIIIIIIIIIIII=,:~=~::,.,:.,.,~:=:?I::?:~=,=,=IIIIIIIIIII???\nIIIIIIIIIIIIIII,,:~::.,:,,.,,,:,,.~:,....::,?IIIIIIIII?I?I??\nIIIIIIIIII==III?,:~,:,+II?,,,,....,~.,.,,,,?IIIIIIIIIIIII???\nIIIIIIIIII~~=:~::,:::IIIIIIIII?,,,=:,..:,.IIIIIIIIII?II?????\nIIIIIIIII:II~~I??~?IIIIIIIIIIIIIII.,...=IIIIIIIIIIIII???????\nIIIIIIII,IIII,IIIIIIIIIIIIIIIIIII=..,.~IIIIIIII?IIII????????\nIIIIII?~?IIIII,IIIIIIIIIIIIIIIII?.,I,.IIII?II?III?II????????\nIIII+~?IIIIIII++IIIIIII,+IIIIIII::II..IIIIIIIIIIIIIIII??????\nIIIIIIIIIIIII?:IIIIIIIIIII:.::..~III,+IIIIIIIIIIIIIIIII?????\nIIIIIIIIIIIIIIIIIIIIIII?III?I?IIIIII.IIII??IIII?IIIII???????\nIIIIIIIIIIIIIIIIIIIIIIIIIII??I??II?I,III?IIIIII?II???I??????\nIIIIIIIIIIIIIIIIIIIIIIIIIII???I????I,?I?II?I?I???I??I???????\nI?II????IIII??????+++~~~:~::,~:~:~~=??+++=?????????+=+==~~=~\n?.??????????????????????????++?++++?????++++?++++++===~~~~~:\n",
		"III7I7IIIIIIIII7I7II7I7I7I77II77777?,:=777777777777777777I77\nIIIII77IIIIIIII7III7III7I777777777I=,:I777777777777777777777\nIIIIIIIIIIIIIIIIIII7IIIIIIII7II77~:=,,III777III77II7IIII7II7\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII~,,~:=7IIII7IIIIIIII7IIIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIIII,::~,,~=IIIIIIIIIIII:.+IIIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII+~,,,=:,:,IIII?+~,,..,:,,IIIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII?IIIIIII,,:?=+,,,,,,...:..IIIII\nIIIIIIIIIIIIIIIIIIIIIIIIIIIII=IIIII?,.,:=:~~~::,...,,,,:IIII\nIIIIIIIIIIIIII?:::~~~=~::,,,,,,,?III~==,:..:~::,,~I.,,,,,III\nIIII?~??I+~,:~I===~:,::,~~::~~~:,:III:=~::~:,~:=IIII=:,,:,II\nIIII~,,::::~:+,~::::::,,,:~~~===,?II++~++,,~,,,7IIIII7IIIIII\nI?I?,,,~=III??.,,:~=~:.,,,,,,,~:~=?I,:~=~:::,=IIIIIIIIIIIIII\n???IIIIIIIIIIII:,,~:~,:~,.,.,,:,:,,~.:,,:,:,IIIIIIIIIIIIIII?\nII?IIIIIIIIIIII?,,,::,,?I?,,,,,.,,,,,,.,,:?IIIIIIIIIIIIIIIII\n????IIIIII?,:,::,:,,,,IIIIIII?~,,,,~,,,,,,IIIIIIIIIIIIIIII?I\n????????I??,?.,,,=,=IIIIIIIIIIIIII,,,:.=7IIIIIIIIIIIIIIIIII?\n???????????:?I?,?IIIIIIIIIIII?IIII.,=IIIIIIIIIIII?I???I?II?I\n???????I???~?III,,II?IIII??II?III.,,IIIIIIIIIIIII??I??I?????\n???????????~?IIII?,???IIII+,+?II.,,IIIIIIIIIIIII?III????????\n?????????+~=??I?II:??III?III=,,..,:IIIIIIIII?I?IIII?????????\n??????????????????~I???????I??I,+IIIIIIIII??????????????????\n??????????????I??II???II??????,+IIIIIII?I?I??I?I?II?????????\n???????????????I??????III??I??.?IIIIIIIII?IIII??I???????????\n?+?+???+++?+??++++?+=+=+?+???+?,=+==++=+=+========~==~~~~:::\n+==+=====++===+=+++=+++==+==++?+++++=+===+=====~~~~~~~~~::~~\n",
		"IIIIIIIIIIIIIIIIIIIII+IIIIIIIIIIIIII::~IIIIII7IIIIIIIIII7III\nIIIIIIIIIIIIIIIIIIIII=IIIIIIIIIIIII?:=~IIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIII+IIIIIIIIIII=:+:,IIIIIIIIIIIIIIIIIIIIII\nIIIIIIIIIIIIIIIIIIIII+IIIIIIIII~:,:+=IIIIIIIIIIIIIIIIIIIIIII\nI?IIIIIIIIIIIIIIIIIII+IIIIIII+:~::~:=:IIIIIIIIIII:,,?IIIIIII\n???IIIIIIIIIII?IIIIII+IIIIIII?+,,,:,,:~IIIII+,,.::,~::IIIIII\n???I??I?IIII??II???II+IIIIIII??IIIIII:.=~I,.:,:~,,:,:,,IIIII\n?????I???II?I?????I+,,?II??~I+??II?.,,,:,:==~:::.,,,,:::IIII\n?????I????I????=,:~:::,,~,,.,,:.=?II:==~,.:,,:::+II~,,::,III\n????:??????:,:,::~:,::,,:~,~:~:~,,??I==:,:+:==:IIII=+I~::III\n????,,,,,,,,,,=::::::,:.,:~,::=:.+?II=:~~:::~,IIIIIIIIIIIIII\n????,,,,,+????,:,,::~~,,:,,.,::::=,:==~~~:+:,IIIIIIIIIIIIIII\n????=+?I???????.,,.,~=,:~,.,,,:,.,.,:::=:~~?IIIIIIIIIIIIIIII\n????????????????,..,,::,II:,,,,.,,.,,.,,:~IIIIIIIIIIIIIIIIII\n?????????????~...,,,,.,IIIIIIII,,.,=:.,,,=IIIIIIIIIIIIIIIIII\n???+?????+?+:.,..,..I+III???IIIIII+,,:.?IIIIIIIIIIIIIIIIIIII\n+???????+?????.,...??=?????I??II?~,.,:IIIIII?IIIIII?IIIIIIII\n??????????????I,=I,..=??????????.,,?,:IIII??I?IIII?II?IIIIII\n+???????????????.~???,,??????~:,.III,,I?II???I???III??????I?\n??+?++???????????:??I?=~?????~.:..,::,????????????I???I?I?II\n++????++???+???+~I?????=???..???I???I?????????????I??????II?\n+++???+?????+??????????I??.?II???II?I????????I????I??II?I??I\n++???????????????????+??=,????????IIII?????????I?I??I?II???I\n++++=++?=++????+??????++~++++?~~~~~~:::::~:~~~~~~==========~\n.=~==++==++===+:++++++++++?+++++=+++=~++=~+~=++==++++++==+==\n",
	};  // http://upload.wikimedia.org/wikipedia/commons/d/dd/Muybridge_race_horse_animated.gif
		// http://glassgiant.com/ascii/
	int i = 0;
	while (true)
	{
		if (_kbhit())
			break;
		cout << "          ";
		for (int j = 0; j < horse[i].length(); j++)
		if (horse[i][j] == '\n')
			cout  << horse[i][j] << "          ";
		else cout << horse[i][j];
		this_thread::sleep_for(chrono::milliseconds(25));
		i = (i + 1) % 15;
		ClearScreen();
	}
	_getch();
	ClearScreen();
}
void rentconfirmation(Box &box, string title)
{
	ClearScreen();
	vector<string> options{ "No", "Yes" };
	Cursor cursor1(1);
	bool more = true;
	bool change = false;
	while (more)
	{
		change = false;
		cout << el >> "Movies/Rent" << el << el >> "Are you sure you want to rent"  << el << el  >> title + " for " + my_to_string(box.getCost(title)) + " $"<< el << el;
		int num = box.timesWhatched(title);
		if (num > 0)
		{
			string message = "You have rented it ";
			if (num == 1)
				message += "once";
			else if (num == 2)
				message += "twice";
			else message += to_string(num) + " times";
			cout >> message;
		}
		cout << el << el<<el<<el;
		for (int i = 0; i < options.size(); i++)
		if (cursor1.getPosition() == i)
		{
			changecolor("black");
			cout >> "> " + options[i] << el << el << el;
			changecolor("white");
		}
		else cout >> options[i] << el << el << el;
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP || input == LEFT)
				change = cursor1.up();
			else if (input == DOWN || input == RIGHT)
				change = cursor1.down();
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == 1)
				{
					if (box.rentMovies(title))
					{
						cout >> "Transaction completed successfuly";
						_getch();
						watch(title);
					}
					else {
						cout >> "Error completing Transaction";
						_getch();
					}
				}
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void rentmenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	bool movieclub = true;
	Cursor cursor1;
	bool changeinoptions= true;
	vector <string> tabs = { "Movie Club", "Seen Movies" };
	int tab = 0;
	vector<Movie> movies;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Movies/Rent";
		if (movieclub)
			auxmessage += "/Movie Club";
		else auxmessage += "/Seen Movies";
		string message = "Please select a movie (TAB to switch)";
		cout << el >> auxmessage << el << el >> message << el << el;
		cout << "                           ";
		for (int i = 0; i < tabs.size(); i++)
		{
			if (tab == i)
			{
				changecolor("black");
				cout << tabs[i];
				changecolor("white");
				cout << "     ";
			}
			else cout << tabs[i] + "     ";
		}
		cout << el<<el;
		vector<Movie> movies;
		if (changeinoptions)
		{
			if (movieclub)
				movies = box.getmovieClub();
			else movies = box.getseenmovies();
			v = prep_list(movies);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == '\t')
			{
				page = 1;
				tab = (tab + 1) % 2;
				movieclub = !movieclub;
				change = true;
				changeinoptions = true;
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = MOVIESMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
					state = MOVIESMENU;
				else
					rentconfirmation(box, retrieve_title(options[cursor1.getPosition()]));
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
string program_to_string(const Program& program)
{
	string time = date_to_string(program.getDate());
	if (program.isRecorded())
		time = "* " + time;
	string s_out = program.getName();
	s_out.resize(40);
	int j = s_out.length() - 1;
	for (int i = time.length() - 1; i >= 0; i--)
	{
		s_out[j] = time[i];
		j--;
	}
	return s_out;
}
vector<vector<string>> prep_list(const vector<Program>& programs)
{
	vector<vector<string>> v_out;
	int size = 7;
	vector<string> v;
	for (int i = 0; i < programs.size(); i++)
	{
		if (v.size() == size)
		{
			v_out.push_back(v);
			v.clear();
		}
		string s = program_to_string(programs[i]);
		v.push_back(s);
	}
	if (v.size() != 0)
		v_out.push_back(v);
	return v_out;
}
void recordconfirmation(Box &box, const Program& program)
{
	ClearScreen();
	vector<string> options{ "No", "Yes" };
	Cursor cursor1(1);
	bool more = true;
	bool change = false;
	while (more)
	{
		change = false;
		cout << el >> "TV/TV Guide/Record" << el << el;
		cout >> normalize("Program name: ", program.getName(),35)<<el;
		cout >> normalize("Channel: " , program.getChannel(),35) << el;
		cout >> normalize("Type: ", program.getType(), 35) << el;
		cout >> normalize("Scheduled: " , date_to_string(program.getDate()),35) << el;
		cout >> normalize("Duration: " , to_string(program.getDuration()) + " minutes",35) << el<<el;
		
		if (program.getDate() < box.getDate())
		{
			cout << el << el << el << el ;
			if (program.getDate() + program.getDuration() > box.getDate() + 0)
			if (box.issettoberecorded(program))
				cout >> "This program is recording right now!";
			else cout >> "This program is on right now!";
			else if (box.issettoberecorded(program))
				cout >> "This program has already been recorded";
			else cout >> "This program has already aired...";
			go_end_of_screen();
			more = false;
			change = true;
			_getch();
		}
		else if (box.issettoberecorded(program))
		{
			cout << el << el << el << el << el ;
			cout >> "This program has already been set to record";
			go_end_of_screen();
			more = false;
			change = true;
			_getch();
		}
		else
		{
			cout >> "Would like to record " + program.getName() + " ?";
		cout << el << el << el << el;
		for (int i = 0; i < options.size(); i++)
		if (cursor1.getPosition() == i)
		{
			changecolor("black");
			cout >> "> " + options[i] << el << el << el;
			changecolor("white");
		}
		else cout >> options[i] << el << el << el;
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP || input == LEFT)
				change = cursor1.up();
			else if (input == DOWN || input == RIGHT)
				change = cursor1.down();
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == 1)
				{
					box.recordprogram(program);
					cout >> "The program has been set to record" << "                      ";
					_getch();
				}

				change = true;
				more = false;
			}
		}
		}
		ClearScreen();
	}
}
void daymenu(Box &box, int &state) //  :(
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector <string> days = { "none", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	vector <string> days_abv = { "Today", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
	int day=0;
	vector<Program> programs;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "TV/TV Guide/Sort by day";
		auxmessage +=  "/" + days[day];
		string message = "Please select a Program (TAB to switch)";
		cout << el >> auxmessage << el << el >> message << el << el;
		if (changeinoptions)
		{
			
			programs = box.listByDay(days[day]);
			v = prep_list(programs);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		cout << "         ";
		for (int i = 0; i < days.size(); i++)
		{
			if (day == i)
			{
				changecolor("black");
				cout << days_abv[i];
				changecolor("white");
				cout << "     ";
			}
			else cout << days_abv[i] + "     ";
		}
		cout << el<<el;
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == '\t')
			{
				day = (day + 1) % 8;
				page = 1;
				change = true;
				changeinoptions = true;
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = LISTMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					change = true;
					more = false;
					state = LISTMENU;
				}
				else
					recordconfirmation(box,  programs[(page -1)*7+cursor1.getPosition()]/*retrieve_title(options[cursor1.getPosition()])*/);
				change = true;
				/*more = false;*/
			}
		}
		ClearScreen();
	}
}  //  :( //
string channel_to_string(const Channel& channel)
{
	string num = to_string(channel.getPrograms().size());
	string s_out = channel.getName();
	s_out.resize(40);
	int j = s_out.length() - 1;
	for (int i = num.length() - 1; i >= 0; i--)
	{
		s_out[j] = num[i];
		j--;
	}
	return s_out;
}
vector<vector<string>> prep_list(const vector<Channel>& channels)
{
	vector<vector<string>> v_out;
	int size = 7;
	vector<string> v;
	for (int i = 0; i < channels.size(); i++)
	{
		if (v.size() == size)
		{
			v_out.push_back(v);
			v.clear();
		}
		string s = channel_to_string(channels[i]);
		v.push_back(s);
	}
	if (v.size() != 0)
		v_out.push_back(v);
	return v_out;
}
void channelmenu2(Box &box, int &state, const Channel& channel)
{
	ClearScreen();
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector <string> days = { "none", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	vector <string> days_abv = { "ALL", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
	int day = 0;
	vector<Program> programs;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "TV/TV Guide/Sort by Channel";
		auxmessage += "/" + channel.getName();
		string message = "Please select a Program (TAB to switch)";
		cout << el >> auxmessage << el << el >> message << el << el;
		if (changeinoptions)
		{

			programs = box.listByChannel(channel.getName(),days[day]);
			v = prep_list(programs);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		cout << "         ";
		for (int i = 0; i < days.size(); i++)
		{
			if (day == i)
			{
				changecolor("black");
				cout << days_abv[i];
				changecolor("white");
				cout << "     ";
			}
			else cout << days_abv[i] + "     ";
		}
		cout << el << el;
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == '\t')
			{
				day = (day + 1) % 8;
				page = 1;
				change = true;
				changeinoptions = true;
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = CHANNELMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					change = true;
					more = false;
					state = CHANNELMENU;
				}
				else
					recordconfirmation(box, programs[(page - 1) * 7 + cursor1.getPosition()]);
				change = true;
				/*more = false;*/
			}
		}
		ClearScreen();
	}
}
void channelmenu1(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector<Channel> channels;
	channels = box.getChannels();
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "TV/TV Guide/Sort by Channel/Choose Channel";
		string message = "Please select a Channel";
		cout << el >> auxmessage << el << el >> message << el << el<<el;
		if (changeinoptions)
		{

			v = prep_list(channels);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = LISTMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
					state = LISTMENU;
				else
					channelmenu2(box, state, channels[(page - 1) * 7 + cursor1.getPosition()]);
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
string type_to_string(string type, Box &box)
{
	string num = to_string(box.getTypeFrequency(type));
	string s_out = type;
	s_out.resize(40);
	int j = s_out.length() - 1;
	for (int i = num.length() - 1; i >= 0; i--)
	{
		s_out[j] = num[i];
		j--;
	}
	return s_out;
}
vector<vector<string>> prep_list(const vector<string>& types, Box &box)
{
	vector<vector<string>> v_out;
	int size = 7;
	vector<string> v;
	for (int i = 0; i < types.size(); i++)
	{
		if (v.size() == size)
		{
			v_out.push_back(v);
			v.clear();
		}
		string s = type_to_string(types[i], box);
		v.push_back(s);
	}
	if (v.size() != 0)
		v_out.push_back(v);
	return v_out;
}
void typemenu2(Box &box, int &state, string type)
{
	ClearScreen();
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector <string> days = { "none", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday" };
	vector <string> days_abv = { "ALL", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun" };
	int day = 0;
	vector<Program> programs;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "TV/TV Guide/Sort by Type";
		auxmessage += "/" + type;
		string message = "Please select a Program (TAB to switch)";
		cout << el >> auxmessage << el << el >> message << el << el;
		if (changeinoptions)
		{

			programs = box.listByType(type,days[day]);
			v = prep_list(programs);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		cout << "         ";
		for (int i = 0; i < days.size(); i++)
		{
			if (day == i)
			{
				changecolor("black");
				cout << days_abv[i];
				changecolor("white");
				cout << "     ";
			}
			else cout << days_abv[i] + "     ";
		}
		cout << el << el;
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == '\t')
			{
				day = (day + 1) % 8;
				page = 1;
				change = true;
				changeinoptions = true;
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = TYPEMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					change = true;
					more = false;
					state = TYPEMENU;
				}
				else
					recordconfirmation(box, programs[(page - 1) * 7 + cursor1.getPosition()]);
				change = true;
				/*more = false;*/
			}
		}
		ClearScreen();
	}
}
void typemenu1(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector<string> types;
	types = box.getTypes();
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "TV/TV Guide/Sort by Type/Choose Type";
		string message = "Please select a Type";
		cout << el >> auxmessage << el << el >> message << el << el<< el;
		if (changeinoptions)
		{

			v = prep_list(types, box);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = LISTMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
					state = LISTMENU;
				else
					typemenu2(box, state, types[(page - 1) * 7 + cursor1.getPosition()]);
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void watchmenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector <string> tabs = { "Live TV", "Recorded Programs" };
	int tab = 0;
	vector<Program> programs;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "TV/TV Guide/Sort by day";
		auxmessage += "/" + tabs[tab];
		string message = "Please select a Program (TAB to switch)";
		cout << el >> auxmessage << el << el >> message << el << el;
		if (changeinoptions)
		{

			programs = box.listByAvailable(tabs[tab]);
			v = prep_list(programs);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		cout << "                           ";
		for (int i = 0; i < tabs.size(); i++)
		{
			if (tab == i)
			{
				changecolor("black");
				cout << tabs[i];
				changecolor("white");
				cout << "     ";
			}
			else cout << tabs[i] + "     ";
		}
		cout << el << el;
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == '\t')
			{
				tab = (tab + 1) % 2;
				page = 1;
				change = true;
				changeinoptions = true;
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = TVMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					change = true;
					more = false;
					state = TVMENU;
				}
				else
					watch(programs[(page - 1) * 7 + cursor1.getPosition()].getName());
				change = true;
				/*more = false;*/
			}
		}
		ClearScreen();
	}
}
void cancelrecording(Box& box, const Program& program)
{
	ClearScreen();
	vector<string> options{ "No", "Yes" };
	Cursor cursor1(1);
	bool more = true;
	bool change = false;
	while (more)
	{
		change = false;
		cout << el >> "TV/TV Guide/Record" << el << el;
		cout >> normalize("Program name: ", program.getName(), 35) << el;
		cout >> normalize("Channel: ", program.getChannel(), 35) << el;
		cout >> normalize("Type: ", program.getType(), 35) << el;
		cout >> normalize("Scheduled: ", date_to_string(program.getDate()), 35) << el;
		cout >> normalize("Duration: ", to_string(program.getDuration()) + " minutes", 35) << el << el;

		if (program.getDate() < box.getDate())
		{
			cout << el >> "Would like to delete this recording?" << el;
			more = true;
		}
		else
		{
			cout << el >> "Would like to cancel this recording?" << el;

			more = true;
		}
		cout << el << el << el;
		for (int i = 0; i < options.size(); i++)
		if (cursor1.getPosition() == i)
		{
			changecolor("black");
			cout >> "> " + options[i] << el << el << el;
			changecolor("white");
		}
		else cout >> options[i] << el << el << el;
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP || input == LEFT)
				change = cursor1.up();
			else if (input == DOWN || input == RIGHT)
				change = cursor1.down();
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == 1)
				{
					box.cancelrecording(program);
					cout >> "Operation completed successfuly" << "                        ";
					_getch();
				}

				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
	ClearScreen();
}
void recordedmenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	int tab = 0;
	vector<Program> programs;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "TV/Recordings";
		string message = "Please select a Program";
		cout << el >> auxmessage << el << el >> message << el;
		if (changeinoptions)
		{

			programs = box.listRecordings();
			v = prep_list(programs);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		
		cout << el << el;
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == '\t')
			{
				tab = (tab + 1) % 2;
				page = 1;
				change = true;
				changeinoptions = true;
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = TVMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					change = true;
					more = false;
					state = TVMENU;
				}
				else
					cancelrecording(box, programs[(page - 1) * 7 + cursor1.getPosition()]);
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void deleteconfirmation(Box &box, int &state, const Channel& channel)
{
	ClearScreen();
	vector<string> options{ "No", "Yes" };
	Cursor cursor1(1);
	bool more = true;
	bool change = false;
	while (more)
	{
		change = false;
		cout << el >> "Actions/Channel/Delete/" + channel.getName() << el << el >> "Are you sure you want to delete " +channel.getName()+ "?" << el << el << el << el;
		cout << el << el << el << el;
		for (int i = 0; i < options.size(); i++)
		if (cursor1.getPosition() == i)
		{
			changecolor("black");
			cout >> "> " + options[i] << el << el << el;
			changecolor("white");
		}
		else cout >> options[i] << el << el << el;
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP || input == LEFT)
				change = cursor1.up();
			else if (input == DOWN || input == RIGHT)
				change = cursor1.down();
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == 1)
				{
					if (box.removeChannel(channel))
					{
						cout >> "Channel deleted successfuly" << "                          ";
						_getch();
						change = true;
						more = false;
					}
					else {
						cout >> "Error!";
						_getch();
					}
				}
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void deletechannelmenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector<Channel> channels;
	channels = box.getChannels();
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Channel/Delete";
		string message = "Please select a Channel";
		cout << el >> auxmessage << el << el >> message << el << el<<el;
		if (changeinoptions)
		{

			v = prep_list(channels);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = CRUDCHANNELMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
					state = CRUDCHANNELMENU;
				else
					deleteconfirmation(box, state, channels[(page - 1) * 7 + cursor1.getPosition()]);
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
vector<vector<string>> prep_list(Box &box)
{
	vector<vector<string>> v_out;
	int size = 7;
	vector<string> v;
	for (int j = 0; j < box.getChannels().size(); j++)
	{
		for (int i = 0; i < (box.getChannels()[j].getPrograms()).size(); i++)
		{
			if (v.size() == size)
			{
				v_out.push_back(v);
				v.clear();
			}
			string s = program_to_string(box.getChannels()[j].getPrograms()[i]);
			v.push_back(s);
		}
	}
	if (v.size() != 0)
		v_out.push_back(v);
	return v_out;
}
void deleteconfirmation(Box &box, int &state, const Program& program, const Channel &channel)
{
	ClearScreen();
	vector<string> options{ "No", "Yes" };
	Cursor cursor1(1);
	bool more = true;
	bool change = false;
	while (more)
	{
		change = false;
		cout << el >> "Actions/Channel/Delete/" + program.getName() << el << el >> "Are you sure you want to delete " + program.getName() + "?" << el << el << el << el;
		cout << el << el << el << el;
		for (int i = 0; i < options.size(); i++)
		if (cursor1.getPosition() == i)
		{
			changecolor("black");
			cout >> "> " + options[i] << el << el << el;
			changecolor("white");
		}
		else cout >> options[i] << el << el << el;
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP || input == LEFT)
				change = cursor1.up();
			else if (input == DOWN || input == RIGHT)
				change = cursor1.down();
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == 1)
				{
					if (box.removeProgram(channel,program))
					{
						cout >> "Program deleted successfuly" << "                        ";
						_getch();
						change = true;
						more = false;
					}
					else {
						cout >> "Error!";
						_getch();
					}
				}
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void deleteprogramenu2(Box &box, int &state, const Channel& channel)
{
	ClearScreen();
	bool more = true;
	int page = 1;
	Cursor cursor1;
	int day = 0;
	vector<Program> programs;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Program/Delete";
		auxmessage += "/" + channel.getName();
		string message = "Please select a Program";
		cout << el >> auxmessage << el << el >> message << el << el<<el;
			programs = box.listByChannel(channel.getName(), "none");
			v = prep_list(programs);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		cout << "         ";
		cout << el << el;
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
			}
			else if (input == DOWN)
			{
				change = cursor1.down();

			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
				}
				else
				{
					change = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
				}
				else
				{
					change = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					change = true;
					more = false;
				}
				else
					deleteconfirmation(box,state, programs[(page - 1) * 7 + cursor1.getPosition()],channel);
				change = true;
				/*more = false;*/
			}
		}
		ClearScreen();
	}
}
void deleteprogrammenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector<Channel> channels;
	channels = box.getChannels();
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Program/Delete";
		string message = "Please select a Channel";
		cout << el >> auxmessage << el << el >> message << el << el<<el;
		if (changeinoptions)
		{

			v = prep_list(channels);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = CRUDPROGRAMMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
					state = CRUDPROGRAMMENU;
				else
					deleteprogramenu2(box, state, channels[(page - 1) * 7 + cursor1.getPosition()]);
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void deleteconfirmation(Box &box, int &state, const string& title)
{
	ClearScreen();
	vector<string> options{ "No", "Yes" };
	Cursor cursor1(1);
	bool more = true;
	bool change = false;
	while (more)
	{
		change = false;
		cout << el >> "Actions/Movie/Delete/" + title << el << el >> "Are you sure you want to delete " + title + "?" << el << el << el << el;
		cout << el << el << el << el;
		for (int i = 0; i < options.size(); i++)
		if (cursor1.getPosition() == i)
		{
			changecolor("black");
			cout >> "> " + options[i] << el << el << el;
			changecolor("white");
		}
		else cout >> options[i] << el << el << el;
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP || input == LEFT)
				change = cursor1.up();
			else if (input == DOWN || input == RIGHT)
				change = cursor1.down();
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == 1)
				{
					if (box.removeMovie(title))
					{
						cout >> "Movie deleted successfuly" << "                           ";
						_getch();
						change = true;
						more = false;
					}
					else {
						cout >> "Error!";
						_getch();
					}
				}
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void deletemoviemenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	bool movieclub = true;
	Cursor cursor1;
	bool changeinoptions = true;
	vector <string> tabs = { "Movie Club", "Seen Movies" };
	int tab = 0;
	vector<Movie> movies;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Movies/Delete";
		if (movieclub)
			auxmessage += "/Movie Club";
		else auxmessage += "/Seen Movies";
		string message = "Please select a movie (TAB to switch)";
		cout << el >> auxmessage << el << el >> message << el << el;
		cout << "                           ";
		for (int i = 0; i < tabs.size(); i++)
		{
			if (tab == i)
			{
				changecolor("black");
				cout << tabs[i];
				changecolor("white");
				cout << "     ";
			}
			else cout << tabs[i] + "     ";
		}
		cout << el << el;
		vector<Movie> movies;
		if (changeinoptions)
		{
			if (movieclub)
				movies = box.getmovieClub();
			else movies = box.getseenmovies();
			v = prep_list(movies);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == '\t')
			{
				page = 1;
				tab = (tab + 1) % 2;
				movieclub = !movieclub;
				change = true;
				changeinoptions = true;
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = CRUDMOVIEMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
					state = CRUDMOVIEMENU;
				else
					deleteconfirmation(box,state, retrieve_title(options[cursor1.getPosition()]));
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void myinputcreatechannel(string &in, bool &change, bool &more, bool &ignore, int &state, int previousstate, Box &box, int &numtries)
{
	unsigned char input = _getch();
	if (input == 224)
	{
		ignore = true;
		change = false;
	}
	else
	if (input == ESCAPE)
	{
		state = previousstate;
		more = false;
		change = true;
	}
	else if (input == ENTER)
	{
		if (in.length() > 20)
		{
			change = true;
			numtries = 1;
		}
		else if (box.createdChannel(in))
		{
			more = false;
			change = true;
			numtries = 0;
			cout >> "Channel created successfuly" << "                          ";
			_getch();
		}
		else
		{
			if (in != "")
			{
				change = true;
				in.clear();
				numtries=-1;
			}
			else change = false;
		}
	}
	else if (input == BACKSPACE)
	{
		if (in == "")
			change = false;
		else
		{
			in = in.substr(0, in.length() - 1);
			change = true;
		}
	}
	else if (ignore)
	{
		ignore = false;
		change = false;
	}
	else
	{
		in += input;
		change = true;
	}
}
void createchannelmenu(Box &box, int &state)
{
	string in;
	bool more = true;
	int numtries = 0;
	bool ignore = false;
	while (more)
	{
		string message;
		if (numtries == 0)
			message = "Please insert the name of the new channel (ESC to go back):";
		else if (numtries ==-1)
			message = "That Channel already exists";
		else message = "Name too long";
		cout << el >> "Actions/Channel/Create" << el << el >> message << el << el << el << el << el << el << el << el << el;
		cout >> "> " + in;
		go_end_of_screen();
		bool change = false;
		bool ignore = false;
		while (!change)
		{
			myinputcreatechannel(in, change, more, ignore, state, CRUDCHANNELMENU, box, numtries);
		}
		ClearScreen();
	}
	ClearScreen();
}
void myinputcreatemovie(string &in, bool &change, bool &more, bool &ignore, int &state, int previousstate, Box &box, int &numtries)
{
	unsigned char input = _getch();
	if (input == 224)
	{
		ignore = true;
		change = false;
	}
	else
	if (input == ESCAPE)
	{
		state = previousstate;
		more = false;
		change = true;
	}
	else if (input == ENTER)
	{
		if (in.length() > 20)
		{
			change = true;
			numtries = 1;
		}
		else if (box.createdMovie(in))
		{
			more = false;
			change = true;
			numtries = 0;
			cout >> "Movie Created successfuly" << "                           ";
			_getch();
		}
		else
		{
			if (in != "")
			{
				change = true;
				in.clear();
				numtries = -1;
			}
			else change = false;
		}
	}
	else if (input == BACKSPACE)
	{
		if (in == "")
			change = false;
		else
		{
			in = in.substr(0, in.length() - 1);
			change = true;
		}
	}
	else if (ignore)
	{
		ignore = false;
		change = false;
	}
	else
	{
		in += input;
		change = true;
	}
}
void createmoviemenu(Box &box, int &state)
{
	string in;
	bool more = true;
	int numtries = 0;
	bool ignore = false;
	while (more)
	{
		string message;
		if (numtries == 0)
			message = "Please insert the title of the new Movie (ESC to go back):";
		else if (numtries == -1)
			message = "That Movie already exists";
		else message = "Title too long";
		cout << el >> "Actions/Movies/Create" << el << el >> message << el << el << el << el << el << el << el << el << el;
		cout >> "> " + in;
		go_end_of_screen();
		bool change = false;
		bool ignore = false;
		while (!change)
		{
			myinputcreatemovie(in, change, more, ignore, state, CRUDMOVIEMENU, box, numtries);
		}
		ClearScreen();
	}
	ClearScreen();
}
void myinputcreateprogram(string &in, bool &change, bool &more, bool &ignore, int &state, int previousstate, Box &box, int &numtries,Channel &channel)
{
	unsigned char input = _getch();
	if (input == 224)
	{
		ignore = true;
		change = false;
	}
	else
	if (input == ESCAPE)
	{
		state = previousstate;
		more = false;
		change = true;
	}
	else if (input == ENTER)
	{
		if (in.length() > 20)
		{
			change = true;
			numtries = 1;
		}
		else if (box.createdProgram(in,channel))
		{
			more = false;
			change = true;
			numtries = 0;
			cout >> "Program Created successfuly" << "                         ";
			_getch();
		}
		else
		{
			if (in != "")
			{
				change = true;
				in.clear();
				numtries = -1;
			}
			else change = false;
		}
	}
	else if (input == BACKSPACE)
	{
		if (in == "")
			change = false;
		else
		{
			in = in.substr(0, in.length() - 1);
			change = true;
		}
	}
	else if (ignore)
	{
		ignore = false;
		change = false;
	}
	else
	{
		in += input;
		change = true;
	}
}
void createprogrammenu2(Box &box, int &state,Channel &channel)
{
	string in;
	bool more = true;
	int numtries = 0;
	bool ignore = false;
	while (more)
	{
		string message;
		if (numtries == 0)
			message = "Please insert the title of the new Program (ESC to go back):";
		else if (numtries == -1)
			message = "That Program already exists";
		else message = "Title too long";
		cout << el >> "Actions/Program/Create" << el << el >> message << el << el << el << el << el << el << el << el << el;
		cout >> "> " + in;
		go_end_of_screen();
		bool change = false;
		bool ignore = false;
		while (!change)
		{
			myinputcreateprogram(in, change, more, ignore, state, CRUDPROGRAMMENU, box, numtries,channel);
		}
		ClearScreen();
	}
	ClearScreen();
}
void createprogrammenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector<Channel> channels;
	channels = box.getChannels();
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Program/Create";
		string message = "Please select a Channel";
		cout << el >> auxmessage << el << el >> message << el << el<<el;
		if (changeinoptions)
		{

			v = prep_list(channels);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = CRUDPROGRAMMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					state = CRUDPROGRAMMENU;
				}
				else
				{
					ClearScreen();
				createprogrammenu2(box, state, channels[(page - 1) * 7 + cursor1.getPosition()]);
				}
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void myinputupdatechannel(string &in, bool &change, bool &more, bool &ignore, int &state, int previousstate, Box &box, int &numtries, Channel &channel)
{
	unsigned char input = _getch();
	if (input == 224)
	{
		ignore = true;
		change = false;
	}
	else
	if (input == ESCAPE)
	{
		state = previousstate;
		more = false;
		change = true;
	}
	else if (input == ENTER)
	{
		if (in.length() > 20)
		{
			change = true;
			numtries = 1;
		}
		else if (box.updateChannel(in, channel))
		{
			more = false;
			change = true;
			numtries = 0;
			cout >> "Program Updated successfuly" << "                         ";
			_getch();
		}
		else
		{
			if (in != "")
			{
				change = true;
				in.clear();
				numtries = -1;
			}
			else change = false;
		}
	}
	else if (input == BACKSPACE)
	{
		if (in == "")
			change = false;
		else
		{
			in = in.substr(0, in.length() - 1);
			change = true;
		}
	}
	else if (ignore)
	{
		ignore = false;
		change = false;
	}
	else
	{
		in += input;
		change = true;
	}
}
void updatechannelmenu2(Box &box, int &state, Channel &channel)
{
	string in;
	bool more = true;
	int numtries = 0;
	bool ignore = false;
	while (more)
	{
		string message;
		if (numtries == 0)
			message = "Please insert the new name of the channel (ESC to go back):";
		else if (numtries == -1)
			message = "That Channel already exists";
		else message = "Name too long";
		cout << el >> "Actions/Channel/Update" << el << el >> message << el << el << el << el << el << el << el << el << el;
		cout >> "> " + in;
		go_end_of_screen();
		bool change = false;
		bool ignore = false;
		while (!change)
		{
			myinputupdatechannel(in, change, more, ignore, state, CRUDCHANNELMENU, box, numtries,channel);
		}
		ClearScreen();
	}
	ClearScreen();
}
void updatechannelmenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector<Channel> channels;
	channels = box.getChannels();
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Channel/Update";
		string message = "Please select a Channel";
		cout << el >> auxmessage << el << el >> message << el << el<<el;
		if (changeinoptions)
		{

			v = prep_list(channels);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = CRUDCHANNELMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					state = CRUDCHANNELMENU;
				}
				else
				{
					ClearScreen();
					updatechannelmenu2(box, state, channels[(page - 1) * 7 + cursor1.getPosition()]);
				}
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void myinputupdatemovie(string &in, bool &change, bool &more, bool &ignore, int &state, int previousstate,Box &box, int &numtries, Movie &movie, int &phase, float &cost, int &multiplier)
{
	unsigned char input = _getch();
	if (phase == 1)
	{
		if (input == 224)
		{
			ignore = true;
			change = false;
		}
		else
		if (input == ESCAPE)
		{
			more = false;
			change = true;
		}
		else if (input == ENTER)
		{
			if (in.length() > 20)
			{
				change = true;
				numtries = 1;
			}
			else
			{
				if (in != "")
				{
					change = true;
					phase = 2;
				}
				else change = false;
			}
		}
		else if (input == BACKSPACE)
		{
			if (in == "")
				change = false;
			else
			{
				in = in.substr(0, in.length() - 1);
				change = true;
			}
		}
		else if (ignore)
		{
			ignore = false;
			change = false;
		}
		else
		{
			in += input;
			change = true;
		}
	}
	else
	{
		if (input == 224)
		{
			ignore = true;
			change = false;
		}
		else
		if (input == ESCAPE)
		{
			change = true;
			phase = 1;
		}
		else if (input == '\t')
		{
			if (multiplier == 1)
				multiplier = 100;
			else multiplier = 1;
			change = true;
		}
		else if (input == ENTER)
		{
			if (box.updateMovie(in, movie, cost))
			{
				more = false;
				change = true;
				numtries = 0;
				cout >> "Movie Updated successfuly" << "                           ";
				_getch();
			}
			else
			{
				in = movie.getTitle();
				change = true;
				numtries = -1;
				phase = 1;
			}
		}
		else if (ignore)
		{
			change = true;
			if (input == UP)
			{
				if (cost + .1 * multiplier < 1000)
					cost += .1 * multiplier;
				else cost = 1000;
			}
			else if (input == DOWN)
			{
				if (cost - .1 * multiplier > 0)
					cost -= .1 * multiplier;
				else cost = 0;
			}
			else if (input == RIGHT)
			{
				if (cost + .01 * multiplier < 1000)
					cost += .01 * multiplier;
				else cost = 1000;
			}
			else if (input == LEFT)
			{
				if (cost - .01 * multiplier > 0)
					cost -= .01 * multiplier;
				else change = 0;
			}
			ignore = false;
		}
	}
}
void updateconfirmationmovie(Box &box, int &state, string title)
{
	Movie movie = box.getmovie(title);
	string in = movie.getTitle();
	bool more = true;
	int numtries = 0;
	bool ignore = false;
	int phase = 1;
	float cost = movie.getCost();
	int multiplier=1;
	ClearScreen();
	while (more)
	{
		string message;
		if (numtries == 0)
			message = "Please insert the new name of the Movie (ESC to go back):";
		else if (numtries == -1)
			message = "That Movie already exists";
		else message = "Name too long";
		cout << el >> "Actions/Channel/Create" << el << el >> message << el << el << el << el;
		cout >> "> " + in;
		if (phase == 2)
		{
			cout << el << el << el << el  >> "Please enter the new cost: " << el >>"Multiplier: " + to_string(multiplier) + "x (TAB to Switch)"<< el << el << el << el ;
			
			cout >> "> " + my_to_string(cost) + " $";
			 
		}
		go_end_of_screen();
		bool change = false;
		bool ignore = false;
		while (!change)
		{
			myinputupdatemovie(in, change, more, ignore, state, CRUDMOVIEMENU, box, numtries, movie, phase, cost, multiplier);
		}
		ClearScreen();
	}
	ClearScreen();
}
void updatemoviemenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	bool movieclub = true;
	Cursor cursor1;
	bool changeinoptions = true;
	vector <string> tabs = { "Movie Club", "Seen Movies" };
	int tab = 0;
	vector<Movie> movies;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Movise/Update";
		if (movieclub)
			auxmessage += "/Movie Club";
		else auxmessage += "/Seen Movies";
		string message = "Please select a movie (TAB to switch)";
		cout << el >> auxmessage << el << el >> message << el << el;
		cout << "                           ";
		for (int i = 0; i < tabs.size(); i++)
		{
			if (tab == i)
			{
				changecolor("black");
				cout << tabs[i];
				changecolor("white");
				cout << "     ";
			}
			else cout << tabs[i] + "     ";
		}
		cout << el << el;
		vector<Movie> movies;
		if (changeinoptions)
		{
			if (movieclub)
				movies = box.getmovieClub();
			else movies = box.getseenmovies();
			v = prep_list(movies);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == '\t')
			{
				page = 1;
				tab = (tab + 1) % 2;
				movieclub = !movieclub;
				change = true;
				changeinoptions = true;
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = CRUDMOVIEMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
					state = CRUDMOVIEMENU;
				else
					updateconfirmationmovie(box, state, retrieve_title(options[cursor1.getPosition()]));
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}
void myinputupdateprogram(string &name, string &type, int &duration, Date &date, bool &change, bool &more, bool &ignore, int &state, Box &box, int &numtries, Program &program, int &phase, int &multiplier1, int &multiplier2, Date &min)
{
	string *in = NULL;
	if (phase == 1)
		in = &name;
	else in = &type;
	unsigned char input = _getch();
	if (phase == 1 || phase ==2)
	{
		if (input == 224)
		{
			ignore = true;
			change = false;
		}
		else
		if (input == ESCAPE)
		{
			if (phase == 1)
			more = false;
			else phase--;
			change = true;
		}
		else if (input == ENTER)
		{
			if (in->length() > 20)
			{
				change = true;
				numtries = 1;
			}
			else
			{
				if (*in != "")
				{
					change = true;
					phase++;
				}
				else change = false;
			}
		}
		else if (input == BACKSPACE)
		{
			if (*in == "")
				change = false;
			else
			{
				*in = in->substr(0, in->length() - 1);
				change = true;
			}
		}
		else if (ignore)
		{
			ignore = false;
			change = false;
		}
		else
		{
			*in += input;
			change = true;
		}
	}
	else if (phase == 3)
	{
		if (input == 224)
		{
			ignore = true;
			change = false;
		}
		else
		if (input == ESCAPE)
		{
			change = true;
			phase--;
		}
		else if (input == '\t')
		{
			if (multiplier1 == 1)
				multiplier1 = 10;
			else multiplier1 = 1;
			change = true;
		}
		else if (input == ENTER)
		{
			change = true;
			phase++;
		}
		else if (ignore)
		{
			change = true;
			if (input == UP)
			{
				if (duration + 10 * multiplier1 <= 1000)
					duration += 10 * multiplier1;
				else duration = 1000;
			}
			else if (input == DOWN)
			{
				if (duration - 10 * multiplier1 >= 0)
					duration -= 10 * multiplier1;
				else duration = 0;
			}
			else if (input == RIGHT)
			{
				if (duration + 1 * multiplier1 <= 1000)
					duration += 1 * multiplier1;
				else duration = 1000;
			}
			else if (input == LEFT)
			{
				if (duration - 1 * multiplier1 >= 0)
					duration -= 1 * multiplier1;
				else duration = 0;
			}
			ignore = false;
			
		}
	}
	else if (phase == 4)
	{
		if (input == 224)
		{
			ignore = true;
			change = false;
		}
		else
		if (input == ESCAPE)
		{
			change = true;
			phase--;
		}
		else if (input == '\t')
		{
			if (multiplier2 == 1)
				multiplier2 = 60;
			else multiplier2 = 1;
			change = true;
		}
		else if (input == ENTER)
		{
			if (box.updateProgram(name, type, duration, date, program))
			{
				more = false;
				change = true;
				numtries = 0;
				cout >> "Program Updated successfuly" << "                          ";
				_getch();
			}
			else
			{
				name = program.getName();
				type = program.getType();
				duration = program.getDuration();
				change = true;
				numtries = -1;
				phase = 1;
			}

		}
		else if (ignore)
		{
			change = true;
			if (input == UP)
			{
				if (date + 10 * multiplier2 <= 10079)
					date = int_to_date(date + 10 * multiplier2);
				else date = int_to_date(10079);
			}
			else if (input == DOWN)
			{
				if (date - 10 * multiplier2 >= 0)
					date = int_to_date(date - 10 * multiplier2);
				else date = int_to_date(0);
			}
			else if (input == RIGHT)
			{
				if (date + 1 * multiplier2 <= 10079)
					date = int_to_date(date + 1 * multiplier2);
				else date = int_to_date(10079);
			}
			else if (input == LEFT)
			{
				if (date - 1 * multiplier2 >= 0)
					date = int_to_date(date - 1 * multiplier2);
				else date = int_to_date(0);
			}
			ignore = false;
		}
	}
}
void updateconfirmationprogram(Box &box, int &state, Program &program, const Channel &channel)
{
	string name = program.getName();
	string type = program.getType();
	int duration = program.getDuration();
	Date date = program.getDate();
	Date min = date;
	bool more = true;
	int numtries = 0;
	bool ignore = false;
	int phase = 1;
	int multiplier1 = 1;
	int multiplier2 = 1;
	ClearScreen();
	while (more)
	{
		string message;
		if (numtries == 0)
			message = "Please insert the new name of the Program (ESC to go back):";
		else if (numtries == -1)
			message = "Error!";
		else message = "Name too long";
		cout << el >> "Actions/Program/Update" << el << el >> message  << el << el;
		cout >> "> " + name;
		if (phase > 1)
		{
			cout << el << el >> "Please enter the new type: " << el<<el;
			cout >> "> " + type;
		}
		if (phase > 2)
		{
			cout << el << el >> "Please enter the new duration: "  << el >> "Multiplier: " + to_string(multiplier1) + "x (TAB to Switch)"<<el<<el;
			cout >> "> " + to_string(duration) + " minutes";
		}
		if (phase > 3)
		{
			cout  << el << el >> "Please enter the new date: " << el >> "Multiplier: " + to_string(multiplier2) + "x (TAB to Switch)"<<el<<el;
			cout >> "> " + date_to_string(date);
		}
		go_end_of_screen();
		bool change = false;
		bool ignore = false;
		while (!change)
		{
			myinputupdateprogram(name, type, duration, date, change, more, ignore, state, box, numtries, program, phase, multiplier1,multiplier2, box.getDate());
		}
		ClearScreen();
	}
	ClearScreen();
}
void updateprogramenu2(Box &box, int &state, const Channel& channel)
{
	ClearScreen();
	bool more = true;
	int page = 1;
	Cursor cursor1;
	int day = 0;
	vector<Program> programs;
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Programs/Update";
		auxmessage += "/" + channel.getName();
		string message = "Please select a Program";
		cout << el >> auxmessage << el << el >> message << el << el;
		programs = box.listByChannel(channel.getName(), "none");
		v = prep_list(programs);
		if (v.size() == 0)
			options.clear();
		else options = v[page - 1];
		cursor1.changeMax(options.size());
		options.push_back("Back");
		cout << "         ";
		cout << el;
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
			}
			else if (input == DOWN)
			{
				change = cursor1.down();

			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
				}
				else
				{
					change = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
				}
				else
				{
					change = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
				{
					change = true;
					more = false;
				}
				else
					updateconfirmationprogram(box, state, programs[(page - 1) * 7 + cursor1.getPosition()], channel);
				change = true;
				/*more = false;*/
			}
		}
		ClearScreen();
	}
}
void updateprogrammenu(Box &box, int &state)
{
	bool more = true;
	int page = 1;
	Cursor cursor1;
	bool changeinoptions = true;
	vector<Channel> channels;
	channels = box.getChannels();
	vector <vector <string>> v;
	vector<string> options;
	while (more)
	{
		bool change = false;
		string auxmessage = "Actions/Program/Update";
		string message = "Please select a Channel";
		cout << el >> auxmessage << el << el >> message << el << el<<el;
		if (changeinoptions)
		{

			v = prep_list(channels);
			if (v.size() == 0)
				options.clear();
			else options = v[page - 1];
			cursor1.changeMax(options.size());
			options.push_back("Back");
		}
		for (int i = 0; i < options.size(); i++)
		{
			if (cursor1.getPosition() == i)
			{
				changecolor("black");
				cout >> "> " + options[i] << el << el;
				changecolor("white");
			}
			else cout >> options[i] << el << el;
		}
		for (; end_of_screen>1;)
		{
			cout << el;
		}
		cout >> "Page " + to_string(page) + "/" + to_string(v.size());
		go_end_of_screen();
		while (!change)
		{
			char input = _getch();
			if (input == UP)
			{
				change = cursor1.up();
				changeinoptions = false;
			}
			else if (input == DOWN)
			{
				change = cursor1.down();
				changeinoptions = false;
			}
			else if (input == LEFT)
			{
				if (page > 1)
				{
					page--;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == RIGHT)
			{
				if (page < v.size())
				{
					page++;
					change = true;
					changeinoptions = true;
				}
				else
				{
					change = false;
					changeinoptions = false;
				}
			}
			else if (input == ESCAPE)
			{
				change = true;
				more = false;
				state = CRUDPROGRAMMENU;
			}
			else if (input == ENTER || input == SPACE)
			{
				if (cursor1.getPosition() == options.size() - 1)
					state = CRUDPROGRAMMENU;
				else
					updateprogramenu2(box, state, channels[(page - 1) * 7 + cursor1.getPosition()]);
				change = true;
				more = false;
			}
		}
		ClearScreen();
	}
}

int main()
{
	srand(unsigned int(time(NULL)));
	intro();
	int state = 1;
	Box box = Box(currentDate());
	if (!box.load())
	{
		cerr << "Error reading files";
		_getch();
		exit(1);
	}
	unsigned int launchtime = unsigned int(time(NULL));
	unsigned int tally = 0;
	while (state != EXIT)
	{
		//atualizacao da hora
		unsigned int checktime = unsigned int (time(NULL)-tally);
		if ((checktime - launchtime) > 60)
		{
			box.setDate(int_to_date(box.getDate() + (checktime - launchtime) / 60));
			tally += ((checktime - launchtime) / 60) * 60;
		}
		switch (state)
		{
		case MAINMENU:
			genericmenu(state, box, "", "Please select an Option:", { "TV", "Movies", "Privileged Actions", "Quit" }, { TVMENU, MOVIESMENU, ACTIONSMENU, ESCAPEMENU });
			break;
		case ESCAPEMENU:
			genericmenu(state, box, "", "Are you sure you want to Exit this Application?", { "No", "Yes" }, { MAINMENU, EXIT });
			break;
		case TVMENU:
			genericmenu(state, box, "TV", "Please select an Option:", { "Watch", "TV Guide", "Recordings", "Back" }, { WATCHMENU, LISTMENU,RECORDEDMENU, MAINMENU });
			break;
		case MOVIESMENU:
			genericmenu(state, box, "Movies", "Please select an Option:", { "Transactions", "Rent", "Back" }, { TRANSACTIONSMENU, RENTMENU, MAINMENU });
			break;
		case ACTIONSMENU:
			if (box.isAuthorized())
				genericmenu(state, box, "Privileged Actions", "Please select an Option:", { "Change Password", "Modify Channel", "Modify Program", "Modify Movie", "Back" }, { PASSWORDMENU, CRUDCHANNELMENU, CRUDPROGRAMMENU, CRUDMOVIEMENU, MAINMENU });
			else insertpassword(box, state);
			break;
		case WATCHMENU:
			watchmenu(box, state);
			break;
		case LISTMENU:
			genericmenu(state, box, "TV/TV Guide", "How would you like to sort the programs?", { "Day", "Channel", "Type", "Back" }, { DAYMENU, CHANNELMENU, TYPEMENU, TVMENU });
			break;
		case RECORDEDMENU:
			recordedmenu(box, state);
			break;
		case DAYMENU:
			daymenu(box, state);
			break;
		case CHANNELMENU:
			channelmenu1(box, state);
			break;
		case TYPEMENU:
			typemenu1(box, state);
			break;
		case TRANSACTIONSMENU:
			transactionsmenu(box, state);
			break;
		case RENTMENU:
			rentmenu(box, state);
			break;
		case PASSWORDMENU:
			changepassword(box, state);
			break;
		case CRUDCHANNELMENU:
			genericmenu(state, box, "Actions/Channels", "Please select an option:", { "Create", "Update", "Delete", "Back" }, { CREATECHANNEL, UPDATECHANNEL, DELETECHANNEL, ACTIONSMENU });
			break;
		case CRUDPROGRAMMENU:
			genericmenu(state, box, "Actions/Program", "Please select an option:", { "Create", "Update", "Delete", "Back" }, { CREATEPROGRAM, UPDATEPROGRAM, DELETEPROGRAM, ACTIONSMENU });
			break;
		case CRUDMOVIEMENU:
			genericmenu(state, box, "Actions/Movies", "Please select an option:", { "Create", "Update", "Delete", "Back" }, { CREATEMOVIE, UPDATEMOVIE, DELETEMOVIE, ACTIONSMENU });
			break;
		case CREATECHANNEL:
			createchannelmenu(box, state);
			break;
		case UPDATECHANNEL:
			updatechannelmenu(box,state);
			break;
		case DELETECHANNEL:
			deletechannelmenu(box, state);
			break;
		case CREATEPROGRAM:
			createprogrammenu(box, state);
			break;
		case UPDATEPROGRAM:
			updateprogrammenu(box, state);
			break;
		case DELETEPROGRAM:
			deleteprogrammenu(box, state);
			break;
		case CREATEMOVIE:
			createmoviemenu(box, state);
			break;
		case UPDATEMOVIE:
			updatemoviemenu(box, state);
			break;
		case DELETEMOVIE:
			deletemoviemenu(box, state);
			break;
		default:
			state = EXIT;
			break;
		}
	}
	box.save();
	outro();
	exit(0);
}
