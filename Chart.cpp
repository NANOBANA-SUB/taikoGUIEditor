#include "pch.h"
#include "Chart.h"
#include "Measure.h"
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
        OutputDebugString(_T("tjaファイルが見つかりませんでした\n"));
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

                // OutputDebugStringを用いてLEVELの値をデバッグ出力ウィンドウに出力します。
                ostringstream oss;
                oss << "LEVEL: " << LEVEL << "\n";
                OutputDebugStringA(oss.str().c_str());
            }
            else if (Trimmedline.compare(0, 3, "BPM") == 0)
            {
                string subLine = Trimmedline.substr(4);
                istringstream iss(subLine);
                iss >> BPM;

                // OutputDebugStringを用いてLEVELの値をデバッグ出力ウィンドウに出力します。
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
        OutputDebugString(_T("tjaファイルが見つかりませんでした\n"));
    }
    else
    {
        bool StartFlag = false;
        bool EndFlag = false;
        bool IsBarLine = true;
        bool IsGoGo = false;
        double HS = 1.0;
        double MeasureBPM = BPM;
        double MeasureVal = 1.0;
        double NotePos = 0.0;
        NoteType Type;
        while (getline(file, line))
        {
            string TrimmedLine = trim(line);

            if (TrimmedLine == "#START")
            {
                OutputDebugString(_T("#START\n"));
                StartFlag = true;
            }
            if (TrimmedLine == "#END")
            {
                OutputDebugString(_T("#END\n"));
                EndFlag = true;
                break;
            }
            else if (StartFlag == true && EndFlag == false)
            {
                //コマンドの処理
                if (TrimmedLine.compare(0, 1, "#") == 0)
                {
                    if (TrimmedLine.compare(0, 10, "#BPMCHANGE") == 0)
                    {
                        string subLine = TrimmedLine.substr(11);
                        istringstream iss(subLine);
                        iss >> MeasureBPM;

                        // OutputDebugStringを用いてLEVELの値をデバッグ出力ウィンドウに出力します。
                        ostringstream oss;
                        oss << "BPMCHANGE: " << MeasureBPM << "\n";
                        OutputDebugStringA(oss.str().c_str());
                    }
                    else if (TrimmedLine.compare(0, 10, "#GOGOSTART") == 0)
                    {
                        IsGoGo = true;
                    }
                    else if (TrimmedLine.compare(0, 8, "#GOGOEND") == 0)
                    {
                        IsGoGo = false;
                    }
                    else if (TrimmedLine.compare(0, 10, "#BARLINEON") == 0)
                    {
                        IsBarLine = true;
                    }
                    else if (TrimmedLine.compare(0, 11, "#BARLINEOFF") == 0)
                    {
                        IsBarLine = false;
                    }
                }
                //ノーツの処理
                else
                {
                    string fumen = TrimmedLine;
                    if (!fumen.empty() && fumen[fumen.size() - 1] == ',')
                    {
                        fumen.erase(fumen.size() - 1);
                    }
                    for (int i = 0; i < fumen.size(); i++)
                    {
                        if (fumen[i] == '0')
                        {
                            Note note = Note(NoteType::None, NotePos);
                            Notes.push_back(note);
                        }
                        else if (fumen[i] == '1')
                        {
                            Note note = Note(NoteType::Don, NotePos);
                            Notes.push_back(note);
                        }
                        else if (fumen[i] == '2')
                        {
                            Note note = Note(NoteType::Katsu, NotePos);
                            Notes.push_back(note);
                        }
                        else if (fumen[i] == '3')
                        {
                            Note note = Note(NoteType::BigDon, NotePos);
                            Notes.push_back(note);
                        }
                        else if (fumen[i] == '4')
                        {
                            Note note = Note(NoteType::BigKatsu, NotePos);
                            Notes.push_back(note);
                        }
                        NotePos += CalcNotePos(fumen, MeasureVal);
                    }

                    Measure measure = Measure(vector<Note>(Notes));

                    measure.Beat = MeasureVal;
                    measure.isGoGo = IsGoGo;
                    measure.isBarline = IsBarLine;
                    measure.HighSpeed = HS;
                    measure.BPM = MeasureBPM;

                    Measures.push_back(measure);

                    NotePos = 0;

                    Notes.clear();
                }
            }
        }
    }
    return Measures;
}

double Chart::CalcNotePos(string fumen, double beat)
{
    //fumenの各ノーツのポジションを求める
            //16分1個の位置は１小節が4/4だとすると50.１小節の長さは800.
            //つまり、800をfumenの要素数で割ればそれがポジションになる(休符も同じく)

    double MeasureLength = 800.0f;

    return beat * MeasureLength / fumen.size();
}

string trim(const string& str)
{
	string s = str;

	//コメント部分を削除
	auto comment_pos = s.find("//");
	if (comment_pos != std::string::npos)
		s.erase(comment_pos, std::string::npos);

	// 先頭の空白を削除
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
		}));

	// 末尾の空白を削除
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
		}).base(), s.end());

	return s;
}
