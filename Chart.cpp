#include "pch.h"
#include "Chart.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

string trim(const string& str);

Chart::Chart(string filePath)
{

}

void Chart::ReadHeader(string filePath)
{
    vector<string> lines;
    string line;

    ifstream file(filePath); // Use variable filePath, not literal string "filePath"
    if (!file)
    {
        OutputDebugString(_T("tja�t�@�C����������܂���ł���\n"));
    }
    else
    {
        while (getline(file, line))
        {
            string Trimmedline = trim(line);

            if (Trimmedline == "#START")
            {
                OutputDebugString(_T("#START\n"));
                break;
            }
            else if (Trimmedline.substr(0, 5) == "TITLE")
            {
                size_t pos = Trimmedline.find(':');
                if (pos != string::npos)
                {
                    TITLE = Trimmedline.substr(pos + 1);
                    std::string debugString = "TITLE:" + TITLE + "\n";
                    OutputDebugStringA(debugString.c_str());
                }
            }
            else if (Trimmedline.compare(0, 5, "LEVEL") == 0)
            {
                string subLine = Trimmedline.substr(6);
                istringstream iss(subLine);
                iss >> LEVEL;

                // OutputDebugString��p����LEVEL�̒l���f�o�b�O�o�̓E�B���h�E�ɏo�͂��܂��B
                ostringstream oss;
                oss << "LEVEL: " << LEVEL << "\n";
                OutputDebugStringA(oss.str().c_str());
            }
            else if (Trimmedline.compare(0, 3, "BPM") == 0)
            {
                string subLine = Trimmedline.substr(4);
                istringstream iss(subLine);
                iss >> BPM;

                // OutputDebugString��p����LEVEL�̒l���f�o�b�O�o�̓E�B���h�E�ɏo�͂��܂��B
                ostringstream oss;
                oss << "BPM: " << BPM << "\n";
                OutputDebugStringA(oss.str().c_str());
            }
        }
    }
}

vector<Measure> Chart::ReadMeasure(string filePath)
{
    string line;
    ifstream file(filePath); // Use variable filePath, not literal string "filePath"
    if (!file)
    {
        OutputDebugString(_T("tja�t�@�C����������܂���ł���\n"));
    }
    else
    {
        while (getline(file, line))
        {

        }
    }
}

string trim(const string& str)
{
	string s = str;

	//�R�����g�������폜
	auto comment_pos = s.find("//");
	if (comment_pos != std::string::npos)
		s.erase(comment_pos, std::string::npos);

	// �擪�̋󔒂��폜
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));

	// �����̋󔒂��폜
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());

	return s;
}
