#include <fstream>
#include <stdexcept>
#include <string>

#include "../includes_usr/fileIO.h"
using namespace std;

std::string convertBookToString(book workingBook);
std::string convertPatronToString(patron workingPatron);
book parseInputToBook(std::string workingString);
patron parseInputToPatron(std::string workingString);
void removeUnwantedChars(std::string &data);

/* clears, then loads books from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries in books
 * 			SUCCESS if all data is loaded
 * */
int loadBooks(std::vector<book> &books, const char* filename)
{
	//clear book vector
	books.clear();

	//load data from file "filename" into book vector
	ifstream inputFile;
	std::string file = std::string(filename);
	inputFile.open(file.c_str(), ios::in);

	if (!inputFile.is_open())
		return COULD_NOT_OPEN_FILE;

	std::string line;

	while (!inputFile.eof()){
		getline(inputFile, line);
		book b;
		b = parseInputToBook(line);
		books.push_back(b);
	}

	inputFile.close();

	//return correct return code

	if (books.size() == 0)
		return NO_BOOKS_IN_LIBRARY;

	return SUCCESS;
}

/**
 * takes in a string containing a line of data from an input file and attempts to parse it into a book
 * returns the book
 */
book parseInputToBook(std::string workingString){
	removeUnwantedChars(workingString);

	string id, title, author, state, patronID;
	int indexOfLastSubstr = 0;
	for (int i = 0; i < sizeof(workingString); i++){
		if (workingString[i] == ','){
			if (id == UNINITIALIZED){
				id = workingString.substr(indexOfLastSubstr, i-indexOfLastSubstr + 1);
				indexOfLastSubstr = i+2;
			}
			else if (title == UNINITIALIZED){
				title = workingString.substr(indexOfLastSubstr, i-indexOfLastSubstr + 1);
				indexOfLastSubstr = i+2;
			}
			else if (author == UNINITIALIZED){
				author = workingString.substr(indexOfLastSubstr, i-indexOfLastSubstr + 1);
				indexOfLastSubstr = i+2;
			}
			else if (state == UNINITIALIZED){
				state = workingString.substr(indexOfLastSubstr, i-indexOfLastSubstr + 1);
				indexOfLastSubstr = i+2;
			}
		}
	}
	patronID = workingString.substr(indexOfLastSubstr);

	book parsedBook;
	parsedBook.book_id = int(id);
	parsedBook.title = title;
	parsedBook.author = author;
	parsedBook.state = int(state);
	parsedBook.loaned_to_patron_id = int(patronID);

	return parsedBook;
}

/* serializes books to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_BOOKS_IN_LIBRARY if there are 0 entries books (do not create file)
 * 			SUCCESS if all data is saved
 * */
int saveBooks(std::vector<book> &books, const char* filename)
{
	if (sizeof(books) == 0)
		return NO_BOOKS_IN_LIBRARY;

	//open file
	ofstream outputFile;
	std::string file = std::string(filename);
	outputFile.open(file.c_str(), ios::out);

	if (!outputFile.is_open())
		return COULD_NOT_OPEN_FILE;

	for (book b : books){
		outputFile << convertBookToString(b) + "\n";
	}

	outputFile.close();

	return SUCCESS;
}

std::string convertBookToString(book workingBook){
	//get book data, convert to std::string if not already that type,
	return (std::string)workingBook.book_id + "," + workingBook.title + ","
			+ workingBook.author + "," + (std::string)workingBook.state + "," + (std::string)workingBook.loaned_to_patron_id;
}

/* clears, then loads patrons from the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons
 * 			SUCCESS if all data is loaded
 * */
int loadPatrons(std::vector<patron> &patrons, const char* filename)
{
	//clear patron vector
	patrons.clear();


	//load data from "filename" into book vector
	ifstream inputFile;
	std::string file = std::string(filename);
	inputFile.open(file.c_str(), ios::in);

	if (!inputFile.is_open())
		return COULD_NOT_OPEN_FILE;

	std::string line;

	while(!inputFile.eof()){
		getline(inputFile, line);
		patron p;
		p = parseInputToPatron(line);
		patrons.push_back(p);
	}

	inputFile.close();

	//return correct code
	if(patrons.size() == 0)
		return NO_PATRONS_IN_LIBRARY;
	return SUCCESS;
}

patron parseInputToPatron(std::string workingString){
	removeUnwantedChars(workingString);

	string id, name, numBooksCheckedOut;
	int indexOfLastSubstr = 0;
	for (int i = 0; i < sizeof(workingString); i++){
		if (workingString[i] == ','){
			if (id == UNINITIALIZED){
				id = workingString.substr(indexOfLastSubstr, i-indexOfLastSubstr + 1);
				indexOfLastSubstr = i+2;
			}
			else if (name == UNINITIALIZED){
				name = workingString.substr(indexOfLastSubstr, i-indexOfLastSubstr + 1);
				indexOfLastSubstr = i+2;
			}
		}
	}
	numBooksCheckedOut = workingString.substr(indexOfLastSubstr);

	patron parsedPatron;
	parsedPatron.patron_id = int(id);
	parsedPatron.name = name;
	parsedPatron.number_books_checked_out = int(numBooksCheckedOut);

	return parsedPatron;
}

/* serializes patrons to the file filename
 * returns  COULD_NOT_OPEN_FILE if cannot open filename
 * 			NO_PATRONS_IN_LIBRARY if there are 0 entries in patrons  (do not create file)
 * 			SUCCESS if all data is saved
 * */
int savePatrons(std::vector<patron> &patrons, const char* filename)
{
	if (sizeof(patrons) == 0)
			return NO_BOOKS_IN_LIBRARY;

		//open file
		ofstream outputFile;
		std::string file = std::string(filename);
		outputFile.open(file.c_str(), ios::out);

		if (!outputFile.is_open())
			return COULD_NOT_OPEN_FILE;

		for (patron p : patrons){
			outputFile << convertPatronToString(p) + "\n";
		}

		outputFile.close();

		return SUCCESS;
}

std::string convertPatronToString(patron workingPatron){
	return (std::string)workingPatron.patron_id + "," + workingPatron.name + "," + workingPatron.number_books_checked_out;
}

void removeUnwantedChars(std::string &data){
	try{
		for (int i = 0; i < sizeof(data); i++){
			switch(data[i]){

			case "\"":
			case " ": data = data.substr(0,i) + data.substr(i+1);
			break;

			default: break;
			}
		}
	}
	catch(const std::out_of_range &e){ }
}
