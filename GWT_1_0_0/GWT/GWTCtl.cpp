///////////////////////////////////////////////////////////////////////////////
// GWTCtl.cpp
// Author: Seree Rakwong
// Date: 21-Dec-2015
//
#include "GWTCtl.h"

using namespace GWT;
using namespace GWT::Controls;

BOOL Control::Create(LPCTSTR lpWindowName,
                         DWORD dwStyle, int x, int y, int width, int height,
                         Window *pParent, UINT nID,//HWND hwndParent, UINT nID,
                         HINSTANCE hInst)
{
  return CreateEx(0, lpWindowName, dwStyle, 
    x, y, width, height,
    pParent, nID);
}

BOOL Control::CreateEx(DWORD dwExStyle,
                           LPCTSTR lpWindowName,
                           DWORD dwStyle, int x, int y, int width, int height,
                           Window *pParent, UINT nID, //HWND hwndParent, UINT nID,
                           HINSTANCE hInst)
{
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

  SetWindowLong(hwnd, GWL_WNDPROC,  (LONG)WindowImpl::StaticWndProc);
  SetWindowLong(hwnd, GWL_USERDATA, (LONG)this);

  return TRUE;
}

LRESULT Control::WndProc(UINT message, WPARAM wParam, LPARAM lParam){
  if(message != WM_PAINT){
    LRESULT res = Window::WndProc(message, wParam, lParam);
  }
  return CallWndProc(message, wParam, lParam);
}


BOOL Animate::Create(DWORD dwStyle, HWND hwndParent, UINT nID, HINSTANCE hInst)
{
  HWND hwnd = Animate_Create(hwndParent, nID, dwStyle, hInst);
  if(hwnd == NULL) return FALSE;
  m_hwnd = hwnd;

  WNDPROC wndproc = (WNDPROC)GetWindowLong(hwnd, GWL_WNDPROC);
  m_lpfnWndProc   = wndproc;

  SetWindowLong(hwnd, GWL_WNDPROC,  (LONG)WindowImpl::StaticWndProc);
  SetWindowLong(hwnd, GWL_USERDATA, (LONG)this);

  return TRUE;
}

