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

#include "sequence.h"

#include <QXmlStreamReader>

#include "../definitions/definitions.h"
#include "embellishmentdefinitions.h"
#include "gracenote.h"
#include "melodynote.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   matchCount
//---------------------------------------------------------
uint Sequence::matchCount() const
      {
      uint matchCount = 0;

      for (auto melodyNote : _melodyNotes)
            matchCount += melodyNote->matchCount();

      if (_triplet)
            matchCount++;

      return matchCount;
      }

//---------------------------------------------------------
//   SequenceReader::SequenceReader
//---------------------------------------------------------
SequenceReader::SequenceReader(Sequence_SPtr item, DefinitionsFileReader& fileReader, QString itemTag, const QString& defId)
      : DefinitionsElementReader(item, fileReader, itemTag, {Tag::bww, Tag::triplet}, true)
      , _defId(defId)
      {}

SequenceReader::SequenceReader(Sequence_SPtr item, DefinitionsFileReader& fileReader, QString itemTag)
      : SequenceReader(item, fileReader, itemTag, "")
      {}

//---------------------------------------------------------
//   readAttributes
//---------------------------------------------------------
void SequenceReader::readAttributes()
      {
      item()._bww = attribute(Tag::bww, false);
      item()._triplet = boolAttribute(Tag::triplet, false);
      }

//---------------------------------------------------------
//   readElement
//---------------------------------------------------------
void SequenceReader::readElement(const QStringRef& tag)
      {
      if (tag == Tag::GraceNote) {
            if (!_currentMelodyNote)
                  _currentMelodyNote = MelodyNote::make();

            GraceNote_SPtr graceNote(GraceNote::make());

            GraceNoteReader reader(graceNote, fileReader());
            reader.readItem();

            if (!reader.hadErrors()) {
                  if (_preBeat) {
                        _currentMelodyNote->_preBeatGraceNotes.push_back(graceNote);
                        }
                  else {
                        _currentMelodyNote->_postBeatGraceNotes.push_back(graceNote);
                        }
                  }
            }
      else if (tag == Tag::MelodyNote) {
            if (!_currentMelodyNote)
                  _currentMelodyNote = MelodyNote::make();

            MelodyNoteReader reader(_currentMelodyNote, fileReader());
            reader.readItem();

            if (!reader.hadErrors())
                  item()._melodyNotes.push_back(_currentMelodyNote);

            _currentMelodyNote = nullptr;
            }
      else if (tag == Tag::Beat) {
            _preBeat = false;

            validateAttributes({});

            readNext();
            }
      else
            elementUnknown(QString("Def id=\"%1\"").arg(_defId));
      }

//---------------------------------------------------------
//   readCompleted
//---------------------------------------------------------
void SequenceReader::readCompleted()
      {
      if (_currentMelodyNote) {
            item()._melodyNotes.push_back(_currentMelodyNote);

            _currentMelodyNote = nullptr;
            }

      for (auto melodyNote : item()._melodyNotes)
            melodyNote->setGraceNoteString();
      }
} // namespace Bagpipe
