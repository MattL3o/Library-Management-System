#include "User.h"
#include "Book.h"
#include "Algorithms.h"
#include <string>

Binary_Sorted_Tree<Librarian> librarians;	//BST of librarians in student.txt
Binary_Sorted_Tree<Reader> readers;			//BST of readers in student.txt
User* current_user;							//Pointer to current user
Reader* current_reader;						//Pointer to current user of reader type. Is only used when reader is using the system
Librarian* current_librarian;

int console_mode;							//READER for a student or teacher, LIBRARIAN for a librarian

void open_books(void);						//Opens all books and book copies from book.txt and copy.txt


void initialize(void);						//Opens the users and prompts login. Also sets initial time
void open_users(void);						//Opens the "student.txt" file and stores the users in either librarians or readers linked list
void prompt_login(void);					//Prompts user to login, changes current user and console mode accordingly
void print_header(string message);			//Prints customizable header message

void open_menu(void);						//Opens either librarian menu or reader menu based on console mode
void librarain_menu(void);					//Opens librarian main menu
void reader_menu(void);						//Opens reader main menu


void search_books(void);					//Allows the user to search for a specific book - i.e book_copy class not book class
void borrow_book(void);						//User can borrow book by ISBN
void view_checked_out_books(void);			//Allows users to see the books they have checked out
void print_all_users_books(void);			//Prints all of a users books, for use in view_checked_out_books and renew_book
void renew_book(void);						//Allows user to renew one of their books if no one else has reserved it
void my_information(void);					//Prints user info
void change_password(void);					//User can change their password
void shutdown(void);						//exits the console and saves all the data back to files
void rewrite_files(void);					//on call rewrites the program data files, used in "shutdown()"
											
void check_reservation_status(Book* b);		//checks whether someone needs to be kicked off the reservation list
void check_reservation_statuses(void);		//checks all books with function above

											//Fucntions by Matt
void add_books();							//Adds books to the library
void return_book();							//Returns books that were borrowed
void reserve_book();						//Reserves books for the user
void cancel_reservation();					//Cancels specified reservation
void search_users();						
void delete_books();
void add_users();
void delete_user();
void my_information_lib();
void change_password_lib();

void Feeling_Lucky();

int main(void) {

	
	initialize();

	while (1) {
		system("cls");

		open_menu();
	}
	

}

void initialize(void) {
	set_clock();
	open_users();
	open_books();
	prompt_login();
}

//To be called in main loop ONLY
//Checks reservation status of a book and deletes if needed
void check_reservation_status(Book* b) {


	int available_copies = b->copy_list.search_by_secondary_key_book_copy(false, SEARCH_BY_BORROWED).get_size();
	if (b->reservedB.get_size() == 0 || available_copies == 0) { return; }					//Under these conditions, there is no need to check reservation status

	int days_remaining = current_date() - b->last_date_check;
	b->days_before_reservation_expiration -= days_remaining;

	while (b->days_before_reservation_expiration < 1) {

		if (b->reservedB.get_size() == 0) { break; }
		string username = *b->reservedB[0];
		readers.search(username)->cancel_book_reservation(b);
		b->days_before_reservation_expiration += 5;

	}

	b->last_date_check = current_date();


}

void check_reservation_statuses(void) {

	//Every time the system goes to the menu, the system checks if anyone has reservations that have expired
	//and deletes them if necessary
	for (int i = 0; i < books.get_size(); i++) {
		check_reservation_status(books[i]);
	}

}

void open_users(void) {
	fstream f;
	f.open("student.txt");
	if (f.fail()) { cout << "Could not open student file!"; exit(1); }

	int type;
	string un, pw;
	f >> type >> un >> pw;
	while (!f.eof()) {

		if (type == 0) { readers.insert(Student(un, pw, "Student")); }
		else if (type == 1){ readers.insert(Teacher(un, pw, "Teacher")); }
		else if (type == 2) { librarians.insert(Librarian(un, pw)); }

		f >> type >> un >> pw;
	}

	f.close();
}

