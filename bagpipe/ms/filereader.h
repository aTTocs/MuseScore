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
#ifndef FILEREADER_H
#define FILEREADER_H

#include <QXmlStreamReader>
#include <QFile>

namespace Ms {
using namespace std;

using String_Vector = vector<QString>;

//---------------------------------------------------------
//   FileReader
//---------------------------------------------------------
class FileReader : public QXmlStreamReader
   {
public:
      FileReader(QFile* file);
      ~FileReader() { popErrors(); }

      QString attribute(const QString& name, bool required);
      int intAttribute(const QString& name, bool required);
      double doubleAttribute(const QString& name, bool required);
      bool boolAttribute(const QString& name, bool required);
      bool hasAttribute(const QString& name);

      void validateAttributes(const String_Vector& names, bool isError = true);
      void attributeNotFound(const QString& attribute, bool isError = true);
      void attributeUnknown(const QString& attribute, bool isError = true);
      void attributeValueInvalid(const QString& attribute, const QString& value, const QString& allowed, bool isError = true);
      void elementUnknown(const QString& parent, bool isError = true);
      void elementNotFound(const QString& element, bool isError = true);
      void elementIdNotFound(const QString& element, const QString& id, bool isError = true);
      void elementDuplicated(bool isError = true);
      void elementIdAlreadyUsed(const QString& id, bool isError = true);
      void elementNameAlreadyUsed(const QString& name, bool isError = true);
      void problemFound(const QString& error, bool isError = true);

      void pushErrors();
      void popErrors();
      uint totalErrorCount() {return _errors.size();}
      uint currentErrorCount();

      void reportErrors(const QString& title);

private:
      void addError(const QString& error, bool isError);

      QString _filename;
      vector<QString> _errors;
      vector<uint> _errorsPosStack;
      int _currentErrorCount = 0;
      bool loggedFirst = false;
   };
} // namespace Ms
#endif // FILEREADER_H
