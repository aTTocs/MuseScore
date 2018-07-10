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

#include "embellishment.h"

#include "../definitions/definitions.h"
#include "embellishmentdefinitions.h"
#include "gracenote.h"
#include "melodynote.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   displayName
//---------------------------------------------------------
const QString& Embellishment::displayName() const
      {
      if (!_displayName.isEmpty())
            return _displayName;
      else
            return _id;
      }

//---------------------------------------------------------
//   EmbellishmentReader
//---------------------------------------------------------
EmbellishmentReader::EmbellishmentReader(Embellishment_SPtr item, DefinitionsFileReader& fileReader)
      : DefinitionsElementReader(item, fileReader, Tag::Embellishment, {Tag::id, Tag::bww, Tag::display, Tag::cann}, true)
      {}

//---------------------------------------------------------
//   readAttributes
//---------------------------------------------------------
void EmbellishmentReader::readAttributes()
      {
      item()._id = attribute(Tag::id, true);
      if (fileReader().definitions().embellishments().embellishmentById(item()._id))
            elementIdAlreadyUsed(item()._id);

      item()._bww = attribute(Tag::bww, false);
      item()._displayName = attribute(Tag::display, false);
      item()._canntaireachd = attribute(Tag::cann, false);
      }

//---------------------------------------------------------
//   readElement
//---------------------------------------------------------
void EmbellishmentReader::readElement(const QStringRef& tag)
      {
      if (tag == Tag::Played) {
            Sequence_SPtr sequence(Sequence::make());
            SequenceReader reader(sequence, fileReader(), Tag::Played, item().id());
            reader.readItem();

            if (item()._played != nullptr)
                  elementDuplicated();

            if (!reader.hadErrors())
                  item()._played = sequence;
            }
      else if (tag == Tag::Abbreviated) {
            Sequence_SPtr sequence(Sequence::make());
            SequenceReader reader(sequence, fileReader(), Tag::Abbreviated, item().id());
            reader.readItem();

            if (item()._abbreviated != nullptr)
                  elementDuplicated();

            if (!reader.hadErrors())
                  item()._abbreviated = sequence;
            }
      else if (tag == Tag::Substitute) {
            const QString& id = attribute(Tag::id, true);

            item()._substitute = fileReader().definitions().embellishments().embellishmentById(id);
            if (!item()._substitute)
                  elementIdNotFound(Tag::Embellishment, id);

            readNext();
            }
      else
            elementUnknown(QString("Def id=\"%1\"").arg(item()._id));
      }

//---------------------------------------------------------
//   readCompleted
//---------------------------------------------------------
void EmbellishmentReader::readCompleted()
      {
      if (item()._played == nullptr)
            elementNotFound(Tag::Played);

      item()._matchCount = item()._played->matchCount();
      }

} // namespace Bagpipe
