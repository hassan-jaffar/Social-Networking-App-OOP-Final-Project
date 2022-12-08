#include <iostream>
#include <string>
#include <fstream>
using namespace std;

class Helper;
class Date;
class Object;
class User;
class Page;
class Activity;
class Comment;
class Post;
class Facebook;

class Helper
{
public:
	static int StringLength(char* str)
	{
		int count = 0;
		while (str[count] != '\0')            //stops counting when finds null
			count++;
		return count;

	}
	static void StringCopy(char*& dest, char*& src) {
		int i = 0;
		while (src[i] != '\0')              //deep copy till null is found
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';     //placing null at last index
	}
	static char* GetStringFromBuffer(char* str)
	{
		char* string1 = new char[StringLength(str) + 1];
		StringCopy(string1, str);                     //re-usability of existing functions

		return string1;
	}
	static char* StringConcatenate(char*& cstring1, char* cstring2)
	{
		int length1 = StringLength(cstring1);
		int length2 = StringLength(cstring2);

		int length = length1 + length2;
		char* newcstring = new char[length + 5];   // +4 to place null

		int counter = 0;        //index counter for the newcstring 

		for (int i = 0; i < length1; i++)   //loop for copying first string
		{
			newcstring[counter] = cstring1[i];
			counter++;
		}
		newcstring[counter++] = ' ';
		for (int i = 0; i < length2; i++)     //loop for copying second string
		{
			newcstring[counter] = cstring2[i];
			counter++;
		}

		newcstring[counter] = '\0';

		return newcstring;
	}

	static char NumtoChar(int num)
	{
		switch (num)
		{
		case 1:
			return '1';
		case 2:
			return '2';
		case 3:
			return '3';
		case 4:
			return '4';
		case 5:
			return '5';
		case 6:
			return '6';
		case 7:
			return '7';
		case 8:
			return '8';
		case 9:
			return '9';
		case 0:
			return '0';
		}
	}
};

class Date {
	friend ostream& operator<<(ostream& out, const Date& date);
	friend istream& operator>>(istream& in, Date& date);
private:
	int day;
	int month;
	int year;

public:
	static Date CurrentDate;
	Date(int d = 0, int m = 0, int y = 0)
	{
		day = d;
		month = m;
		year = y;
	}

	~Date()           //no pointer so no deletion required
	{
	
	}

	void setDay(int d)
	{
		day = d;
	}
	void setMonth(int m)
	{
		month = m;
	}
	void setYear(int y)
	{
		year = y;
	}

	int getDay()
	{
		return day;
	}
	int getMonth()
	{
		return month;
	}
	int getYear()
	{
		return year;
	}

	Date GetDate()
	{
		return CurrentDate;
	}

	bool operator==(const Date& rhs)
	{
		if (year == rhs.year)
		{
			if (month == rhs.month)
			{
				if (day == rhs.day || (day + 1) == rhs.day)
					return true;
			}
		}
		return false;
	}
};

ostream& operator<<(ostream& out, const Date& date)        //replaced the print function
{
	out << date.day << "/" << date.month << "/" << date.year << endl;
	return out;
}

istream& operator>>(istream& in, Date& date)            //replaced the input function
{
	int num;
	in >> num;
	date.setDay(num);
	in >> num;
	date.setMonth(num);
	in >> num;
	date.setYear(num);
	return in;
}

class Object                       //parent class
{
protected:              //accessible in child class but not elsewhere
	char* ID;
public:
	Object()
	{
		ID = 0;
	}
	virtual ~Object()
	{
		delete ID;
	}
	virtual char* GetID()
	{
		return 0;
	}
	virtual char* GetName()
	{
		return 0;
	}
	virtual void AddToTimeline(Post* ptr)
	{

	}
	virtual void PrintDetailView()
	{

	}
};


class Activity
{
private:
	int Type;                          //checks for type are set in view post
	char* Value;
public:
	Activity()
	{
		Type = 0;
		Value = 0;
	}
	~Activity()
	{
		delete[] Value;
	}

	int GetType()
	{
		return Type;
	}

	char* GetValue()
	{
		return Value;
	}
	void SetValues(int n, char* val)
	{
		Type = n;
		Value = Helper::GetStringFromBuffer(val);
	}
};

class Comment
{
	friend ostream& operator<<(ostream& out, const Comment& comment);
private:
	char* ID;
	char* Text;
	Object* CommentedBy;
public:
	static int totalComments;
	Comment()
	{
		ID = 0;
		Text = 0;
		CommentedBy = 0;
	}
	~Comment()
	{
		delete[] ID;
		delete[] Text;
		//wont delete commentedby. it will be deleted in their own destructors because associated
	}
	char* GetID()
	{
		return ID;
	}

