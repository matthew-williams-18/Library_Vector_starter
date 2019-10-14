#include <string>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iostream>

#include "../includes_usr/library.h"
#include "../includes_usr/datastructures.h"
#include "../includes_usr/fileIO.h"
using namespace std;

int nextPatronID();

//NOTE: please ensure patron and book data are loaded from disk before calling the following
//NOTE: also make sure you save patron and book data to disk any time you make a change to them
//NOTE: for files where data is stored see constants.h BOOKFILE and PATRONFILE

vector<book> bookCollection;
vector<patron> patronList;

/*
 * clear books and patrons containers
 * then reload them from disk 
 */
void reloadAllData(){
	loadBooks(bookCollection, (char*)BOOKFILE);
	loadPatrons(patronList, (char*)PATRONFILE);
}

/* checkout a book to a patron
 * first load books and patrons from disk
 * make sure patron enrolled (patronid is assigned to a patron in patrons container)
 * make sure book in collection (bookid is assigned to a book in books container)
 * 
 * see if patron can check out any more books 
 * 	if not return TOO_MANY_OUT patron has the MAX_BOOKS_ALLOWED_OUT
 * 	
 * if so then check the book out to the patron, set the following fields for the book in the
 * books container
 * book.loaned_to_patron_id = patronid;
 * book.state = OUT;
 * 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS checkout worked
 *         PATRON_NOT_ENROLLED
 * 		   BOOK_NOT_IN_COLLECTION
 *         TOO_MANY_OUT patron has the max number of books allowed checked out
 */
int checkout(int bookid, int patronid){
	reloadAllData();

	//test by ID if patron is in the patronList
	bool patronFound = false;
	patron person;
	for (patron p : patronList){
		if (p.patron_id == patronid)
			patronFound = true; person = p; break;
	}
	//if not on list, return PATRON_NOT_ENROLLED
	if(!patronFound)
		return PATRON_NOT_ENROLLED;

	//test by ID if book is in the bookCollection
	bool bookFound = false;
	book novel;
	int bIndex = 0;
	for (book b : bookCollection){
		if (b.book_id == bookid)
			bookFound = true; novel = b; break;
		bIndex++;
	}
	//if not in collection, return BOOK_NOT_IN_COLLECTION
	if(!bookFound)
		return BOOK_NOT_IN_COLLECTION;

	if (person.number_books_checked_out >= MAX_BOOKS_ALLOWED_OUT)
		return TOO_MANY_OUT;


	bookCollection[bIndex].loaned_to_patron_id = patronid;
	bookCollection[bIndex].state = OUT;

	saveBooks(bookCollection, (char*)BOOKFILE);
	savePatrons(patronList, (char*)PATRONFILE);

	return SUCCESS;
}

/* check a book back in 
 * first load books and patrons from disk
 * make sure book in collection (bookid is assigned to a book in books container)
 *  	
 * if so find the the patron the book is checked out to and decrement his/hers number_books_checked_out
 * then check the book back in by marking the book.loaned_to_patron_id = NO_ONE and the book.state = IN;
 
 * Finally save the contents of the books and patrons containers to disk
 * 
 * returns SUCCESS check-in worked
 * 		   BOOK_NOT_IN_COLLECTION
 */
int checkin(int bookid){
	reloadAllData();

	//test by ID if patron is in the bookCollection
	bool bookFound = false;
	book novel;
	int bIndex = 0;
	for (book b : bookCollection){
		if (b.book_id == bookid)
			bookFound = true; novel = b; break;
		bIndex++;
	}
	//if not in collection, return BOOK_NOT_IN_COLLECTION
	if(!bookFound)
		return BOOK_NOT_IN_COLLECTION;

	for (int i = 0; i < patronList.size(); i++){
		if (patronList[i].patron_id == novel.loaned_to_patron_id)
			patronList[i].number_books_checked_out--; break;
	}

	bookCollection[bIndex].loaned_to_patron_id = NO_ONE;
	bookCollection[bIndex].state = IN;

	return SUCCESS;
}

/*
 * enroll a patron, duplicate names are fine as patrons are uniquely identified by their patronid
 * first load books and patrons from disk
 * create a patron object, initialize its fields as appropriate, assign him/her the nextPatronID
 * then push the patron onto the patrons container
 * save all changes to the patrons container to disk
 * return 
 *    the patron_id of the person added
 */
int enroll(std::string &name){
	reloadAllData();

	patron neu;
	neu.patron_id = nextPatronID;
	neu.name = name;
	neu.number_books_checked_out = 0;

	return neu.patron_id;
}

/*
 * the number of books in the books container
 * (ie. if 3 books returns 3)
 * 
 */
int numbBooks(){ return bookCollection.size(); }

/*
 * the number of patrons in the patrons container
 * (ie. if 3 patrons returns 3)
 */
int numbPatrons(){ return patronList.size(); }

/*the number of books patron has checked out
 *
 *returns a positive number indicating how many books are checked out 
 *        or PATRON_NOT_ENROLLED         
 */
int howmanybooksdoesPatronHaveCheckedOut(int patronid){
	//test by ID if patron is in the patronList
	for (patron p : patronList){
		if (p.patron_id == patronid)
			return p.number_books_checked_out;
	}
	//if not on list, return PATRON_NOT_ENROLLED
	return PATRON_NOT_ENROLLED;
}

/* search through patrons container to see if patronid is there
 * if so returns the name associated with patronid in the variable name
 * 
 * returns SUCCESS found it and name in name
 *         PATRON_NOT_ENROLLED no patron with this patronid
 */
int whatIsPatronName(std::string &name,int patronid){
	//test by ID if patron is in the patronList
	for (patron p : patronList){
		if (p.patron_id == patronid)
			name = p.name; return SUCCESS;
	}
	//if not on list, return PATRON_NOT_ENROLLED
	return PATRON_NOT_ENROLLED;
}

int nextPatronID(){
	int highestIDNum = -1;  //initialize to a number certain to be lower than any possible setting for a patron ID
	for (patron p : patronList){
		highestIDNum = (p.patron_id > highestIDNum) ? p.patron_id : highestIDNum;
	}

	return highestIDNum+1;
}
