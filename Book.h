#pragma once
#include <iostream>
#include "Data_Structures.h"
#include "time.h"
#include <fstream>

using std::ostream;
using std::istream;

using std::fstream;
using std::string;

class Book_Copy {
private:
	Date expiration_date;
	bool borrowed = 0;
	int ID;
	string readers_name = "";
public:
	long long int ISBN;
	//constructors
	Book_Copy();
	Book_Copy(int ID, long long int ISBN);

	//accessors
	bool is_borrowed(void);
	Date get_expiration_date(void);
	bool is_expired(void);
	int get_id(void);
	void print(void);
	string get_reader(void);
	void returning(void);					//Do not call this to return a book - instead return it through the reader

	//mutators
	void set_reader(string username, Date expiration_date);

	//operator overloading
	bool operator > (Book_Copy& book);
	bool operator < (Book_Copy& book);
	bool operator == (int ID);
	bool operator == (Book_Copy book);
	friend bool operator < (int ID, Book_Copy& book);
	friend bool operator > (int ID, Book_Copy& book);

	friend fstream& operator << (fstream& fs, Book_Copy& book);
	friend ostream& operator << (ostream& os, Book_Copy& book);
	friend istream& operator >> (istream& input, Book_Copy& book);
};

class Book {
private:
	long long int ISBN;
	string title;
	string subject;
	string author;



public:
	int favor;
	Binary_Sorted_Tree<Book_Copy> copy_list;
	Linked_List<string> reservedB;
	Book();
	Book(long long int ISBN, string title, string subject, string author);
	long long int get_ISBN();
	string get_title();
	string get_subject();
	string get_author();
	int get_favor();
	friend fstream& operator >> (fstream& fs, Book& book);
	bool operator > (Book& book);
	bool operator < (Book& book);
	bool operator ==(long long int ISBN);
	friend bool operator < (long long int ISBN, Book& book);
	friend bool operator > (long long int ISBN, Book& book);
	friend fstream& operator << (fstream& fs, Book& book);
	friend ostream& operator << (ostream& os, Book& book);
	friend istream& operator >> (istream& input, Book& book);
	void returning_copy(void);

	//Used to check whether reservations have expired or not
	//Does not make sense to have them as private because they need to be accessed freely in the main function
	Date last_date_check = Date();
	int days_before_reservation_expiration = 5;
};

extern Binary_Sorted_Tree<Book> books;