	char** ReadCommentFromFile(ifstream& fin)
	{
		char temp[80];
		fin >> temp;

		ID = Helper::GetStringFromBuffer(temp);

		char** tempIDs = new char* [2];

		fin >> temp;
		tempIDs[0] = Helper::GetStringFromBuffer(temp);       //gets the post on which comment is made

		fin >> temp;
		tempIDs[1] = Helper::GetStringFromBuffer(temp);      //gets the object who made the comment

		fin.ignore();                     //ignores the character in stream and reads the whole sentence
		fin.getline(temp, 80);
		Text = Helper::GetStringFromBuffer(temp);

		return tempIDs;          //these two attributes need to be associated in facebook function
	}

	void SetCommentedBy(Object* ptr)
	{
		CommentedBy = ptr;
		cout << ptr->GetID() << " commented on this post" << endl;
	}

	void SetValues(char* text, Object* user)
	{
		totalComments++;
		ID = new char[4];
		ID[0] = 'c';
		ID[1] = Helper::NumtoChar(totalComments/10);
		ID[2] = Helper::NumtoChar(totalComments % 10);
		ID[3] = '\0';
		//ID = Helper::StringConcatenate("c", to_string(totalComments).c_str());
		cout << endl << "new comment added with the ID " << ID << endl;
		Text = Helper::GetStringFromBuffer(text);
		CommentedBy = user;
	}
};

ostream& operator<<(ostream& out, const Comment& comment)
{
	out << comment.CommentedBy->GetName() << " wrote: " << comment.Text << endl;    //used in view post of post
	return out;
}

class Post
{
private:
	char* ID;
	Activity* Act;
	char* Text;
	Date SharedDate;
	Object* SharedBy;
	Comment** Comments;
	Object** LikedBy;
	int totalComments;
	int totalLikes;
public:
	static int totalPosts;
	static int likeCounter;            //just need some static counters for extra functionalities
	Post()
	{
		ID = 0;
		Act = 0;
		Text = 0;
		SharedBy = 0;
		Comments = 0;
		LikedBy = 0;
		totalComments = 0;
		totalLikes = 0;
	}
	~Post()
	{
		delete[] ID;
		delete Act;
		delete[] Text;
		for (int i = 0; i < totalComments; i++)
			delete Comments[i];
		delete[] Comments;
		delete[] LikedBy;
	}

	char** ReadPostFromFile(ifstream& fin)
	{
		int num = 0;
		char temp[80];

		fin >> num;
		if (num == 2)            //allocates space to activity only if there exists one
			Act = new Activity;

		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);

		fin >> SharedDate;

		fin.ignore();         //ignores character in input stream
		fin.getline(temp, 80);
		Text = Helper::GetStringFromBuffer(temp);

		if (Act != 0)        //only  if there was an activity allocated on heap 
		{
			fin >> num;
			fin.ignore();
			fin.getline(temp, 80);
			Act->SetValues(num, temp);
		}

		fin >> temp;
		char** tempID = 0;
		tempID = new char* [11];          // 1 object who made the post and 10 objects who liked it
		tempID[0] = Helper::GetStringFromBuffer(temp);

		fin >> temp;
		while (temp[0] != '-')
		{
			tempID[++totalLikes] = Helper::GetStringFromBuffer(temp);      //pre increment because first index has the object who posted
			fin >> temp;
		}

