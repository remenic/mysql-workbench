/* 
 * Copyright (c) 2008, 2014, Oracle and/or its affiliates. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; version 2 of the
 * License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include <map>
#include <memory>

#include "grtpp_module_cpp.h"
#include "cppdbc.h"

#include "grts/structs.db.mgmt.h"

#define DOC_DbMySQLQueryImpl \
"Query execution and utility routines for  MySQL servers.\n"\
"\n"\
"This module provides a series of routines for executing queries and other\n"\
"convenience functions on a live MySQL server.\n"\
"Most functions take a connection id as the first parameter, which identifies\n"\
"a connection previously opened with the openConnection() function."

class DbMySQLQueryImpl : public grt::ModuleImplBase
{
public:
  DbMySQLQueryImpl(grt::CPPModuleLoader *loader) 
  : grt::ModuleImplBase(loader),
  _last_error_code(0), _connection_id(0), _resultset_id(0), _tunnel_id(0)
  {}

  virtual ~DbMySQLQueryImpl()
  {}

  DEFINE_INIT_MODULE_DOC("1.0", "Oracle", DOC_DbMySQLQueryImpl, grt::ModuleImplBase,
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::openConnection,
                                              "Open a connection to the MySQL server, using the given connection info object.\n"
                                              "Returns a connection-id value that can be used in the other functions in the module or -1 on error. See lastError() for the exact error.\n"
                                              "Connections must be closed with closeConnection() after use.",
                                              "info the connection information object for the MySQL instance to connect to"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::openConnectionP,
                                               "Open a connection to the MySQL server, using the given connection info object and password.\n"
                                               "Returns a connection-id value that can be used in the other functions in the module or -1 on error. See lastError() for the exact error.\n"
                                               "Connections must be closed with closeConnection() after use.",
                                               "info the connection information object for the MySQL instance to connect to\n"
                                               "password the password for the account used by the connection"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::closeConnection,
                                              "Closes a MySQL server connection opened by openConnection()",
                                              "conn_id the connection id"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::lastError,
                                              "Return the MySQL error message generated by the last executed command, if any. Use only when opening connections.",
                                              ""),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::lastErrorCode,
                                              "Return the MySQL error code from the last executed command, if any. Use only when opening connections.",
                                              ""),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::lastConnectionError,
                                              "Return the MySQL error message generated by the last executed command for a connection, if any.",
                                              ""),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::lastConnectionErrorCode,
                                              "Return the MySQL error code from the last executed command for a connection, if any.",
                                              ""),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::lastUpdateCount,
                                              "Return the number of affected rows of the last executed statement.",
                                              ""),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::execute,
                                              "Executes a statement in the server. For queries generating a resultset, see executeQuery()\n"
                                              "Returns -1 on error or >= 0 on success.",
                                              "conn_id the connection id\n"
                                              "query the statement/query to be executed"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::executeQuery,
                                              "Executes a query in the server, such as SELECT statements.\n"
                                              "Returns the result_id of the generated resultset. Use the result* functions to get the results.\n"
                                              "You must call closeResult() on the returned id once done with it.",
                                              "conn_id the connection id\n"
                                              "query the statement/query to be executed"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultNumRows,
                                              "Gets the number of rows returned by the last executeQuery() call.",
                                              "result_id the resultset identifier, returned by executeQuery()"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultNumFields,
                                              "Gets the number of fields in the resultset from the last executeQuery() call.",
                                              "result_id the resultset identifier, returned by executeQuery()"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFieldType,
                                              "Gets the datatype name of a field from the resultset.",
                                              "result_id the resultset identifier, returned by executeQuery()\n"
                                              "field the index of the resultset field"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFieldName,
                                              "Gets the name of a field from the resultset.",
                                              "result_id the resultset identifier, returned by executeQuery()\n"
                                              "field the index of the resultset field"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFirstRow,
                                              "Rewinds resultset pointer to 1st row. Returns whether there are any rows in resultset.\n",
                                              "result_id the resultset identifier, returned by executeQuery()"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultNextRow,
                                              "Checks whether there's more rows in the resultset and advances it to the next one, if possible.\n"
                                              "Sample usage:\n"
                                              "    while DbMySQLQuery.resultNextRow(res):\n"
                                              "        print DbMySQLQuery.resultFieldIntValue(res, 0)",
                                              "result_id the resultset identifier, returned by executeQuery()"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFieldIntValue,
                                              "Returns the integer value in the given field of the resultset.",
                                              "result_id the resultset identifier, returned by executeQuery()\n"
                                              "field the index of the resultset field"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFieldDoubleValue,
                                              "Returns the double (floating point) value in the given field of the resultset.",
                                              "result_id the resultset identifier, returned by executeQuery()\n"
                                              "field the index of the resultset field"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFieldStringValue,
                                              "Returns the string value in the given field of the resultset.",
                                              "result_id the resultset identifier, returned by executeQuery()\n"
                                              "field the index of the resultset field"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFieldIntValueByName,
                                              "Returns the integer value in the given field of the resultset.",
                                              "result_id the resultset identifier, returned by executeQuery()\n"
                                              "name the name of the resultset field"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFieldDoubleValueByName,
                                              "Returns the double (floating point) value in the given field of the resultset.",
                                              "result_id the resultset identifier, returned by executeQuery()\n"
                                              "name the name of the resultset field"),                         
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::resultFieldStringValueByName,
                                              "Returns the string value in the given field of the resultset.",
                                              "result_id the resultset identifier, returned by executeQuery()\n"
                                              "name the name of the resultset field"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::closeResult,
                                              "Closes the resultset freeing associated resources.",
                                              "result_id the resultset identifier, returned by executeQuery()"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::loadSchemata,
                                              "Deprecated.",
                                              ""),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::loadSchemaObjects,
                                              "Deprecated.",
                                              ""),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::loadSchemaList,
                                              "Utility function to get the full list of schemas.",
                                              "conn_id the connection id"),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::loadSchemaObjectList,
                                              "Utility function to get the list of objects in the given schema, of the given type.",
                                              "conn_id the connection id\n"
                                              "schema name of the schema from where to get the list of objects\n"
                                              "object_type type of objects to fetch. One of: table, view, routine, trigger. Passing an empty string will fetch everything."),
                  DECLARE_MODULE_FUNCTION(DbMySQLQueryImpl::generateDdlScript),
                  DECLARE_MODULE_FUNCTION(DbMySQLQueryImpl::openTunnel),
                  DECLARE_MODULE_FUNCTION(DbMySQLQueryImpl::getTunnelPort),
                  DECLARE_MODULE_FUNCTION(DbMySQLQueryImpl::closeTunnel),
                  DECLARE_MODULE_FUNCTION_DOC(DbMySQLQueryImpl::getServerVariables,
                                              "Utility function to return a dictionary containing name/value pairs for the server variables, as returned by SHOW VARIABLES.",
                                              "conn_id the connection id"),
                  NULL);

  // returns connection-id or -1 for error
  int openConnection(const db_mgmt_ConnectionRef &info);
  int openConnectionP(const db_mgmt_ConnectionRef &info, const grt::StringRef &password);

  int closeConnection(int conn);

  std::string lastError();
  int lastErrorCode();
  
  int lastConnectionErrorCode(int conn);
  std::string lastConnectionError(int conn);
  uint64_t lastUpdateCount(int conn);

  // returns 1/0 for ok, -1 for error
  int execute(int conn, const std::string &query);

  // returns result-id or -1 for error
  int executeQuery(int conn, const std::string &query);

  size_t resultNumRows(int result);
  int resultNumFields(int result);
  std::string resultFieldType(int result, int field);
  std::string resultFieldName(int result, int field);
  // returns 1 if ok, 0 if no more rows
  int resultFirstRow(int result);
  int resultNextRow(int result);
  grt::IntegerRef resultFieldIntValue(int result, int field);
  double resultFieldDoubleValue(int result, int field);
  grt::StringRef resultFieldStringValue(int result, int field);
  
  grt::IntegerRef resultFieldIntValueByName(int result, const std::string &field);
  double resultFieldDoubleValueByName(int result, const std::string &field);
  grt::StringRef resultFieldStringValueByName(int result, const std::string &field);
  
  int closeResult(int result);

  int loadSchemata(int conn, grt::StringListRef schemata);
  int loadSchemaObjects(int conn, grt::StringRef schema, grt::StringRef object_type, grt::DictRef objects);

  grt::DictRef getServerVariables(int conn);
  
  grt::StringListRef loadSchemaList(int conn);
  grt::DictRef loadSchemaObjectList(int conn, grt::StringRef schema, grt::StringRef object_type);

  std::string generateDdlScript(grt::StringRef schema, grt::DictRef objects);

  
  // open SSH tunnel using the connection info
  // returns tunnel id or 0 if no tunnel needed
  int openTunnel(const db_mgmt_ConnectionRef &info);
  int getTunnelPort(int tunnel);
  int closeTunnel(int tunnel);

  std::string scramblePassword(const std::string& pass);

private:
  struct ConnectionInfo
  {
    typedef boost::shared_ptr<ConnectionInfo> Ref;
  
    ConnectionInfo(sql::ConnectionWrapper &c) : conn(c), last_error_code(0), last_update_count(0) {}
    
    sql::Connection *prepare()
    {
      last_error.clear();
      last_error_code = 0;
      last_update_count = 0;
      return conn.get();
    }
    
    sql::ConnectionWrapper conn;
    std::string last_error;
    int last_error_code;
    uint64_t last_update_count;
  };

  base::Mutex _mutex;
  std::map<int, ConnectionInfo::Ref> _connections;
  std::map<int, sql::ResultSet*> _resultsets;
  std::map<int, boost::shared_ptr<sql::TunnelConnection> > _tunnels;
  std::string _last_error;
  int _last_error_code;

  int _connection_id;
  int _resultset_id;
  int _tunnel_id;
};



GRT_MODULE_ENTRY_POINT(DbMySQLQueryImpl);

#define CLEAR_ERROR() do { _last_error.clear(); _last_error_code = 0; } while (0)

int DbMySQLQueryImpl::openConnection(const db_mgmt_ConnectionRef &info)
{
  return openConnectionP(info, grt::StringRef());
}


int DbMySQLQueryImpl::openConnectionP(const db_mgmt_ConnectionRef &info, const grt::StringRef &password)
{
  sql::DriverManager *dm = sql::DriverManager::getDriverManager();
  
  if (!info.is_valid())
    throw std::invalid_argument("connection info is NULL");

  int new_connection_id = -1;

  CLEAR_ERROR();
  try
  {
    {
      base::MutexLock lock(_mutex);
      new_connection_id = ++_connection_id;
    }
    sql::ConnectionWrapper conn;
    if (password.is_valid())
    {
      sql::Authentication::Ref auth = sql::Authentication::create(info, "");
      auth->set_password(password.c_str());

       conn = dm->getConnection(info, boost::shared_ptr<sql::TunnelConnection>(), auth);
    }
    else
        conn = dm->getConnection(info);

    base::MutexLock lock(_mutex);
    _connections[new_connection_id] = ConnectionInfo::Ref(new ConnectionInfo(conn));
  }
  catch (sql::SQLException &exc)
  {
    _last_error = exc.what();
    _last_error_code = exc.getErrorCode();
    base::MutexLock lock(_mutex);
    if (_connections.find(new_connection_id) != _connections.end())
      _connections.erase(new_connection_id);
    return -1;    
  }
  /*
  catch (std::exception &exc)
  {
    _last_error = exc.what();
    Lock lock(_mutex);
    if (_connections.find(new_connection_id) != _connections.end())
      _connections.erase(new_connection_id);
    return -1;
  }*/
  
  return new_connection_id;
}


