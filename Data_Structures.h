#pragma once

#define SEARCH_BY_AUTHOR	1
#define SEARCH_BY_SUBJECT	2
#define SEARCH_BY_TITLE		3
#define SEARCH_BY_BORROWED	4

using std::string;

//Linked List
template <typename type>
struct Node{
	type value;
	Node<type>* next = nullptr;
	Node(type value) { this->value = value; }
	~Node(){}
};

template <typename type>
class Linked_List {
private:
	Node<type>* head = nullptr;
	Node<type>* cur_node = nullptr;
	int cur_node_index = -1;
	int size = 0;

	void reset_cur_node(void) {
		if (head == nullptr) {
			cur_node = nullptr;
			cur_node_index = -1;
		}
		else {
			cur_node = head;
			cur_node_index = 0;
		}
	}

public:
	Linked_List(){}

	//For list traversing
	//uses property of linked list to make for loops in main file more effiecent
	//Stores previously visited node in "cur_node" so returning to get the next element in the list takes less iterations
	Node<type>* node_return(int index) {

		if (head == nullptr) { return nullptr; }

		if (cur_node == nullptr || cur_node_index > index) {
			reset_cur_node();
		}
		while (cur_node_index < index) {

			cur_node = cur_node->next;
			cur_node_index++;
		}

		return cur_node;

	}

	//Creates a new node at the specified index of the list assuming the list is already this big
	void push(type val, int index){
		size++;
		Node<type>* insert = new Node<type>(val);

		if (index <= 0) {
			insert->next = head;
			head = insert;
			reset_cur_node();
			return;
		}

		Node<type>* cur = node_return(index - 1);

		Node<type>* tmp = cur->next;
		cur->next = insert;
		insert->next = tmp;

		reset_cur_node();

	}

	int push_back(type val) {
		push(val, size);
		//Returns index it's stored at
		return size - 1;
		
	}

	void remove(int index) {
		size--;
		Node<type>* cur;

		if (index <= 0) {
			cur = head;
			head = head->next;
			delete cur;
			reset_cur_node();
			return;
		}

		cur = node_return(index - 1);
		Node<type>* removal = cur->next;
		cur->next = removal->next;
		delete removal;

		reset_cur_node();

	}

	void remove(type* value) {

		int index = find(value);
		if(index != -1){ remove(index); }
	}

	int get_size(void) { return size; }

	type* operator [] (const int index) {
		return &(node_return(index)->value);
	}

	int find(type* valueptr) {

		if (head == nullptr) { return -1; }
		Node<type>* nptr = head;
		int index = 0;
		while (1) {
			
			if (nptr == nullptr) { return -1; }
			if (nptr->value == *valueptr) { return index; }
			nptr = nptr->next;
			index++;
		}

	}

	void reverse(void) {

		int i = 0;
		int j = get_size() - 1;

		while (i < j) {
			swap(*operator[](i), *operator[](j));
			i++;
			j--;
		}

		reset_cur_node();

	}

};

//Binary Sorted Tree
template <typename type>
struct BST_Node {
	type value;
	BST_Node<type>* left = nullptr;
	BST_Node<type>* right = nullptr;
	BST_Node(type value) { this->value = value; }
	~BST_Node() {}
};

template <typename type>
class Binary_Sorted_Tree {
private:
	BST_Node<type>* head = nullptr;
	int size = 0;
public:
	
	Binary_Sorted_Tree() {}
	~Binary_Sorted_Tree() {}
	Binary_Sorted_Tree(type head) { this->head = new BST_Node<type>(head); }

	int get_size(void) { return size; }

	void insert(type value) {

		size++;

		if (head == nullptr) {
			this->head = new BST_Node<type>(value);
			return;
		}
		
		BST_Node<type>* cur_node = head;

		while (1) {

			if (value < cur_node->value) { 
				if (cur_node->left == nullptr) {
					cur_node->left = new BST_Node<type>(value);
					return;
				}
				else { cur_node = cur_node->left; }
			}
			else { 
				if (cur_node->right == nullptr) {
					cur_node->right = new BST_Node<type>(value);
					return;
				}
				else { cur_node = cur_node->right; }
			}	
		}
	}
	bool is_empty(void) {
		if (head == nullptr) { return true; }
		return false;
	}
private:
	template <typename search_type>
	BST_Node<type>* remove_sub(BST_Node<type>* root, search_type value) {

		if (root == nullptr) { return root; }
		else  if (value < root->value) { root->left = remove_sub(root->left, value); }
		else if (value > root->value) { root->right = remove_sub(root->right, value); }
		else {

			if (root->left == nullptr && root->right == nullptr) {
				delete root;
				root = nullptr;
			}

			else if (root->left == nullptr) {
				BST_Node<type>* tmp = root;
				root = root->right;
				delete tmp;
			}

			else if (root->right == nullptr) {
				BST_Node<type>* tmp = root;
				root = root->left;
				delete tmp;
			}

			else {
				BST_Node<type>* tmp = least_in_tree(root->right);
				root->value = tmp->value;
				root->right = remove_sub(root->right, tmp->value);
			}

		}
		
		return root;
	}

