#include "User.h"
#include "time.h"

//User Class
//Constructors
User::User() {
	username = "NULL";
	password = "NULL";
}

User::User(string username, string password) {
	this->username = username;
	this->password = password;
}



//ACCESSORS
string User::get_username(void) { return username; }
string User::get_password(void) { return password; }
int User::get_user_type(void) { return USER; }

//MUTATORS
void User::set_username(string username) { this->username = username; }
void User::set_password(string password) { this->password = password; }

//OPERATOR OVERLOADING
bool User::operator < (User& user) {

	if (this->username < user.username) { return true; }
	return false;

}
bool User::operator > (User& user) {

	if (this->username > user.username) { return true; }
	return false;

}

bool User::operator == (string username) {
	if (this->username == username) { return true; }
	return false;
}

bool operator < (string username, User& user){
	if (username < user.username) { return true; }
	return false;
}
bool operator > (string username, User& user){
	if (username > user.username) { return true; }
	return false;
}


//Librarian Class - Subclass of User
int Librarian::get_user_type(void) { return LIBRARIAN; }

fstream& operator << (fstream& fs, Librarian& user) {

	fs << "2\t"
		<< user.get_username() << "\t"
		<< user.get_password() << endl;
	return fs;
}

ostream& operator << (ostream& output, Librarian& user) {
	output << "2\t"
		<< user.get_username() << "\t"
		<< user.get_password() << endl;
	return output;
}
istream& operator >> (istream& input, Librarian& user) {
	std::string username, password;
	input >> username >> password;

	user.set_password(password);
	user.set_username(username);
	return input;
}

//Reader Class - Subclass of User
int Reader::get_user_type(void) { return READER; }
string Reader::get_type(void) { return type; }

void Reader::borrow_book(Book_Copy* b) {

	b->set_reader(username, add_days(current_date(), borrow_time));
	checked_books.push_back(b);
	

}

void Reader::renew_book(Book_Copy* b) {
	return_book(b);
	borrow_book(b);
}

void Reader::return_book(Book_Copy* b) {

	if (b->is_expired()) { penalities++; }
	checked_books.remove(&b);
	b->returning();

}

void Reader::reserve_book(Book* b) {

	reserved_books.push_back(b);
	b->reservedB.push_back(username);

}
void Reader::cancel_book_reservation(Book* b) {

	reserved_books.remove(&b);
	b->reservedB.remove(&username);

}

bool Reader::can_borrow(void) {
	if (checked_books.get_size() >= max_copies) { return false; }
	for (int i = 0; i < checked_books.get_size(); i++) {
		Book_Copy* b = *checked_books[i];
		if (b->is_expired()) { return false; }
	}

	return true;
}

int Reader::get_max_copies(void) { return max_copies; }
int Reader::get_max_borrow_time(void) { return borrow_time; }
int Reader::get_penalities(void) { return penalities; }

//Student Class - Subclass of Reader
Student::Student() {
	max_copies = 5;
	borrow_time = 30;
	username = "NULL";
	password = "NULL";
}

Student::Student(string username, string password, string t) : Student::Student() {
	this->username = username;
	this->password = password;
	type = t;
}

int Student::get_user_type(void) { return STUDENT; }

//Teacher Class - Subclass of Reader
Teacher::Teacher() {
	max_copies = 10;
	borrow_time = 50;
	username = "NULL";
	password = "NULL";
}

Teacher::Teacher(string username, string password, string t) : Teacher::Teacher() {
	this->username = username;
	this->password = password;
	type = t;
}

int Teacher::get_user_type(void) { return TEACHER; }

fstream& operator << (fstream& fs, Reader& user) {
	int type = 0;
	if (user.get_type() == "Teacher") { type = 1; }

	fs << type << "\t"
		<< user.get_username() << "\t"
		<< user.get_password() << endl;
	return fs;

}

ostream& operator << (ostream& output, Reader& user) {
	int type = 0;
	if (user.get_type() == "Teacher") { type = 1; }

	output << type << "\t"
		<< user.get_username() << "\t"
		<< user.get_password() << endl;
	return output;
}
istream& operator >> (istream& input, Reader& user) {
	int type;
	std::string username, password;
	input >> type >> username >> password;

	user.type = type;
	user.set_password(password);
	user.set_username(username);
	return input;
}