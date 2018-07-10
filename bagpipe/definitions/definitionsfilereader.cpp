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

#include "definitionsfilereader.h"

#include <algorithm>
#include <QQmlListProperty> // Clang
#include <QVariant> // Clang
#include <QMessageBox>

#include "libmscore/mscore.h"

#include "../gracenotes/gracenote.h"
#include "../gracenotes/melodynote.h"
#include "../ms/logging.h"
#include "definitions.h"
#include "definitionsreader.h"

namespace Bagpipe {
using namespace std;

//---------------------------------------------------------
//   DefinitionsFileReader
//---------------------------------------------------------
DefinitionsFileReader::DefinitionsFileReader(QFile* file, Definitions_SPtr definitions)
      : Ms::FileReader(file)
      , _definitions(definitions)
      {
      while (readNextStartElement()) {
            const QStringRef& tag(name());
            if (tag == Tag::Definitions) {
                  DefinitionsReader reader(definitions, *this);
                  reader.readItem();
                  }
            else
                  elementUnknown("root");
            }

      reportErrors("Bagpipe Definitions File Error");
      }

//---------------------------------------------------------
//   styleFile
//---------------------------------------------------------
QFileInfo DefinitionsFileReader::styleFile(const QString& filename)
      {
      QString path;

#if defined(Q_OS_IOS)
      path = QString("%1/%2").arg(Ms::MScore::globalShare()).arg(filename);
#elif defined(Q_OS_ANDROID)
      path = QString(":/styles/%1").arg(filename);
#else
      path = QString("%1styles/%2").arg(Ms::MScore::globalShare()).arg(filename);
#endif

      return QFileInfo(path);
      }

//---------------------------------------------------------
//   getFile
//---------------------------------------------------------
QFile* DefinitionsFileReader::getFile(const QString& filename)
      {
      QFileInfo fileInfo(filename);

      if (!fileInfo.isAbsolute())
            fileInfo = styleFile(filename);

      if (!fileInfo.exists())
            fileInfo = styleFile("bagpipeDefinitions.xml");

      if (!fileInfo.exists()) {
            QString error = QString("BagpipeDefinitions file doesn't exist: %1").arg(fileInfo.path());
            Ms::MScore::lastError = error;
            qCDebug(bagpipe) << error.toStdString().c_str();
            return nullptr;
            }

      QFile* file = new QFile(fileInfo.filePath());
      if (!file->open(QIODevice::ReadOnly)) {
            QString error = QObject::tr("Cannot open bagpipe definitions:\n%1\n%2").arg(file->fileName()).arg(file->errorString());
            Ms::MScore::lastError = error;
            qCDebug(bagpipe) << error.toStdString().c_str();
            return nullptr;
            }

      return file;
      }
} // namespace Bagpipe
