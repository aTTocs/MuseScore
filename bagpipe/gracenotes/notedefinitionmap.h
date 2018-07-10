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
#ifndef BAGPIPE_NOTEDEFINITIONMAP_H
#define BAGPIPE_NOTEDEFINITIONMAP_H

#include <memory>

#include "notedefinition.h"

namespace Bagpipe {
using namespace std;

class INoteDefinitionMap;
using INoteDefinitionMap_UPtr = unique_ptr<INoteDefinitionMap>;

//---------------------------------------------------------
//   INoteDefinitionMap
//---------------------------------------------------------
class INoteDefinitionMap
   {
public:
      virtual ~INoteDefinitionMap() = default;

      virtual void add(NoteDefinition_SPtr noteDefinition) = 0;
      virtual NoteDefinition_SPtr get(const QString& id) const = 0;
      virtual NoteDefinition_SPtr first() const = 0;
      virtual NoteDefinition_SPtr last() const = 0;

      virtual void finalizeNotes() = 0;
   };

INoteDefinitionMap_UPtr makeNoteDefinitionMap();

} // namespace Bagpipe
#endif // BAGPIPE_NOTEDEFINITIONMAP_H