int DbMySQLQueryImpl::closeConnection(int conn)
{
  CLEAR_ERROR();

  base::MutexLock lock(_mutex);
  if (_connections.find(conn) == _connections.end())
    throw std::invalid_argument("Invalid connection");
  _connections.erase(conn);

  return 0;
}


std::string DbMySQLQueryImpl::lastError()
{
  return _last_error;
}


int DbMySQLQueryImpl::lastErrorCode()
{
  return _last_error_code;
}


int DbMySQLQueryImpl::execute(int conn, const std::string &query)
{
  CLEAR_ERROR();
  
  ConnectionInfo::Ref cinfo;
  sql::Connection *con= 0;
  {
    base::MutexLock lock(_mutex);
    if (_connections.find(conn) == _connections.end())
      throw std::invalid_argument("Invalid connection");
    cinfo = _connections[conn];
    con = cinfo->prepare();
  }

  try
  {
    std::auto_ptr<sql::Statement> pstmt(con->createStatement());
    int r = pstmt->execute(query) ? 1 : 0;
    cinfo->last_update_count = pstmt->getUpdateCount();
    return r;
  }
  catch (sql::SQLException &exc)
  {
    _last_error = exc.what();
    _last_error_code = exc.getErrorCode();
    cinfo->last_error = exc.what();
    cinfo->last_error_code = _last_error_code;
    return -1;
  }
  catch (std::exception &e)
  {
    _last_error = e.what();
    cinfo->last_error = e.what();
    return -1;
  }
  
  return -1;
}


