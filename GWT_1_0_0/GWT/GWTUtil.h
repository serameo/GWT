///////////////////////////////////////////////////////////////////////////////
// GWTUtil.h
// Author: Seree Rakwong
// Date: 25-Dec-2015
//

#ifndef GWTUTIL_H
#define GWTUTIL_H

#include <string>
#include <map>
#include <assert.h>

#ifdef UNICODE
typedef std::wstring GWTString;
#else
typedef std::string GWTString;
#endif

#define GWTASSERT(expr)           assert((expr))



#endif //GWTUTIL_H
