#ifndef _MENU
#define _MENU

#include <string>
#include <vector>

#include "Cursor.h"

using namespace std;

class Menu
{
	string name;
	string text;
	Cursor cursor;
	Menu* previous;
	vector<Menu*> next;
public:
	Menu(string name, string text,int numoptions);
	Menu();
	Menu* createMenus();
	bool setNext(vector<Menu*> v);
	bool setPrevious(Menu* ptr);
};

#endif