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

#include "notedefinitionmap.h"

#include <map>

#include "notedefinition.h"

namespace Bagpipe {
using namespace std;

using NoteDefinition_SPtr_StringMap = map<QString, NoteDefinition_SPtr>;
using NoteDefinition_SPtr_IntMap = map<int, NoteDefinition_SPtr>;

class NoteDefinitionMap : public INoteDefinitionMap
   {
public:
      NoteDefinitionMap() = default;
      virtual ~NoteDefinitionMap() = default;

      virtual void add(NoteDefinition_SPtr noteDefinition);
      virtual NoteDefinition_SPtr get(const QString& id) const;
      virtual NoteDefinition_SPtr first() const { return s_noteMidiMap.begin()->second; }
      virtual NoteDefinition_SPtr last() const { return s_noteMidiMap.rbegin()->second; }

      virtual void finalizeNotes();

private:
      NoteDefinition_SPtr_StringMap s_noteNameMap;
      NoteDefinition_SPtr_IntMap s_noteMidiMap;
   };

INoteDefinitionMap_UPtr makeNoteDefinitionMap() { return INoteDefinitionMap_UPtr(new NoteDefinitionMap()); }

void NoteDefinitionMap::add(NoteDefinition_SPtr noteDefinition)
      {
      s_noteNameMap.emplace(noteDefinition->id(), noteDefinition);
      }

//---------------------------------------------------------
//   get
//---------------------------------------------------------
NoteDefinition_SPtr NoteDefinitionMap::get(const QString& id) const
      {
      auto iter = s_noteNameMap.find(id);
      if (iter != s_noteNameMap.end())
            return iter->second;

      return nullptr;
      }

//---------------------------------------------------------
//   finalizeNotes
//---------------------------------------------------------
void NoteDefinitionMap::finalizeNotes()
      {
      // Set prev/next
      NoteDefinition_SPtr note;

      for (auto const &iter : s_noteNameMap) {
            note = iter.second;
            s_noteMidiMap.emplace(note->midi(), note);
            }

      NoteDefinition_SPtr prev = nullptr;

      for (auto const &iter : s_noteMidiMap) {
            note = iter.second;

            if (prev) {
                  prev->_next = note;
                  note->_prev = prev;
                  }
            else {
                  note->_prev = note; // self
                  }

            prev = note;
            }

      //prev is now the last one
      prev->_next = prev;
      }
} // namespace Bagpipe
