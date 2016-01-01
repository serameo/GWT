#include "GWT.h"
#include "TestRc.h"

using namespace GWT::Windows;
using namespace GWT::Controls;

//#pragma message( "We need to link TestRc.res" )

HINSTANCE g_hInstance = NULL;

class MyDlg : public GWTDialog
{
public:
  MyDlg(GWTWindow *pParent)
    : GWTDialog(pParent, IDD_DIALOG1)
  {
  }
};


class MyWnd : public GWTFrameWindow
{
public:
  Button m_ModalDialog;
  Button m_ModalessDialog;
  MyDlg *m_pDlg;

public:
  MyWnd():GWTFrameWindow(IDR_FRAMEMENU1),m_pDlg(NULL){}
  virtual ~MyWnd()
  {
    if (m_pDlg) delete m_pDlg;
  }
protected:
  virtual void OnDestroy()
  {
    PostQuitMessage(0);
  }
  int OnCreate(LPCREATESTRUCT lpcs)
  {
      if (GWTFrameWindow::OnCreate(lpcs) != 1)
          return 0;

    m_ModalDialog.Create(TEXT("Call Modal Dialog"),
      WS_CHILD|WS_VISIBLE,
      20, 20, 200, 30, this,
      IDOK, lpcs->hInstance);
    m_ModalessDialog.Create(TEXT("Call Modaless Dialog"),
      WS_CHILD|WS_VISIBLE,
      20, 80, 200, 30, this,
      IDCANCEL, lpcs->hInstance);
    return 1;
  }

  DECLARE_COMMAND_MAP()

  void OnModalessDialog()
  {
    if (!m_pDlg)
    {
      m_pDlg = new MyDlg(this);
    }
    m_pDlg->DoModaless();
  }
  void OnModalDialog()
  {
    GWTDialog dlg(this, IDD_DIALOG1);
    UINT nID = dlg.DoModal();
    if (IDOK == nID)
    {
      MessageBox(m_hwnd, TEXT("You clicked OK"), TEXT("GWT TEST"), MB_OK|MB_ICONINFORMATION);
    }
    else if (IDCANCEL == nID)
    {
      MessageBox(m_hwnd, TEXT("You clicked Cancel"), TEXT("GWT TEST"), MB_OK|MB_ICONEXCLAMATION);
    }
  }
  void OnFileExit()
  {
      Destroy();
  }
};
BEGIN_COMMAND_MAP(MyWnd, GWTFrameWindow)
  ON_CONTROL_MAP(IDOK, BN_CLICKED, OnModalDialog)
  ON_CONTROL_MAP(IDCANCEL, BN_CLICKED, OnModalessDialog)
  ON_COMMAND_MAP(ID_FILE_EXIT, OnFileExit)
END_COMMAND_MAP(MyWnd, GWTFrameWindow)

class MyMDIChild : public GWTMDIChildWindow
{
public:
    MyMDIChild(GWTMDIFrameWindow *pFrame)
        : GWTMDIChildWindow(pFrame, IDR_CHILDMENU1, 1)
    {
    }
};
class MyMDIFrame : public GWTMDIFrameWindow
{
public:
    MyMDIFrame(UINT nIDResource, UINT nIDWindowMenu, UINT nIDFirstChild)
        : GWTMDIFrameWindow(nIDResource, nIDWindowMenu, nIDFirstChild)
    {
    }

    DECLARE_COMMAND_MAP()
protected:
  virtual void OnDestroy()
  {
    PostQuitMessage(0);
  }

  void OnFileNew()
  {
      GWTMDIChildWindow* pChild = new MyMDIChild(this);
      CreateMDIWindow(&pChild, TEXT("Hello"));
  }
};

BEGIN_COMMAND_MAP(MyMDIFrame, GWTMDIFrameWindow)
    ON_COMMAND_MAP(ID_FILE_NEW, OnFileNew)
END_COMMAND_MAP(MyMDIFrame, GWTMDIFrameWindow)

class MyApp : public GWTApplication
{
public:
  MyApp(HINSTANCE hInstance, LPSTR lpszCmdLine, int nCmdShow)
    : GWTApplication(hInstance, lpszCmdLine, nCmdShow)
  {
  }

protected:
  virtual BOOL InitInstance()
  {
    //m_pMainWnd = new MyWnd();
    //m_pMainWnd->CreateEx(0, 
    //  TEXT("Main Window"),
    //  WS_OVERLAPPEDWINDOW, 
    //  CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
    //  NULL, NULL, m_hInstance);

    m_pMainWnd = new MyMDIFrame(IDR_FRAMEMENU1, 0, 10000);
    m_pMainWnd->CreateEx(0, 
      TEXT("Main Window"),
      WS_OVERLAPPEDWINDOW, 
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
      NULL, NULL, m_hInstance);

    m_pMainWnd->ShowWindow(m_nCmdShow);
    m_pMainWnd->UpdateWindow();
    return TRUE;
  }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpszCmdLine, int nCmdShow)
{
  MyApp app(hInstance, lpszCmdLine, nCmdShow);
  return app.Run();
}
