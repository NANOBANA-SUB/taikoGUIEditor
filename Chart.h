#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Measure.h"
#include "Note.h"

using namespace std;
class Chart
{
public:
	vector<Measure> Measures;
	vector<Note> Notes;
	string TITLE;
	int LEVEL;
	double BPM;

	Chart(string filePath);

	string getTitle() const { return TITLE; }
	double getBPM() const { return BPM; }

	void ReadHeader(string filePath);
	vector<Measure> ReadMeasure(string filePath);
	double CalcNotePos(string fumen, double beat);
};

