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

#include "notedefinition.h"

#include <map>

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   NoteDefinitionReader
//---------------------------------------------------------
NoteDefinitionReader::NoteDefinitionReader(NoteDefinition_SPtr item, DefinitionsFileReader& fileReader)
      : DefinitionsElementReader(item, fileReader, Tag::NoteDefinition, {Tag::id, Tag::midi, Tag::cann}, false)
      {}

//---------------------------------------------------------
//   readAttributes
//---------------------------------------------------------
void NoteDefinitionReader::readAttributes()
      {
      item()._id = attribute(Tag::id, true).toUpper();
      item()._midi = intAttribute(Tag::midi, true);
      item()._canntaireachd = attribute(Tag::cann, false);

      if (item()._id.isEmpty())
            problemFound("id is empty");

      if (item()._midi <= 0)
            problemFound("midi is less than zero.");
      }
} // namespace Bagpipe
