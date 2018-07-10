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

#include "match.h"

#include "libmscore/chord.h"
#include "libmscore/note.h"
#include "libmscore/pitchspelling.h"

#include "../definitions/definitions.h"
#include "embellishment.h"
#include "embellishmentdefinitions.h"

namespace Ms {
using namespace Bagpipe;

//---------------------------------------------------------
//   Match
//---------------------------------------------------------
Match::Match(const Definitions& definitions, Chord* prevChord, vector<Chord*> graceNotes, Chord* chord)
      : _definitions(definitions)
      , _prevChord(prevChord)
      , _graceNotes(graceNotes)
      , _chord(chord)
      {
      findMatch();
      }

//---------------------------------------------------------
//   findMatch
//---------------------------------------------------------
void Match::findMatch()
      {
      if (_graceNotes.size() == 0)
            return;

      QString graceNotesString = "";

      for (Chord* chord : _graceNotes) {
            if (!graceNotesString.isEmpty())
                  graceNotesString += " ";

            // Bagpipe embellishments should only have one note per grace note.
            Note* note = chord->notes()[0];

            int octave = (note->epitch() / 12) - 1;

            QString tpc = tpc2name(note->tpc2(), NoteSpellingType::STANDARD, NoteCaseType::AUTO, false);

            if (tpc == "G" || tpc == "A")
                  tpc = (octave == 4 ? "L" : "H") + tpc;

            graceNotesString += tpc;
            }

      if (!_definitions.embellishments().containsEmbellishmentsByNotes(graceNotesString))
            return;

      const Embellishment_SPtr_List& embellishments = _definitions.embellishments().embellishmentsByNotes(graceNotesString);
      for (auto embellishment : embellishments)
            if (matches(embellishment, graceNotesString)) {
                  _embellishment = embellishment;
                  break;
                  }
      }

//---------------------------------------------------------
//   matches
//---------------------------------------------------------
bool Match::matches(const Embellishment_SPtr& embellishment, const QString& graceNoteString)
      {
      return false;
      }
} // namespace Ms
