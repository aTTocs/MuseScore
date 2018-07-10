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
#ifndef BAGPIPE_CATEGORY_H
#define BAGPIPE_CATEGORY_H

#include "../definitions/definitionselementreader.h"
#include "duration.h"
#include "embellishment.h"

namespace Bagpipe {
using namespace std;

class Category;
using Category_SPtr = shared_ptr<Category>;
using Category_WPtr = weak_ptr<Category>;
using Category_SPtr_Vector = vector<Category_SPtr>;
using Category_SPtr_Map = map<QString, Category_SPtr>;

//---------------------------------------------------------
//   Category
//---------------------------------------------------------
class Category
      {
public:
      Category() = default;

      const QString& name() const { return _name; }

      const Category_WPtr& parent() const { return _parent; }
      void setParent(Category_WPtr parent) {_parent = parent;}

      const Category_SPtr_Vector& categories() const { return _children; }

      static Category_SPtr make() { return Category_SPtr(new Category()); }

private:
      friend class CategoryReader;
      friend class EmbellishmentDefinitions;
      friend class EmbellishmentDefinitionsReader;

      QString _name;
      Category_WPtr _parent;
      Category_SPtr_Vector _children;
      Embellishment_SPtr_Vector _embellishments;
   };

//---------------------------------------------------------
//   CategoryReader
//---------------------------------------------------------
class CategoryReader : public DefinitionsElementReader<Category>
      {
public:
      CategoryReader(Category_SPtr item, DefinitionsFileReader& fileReader);
      virtual ~CategoryReader() = default;

protected:
      virtual void readAttributes();
      virtual void readElement(const QStringRef& tag);
      };
} // namespace Bagpipe
#endif // BAGPIPE_CATEGORY_H
