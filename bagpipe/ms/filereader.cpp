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

#include "filereader.h"

#include <QMessageBox>

#include "logging.h"

namespace Ms {
using namespace std;

//---------------------------------------------------------
//   FileReader
//---------------------------------------------------------
FileReader::FileReader(QFile* file)
      : QXmlStreamReader(file)
      {
      _filename = file->fileName();

      pushErrors(); // so errorCount works if it's called before the first real push.
      }

//---------------------------------------------------------
//   attribute
//---------------------------------------------------------
QString FileReader::attribute(const QString& name, bool required)
      {
      if (!hasAttribute(name)) {
            if (required)
                  attributeNotFound(name);

            return "";
            }

      QString value = attributes().value(name).toString();
      if (value.isEmpty())
            addError(QString("%1 is empty").arg(name), true);

      return value;
      }

//---------------------------------------------------------
//   intAttribute
//---------------------------------------------------------
int FileReader::intAttribute(const QString& name, bool required)
      {
      if (hasAttribute(name))
            return attributes().value(name).toInt();

      if (required)
            attributeNotFound(name);

      return 0;
      }

//---------------------------------------------------------
//   doubleAttribute
//---------------------------------------------------------
double FileReader::doubleAttribute(const QString& name, bool required)
      {
      if (hasAttribute(name))
            return attributes().value(name).toDouble();

      if (required)
            attributeNotFound(name);

      return 0.0;
      }

//---------------------------------------------------------
//   boolAttribute
//---------------------------------------------------------
bool FileReader::boolAttribute(const QString& name, bool required)
      {
      if (hasAttribute(name)) {
            QString value = attributes().value(name).toString().toLower();

            if (value == "true")
                  return true;
            else if (value == "false")
                  return false;
            else
                  attributeValueInvalid(name, value, "true or false");

            return false;
            }

      if (required)
            attributeNotFound(name);

      return false;
      }

//---------------------------------------------------------
//   hasAttribute
//---------------------------------------------------------
bool FileReader::hasAttribute(const QString& name)
      {
      return attributes().hasAttribute(name);
      }

//---------------------------------------------------------
//   validateAttributes
//---------------------------------------------------------
void FileReader::validateAttributes(const String_Vector& names, bool isError)
      {
      foreach (const QXmlStreamAttribute& attribute, attributes())
            if (find(names.begin(), names.end(), attribute.name()) == names.end())
                  attributeUnknown(attribute.name().toString(), isError);
      }

//---------------------------------------------------------
//   attributeNotFound
//---------------------------------------------------------
void FileReader::attributeNotFound(const QString& attribute, bool isError)
      {
      addError(QString("<%1>\n\"%2\" attribute not found").arg(name().toString()).arg(attribute), isError);
      }

//---------------------------------------------------------
//   attributeUnknown
//---------------------------------------------------------
void FileReader::attributeUnknown(const QString& attribute, bool isError)
      {
      addError(QString("<%1>\nUnknown attribute %2=\"\"").arg(name().toString()).arg(attribute), isError);
      }

//---------------------------------------------------------
//   attributeValueInvalid
//---------------------------------------------------------
void FileReader::attributeValueInvalid(const QString& attribute, const QString& value, const QString& allowed, bool isError)
      {
      addError(QString("<%1 %2=\"%3\">\nInvalid value - Must be %4").arg(name().toString()).arg(attribute).arg(value).arg(allowed), isError);
      }

//---------------------------------------------------------
//   elementUnknown
//---------------------------------------------------------
void FileReader::elementUnknown(const QString& parent, bool isError)
      {
      QString id;

      if (hasAttribute("id")) id = QString(" id=\"%1\"").arg(attribute("id", false));
      if (hasAttribute("name")) id = QString(" name=\"%1\"").arg(attribute("name", false));

      addError(QString("<%1>\nUnknown element <%2%3>").arg(parent).arg(name().toString()).arg(id), isError);

      skipCurrentElement();
      }

//---------------------------------------------------------
//   elementNotFound
//---------------------------------------------------------
void FileReader::elementNotFound(const QString& element, bool isError)
      {
      addError(QString("<%1> not found").arg(element), isError);
      }

//---------------------------------------------------------
//   elementIdNotFound
//---------------------------------------------------------
void FileReader::elementIdNotFound(const QString& element, const QString& id, bool isError)
      {
      addError(QString("<%1>\n<%2 id=\"%3\"> not found").arg(name().toString()).arg(element).arg(id), isError);
      }

//---------------------------------------------------------
//   elementDuplicated
//---------------------------------------------------------
void FileReader::elementDuplicated(bool isError)
      {
      addError(QString("<%1>\nDuplicate element").arg(name().toString()), isError);
      }

//---------------------------------------------------------
//   elementIdAlreadyUsed
//---------------------------------------------------------
void FileReader::elementIdAlreadyUsed(const QString& id, bool isError)
      {
      addError(QString("<%1 id=\"%2\">\nid already used").arg(name().toString()).arg(id), isError);
      }

//---------------------------------------------------------
//   elementNameAlreadyUsed
//---------------------------------------------------------
void FileReader::elementNameAlreadyUsed(const QString& name, bool isError)
      {
      addError(QString("<%1 name=\"%2\">\nname already used").arg(this->name().toString()).arg(name), isError);
      }

//---------------------------------------------------------
//   problemFound
//---------------------------------------------------------
void FileReader::problemFound(const QString& error, bool isError)
      {
      addError(error, isError);
      }

//---------------------------------------------------------
//   addError
//---------------------------------------------------------
void FileReader::addError(const QString& error, bool isError)
      {
      QString errorString = QString("Line %1 Col %2:\n%3").arg(lineNumber()).arg(columnNumber()).arg(error);

      errorString.replace("\n", "\n          ");

      if (!loggedFirst) {
            loggedFirst = true;
            qCInfoNoFile(bagpipe) << "##########################\nError reading Bagpipe Definitions file\n" << _filename;
            }

      qCInfoNoFile(bagpipe) << errorString.toStdString().c_str();

      if (isError)
            _errors.push_back(errorString);
      }

//---------------------------------------------------------
//   push
//---------------------------------------------------------
void FileReader::pushErrors()
      {
      _errorsPosStack.push_back(_errors.size());
      }

//---------------------------------------------------------
//   pop
//---------------------------------------------------------
void FileReader::popErrors()
      {
      _errorsPosStack.pop_back();
      }

//---------------------------------------------------------
//   count
//
//   Returns: number of number of errors added since the last push
//---------------------------------------------------------
uint FileReader::currentErrorCount()
      {
      return _errors.size() - _errorsPosStack.back();
      }

//---------------------------------------------------------
//   reportErrors
//---------------------------------------------------------
void FileReader::reportErrors(const QString& title )
      {
      if (atEnd()) {
            if (hasError())
                  if (error() != Error::PrematureEndOfDocumentError)
                        addError(QString("Internal reader error: %1").arg(errorString()), true);
            }
      else
            addError("Reading was completed, but not at the end of file.", true);

      if (loggedFirst)
            qCInfoNoFile(bagpipe) << "End Bagpipe Definitions file errors\n##########################";

      if (totalErrorCount() == 0)
            return;

      QString errors(QString("Error reading %1\n").arg(_filename));

      for (auto error : _errors) {
            errors += "\n" + error;
            }

      QMessageBox::warning(nullptr, title, errors, QMessageBox::Ok, QMessageBox::NoButton);
      }
} // namespace Ms
