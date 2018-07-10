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
#ifndef BAGPIPE_MATCH_H
#define BAGPIPE_MATCH_H

#include "../definitions/definitions.h"
#include "embellishmentdefinitions.h"

namespace Ms {
using namespace Bagpipe;

class Chord;

//---------------------------------------------------------
//   Match
//---------------------------------------------------------
class Match
      {
public:
      Match(const Definitions& definitions, Chord* prevChord, vector<Chord*> graceNotes, Chord* chord);

      Embellishment_SPtr match() { return _embellishment; }

private:
      const Definitions& _definitions;
      Chord* _prevChord;
      const vector<Chord*>& _graceNotes;
      Chord* _chord;

      Embellishment_SPtr _embellishment;

      void findMatch();
      bool matches(const Embellishment_SPtr& embellishment, const QString& graceNoteString);
      };
} // namespace Ms
#endif // BAGPIPE_MATCH_H