void open_books(void) {

	fstream f;
	f.open("book.txt");
	if (f.fail()) { cout << "Could not open student file!"; exit(1); }

	while (!f.eof()) {
		Book b;
		f >> b;

		if (b.get_title() == "") { break; }

		books.insert(b);

	}

	f.close();

	f.open("copy.txt");
	if (f.fail()) { cout << "Could not open student file!"; exit(1); }

	long long int ISBN;
	int id;
	f >> ISBN >> id;

	while (!f.eof()) {

		Book_Copy b(id, ISBN);
		
		Book* parent = books.search(ISBN);


		if (parent != nullptr) { parent->copy_list.insert(b); }

		f >> ISBN >> id;

	}
	f.close();
}

void prompt_login(void) {

	system("cls");

	print_header("Login to Library System");
	cout << "Enter username, or type '-1' to shut down the system\n";

	while (1) {

		string username, password;

		cout << "Username: ";
		cin >> username;

		if (username == "-1") {
			shutdown();
		}

		cout << "Password: ";
		cin >> password;


		Reader* user = readers.search(username);
		if (user != nullptr && user->get_password() == password) {
			
			current_user = user;
			current_reader = user;
			console_mode = READER;
			return;

		}

		Librarian* l_user = librarians.search(username);
		if (l_user != nullptr && l_user->get_password() == password) {

			current_user = l_user;
			current_librarian = l_user;
			console_mode = LIBRARIAN;
			return;
		}

		cout << "You've entered an invalid username or password! Please try again!" << endl;

	}

}

void shutdown(void) {
	rewrite_files();
	cout << "System has succesfully exited.";
	exit(1);
}


void open_menu(void) {
	if (console_mode == READER) { reader_menu(); }
	else { librarain_menu(); }
}

void librarain_menu(void) {

	system("cls");

	print_header("Welcome to My Library!");
	cout << "Welcome back, " << current_user->get_username() << endl << endl;

	cout << "Please choose:" << endl
		<< "\t1 -- Add Books\n"
		<< "\t2 -- Delete Books\n"
		<< "\t3 -- Search Users\n"
		<< "\t4 -- Add Users\n"
		<< "\t5 -- Delete Users\n"
		<< "\t6 -- My Information\n"
		<< "\t7 -- Change Password\n"
		<< "\t0 -- Log out\n";

	int choice;
	cin >> choice;

	check_reservation_statuses();

	switch (choice) {
	case 1: system("cls"); add_books(); break;
	case 2: system("cls"); delete_books(); break;
	case 3: system("cls"); search_users(); break;
	case 4: system("cls"); add_users(); break;
	case 5: system("cls"); delete_user(); break;
	case 6: system("cls"); my_information_lib(); break;
	case 7: change_password_lib(); break;
	case 0: system("cls"); prompt_login(); break;
	}


}

void change_password_lib(void) {

	system("cls");

	print_header("Change your password");
	cout << "Enter current password: ";

	string password;
	cin >> password;
	if (password != current_librarian->get_password()) {
		cout << "Invalid password\n";
		system("pause");
		change_password();
		return;
	}

	cout << "Enter your new password: ";
	cin >> password;
	current_librarian->set_password(password);

	cout << "password succesfully changed\n";
	system("pause");
	return;


}

void my_information_lib() {
	print_header("My Information");
	cout << "Username: " << current_user->get_username() << endl;
	cout << "Password: " << current_user->get_password() << endl << endl;
	system("pause");
	return;
}