		return tempID;        //cant associate in post class. that is done in facebook class which has all data
	}

	void AddComment(Comment* commentID)
	{
		if (Comments == 0)
			Comments = new Comment * [10];

		if (totalComments < 10)              //max count is 10
		{
			Comments[totalComments++] = commentID;        //association
			cout << endl << GetID() << " got a new comment: " << commentID->GetID() << endl;
		}
		else
			cout << "\nLimit of comments reached!\n\n";

	}

	void PostNewComment(const char* text, Object* user)
	{
		if (totalComments < 10)              //max count is 10
		{
			int i = totalComments++;
			Comments[i] = new Comment;
			char newtext[40];           //clash of versions. cant use const char pointer at the place of char pointer. 
			int j = 0;
			for (j = 0; text[j] != '\0'; j++)
				newtext[j] = text[j];
			newtext[j] = '\0';
			Comments[i]->SetValues(newtext, user);
		}
		else
			cout << "\nLimit of comments reached!\n\n";
	}


	void AddLike(Object* user)
	{
		if (totalLikes < 10)
		{
			LikedBy[totalLikes++] = user;
			cout << user->GetID() << " liked the post: " << ID << endl;
		}
		else
			cout << "\nLimit of likes reached!\n\n";
	}

	virtual void ViewPost(Object* ptr)
	{
		if (Act != 0)           //only if activity existed
		{
			if (ptr != 0)
				cout << "\t---" << ptr->GetName() << " is ";
			if (Act->GetType() == 1)                        //different texts for different types
				cout << " feeling ";
			else if (Act->GetType() == 2)
				cout << " thinking about ";
			else if (Act->GetType() == 3)
				cout << "  making ";
			else if (Act->GetType() == 4)
				cout << "  celebrating ";
			cout << Act->GetValue() << endl;
		}
		else
		{
			if (ptr != 0)
				cout << "\t---" << ptr->GetName() << " shared \n";
		}
		cout << "\t\t\'" << Text << "\'" << "\t\t" << SharedDate << endl;
		for (int i = 0; i < totalComments; i++)
			cout << "\t\t\t" << *(Comments[i]) << endl;         //dereferencing the comments at every index
		cout << "\t\t\t****************************************\t\n\n\n";
	}

	void PrintLikedList()
	{
		for (int i = 0; i < totalLikes; i++)
		{
			LikedBy[i]->PrintDetailView();
		}
		cout << endl << endl;
	}


	//setters
	void SetSharedBy(Object* objptr)
	{
		SharedBy = objptr;
		cout << objptr->GetID() << " shared the post: " << ID << " on " << SharedDate << endl;
	}

	void SetLikedBy(Object* objptr)
	{
		if (LikedBy == 0)
			LikedBy = new Object * [10];
		if (likeCounter < 10)
		{
			LikedBy[likeCounter++] = objptr;
			cout << objptr->GetID() << " liked the post: " << ID << endl;
		}
		else
			cout << "\nLimit of likes reached!\n\n";
	}

	void SetSharedDate(Date date)
	{
		SharedDate.setDay(date.getDay());
		SharedDate.setMonth(date.getMonth());
		SharedDate.setYear(date.getYear());
	}

	void SetText(char* text)
	{
		Text = Helper::GetStringFromBuffer(text); 
	}

	void SetID(char* id)
	{
		ID = Helper::GetStringFromBuffer(id);
	}

	virtual void SetValues(int id, Date date, const char* text, Post* ptr)
	{

	}


	//gettters
	int GetTotalLikes()
	{
		return totalLikes;
	}

	Object* GetSharedBy()
	{
		return SharedBy;
	}

	char* GetText()
	{
		return Text;
	}

	char* GetID()
	{
		return ID;
	}

	Date GetSharedDate()
	{
		return SharedDate;
	}
};

class Memory : public Post
{
private:
	Post* ActualPost;
public:
	Memory()
	{
		ActualPost = 0;
	}
	~Memory()     //does not need to delete actual post because it is associated with posts in timeline
	{

	}
	void ViewPost(Object* ptr)
	{
		cout << "\t~~~" << ptr->GetName() << " shared a memory~~~" << endl;
		cout << "\t\t" << GetText() << "\t" << GetSharedDate() << "\n\t";
		cout << "\t\t\t\t(" << GetSharedDate().getYear() - ActualPost->GetSharedDate().getYear() << " Years Ago)\n\n\t";
		ActualPost->ViewPost(ptr);
	}

	void SetValues(Date date, const char* text, Post* ptr)
	{
		ActualPost = ptr;
		char temp[80];
		int i = 0;
		int id = ++(Post::totalPosts);
		temp[0] = 'p';
		temp[1] = 'o';
		temp[2] = 's';
		temp[3] = 't';
		temp[4] = Helper::NumtoChar(id / 10);
		temp[5] = Helper::NumtoChar(id % 10);
		temp[6] = '\0';
		SetID(temp);
		SetSharedDate(date);
		for (i = 0; text[i] != '\0'; i++)
			temp[i] = text[i];
		temp[i] = '\0';
		SetText(temp);
		SetSharedBy(ActualPost->GetSharedBy());
	}

	
};


class Page : public Object
{
private:
	char* Title;
	Post** Timeline;
	int totalPosts;
public:
	static int totalPages;       //static data
	Page()
	{
		ID = Title = 0;
		Timeline = 0;
		totalPosts = 0;
	}
	~Page()
	{
		//ID deleted in parent class destructor
		delete[] Title;
		for (int i = 0; i < totalPosts; i++)
			delete Timeline[i];
		delete[] Timeline;
	}

