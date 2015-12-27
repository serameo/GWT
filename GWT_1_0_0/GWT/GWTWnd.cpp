///////////////////////////////////////////////////////////////////////////////
// GWTWnd.cpp
// Author: Seree Rakwong
// Date: 21-Dec-2015
//
#include "GWTWnd.h"
#include <map>
#include <vector>
#include <algorithm>

using namespace GWT;
using namespace GWT::Windows;
using namespace std;

typedef std::map<HWND, Window*>     NativeWindowMap;
typedef NativeWindowMap::iterator   NativeWindowMapIterator;
NativeWindowMap *SharedNativeWindowMap = NULL;

typedef std::vector<Window*>             ModalessDialogVector;
typedef ModalessDialogVector::iterator   ModalessDialogVectorIterator;
ModalessDialogVector *SharedModalessDialogs = NULL;

BOOL GWT::Windows::AreDialogMessages(LPMSG lpMsg)
{
  if (SharedModalessDialogs)
  {
    ModalessDialogVectorIterator it;
    for (  it  = SharedModalessDialogs->begin();
           it != SharedModalessDialogs->end();
         ++it)
    {
      //Dialog *pDlg = (Dialog*)(*it);
      if (IsDialogMessage((*it)->GetHWND(), lpMsg))
        return TRUE;
    }
  }
  return FALSE;
}

BOOL WindowImpl::RegisterWnd(HINSTANCE hInstance)
{
  WNDCLASSEX wc;
  wc.cbClsExtra = 0;
  wc.cbSize = sizeof(wc);
  wc.cbWndExtra = 0;
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wc.hCursor = LoadCursor(NULL, IDC_ARROW);
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hIconSm = wc.hIcon;
  wc.hInstance = hInstance;
  wc.lpfnWndProc = (WNDPROC)WindowImpl::StaticWndProc;
  wc.lpszClassName = GetClassName();
  wc.lpszMenuName = NULL;
  wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
  return RegisterClassEx(&wc);
}

BOOL CALLBACK WindowImpl::StaticDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  WindowImpl *pDlg = (WindowImpl*)GetWindowLong(hwnd, GWL_USERDATA);
  switch (message)
  {
  case WM_INITDIALOG:
    {
      pDlg = (WindowImpl*)lParam;
      pDlg->m_hwnd = hwnd;
      SetWindowLong(hwnd, GWL_USERDATA, (LONG)lParam);
    }
    break;
  }
  if (pDlg)
  {
    return pDlg->WndProc(message, wParam, lParam);
  }

  return FALSE;
}

Window *Window::FromHandle(HWND hwnd)
{
  if (!SharedNativeWindowMap)
  {
    SharedNativeWindowMap = new NativeWindowMap();
  }
  NativeWindowMapIterator it = SharedNativeWindowMap->find(hwnd);
  if (it != SharedNativeWindowMap->end())
  {
    if (! ::IsWindow((*it).first))
    {
      (*it).second->Detach();
      SharedNativeWindowMap->erase(it);
      return NULL;
    }
    return (*it).second;
  }
  Window *pWnd = new Window();
  pWnd->Attach(hwnd);
  (*SharedNativeWindowMap)[hwnd] = pWnd;
  return pWnd;
}

Dialog::Dialog(Window *pParent, UINT nIDTemplate)
  : m_pParentWnd(pParent)
  , m_pszTemplate(NULL)
  , m_nIDTemplate(nIDTemplate)
{
}

Dialog::Dialog(Window *pParent, LPCTSTR lpszTemplate)
  : m_pParentWnd(pParent)
  , m_pszTemplate(lpszTemplate)
  , m_nIDTemplate(0)
{
}

Dialog::~Dialog()
{
  std::map<HWND, GWT::Windows::Window*>::iterator it = m_wndMap.begin();
  for (; it != m_wndMap.end(); ++it)
  {
    (*it).second->Detach();
  }

  ModalessDialogVectorIterator it2 = std::find(SharedModalessDialogs->begin(),
                                               SharedModalessDialogs->end(),
                                               this);
  if (it2 != SharedModalessDialogs->end())
  {
    SharedModalessDialogs->erase(it2);
    m_hwnd = NULL;
  }
}

BOOL Dialog::DoModaless(HINSTANCE hInstance)
{
  if (m_hwnd && ::IsWindow(m_hwnd))
  {
    // bring it to front
    ShowWindow(SW_SHOW);
    UpdateWindow();
    SetFocus();
    return TRUE;
  }
  if (!hInstance)
  {
    hInstance = (HINSTANCE)::GetModuleHandle(NULL);
  }
  m_hwnd = CreateDialogParam(hInstance, 
    (m_nIDTemplate ? MAKEINTRESOURCE(m_nIDTemplate) : m_pszTemplate),
    m_pParentWnd->GetHWND(),
    (DLGPROC)WindowImpl::StaticDlgProc, (LPARAM)this);
  if (!m_hwnd)
    return FALSE;

  ShowWindow(SW_SHOW);
  UpdateWindow();
  // 
  if (!SharedModalessDialogs)
    SharedModalessDialogs = new ModalessDialogVector();
  
  ModalessDialogVectorIterator it = std::find(SharedModalessDialogs->begin(),
                                              SharedModalessDialogs->end(),
                                              this);
  if (it == SharedModalessDialogs->end())
    SharedModalessDialogs->push_back(this);
  return TRUE;
}

