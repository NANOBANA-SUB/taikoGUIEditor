#pragma once
#include <vector>
#include "Note.h"

using namespace std;

class Measure
{
public:
	vector<Note> Notes;
	double Beat;
	double BPM;
	bool isGoGo;
	bool isBarline;
	float HighSpeed;

	Measure(vector<Note> notes);
};

