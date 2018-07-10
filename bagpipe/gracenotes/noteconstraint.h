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
#ifndef BAGPIPE_NOTECONSTRAINT_H
#define BAGPIPE_NOTECONSTRAINT_H

#include <memory>

#include "../definitions/definitionselementreader.h"
#include "duration.h"
#include "notedefinition.h"
#include "notedefinitionmap.h"

namespace Bagpipe {
using namespace std;

class NoteConstraint;
using NoteConstraint_SPtr = shared_ptr<NoteConstraint>;

enum class Beam { NotSet, Left, Right };

//---------------------------------------------------------
//   NoteConstraint
//---------------------------------------------------------
class NoteConstraint
   {
public:
      NoteConstraint(const QString& note, Beam beam, bool dotted);

      void setDuration(Duration_SPtr duration) { _duration = duration; }
      QString setNotes(const INoteDefinitionMap& noteDefinitions);

      uint matchCount() const;

      static NoteConstraint_SPtr make(const QString& note, Beam beam, bool dotted) { return NoteConstraint_SPtr(new NoteConstraint(note, beam, dotted)); }

private:
      QString _note;
      Duration_SPtr _duration;
      Beam _beam = Beam::NotSet;
      bool _dotted = false;
      bool _equal = false;

      void addNotes(NoteDefinition_SPtr first, NoteDefinition_SPtr last);

#pragma clang diagnostic ignored "-Wpadded"
      NoteDefinition_SPtr_Vector _validNotes;
   };
} // namespace Bagpipe
#endif // BAGPIPE_NOTECONSTRAINT_H
