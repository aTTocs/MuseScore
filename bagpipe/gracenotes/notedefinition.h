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
#ifndef BAGPIPE_NOTEDEFINITION_H
#define BAGPIPE_NOTEDEFINITION_H

#include <memory>
#include <vector>
#include <QString>

#include "../definitions/definitionselementreader.h"

namespace Bagpipe {
using namespace std;

class NoteDefinition;
using NoteDefinition_SPtr = shared_ptr<NoteDefinition>;
using NoteDefinition_WPtr = weak_ptr<NoteDefinition>;
using NoteDefinition_SPtr_Vector = vector<NoteDefinition_SPtr>;

//---------------------------------------------------------
//   NoteDefinition
//---------------------------------------------------------
class NoteDefinition
   {
public:
      NoteDefinition() = default;

      const QString& id() const { return _id; }
      int midi() const { return _midi; }
      const QString& canntaireachd() const { return _canntaireachd; }

      const NoteDefinition_SPtr prev() { NoteDefinition_SPtr temp = _prev.lock(); return temp.get() == this ? nullptr : temp; }
      const NoteDefinition_SPtr next() { NoteDefinition_SPtr temp = _next.lock(); return temp.get() == this ? nullptr : temp; }

      static NoteDefinition_SPtr make() { return NoteDefinition_SPtr(new NoteDefinition()); }

private:
      friend class NoteDefinitionReader;
      friend class NoteDefinitionMap;

      NoteDefinition_WPtr _prev;
      NoteDefinition_WPtr _next;

      QString _id;
      int     _midi = 0;
      QString _canntaireachd;
   };

//NoteDefinition_SPtr makeNoteDefinition() { return NoteDefinition_SPtr(new NoteDefinition()); }

//---------------------------------------------------------
//   NoteDefinitionReader
//---------------------------------------------------------
class NoteDefinitionReader : public DefinitionsElementReader<NoteDefinition>
      {
public:
      NoteDefinitionReader(NoteDefinition_SPtr item, DefinitionsFileReader& fileReader);
      virtual ~NoteDefinitionReader() = default;

protected:
      virtual void readAttributes();
      };
} // namespace Bagpipe
#endif // BAGPIPE_NOTEDEFINITION_H
