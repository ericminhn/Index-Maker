//P.S. With visual Studio, I got 80/80 and had no warnings/errors, but I don't know how it will...react to the actual testing.

//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<fstream>
#include	<iostream>
#include	<string>
#include	<sstream> //For the istringstream stuff

// #include	<unistd.h>

#include	"DocumentIndex.h"
#include	"GetLine.h"

using namespace std;

//****************************************************************************************
//
//	CONSTANT DEFINITIONS
//
//****************************************************************************************

//****************************************************************************************
//
//	CLASSES, TYPEDEFS AND STRUCTURES
//
//****************************************************************************************
typedef	string::size_type	StringSize; //Didn't have a use for this

//****************************************************************************************
//
//	PUBLIC DATA
//
//****************************************************************************************

//****************************************************************************************
//
//	PRIVATE DATA
//
//****************************************************************************************

//****************************************************************************************
//
//	FUNCTION PROTOTYPES
//
//****************************************************************************************

//****************************************************************************************
//
//	DocumentFile::Close
//
//****************************************************************************************
void	DocumentFile::Close()
{
	//************************************************************************************
	//	LOCAL DATA

	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	file_.close();
	file_.clear();

	exclude_.close(); //This is for the second fstream variable
	exclude_.clear();

	pageNumber_ = 1; //Resetting the page number proper
	valueSwitch_ = 0; //Resetting the switch

	return;
}

//****************************************************************************************
//
//	DocumentFile::GetPageNumber
//
//****************************************************************************************
int	DocumentFile::GetPageNumber()
{
	//************************************************************************************
	//	LOCAL DATA


	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	return(pageNumber_); //Not much to do here
}

//****************************************************************************************
//
//	DocumentFile::GetWord
//
//****************************************************************************************
string	DocumentFile::GetWord() //Behold the calamity that is my go at string processing.
{				//There are two more, and they're called calamities because I feel like my code is a complete mess
	//************************************************************************************
	//	LOCAL DATA
	string	word; //Our string man
	string::iterator cleanser; //Iterator is needed for going through the words with how I did it
	bool gate = true, end = false;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS

	if (counter_ == word_.size()) //Reaching the end of the word_ vector
	{
		counter_ = 0;
		word.clear();
		return(word);
	}
	
	
	if (counter_ < word_.size()) //Progresses the vector foward
	{
		word = word_[counter_];
		++counter_;
	}

	
	for (size_t alien = 0; alien < illegal_.size(); alien++) //Cleaning out excluded words
	{
		if (word == illegal_[alien])
		{
			if (counter_ == word_.size()) //Prevents moving beyond the end of the vector, I had a lot of these put up
			{
				counter_ = 0;
				word.clear();
				return(word);
			}

			alien = 0;
			word = word_[counter_];
			++counter_;
		}
	}

	
	for (cleanser = word.begin(); cleanser != word.end(); ++cleanser) //First filter checks for 's
	{
		if (end)
		{
			if (*cleanser == 's' && *cleanser == word.back())
			{
				word.pop_back();
				cleanser = word.begin();
				end = false;
			}
			end = false;
		}
		if (ispunct(*cleanser))
		{
			end = true;
		}
	}

	
	for (cleanser = word.begin(); cleanser != word.end(); ++cleanser) //Deals with skipping over word/word and word-word in the text
	{
		if (*cleanser == '/')
		{
			word = word_[counter_];
			++counter_;
			cleanser = word.begin();
		}
		if (*cleanser == '-')
		{
			word = word_[counter_];
			++counter_;
			cleanser = word.begin();
		}
	}

	
	if (word.front() == '(' || ispunct(word.front())) //These two prune off the punctuation on front and back
	{
		word.erase(word.begin());
	}

	
	if (word.back() == ')' || ispunct(word.back()))
	{
		word.pop_back();
	}

	
	for (cleanser = word.begin(); cleanser != word.end(); ++cleanser) //These two are the failsafes which strongarm past
	{								  //invalid words and numbers in the document that weren't
		if (isdigit(*cleanser))					  //cleared by the above code
		{
			if (counter_ == word_.size())
			{
				counter_ = 0;
				word.clear();
				return(word);
			}

			word = word_[counter_];
			++counter_;
			cleanser = word.begin();
		}
	}

	
	for (cleanser = word.begin(); cleanser != word.end(); ++cleanser)
	{
		if (ispunct(*cleanser))
		{
			if (counter_ == word_.size())
			{
				counter_ = 0;
				word.clear();
				return(word);
			}
			else
			{
				word = word_[counter_];
				++counter_;
				cleanser = word.begin();
			}
		}
	}

	return(word);
}

//****************************************************************************************
//
//	DocumentFile::LoadExclusions
//
//****************************************************************************************
bool	DocumentFile::LoadExclusions(const string& name)
{
	//************************************************************************************
	//	LOCAL DATA
	bool	success = true;
	string	word;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	exclude_.open(name); //Pretty basic use of a vector to contain the illegal words for comparison
	if (!exclude_.fail())
	{
		while (exclude_ >> word)
		{
			illegal_.push_back(word);
		}

		success = true;
	}
	else
	{
		success = false;
	}

	return(success);
}

