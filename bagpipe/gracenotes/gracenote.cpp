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

#include "gracenote.h"

#include <set>
#include <QXmlStreamReader>

#include "../definitions/definitions.h"
#include "embellishmentdefinitions.h"
#include "notedefinition.h"

namespace Bagpipe {
using namespace std;


//---------------------------------------------------------
//   GraceNote
//---------------------------------------------------------
GraceNote::GraceNote()
      {
      _duration = Duration::make("1/32");
      }

//---------------------------------------------------------
//   matchCount
//---------------------------------------------------------
uint GraceNote::matchCount() const
      {
      uint matchCount = 1; // for required _note

      if (_duration && !_duration->noteDuration().isEmpty())
            matchCount++;

      if (_fermata)
            matchCount++;

      return matchCount;
      }

//---------------------------------------------------------
//   GraceNoteReader
//---------------------------------------------------------
GraceNoteReader::GraceNoteReader(GraceNote_SPtr item, DefinitionsFileReader& fileReader)
      : DefinitionsElementReader(item, fileReader, Tag::GraceNote, {Tag::note, Tag::duration, Tag::fermata}, false)
      {}

//---------------------------------------------------------
//   readAttributes
//---------------------------------------------------------
void GraceNoteReader::readAttributes()
      {      
      QString note = attribute(Tag::note, true).toUpper();
      QString durationValue = attribute(Tag::duration, false);
      item()._fermata = boolAttribute(Tag::fermata, false);

      if (!durationValue.isEmpty()) {
            Duration_SPtr duration = Duration::make();

            QString result = duration->setValues(durationValue, fileReader().definitions().embellishments().customDurations());
            if (!result.isEmpty())
                  problemFound(result, true);

            item()._duration = duration;
            }

      item()._note = fileReader().definitions().noteDefinitionMap().get(note);

      if (!item()._note)
            elementIdNotFound(Tag::NoteDefinition, note);
      }
} // namespace Bagpipe
