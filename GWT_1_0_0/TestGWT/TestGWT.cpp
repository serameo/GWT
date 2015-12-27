#include "GWT.h"
#include "TestRc.h"

using namespace GWT::Windows;
using namespace GWT::Controls;

//#pragma message( "We need to link TestRc.res" )

HINSTANCE g_hInstance = NULL;

class MyDlg : public Dialog
{
public:
  MyDlg(Window *pParent)
    : Dialog(pParent, IDD_DIALOG1)
  {
  }
};


class MyWnd : public FrameWindow
{
public:
  Button m_OK;
  MyDlg *m_pDlg;

public:
  MyWnd():m_pDlg(NULL){}
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
    m_OK.Create(TEXT("OK"),
      WS_CHILD|WS_VISIBLE,
      20, 20, 80, 30, this,
      IDOK, lpcs->hInstance);
    return 1;
  }

  DECLARE_COMMAND_MAP()

  void OnOK()
  {
    if (!m_pDlg)
    {
      m_pDlg = new MyDlg(this);
    }
    //m_pDlg->DoModaless();
    UINT nID = m_pDlg->DoModal();
    if (IDOK == nID)
    {
      MessageBox(m_hwnd, TEXT("You clicked OK"), TEXT("GWT TEST"), MB_OK|MB_ICONINFORMATION);
    }
    else if (IDCANCEL == nID)
    {
      MessageBox(m_hwnd, TEXT("You clicked Cancel"), TEXT("GWT TEST"), MB_OK|MB_ICONEXCLAMATION);
    }
  }
};
BEGIN_COMMAND_MAP(MyWnd, FrameWindow)
  ON_COMMAND_MAP(IDOK, BN_CLICKED, OnOK)
END_COMMAND_MAP(MyWnd, FrameWindow)

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
    m_pMainWnd = new MyWnd();
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