	void ReadPageFromFile(ifstream& fin)
	{
		char temp[50] = { '\0' };
		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);
		fin.ignore();                           //ignore the characters in the stream
		fin.getline(temp, 50);                  //reads full name including the spaces
		Title = Helper::GetStringFromBuffer(temp);
	}

	void PrintDetailView()
	{
		cout << ID << " - " << Title << endl;
	}

	void AddToTimeline(Post* ptr)
	{
		if (Timeline == 0)              //allocates new space if this is the first post on timeline
			Timeline = new Post * [10];
		if (totalPosts < 10)
		{
			Timeline[totalPosts++] = ptr;
			cout << ptr->GetID() << " added to the timeline of " << GetName() << endl;
		}
		else
			cout << "Limit of posts on timeline reached\n";
	}

	void ViewPostsOnHome(Date date)
	{
		for (int i = 0; i < totalPosts; i++)
		{
			if (Timeline[i]->GetSharedDate() == date)
				Timeline[i]->ViewPost(this);
		}
	}

	void ViewTimeline()
	{
		cout << "\n----------------------------------------------------------------------------------------------------\n\n";
		cout << GetName() << " - Timeline" << endl << endl;
		for (int i = 0; i < totalPosts; i++)
		{
			Timeline[i]->ViewPost(this);
		}
	}

	//Getters
	char* GetID()
	{
		return ID;
	}

	char* GetName()
	{
		return Title;
	}
};

class User : public Object
{
private:
	char* firstName;
	char* lastName;
	User** friendList;
	Page** likedPages;
	int totalFriends;
	int totalLikedPages;
	int totalPosts;
	Post** Timeline;
public:
	static int totalUsers;      //static data members
	static int friendCounter;

	//difference between totalFriends and friendCounter: totalFriends is diffferent for every user and it is incremented and set
	//during the reading from file which creates a temp friendlist. friendCounter is just a helper counter common for all users
	//it is only used  to associate friends for every user and saves totalFriends for the last user that was associated only

	User()
	{
		ID = firstName = lastName = 0;              //initialization
		friendList = 0;
		likedPages = 0;
		totalFriends = 0;
		totalLikedPages = 0;
		totalPosts = 0;
		Timeline = 0;
	}
	~User()
	{
		delete[] firstName;
		delete[] lastName;
		delete[] friendList;
		delete[] likedPages;
		for (int i = 0; i < totalPosts; i++)
			delete Timeline[i];
		delete[] Timeline;
	}

	char** ReadUserFromFile(ifstream& fin)
	{
		likedPages = new Page * [10];            //hardcoding maximum count 10
		friendList = new User * [10];

		char temp[30] = { '\0' };

		fin >> temp;
		ID = Helper::GetStringFromBuffer(temp);

		fin >> temp;
		firstName = Helper::GetStringFromBuffer(temp);

		fin >> temp;
		lastName = Helper::GetStringFromBuffer(temp);

		fin >> temp;

		char** tempFriends = 0;           //this temp friends array is created in this scope and is returned to load users function
								//its purpose is to store temporarily list of friends and associate them after all users are read

		if (temp[0] != '-')           //checks if first index is minus sign because only possible minus is -1 that symbols end
		{
			tempFriends = new char* [10];           //not doing dynamic growth of the array
			while (temp[0] != '-')
			{
				tempFriends[totalFriends++] = Helper::GetStringFromBuffer(temp);
				fin >> temp;
			}
		}
		return tempFriends;
	}

	//Getters
	char* GetID()
	{
		return ID;
	}

	char* GetName()
	{              //returns concatenated name
		char* fullName = Helper::StringConcatenate(firstName, lastName);
		return fullName;
	}

	int GetTotalFriends()
	{
		return totalFriends;
	}

	int GetTotalLikedPages()
	{
		return totalLikedPages;
	}



	void LikePage(Page* ptr)
	{
		if (totalLikedPages < 10)          //hardcoded max count 10
		{
			likedPages[totalLikedPages++] = ptr;                //association
			cout << endl << GetName() << " liked the page " << ptr->GetName() << endl;
		}
		else
			cout << "\nLimit of liked pages reached!\n\n";
	}

	void AddFriend(User* ptr)
	{
		if (friendCounter < 10)              //max count is 10
		{
			friendList[friendCounter++] = ptr;        //association
			cout << endl << GetName() << " added a new friend: " << ptr->GetName() << endl;
		}
		else
			cout << "\nLimit of friends reached!\n\n";
	}