	BST_Node<type>* least_in_tree(BST_Node<type>* root) {
		while (1) {

			if (root->left == nullptr) { return root; }
			else { root = root->left; }

		}
	}

public:
	template <typename search_type>

	void remove(search_type value) {
		size--;
		head = remove_sub(head, value);
		return;
		

	}

//Returns a linked list from a binary sorted tree for further data processing
private:
	void BST_to_linked_list_sub(BST_Node<type>* node, Linked_List<type*>& list) {

		if (node == nullptr) { return; }
		BST_to_linked_list_sub(node->left, list);
		list.push_back(&(node->value));
		BST_to_linked_list_sub(node->right, list);

	}
public:
	Linked_List<type*> BST_to_linked_list(void) {
		Linked_List<type*> list;
		BST_to_linked_list_sub(head, list);
		return list;
	}


private:

	template<typename search_type>
	BST_Node<type>* search_sub(search_type value) {

		BST_Node<type>* cur_node = head;

		while (1) {


			if (cur_node->value == value) {
				return cur_node;
			}


			if (value < cur_node->value) {
				if (cur_node->left == nullptr) { return nullptr; }
				cur_node = cur_node->left;
			}
			else {
				if (cur_node->right == nullptr) { return nullptr; }
				cur_node = cur_node->right;
			}

		}

	}

public:
	template <typename search_type>
	type* search(search_type value) {

		return &(search_sub(value)->value);

	}

private:
	//TO BE USED BY BOOK CLASS BST ONLY
	template <typename v_type>
	void search_by_secondary_sub(v_type value, v_type(type::*fptr)(void), Linked_List<type*>& list, BST_Node<type>* node) {

		if (node != nullptr) {
			search_by_secondary_sub(value, fptr, list, node->left);
			if ((node->value.*fptr)() == value) { list.push_back(&(node->value)); }
			search_by_secondary_sub(value, fptr, list, node->right);
		}
	}
public:
	
	//TO BE USED BY BOOK CLASS BST ONLY
	template <typename v_type>
	Linked_List<type*> search_by_secondary_key_book(v_type value, int key) {

		if (head == nullptr) { return Linked_List<type*>(); }

		v_type(type:: * fptr)(void);
		if (key == SEARCH_BY_AUTHOR) { fptr = &type::get_author; }
		else if (key == SEARCH_BY_SUBJECT) { fptr = &type::get_subject; }
		else /* if (key == SEARCH_BY_TITLE) */ { fptr = &type::get_title; }

		Linked_List<type*> book_list;
		search_by_secondary_sub(value, fptr, book_list, head);
		return book_list;
	}

	//TO BE USED BY BOOK_COPY CLASS BST ONLY
	template <typename v_type>
	Linked_List<type*> search_by_secondary_key_book_copy(v_type value, int key) {

		if (head == nullptr) { return Linked_List<type*>(); }

		v_type(type:: * fptr)(void);
		/* if (key == SEARCH_BY_BORROWED) {*/ fptr = &type::is_borrowed;  //}
		Linked_List<type*> book_list;
		search_by_secondary_sub(value, fptr, book_list, head);
		return book_list;
	}

	private:
	void bracket_sub(BST_Node<type>* head, type*& ret_val, int& index) {

		if (ret_val != nullptr) { return; }

		if(head->left != nullptr) {
			bracket_sub(head->left, ret_val, index);
		}

		if (index == 0) {
			if (ret_val == nullptr) { ret_val = &(head->value);	}
			return;
		}

		index--;

		if(head->right != nullptr) {
			bracket_sub(head->right, ret_val, index);
		}
		
		return;
	}

	public:
	type* operator [] (const int index) {
		
		int index_count = index;
		type* ret_val = nullptr;
		bracket_sub(head, ret_val, index_count);
		return ret_val;
		
	}

	private:
		void greatest_element_sub(BST_Node<type>* head, type*& greatest) {

			if (head->right != nullptr) {
				greatest_element_sub(head->right, greatest);
			}
			else { greatest = &(head->value); }
		}

	public:
		type* greatest_element(void) {

			type* return_type = nullptr;
			greatest_element_sub(head, return_type);

			return return_type;

		}

};