#pragma once

class Note
{
public:
    enum NoteType
    {
        //�Ƃ肠�����x���ƃh������
        None = 0,
        Don = 1,
        Katsu = 2,
        BigDon = 3,
        BigKatsu = 4,
    };

    NoteType Type;
    double Position;

    Note(NoteType notetype, double position);
};

