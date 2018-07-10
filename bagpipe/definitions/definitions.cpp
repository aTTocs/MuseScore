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

#include "definitions.h"

#include "../gracenotes/embellishmentdefinitions.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   Definitions
//---------------------------------------------------------
Definitions::Definitions()
      :_noteDefinitionMap(makeNoteDefinitionMap())
      {
      _embellishments = EmbellishmentDefinitions::make();
      }
} // namespace Bagpipe
