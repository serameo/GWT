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

typedef std::map<HWND, GWTWindow*>     NativeWindowMap;
typedef NativeWindowMap::iterator   NativeWindowMapIterator;
NativeWindowMap *SharedNativeWindowMap = NULL;

typedef std::vector<GWTWindow*>             ModalessDialogVector;
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

BOOL GWTWindowImpl::RegisterWnd(HINSTANCE hInstance)
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
  wc.lpfnWndProc = (WNDPROC)GWTWindowImpl::StaticWndProc;
  wc.lpszClassName = GetClassName();
  wc.lpszMenuName = NULL;
  wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
  return RegisterClassEx(&wc);
}

BOOL CALLBACK GWTWindowImpl::StaticDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  GWTWindowImpl *pDlg = (GWTWindowImpl*)GetWindowLong(hwnd, GWL_USERDATA);
  switch (message)
  {
  case WM_INITDIALOG:
    {
      pDlg = (GWTWindowImpl*)lParam;
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

LRESULT CALLBACK GWTWindowImpl::StaticMDIChildProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  GWTWindowImpl *pMDIChild = (GWTWindowImpl*)GetWindowLongPtr(hwnd, GWL_USERDATA);
  switch (message)
  {
  case WM_INITDIALOG:
    {
      pMDIChild = (GWTWindowImpl*)lParam;
      pMDIChild->m_hwnd = hwnd;
      SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG)lParam);
    }
    break;
  }
  if (pMDIChild)
  {
    return pMDIChild->WndProc(message, wParam, lParam);
  }
  return DefMDIChildProc(hwnd, message, wParam, lParam);
}

LRESULT CALLBACK GWTWindowImpl::StaticMDIFrameProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  GWTWindowImpl *pMDIFrame = (GWTWindowImpl*)GetWindowLongPtr(hwnd, GWL_USERDATA);
  switch (message)
  {
  case WM_CREATE:
    {
        LPCREATESTRUCT lpcs = (LPCREATESTRUCT)lParam;
      pMDIFrame = (GWTWindowImpl*)lpcs->lpCreateParams;
      pMDIFrame->m_hwnd = hwnd;
      SetWindowLongPtr(hwnd, GWL_USERDATA, (LONG)pMDIFrame);
    }
    break;
  }
  if (pMDIFrame)
  {
    return pMDIFrame->WndProc(message, wParam, lParam);
  }
  return DefFrameProc(hwnd, GetWindow(hwnd, GW_CHILD), message, wParam, lParam);
}


GWTWindow *GWTWindow::FromHandle(HWND hwnd)
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
  GWTWindow *pWnd = new GWTWindow();
  pWnd->Attach(hwnd);
  (*SharedNativeWindowMap)[hwnd] = pWnd;
  return pWnd;
}

BOOL GWTWindow::Create(LPCTSTR lpWindowName,
                DWORD dwStyle, int x, int y, int width, int height,
                GWTWindow *pParent, HMENU hMenu,
                HINSTANCE hInst)
{
    return CreateEx(0, lpWindowName, dwStyle,
        x, y, width, height,
        pParent, hMenu, hInst);
}
BOOL GWTWindow::CreateEx(DWORD dwExStyle,
                LPCTSTR lpWindowName,
                DWORD dwStyle, int x, int y, int width, int height,
                GWTWindow *pParent, HMENU hMenu,
                HINSTANCE hInst)
{
    m_pParentWnd = pParent;
    return GWTWindowImpl::CreateEx(dwExStyle, lpWindowName, dwStyle,
        x, y, width, height,
        (pParent ? pParent->GetHWND() : NULL), hMenu, hInst);
}


GWTDialog::GWTDialog(GWTWindow *pParent, UINT nIDTemplate)
  : m_pParentWnd(pParent)
  , m_pszTemplate(NULL)
  , m_nIDTemplate(nIDTemplate)
{
}

GWTDialog::GWTDialog(GWTWindow *pParent, LPCTSTR lpszTemplate)
  : m_pParentWnd(pParent)
  , m_pszTemplate(lpszTemplate)
  , m_nIDTemplate(0)
{
}

GWTDialog::~GWTDialog()
{
  std::map<HWND, GWT::Windows::GWTWindow*>::iterator it = m_wndMap.begin();
  for (; it != m_wndMap.end(); ++it)
  {
    (*it).second->Detach();
    delete (*it).second;
  }

  if (SharedModalessDialogs)
  {
      ModalessDialogVectorIterator it2 = std::find(SharedModalessDialogs->begin(),
                                                   SharedModalessDialogs->end(),
                                                   this);
      if (it2 != SharedModalessDialogs->end())
      {
        SharedModalessDialogs->erase(it2);
        m_hwnd = NULL;
      }
  }
}