void add_users() {
	print_header("Add a new user");
	string username, password;
	int type;
	cout << "Enter Type (0 = Student, 1 = Teacher, 2 = Librarian): ";
	cin >> type;
	cout << "Enter Username (Enter 0 to return to menu): ";
	cin >> username; if (username == "0") { system("cls"); return; }
	Librarian* l = librarians.search(username);
	Reader* r = readers.search(username);
	if (l != nullptr) {
		cout << "Username exists, please try again!" << endl;
		system("pause");
		system("cls");
		add_users();
		return;
	}
	else if (r != nullptr) {
		cout << "Username exists, please try again!" << endl;
		system("pause");
		system("cls");
		add_users();
		return;
	}
	cout << "Enter Password (Enter 0 to return to menu): ";
	cin >> password; if (password == "0") { system("cls"); return; }
	if (type == 0) { readers.insert(Student(username, password, "Student")); }
	else if (type == 1) { readers.insert(Teacher(username, password, "Teacher")); }
	else if (type == 2) { librarians.insert(Librarian(username, password)); }
	cout << "User has been added!" << endl;
	system("pause");
	system("cls");
	return;
}

void delete_user() {
	print_header("Delete an existing user");
	string username;
	int index = 0, index2 = 0;
	cout << "Enter Username (Enter 0 to return to menu): ";
	cin >> username; if (username == "0") { system("cls"); return; }
	Librarian* l = librarians.search(username);
	Reader* r = readers.search(username);
	if (l != nullptr) {
		librarians.remove(username);
		cout << username << " has been removed from the system!" << endl;
		system("pause");
		system("cls");
		return;
	}
	else if (r != nullptr) {
		if (r->checked_books.get_size() != 0) {
			cout << username << " still has books and cannot be removed" << endl;
			system("pause");
			system("cls");
			return;
		}
		while (r->reserved_books.node_return(index2) != nullptr) {
			while (r->reserved_books.node_return(index)->value->reservedB.node_return(index)->value != username) {
				index++;
			}
			r->reserved_books.node_return(index)->value->reservedB.remove(index);
			index = 0;
			index2++;
		}
		readers.remove(username);
		cout << username << " has been removed from the system!" << endl;
		system("pause");
		system("cls");
		return;
	}
	else {
		cout << "User does not exist" << endl;
		system("pause");
		system("cls");
		return;
	}
}

void search_users() {
	print_header("Search for a user");
	int index = 0;
	string username;
	Reader* r;
	Librarian* l;
	cout << "Enter Username (Enter 0 to return to menu): ";
	cin >> username; if (username == "0") { system("cls"); return; }
	r = readers.search(username);
	l = librarians.search(username);
	if (r != nullptr) {
		cout << "Username: " << r->get_username() << endl;
		cout << "Password: " << r->get_password() << endl;
		cout << "Type: " << r->get_type() << endl;
		cout << "Current books: " << endl;
		while (r->checked_books.node_return(index) != nullptr) {
			cout << "\t";
			cout << "ISBN: ";
			cout << r->checked_books.node_return(index)->value->ISBN;
			cout << " ID: ";
			cout << r->checked_books.node_return(index)->value->get_id() << endl;
			index++;
		}
	}
	else if (l != nullptr) {
		cout << "Username: " << l->get_username() << endl;
		cout << "Password: " << l->get_password() << endl;
	}
	else { cout << "That user does not exist!\n"; }
	system("pause");
	return;
}

int greatest_id(void) {

	int largest_all = 0;

	for (int i = 0; i < books.get_size(); i++) {

		Book* b = books[i];

		int largest_id = b->copy_list.greatest_element()->get_id();

		if ( largest_id > largest_all) {
			largest_all = largest_id;
		}

	}

	return largest_all;

}

Book_Copy* find_book_copy(int id) {

	for (int i = 0; i < books.get_size(); i++) {

		Book* b = books[i];

		Book_Copy* book_copy = b->copy_list.search(id);
		if (book_copy != nullptr) { return book_copy; }

	}

	return nullptr;
}

