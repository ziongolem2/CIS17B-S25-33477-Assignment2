// Saul Resendiz
// CIS-17B
// Lesson 2: Library Base system

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class Book
{
private:
	string title;
	string author;
	string ISBN;
	bool availability;

public:
	Book(string ttl, string auth, string I) : title(ttl), author(auth), ISBN(I), availability(true) {}

	string getTitle() const
	{
		return title;
	}

	string getISBN() const
	{
		return ISBN;
	}

	string getAuthor() const
	{
		return author;
	}

	bool availabilityReturn() const
	{
		return availability;
	}

	void setAvailableStatus(bool status) { availability = status; }

	void display() const
	{
		cout << "Book Title: " << title << ", Author: " << author << ", ISBN: " << ISBN
			<< ", Available? ";

		if (availability)
		{
			cout << "Yes";
		}
		else
		{
			cout << "No";
		}

		cout << endl;
	}
};

class User
{
private:
	string name;
	int userID;
	vector<string> userBorrowedBooks;	// List of borrowed books

public:
	User(string usersName, int usersID) : name(usersName), userID(usersID) {}

	string getName() const
	{
		return name;
	}

	int getUserID() const
	{
		return userID;
	}

	// Functions for borrowed books
	void borrowBook(const string& isbn)
	{
		userBorrowedBooks.push_back(isbn);
	}

    // Function for returned books
	void returnBook(const string& isbn);

};


class Student : public User		// Inherits from User
{
public:
	Student(string usersName, int usersID) : User(usersName, usersID) {}
};


class Faculty : public User		// Also inherits from User
{
public:
	Faculty(string usersName, int usersID) : User(usersName, usersID) {}
};

// Creating our Library class as a Singleton (only one instance of this class exists)
class Library
{
private:
	static Library* xptr;

	vector<Book> vectorOfBooks;
	vector<User*> vectorOfUsers;

	Library() {}

public:
	static Library* getInstance()
	{
		if (xptr == nullptr)
			xptr = new Library();

		return xptr;
	}

	//Functions

	Book* searchBook(const string& isbn)    // For searching a book (isbn)
	{
		for (int i = 0; i < vectorOfBooks.size(); i++)  // iterating through vector of books
		{
			if (vectorOfBooks[i].getISBN() == isbn) 
			{
				return &vectorOfBooks[i];  // return book address 
			}
		}
		return nullptr;  // if N/A
	}

	void addBook(const Book& book)  // Adding book object to vector of Books
	{
		vectorOfBooks.push_back(book); 
	}

	void registerUser(User* user)   // Adding user obj to vector of Users
	{
		vectorOfUsers.push_back(user); 
	}

	void borrowBook(int usersID, const string& isbn)    // func to borrow a book
	{
	    // Making sure a user with the given ID exists before letting borrow book
	    User* user = searchUser(usersID);
        if (!user) // If user with the corresponding ID not found:
    {
        cout << "Sorry, User with ID " << usersID << " doesn't exist." << endl;
        return;  
    }
    
		Book* book = searchBook(isbn);
		if (book)	// If book is not a null ptr
		{                  
			if (book->availabilityReturn())  
			{                                           // Check book availability
				book->setAvailableStatus(false);    
				cout << "Book borrowed successfully." << endl;
			}
			else 
			{
				cout << "Book is unavailable." << endl; 
			}
		}
		else
		{
			cout << "Book not found." << endl;    // iNVALID input
		}
	}

	void returnBook(int usersID, const string& isbn)    // Returning book
	{
		// searching for book in library first
		Book* book = searchBook(isbn);

		if (book)   // if book is not a nullptr
		{
			// make sure book is currently borrowed
			if (!book->availabilityReturn())
			{
				// make the book available again
				book->setAvailableStatus(true);
				cout << "Book returned successfully." << endl;
			}
			else
			{
				// book not found
				cout << "Book was never borrowed." << endl;
			}
		}
		else
		{
			// Book not in system
			cout << "No book found" << endl;
		}
	}
	