int DbMySQLQueryImpl::executeQuery(int conn, const std::string &query)
{
  CLEAR_ERROR();
  
  ConnectionInfo::Ref cinfo;
  sql::Connection *con= 0;
  {
    base::MutexLock lock(_mutex);
    if (_connections.find(conn) == _connections.end())
      throw std::invalid_argument("Invalid connection");
    cinfo = _connections[conn];
    con = cinfo->prepare();
  }

  try
  {
    std::auto_ptr<sql::Statement> pstmt(con->createStatement());
    sql::ResultSet *res = pstmt->executeQuery(query);

    ++_resultset_id;

    cinfo->last_update_count = pstmt->getUpdateCount();
    _resultsets[_resultset_id] = res;
  }
  catch (sql::SQLException &exc)
  {
    _last_error = exc.what();
    _last_error_code = exc.getErrorCode();
    cinfo->last_error = exc.what();
    cinfo->last_error_code = _last_error_code;
    return -1;
  }
  catch (std::exception &e)
  {
    _last_error = e.what();
    cinfo->last_error = e.what();
    return -1;
  }

  return _resultset_id;
}


uint64_t DbMySQLQueryImpl::lastUpdateCount(int conn)
{
  base::MutexLock lock(_mutex);
  if (_connections.find(conn) == _connections.end())
    throw std::invalid_argument("Invalid connection");
  return _connections[conn]->last_update_count;
}


