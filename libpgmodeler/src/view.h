/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Implements the operations to manipulate views on database.
# Creation date: 07/04/2008
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
#ifndef VIEW_H
#define VIEW_H

#include "reference.h"
#include "basetable.h"

class View: public BaseTable {
 private:
  //Stores the references to expressions and objects
  vector<Reference> references;

  /* Vectors that stores indexes to the view references in each
     SQL part: SELECT-FROM, FROM-WHERE and after WHERE */
  vector<unsigned> exp_select,
                   exp_from,
                   exp_where;

  //Sets the declaration attribute used by the SchemaParser
  void setDeclarationAttribute(void);

  //Sets the references attribute used by the SchemaParser
  void setReferencesAttribute(void);

  /* Returns the reference index on the view. When it doesn't exists,
     the method returns -1 */
  int getReferenceIndex(Reference &refer);

  //Returns the reference to internal expression list according to the SQL expression type
  vector<unsigned> *getExpressionList(unsigned sql_type);

 public:
  View(void);

  /* Adds a reference to the view specifying the SQL expression type for it
     (refer to class Reference::SQL_??? constants). The 'expr_id' parameter is the
     index where the reference must be inserted. By defaul the method always adds
     new references at the end of the list */
  void addReference(Reference &refer, unsigned sql_type, int expr_id=-1);

  /* Remove the reference from the view using its index, removing all the elements
     from the exp_??? vectors when they make use of the deleted reference. */
  void removeReference(unsigned ref_id);

  //Removes all the references from the view
  void removeReferences(void);

  //Removes an element from the expression list specified by the 'sql_type' parameter
  void removeReference(unsigned expr_id, unsigned sql_type);

  //Returns the reference count from view
  unsigned getReferenceCount(void);

  /* Returns the element count on the specified SQL expression type list (sql_type).
     It possible to filter the reference type via 'ref_type' which must be filled
     with the Reference::REFER_??? constants */
  unsigned getReferenceCount(unsigned sql_type, int ref_type=-1);

  //Returs one reference using its index
  Reference getReference(unsigned ref_id);

  /* Retuns one reference in the specified position (ref_id) on the
     specified expression list (sql_type) */
  Reference getReference(unsigned ref_id, unsigned sql_type);

  //Returns the specified reference index on the specified expression list
  int getReferenceIndex(Reference &ref, unsigned sql_type);

  //Returns the SQL / XML definition for the view
  QString getCodeDefinition(unsigned def_type);

  /* Returns whether the view references columns added
     by relationship. This method is used as auxiliary
     to control which view reference columns added by the
     relationship in order to avoid referece breaking due constants
     connections and disconnections of relationships */
  bool isReferRelationshipAddedColumn(void);

  //Returns if the view is referencing the specified table
  bool isReferencingTable(Table *tab);

  //Returns if the view is referencing the specified column
  bool isReferencingColumn(Column *col);

  //Copy the attributes between two views
  void operator = (View &visao);
};

#endif
