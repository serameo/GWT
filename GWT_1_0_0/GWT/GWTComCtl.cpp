///////////////////////////////////////////////////////////////////////////////
// GWTComCtl.cpp
// Author: Seree Rakwong
// Date: 21-Dec-2015
//
#include "GWTComCtl.h"

using namespace GWT;
using namespace GWT::Windows;
using namespace GWT::CommonControls;

CommonControl::CommonControl(DWORD dwICC)
  : m_dwICC(dwICC)
{
}

BOOL CommonControl::Create(LPCTSTR lpWindowName,
                         DWORD dwStyle, int x, int y, int width, int height,
                         GWTWindow *pParent, UINT nID,//HWND hwndParent, UINT nID,
                         HINSTANCE hInst)
{
  return CommonControl::CreateEx(0, lpWindowName,
    dwStyle, x, y, width, height,
    pParent, nID, hInst);
}

BOOL CommonControl::CreateEx(DWORD dwExStyle,
                           LPCTSTR lpWindowName,
                           DWORD dwStyle, int x, int y, int width, int height,
                           GWTWindow *pParent, UINT nID,//HWND hwndParent, UINT nID,
                           HINSTANCE hInst)
{
  if(!InitControl()) return FALSE;

  if(m_bIsAttached){
    // do not allow create if it is still attached another window handle
    return FALSE;
  }
  HWND hwnd = CreateWindowEx(dwExStyle, GetClassName(), lpWindowName,
                dwStyle, x, y, width, height,
                pParent->GetHWND(), (HMENU)nID, 
                (hInst == NULL ? (HINSTANCE)GetModuleHandle(NULL) : hInst), 
                this);
  if(hwnd == NULL) return FALSE;
  m_hwnd = hwnd;

  WNDPROC wndproc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
  m_lpfnWndProc   = wndproc;

  SetWindowLong(hwnd, GWL_WNDPROC,  (LONG)GWTWindowImpl::StaticWndProc);
  SetWindowLong(hwnd, GWL_USERDATA, (LONG)this);

    m_pParentWnd = pParent;

  return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
ImageList::ImageList()
  : m_himl(NULL),
    m_bIsAttached(FALSE)
{
}

ImageList::~ImageList()
{
  if(!m_bIsAttached){
    Destroy();
  }
}

BOOL ImageList::Create(int cx, int cy, UINT flags, int init, int grow)
{
  if(m_bIsAttached){
    return NULL;
  }
  HIMAGELIST himl = ImageList_Create(cx, cy, flags, init, grow);
  if(himl == NULL) return FALSE;

  m_himl = himl;
  return TRUE;
}

BOOL ImageList::Destroy()
{
  if(m_bIsAttached){
    return TRUE;
  }
  BOOL bDestroy = ImageList_Destroy(m_himl);
  return bDestroy;
}

HIMAGELIST ImageList::Attach(HIMAGELIST himl)
{
  if(m_himl != NULL && !m_bIsAttached){
    // cannot attach because the current handle
    // having been created
    return NULL;
  }
  if(himl == m_himl || himl == NULL){
    return himl;
  }
  HIMAGELIST himlCur = m_himl;
  m_himl = himl;
  m_bIsAttached = TRUE;
  return himlCur;
}

HIMAGELIST ImageList::Detach()
{
  if(!m_bIsAttached){
    return NULL;
  }
  HIMAGELIST himl = m_himl;
  m_bIsAttached = FALSE;
  m_himl = NULL;
  return himl;
}

