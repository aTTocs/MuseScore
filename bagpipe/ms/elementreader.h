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
#ifndef ELEMENTREADER_H
#define ELEMENTREADER_H

#include <memory>
#include <initializer_list>

#include "filereader.h"

namespace Ms {

//---------------------------------------------------------
//   ElementReader
//---------------------------------------------------------
template<class TItem, class TFileReader = FileReader>
class ElementReader
   {
public:
      // Returns true after read() is done, if errors occured during read()
      bool hadErrors() const {return _hadErrors;}

      void readItem()
            {
            Q_ASSERT(_fileReader.name() == _itemTag);
            _fileReader.pushErrors();
            _fileReader.validateAttributes(_attributes);
            if (_attributes.size() > 0)
                  readAttributes();

            if (_containsElements)
                  while (readNextStartElement())
                        readElement(_fileReader.name());
            else
                  _fileReader.readNext();

            readCompleted();
            _hadErrors = _fileReader.currentErrorCount() != 0;
            _fileReader.popErrors();
            }

protected:
      virtual void readAttributes() {}
      virtual void readElement(const QStringRef& /*tag*/) {}
      virtual void readCompleted() {}

      ElementReader(shared_ptr<TItem> item, TFileReader& fileReader, QString itemTag, initializer_list<const QString> attributes, bool containsElements)
            : _item(item), _fileReader(fileReader), _itemTag(itemTag), _containsElements(containsElements)
            {
            foreach (auto att, attributes)
                  _attributes.push_back(att);
            }

      TItem& item() {return *_item;}
      shared_ptr<TItem>& itemPtr() {return _item;}
      TFileReader& fileReader() {return _fileReader;}

      // DefinitionsReader mirror functions
      QString attribute(const QString& name, bool required) {return _fileReader.attribute(name, required);}
      int intAttribute(const QString& name, bool required) {return _fileReader.intAttribute(name, required);}
      double doubleAttribute(const QString& name, bool required) {return _fileReader.doubleAttribute(name, required);}
      bool boolAttribute(const QString& name, bool required) {return _fileReader.boolAttribute(name, required);}

      void validateAttributes(const String_Vector& names, bool isError = true) {_fileReader.validateAttributes(names, isError);}
      void attributeNotFound(const QString& attribute, bool isError = true) {_fileReader.attributeNotFound(attribute, isError);}
      void attributeUnknown(const QString& attribute, bool isError = true) {_fileReader.attributeUnknown(attribute, isError);}
      void attributeValueInvalid(const QString& attribute, const QString& value, const QString& allowed, bool isError = true) {_fileReader.attributeValueInvalid(attribute, value, allowed, isError);}
      void elementUnknown(const QString& parent, bool isError = true) {_fileReader.elementUnknown(parent, isError);}
      void elementNotFound(const QString& element, bool isError = true) {_fileReader.elementNotFound(element, isError);}
      void elementIdNotFound(const QString& element, const QString& id, bool isError = true) {_fileReader.elementIdNotFound(element, id, isError);}
      void elementDuplicated(bool isError = true) {_fileReader.elementDuplicated(isError);}
      void elementIdAlreadyUsed(const QString& id, bool isError = true) {_fileReader.elementIdAlreadyUsed(id, isError);}
      void elementNameAlreadyUsed(const QString& name, bool isError = true) {_fileReader.elementNameAlreadyUsed(name, isError);}
      void problemFound(const QString& error, bool isError = true) {_fileReader.problemFound(error, isError);}

      // returns true if errors occurred during the read
      bool hasErrors() {return _fileReader.currentErrorCount() != 0;}

      // XmlReader mirror functions
      bool readNextStartElement() {return _fileReader.readNextStartElement();}
      QXmlStreamReader::TokenType readNext() {return _fileReader.readNext();}
      QString readElementText() {return _fileReader.readElementText();}

private:
      shared_ptr<TItem> _item;
      TFileReader& _fileReader;
      QString _itemTag;
      String_Vector _attributes;
      bool _containsElements = false;

      bool _hadErrors = false;
   };
} // namespace Ms
#endif // ELEMENTREADER_H
