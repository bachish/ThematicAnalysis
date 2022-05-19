#ifndef LA_PROJECT_INFO__H
#define LA_PROJECT_INFO__H
#pragma once

 #include <lem/solarix/coord_pair.h>
 
 namespace Solarix
 {

  // *************************************** 
  // �������������� ���������� � ��������.
  // *************************************** 
  struct LA_ProjectInfo
  {
   CP_Array coords; // ������������ ����, ������� ���� �������������
                    // ��������� ��������.

   LA_ProjectInfo(void) {}
   LA_ProjectInfo( const LA_ProjectInfo &x ) : coords(x.coords) {}
   void operator=( const LA_ProjectInfo &x ) { coords=x.coords; }

   bool operator==( const LA_ProjectInfo &x ) const
   {
    if( coords.size()!=x.coords.size() )
     return false;

    for( lem::Container::size_type i=0; i<coords.size(); ++i )
     if( x.coords.FindOnce( coords[i] )==UNKNOWN )
      return false;

    return true;
   }

   bool empty(void) const { return coords.empty(); }
  };
 }

#endif
