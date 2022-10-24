/*
	WELCOME TO ABCU COURSE PLANNER
		   BY Austin Palmer

	This program takes an input CSVfile and stores the data in a
	binary tree where you can then sort and serarch for the data
	You have the options of:
	1). Loading course data
	2). Print course list
	3). Print course data
	4). Exit

	The data will be organized in an alphanumeric style.

	Reference for binary search tree:
	SNHU CS300: Binary Search Tree Assignment by Austin Palmer

	Reference for filtering through a CSV file:
	https://www.geeksforgeeks.org/csv-file-management-using-c/

	Enjoy!
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

// Create course structure
struct Course {
	string courseId, courseName;
	vector<string> preRequisites;
	// Constructor
	Course() {}
};

// Create nodes to store in Binary Tree
struct Node {
	Course course;
	Node* left;
	Node* right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a course
	Node(Course a) :
		Node() {
		course = a;
	}
};

class BinarySearchTree {

private:
	Node* root;

	void addNode(Node* node, Course course);
	Node* removeNode(Node* node, string courseId);
	void inOrder(Node* node);

public:
	// Constructor
	BinarySearchTree();
	// Order
	void InOrder();
	// Insert course
	void Insert(Course course);
	// Remove course
	void Remove(string courseId);
	// Search for course
	Course Search(string courseId);
};

// BST constructor
BinarySearchTree::BinarySearchTree() {
	// Set root to nullptr
	root = nullptr;

}

// BST InOrder from root
void BinarySearchTree::InOrder() {
	// FixMe (2): In order root
	// call inOrder fuction and pass root
	inOrder(root);
}


// BST In order
void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		// InOrder left
		inOrder(node->left);
		cout << node->course.courseId << ": "
			<< node->course.courseName << " | "
			<< node->course.preRequisites[0] << " | "
			<< node->course.preRequisites[1] << endl;
		// InOder right
		inOrder(node->right);
	}
}

// BST Search
Course BinarySearchTree::Search(string courseId) {
	// set current node equal to root
	Node* current = root;

	// keep looping downwards until bottom reached or matching courseId found
	while (current != nullptr) {
		// if match found, return current course
		if (current->course.courseId.compare(courseId) == 0) {
			return current->course;
		}
		// if course is smaller than current node then traverse left
		if (current->course.courseId.compare(courseId) > 0) {
			current = current->left;
		}
		// else larger so traverse right
		else {
			current = current->right;
		}
	}
	Course course;
	return course;
}

// BST Insert
void BinarySearchTree::Insert(Course course) {
	// if root does not equal null ptr
	if (root != nullptr) {
		// root is equal to new node course
		root = new Node(course);
	}
	// else
	else {
		// add Node root and course
		addNode(root, course);
	}
}

// BST Remove
void BinarySearchTree::Remove(string courseId) {
	// remove node root courseId
	removeNode(root, courseId);
}

// BST addNode
void BinarySearchTree::addNode(Node* node, Course course) {
	// if node is larger then add to left
	if (node->course.courseId.compare(course.courseId) > 0) {
		// if no left node
		if (node->left == nullptr) {
			// this node becomes left
			node->left = new Node(course);
		}
		// else recurse down the left node
		else {
			node = node->left;
		}
	}
	// else
	else {
		// if no right node
		if (node->right == nullptr) {
			// this node becomes right
			node->right = new Node(course);
		}
		//else
		else {
			// recurse down the left node
			node = node->left;
		}
	}
}

// BST removeNode
Node* BinarySearchTree::removeNode(Node* node, string courseId) {
	// Avoid crash
	if (node == nullptr) {
		return node;
	}

	// recurse left
	if (courseId.compare(node->course.courseId) < 0) {
		node->left = removeNode(node->left, courseId);
	}
	// recurse right
	else if (courseId.compare(node->course.courseId) > 0) {
		node->right = removeNode(node->right, courseId);
	}
	else {
		// if both children are nullptr
		if (node->left == nullptr && node->right == nullptr) {
			delete node;
			node = nullptr;
		}
		// if left child is not nullptr
		else if (node->left != nullptr && node->right == nullptr) {
			Node* temp = node;
			node = node->left;
			delete temp;
		}
		// if right child is not nullptr
		else if (node->right != nullptr && node->left == nullptr) {
			Node* temp = node;
			node = node->right;
			delete temp;
		}
		// if neither children are nullptr
		else {
			// Create node to the right
			Node* temp = node->right;
			// while left child is not nullptr
			while (temp->left != nullptr) {
				// move to the left
				temp = temp->left;
			}
			// insert at next available location
			node->course = temp->course;
			// remove node to the right
			node->right = removeNode(node->right, temp->course.courseId);
		}
	}
	return node;
}

int main()
{
	int choice = 0;
	vector<string> row;
	fstream inFile;
	string fileName, line, word, temp, courseId;

	// Define a binary search tree to hold all courses
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Course course;

	// Prompt for file
	cout << "Welcome to the course planner." << endl;
	cout << "Enter the directory of the file you would like to open."
		<< endl;
	cin >> fileName;

	// Open file
	inFile.open(fileName);

	// Check that file opened properly
	if (inFile.fail()) {
		cout << "Error - Failed to open " << fileName << endl;
		return -1;
	}

	// While choice is not exit
	while (choice != 9) {
		// Display main menu
		cout << "1: Load Data Structure." << endl;
		cout << "2: Print Course List." << endl;
		cout << "3: Print Course." << endl;
		cout << "9: Exit." << endl;
		cout << endl;

		// Prompt for user slection
		cout << "Select an option from the menu: " << endl;
		cin >> choice;

		// Exit if first choice is 9
		if (choice == 9) {
			cout << "Thank you for using the course planner! Goodbye"
				<< endl;
			return 0;
		}
		// Else check choice
		switch(choice) {
			// Load data structure
			case 1:
				// While not end of file
				while (inFile >> temp) {
					unsigned int i = 0;
					row.clear();
					// Get first line
					getline(inFile, line);
					// Initialize string stream
					stringstream s(line);
					// Separate words by each comma
					while (getline(s, word, ',')) {
						// Push word back in vector row
						row.push_back(word);
						i++;
					}
					// Loop through line
					course.courseId = row[0];
					course.courseName = row[1];
					// If there is a prerequisite
					if (i == 2) {
						course.preRequisites[0] = row[2];
					}
					// If there are two prerequisites
					if (i == 3) {
						course.preRequisites[1] = row[3];
					}
					bst->Insert(course);
				}
				break;
			// Print Course List
			case 2:
				cout << "Here is a sample schedule: " << endl;
				// Sort in order
				bst->InOrder();
				break;
			// Print Course
			case 3:
				cout << "What course do you want to know about? " << endl;
				cin >> courseId;
				// Find course
				bst->Search(courseId);
				break;

			// Invalid option
			default:
				cout << choice << " is not a valid option" << endl;
				break;
		}

		// Clear screen
		for (unsigned t = 0; t < 10; t++) {
			cout << endl;
		}
	}

	inFile.close();

}
