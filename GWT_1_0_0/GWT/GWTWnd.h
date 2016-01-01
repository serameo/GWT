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

    BOOL AreDialogMessages(LPMSG lpMsg);

    class GWTDialog;
    class GWTMDIFrameWindow;
    class GWTMDIChildWindow;
    class GWTWindowImpl : public TWindow<GWTWindowImpl>
    {
      friend class GWTDialog;
      friend class GWTMDIFrameWindow;
      friend class GWTMDIChildWindow;
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTWindowImpl_ClassName"); }
      static BOOL CALLBACK StaticDlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
      static LRESULT CALLBACK StaticMDIChildProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
      static LRESULT CALLBACK StaticMDIFrameProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    protected:
      virtual BOOL    RegisterWnd(HINSTANCE);
    };

    class GWTWindow : public GWTWindowImpl
    {
    protected:
        GWTWindow *m_pParentWnd;
    public:
        GWTWindow():m_pParentWnd(NULL){}
        GWTWindow *GetParent(){ return m_pParentWnd; }

          BOOL     Create(LPCTSTR lpWindowName,
                          DWORD dwStyle, int x, int y, int width, int height,
                          GWTWindow *pParent, HMENU hMenu,
                          HINSTANCE hInst);
          BOOL     CreateEx(DWORD dwExStyle,
                            LPCTSTR lpWindowName,
                            DWORD dwStyle, int x, int y, int width, int height,
                            GWTWindow *pParent, HMENU hMenu,
                            HINSTANCE hInst);

      virtual LPCTSTR GetClassName(){ return TEXT("GWTGWTWindow_ClassName"); }
      static  GWTWindow* FromHandle(HWND hwnd);
      virtual BOOL IsTranslateAccelerator(LPMSG lpMsg){ return FALSE; }
    };

    class GWTFrameWindow : public GWTWindow
    {
    protected:
        UINT m_nIDResource;
        HACCEL m_hAccel;
        HMENU m_hMenu;
    public:
        GWTFrameWindow(UINT nIDResource = 0)
            : m_nIDResource(nIDResource)
            , m_hAccel(NULL)
            , m_hMenu(NULL)
        {}
        virtual ~GWTFrameWindow();
      virtual LPCTSTR GetClassName(){ return TEXT("GWTFrameWindow_ClassName"); }

    protected:
        //virtual BOOL RegisterWnd(HINSTANCE hInstance);
        virtual BOOL IsTranslateAccelerator(LPMSG lpMsg);
        virtual int PreCreateWindow(LPCREATESTRUCT lpcs);
    };

    class GWTView : public GWTWindow
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTGWTView_ClassName"); }
    };

    class GWTSplitView : public GWTView
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTSplitView_ClassName"); }
    };

    class GWTScrollView : public GWTView
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTScrollView_ClassName"); }
    };

    class GWTFormView : public GWTScrollView
    {
    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTFormView_ClassName"); }
    };

    //
    //ref: https://www-user.tu-chemnitz.de/~ygu/petzold/ch19c.htm
    //
    class GWTMDIChildWindow;
    class GWTMDIFrameWindow : public GWTFrameWindow
    {
    protected:
        class GWTMDIClient : public GWTView
        {
        protected:
            std::map<HWND, GWTMDIChildWindow*> m_children;

        public:
            virtual LPCTSTR GetClassName(){ return TEXT("MDICLIENT"); }
            virtual BOOL IsTranslateAccelerator(LPMSG lpMsg)
            { return ::TranslateMDISysAccel(m_hwnd, lpMsg); }
            static const UINT ID = 0XCAC;
            void RegisterChild(HWND hwndChild, GWTMDIChildWindow* pChild);
            static const UINT GetClientID() { return ID; }


            GWTMDIChildWindow *MDIGetActive();
            void MDITile(){ SendMessage(WM_MDITILE); }
            void MDICascade(){ SendMessage(WM_MDICASCADE); }
            void MDIIconArrange(){ SendMessage(WM_MDIICONARRANGE); }
            void MDIRestore(GWTMDIChildWindow *pChild);
            void MDIDestroy(GWTMDIChildWindow *pChild);
            void MDISetMenu(HMENU hMenu, HMENU hSubWindowMenu)
            { SendMessage(WM_MDISETMENU, (WPARAM)hMenu, (LPARAM)hSubWindowMenu); }
        };

    protected:
        GWTMDIClient *m_pMDIClient;
        UINT m_nIDWindowMenu;
        UINT m_nIDFirstChild;

        GWTMDIFrameWindow();
        GWTMDIFrameWindow(const GWTMDIFrameWindow&);
    public:
        GWTMDIFrameWindow(UINT nIDResource, UINT nIDWindowMenu, UINT nIDFirstChild = 10000)
            : GWTFrameWindow(nIDResource)
            , m_nIDWindowMenu(nIDWindowMenu)
            , m_nIDFirstChild(nIDFirstChild)
        {
            m_pMDIClient = new GWTMDIClient();
        }
        virtual ~GWTMDIFrameWindow()
        {
            if (m_pMDIClient)
            {
                m_pMDIClient->Destroy();
                delete m_pMDIClient;
            }
        }
        virtual BOOL RegisterWnd(HINSTANCE hInstance);
        virtual LPCTSTR GetClassName(){ return TEXT("GWTMDIFrameWindow_ClassName"); }
        GWTMDIClient *GetMDIClient() const { return m_pMDIClient; }
        virtual BOOL IsTranslateAccelerator(LPMSG lpMsg)
        { 
            return (GWTFrameWindow::IsTranslateAccelerator(lpMsg) || m_pMDIClient->IsTranslateAccelerator(lpMsg));
        }
        UINT GetWindowMenuPosition() const { return m_nIDWindowMenu; }
        UINT GetFirstWindowMenuID() const { return m_nIDFirstChild; }
        BOOL CreateMDIWindow(GWTMDIChildWindow **ppChild, 
            LPCTSTR lpWindowName,
            DWORD dwStyle = MDIS_ALLCHILDSTYLES,
            int x = CW_USEDEFAULT, 
            int y = CW_USEDEFAULT, 
            int w = CW_USEDEFAULT, 
            int h = CW_USEDEFAULT,
            HINSTANCE hInstance = NULL);

    protected:
        virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        virtual int OnCreate(LPCREATESTRUCT lpcs);
    };

    class GWTMDIChildWindow : public GWTFrameWindow
    {
    protected:
        GWTMDIFrameWindow *m_pMDIFrameWindow;
        UINT m_nWindowMenuPos;

        GWTMDIChildWindow();
        GWTMDIChildWindow(const GWTMDIChildWindow&);
    public:
        GWTMDIChildWindow(GWTMDIFrameWindow *pMDIFrameWindow, UINT nIDResource, UINT nWindowMenuPos);
      virtual LPCTSTR GetClassName(){ return TEXT("GWTMDIChildWindow_ClassName"); }
      GWTMDIFrameWindow *GetFrame(){ return m_pMDIFrameWindow; }
      HMENU GetSubWindowMenu()
      { return GetSubMenu(GetMenu(), m_nWindowMenuPos); }

    protected:
        virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        virtual BOOL RegisterWnd(HINSTANCE hInstance);
        virtual void OnMDIActivate();
    };
    //
    // resource: https://msdn.microsoft.com/en-us/library/GWTWindows/desktop/aa381002(v=vs.85).aspx
    // ref: https://msdn.microsoft.com/en-us/library/GWTWindows/desktop/aa381003(v=vs.85).aspx
    class GWTDialog : public GWTWindow
    {

    protected:
      LPCTSTR m_pszTemplate;
      UINT m_nIDTemplate;
      GWTWindow *m_pParentWnd;
      std::map<HWND, GWTWindow*> m_wndMap;

    public:
      virtual LPCTSTR GetClassName(){ return TEXT("GWTGWTDialog_ClassName"); }
      GWTDialog(GWTWindow *pParent, UINT nIDTemplate);
      GWTDialog(GWTWindow *pParent, LPCTSTR lpszTemplate);
      virtual ~GWTDialog();

      BOOL DoModaless(HINSTANCE hInstance = NULL);
      UINT DoModal(HINSTANCE hInstance = NULL);
      BOOL EndDialog(UINT nID){ return ::EndDialog(m_hwnd, nID); }

      GWTWindow* GetDlgItem(UINT nID);
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

    class GWTApplication
    {
    protected:
      GWTWindow     *m_pMainWnd;
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
      GWTWindow* GetMainWindow(){ return m_pMainWnd; }

    protected:
      virtual BOOL InitInstance(){ return FALSE; }
      virtual void ExitInstance(int nCode){}
    };
  } // namespace GWTWindows

} // namespace GWT

#endif // GWTWND_H
