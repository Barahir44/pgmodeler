/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Core library (libpgmodeler)
# Description: Implements the operations to manipulate tables on the database.
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
#ifndef TABLE_H
#define TABLE_H

#include "basegraphicobject.h"
#include "basetable.h"
#include "column.h"
#include "constraint.h"
#include "index.h"
#include "rule.h"
#include "trigger.h"
#include "function.h"
#include "role.h"

class Table: public BaseTable {
 private:
  //Vectors that store basic table attributes
  vector<TableObject *> columns;
  vector<TableObject *> constraints;
  vector<TableObject *> indexes;
  vector<TableObject *> rules;
  vector<TableObject *> triggers;

  //Stores the tables that 'this' object inherits attributes
  vector<Table *> ancestor_tables;

  //Stores the tables that 'this' object clones the attributes
  vector<Table *> copy_tables;

  //Indicates if the table accepts OIDs
  bool with_oid;

  /* Gets one table ancestor (OBJ_TABLE) or copy (BASE_TABLE) using its name and stores
     the index of the found object on parameter 'obj_idx' */
  BaseObject *getObject(const QString &name, ObjectType obj_type, int &obj_idx);

  //The methods below generates the table attributes used by the SchemaParser
  void setColumnsAttribute(unsigned def_type);
  void setConstraintsAttribute(unsigned def_type);
  void setTriggersAttribute(unsigned def_type);
  void setIndexesAttribute(unsigned def_type);
  void setRulesAttribute(unsigned def_type);
  void setCommentAttribute(TableObject *tab_obj);

 protected:
  //Adds an ancestor table
  void addAncestorTable(Table *tab, int idx=-1);

  //Adds an copy table
  void addCopyTable(Table *tab, int idx=-1);

  //Removes an acestor table using its name
  void removeAncestorTable(const QString &name);

  //Removes an acestor table using its index
  void removeAncestorTable(unsigned idx);

  //Removes an copy table using its name
  void removeCopyTable(const QString &name);

  //Removes an copy table using its index
  void removeCopyTable(unsigned idx);

 public:
   Table(void);
  ~Table(void);

  void setName(const QString &name);
  void setSchema(BaseObject *schema);

  //Defines if the table accepts OIDs
  void setWithOIDs(bool value);

  //Adds an object to the table. It can be inserted at a specified index 'obj_idx'.
  void addObject(BaseObject *obj, int obj_idx=-1, bool copy_tab=false);

  //Gets a object from table through its index and type
  BaseObject *getObject(unsigned obj_idx, ObjectType obj_type);

  //Gets a object from table through its name and type
  BaseObject *getObject(const QString &name, ObjectType obj_type);

  //Removes a object from table through its index and type
  void removeObject(unsigned obj_idx, ObjectType obj_type);

  //Removes a object from table through its name and type
  void removeObject(const QString &name, ObjectType obj_type);

  //Removes the specified object from table
  void removeObject(BaseObject *obj);

  //Adds a column to table (optionally the user can add the object at the specified index 'idx')
  void addColumn(Column *col, int idx=-1);

  //Adds a constraint to table (optionally the user can add the object at the specified index 'idx')
  void addConstraint(Constraint *constr, int idx=-1);

  //Adds a trigger to table (optionally the user can add the object at the specified index 'idx')
  void addTrigger(Trigger *trig, int idx=-1);

  //Adds a index to table (optionally the user can add the object at the specified index 'idx')
  void addIndex(Index *ind, int idx=-1);

  //Adds a rule to table (optionally the user can add the object at the specified index 'idx')
  void addRule(Rule *reg, int idx_reg=-1);

  /* Gets a column through its name. The boolean parameter is used
     to search columns referencing their old names */
  Column *getColumn(const QString &name, bool ref_old_name=false);

  //Gets a column through its index
  Column *getColumn(unsigned idx);

  //Gets a constraint through its name.
  Constraint *getConstraint(const QString &name);

  //Gets a constraint through its index
  Constraint *getConstraint(unsigned idx);

  //Gets a trigger through its name.
  Trigger *getTrigger(const QString &name);