	void ViewFriendList()
	{
		cout << endl << GetName() << " Friend List:\n";
		if (totalFriends == 0)
			cout << "\nThe friend list is empty\n\n";
		else
		{
			for (int i = 0; i < totalFriends; i++)
			{
				friendList[i]->PrintDetailView();       //modular approach
			}
		}
	}

	void PrintDetailView()
	{
		cout << ID << " - " << firstName << " " << lastName << endl;
	}

	void ViewLikedPages()
	{
		cout << endl << GetName() << " Liked Pages List:\n";
		if (totalLikedPages == 0)
			cout << "\nNo liked pages\n\n";
		else
		{
			for (int i = 0; i < totalLikedPages; i++)
			{
				likedPages[i]->PrintDetailView();       //modular approach
			}
		}
	}

	void AddToTimeline(Post* ptr)            //needed when reading and associating data
	{
		if (Timeline == 0)
			Timeline = new Post*[10];
		if (totalPosts < 10)
		{
			Timeline[totalPosts++] = ptr;
			cout << ptr->GetID() << " added to the timeline of " << GetName() << endl;
		}
		else
			cout << "Limit of posts on timeline reached\n";
	}

	void ViewHome(Date date)     //displays all posts of current and previous day by all friends and likedpages
	{
		cout << "\n----------------------------------------------------------------------------------------------------\n\n";
		cout << GetName() << " - Homepage" << endl << endl;
		for (int i = 0; i < totalFriends; i++)
		{
			friendList[i]->ViewPostsOnHome(date);
		}
		for (int i = 0; i < totalLikedPages; i++)
		{
			likedPages[i]->ViewPostsOnHome(date);
		}
	}

	void SeeMemories(Date date)       //shows all the posts on user's timeline of that specific day
	{
		cout << "\n----------------------------------------------------------------------------------------------------\n\n";
		cout << "On this Day:\n";
		for (int i = 0; i < totalPosts; i++)
		{
			Date temp = Timeline[i]->GetSharedDate();     //copy constructor not needed because no pointer
			if (temp.getDay() == date.getDay() && temp.getMonth() == date.getMonth() && temp.getYear() != date.getYear())
			{
				cout << date.getYear() - temp.getYear() << " Years Ago \n";
				Timeline[i]->ViewPost(this);
			}
		}
	}

	void ShareMemory(const char* ID, const char* text, Date date)      //mushkilest function :(
	{
		if (totalPosts < 10)
		{
			Post* ptr = SearchForPost(ID);
			if (ptr != 0)
			{
				int i = 0;
				for (i = 0; i <= totalPosts; i++)
				{
					Timeline[totalPosts - i + 1] = Timeline[totalPosts - i];
				}
				totalPosts++;
				Memory* temp = new Memory;
				temp->SetValues(date, text, ptr);
				Timeline[0] = temp;
			}
			else
				cout << "Post not avaliable on " << GetName() << "'s Timeline\n";
		}
		else
			cout << "Limit of posts on timeline reached\n";
	}

	Post* SearchForPost(const char* ID)          //searches the index on timeline where the post is present
	{
		int i = 0, j = 0;
		char* temp;
		bool resp = false;
		while ((i < totalPosts) && (resp == false))
		{
			temp = Helper::GetStringFromBuffer(Timeline[i]->GetID());
			int end = Helper::StringLength(temp);
			resp = true;
			for (j = 0; j < end; j++)
			{
				if (temp[j] != ID[j])
				{
					resp = false;
					break;
				}
			}
			if (resp == true)
				return Timeline[i];
			i++;
		}
		return 0;
	}

	void ViewPostsOnHome(Date date)        //prints both posts and memories on x user's home
	{
		for (int i = 0; i < totalPosts; i++)
		{
			if (Timeline[i]->GetSharedDate() == date)
				Timeline[i]->ViewPost(this);
		}
	}

	void ViewTimeline()            //simple printing
	{
		cout << "\n----------------------------------------------------------------------------------------------------\n\n";
		cout << GetName() << " - Timeline" << endl << endl;
		for (int i = 0; i < totalPosts; i++)
		{
			Timeline[i]->ViewPost(this);
		}
	}
};

