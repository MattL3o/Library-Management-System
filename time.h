#pragma once
#include <iostream>

//thinned out version of tm
class Date {

private:
	int year;
	int mon;
	int day;
public:

	Date();
	Date(int mon, int day, int year);
	std::string to_string(void);
	void add_day(int more_days);
	friend std::string print_time(Date date);	//Returns a string with the date given a tm
	friend Date add_days(Date date, int more_days);	//Adds days to a given date following a calendar format
	//operator overloading
	bool operator < (Date d);
	bool operator > (Date d);
	bool operator <= (Date d);
	bool operator >= (Date d);
	int operator - (Date d);				//Returns amount of days difference between the two dates
	bool operator == (Date d);
	bool operator != (Date d);
	void operator = (Date d);



};

Date current_date(void);					//Returns the current date based off of the amount of clocks that have passed from the clock_initial
void set_clock(void);						//Prompts user to enter the initial date