  //Gets a trigger through its index
  Trigger *getTrigger(unsigned idx);

  //Gets a index object through its name
  Index *getIndex(const QString &name);

  //Gets a index object through its position
  Index *getIndex(unsigned idx);

  //Gets a rule through its name
  Rule *getRule(const QString &name);

  //Gets a rule through its index
  Rule *getRule(unsigned idx);

  //Gets a ancestor table through its name
  Table *getAncestorTable(const QString &name);

  //Gets a ancestor table through its index
  Table *getAncestorTable(unsigned idx);

  //Gets a copy table through its name
  Table *getCopyTable(const QString &name);

  //Gets a copy table through its index
  Table *getCopyTable(unsigned idx);

  //Gets the column count
  unsigned getColumnCount(void);

  //Gets the constraint count
  unsigned getConstraintCount(void);

  //Gets the trigger count
  unsigned getTriggerCount(void);

  //Gets the index count
  unsigned getIndexCount(void);

  //Gets the rule count
  unsigned getRuleCount(void);

  //Gets the ancestor table count
  unsigned getAncestorTable(void);

  //Gets the copy tables count
  unsigned getCopyTable(void);

  /* Gets the the count for the specified object type. The boolean parameter indicates
     that objects added by relationship must be counted */
  unsigned getObjectCount(ObjectType obj_type, bool inc_added_by_rel=true);

  //Removes a column through its name
  void removeColumn(const QString &name);

  //Removes a column through its index
  void removeColumn(unsigned idx);

  //Removes a constraint through its name
  void removeConstraint(const QString &name);

  //Removes a constraint through its index
  void removeConstraint(unsigned idx);

  //Removes a trigger through its name
  void removeTrigger(const QString &name);

  //Removes a trigger through its index
  void removeTrigger(unsigned idx);

  //Removes a index through its name
  void removeIndex(const QString &name);

  //Removes a index through its position
  void removeIndex(unsigned idx);

  //Removes a rule through its name
  void removeRule(const QString &name);

  //Removes a rule through its index
  void removeRule(unsigned idx);

  //Returns the SQL / XML definition for table
  virtual QString getCodeDefinition(unsigned def_type);

  //Gets the object index using its name and type
  int getObjectIndex(const QString &name, ObjectType obj_type);

  //Returns the index for the specified table object
  int getObjectIndex(TableObject *obj);

  //Returns the primary key of the table. Returns NULL when it doesn't exists
  Constraint *getPrimaryKey(void);

  /* Stores on the specified vector 'fks' the foreign key present on table. The
     boolean paramenter is used to include those foreign keys includes by relationship. */
  void getForeignKeys(vector<Constraint *> &fks, bool inc_added_by_rel=false);

  //Returns if the table is configured with oids
  bool isWithOIDs(void);

  //Protects the table and its aggregated objects against modification
  void setProtected(bool value);

  /* Returns if the specified column is referenced by one of the constraints on table.
     The user must specify the constraint type */
  bool isConstraintRefColumn(Column *column, ConstraintType constr_type);

  //Swaps two objects position
  void swapObjectsIndexes(ObjectType obj_type, unsigned idx1, unsigned idx2);

  //Returns if the table references objects added by relationship
  bool isReferRelationshipAddedObject(void);

  //Copy the attributes between two tables
  void operator = (Table &tabela);

  //Returns the specified object type list
  vector<TableObject *> *getObjectList(ObjectType obj_type);

  /* Gets objects which refer to object of the parameter (directly or indirectly) and stores them in a vector.
     The 'exclusion_mode' is used to speed up the execution of the method when it is used to validate the
     deletion of the object, getting only the first reference to the object candidate for deletion.
     To get ALL references to the object must be specified as 'false' the parameter 'exclusion_mode'. */
  void getColumnReferences(Column *column, vector<TableObject *> &refs, bool exclusion_mode=false);

  /* Returns if some of the foreign keys references the specified table. This method only considers the foreign keys
     created by the user. Relationship created foreign keys are discarded from the search. */
  bool isReferTableOnForeignKey(Table *ref_tab);

  friend class Relationship;
  friend class OperationList;
};

#endif