class Facebook            //singleton. controlling class
{
private:
	Page** Pages;
	User** Users;
	Post** Posts;
	Comment** Comments;
public:
	Facebook()
	{
		Pages = 0;
		Users = 0;
		Posts = 0;
		Comments = 0;
	}
	~Facebook()
	{
		//in-line deallocations
		for (int i = 0; i < Page::totalPages; i++)
			delete Pages[i];
		delete[] Pages;
		for (int i = 0; i < User::totalUsers; i++)
			delete Users[i];
		delete[] Users;

		delete[] Posts;

		delete[] Comments;
	}

	void LoadData(const char* pagesFile, const char* usersFile, const char* postsFile, const char* commentsFile)
	{
		LoadPages(pagesFile);        //seperate functions for all files
		LoadUsers(usersFile);
		LoadPosts(postsFile);
		LoadComments(commentsFile);
	}

	void LoadPages(const char* pagesFile)
	{
		ifstream fin;
		fin.open(pagesFile);
		if (fin.is_open())
		{
			fin >> Page::totalPages;             //reading static data
			Pages = new Page * [Page::totalPages];
			for (int i = 0; i < Page::totalPages; i++)
			{
				Pages[i] = new Page;
				Pages[i]->ReadPageFromFile(fin);        //initializes and reads pages for every ith index
			}
		}
		else
			cout << "\nError opening file\n\n";

		fin.close();
	}

	void LoadUsers(const char* usersFile)
	{
		char*** TempFriendIDs = 0; //triple pointer. its every ith index gets a double pointer from the read function of users class
								  //that double pointer points to an array of friends ID which are stored in it temporarily
								 //these will be copied in ith users friendlist once all the users are read
								//it is a pointer pointing to pointers that point to list of temporary friendlist

		ifstream fin;
		fin.open(usersFile);
		if (fin.is_open())
		{
			fin >> User::totalUsers;
			Users = new User * [User::totalUsers];

			TempFriendIDs = new char** [User::totalUsers];    //creates double pointers for every user

			for (int i = 0; i < User::totalUsers; i++)
			{
				Users[i] = new User;
				TempFriendIDs[i] = Users[i]->ReadUserFromFile(fin);  //gets temp friendlist for every user

				char temp[20];
				//char** tempPages = 0;
				fin >> temp;

				//reading and associating liked pages together

				//Associating liked pages here
				if (temp[0] != '-')
				{
					while (temp[0] != '-')
					{
						Page* ptr = SearchPageByID(temp);//Facebook's Search Page (this->SearchPageByID)
						if (ptr != 0)
							Users[i]->LikePage(ptr);
						else
							cout << "\nPage not found!\n\n";

						fin >> temp;
					}
				}
				cout << "\n----------------------------------------------------------------------------------------------------\n";
				
			}
			//ASSOCIATE FRIENDS	
			for (int i = 0; i < User::totalUsers; i++)
			{
				int endpoint = Users[i]->GetTotalFriends();       //gets total num of friends for every user for the loop to end
				User::friendCounter = 0;          //makes it zero for every ith user
				for (int j = 0; j < endpoint; j++)
				{
					User* ptr = SearchUserByID(TempFriendIDs[i][j]);     //gets pointer that points to the address of that user
					if (ptr != 0)
						Users[i]->AddFriend(ptr);
					else
						cout << "\nUser not found!\n\n";
				}
				cout << "\n----------------------------------------------------------------------------------------------------\n";
			}

			//deallocating temp friend ids
			for (int i = 0; i < User::totalUsers; i++)
			{
				int end = Users[i]->GetTotalFriends();
				for (int j = 0; j < end; j++)
					delete TempFriendIDs[i][j];

				delete TempFriendIDs[i];
			}
			delete[] TempFriendIDs;
		}
		else
			cout << "\nError opening file\n\n";

		fin.close();
	}

