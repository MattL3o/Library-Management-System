#pragma once
#include <iostream>
#include <list>
#include <fstream>
#include "Book.h"
#include "Data_Structures.h"
#define USER	  0
#define LIBRARIAN 1
#define READER	  2
#define TEACHER	  3
#define STUDENT	  4

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::list;
using std::fstream;

class User {
	protected: 
		string username;
		string password;
	public:
		//CONSTRUCTORS
		User();
		User(string username, string password);

		//ACCESSORS
		string get_username(void);
		string get_password();
		virtual int get_user_type(void);

		//MUTATORS
		void set_username(string username);
		void set_password(string passowrd);

		//OPERATOR OVERLOADING
		bool operator < (User& user);
		bool operator > (User& user);
		bool operator == (string username);
		friend bool operator < (string username, User& user);
		friend bool operator > (string username, User& user);

		
};

class Librarian : public User {
public:
	int get_user_type(void);
	Librarian() : User::User() {}
	Librarian(string username, string password) : User::User(username, password){}
	friend fstream& operator << (fstream& fs, Librarian& user);
	friend ostream& operator << (ostream& output, Librarian& user);
	friend istream& operator >> (istream& input, Librarian& user);

};

class Reader : public User {
protected:
	int max_copies = 0;
	int borrow_time = 0;
	int penalities = 0;
	
public:
	virtual int get_user_type(void);
	string get_type(void);
	string type = " ";
	Linked_List<Book*> reserved_books;
	Linked_List<Book_Copy*> checked_books;
	void borrow_book(Book_Copy* b);					//To be used only when borrowing a book for the first time (NOT RENEWING)
	void renew_book(Book_Copy* b);					//To be used for only renewing
	void return_book(Book_Copy* b);
	void reserve_book(Book* b);
	void cancel_book_reservation(Book* b);
	bool can_borrow(void);
	int get_max_copies(void);
	int get_max_borrow_time(void);
	int get_penalities(void);
	friend fstream& operator << (fstream& fs, Reader& user);
	friend ostream& operator << (ostream& output, Reader& user);
	friend istream& operator >> (istream& input, Reader& user);
};

class Student : public Reader {
public:
	int get_user_type(void);
	Student();
	Student(string username, string password, string t);
};
class Teacher : public Reader {
public:
	int get_user_type(void);
	Teacher();
	Teacher(string username, string password, string t);
};