///////////////////////////////////////////////////////////////////////////////
// GWTWnd.h
// Author: Seree Rakwong
// Date: 21-Dec-2015
//

#ifndef GWTWND_H
#define GWTWND_H

#include "GWTTempl.h"
#include "GWTUtil.h"
#include <map>

namespace GWT
{

  namespace Windows
  {

    class Dialog;
    class WindowImpl : public TWindow<WindowImpl>
    {
      friend class Dialog;
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("WindowImpl_ClassName"); }
      static BOOL CALLBACK StaticDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    protected:
      virtual BOOL    RegisterWnd(HINSTANCE);
    };

    class Window : public WindowImpl
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTWindow_ClassName"); }
      static   Window* FromHandle(HWND hwnd);
    };

    class FrameWindow : public Window
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTFrameWindow_ClassName"); }
    };

    class View : public Window
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTView_ClassName"); }
    };

    class SplitView : public View
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTSplitView_ClassName"); }
    };

    class ScrollView : public View
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTScrollView_ClassName"); }
    };

    class FormView : public ScrollView
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTFormView_ClassName"); }
    };

    class MDIFrameWindow : public FrameWindow
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTMDIFrameWindow_ClassName"); }
    };

    class MDIChildWindow : public FrameWindow
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTMDIChildWindow_ClassName"); }
    };
    //
    // resource: https://msdn.microsoft.com/en-us/library/windows/desktop/aa381002(v=vs.85).aspx
    // ref: https://msdn.microsoft.com/en-us/library/windows/desktop/aa381003(v=vs.85).aspx
    class Dialog : public Window
    {

    protected:
      LPCTSTR m_pszTemplate;
      UINT m_nIDTemplate;
      Window *m_pParentWnd;
      std::map<HWND, Window*> m_wndMap;

    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTDialog_ClassName"); }
      Dialog(Window *pParent, UINT nIDTemplate);
      Dialog(Window *pParent, LPCTSTR lpszTemplate);
      virtual ~Dialog();

      BOOL DoModaless(HINSTANCE hInstance = NULL);
      UINT DoModal(HINSTANCE hInstance = NULL);
      BOOL EndDialog(UINT nID){ return ::EndDialog(m_hwnd, nID); }

      Window* GetDlgItem(UINT nID);
      GWTString GetDlgItemText(UINT nID);
      UINT GetDlgItemText(UINT nID, LPTSTR lpszText, int cchMax);
      BOOL SetDlgItemText(UINT nID, LPCTSTR lpszText);

    protected:
      virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
      virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
      { return (LRESULT)FALSE; }
      virtual void OnOK(){ EndDialog(IDOK); }
      virtual void OnCancel(){ EndDialog(IDCANCEL); }
      virtual int OnInitDialog();
    };

    BOOL AreDialogMessages(LPMSG lpMsg);

    class GWTApplication
    {
    protected:
      Window     *m_pMainWnd;
      GWTString   m_strCmdLine;
      int         m_nCmdShow;
      HINSTANCE   m_hInstance;

    public:
      GWTApplication(HINSTANCE hInstance, LPSTR lpszCmdLine, int nCmdShow);
      virtual ~GWTApplication();
      virtual int Run();

      HINSTANCE GetInstance(){ return m_hInstance; }
      GWTString GetCommandLine(){ return m_strCmdLine; }
      int GetCommandShow(){ return m_nCmdShow; }
      Window* GetMainWindow(){ return m_pMainWnd; }

    protected:
      virtual BOOL InitInstance(){ return FALSE; }
      virtual void ExitInstance(int nCode){}
    };
  } // namespace Windows

} // namespace GWT

#endif // GWTWND_H
