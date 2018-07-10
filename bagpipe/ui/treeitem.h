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
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

#include "../gracenotes/category.h"

namespace Bagpipe {

class TreeItem
      {
public:
      explicit TreeItem(const Category& category, TreeItem* parentItem = nullptr);
      ~TreeItem();

      void appendChild(TreeItem* child);

      TreeItem* child(int row);
      int childCount() const;
      int columnCount() const;
      const Category& data(int column) const;
      int row() const;
      TreeItem* parentItem();

private:
      QList<TreeItem*> m_childItems;
      const Category& m_itemData;
      TreeItem* m_parentItem;
      };
} // namespace Bagpipe
#endif // TREEITEM_H