void add_books() {
	print_header("Request a new Copy");
	long long int isbn;
	int id = greatest_id() + 1;
	string title, author, category;
	Book_Copy b2;
	cout << "Enter ISBN (Enter 0 to return to menu): ";
	cin >> isbn; if (isbn == 0) { system("cls"); return; }
	cout << "Enter Title (Enter 0 to return to menu): ";
	cin >> title; if (title == "0") { system("cls"); return; }
	cout << "Enter Author (Enter 0 to return to menu): ";
	cin >> author; if (author == "0") { system("cls"); return; }
	cout << "Enter Genre (Enter 0 to return to menu): ";
	cin >> category; if (category == "0") { system("cls"); return; }
	Book* parent = books.search(isbn);
	if (parent == nullptr) {
		Book b = Book(isbn, title, category, author);
		books.insert(b);
		parent = books.search(isbn);
		b2 = Book_Copy(id, isbn);
		parent->copy_list.insert(b2);
	}
	else {
		b2 = Book_Copy(id, isbn);
		parent->copy_list.insert(b2);
	}
	cout << "Copy has been added to the library!" << endl;
	system("pause");
	system("cls");
	return;
}

void delete_books() {
	print_header("Remove a book copy");
	int id;
	cout << "Enter the book ID (Enter 0 to return to menu): ";
	cin >> id; if (id == 0) { return; }
	
	Book_Copy* book_copy = find_book_copy(id);
	if (book_copy == nullptr) {
		cout << "That is not a valid ID" << endl;
		system("pause");
		return;

	}
	
	if (book_copy->is_borrowed()) {

		Reader* r = readers.search(book_copy->get_reader());
		r->return_book(book_copy);

	}
	

	Book* parent = books.search(book_copy->ISBN);
	parent->copy_list.remove(id);
	cout << "Book copy has been removed from library" << endl;
	if (parent->copy_list.is_empty()) {
		books.remove(parent->get_ISBN());
		cout << "This Book has been removed from our library" << endl;
	}
	system("pause");
	system("cls");
	return;
}

void reader_menu(void) {
	print_header("Welcome to My Library!");
	cout << "Welcome back, " << current_user->get_username() << endl << endl;

	cout << "Please choose:" << endl
		<< "\t1  -- Search Book\n"
		<< "\t2  -- Borrow Book\n"
		<< "\t3  -- Return Book\n"
		<< "\t4  -- Renew Book\n"
		<< "\t5  -- Reserve Book\n"
		<< "\t6  -- Cancel Book Reservation\n"
		<< "\t7  -- My Information\n"
		<< "\t8  -- Change Password\n"
		<< "\t9  -- I'm Feeling Lucky\n"
		<< "\t10 -- View Your Checked-out Books\n"
		<< "\t0  -- Log out\n";

	int choice;
	cin >> choice;

	check_reservation_statuses();

	switch (choice) {
	case 1: search_books(); break;
	case 2: borrow_book(); break;
	case 3: system("cls"); return_book(); break;
	case 4: renew_book();  break;
	case 5: system("cls"); reserve_book(); break;
	case 6: system("cls"); cancel_reservation(); break;
	case 7: my_information(); break;
	case 8: change_password(); break;
	case 9: system("cls"); Feeling_Lucky(); break;
	case 10: view_checked_out_books(); break;
	case 0: system("cls"); prompt_login(); break;
	}
}


void borrow_book() {

	system("cls");

	long long int ISBN = 0;
	print_header("Borrow a book from the library");

	if (!current_reader->can_borrow()) {
		cout << "You cannot borrow more books either because you have an overdue book, or you have checked out the limit!\n";
		system("pause");
		return;
	}


	cout << "Enter the ISBN number of the book, or enter 0 to return to the main menu: ";
	cin >> ISBN;

	if (ISBN == 0) { return; }

	Book* b = books.search(ISBN);

	if (b == nullptr) {
		cout << "That book does not exist." << endl;
		system("pause");
		return;
	}

	Linked_List<Book_Copy*> book_copies = b->copy_list.search_by_secondary_key_book_copy(false, SEARCH_BY_BORROWED);

	if (book_copies.get_size() <= 0) {
		cout << "This book's copies are currently unavailable.\n";
		system("pause");
		return;

	}

	Book_Copy* to_checkout = *book_copies[0];

	//If the user reserved the book
	if (current_reader->reserved_books.find(&b) != -1) {

		//find the index in the reserved list
		string username = current_reader->get_username();
		int index = b->reservedB.find(&username);

		//If there is enough copies for the user with a reservation to check out a book then
		//take it out
		if (index < book_copies.get_size()) {
			current_reader->cancel_book_reservation(b);
			current_reader->borrow_book(to_checkout);
		}
		else {
			cout << "You are still not high enough on the waitlist. Try again soon!\n";
			system("pause");
			return;
		}


	}

	else if (b->reservedB.get_size() >= book_copies.get_size()) {

		cout << "All copies of this book have been reserved.\n";
		system("pause");
		return;

	}
	else { current_reader->borrow_book(to_checkout); }

	cout << "Succesfully checked out:\n";
	to_checkout->print();
	system("pause");

	

}