	void searchByTitle(const string& title)
{
    bool exists = false;
    for (int i = 0; i < vectorOfBooks.size(); i++)  // Iterating through books vector
    {
        if (vectorOfBooks[i].getTitle() == title)  // see if they match
        {
            vectorOfBooks[i].display();  // if found, display its info
            exists = true;
        }
    }

    if (!exists) // if not found
    {
        cout << "No books found." << endl; 
    }
}

    // Search for book by author
   void searchByAuthor(const string& author)
{
    bool exists = false;
    for (int i = 0; i < vectorOfBooks.size(); i++)  // Iterating through books vector
    {
        if (vectorOfBooks[i].getAuthor() == author)  // see if they match
        {
            vectorOfBooks[i].display();  // if book found, display its info
            exists = true;
        }
    }

    if (!exists)
    {
        cout << "No books found." << endl; 
    }
}

User* searchUser(int userID)    // For user ID validation
{
    for (int i = 0; i < vectorOfUsers.size(); i++) //Iterate through all possilbe users 
    {
        if (vectorOfUsers[i]->getUserID() == userID)
        {
            return vectorOfUsers[i];  // Return the corresponding User
        }
    }
    return nullptr;  //If none found
}


};

Library* Library::xptr = nullptr;	// Initialgize static instance

// LIBRARY MENU DISPLAY

int main() 
{
	Library* library = Library::getInstance();
	int choice;

	while (true) 
	{
	cout << "\nWelcome to the library! Enter 1-7, your choice. \n";
	cout << "1. Add Book\n";
	cout << "2. Register New User\n";
	cout << "3. Borrow Book\n";
	cout << "4. Return Book\n";
    cout << "5. Search Book by Title\n";
    cout << "6. Search Book by Author\n";
    cout << "7. Exit\n";

		cout << "Enter your choice: ";
		cin >> choice;
		cin.ignore();

		if (choice == 1)    // Add Book
		{
			string title, author, isbn;
			cout << "Enter book title: "; getline(cin, title);
			cout << "Enter author: "; getline(cin, author);
			cout << "Enter ISBN: "; getline(cin, isbn);
			library->addBook(Book(title, author, isbn));
			cout << "\nBook successfully added." << endl;;
		}
		else if (choice == 2)   // Register user (Student or Faculty)
		{
		    int userType;
		    cout << "Enter 1 if student, or 2 if faculty." << endl;
			string name;
			int id;
			
			cin >> userType;
			cin.ignore();
			
			if (userType == 1)
			{
			cout << "Enter user name: "; 
			getline(cin, name);
			cout << "Enter user ID: "; 
			cin >> id;
			library->registerUser(new Student(name, id));
			cout << "\nUser registered with ID# :" << id << endl;
			}
			
			else if (userType ==2)
			{
			cout << "Enter user name: "; 
			getline(cin, name);
			cout << "Enter user ID: "; 
			cin >> id;
			library->registerUser(new Faculty(name, id));
            cout << "\nUser registered with ID# :" << id << endl;			}
			
			else
			{
			    cout << "\nInvalid Input." << endl;
			}
		
		}
		else if (choice == 3)   // Borrw Book (with valid ID validation)
		{
			int userID;
			string isbn;
			cout << "Enter user ID: "; cin >> userID;
			cout << "Enter ISBN: "; cin >> isbn;
			library->borrowBook(userID, isbn);
		}
		else if (choice == 4)   // Return Book
		{
			int userID;
			string isbn;
			cout << "Enter user ID: "; cin >> userID;
			cout << "Enter ISBN: "; cin >> isbn;
			library->returnBook(userID, isbn);
		}
		else if (choice == 5)   // Search book w/ Title
		{
            string title;
            cout << "Enter book title to search: ";
            getline(cin, title);
            library->searchByTitle(title);
        }
        else if (choice == 6)   // Search book w/ Author
        {
            string author;
            cout << "Enter author name to search: ";
            getline(cin, author);
            library->searchByAuthor(author);
        }
        else if (choice == 7)   // Exit Program
        {
            cout << "Thanks for using Library System!" << endl;
            break;
        }
        else 
        {
            cout << "Invalid choice." << endl;
        }
    }

	return 0;
}
