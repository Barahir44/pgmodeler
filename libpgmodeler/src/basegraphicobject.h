/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Class: BaseGraphicObject
# Description: This is a basic implementation to control graphical objects such as
#              tables, relationships, textboxes, views.
#              The full graphical representation of these objects are made in an separated layer
#              this is treated in the GraphicObject class on libobjrenderer library
# Creation date: 17/09/2006
#
# Copyright 2006-2013 - Raphael Araújo e Silva <rkhaotix@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/
#ifndef BASE_GRAPHIC_OBJECT_H
#define BASE_GRAPHIC_OBJECT_H

#include <QObject>
#include <QPointF>
#include "baseobject.h"

class BaseGraphicObject: public QObject, public BaseObject {
 private:
  Q_OBJECT

  /* Stores the position of the object on the model. This attribute is used only
     to memorize the position of the object in xml code generation */
  QPointF position;

  /* Indicates that the object structure was modified somehow
     and it is needed to be updated or specially treated */
  bool is_modified;

  /* Stores a reference to the object which is currently the receiver
     of signals emitted by the instance of this class. The receiver is an object that
     represents the 'this' object in the QGraphicsScene. This attribute breaks
     some concepts of OO but is required for some cases when is necessary to
     recover the graphical object in a fast way (rather than find
     it in the QGraphcisScene) */
  QObject *receiver_object;

 protected:
  /* Method that defines the objects position attributes used in generation
     of XML code definition */
  void setPositionAttribute(void);

  //Defines the receveir objects that represents the 'this' object on the QGraphicsScene
  void setReceiverObject(QObject *obj);

 public:
  BaseGraphicObject(void);
  ~BaseGraphicObject(void){}

  /* Sets whether the object is protected or not (method overloading
     from base class BaseObject) the difference is that this method
     emits the signal s_objectProtected() */
  virtual void setProtected(bool value);

  //Sets the object's position
  void setPosition(QPointF pos);

  /* Sets if the objects is modified or not.
     This method emits the signal s_objectModified() */
  virtual void setModified(bool value);

  //Returns the modified status of the object
  bool isModified(void);

  //Returns the current position of the object
  QPointF getPosition();

  //Assigns on object to other mading the correct attribute copy
  void operator = (BaseGraphicObject &obj);

  //Gets the current receiver object that graphically represents the 'this' object
  QObject *getReceiverObject(void);

  //Returns the code definition of the object
  virtual QString getCodeDefinition(unsigned tipo_def)=0;

 signals:
  //Signal emitted when the user calls the setModified() method
  void s_objectModified(void);
  //Signal emitted when the user calls the setProtected() method
  void s_objectProtected(bool);

  friend class CenaObjetos;
  friend class BaseObjectView;
  friend class DatabaseModel;
};

#endif
