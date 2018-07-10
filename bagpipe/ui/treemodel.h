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
#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QStandardItemModel>
#include <QModelIndex>
#include <QTimer>
#include <QVariant>

#include "../gracenotes/category.h"

namespace Bagpipe {


class xMyModel : public QStandardItemModel
{
#pragma clang diagnostic ignored "-Wunknown-warning-option"
    Q_OBJECT
public:
    xMyModel(QObject *parent);
    //int rowCount(const QModelIndex &parent = QModelIndex()) const override ;
    //int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    //QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private slots:
    //void timerHit();

private:
    //unique_ptr<QTimer> _timer = nullptr;
};

} // namespace Bagpipe
#endif // TREEMODEL_H