BOOL GWTDialog::DoModaless(HINSTANCE hInstance)
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
    (DLGPROC)GWTWindowImpl::StaticDlgProc, (LPARAM)this);
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

UINT GWTDialog::DoModal(HINSTANCE hInstance)
{
  if (!hInstance)
  {
    hInstance = (HINSTANCE)::GetModuleHandle(NULL);
  }

  UINT nID = DialogBoxParam(hInstance, 
    (m_nIDTemplate ? MAKEINTRESOURCE(m_nIDTemplate) : m_pszTemplate),
    m_pParentWnd->GetHWND(),
    (DLGPROC)GWTWindowImpl::StaticDlgProc, (LPARAM)this);
  return (nID);
}

LRESULT GWTDialog::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
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
    GWTWindow::WndProc(message, wParam, lParam);
    return 1;

  default: return 0;
  }
  return 0;
}

GWTWindow *GWTDialog::GetDlgItem(UINT nID)
{
//#pragma message("Dialog::GetDlgItem - [NOT IMPLEMENTED SUCCESSFULLY]")

  HWND hwnd = ::GetDlgItem(m_hwnd, nID);
  if (!hwnd)
    return NULL;

  GWTWindow *pWnd = (GWTWindow*)GetWindowLong(hwnd, GWL_USERDATA);
  
  return pWnd;
}

GWTString GWTDialog::GetDlgItemText(UINT nID)
{
  TCHAR szText[MAX_PATH];
  UINT nLen = GetDlgItemText(nID, szText, MAX_PATH);
  GWTString sText(szText);
  return sText;
}

UINT GWTDialog::GetDlgItemText(UINT nID, LPTSTR lpszText, int cchMax)
{
  TCHAR szText[MAX_PATH];
  return ::GetDlgItemText(m_hwnd, nID, szText, MAX_PATH);
}

BOOL GWTDialog::SetDlgItemText(UINT nID, LPCTSTR lpszText)
{
  return ::SetDlgItemText(m_hwnd, nID, lpszText);
}

int GWTDialog::OnInitDialog()
{
  HWND hwndChild = GetWindow(m_hwnd, GW_CHILD);
  for(; hwndChild != NULL; hwndChild = GetWindow(hwndChild, GW_HWNDNEXT))
  {
    GWTWindow *pWnd = new GWTWindow();
    pWnd->Attach(hwndChild);
    SetWindowLong(hwndChild, GWL_USERDATA, (LONG)pWnd);
    m_wndMap[hwndChild] = pWnd;
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
        if (m_pMainWnd && !m_pMainWnd->IsTranslateAccelerator(&msg))
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
    }
  }
  ExitInstance(0);
  return 0;
}

BOOL GWTMDIFrameWindow::RegisterWnd(HINSTANCE hInstance)
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
  wc.lpfnWndProc = (WNDPROC)GWTWindowImpl::StaticMDIFrameProc;
  //wc.lpfnWndProc = (WNDPROC)GWTWindowImpl::StaticWndProc;
  wc.lpszClassName = GetClassName();
  wc.lpszMenuName = NULL;//(m_nIDResource != 0 ? MAKEINTRESOURCE(m_nIDResource) : NULL);
  wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
  return RegisterClassEx(&wc);
}

GWTFrameWindow::~GWTFrameWindow()
{
    DestroyMenu(m_hMenu);
}

int GWTFrameWindow::PreCreateWindow(LPCREATESTRUCT lpcs)
{
    if (m_nIDResource != 0)
    {
        m_hAccel = LoadAccelerators(lpcs->hInstance, MAKEINTRESOURCE(m_nIDResource));
        m_hMenu = LoadMenu(lpcs->hInstance, MAKEINTRESOURCE(m_nIDResource));
        //lpcs->hMenu = m_hMenu;
    }
    return 1;
}

BOOL GWTFrameWindow::IsTranslateAccelerator(LPMSG lpMsg)
{
    if (m_hAccel)
    {
        return ::TranslateAccelerator(m_hwnd, m_hAccel, lpMsg);
    }
    return FALSE;
}

int GWTMDIFrameWindow::OnCreate(LPCREATESTRUCT lpcs)
{
    if (GWTFrameWindow::OnCreate(lpcs) != 1)
        return 0;

    CLIENTCREATESTRUCT ccs;  
    // Retrieve the handle to the window menu and assign the 
    // first child window identifier.
    SetMenu(m_hwnd, m_hMenu);
    HMENU hMenu = GetMenu();
    GWTASSERT(m_hMenu == hMenu);
    DrawMenuBar();

    ccs.hWindowMenu = GetSubMenu(hMenu, m_nIDWindowMenu);
    ccs.idFirstChild = m_nIDFirstChild;

    HWND hwndMDIClient = CreateWindowEx(0, m_pMDIClient->GetClassName(), (LPCTSTR) NULL, 
            WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL, 
            0, 0, 0, 0, m_hwnd, (HMENU)GWTMDIClient::ID, 
            lpcs->hInstance, (LPSTR) &ccs);
    if (hwndMDIClient)
    {
        SetWindowLongPtr(hwndMDIClient, GWL_USERDATA, (LONG)(m_pMDIClient));
        m_pMDIClient->Attach(hwndMDIClient);
        m_pMDIClient->ShowWindow(SW_SHOW);
        return 1;
    }

    return 0;
}

