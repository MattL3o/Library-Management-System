#include <string>
#include "time.h"

clock_t clock_initial;						//Stores the initial clock time after clock is set in execution
Date initial_date;							 //Stores the initial date given by the user at startup

void set_clock(void) {
	bool correct_date = 0;

	int mon, day, year;
	while (!correct_date) {

		std::string d_string;

		std::cout << "Please enter the date in 00/00/0000 format: ";

		std::cin >> d_string;

		bool error = 0;
		if (d_string[0] < '0' || d_string[0] > '1') { error = 1; }
		if (d_string[1] < '0' || d_string[1] > '9') { error = 1; }
		if (d_string[3] < '0' || d_string[3] > '3') { error = 1; }
		if (d_string[4] < '0' || d_string[4] > '9') { error = 1; }
		if (d_string[6] != '2') { error = 1; }
		if (d_string[7] != '0') { error = 1; }
		if (d_string[8] < '2' || d_string[8] > '9') { error = 1; }
		if (d_string[9] < '0' || d_string[9] > '9') { error = 1; }
		if (error) { std::cout << "Invalid date format, please try again!" << std::endl; continue; }




		std::string smon = "", sday = "", syear = "";


		smon += d_string[0];
		smon += d_string[1];

		sday += d_string[3];
		sday += d_string[4];

		syear += d_string[6];
		syear += d_string[7];
		syear += d_string[8];
		syear += d_string[9];

		mon = std::stoi(smon);
		day = std::stoi(sday);
		year = std::stoi(syear);

		if (mon < 1 || mon > 12 || day < 1 || day > 31 || year < 2022 || year > 2099) {
			std::cout << "That is an invalid date, please try again.\n";
			continue;
		}
		else {
			correct_date = 1;
		}
	}

	Date my_date(mon, day, year);

	initial_date = my_date;
	clock_initial = clock();
}

Date current_date(void) { return add_days(initial_date, (clock() - clock_initial) / (3 * CLOCKS_PER_SEC)); }

std::string print_time(Date date) {

	std::string out = "";


	out = std::to_string(date.mon);
	out += "/";
	out += std::to_string(date.day);
	out += "/";
	out += std::to_string(date.year);

	return out;
}

//returns whether the year parameter is a leap year
bool is_leap_year(int year) {
	if (year % 4 == 0) {
		return true;
	}
	else { return false; }
}

//returns day in month starting with 1 = January
int days_in_mon(int month, int year) {

	switch (month) {
	case 1: return 31;
	case 2:
		if (is_leap_year(year)) { return 29; }
		else { return 28; }
	case 3: return 30;
	case 4: return 31;
	case 5: return 31;
	case 6: return 30;
	case 7: return 31;
	case 8: return 31;
	case 9: return 30;
	case 10: return 31;
	case 11: return 30;
	case 12: return 31;

	}

	return 0;
}

Date add_days(Date date, int more_days) {

	while (more_days > 0) {

		date.day += 1;

		if (date.day > days_in_mon(date.mon, date.year)) {
			date.mon++;
			date.day = 1;

			if (date.mon > 12) {
				date.mon = 1;
				date.year++;
			}
		}

		more_days--;
	}
	return date;

}


//Date Constructors
Date::Date() {
	year = NULL;
	mon = NULL;
	day = NULL;
}
Date::Date(int mon, int day, int year) {
	this->year = year;
	this->day = day;
	this->mon = mon;
}

//Date Accessors
std::string Date::to_string(void) { return print_time(*this); }

//Date Mutators
void Date::add_day(int more_days) { *this = add_days(*this, more_days); return; }

//operator overloading Date
bool Date::operator < (Date d) {

	if (this->year < d.year) { return true; }
	if (this->year > d.year) { return false; }

	if (this->mon < d.mon) { return true; }
	if (this->mon > d.mon) { return false; }

	if (this->day < d.day) { return true; }
	if (this->day > d.day) { return false; }
	return false;

}
bool Date::operator > (Date d) {

	if (this->year > d.year) { return true; }
	if (this->year < d.year) { return false; }

	if (this->mon > d.mon) { return true; }
	if (this->mon < d.mon) { return false; }

	if (this->day > d.day) { return true; }
	if (this->day < d.day) { return false; }
	return false;

}
	
bool Date::operator <= (Date d) {

	if (this->year < d.year) { return true; }
	if (this->year > d.year) { return false; }

	if (this->mon < d.mon) { return true; }
	if (this->mon > d.mon) { return false; }

	if (this->day < d.day) { return true; }
	if (this->day > d.day) { return false; }
	return true;

}
bool Date::operator >= (Date d) {

	if (this->year > d.year) { return true; }
	if (this->year < d.year) { return false; }

	if (this->mon > d.mon) { return true; }
	if (this->mon < d.mon) { return false; }

	if (this->day > d.day) { return true; }
	if (this->day < d.day) { return false; }
	return true;;

}


bool Date::operator == (Date d) {
	if (year == d.year && mon == d.mon && day == d.day) {
		return true;
	}
	return false;
}
bool Date::operator != (Date d) {
	if (!(operator == (d))) { return true; }
	return false;
}

void Date::operator = (Date d) {
	year = d.year;
	mon = d.mon;
	day = d.day;
}

int Date::operator - (Date d) {

	int difference = 0;

	Date d1, d2;
	if (*this < d) {
		d1 = *this;
		d2 = d;
	}

	if (*this >= d) {
		d2 = *this;
		d1 = d;
	}

	while (d2 != d1) {
		d1.add_day(1);
		difference++;
	}

	if (*this < d) { return -1 * difference; }
	return difference;

}