UINT Dialog::DoModal(HINSTANCE hInstance)
{
  if (!hInstance)
  {
    hInstance = (HINSTANCE)::GetModuleHandle(NULL);
  }

  UINT nID = DialogBoxParam(hInstance, 
    (m_nIDTemplate ? MAKEINTRESOURCE(m_nIDTemplate) : m_pszTemplate),
    m_pParentWnd->GetHWND(),
    (DLGPROC)WindowImpl::StaticDlgProc, (LPARAM)this);
  return (nID);
}

LRESULT Dialog::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
  case WM_INITDIALOG: 
    return OnInitDialog();
  //case WM_SYSCOMMAND:
  //  return 0;
  case WM_COMMAND: 
    {
      UINT id      = LOWORD(wParam);
      UINT code    = HIWORD(wParam);
      HWND hwndCtl = (HWND)lParam;
      switch (id)
      {
      case IDOK: 
        OnOK(); 
        break;
      case IDCANCEL: 
        OnCancel(); 
        break;
      default: 
        OnCommand(id, code, hwndCtl); 
        break;
      }
    }
    return 1;

  case WM_NOTIFY:
  //case WM_CTLCOLORMSGBOX:
  //case WM_CTLCOLOREDIT:
  //case WM_CTLCOLORLISTBOX:
  //case WM_CTLCOLORBTN:
  //case WM_CTLCOLORDLG:
  //case WM_CTLCOLORSCROLLBAR:
  //case WM_CTLCOLORSTATIC:
    // mouse events
  case WM_LBUTTONDOWN:
  case WM_LBUTTONUP:
  case WM_LBUTTONDBLCLK:
  case WM_RBUTTONDOWN:
  case WM_RBUTTONUP:
  case WM_RBUTTONDBLCLK:
  case WM_MOUSEMOVE:
    // owner draw events
  case WM_COMPAREITEM:
  case WM_DRAWITEM:
  case WM_DELETEITEM:
  case WM_MEASUREITEM:
    // keyboard events
  case WM_CHAR:
  case WM_SYSCHAR:
  case WM_DEADCHAR:
  case WM_SYSDEADCHAR:
  case WM_KEYDOWN:
  case WM_KEYUP:
  case WM_SYSKEYDOWN:
  case WM_SYSKEYUP:
  case WM_SETFOCUS:
  case WM_KILLFOCUS:
  case WM_SIZE:
  case WM_MOVE:
  case WM_QUERYOPEN:
  case WM_ACTIVATE:
    Window::WndProc(message, wParam, lParam);
    return 1;

  default: return 0;
  }
  return 0;
}

Window *Dialog::GetDlgItem(UINT nID)
{
//#pragma message("Dialog::GetDlgItem - [NOT IMPLEMENTED SUCCESSFULLY]")

  HWND hwnd = ::GetDlgItem(m_hwnd, nID);
  if (!hwnd)
    return NULL;

  Window *pWnd = (Window*)GetWindowLong(hwnd, GWL_USERDATA);
  
  return pWnd;
}

GWTString Dialog::GetDlgItemText(UINT nID)
{
  TCHAR szText[MAX_PATH];
  UINT nLen = GetDlgItemText(nID, szText, MAX_PATH);
  GWTString sText(szText);
  return sText;
}

UINT Dialog::GetDlgItemText(UINT nID, LPTSTR lpszText, int cchMax)
{
  TCHAR szText[MAX_PATH];
  return ::GetDlgItemText(m_hwnd, nID, szText, MAX_PATH);
}

BOOL Dialog::SetDlgItemText(UINT nID, LPCTSTR lpszText)
{
  return ::SetDlgItemText(m_hwnd, nID, lpszText);
}

int Dialog::OnInitDialog()
{
  HWND hwndChild = GetWindow(m_hwnd, GW_CHILD);
  for(; hwndChild != NULL; hwndChild = GetWindow(hwndChild, GW_HWNDNEXT))
  {
    //UINT nID = GetWindowLong(hwndChild, GWL_ID);
    Window *pWnd = new Window();
    pWnd->Attach(hwndChild);
    SetWindowLong(hwndChild, GWL_USERDATA, (LONG)pWnd);
  }
  return 1;
}




GWTApplication::GWTApplication(HINSTANCE hInstance, LPSTR lpszCmdLine, int nCmdShow)
  : m_hInstance(hInstance)
  , m_strCmdLine((LPCTSTR)lpszCmdLine)
  , m_nCmdShow(nCmdShow)
  , m_pMainWnd(NULL)
{
}

GWTApplication::~GWTApplication()
{
}

int GWTApplication::Run()
{
  if (!InitInstance())
  {
    ExitInstance(-1);
    return -1;
  }
  MSG msg;
  while(GetMessage(&msg, NULL, 0, 0))
  {
    if (!AreDialogMessages(&msg))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }
  ExitInstance(0);
  return 0;
}
