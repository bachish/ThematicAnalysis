// ----------------------------------------------------------------------------
// File FILE_LOCATION.H
//
// (c) by Koziev Elijah
//     Solarix Intellectronix project - http://www.solarix.ru
//
// Content:
// LEM C++ library
// ����� File_Location - ��������� ����� �����.
//
// 22.10.2006 - �������� ����� ��� ���������� ���������� - ��������� ���� �
//              ����������� ������.
// ----------------------------------------------------------------------------
//
// CD->21.03.2005
// LC->20.07.2008
// --------------

#ifndef FILE_LOCATION__H
#define FILE_LOCATION__H
#pragma once

 #include <lem/fstring.h>
 #include <lem/streams.h>
 #include <lem/date_time.h>

 namespace Solarix
 {
  namespace Search_Engine
  {
   using lem::UFString;

  // ***********************************************************************
  // ���������� ��� ����� - ������ � ������� � ����� ������������ �����
  // (��������� �������� �������, �����, ��������).
  // ***********************************************************************
  class File_Location
  {
   public:
    lem::UFString name;                  // ���� � �����
    lem::Stream::pos_type size; // ������ �����
    boost::posix_time::ptime creat; // ����� �������� �����
    boost::posix_time::ptime modif; // ����� ��������� ����������� ����� 

    typedef enum {
                  LOCALHOST, // ������� ���� �� ����������
                  ARCHIVE,   // ������ ������
                  INTERNET,  // �� ��������� ���-�������
                  CDROM,     // �� ������� ��������
                  MEMORYTEXT // ��������� ���� � ������
                 } LocType;

    LocType loc_type;      // ��� ����������
    UFString location;     // ���������� (��� ������, ��� CD)

    UFString tmp_filename; // ��� ��������� ����� ��������� (��� ���������
                           // URI � ������������� �������)  

    UFString file_format;  // ������� �������� ������� �����

    explicit File_Location(void) { loc_type=LOCALHOST; size=0; }

    File_Location( const File_Location&x )
     : name(x.name), creat(x.creat), modif(x.modif), location(x.location),
       tmp_filename(x.tmp_filename), file_format(x.file_format)
    {
     size = x.size;
     loc_type = x.loc_type;
    } 

	void operator=( const File_Location &x )
    {
     name = x.name;
     creat = x.creat;
     modif = x.modif;
     location = x.location;
     tmp_filename = x.tmp_filename;
     file_format = x.file_format;
     size = x.size;
     loc_type = x.loc_type;
    } 


    explicit File_Location( const UFString &filename )
    : name(filename) { loc_type=LOCALHOST; size=0; }

    explicit File_Location( const UFString &filename, const UFString &Loc, LocType lt )
    : name(filename), location(Loc) { loc_type=lt; size=0; }

    // ������������ ��� ������������ ��� ����� � ��� ������������.
    // ���� HumanReadable=false, �� ����������� �������� ���� ��� 
    // ����������� ������ ���������.
    const UFString GetFullName( bool HumanReadable=true ) const;

    // ���������� ���� � �����.
    const lem::Path GetAbsolutePath(void) const;

    // �������������� ����� - ����� ����� ���� ������� �����������.
    // ��� ������������� ������� � ��������� URI ����� ������� ��������� ���������.
    const UFString GetClickable(void) const;

    void SetLocalHost(void) { loc_type=LOCALHOST; }

    inline File_Location::LocType GetLocType(void) const { return loc_type; }
    inline bool IsLocalhost(void) const { return loc_type==LOCALHOST; }
    inline bool IsArchived(void) const  { return loc_type==ARCHIVE; }
    inline bool IsInternet(void) const  { return loc_type==INTERNET; }
    inline bool IsCdRom(void) const     { return loc_type==CDROM; }
    inline bool IsMemoryText(void) const     { return loc_type==MEMORYTEXT; }

    inline const UFString& GetName(void) const     { return name;         }
    inline const UFString& GetLocation(void) const { return location;     }
    inline const UFString& GetFormat(void) const   { return file_format;  }
    inline const UFString& GetTmp(void) const      { return tmp_filename; }
    inline lem::Stream::pos_type GetSize(void) const { return size; }

    inline const boost::posix_time::ptime& GetCreat(void) const   { return creat; }
    inline const boost::posix_time::ptime& GetModif(void) const   { return modif; }

    inline bool empty(void) const { return name.empty(); }

    inline void clear(void) 
    {
     name.clear();
     size=0;
     location.clear();
     tmp_filename.clear();
     file_format.clear();
    } 

    void SaveBin( lem::Stream& bin ) const;
    void LoadBin( lem::Stream& bin );

    bool operator==( const File_Location &x ) const;
    bool operator!=( const File_Location &x ) const;
    bool operator>( const File_Location &x ) const;
    bool operator<( const File_Location &x ) const;

    void RebasePath( const lem::UFString &old_prefix, const lem::UFString &new_prefix );
  };

  } // namespace Search_Engine
 } // namespace Solarix

#endif
