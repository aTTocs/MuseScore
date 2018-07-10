//=============================================================================
//  MuseScore
//  Music Composition & Notation
//  $Id:$
//
//  Copyright (C) 2018 Werner Schweer and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2
//  as published by the Free Software Foundation and appearing in
//  the file LICENSE.GPL
//=============================================================================

#include "noteconstraint.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   NoteConstraint
//---------------------------------------------------------
NoteConstraint::NoteConstraint(const QString& note, Beam beam, bool dotted)
      : _note(note)
      , _beam(beam)
      , _dotted(dotted)
      {}

//---------------------------------------------------------
//   setNotes
//---------------------------------------------------------
QString NoteConstraint::setNotes(const INoteDefinitionMap& noteDefinitions)
      {
      QString error("NoteConstraint Note Error");

      NoteDefinition_SPtr noteDef;

      QStringList list = _note.split(',', QString::SplitBehavior::SkipEmptyParts);
      for (auto item : list) {
            QString itemNote = item.trimmed();

            noteDef = noteDefinitions.get(itemNote);
            if (noteDef)
                  _validNotes.push_back(noteDef);
            else if (item == Tag::equal)
                  _equal = true;
            else {
                  itemNote = itemNote.right(itemNote.length() - 1);
                  noteDef = noteDefinitions.get(itemNote);
                  if (!noteDef)
                        error += ", <Note id=\"" + itemNote + "\" not found";
                  else if (item.startsWith("+"))
                        addNotes(noteDef->next(), noteDefinitions.last());
                  else if (item.startsWith("-"))
                        addNotes(noteDefinitions.first(), noteDef->prev());
                  else if (item.startsWith("!")) {
                        addNotes(noteDefinitions.first(), noteDef->prev());
                        addNotes(noteDef->next(), noteDefinitions.last());
                        }
                  else
                        error += ", <Note id=\"" + itemNote + "\" not found";
                  }
            }

      if (error == "NoteConstraint Note Error")
            return "";

      return error;
      }

//---------------------------------------------------------
//   matchCount
//---------------------------------------------------------
uint NoteConstraint::matchCount() const
      {
      uint matchCount = 0;

      if (!_note.isEmpty())
            matchCount++;

      if (_duration && !_duration->noteDuration().isEmpty())
            matchCount++;

      if (_beam != Beam::NotSet)
            matchCount++;

      if (_dotted)
            matchCount++;

      if (_equal)
            matchCount++;

      return matchCount;
      }

//---------------------------------------------------------
//   addNotes
//---------------------------------------------------------
void NoteConstraint::addNotes(NoteDefinition_SPtr first, NoteDefinition_SPtr last)
      {
      NoteDefinition_SPtr note = first;
      while (note) {
            _validNotes.push_back(note);

            note = note != last ? note->next() : nullptr;
            }
      }

} // namespace Bagpipe