int DbMySQLQueryImpl::lastConnectionErrorCode(int conn)
{
  base::MutexLock lock(_mutex);
  if (_connections.find(conn) == _connections.end())
    throw std::invalid_argument("Invalid connection");
  return _connections[conn]->last_error_code;
}


std::string DbMySQLQueryImpl::lastConnectionError(int conn)
{
  base::MutexLock lock(_mutex);
  if (_connections.find(conn) == _connections.end())
    throw std::invalid_argument("Invalid connection");
  return _connections[conn]->last_error;
}


size_t DbMySQLQueryImpl::resultNumRows(int result)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];

  return res->rowsCount();
}


int DbMySQLQueryImpl::resultNumFields(int result)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];
  
  return res->getMetaData()->getColumnCount();
}


std::string DbMySQLQueryImpl::resultFieldType(int result, int field)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];
  
  return res->getMetaData()->getColumnTypeName(field);  
}


std::string DbMySQLQueryImpl::resultFieldName(int result, int field)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];

  return res->getMetaData()->getColumnName(field);
}


int DbMySQLQueryImpl::resultFirstRow(int result)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];  
  return res->first() ? 1 : 0;
}


int DbMySQLQueryImpl::resultNextRow(int result)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];  
  return res->next() ? 1 : 0;
}


