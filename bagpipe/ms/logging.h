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
#ifndef LOGGING_H
#define LOGGING_H

#include <QLoggingCategory>

#define qCInfoNoFile(category, ...) \
    for (bool qt_category_enabled = category().isInfoEnabled(); qt_category_enabled; qt_category_enabled = false) \
        QMessageLogger("", 0, "", category().categoryName()).info(__VA_ARGS__)

Q_DECLARE_LOGGING_CATEGORY(bagpipe)

#endif // LOGGING_H
