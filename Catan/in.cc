#include "in.h"
#include <sstream>
#include <vector>
#include <string>
using namespace std;

istream* getRandLayout(const int& seed){
	srand(seed);// Set seed.
	string s;
	vector <int> v;

	for (int i = 2;i < 13;++i){// Get number.
		if (i == 2 || i == 7 || i == 12){// When there is only one.
			v.emplace_back(i);
		} else {// There are two same numbers.
			v.emplace_back(i);
			v.emplace_back(i);
		}
	}

	for (int i = 0; i < 19; ++i){// Randomlized.
		int j = (rand()%19);
		swap(v[j],v[i]);
	}

    // Put all the number in a string, spreated by spaces.
	for (int i = 0; i < 19; ++i){
		if (i != 0) s += " ";
		if (v[i] == 7){
			s += "5 ";
		}else {
			int tmp = (rand() % 5);
			s += to_string(tmp);
			s += " ";
		}
		s += to_string(v[i]);
	}

	istream* in = new istringstream(s);
	return in;
}

// This is random layout, set the seed to time(0), others are the same.
istream* getRealRandLayout(){
	srand((unsigned)time(0)); 
	string s;
	vector <int> v;

	for (int i = 2;i < 13;++i){
		if (i == 2 || i == 7 || i == 12){
			v.emplace_back(i);
		} else {
			v.emplace_back(i);
			v.emplace_back(i);
		}
	}

	for (int i = 0; i < 19; ++i){
		int j = (rand()%19);
		swap(v[j],v[i]);
	}

	for (int i = 0; i < 19; ++i){
		if (i != 0) s += " ";
		if (v[i] == 7){
			s += "5 ";
		}else {
			int tmp = (rand() % 5);
			s += to_string(tmp);
			s += " ";
		}
		s += to_string(v[i]);
	}

	istream* in = new istringstream(s);
	return in;
}