void print_all_users_books(void) {
	for (int i = 0; i < current_reader->checked_books.get_size(); i++) {
		Book_Copy* b = *(current_reader->checked_books[i]);
		b->print();
	}
}

void view_checked_out_books(void) {

	system("cls");
	print_header("View Your Checked-Out Books");
	print_all_users_books();
	system("pause");

}

void renew_book(void) {

	system("cls");
	print_header("Renew Your Books");
	cout << "Your Books: " << endl;
	print_all_users_books();

	int id = -1;

	cout << "Enter a book ID to renew or -1 to return: ";
	cin >> id;

	if (id == -1) { return; }

	Book_Copy* b = nullptr;

	for (int i = 0; i < current_reader->checked_books.get_size(); i++) {
		Book_Copy* this_bc = *(current_reader->checked_books[i]);
		if (this_bc->get_id() == id) {
			b = this_bc;
			break;
		}
	}

	if (b == nullptr) {
		cout << "That is an invalid ID\n";
		system("pause");
		return;
	}

	Book* parent = books.search(b->ISBN);
	;

	//Determines whether the amount of people reserving the book is more than the amount of available copies
	//is yes the user will be unable to renew their book
	//if no the user can renew because the people who reserved it can borrow a different unborrowed copy

	int available_copies = parent->copy_list.search_by_secondary_key_book_copy(false, SEARCH_BY_BORROWED).get_size();
	int reservees = parent->reservedB.get_size();

	if (reservees != 0 && reservees  >= available_copies) {
		cout << "You cannot renew this book because it is reserved by another user\n";
		system("pause");
		return;
	}

	current_reader->renew_book(b);

	cout << "Successfully Renewed: \n";
	b->print();
	system("pause");


}

void my_information(void) {

	system("cls");
	print_header(current_reader->get_username() + "'s Information");
	cout << "Username: " << current_reader->get_username() << endl
		<< "Password: " << current_reader->get_password() << endl
		<< "Maximum Book Allotment: " << current_reader->get_max_copies() << endl
		<< "Maximum Borrow Time: " << current_reader->get_max_borrow_time() << endl
		<< "Penalities: " << current_reader->get_penalities()
		<< endl
		<< "Current borrowed books: \n";
	print_all_users_books();
	system("pause");
	return;
}

void change_password(void) {

	system("cls");

	print_header("Change your password");
	cout << "Enter current password: ";
	
	string password;
	cin >> password;
	if (password != current_reader->get_password()) {
		cout << "Invalid password\n";
		system("pause");
		change_password();
		return;
	}

	cout << "Enter your new password: ";
	cin >> password;
	current_reader->set_password(password);

	cout << "password succesfully changed\n";
	system("pause");
	return;


}

void return_book() {
	print_header("Return a Book");
	int id, index = 0, index2 = 0, choice;
	cout << "Enter book ID (Enter 0 to return to menu): ";
	cin >> id;
	if (id == 0) { system("cls"); return; }

	Book_Copy* returning = nullptr;
	for (int i = 0; i < current_reader->checked_books.get_size(); i++) {

		Book_Copy* cur_book = *current_reader->checked_books[i];
		if (cur_book->get_id() == id) {
			returning = cur_book;
			break;
		}
	}
	if (returning == nullptr) {
		cout << "You did not borrow this book\n";
		system("pause");
		return;
	}

	Book* parent = books.search(returning->ISBN);

	current_reader->return_book(returning);

	cout << "Book has been returned!" << endl;
	cout << "Did you enjoy the book? (1 = yes , Any other number = no): ";
	cin >> choice;
	if (choice == 1) { parent->favor++; }
	system("pause");
	system("cls");
	return;
}



