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

#include "melodynote.h"

#include <QXmlStreamReader>

#include "../definitions/definitions.h"
#include "duration.h"
#include "embellishmentdefinitions.h"
#include "gracenote.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   MelodyNoteReader
//---------------------------------------------------------
MelodyNoteReader::MelodyNoteReader(MelodyNote_SPtr item, DefinitionsFileReader& fileReader)
      : DefinitionsElementReader(item, fileReader, Tag::MelodyNote, {Tag::note, Tag::duration, Tag::beam, Tag::dotted, Tag::symbolposition, Tag::symbol}, false)
      {}

//---------------------------------------------------------
//   readAttributes
//---------------------------------------------------------
void MelodyNoteReader::readAttributes()
      {
      QString note = attribute(Tag::note, false);
      QString durationValue = attribute(Tag::duration, false);
      QString beam = attribute(Tag::beam, false).toLower();
      bool dotted = boolAttribute(Tag::dotted, false);
      item()._symbol = attribute(Tag::symbol, false);
      item()._symbolPosition = attribute(Tag::symbolposition, false).toLower();

      Beam eBeam = Beam::NotSet;
      if (beam == Tag::left)
            eBeam = Beam::Left;
      else if (beam == Tag::right)
            eBeam = Beam::Left;
      else if (!beam.isEmpty())
            attributeValueInvalid(Tag::beam, beam, Tag::left + "," + Tag::right);

      item()._noteConstraint = NoteConstraint_SPtr(NoteConstraint::make(note, eBeam, dotted));
      if (item()._noteConstraint) {
            QString result = item()._noteConstraint->setNotes(fileReader().definitions().noteDefinitionMap());
            if (!result.isEmpty())
                  problemFound(result, false);

            if (!durationValue.isEmpty()) {
                  Duration_SPtr duration = Duration::make();

                  result = duration->setValues(durationValue, fileReader().definitions().embellishments().customDurations());
                  if (!result.isEmpty())
                        problemFound(result, true);

                  item()._noteConstraint->setDuration(duration);
                  }
            }
      }

//---------------------------------------------------------
//   matchCount
//---------------------------------------------------------
uint MelodyNote::matchCount() const
      {
      uint matchCount = 0;

      if (_noteConstraint)
            matchCount = _noteConstraint->matchCount();

      for (auto graceNote : _preBeatGraceNotes)
            matchCount += graceNote->matchCount();

      for (auto graceNote : _postBeatGraceNotes)
            matchCount += graceNote->matchCount();

      return matchCount;
      }

//---------------------------------------------------------
//   setGraceNoteString
//---------------------------------------------------------
void MelodyNote::setGraceNoteString()
      {
      _graceNoteString = "";

      for (auto graceNote : _preBeatGraceNotes) {
            if (_graceNoteString.size() > 0)
                  _graceNoteString += " ";

            _graceNoteString += graceNote->note().id();
            }

      for (auto graceNote : _postBeatGraceNotes) {
            if (_graceNoteString.size() > 0)
                  _graceNoteString += " ";

            _graceNoteString += graceNote->note().id();
            }
      }

} // namespace Bagpipe
