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
#pragma once
#ifndef BAGPIPE_MELODYNOTE_H
#define BAGPIPE_MELODYNOTE_H

#include <memory>

#include "../definitions/definitionselementreader.h"
#include "sequence.h"
#include "noteconstraint.h"

namespace Bagpipe {
using namespace std;

class MelodyNote;
using MelodyNote_SPtr = shared_ptr<MelodyNote>;
using MelodyNote_SPtr_Vector = vector<MelodyNote_SPtr>;

class GraceNote;
using GraceNote_SPtr = shared_ptr<GraceNote>;
using GraceNote_SPtr_Vector = vector<GraceNote_SPtr>;

//---------------------------------------------------------
//   MelodyNote
//---------------------------------------------------------
#pragma clang diagnostic ignored "-Wweak-vtables"
class MelodyNote
   {
public:
      virtual ~MelodyNote() = default;

      const QString& graceNoteString() const { return _graceNoteString; }

      uint matchCount() const;

      static MelodyNote_SPtr make() { return MelodyNote_SPtr(new MelodyNote()); }

private:
      friend class MelodyNoteReader;
      friend class SequenceReader;

      GraceNote_SPtr_Vector _preBeatGraceNotes;
      GraceNote_SPtr_Vector _postBeatGraceNotes;

      QString _symbol;
      QString _symbolPosition;

      QString _graceNoteString;

      NoteConstraint_SPtr _noteConstraint = nullptr;

      void setGraceNoteString();
   };

//---------------------------------------------------------
//   MelodyNoteReader
//---------------------------------------------------------
class MelodyNoteReader : public DefinitionsElementReader<MelodyNote>
      {
public:
      MelodyNoteReader(MelodyNote_SPtr item, DefinitionsFileReader& fileReader);
      virtual ~MelodyNoteReader() = default;

protected:
      virtual void readAttributes();
      };
} // namespace Bagpipe
#endif // BAGPIPE_MELODYNOTE_H