void search_books(void) {

	int option = -1;

	system("cls");
	print_header("Search for a book");
	cout << "Please select an option:" << endl
		<< "\t1) Search by ISBN\n"
		<< "\t2) Search by Title\n"
		<< "\t3) Search by Category\n"
		<< "\t0) Return to menu\n";

	cout << "Option: ";
	cin >> option;

	Linked_List<Book_Copy*> book_list;
	Linked_List<Book*> parents;

	string input_string;

	Book* b = nullptr;
	switch (option) {
	case 0: return;
	case 1:
		long long int ISBN;
		cout << "Enter ISBN: ";
		cin >> ISBN;
		b = books.search(ISBN);
		if(b != nullptr){ parents.push_back(b); }
		break;
	case 2:
		cout << "Enter Title: ";
		cin >> input_string;
		parents = books.search_by_secondary_key_book(input_string, SEARCH_BY_TITLE);
		break;
	case 3:
		cout << "Enter Category: ";
		cin >> input_string;
		parents = books.search_by_secondary_key_book(input_string, SEARCH_BY_SUBJECT);
		break;

	default:
		cout << "That was an invalid option, try again!\n";
		system("pause");
		search_books();
		return;
	}


	//Stops execution if no books were found
	if (parents.get_size() == 0) {
		cout << "No books matched that search criteria\n";
		system("pause");
		return;
	}

	quick_sort(parents, &Book::get_ISBN);
	quick_sort(parents, &Book::get_author);
	quick_sort(parents, &Book::get_title);			//Sort parents in order


	//Combine all copies into one list
	for (int i = 0; i < parents.get_size(); i++) {
		
		Linked_List<Book_Copy*> copies = (**parents[i]).copy_list.BST_to_linked_list();
		for (int j = 0; j < copies.get_size(); j++) {
			book_list.push_back(*copies[j]);
		}

	}

	quick_sort(book_list, &Book_Copy::get_expiration_date);
	quick_sort(book_list, &Book_Copy::is_borrowed);
	
	for (int i = 0; i < book_list.get_size(); i++) { (*book_list[i])->print(); }

	system("pause");

}

void reserve_book() {
	long long int isbn;
	print_header("Reserve a Book");
	cout << "Enter ISBN (Enter 0 to return to menu): ";
	cin >> isbn;
	if (isbn == 0) { system("cls"); return; }
	Book* book = books.search(isbn);

	for (int i = 0; i < current_reader->checked_books.get_size(); i++) {
		Book_Copy* bc = *(current_reader->checked_books[i]);
		if (bc->ISBN == isbn) {
			cout << "You are already borrowing this book\n";
			system("pause");
			return;
		}
	}

	for (int i = 0; i < current_reader->reserved_books.get_size(); i++) {
		Book* b = *current_reader->reserved_books[i];
		if (b->get_ISBN() == isbn) {
			cout << "You've already reserved this book\n";
			system("pause");
			return;
		}
	}

	if (book == nullptr) {
		cout << "That book does not exist.\n";
		system("pause");
		return;
	}

	Linked_List<Book_Copy*> bcl = book->copy_list.search_by_secondary_key_book_copy(false, SEARCH_BY_BORROWED);
	
	if (bcl.get_size() == 0) {
		current_reader->reserve_book(book);
		cout << "Book has been reserved!" << endl;
		system("pause");
		return;
	}
	else {
		cout << "A copy is available! No reservation needed!" << endl;
		system("pause");
		return;
	}
}

