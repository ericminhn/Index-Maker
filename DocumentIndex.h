//P.S. With visual Studio, I got 80/80 and had no warnings/errors, but I don't know how it will...react to the actual testing.

#ifndef	DocumentIndex_h
#define	DocumentIndex_h

//****************************************************************************************
//
//	INCLUDE FILES
//
//****************************************************************************************
#include	<fstream>
#include	<map>
#include	<set>
#include	<string>
#include	<sstream> //Fgured I put this here just in case
#include	<vector>

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
typedef	string::size_type	StringSize; //Didn't notice this until now, forgive me

class	DocumentFile
{
public:
	DocumentFile() : pageNumber_(1), counter_(0), valueSwitch_(0){ } //Initializing done here

	void	Close();

	int	GetPageNumber();			//	Returns the current pge number.

	string	GetWord();					//	Returns the next legal word not on the exception list;
										//	returns an empty string if there are no more words in
										//	the line.

	bool	LoadExclusions(const string& name);	//	Loads a list of words to be excluded from the index
												//	from a file of the given name.

	bool	Open(const string& name);	//	Opens a document file of the given name.

	bool	Read();						//	Reads the next line of the document file, skipping over
										//	the double empty lines that mark page separations.
										//	Returns false if there are no more lines in the file.

	void	Switch(int value);

private:
	StringSize	beginPosition_; //I will admit, for some reason, I didn't ever find a use for this and forgot it entirely.

	fstream		file_, exclude_; //Exclude's for the exclusion stuff

	int		pageNumber_, valueSwitch_; //Pagenumber and switch

	string		line_, stopperLine_; //I just replaced text_ with line_, just fits better to me. Also have stopper there for use

	size_t		counter_; //Important cog

	vector<string> 	word_, illegal_; //More vectors for use
};

class	DocumentIndex
{
public:
	void	Create(DocumentFile& documentFile);	//	Creates an index for the given document file.

	void	Show(ostream& stream);

	void	Write(ostream& indexFile);	//	Writes the index to the given file.
										//	The argument is a stream so that this function
										//	can be called to wrtite its output to cout for
										//	test purposes.

private:


	vector<int>	pageNumbers; //Page numbers for each word

	map<string, 	vector<int>> indexTree; //Our map

	vector<string> 	document; //Raw words

	vector<int> 	numbers; //Raw numbers

	set<string> 	checker; //Our organized, sorted vector of likely candidates for the map
};

#endif
