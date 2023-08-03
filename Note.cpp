#include "pch.h"
#include "Note.h"

Note::Note(Note::NoteType notetype, double position) 
{
    this->Type = notetype;
    this->Position = position;
}
