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

#include "definitionsreader.h"

#include "../gracenotes/embellishmentdefinitions.h"
#include "../gracenotes/notedefinition.h"
#include "definitions.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   DefinitionsReader
//---------------------------------------------------------
DefinitionsReader::DefinitionsReader(Definitions_SPtr item, DefinitionsFileReader& fileReader)
      : DefinitionsElementReader(item, fileReader, Tag::Definitions, {}, true)
      {}

//---------------------------------------------------------
//   readElement
//---------------------------------------------------------
void DefinitionsReader::readElement(const QStringRef& tag)
      {
      if (tag == Tag::Notes) {
            readNotes();
            }
      else if (tag == Tag::EmbellishmentDefinitions) {
            EmbellishmentDefinitionsReader reader(item()._embellishments, fileReader());
            reader.readItem();
            }
      else
            elementUnknown(Tag::Definitions);
      }

//---------------------------------------------------------
//   readNotes
//---------------------------------------------------------
void DefinitionsReader::readNotes()
      {
      Q_ASSERT(fileReader().name() == Tag::Notes);

      INoteDefinitionMap& noteDefinitionMap = *item()._noteDefinitionMap;

      while (readNextStartElement()) {
            const QStringRef& tag(fileReader().name());

            if (tag == Tag::NoteDefinition) {
                  NoteDefinition_SPtr noteDef(NoteDefinition::make());
                  NoteDefinitionReader reader(noteDef, fileReader());
                  reader.readItem();

                  if (noteDefinitionMap.get(noteDef->id()))
                        elementIdAlreadyUsed(noteDef->id());

                  if (!reader.hadErrors())
                        noteDefinitionMap.add(noteDef);
                  }
            else
                  elementUnknown("Notes");
            }

      noteDefinitionMap.finalizeNotes();
      }
} // namespace Bagpipe
