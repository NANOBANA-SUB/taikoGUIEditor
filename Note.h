#pragma once
enum NoteType
{
    //とりあえず休符とドンだけ
    None = 0,
    Don = 1,
    Katsu = 2,
    BigDon = 3,
    BigKatsu = 4,
};
class Note
{
public:
    NoteType Type;
    double Position;

    Note(NoteType notetype, double position);
};

