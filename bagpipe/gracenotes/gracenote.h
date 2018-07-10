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
#ifndef BAGPIPE_GRACENOTE_H
#define BAGPIPE_GRACENOTE_H

#include "../definitions/definitionselementreader.h"
#include "notedefinition.h"
#include "sequence.h"
#include "duration.h"

namespace Bagpipe {
using namespace std;

class GraceNote;
using GraceNote_SPtr = shared_ptr<GraceNote>;
using GraceNote_SPtr_Vector = vector<GraceNote_SPtr>;

//---------------------------------------------------------
//   GraceNote
//---------------------------------------------------------
#pragma clang diagnostic ignored "-Wweak-vtables"
#pragma clang diagnostic ignored "-Wpadded"
class GraceNote
   {
public:
      GraceNote();
      virtual ~GraceNote() = default;

      const NoteDefinition& note() const { return *_note; }

      uint matchCount() const;

      static GraceNote_SPtr make() { return GraceNote_SPtr(new GraceNote()); }

private:
      friend class GraceNoteReader;

      Duration_SPtr _duration;
      bool _fermata = false;

      NoteDefinition_SPtr _note = nullptr;
   };

//---------------------------------------------------------
//   GraceNoteReader
//---------------------------------------------------------
class GraceNoteReader : public DefinitionsElementReader<GraceNote>
      {
public:
      GraceNoteReader(GraceNote_SPtr item, DefinitionsFileReader& fileReader);
      virtual ~GraceNoteReader() = default;

protected:
      virtual void readAttributes();
      };
} // namespace Bagpipe
#endif // BAGPIPE_GRACENOTE_H