GWTMDIChildWindow::GWTMDIChildWindow(GWTMDIFrameWindow *pMDIFrameWindow, UINT nIDResource, UINT nWindowMenuPos)
    : GWTFrameWindow(nIDResource)
    , m_pMDIFrameWindow(pMDIFrameWindow)
    , m_nWindowMenuPos(nWindowMenuPos)
{
    HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);
    RegisterWnd(hInstance);
}

BOOL GWTMDIChildWindow::RegisterWnd(HINSTANCE hInstance)
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
  wc.lpfnWndProc = (WNDPROC)GWTWindowImpl::StaticMDIChildProc;
  wc.lpszClassName = GetClassName();
  wc.lpszMenuName = NULL;(m_nIDResource != 0 ? MAKEINTRESOURCE(m_nIDResource) : NULL);
  wc.style = CS_HREDRAW|CS_VREDRAW|CS_OWNDC;
  return RegisterClassEx(&wc);
}


BOOL GWTMDIFrameWindow::CreateMDIWindow(GWTMDIChildWindow **ppChild, 
    LPCTSTR lpWindowName, DWORD dwStyle,
    int x, int y, int w, int h, 
    HINSTANCE hInstance)
{
    GWTASSERT(ppChild);
    GWTASSERT(*ppChild);

    if (!hInstance)
        hInstance = (HINSTANCE)::GetModuleHandle(NULL);

    //HWND hwndChild = ::CreateMDIWindow((*ppChild)->GetClassName(),
    //    lpWindowName, dwStyle,
    //    x, y, w, h,
    //    m_pMDIClient->GetHWND(),
    //    hInstance, NULL);
    MDICREATESTRUCT    mdicreate ;
    mdicreate.szClass = (*ppChild)->GetClassName();
    mdicreate.szTitle = lpWindowName;
    mdicreate.hOwner  = hInstance;
    mdicreate.x       = x;
    mdicreate.y       = y;
    mdicreate.cx      = w;
    mdicreate.cy      = h;
    mdicreate.style   = 0;
    mdicreate.lParam  = (LPARAM)(*ppChild);
               
    HWND hwndChild = (HWND) m_pMDIClient->SendMessage(WM_MDICREATE, 0, (LPARAM)&mdicreate) ;

    if (hwndChild)
    {
        SetWindowLongPtr(hwndChild, GWL_USERDATA, (LONG)(*ppChild));
        (*ppChild)->Attach(hwndChild);
        //m_pMDIClient->SubclassWnd(hwndChild);
        m_pMDIClient->RegisterChild(hwndChild, (*ppChild));
        return TRUE;
    }
    //delete *ppChild;
    return FALSE;
}

GWTMDIChildWindow *GWTMDIFrameWindow::GWTMDIClient::MDIGetActive()
{
    HWND hwndActive = (HWND)SendMessage(WM_MDIACTIVATE);
    std::map<HWND, GWTMDIChildWindow*>::iterator it = m_children.find(hwndActive);
    if (it == m_children.end())
        return NULL;
    return (*it).second;
}

void GWTMDIFrameWindow::GWTMDIClient::RegisterChild(HWND hwndChild, GWTMDIChildWindow* pChild)
{
    m_children[hwndChild] = pChild;
}

LRESULT GWTMDIChildWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_MDIACTIVATE:
        OnMDIActivate();
        return 0;
    }
    return DefMDIChildProc(m_hwnd, message, wParam, lParam);
}

void GWTMDIChildWindow::OnMDIActivate()
{
    GetParent()->SendMessageW(WM_MDISETMENU, (WPARAM)GetMenu(), (LPARAM)GetSubWindowMenu());
}

LRESULT GWTMDIFrameWindow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{ 
    switch (message)
    {
    case WM_COMMAND:
        GWTMDIChildWindow *pChild = m_pMDIClient->MDIGetActive();
        if (pChild)
        {
            pChild->SendMessage(WM_COMMAND, wParam, lParam);
        }
        break;
    }
    return DefFrameProc(m_hwnd, m_pMDIClient->GetHWND(), message, wParam, lParam); 
}