void cancel_reservation() {
	long long int isbn;
	print_header("Cancel Book Reservation");
	cout << "Enter ISBN (Enter 0 to return to menu): ";
	cin >> isbn; if (isbn == 0) { system("cls"); return; }
	Book* book = books.search(isbn);

	if (book == nullptr) {
		cout << "That book does not exist\n";
		system("pause");
		return;
	}

	string username = current_reader->get_username();
	int reservation_index = book->reservedB.find(&username);
	if (reservation_index == -1) {
		cout << "No reservation for this user on this book exists" << endl;
		system("pause");
		system("cls");
		return;
	}

	current_reader->cancel_book_reservation(book);

	cout << "Reservation has been removed" << endl;
	system("pause");
	system("cls");
	return;
}

void Feeling_Lucky() {
	print_header("Feeling Lucky? ");

	Linked_List<Book*> lucky = books.BST_to_linked_list();

	cout << "Here's Top Ten Liked Books: " << endl << endl;

	//sort it ascending order and then reverse
	quick_sort(lucky, &Book::get_favor);
	lucky.reverse();

	for (int i = 0; i < 10; i++) {
		if (lucky[i] == nullptr) { break; }
		Book* b = *lucky[i];
		cout << "\t" << b->get_title() << " Total Likes: " << b->get_favor() << endl;
	}
	cout << endl;
	system("pause");
	return;
}

void rewrite_files(void) {

	//Rewrite Book Files
	fstream fs_b, fs_bc;
	fs_b.open("book.txt", std::ofstream::out | std::ofstream::trunc);
	fs_bc.open("copy.txt", std::ofstream::out | std::ofstream::trunc);

	if (fs_b.fail()) {
		cout << "Could not open book.txt";
		exit(1);
	}
	if (fs_bc.fail()) {
		cout << "Could not open copy.txt";
		exit(1);
	}

	//Opens one book at a time and saves it to book file
	//While saving book, saves all book_copies to copies file
	for (int i = 0; i < books.get_size(); i++) {
		Book* book = books[i];
		fs_b << *book;
		for (int j = 0; j < book->copy_list.get_size(); j++) {
			Book_Copy* book_copy = book->copy_list[j];
			fs_bc << *book_copy;
		}
	}
	fs_b.close();
	fs_bc.close();
	
	///Rewrite User file
	fstream fs_u;
	fs_u.open("student.txt", std::ofstream::out | std::ofstream::trunc);
	
	if (fs_u.fail()) {
		cout << "Could not open student.txt";
		exit(1);
	}

	//print all librarians to file
	for (int i = 0; i < librarians.get_size(); i++) {
		fs_u << *librarians[i];
	}

	for (int i = 0; i < readers.get_size(); i++) {
		fs_u << *readers[i];
	}

	fs_u.close();


}


void print_header(string message) {

	message = message + " | " + current_date().to_string();

	const int length = 70;

	if (message.length() > (length - 2)) { std::cout << "Header Error\n"; return; }

	string tmp = "";
	for (int i = 0; i < length; i++) { tmp += "="; }
	std::cout << tmp << std::endl;												//Print top line

	tmp = "|";
	for (int i = 0; i < length - 2; i++) { tmp += " "; }
	tmp += "|";																	//Print Space Line
	std::cout << tmp << std::endl;


	int space_num = (length - message.length() - 2) / 2;						//Start middle line build

	tmp = "|";
	for (int i = 0; i < space_num; i++) { tmp += " "; }
	tmp += message;

	space_num = length - tmp.length() - 1;
	for (int i = 0; i < space_num; i++) { tmp += " "; }

	tmp += "|";
	std::cout << tmp << std::endl;									//Print Middle line

	tmp = "|";
	for (int i = 0; i < length - 2; i++) { tmp += " "; }
	tmp += "|";																	//Print Space Line
	std::cout << tmp << std::endl;

	tmp = "";
	for (int i = 0; i < length; i++) { tmp += "="; }
	std::cout << tmp << std::endl << std::endl;									//Print bottom line

}