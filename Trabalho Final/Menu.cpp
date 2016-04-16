#include "Menu.h"

Menu::Menu(string name, string text, int numoptions)
{
	this->name = name;
	this->text = text;
	this->cursor = Cursor(numoptions);
	previous = NULL;
}

Menu::Menu()
{
	Menu* watch_ptr = new Menu("Watch", "Please select a program to watch:", 10);
	Menu* list_ptr = new Menu("List", "Please select a program to watch:", 10);
}
bool Menu::setNext(vector<Menu*> v)
{
	this->next = v;
	return true;
}
bool Menu::setPrevious(Menu* ptr)
{
	this->previous = ptr;
	return true;
}