grt::IntegerRef DbMySQLQueryImpl::resultFieldIntValue(int result, int field)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];

  if (res->isNull(field))
    return grt::IntegerRef(0);
  else
    return grt::IntegerRef(res->getInt(field));
}


double DbMySQLQueryImpl::resultFieldDoubleValue(int result, int field)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];
  
  return res->getDouble(field);
}


grt::StringRef DbMySQLQueryImpl::resultFieldStringValue(int result, int field)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];

  if (res->isNull(field))
    return grt::StringRef();
  else 
    return grt::StringRef(res->getString(field));
}


grt::IntegerRef  DbMySQLQueryImpl::resultFieldIntValueByName(int result, const std::string &field)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];

  if (res->isNull(field))
    return grt::IntegerRef(0);
  else
    return grt::IntegerRef(res->getInt(field));
}


double DbMySQLQueryImpl::resultFieldDoubleValueByName(int result, const std::string &field)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];
  
  return res->getDouble(field);
}


grt::StringRef DbMySQLQueryImpl::resultFieldStringValueByName(int result, const std::string &field)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    throw std::invalid_argument("Invalid resultset");
  sql::ResultSet *res = _resultsets[result];
  
  if (res->isNull(field))
    return grt::StringRef();
  else
    return grt::StringRef(res->getString(field));
}


int DbMySQLQueryImpl::closeResult(int result)
{
  base::MutexLock lock(_mutex);
  if (_resultsets.find(result) == _resultsets.end())
    return -1;
  sql::ResultSet *res = _resultsets[result];
  delete res;
  _resultsets.erase(result);
  return 0;
}


int DbMySQLQueryImpl::loadSchemata(int conn, grt::StringListRef schemata)
{
  CLEAR_ERROR();
  
  ConnectionInfo::Ref cinfo;
  sql::Connection *con = 0;
  {
    base::MutexLock lock(_mutex);
    if (_connections.find(conn) == _connections.end())
      throw std::invalid_argument("Invalid connection");
    cinfo = _connections[conn];
    con = cinfo->prepare();
  }

  try
  {
    sql::DatabaseMetaData *dbc_meta(con->getMetaData());
    std::auto_ptr<sql::ResultSet> rset(dbc_meta->getSchemaObjects("", "", "schema"));
    while (rset->next())
    {
      std::string name = rset->getString("name");
      schemata.insert(name);
      //schemata_ddl.insert(rset->getString("ddl"));
    }
  }
  catch (sql::SQLException &exc)
  {
    _last_error = exc.what();
    _last_error_code = exc.getErrorCode();
    cinfo->last_error = exc.what();
    cinfo->last_error_code = _last_error_code;
    return -1;
  }
  catch (std::exception &e)
  {
    _last_error = e.what();
    cinfo->last_error = e.what();
    return -1;
  }
  
  return 0;
}


grt::DictRef DbMySQLQueryImpl::getServerVariables(int conn)
{
  grt::DictRef dict(get_grt());
  
  CLEAR_ERROR();
  
  ConnectionInfo::Ref cinfo;
  sql::Connection *con = 0;
  {
    base::MutexLock lock(_mutex);
    if (_connections.find(conn) == _connections.end())
      throw std::invalid_argument("Invalid connection");
    cinfo = _connections[conn];
    con = cinfo->prepare();
  }
  
  try
  {
    std::auto_ptr<sql::Statement> pstmt(con->createStatement());
    std::auto_ptr<sql::ResultSet> rset(pstmt->executeQuery("show variables"));

    while (rset->next())
    {
      std::string name = rset->getString("Variable_name");
      std::string value = rset->getString("Value");
      dict[name] = grt::StringRef(value);
    }
  }
  catch (sql::SQLException &exc)
  {
    _last_error = exc.what();
    _last_error_code = exc.getErrorCode();
    cinfo->last_error = exc.what();
    cinfo->last_error_code = _last_error_code;
  }
  catch (std::exception &e)
  {
    _last_error = e.what();
    cinfo->last_error = e.what();
  }
  
  return dict;
}


