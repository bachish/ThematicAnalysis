// -----------------------------------------------------------------------------
// File DB.H
//
// (c) Koziev Elijah
//
// Content:
// DataBase interface classes. ����������� ���� ������. ����������
// ���������� � �� ����� ODBC � ADO.NET.
// -----------------------------------------------------------------------------
//
// CD->11.12.2000
// LC->19.09.2009
// --------------

#ifndef LEM_DB__H 
#define LEM_DB__H
#pragma once

 #include <lem/config.h>

 #include <lem/containers.h>
 #include <lem/oformatter.h>
 #include <lem/date_time.h>
 #include <lem/xml_parser.h>
 #include <lem/nullable.h>

 #if defined LEM_WINDOWS
  #if defined LEM_MFC
   #include <afxwin.h>
  #else 
   #include <windows.h>
  #endif

  #include <sql.h>
  #include <sqlext.h>
 #endif

 #if defined LEM_DOT_NET
  #using <System.Data.DLL>
  #using <System.XML.DLL>
  #include <vcclr.h>
 #endif

 #if defined LEM_WXWIDGETS && defined LEM_WINDOWS
  #include <databaselayer/DatabaseLayer.h>
 #endif

 #if defined LEM_USE_OCI
  #include <oci.h>
  #include <ocidem.h>
 #endif

 namespace lem
 {
  namespace Database
  {
   using lem::Collect;
   using lem::FString;
   
  struct E_DB_Error
  {
   std::string text;

   E_DB_Error() {}
   E_DB_Error( const char *Text ):text(Text) {}

   E_DB_Error( const E_DB_Error &x ) : text( x.text ) {}
   void operator=( const E_DB_Error &x )
   { text = x.text; }
  };


  // ************************************************
  // ��������� ��������� ��� ������ SQL-���������
  // ************************************************
  struct SQL_Param
  {
   bool out;          // false, ���� �������, true - ������������.
   std::string name;  // ��� (��� ADO)
   std::string value; // �������� ���������
   int max_len;       // ��� ������������ - ����. ����� ����������

   SQL_Param(
             const std::string &Name,
             bool IsOut,
             const std::string &Value,
             int MaxLen
            );

   SQL_Param( const SQL_Param& x );

   void operator=( const SQL_Param& x );
  };

  // **********************************************
  // ��������� ������� ��������������� ���������.
  // **********************************************
  struct Column_Descriptor
  {
   int index;        // ������ (������� � 1)
   std::string name; // ���
   int length;       // ����� ��� ��������� ������

   typedef enum {
                 CT_UNKNOWN,
                 CT_CHAR,
                 CT_DATE
                } sql_type;

   sql_type type;      // ���� ����

   Column_Descriptor(void);
   Column_Descriptor( const Column_Descriptor &x );
   const Column_Descriptor& operator=( const Column_Descriptor &x );
  };

  // *************************************************************
  // ������� ����� ��� ������������� ��������������� � ����������
  // ������ ��������� (�������). ��������� ����� ������������
  // ������������� ��������� ������ ��� ��������� �������� � �����
  // ���� ������� ������ ��������������� ���������.
  // *************************************************************
  class SQL_Cursor
  {
   private:
    // ����������� ���������.
    SQL_Cursor( const SQL_Cursor& );
    void operator=( const SQL_Cursor& );

   public:
    SQL_Cursor(void);

    // ����������� ���������� - ������ ���� ���������� � ������-�������
    // ��� ���������� ����������� ������.
    virtual ~SQL_Cursor(void);

    virtual void Get( int iColumn, lem::Nullable<lem::FString> &Res );
    virtual void Get( int iColumn, lem::Nullable<lem::UFString> &Res );
    virtual void Get( int iColumn, lem::Nullable<int> &Res )=0;
    virtual void Get( int iColumn, lem::Nullable<double> &Res )=0;
    virtual void Get( int iColumn, lem::Nullable<boost::gregorian::date> &Res, const char *Format );
    virtual void Get( int iColumn, lem::Nullable<boost::posix_time::ptime> &Res, const char *Format );

    // ����������� �� ��������� ������ � �������������� ���������.
    // ������ false, ���� ����������� ���������� (������ ���������).
    virtual bool Fetch(void)=0;

    // ������� �������� � �������������� ���������
    virtual int Get_n_Col(void);

    // ������� ����� � �������������� ���������
    virtual int Get_n_Row(void);

    // ���������� � �������� ��������������� ���������.
    virtual void Cols_Info( lem::PtrCollect<Column_Descriptor> &info );
  };



  // ****************************************************************
  // ����������� �������� ������ (������ ��).
  // ****************************************************************
  class SQL_Data_Source
  {
   protected:
    lem::zbool connected;      // DB has already been connected.
    lem::OFormatter *trace;    // ����� ��� ������ �������������� ���������
    lem::zbool handle_errors;  //

   protected:
    virtual void TraceSql(
                          const char *mes,
                          const char *sql,
                          const char *a
                         );

    // ���������� ����� ��������� ����������� � ��.
    virtual void Connected(void);

   public:
    SQL_Data_Source(void);
    virtual ~SQL_Data_Source(void);

    virtual bool Connect(void)=0;
    virtual bool Disconnect(void)=0;

    // ��������� ��������� �����������
    virtual bool IsConnected(void) const;

    // ����� ��� ������ �����������
    virtual void SetTrace( OFormatter *Trace );

    // ������ true ���� ���������� ��������������.
    virtual bool DoesSupportTxn() const;

    // ���������� ����������.
    virtual bool RollBack(void)=0;
    virtual bool Commit(void)=0;

    // *******************************************************
    // ������� ���� ������� � ��������� �������.
    // *******************************************************
    virtual bool CleanTable( const char *TableName )=0;

    // ���������������� � ���������� SQL-�������. ������������
    // ��������� �� ������������ ������.
    virtual SQL_Cursor* OpenCursor( const char *Command )=0;
    virtual SQL_Cursor* OpenCursor( const lem::FString &Command );

    virtual void Procedure(
                           const char *ProcName,
                           std::vector< SQL_Param* > &params
                          )=0;

    virtual void Command( const char *SqlCommand )=0;
    virtual int read_int( const char *Sql )=0;

    // ������ ���������� ������� �������� ��������� ������
    // (������, ���� � �.�.)
    virtual std::string GetDesc(void) const;
  };

  // ----------------------------------------------------------------


  #if defined(LEM_WATCOM) && LEM_VERSION==11
   #define SQLHENV                 HENV
   #define SQLHDBC                 HDBC
   #define SQLHSTMT                HSTMT
   #define SQLUSMALLINT            UWORD
   #define SQLSMALLINT             SWORD
   #define SQL_TRANSACTION_CAPABLE SQL_TXN_CAPABLE
   #define SQL_TC_NONE             0
   #define SQL_ASYNC_ENABLE_OFF    0UL
   #define SQL_ASYNC_ENABLE_ON     1UL
  #endif

  struct ODBC_DSN
  {
   lem::FString name; // ���������� ����� ��� �����������
   lem::FString desc; // ������������ ��������

   ODBC_DSN() {}
   ODBC_DSN( const ODBC_DSN &x ) { name=x.name; desc=x.desc; }
   ODBC_DSN( const char *Name, const char *Desc ) { name=Name; desc=Desc; }

   inline void operator=( const ODBC_DSN &x ) { name=x.name; desc=x.desc; }
  };


  class ODBC_Cursor : public SQL_Cursor
  {
   private:
    SQLHSTMT handle;

   public:
    ODBC_Cursor( SQLHSTMT Handle );
    virtual ~ODBC_Cursor();

    virtual void Get( int iColumn, lem::Nullable<lem::FString> &Res );
    virtual void Get( int iColumn, lem::Nullable<lem::UFString> &Res );
    virtual void Get( int iColumn, lem::Nullable<int> &Res );
    virtual void Get( int iColumn, lem::Nullable<double> &Res );

    virtual bool Fetch(void);

    virtual int Get_n_Col();
    virtual int Get_n_Row();
    virtual void Cols_Info( lem::PtrCollect<Column_Descriptor> &info );
  };


  #if defined LEM_DOT_NET

  using namespace System::Data;
  using namespace System::Data::OleDb;

  class ADO_NET_Cursor : public SQL_Cursor
  {
   private:
    gcroot<OleDbConnection*> con;
    gcroot<OleDbDataAdapter*> adapter;
    gcroot<OleDbCommand*> command;
    gcroot<DataSet*> data_set;
    gcroot<DataTable*> table;
    gcroot<DataRowCollection*> rows;
    gcroot<DataRow*> ROW; // ������� ������
    int count; // Number of rows
    int irow; // Current row

   public:
    ADO_NET_Cursor( gcroot<OleDbConnection*> Con, const char *Sql );
    virtual ~ADO_NET_Cursor();

    virtual int GetInt( int iColumn );
    virtual double GetDouble( int iColumn );
    virtual void GetStr( int iColumn, int MaxSize, std::string &Res );
    virtual bool Fetch();
  };
  #endif

  struct ODBC_Driver
  {
   lem::FString desc;
   lem::FString attr;

   ODBC_Driver() {};
   ODBC_Driver( const ODBC_Driver &x ) { desc=x.desc; attr=x.attr; }
   ODBC_Driver( const char *Desc, const char *Attr ) { desc=Desc; attr=Attr; }

   inline void operator=( const ODBC_Driver &x ) { desc=x.desc; attr=x.attr; }
  };



  // ****************************************************************
  // ODBC API incapsulator.
  // ****************************************************************
  class SQL_ODBC : public SQL_Data_Source
  {
   private:

    #if ODBCVER==0x0201
     HENV hEnv;
     HDBC hDbc;
    #else
     SQLHENV hEnv;
     SQLHDBC hDbc;
    #endif

    SQLHSTMT h_last_statement;

    void ParseList( char *s ) const;

   protected:
    int i_bind_col;
    unsigned int error;  // Error code
    bool internal_error;
    bool internal_abort;
    bool use_gui;

    lem::FString params;
    lem::FString dns, uid, psw;

   public:
    SQL_ODBC( const char *Params="" );
    SQL_ODBC( const char *DNS, const char *UID, const char *PSW );

    virtual ~SQL_ODBC();

    virtual bool Connect(void);
    virtual bool Disconnect(void);

    virtual bool WasError(void) const;
    virtual void Check(void);
    virtual void Failed(void);

    inline void AbortInternally() { internal_abort=true;  }
    inline void AbortExternally() { internal_abort=false; }

    inline void UseGui()     { use_gui=true;  }
    inline void DontUseGui() { use_gui=false; }

    virtual bool CleanTable( const char *TableName );

    virtual SQLHSTMT Execute( const char *s );
    virtual SQLHSTMT Prepare( const char *s );

    virtual SQL_Cursor* OpenCursor( const char *Command );

    virtual bool Do( SQLHSTMT hstmt, bool Async=false );
    virtual bool Done( SQLHSTMT hstmt );
    virtual bool Cancel( SQLHSTMT hstmt );
    virtual bool ExecuteNow( const char *s );
    virtual void Call( const char *Sql );

    virtual bool Executed( SQLHSTMT hStatement );

    virtual bool BindCol(
                         SQLHSTMT hStatement,
                         int iColumn,
                         int Type,
                         void *value_buffer,
                         int buffer_size,
                         INT32 *pcbValue=NULL
                        );

    virtual bool BindParam(
                           SQLHSTMT hStatement,
                           int iColumn,
                           int InOut,
                           int C_Type,
                           int Sql_Type,
                           void *value_buffer,
                           int buffer_size,
                           long *pcbValue=NULL
                          );

    virtual bool BindOutParam(
                              SQLHSTMT hStatement,
                              int iColumn,
                              void *value_buffer,
                              int buffer_size
                             );

    virtual bool BindInParam(
                             SQLHSTMT hStatement,
                             int iColumn,
                             void *value_buffer,
                             int buffer_size
                            );

    virtual bool BindCol(
                         SQLHSTMT hStatement,
                         void *value_buffer,
                         int buffer_size
                        );

    virtual bool BindIntCol( SQLHSTMT hStatement, int *value_buffer );

    virtual bool Fetch( SQLHSTMT hStatement );

    const CString read_str( const char *Default="" );
    const CString read_string( const char *Sql, const char *Default="" );

    virtual int read_int( const char *Sql );
    virtual const lem::FString read_fstring( const char *Sql );

    #if !defined LEM_NOREAL
    lem::Math::REAL read_real( const char *Sql, lem::Math::REAL Default=0 );
    #endif

    virtual void EnableAutoTxn(void);
    virtual void DisableAutoTxn(void);

    virtual bool RollBack(void);
    virtual bool Commit(void);

    void Get_DSN_List( lem::Collect<ODBC_DSN> &list );
    void Get_Drv_List( lem::Collect<ODBC_Driver> &list );

    static void ListDSN( lem::Collect<ODBC_DSN> &list );
 
    const lem::FString GetInfo( SQLUSMALLINT InfoType ) const;
    unsigned GetInfo_i( SQLUSMALLINT InfoType ) const;

    const lem::FString GetDSN(void) const;
    const lem::FString GetServerName(void) const;
    const lem::FString GetDBMSName(void) const;
    const lem::FString GetDBMSVer(void) const;
    const lem::FString GetUserName(void) const;

    const lem::FString GetAccessibleTables(void) const;
    const lem::FString GetAccessibleProcs(void) const;
    int GetDefTxnIsolation(void) const;

    int GetTxnCapable(void) const;
    int GetMaxColName(void) const;
    int GetMaxStmtLen(void) const;

    // ������ true, ���� �������� ������ ������������ ����������.
    virtual bool DoesSupportTxn(void) const;

    virtual void Procedure(
                           const char *ProcName,
                           std::vector< SQL_Param* > &params
                          );
    virtual void Command( const char *SqlCommand );

    virtual std::string GetDesc() const { return GetDSN().c_str(); }
  };

  class OracleOdbc_DB : public SQL_ODBC
  {
   public:
    OracleOdbc_DB( const char *Service, const char *Uid, const char *Psw );
  };



  #if defined LEM_DOT_NET
  class SQL_ADO_NET : public SQL_Data_Source
  {
   private:
    gcroot<OleDbConnection*> con;
    gcroot<OleDbTransaction*> trx; // ������� �������� ����������

   public:
    SQL_ADO_NET( OleDbConnection *Con );

    virtual bool Connect() { return true; }
    virtual bool Disconnect() { return true; }

    virtual bool WasError() const { return false; }
    virtual void Check() {}
    virtual void Failed() {}

    // ************************************************************
    // �������, ��� ADO ������ ������������ ����������.
    // ************************************************************
    virtual bool DoesSupportTxn() const { return true; }

    virtual bool RollBack();
    virtual bool Commit();

    // **********************************************************
    // ������������ ���������������/������� ���������� ����������
    // ����������.
    // **********************************************************
    virtual void EnableAutoTxn()  {}
    virtual void DisableAutoTxn() {}

    // *******************************************************
    // ������� ���� ������� � ��������� �������.
    // *******************************************************
    virtual bool CleanTable( const char *TableName );

    // **********************************************************
    // ���������������� � ���������� SQL-�������. ������������
    // ��������� �� ������������ ������ ADO.NET
    // **********************************************************
    virtual SQL_Cursor* OpenCursor( const char *Command );
    virtual SQL_Cursor* OpenCursor( const lem::FString &Command );

    virtual void Procedure(
                           const char *ProcName,
                           std::vector< SQL_Param* > &params
                          );
    virtual void Command( const char *SqlCommand );
    virtual int read_int( const char *Sql );
  };
  #endif


  extern void get_dsn( lem::Collect<ODBC_DSN> &lst );


  // ****************************************************************
  // ������ � XML ������ ��� � ������� ��.
  // ****************************************************************

  class SQL_XML_Cursor : public SQL_Cursor
  {
   private:
    const lem::Xml::Node *base;
    UFString leaf_name;
    lem::Xml::Iterator iter;
    vector<string> columns; // ����� ����������� �����

   public:
    SQL_XML_Cursor(
                   const lem::Xml::Node *Base,
                   const UFString &Leaf_name,
                   const vector<string> &Columns
                  );

    virtual int GetInt( int iColumn );
    virtual double GetDouble( int iColumn );
    virtual void GetStr( int iColumn, int MaxSize, std::string &Res );
    virtual bool Fetch(void);
  };


  class XML_Data_Source
  {
   private:
    boost::shared_ptr<lem::Xml::Parser> xml;
    const lem::Xml::Node* base;

   public:
    XML_Data_Source( const lem::Path &xml_filename );

    virtual bool Connect(void);
    virtual bool Disconnect(void);

    virtual bool RollBack(void);
    virtual bool Commit(void);

    virtual void EnableAutoTxn(void);
    virtual void DisableAutoTxn(void);

    virtual bool CleanTable( const char *TableName );

    virtual SQL_Cursor* OpenCursor( const char *Command );

    virtual void Procedure(
                           const char *ProcName,
                           std::vector< SQL_Param* > &params
                          );

    virtual void Command( const char *SqlCommand );
    virtual int read_int( const char *Sql );
  };



  #if defined LEM_USE_OCI

  class OCI_DataSource : public SQL_Data_Source
  {
   protected:
    friend class OCI_Cursor;
 
    lem::FString tns, user, password;
    Lda_Def lda;
    ub1 hda[HDA_SIZE];

   public:
    OCI_DataSource( const char *Tns, const char *User, const char *Psw );
    virtual ~OCI_DataSource(void);

    virtual bool Connect(void);
    virtual bool Disconnect(void);

    // ������ true ���� ���������� ��������������.
    virtual bool DoesSupportTxn() const;

    // ���������� ����������.
    virtual bool RollBack(void);
    virtual bool Commit(void);

    // ������� ���� ������� � ��������� �������.
    virtual bool CleanTable( const char *TableName );

    // ���������������� � ���������� SQL-�������. ������������
    // ��������� �� ������������ ������.
    virtual SQL_Cursor* OpenCursor( const char *Command );

    virtual void Procedure(
                           const char *ProcName,
                           std::vector< SQL_Param* > &params
                          );

    virtual void Command( const char *SqlCommand );
    virtual int read_int( const char *Sql );

    // ������ ���������� ������� �������� ��������� ������ (������, ���� � �.�.)
    virtual std::string GetDesc(void) const;
  };

  class OCI_Cursor : public SQL_Cursor
  {
   public: 
    lem::zbool ok;

   private:
    Cda_Def cda;
    OCI_DataSource *conn;

	struct _cs_datefmt desc[MAX_SELECT_LIST_SIZE];
	struct _column_data def[MAX_SELECT_LIST_SIZE];

    int Describe(void);

   public:
    OCI_Cursor( OCI_DataSource *c, const char *sql );
    virtual ~OCI_Cursor(void);

    virtual int GetInt( int iColumn );
    virtual double GetDouble( int iColumn );
    virtual void GetStr( int iColumn, int MaxSize, std::string &Res );
    virtual bool Fetch(void);
  };

  #endif

  } // namespace Database
 } // namespace 'lem'

#endif
