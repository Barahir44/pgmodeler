/*
# PostgreSQL Database Modeler (pgModeler)
# Sub-project: Parsers library (libparsers)
# Description: ParserEsquema class definition used to create SQL definition
#              for model objects from schemas files (stored on disk).
# Creation date: 19/06/2008
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
#ifndef SCHEMA_PARSER_H
#define SCHEMA_PARSER_H

#include "globalattributes.h"
#include "exception.h"
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <QDir>

class SchemaParser {
 private:
  /* Indicates that the parser should ignore unknown
     attributes avoiding raising exceptions */
  static bool ignore_unk_atribs;

  static const char  CHR_COMMENT,      //Character that starts a comment
                     CHR_LINE_END,     //Character that indicates end of line
                     CHR_SPACE,        //Character that indicates spacing
                     CHR_TABULATION,   //Character that indicates tabulation
                     CHR_INI_ATTRIB,   //Character that indicates a reference to an attribute
                     CHR_MID_ATTRIB,   //Character that delimits on the left the attribute name
                     CHR_END_ATTRIB,   //Character that delimits on the right the attribute name
                     CHR_INI_CONDITIONAL,//Character that starts a conditional instruction
                     CHR_INI_METACHAR,   //Character that starts a metacharacter
                     CHR_INI_PURETEXT,   //Character that starts a puretext
                     CHR_END_PURETEXT;   //Character that ends a puretext

  //Tokens related to conditional instructions
  static const QString TOKEN_IF,  //%if
                       TOKEN_THEN,//%then
                       TOKEN_ELSE,//%else
                       TOKEN_END; //%end

  //Tokens related to metacharacters
  static const QString TOKEN_META_SP,//$sp (space)
                       TOKEN_META_BR,//$br (line break)
                       TOKEN_META_TB;//$tb (tabulation)


  //Loads a schema file and inserts its line in a buffer
  static void loadFile(const QString &file);

  //Get an attribute name from the buffer on the current position
  static QString getAttribute(void);

  //Get an conditional instruction from the buffer on the current position
  static QString getConditional(void);

  //Get an metacharacter from the buffer on the current position
  static QString getMetaCharacter(void);

  /* Get an word from the buffer on the current position (word is any string that isn't
     a conditional instruction or comment) */
  static QString getWord(void);

  //Gets a pure text, ignoring elements of the language
  static QString getPureText(void);

  /* Returns whether a character is special i.e. indicators of attributes
     or conditional instructions */
  static bool isSpecialCharacter(char chr);

  //Filename that was loaded by the parser
  static QString filename;

  /* Vectorial representation of the loaded file. This is the buffer that is
     analyzed by de parser */
  static vector<QString> buffer;

  static unsigned line, //Current line where the parser reading is
                  column, //Current column where the parser reading is

                  /* Comment line ammout extracted. This attribute is used to make the correct
                     reference to the line on file that has syntax errors */
                  comment_count;

  //PostgreSQL version currently used by the parser
  static QString pgsql_version;

 public:

  /* Constants used to refernce PostgreSQL versions. Created for convenience only
     because the versions will be detected by the presence of directories versions
     within the schemas directory */
  static const QString  PGSQL_VERSION_80,
                        PGSQL_VERSION_81,
                        PGSQL_VERSION_82,
                        PGSQL_VERSION_83,
                        PGSQL_VERSION_84,
                        PGSQL_VERSION_90,
                        PGSQL_VERSION_91;

  //Constants used to get a specific object definition
  static const unsigned SQL_DEFINITION=0,
                        XML_DEFINITION=1;

  /* Set the version of PostgreSQL to be adopted by the parser in obtaining
     the definition of the objects. This function should always be called at
     software startup or when the user wants to change the default version
     of the database */
  static void setPgSQLVersion(const QString &db_version);

  /* Returns the complete xml/sql definition for an database object represented by the
     map 'attributes'. For SQL definition is necessary to indicate the version of PostgreSQL
     in order to the to correct schema be loaded */
  static QString getCodeDefinition(const QString &obj_name, map<QString, QString> &attributes, unsigned def_type);

  /* Generic method that loads a schema file and for a given map of attributes
     this method returns the data of the file analyzed and filled with the values ​​of the
     attributes map */
  static QString getCodeDefinition(const QString &filename, map<QString, QString> &attributes);

  /* Returns the PostgreSQL version available based on the subdirectories on schema/sql.
     To be recognized as a version, the directory name must follow the rule: [NUMBER].[NUMBER](.[NUMBER])*/
  static void getPgSQLVersions(vector<QString> &versions);

  //Retorns the current PostgreSQL version used by the parser
  static QString getPgSQLVersion(void);

  //Resets the parser in order to do new analysis
  static void restartParser(void);

  //Set if the parser must ignore unknown attributes avoiding expcetion throwing
  static void setIgnoreUnkownAttributes(bool ignore);
};

#endif