grt::StringListRef DbMySQLQueryImpl::loadSchemaList(int conn)
{
  grt::StringListRef list(get_grt());
  if (loadSchemata(conn, list) == 0)
    return list;
  return grt::StringListRef();
}


int DbMySQLQueryImpl::loadSchemaObjects(int conn, grt::StringRef schema, grt::StringRef object_type, grt::DictRef objects)
{
  CLEAR_ERROR();
  
  ConnectionInfo::Ref cinfo;
  sql::Connection *con = 0;
  {
    base::MutexLock lock(_mutex);
    if (_connections.find(conn) == _connections.end())
      throw std::invalid_argument("Invalid connection");
    cinfo = _connections[conn];
    con = cinfo->prepare();
  }

  try
  {
    std::list<std::string> object_types;
    if (object_type.empty())
    {
      object_types.push_back("table");
      object_types.push_back("view");
      object_types.push_back("routine");
      object_types.push_back("trigger");
    }
    else
    {
      object_types.push_back(object_type);
    }
    sql::DatabaseMetaData *dbc_meta(con->getMetaData());
    for (std::list<std::string>::const_iterator i= object_types.begin(), end= object_types.end(); i != end; ++i)
    {
      std::auto_ptr<sql::ResultSet> rset(dbc_meta->getSchemaObjects("", *schema, *object_type));
      while (rset->next())
      {
        std::string name = rset->getString("name");
        std::string ddl = rset->getString("ddl");
        objects.gset(name, ddl);
      }
    }
  }
  catch (sql::SQLException &exc)
  {
    _last_error = exc.what();
    _last_error_code = exc.getErrorCode();
    cinfo->last_error = exc.what();
    cinfo->last_error_code = _last_error_code;
    return -1;
  }
  catch (std::exception &e)
  {
    _last_error = e.what();
    cinfo->last_error = e.what();
    return -1;
  }
  
  return 0;
}


grt::DictRef DbMySQLQueryImpl::loadSchemaObjectList(int conn, grt::StringRef schema, grt::StringRef object_type)
{
  grt::DictRef objects(get_grt());
  if (loadSchemaObjects(conn, schema, object_type, objects) == 0)
    return objects;
  return grt::DictRef();
}


std::string DbMySQLQueryImpl::generateDdlScript(grt::StringRef schema, grt::DictRef objects)
{
  const std::string delimiter= "$$";
  std::string ddl_script = "DELIMITER " + delimiter + "\n\n";
  ddl_script += "USE `" + *schema + "`\n" + delimiter + "\n\n";
  for (grt::DictRef::const_iterator i = objects.begin(), end = objects.end(); i != end; ++i)
  {
    std::string name = i->first;
    std::string ddl = (grt::StringRef::can_wrap(i->second)) ? grt::StringRef::cast_from(i->second) : "";
    if (g_utf8_validate(ddl.c_str(), -1, NULL))
      ddl_script += ddl;
    else
      ddl_script += "CREATE ... `" + *schema + "`.`" + name + "`: DDL contains non-UTF symbol(s)";
    ddl_script += "\n" + delimiter + "\n\n";
  }
  return ddl_script;
}




int DbMySQLQueryImpl::openTunnel(const db_mgmt_ConnectionRef &info)
{
  sql::DriverManager *dm = sql::DriverManager::getDriverManager();
  boost::shared_ptr<sql::TunnelConnection> tun = dm->getTunnel(info);
  if (tun)
  {
    _tunnels[++_tunnel_id] = tun;
    return _tunnel_id;
  }
  return 0;
}


int DbMySQLQueryImpl::getTunnelPort(int tunnel)
{
  if (_tunnels.find(tunnel) == _tunnels.end())
    throw std::invalid_argument("Invalid tunnel-id");
  return _tunnels[tunnel]->get_port();
}

int DbMySQLQueryImpl::closeTunnel(int tunnel)
{
  if (_tunnels.find(tunnel) == _tunnels.end())
    throw std::invalid_argument("Invalid tunnel-id");
  _tunnels.erase(tunnel);
  return 0;
}

