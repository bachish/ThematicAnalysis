// ----------------------------------------------------------------------------
// File BASE_ARCHIVE.H
//
// (c) 2005 by Koziev Elijah     all rights reserved 
//
// SOLARIX Intellectronix Project http://www.solarix.ru
//                                http://sourceforge.net/projects/solarix  
//
// Licensed under the terms of GNU Lesser GPL
// You must not eliminate, delete or supress these copyright strings
// from the file!
//
// Content:
// LEM C++ library  http://www.solarix.ru
// ������� ����� ��� ������������� ����������� �������.
// ----------------------------------------------------------------------------
//
// CD->22.12.2004
// LC->17.08.2007
// --------------

#ifndef LEM_BASE_ARCHIVE__H
#define LEM_BASE_ARCHIVE__H
#pragma once

#include <lem/streams.h>
#include <lem/path.h>
#include <lem/smart_pointers.h>
#include <lem/solarix/file_factory.h>
#include <lem/solarix/se_plugin_options.h>

namespace lem
{
 namespace Pack
 {
  // ***********************************************************
  // ������ ������ (��� �� ����� ���� ���������!) ����������
  // ��� ��������� ������� ���������� ����� � ������.
  // ***********************************************************
  class Base_File_Processor
  {
   public:
    Base_File_Processor(void) {}

    virtual Solarix::Search_Engine::Detected_Format DoUnpack( const lem::Path& name )=0;
    virtual void FileDone(
                          const lem::Path& name, // ��� ����� � ������
                          lem::Stream &buffer // ����� � ������������� ������
                         )=0;
  };

  // *******************************************
  // ������� ����� ��� ������������ �������.
  // *******************************************

  class Base_Archive
  {
   protected:
    boost::shared_ptr<Stream> in; // ������� ����� - ��� �����.
    lem::MemStream *outfile; // ���������������� � ��������� ���� �� ������

    Base_File_Processor *processor;

    virtual void File_Extracted( const lem::Path &name );

   public:
    Base_Archive(void);
    virtual ~Base_Archive(void);

    virtual bool Open( boost::shared_ptr<Stream> Stream, const Solarix::Search_Engine::PluginOptions *Options );

    // ��������� � ������������ ��� ����� �� ������
    virtual void Extract_Files( Base_File_Processor &x )=0;

    // ����� ������� � ���������� ������������ ��������� ����
    // ������������ ���� � ���������� �����, ���� ���������� �� � ����������� ������.
    virtual const lem::Path Extract_File( const UFString &filename, lem::Pack::Base_File_Processor &x )=0;

    // ���� ������ ������ ���������, ���������� ����������� ��� ��������� ������.
    virtual void Extract_Commentary( UFString &buffer ) {}
  };

  // **************************************
  // ��������� ��� �������� �������������
  // **************************************
  class IUnpackerFactory
  {
   public:
    IUnpackerFactory(void) {}

    virtual Base_Archive* New(void* ptr=NULL) const=0;

    // ����� �� ����������� ��������� ��������� ����?
    virtual bool CanExtractSingleFile(void) const { return false; }
  };

 } // namespace Pack

} // namespace lem


#endif