	void LoadPosts(const char* postsFile)
	{
		char*** tempObjIDs = 0;
		ifstream fin;
		fin.open(postsFile);
		if (fin.is_open())
		{
			fin >> Post::totalPosts;             //reading static data
			Posts = new Post * [Post::totalPosts];
			tempObjIDs = new char** [Post::totalPosts];
			for (int i = 0; i < Post::totalPosts; i++)
			{
				Posts[i] = new Post;
				tempObjIDs[i] = Posts[i]->ReadPostFromFile(fin);        //initializes and reads posts for every ith index
			}

			for (int i = 0; i < Post::totalPosts; i++)
			{
				//associate shared by object here
				Object* ObjPtr = SearchObjectByID(tempObjIDs[i][0]);
				if (ObjPtr != 0)
				{
					Posts[i]->SetSharedBy(ObjPtr);
					ObjPtr->AddToTimeline(Posts[i]);
				}
				else
					cout << "Object not found" << endl;
				int endpoint = Posts[i]->GetTotalLikes();       //gets total num of likes for every post for the loop to end
				Post::likeCounter = 0;          //makes it zero for every ith user
				for (int j = 1; j <= endpoint; j++)
				{
					//tempObjIDs association
					//first index contained the object shared by
					//rest is the array of like by objects
					Object* ObjPtr = SearchObjectByID(tempObjIDs[i][j]);
					if (ObjPtr != 0)
						Posts[i]->SetLikedBy(ObjPtr);
					else
						cout << "Object not found" << endl;
				}
				cout << "\n----------------------------------------------------------------------------------------------------\n";
			}

			//deallocating temp obj ids
			for (int i = 0; i < Post::totalPosts; i++)
			{
				int end = Posts[i]->GetTotalLikes();
				for (int j = 0; j <= end; j++)
					delete tempObjIDs[i][j];

				delete tempObjIDs[i];
			}
			delete[] tempObjIDs;
		}
		else
			cout << "\nError opening file\n\n";

		fin.close();
	}

	void LoadComments(const char* commentsFile)
	{
		char** tempIDs = 0;
		ifstream fin;
		fin.open(commentsFile);
		if (fin.is_open())
		{
			fin >> Comment::totalComments;
			Comments = new Comment * [Comment::totalComments];
			for (int i = 0; i < Comment::totalComments; i++)
			{
				Comments[i] = new Comment;
				tempIDs = Comments[i]->ReadCommentFromFile(fin);


				//tempIDs[0] contained the ID of post on which comment is made
				Post* ptr = SearchPostByID(tempIDs[0]);
				if (ptr != 0)
					ptr->AddComment(Comments[i]);
				else
					cout << "Post not found\n";

				//tempIDs[1] contains the ID of the object who made the comment
				//we need to associate it to page or user 
				//polymorphism

				Object* objptr = SearchObjectByID(tempIDs[1]);
				if (objptr != 0)
					Comments[i]->SetCommentedBy(objptr);
				else
					cout << "Object not found\n";
				cout << "\n----------------------------------------------------------------------------------------------------\n";
			}

			//deallocating temp objects
			for (int i = 0; i < 2; i++)
				delete tempIDs[i];
			delete tempIDs;

		}
		else
			cout << "\nError opening file\n\n";

		fin.close();
	}


	//search functions could have been made using templates because same functionality for all functions except class type

	Object* SearchObjectByID(const char* ID)
	{
		int index = 0;
		int endpoint = 0;
		bool found = false;
		if (ID[0] == 'p')
		{
			return SearchPageByID(ID);
		}
		else if (ID[0] == 'u')
		{
			return SearchUserByID(ID);
		}
		else
			return 0;        //null pointer if ID does not exist
	}

	Page* SearchPageByID(const char* pageID)
	{
		int index = 0;
		bool found = false;
		while (found == false && index < Page::totalPages)    //checks if key not found already and index within size of list
		{
			for (int j = 0; (pageID[j] != '\0'); j++)
			{
				found = true;
				if (Pages[index]->GetID()[j] != pageID[j])    //checks every index of key and list's value's roll no
				{
					found = false;
					break;           //breaks the checking even if one mismatch found. avoids extra processing
				}
			}
			if (found == true)       //returns the index where key matches
				return Pages[index];
			index++;
		}
		return 0;    //null pointer if pageID does not exist
	}

	User* SearchUserByID(const char* userID)
	{
		int index = 0;
		bool found = false;
		while (found == false && index < User::totalUsers)    //checks if key not found already and index within size of list
		{
			for (int j = 0; (userID[j] != '\0'); j++)
			{
				found = true;
				if (Users[index]->GetID()[j] != userID[j])    //checks every index of key and list's value's roll no
				{
					found = false;
					break;           //breaks the checking even if one mismatch found. avoids extra processing
				}
			}
			if (found == true)       //returns the index where key matches
				return Users[index];
			index++;
		}
		return 0; //null pointer if the userUD does not exist
	}

	Post* SearchPostByID(const char* postID)
	{
		int index = 0;
		bool found = false;
		while (found == false && index < Post::totalPosts)    //checks if key not found already and index within size of list
		{
			for (int j = 0; (postID[j] != '\0'); j++)
			{
				found = true;
				if (Posts[index]->GetID()[j] != postID[j])    //checks every index of key and list's value's roll no
				{
					found = false;
					break;           //breaks the checking even if one mismatch found. avoids extra processing
				}
			}
			if (found == true)       //returns the index where key matches
				return Posts[index];
			index++;
		}
		return 0; //null pointer if the postUD does not exist
	}

