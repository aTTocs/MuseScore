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

#include "treeitem.h"

#include <QStringList>

namespace Bagpipe {

//---------------------------------------------------------
//   TreeItem
//---------------------------------------------------------
TreeItem::TreeItem(const Category& category, TreeItem* parent)
      : m_itemData(category)
      {
      m_parentItem = parent;
      }

//---------------------------------------------------------
//   ~TreeItem
//---------------------------------------------------------
TreeItem::~TreeItem()
      {
      qDeleteAll(m_childItems);
      }

//---------------------------------------------------------
//   appendChild
//---------------------------------------------------------
void TreeItem::appendChild(TreeItem* item)
      {
      m_childItems.append(item);
      }

//---------------------------------------------------------
//   child
//---------------------------------------------------------
TreeItem* TreeItem::child(int row)
      {
      return m_childItems.value(row);
      }

//---------------------------------------------------------
//   childCount
//---------------------------------------------------------
int TreeItem::childCount() const
      {
      return m_childItems.count();
      }

//---------------------------------------------------------
//   columnCount
//---------------------------------------------------------
int TreeItem::columnCount() const
      {
      return 1; //m_itemData.count();
      }

//---------------------------------------------------------
//   data
//---------------------------------------------------------
const Category& TreeItem::data(int /*column*/) const
      {
      return m_itemData; //.value(column);
      }

//---------------------------------------------------------
//   parentItem
//---------------------------------------------------------
TreeItem* TreeItem::parentItem()
      {
      return m_parentItem;
      }

//---------------------------------------------------------
//   row
//---------------------------------------------------------
int TreeItem::row() const
      {
      if (m_parentItem)
            return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

      return 0;
      }
} // namespace Bagpipe