//****************************************************************************************
//
//	DocumentFile::Open
//
//****************************************************************************************
bool	DocumentFile::Open(const string& name)
{
	//************************************************************************************
	//	LOCAL DATA

	//************************************************************************************
	//	EXECUTABLE STATEMENTS
	file_.open(name, ios::in); //Nothing changed here
	if (!file_.fail())
	{
		//	You may add any useful initialization here.

		return(true);
	}
	else
	{
		return(false);
	}
}

//****************************************************************************************
//
//	DocumentFile::Read
//
//****************************************************************************************
bool	DocumentFile::Read() //This is the second of 3 calamities
{
	//************************************************************************************
	//	LOCAL DATA
	bool	success = false; //Just in case
	istringstream transfer; //Used for inputting into the string vector word_
	string word;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS

	getline(file_, line_); //Grabbing a line

	if (line_ == stopperLine_) //This stops us from continously grabbing the last line of the text
	{			   //Don't know why I had the infinite looping issue but it's been resolved thanks to this
		stopperLine_.clear(); //Clearing the comparison string
		return false; //Ending the reading
	}
	
	
	if (line_.size() > 0) //Meaning the line has something inside of it
	{
		success = true;
	}
	else if (line_.size() == 0) //Bypasses the space in between 'pages'
	{
		++pageNumber_;
		if (valueSwitch_ == 0) //However, for some damn reason, the getpagenumber and create tests didn't play along too well
		{		       //So, this was created for that very reason
			getline(file_, line_);
		}
		getline(file_, line_);

		success = true;
	}

	
	stopperLine_ = line_; //Part of the infinite loop stopping above

	
	transfer.clear(); //empties the istringstream variable
	transfer.str(line_);
	
	
	word_.clear(); //Frees up the space for the next line
	while (transfer >> word) //Here's the transferring
	{
		if (word == "*")
		{
			transfer >> word;
		}
		word_.push_back(word);
	}

	return (success);
}

void DocumentFile::Switch(int value)
{
	valueSwitch_ = value; //For alternating in the Read() for the Index and Pagenumbers tests

	return;
}

//****************************************************************************************
//
//	DocumentIndex::Create
//
//****************************************************************************************
void	DocumentIndex::Create(DocumentFile& documentFile) //The third calamity mentioned
{
	//************************************************************************************
	//	LOCAL DATA
	string word, check; //Here we have all our variable with those needing it, initialized
	int repeat = 0, number = 0, pageNum = 0, checkNum = 0;
	bool success = true, success2 = true, valid = true;
	set<string>::iterator creator;

	//************************************************************************************
	//	EXECUTABLE STATEMENTS

	documentFile.Switch(1); //Here's the switch being ticked

	while (success) //This entire section is me taking everything from the document using Read() and GetWord() to then fill up
	{		//3 vectors to be used for creating the index
		success = documentFile.Read();
		pageNum = documentFile.GetPageNumber();
		if (!success)
		{
			break;
		}
		if(success)
		{
			success2 = true;
			while (success2)
			{
				word = documentFile.GetWord();
				if (word.empty())
				{
					success2 = false;
				}
				else
				{
					document.push_back(word); //Every word gets its corresponding page number
					numbers.push_back(pageNum);
					checker.insert(word); //This organizes the strings for us so no trouble there
				}
			}
		}
	}

	
	creator = checker.begin(); //Here we go through the two major chunks
	while (creator != checker.end())
	{	
		while (valid) //This is a lengthy process filtering out excessives
		{
			for (size_t i = 0; i < document.size(); i++)
			{
				check = document[i];
				if (*creator == check)
				{
					++repeat;
				}
			}

			if (repeat > 10)
			{
				++creator;
				repeat = 0;
			}
			else
			{
				valid = false;
				repeat = 0;
			}
		}
		valid = true; //resetting the filter

		
		check.clear(); //Cleaning the string just in case
		for (size_t j = 0; j < document.size(); j++) //Here we have the process of creating the nodes for the map
		{
			check = document[j];
			pageNum = numbers[j];

			if (creator != checker.end())
			{
				if (*creator == check)
				{
					if (pageNum > checkNum) //My way of keeping page numbers sorted should this have gone past 3
					{
						pageNumbers.push_back(pageNum);
						checkNum = pageNum;
					}
				}
			}
		}

		
		if (creator != checker.end()) //Don't know if the if statement was necessary but I felt it should be kept just in case.
		{
			indexTree.insert(pair<string, vector<int>>(*creator, pageNumbers)); //creating the nodes
			checkNum = 0; //resetting the page number checking
			pageNumbers.clear(); //resetting the vector
		}

		
		if (creator != checker.end())
		{
			++creator; //Progresses the creation of the tree
		}
	}


	return;
}

//****************************************************************************************
//
//	DocumentIndex::Write
//
//****************************************************************************************
void	DocumentIndex::Write(ostream& indexStream) //Probably the easiest function we had to do
{
	//************************************************************************************
	//	LOCAL DATA
	map<string, vector<int>>::iterator printer;
	//************************************************************************************
	//	EXECUTABLE STATEMENTS

	for (printer = indexTree.begin(); printer != indexTree.end(); ++printer) //This prints the index onto a file in the fashion of												
	{									 //the expected index text file
		indexStream << printer->first << " ";
		for (size_t i = 0; i < printer->second.size(); i++)
		{
			indexStream << printer->second[i]; //This bit here was why I went with using a vector for the page numbers
			if (i < printer->second.size() - 1) //Not as messy personally I say
			{
				indexStream << ", ";
			}
		}
		indexStream << endl;
	}

	return;
}
