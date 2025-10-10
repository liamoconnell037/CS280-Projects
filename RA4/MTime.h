#ifndef Time_H
#define Time_H

#include <iostream>

using namespace std;

//Time class representing military time 24-hours format only
class MTime 
{
	private:
	int hours;
	int minutes;
	int seconds;
	//bool standard;
	
	public:
	
	MTime(int hrs = 0, int min = 0, int sec = 0);
	
	void SetTime(int hrs = 0, int min = 0, int sec = 0 );
					
	//Display Time object in 24-hours format
	void DisplayTime();
	
	int getHours() const {
		return hours;
	};
	int getMinutes() const {
		return minutes;
	};
	int getSeconds() const {
		return seconds;
	};
	
	//Overloaded operator functions for comparisons between Time objects
	bool operator==(MTime & OtherTime); 
	bool operator>(MTime & OtherTime);
	
	friend ostream & operator<<(ostream & out, const MTime &timeObj);
	
};


#endif