	Comment* SearchCommentByID(const char* commentID)
	{
		int index = 0;
		bool found = false;
		while (found == false && index < Comment::totalComments)    //checks if key not found already and index within size of list
		{
			for (int j = 0; (commentID[j] != '\0'); j++)
			{
				found = true;
				if (Comments[index]->GetID()[j] != commentID[j])    //checks every index of key and list's value's roll no
				{
					found = false;
					break;           //breaks the checking even if one mismatch found. avoids extra processing
				}
			}
			if (found == true)       //returns the index where key matches
				return Comments[index];
			index++;
		}
		return 0; //null pointer if the commentUD does not exist
	}



	User* SetCurrentUser(const char* userID)
	{
		cout << "\n----------------------------------------------------------------------------------------------------\n";
		User* ptr = SearchUserByID(userID);
		if (ptr != 0)
		{
			cout << ptr->GetName() << " successfully set as current user\n";
			return ptr;       //returns address of that particular user 
		}
		else
			cout << "\nUser not found!\n\n";

		return 0;
	}

	void SetCurrentDate(int d, int m, int y)
	{
		Date::CurrentDate.setDay(d);
		Date::CurrentDate.setMonth(m);
		Date::CurrentDate.setYear(y);
	}

	void ViewLikedList(const char* ID)
	{
		cout << "\n----------------------------------------------------------------------------------------------------\n";
		cout << "Post Liked By: \n";
		Post* ptr = SearchPostByID(ID);
		ptr->PrintLikedList();
	}

	void LikePost(const char* ID, Object* user)
	{
		Post* ptr = SearchPostByID(ID);
		ptr->AddLike(user);
	}

	void PostComment(const char* ID, const char* text, Object* user)
	{
		Post* ptr = SearchPostByID(ID);
		ptr->PostNewComment(text, user);
	}

	void ViewPost(const char* ID)
	{
		cout << "\n----------------------------------------------------------------------------------------------------\n";
		Post* ptr = SearchPostByID(ID);
		ptr->ViewPost(ptr->GetSharedBy());
	}

	void ViewPage(const char* ID)
	{
		cout << "\n----------------------------------------------------------------------------------------------------\n";
		Page* ptr = SearchPageByID(ID);
		ptr->ViewTimeline();
	}
};

//global initialization of static variables
int Page::totalPages = 0;
int User::totalUsers = 0;
int User::friendCounter = 0;
int Post::totalPosts = 0;
int Post::likeCounter = 0;
int Comment::totalComments = 0;
Date Date::CurrentDate;

void main()
{
	cout << "\n\tREADING AND ASSOCIATING DATA\n\n";
	Facebook fb;
	fb.LoadData("Pages.txt", "Users.txt", "Posts.txt", "Comments.txt");

	cout << "\n\n\n\t\t\tRUNNING COMMANDS\n\n";
	fb.SetCurrentDate(15, 11, 2017);
	cout << "System Date: \t" << Date::CurrentDate.GetDate() << endl;

	User* currentUser = fb.SetCurrentUser("u7");

	if (currentUser != 0)
	{
		currentUser->ViewFriendList();

		currentUser->ViewLikedPages();

		currentUser->ViewHome(Date::CurrentDate);

		currentUser->ViewTimeline();

		fb.ViewLikedList("post5");

		fb.LikePost("post5", currentUser);

		fb.ViewLikedList("post5");

		fb.PostComment("post4", "Good Luck for your Result", currentUser);

		fb.ViewPost("post4");

		fb.PostComment("post8", "Thanks for the wishes", currentUser);

		fb.ViewPost("post8");

		currentUser->SeeMemories(Date::CurrentDate);

		currentUser->ShareMemory("post10", "Never thought I will be specialist in this field...", Date::CurrentDate);

		currentUser->ViewTimeline();

		fb.ViewPage("p1");
	}
	else
		cout << "\nUser not found\n\n";

	currentUser = fb.SetCurrentUser("u11");
	if (currentUser != 0)
	{
		currentUser->ViewHome(Date::CurrentDate);

		currentUser->ViewTimeline();
	}
	else
		cout << "\nUser not found\n\n";

	cout << User::totalUsers << endl << Post::totalPosts << endl << Page::totalPages << endl << Comment::totalComments << endl;
 }