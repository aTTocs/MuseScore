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

#include "category.h"

#include "../definitions/definitions.h"
#include "embellishment.h"
#include "embellishmentdefinitions.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   CategoryReader
//---------------------------------------------------------
CategoryReader::CategoryReader(Category_SPtr item, DefinitionsFileReader& fileReader)
      : DefinitionsElementReader(item, fileReader, Tag::Category, {Tag::name}, true)
      {}

//---------------------------------------------------------
//   read
//---------------------------------------------------------
void CategoryReader::readAttributes()
      {
      item()._name = attribute(Tag::name, true);

      if (fileReader().definitions().embellishments().category(item()._name))
            elementIdAlreadyUsed(item()._name);
      }

//---------------------------------------------------------
//   readElement
//---------------------------------------------------------
void CategoryReader::readElement(const QStringRef& tag)
      {
      if (tag == Tag::Embellishment) {
            Embellishment_SPtr embellishment(Embellishment::make());
            embellishment->setCategory(itemPtr());

            EmbellishmentReader reader(embellishment, fileReader());
            reader.readItem();

            if (!reader.hadErrors() && fileReader().definitions().embellishments().addEmbellishment(embellishment))
                  item()._embellishments.push_back(embellishment);
            }
      else if (tag == Tag::EmbellishmentReference) {
            auto id = attribute(Tag::id, true);

            Embellishment_SPtr embellishment = fileReader().definitions().embellishments().embellishmentById(id);

            if (!embellishment)
                  elementIdNotFound(Tag::Embellishment, id);

            if (fileReader().currentErrorCount() == 0)
                  item()._embellishments.push_back(embellishment);

            readNext();
            }
      else if (tag == Tag::Category) {
            Category_SPtr category(Category::make());
            category->setParent(itemPtr());

            CategoryReader reader(category, fileReader());
            reader.readItem();

            if (!reader.hadErrors() && fileReader().definitions().embellishments().addCategory(category))
                  item()._children.push_back(category);
            }
      else
            elementUnknown(QString("Category name=\"%1\"").arg(item()._name));
      }
} // namespace Bagpipe
