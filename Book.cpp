#include "Book.h"
#include "time.h"
#include <fstream>
#include <iostream>

using std::cout;
using std::endl;

Binary_Sorted_Tree<Book> books;				//Stores all books and book copies

//Book Class
Book::Book() {
	ISBN = 0;
	title = "NULL";
	subject = "NULL";
	author = "NULL";
	favor = 0;
}

Book::Book(long long int ISBN, string title, string subject, string author) {
	this->ISBN = ISBN;
	this->title = title;
	this->subject = subject;
	this->author = author;
	favor = 0;
}

long long int Book::get_ISBN() { return ISBN; }
string Book::get_title() { return title; }
string Book::get_subject() { return subject; }
string Book::get_author() { return author; }
int Book::get_favor() { return favor; }

fstream& operator>> (fstream& fs, Book& book) {

	long long int ISBN;
	string title, subject, author;

	fs >> ISBN >> title >> author >> subject;

	book.ISBN = ISBN;
	book.title = title;
	book.subject = subject;
	book.author = author;

	return fs;
}

bool Book::operator > (Book& book) {

	if (this->ISBN > book.ISBN) { return true; }
	return false;

}
bool Book::operator < (Book& book) {
	if (this->ISBN < book.ISBN) { return true; }
	return false;
}

bool Book::operator ==(long long int ISBN) {
	if (this->ISBN == ISBN) { return true; }
	return false;
}

bool operator < (long long int ISBN, Book& book){
	if (ISBN < book.ISBN) { return true; }
	return false;
}
bool operator > (long long int ISBN, Book& book){
	if (ISBN > book.ISBN) { return true; }
	return false;
}

fstream& operator << (fstream& fs, Book& book) {

	fs << book.ISBN << "\t"
		<< book.title << "\t"
		<< book.author << "\t"
		<< book.subject << endl;

	return fs;
}

void Book::returning_copy(void) {
	last_date_check = current_date();
	days_before_reservation_expiration = 5;
}

ostream& operator << (ostream& os, Book& book) {
	os << book.ISBN << "\n"
		<< book.title << "\n"
		<< book.author << "\n"
		<< book.subject << endl;

	return os;
}


istream& operator >> (istream& input, Book& book) {

	long long int ISBN;
	string title, subject, author;
	input >> ISBN >> title >> author >> subject;
	book.ISBN = ISBN;
	book.title = title;
	book.subject = subject;
	book.author = author;
	return input;
}



//Book_Copy Class
//constructors
Book_Copy::Book_Copy() {
	ID = NULL;
	ISBN = NULL;
	expiration_date = Date();

}

Book_Copy::Book_Copy(int ID, long long int ISBN) {

	this->ID = ID;
	this->ISBN = ISBN;
	expiration_date = Date();

}


//accessors
bool Book_Copy::is_borrowed(void) { return borrowed; }

Date Book_Copy::get_expiration_date(void) { return expiration_date; }

bool Book_Copy::is_expired(void) {
	if (current_date() > expiration_date) { return true; }
	return false;
}

int Book_Copy::get_id(void) { return ID; }

string Book_Copy::get_reader(void) { return readers_name; }


//mutators
void Book_Copy::set_reader(string username, Date expiration_date){
	readers_name = username;
	this->expiration_date = expiration_date;
	borrowed = 1;

}

void Book_Copy::returning(void) {

	Book* b = books.search(this->ISBN);
	b->returning_copy();

	readers_name = "";
	expiration_date = Date();
	borrowed = 0;
}

void Book_Copy::print(void) {
	cout << *this;
}

//operator overloading
bool Book_Copy::operator > (Book_Copy& book) {
	if (this->ID > book.ID) { return true; }
	return false;
}
bool Book_Copy::operator < (Book_Copy& book) {
	if (this->ID < book.ID) { return true; }
	return false;
}

bool Book_Copy::operator == (int ID) {
	if (this->ID == ID) { return true; }
	return false;
}

bool Book_Copy::operator == (Book_Copy book) {
	if (ID == book.ID) { return true; }
	return false;
}

bool operator < (int ID, Book_Copy& book) {
	if (ID < book.ID) { return true; }
	return false;
}

bool operator > (int ID, Book_Copy& book) {
	if (ID > book.ID) { return true; }
	return false;
}

fstream& operator << (fstream& fs, Book_Copy& book) {

	fs << book.ISBN << "\t"
		<< book.ID << endl;

	return fs;
}

ostream& operator << (ostream& os, Book_Copy& book) {
	Book* parent = books.search(book.ISBN);
	os << "Book ID: " << book.ID << endl
		<< "\tTitle: " << parent->get_title() << endl
		<< "\tAuthor: " << parent->get_author() << endl
		<< "\tCategory: " << parent->get_subject() << endl
		<< "\tISBN: " << parent->get_ISBN() << endl
		<< "\tStatus: ";

	if (book.is_borrowed()) { cout << "Borrowed, expiration date is " << book.get_expiration_date().to_string(); }
	else { os << "Not currently borrowed"; }

	os << endl;

	os << "\tReserved by " << parent->reservedB.get_size() << " people" << endl
		<< endl;

	return os;
}
istream& operator >> (istream& input, Book_Copy& book) {
	long long int ISBN;
	int id;
	input >> ISBN >> id;
	book.ISBN = ISBN;
	book.ID = id;
	return input;
}