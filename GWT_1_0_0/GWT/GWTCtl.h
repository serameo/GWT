///////////////////////////////////////////////////////////////////////////////
// GWTCtl.h
// Author: Seree Rakwong
// Date: 21-Dec-2015
//

#ifndef GWTCTL_H
#define GWTCTL_H

#include "GWTWnd.h"

using namespace GWT::Windows;

namespace GWT
{
////////////////////////////////////////////////////////////////////////////
// class Control
// ref: http://msdn.microsoft.com/en-us/library/windows/desktop/bb773169(v=vs.85).aspx
//

  namespace Controls
  {
    class Control : public GWTWindow
    {
    public:
      virtual LPCTSTR GetClassName() = 0;
      BOOL     Create(LPCTSTR lpWindowName,
                      DWORD dwStyle, int x, int y, int width, int height,
                      GWTWindow *pParent, UINT nID,//HWND hwndParent, UINT nID,
                      HINSTANCE hInst = NULL);
      BOOL     CreateEx(DWORD dwExStyle,
                        LPCTSTR lpWindowName,
                        DWORD dwStyle, int x, int y, int width, int height,
                        GWTWindow *pParent, UINT nID,//HWND hwndParent, UINT nID,
                        HINSTANCE hInst = NULL);
      // this is not implemented GetClassName()
      // because we need it to be a abstract class
    protected:
      virtual LRESULT WndProc(UINT message, WPARAM wParam, LPARAM lParam);
    };

    class Button : public Control
    {
    public:
      virtual LPCTSTR GetClassName() { return TEXT("BUTTON"); }
      // button messages mapping
      void Click()
      { SendMessage(BM_CLICK); }
      UINT GetCheck()
      { return (UINT)SendMessage(BM_GETCHECK); }
      HICON GetIcon()
      { return (HICON)SendMessage(BM_GETIMAGE, IMAGE_ICON); }
      HBITMAP GetBitmap()
      { return (HBITMAP)SendMessage(BM_GETIMAGE, IMAGE_BITMAP); }
      UINT GetState()
      { return (UINT)SendMessage(BM_GETSTATE); }
      void SetCheck(UINT fCheck)
      { SendMessage(BM_SETCHECK, (WPARAM)fCheck); }
      void SetDontClick(BOOL fDontClick = TRUE)
      { SendMessage(BM_SETDONTCLICK, (WPARAM)fDontClick); }
      HICON SetIcon(HICON hIcon)
      { return (HICON)SendMessage(BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon); }
      HBITMAP SetBitmap(HBITMAP hbmp)
      { return (HBITMAP)SendMessage(BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbmp); }
      void SetState(BOOL fState = TRUE)
      { SendMessage(BM_SETSTATE, (WPARAM)fState); }
      void SetStyle(DWORD dwStyle, BOOL fRedraw)
      { SendMessage(BM_SETSTYLE, (WPARAM)dwStyle, MAKELPARAM(fRedraw, 0)); }

      BOOL GetIdealSize(SIZE *pSize)
      { return (BOOL)SendMessage(BCM_GETIDEALSIZE, 0, (LPARAM)pSize); }
      BOOL GetImageList(BUTTON_IMAGELIST *pImg)
      { return (BOOL)SendMessage(BCM_GETIMAGELIST, 0, (LPARAM)pImg); }
      BOOL GetNote(LPTSTR pszNote, DWORD *pdwSize)
      { return (BOOL)SendMessage(BCM_GETNOTE, (WPARAM)pdwSize, (LPARAM)pszNote); }
      UINT GetNoteLength()
      { return (UINT)SendMessage(BCM_GETNOTELENGTH); }
      BOOL GetSplitInfo(BUTTON_SPLITINFO *pSplitInfo)
      { return (BOOL)SendMessage(BCM_GETSPLITINFO, 0, (LPARAM)pSplitInfo); }
      BOOL GetTextMargin(RECT *pRect)
      { return (BOOL)SendMessage(BCM_GETTEXTMARGIN, 0, (LPARAM)pRect); }
      BOOL SetDropDownState(BOOL fState = TRUE)
      { return (BOOL)SendMessage(BCM_SETDROPDOWNSTATE, (WPARAM)fState); }
      BOOL SetImageList(BUTTON_IMAGELIST *pImg)
      { return (BOOL)SendMessage(BCM_SETIMAGELIST, 0, (LPARAM)pImg); }
      BOOL SetNote(LPCTSTR pszNote)
      { return (BOOL)SendMessage(BCM_SETNOTE, 0, (LPARAM)pszNote); }
      BOOL SetShield(BOOL fShield)
      { return (BOOL)SendMessage(BCM_SETNOTE, 0, (LPARAM)fShield); }
      BOOL SetSplitInfo(BUTTON_SPLITINFO *pSplitInfo)
      { return (BOOL)SendMessage(BCM_SETSPLITINFO, 0, (LPARAM)pSplitInfo); }
      BOOL SetTextMargin(RECT *pRect)
      { return (BOOL)SendMessage(BCM_SETTEXTMARGIN, 0, (LPARAM)pRect); }

    };

    class Edit : public Control
    {

    public:
      virtual LPCTSTR GetClassName() { return TEXT("EDIT"); }

      // edit messages mapping
      BOOL CanUndo()
      { return Edit_CanUndo(m_hwnd); }
      void EmptyUndoBuffer()
      { Edit_EmptyUndoBuffer(m_hwnd); }
      BOOL Enable(BOOL fEnable = TRUE)
      { return Edit_Enable(m_hwnd, fEnable); }
      void FmtLines(BOOL fAddEOL = TRUE)
      { Edit_FmtLines(m_hwnd, fAddEOL); }
      BOOL GetCueBannerText(LPTSTR lpsz, int cchMax)
      { return Edit_GetCueBannerText(m_hwnd, lpsz, cchMax); }
      int GetFirstVisibleLine()
      { return Edit_GetFirstVisibleLine(m_hwnd); }
      HLOCAL GetHandle()
      { return Edit_GetHandle(m_hwnd); }
      int GetLine(int line, LPTSTR lpsz, int cchMax)
      { return Edit_GetLine(m_hwnd, line, lpsz, cchMax); }
      int GetLineCount()
      { return Edit_GetLineCount(m_hwnd); }
      BOOL GetModify()
      { return Edit_GetModify(m_hwnd); }
      TCHAR GetPasswordChar()
      { return Edit_GetPasswordChar(m_hwnd); }
      void GetRect(RECT *lprc)
      { Edit_GetRect(m_hwnd, lprc); }
      DWORD GetSel()
      { return Edit_GetSel(m_hwnd); }
      int GetText(LPTSTR lpsz, int cchMax)
      { return GetWindowText(lpsz, cchMax); }
      int GetTextLength()
      { return GetWindowTextLength(); }
      EDITWORDBREAKPROC GetWordBreakProc()
      { return Edit_GetWordBreakProc(m_hwnd); }
      BOOL HideBalloonTip()
      { return Edit_HideBalloonTip(m_hwnd); }
      void LimitText(int cchMax)
      { Edit_LimitText(m_hwnd, cchMax); }
      int LineFromChar(int ch)
      { return Edit_LineFromChar(m_hwnd, ch); }
      int LineIndex(int line)
      { return Edit_LineIndex(m_hwnd, line); }
      int LineLength(int line)
      { return Edit_LineLength(m_hwnd, line); }

    #if (_WIN_NT >= 0x0800)
      DWORD NoSetFocus()
      { return Edit_NoSetFocus(m_hwnd); }
      DWORD TakeFocus()
      { return Edit_TakeFocus(m_hwnd); }
    #endif

      void ReplaceSel(LPCTSTR lpszReplace)
      { Edit_ReplaceSel(m_hwnd, lpszReplace); }
      void Scroll(int dv, int dh)
      { Edit_Scroll(m_hwnd, dv, dh); }
      BOOL ScrollCaret()
      { return Edit_ScrollCaret(m_hwnd); }
      BOOL SetCueBannerText(LPCTSTR lpsz)
      { return Edit_SetCueBannerText(m_hwnd, lpsz); }
      BOOL SetCueBannerTextFocused(LPCWSTR lpcwText, BOOL fDrawFocused)
      { return Edit_SetCueBannerTextFocused(m_hwnd, lpcwText, fDrawFocused); }
      void SetHandle(HLOCAL hLocal)
      { Edit_SetHandle(m_hwnd, hLocal); }
      void SetModify(BOOL fModified = TRUE)
      { Edit_SetModify(m_hwnd, fModified); }
      void SetPasswordChar(TCHAR ch)
      { Edit_SetPasswordChar(m_hwnd, ch); }
      BOOL SetReadOnly(BOOL fReadOnly = TRUE)
      { return Edit_SetReadOnly(m_hwnd, fReadOnly); }
      void SetRect(RECT *lprc)
      { Edit_SetRect(m_hwnd, lprc); }
      void SetRectNoPaint(RECT *lprc)
      { Edit_SetRectNoPaint(m_hwnd, lprc); }
      void SetSel(int start = 0, int end = -1)
      { Edit_SetSel(m_hwnd, start, end); }
      void SetTabStops(int cTabs, int *pTabs)
      { Edit_SetTabStops(m_hwnd, cTabs, pTabs); }
      int SetText(LPCTSTR lpsz)
      { return SetWindowText(lpsz); }
      void SetWordBreakProc(EDITWORDBREAKPROC lpfnWordBreak)
      { Edit_SetWordBreakProc(m_hwnd, lpfnWordBreak); }
      BOOL ShowBalloonTip(PEDITBALLOONTIP peditballoontip)
      { return Edit_ShowBalloonTip(m_hwnd, peditballoontip); }
      BOOL Undo()
      { return Edit_Undo(m_hwnd); }

    };

    class ListBox : public Control
    {
    public:
      virtual LPCTSTR GetClassName() 
      { 
        static LPCTSTR s_pszClassName = TEXT("LISTBOX"); 
        return s_pszClassName; 
      }

      void DrawInsert(int nItem)
      { return ::DrawInsert(GetParent()->GetHWND(), m_hwnd, nItem); }
      DWORD GetListBoxInfo()
      { return ::GetListBoxInfo(m_hwnd); }
      int LBItemFromPt(POINT pt, BOOL bAutoScroll)
      { return ::LBItemFromPt(m_hwnd, pt, bAutoScroll); }
      BOOL MakeDragList()
      { return ::MakeDragList(m_hwnd); }

      int AddFile(LPCTSTR lpFileName)
      { return SendMessage(LB_ADDFILE, 0, (LPARAM)lpFileName); }
      int AddString(LPCTSTR lpString)
      { return SendMessage(LB_ADDSTRING, 0, (LPARAM)lpString); }
      int DeleteString(int index)
      { return SendMessage(LB_DELETESTRING, (WPARAM)index); }
      int Dir(UINT flags, LPCTSTR lpPath)
      { return SendMessage(LB_DIR, (WPARAM)flags, (LPARAM)lpPath); }
      int FindString(int index, LPCTSTR lpString)
      { return SendMessage(LB_FINDSTRING, (WPARAM)index, (LPARAM)lpString); }
      int FindStringExact(int index, LPCTSTR lpString)
      { return SendMessage(LB_FINDSTRINGEXACT, (WPARAM)index, (LPARAM)lpString); }
      int GetAnchorIndex()
      { return SendMessage(LB_GETANCHORINDEX); }
      int GetCaretIndex()
      { return SendMessage(LB_GETCARETINDEX); }
      int GetCount()
      { return SendMessage(LB_GETCOUNT); }
      int GetCurSel()
      { return SendMessage(LB_GETCURSEL); }
      int GetHorizontalExtent()
      { return SendMessage(LB_GETHORIZONTALEXTENT); }
      DWORD GetItemData(int index)
      { return SendMessage(LB_GETITEMDATA, (WPARAM)index); }
      int GetItemHeight(int index)
      { return SendMessage(LB_GETITEMHEIGHT, (WPARAM)index); }
      int GetItemRect(int index, RECT *pRect)
      { return SendMessage(LB_GETITEMRECT, (WPARAM)index, (LPARAM)pRect); }
    //  int GetListBoxInfo()
    //  { return SendMessage(LB_GETLISTBOXINFO); }
      int GetLocale()
      { return SendMessage(LB_GETLOCALE); }
      int GetSel(int index)
      { return SendMessage(LB_GETSEL, (WPARAM)index); }
      int GetSelCount()
      { return SendMessage(LB_GETSELCOUNT); }
      int GetSelItems(int *pIndexes, int cMax)
      { return SendMessage(LB_GETSELITEMS, (WPARAM)cMax, (LPARAM)pIndexes); }
      int GetText(int index, LPTSTR lpString)
      { return SendMessage(LB_GETTEXT, (WPARAM)index, (LPARAM)lpString); }
      int GetTextLen(int index)
      { return SendMessage(LB_GETTEXTLEN, (WPARAM)index); }
      int GetTopIndex()
      { return SendMessage(LB_GETTOPINDEX); }
      int InitStorage(int number, DWORD dwSize)
      { return SendMessage(LB_INITSTORAGE, (WPARAM)number, (LPARAM)dwSize); }
      int InsertString(int index, LPCTSTR lpString)
      { return SendMessage(LB_INSERTSTRING, (WPARAM)index, (LPARAM)lpString); }
      int ItemFromPoint(int x, int y)
      { return SendMessage(LB_ITEMFROMPOINT, 0, MAKELPARAM(x, y)); }
      void ResetContent()
      { SendMessage(LB_RESETCONTENT); }
      int SelectString(int index, LPCTSTR lpString)
      { return SendMessage(LB_SELECTSTRING, (WPARAM)index, (LPARAM)lpString); }
      int SelItemRange(int start, int end, BOOL fSelected = TRUE)
      { return SendMessage(LB_SELITEMRANGE, (WPARAM)fSelected, MAKELPARAM(start, end)); }
      int SelItemRangeEx(int start, int end)
      { return SendMessage(LB_SELITEMRANGEEX, (WPARAM)start, (LPARAM)end); }
      int SetAnchorIndex(int index)
      { return SendMessage(LB_SETANCHORINDEX, (WPARAM)index); }
      int SetCaretIndex(int index, BOOL fScroll = TRUE)
      { return SendMessage(LB_SETCARETINDEX, (WPARAM)index, (LPARAM)fScroll); }
      void SetColumnWidth(int width)
      { SendMessage(LB_SETCOLUMNWIDTH, (WPARAM)width); }
      int SetCount(int count)
      { return SendMessage(LB_SETCOUNT, (WPARAM)count); }
      int SetCurSel(int index)
      { return SendMessage(LB_SETCURSEL, (WPARAM)index); }
      void SetHorizontalExtent(int horz)
      { SendMessage(LB_SETHORIZONTALEXTENT, (WPARAM)horz); }
      int SetItemData(int index, DWORD data)
      { return SendMessage(LB_SETITEMDATA, (WPARAM)index, (LPARAM)data); }
      int SetItemHeight(int index, int height)
      { return SendMessage(LB_SETITEMHEIGHT, (WPARAM)index, (LPARAM)height); }
      int SetLocale(int locale)
      { return SendMessage(LB_SETLOCALE, (WPARAM)locale); }
      int SetSel(int index, BOOL fSelect = TRUE)
      { return SendMessage(LB_SETSEL, (WPARAM)fSelect, (LPARAM)index); }
      BOOL SetTabStops(int *pTabs, int number)
      { return (BOOL)SendMessage(LB_SETTABSTOPS, (WPARAM)number, (LPARAM)pTabs); }
      int SetTopIndex(int index)
      { return SendMessage(LB_SETTOPINDEX, (WPARAM)index); }

    };

    class ComboBox : public Control
    {
    public:
      virtual LPCTSTR GetClassName() { return TEXT("COMBOBOX"); }

      // combo box messages mapping
      int AddItemData(LPARAM data)
      { return ComboBox_AddItemData(m_hwnd, data); }
      int AddString(LPCTSTR lpsz)
      { return ComboBox_AddString(m_hwnd, lpsz); }
      int DeleteString(int index)
      { return ComboBox_DeleteString(m_hwnd, index); }
      int Dir(UINT attrs, LPCTSTR lpszFileSpec)
      { return ComboBox_Dir(m_hwnd, attrs, lpszFileSpec); }
      BOOL Enable(BOOL fEnable = TRUE)
      { return ComboBox_Enable(m_hwnd, fEnable); }
      int FindItemData(LPARAM data, int indexStart = -1)
      { return ComboBox_FindItemData(m_hwnd, indexStart, data); }
      int FindString(LPCTSTR lpsz, int indexStart = -1)
      { return ComboBox_FindString(m_hwnd, indexStart, lpsz); }
      int FindStringExact(LPCTSTR lpsz, int indexStart = -1)
      { return ComboBox_FindStringExact(m_hwnd, indexStart, lpsz); }
      int GetCount()
      { return ComboBox_GetCount(m_hwnd); }
      BOOL GetCueBannerText(LPTSTR lpsz, int cchText)
      { return (BOOL)ComboBox_GetCueBannerText(m_hwnd, lpsz, cchText); }
      int GetCurSel()
      { return ComboBox_GetCurSel(m_hwnd); }
      void GetDroppedControlRect(RECT *pRect)
      { return ComboBox_GetDroppedControlRect(m_hwnd, pRect); }
      BOOL GetDroppedState()
      { return ComboBox_GetDroppedState(m_hwnd); }
      UINT GetExtendedUI()
      { return ComboBox_GetExtendedUI(m_hwnd); }
      LRESULT GetItemData(int index)
      { return ComboBox_GetItemData(m_hwnd, index); }
      int GetItemHeight()
      { return ComboBox_GetItemHeight(m_hwnd); }
      int GetLBText(int index, LPTSTR lpszBuffer)
      { return ComboBox_GetLBText(m_hwnd, index, lpszBuffer); }
      int GetLBTextLen(int index)
      { return ComboBox_GetLBTextLen(m_hwnd, index); }
      int GetMinVisible()
      { return ComboBox_GetMinVisible(m_hwnd); }
      int GetText(LPTSTR lpsz, int cchMax)
      //{ return ComboBox_GetText(m_hwnd, lpsz, cchMax); }
      { return GetWindowText(lpsz, cchMax); }
      int GetTextLength()
      //{ return ComboBox_GetTextLength(m_hwnd); }
      { return GetWindowTextLength(); }
      int InsertItemData(LPARAM data, int index = -1)
      { return ComboBox_InsertItemData(m_hwnd, index, data); }
      int InsertString(LPCTSTR lpsz, int index = -1)
      { return ComboBox_InsertString(m_hwnd, index, lpsz); }
      int LimitText(int cchMax)
      { return ComboBox_LimitText(m_hwnd, cchMax); }
      int ResetContent()
      { return ComboBox_ResetContent(m_hwnd); }
      int SelectItemData(LPARAM data, int indexStart = -1)
      { return ComboBox_SelectItemData(m_hwnd, indexStart, data); }
      int SelectString(LPCTSTR lpsz, int indexStart = -1)
      { return ComboBox_SelectString(m_hwnd, indexStart, lpsz); }
      BOOL SetCueBannerText(LPCTSTR lpsz)
      { return ComboBox_SetCueBannerText(m_hwnd, lpsz); }
      int SetCurSel(int index)
      { return ComboBox_SetCurSel(m_hwnd, index); }
      int SetExtendedUI(UINT flags)
      { return ComboBox_SetExtendedUI(m_hwnd, flags); }
      int SetItemData(int index, LPARAM data)
      { return ComboBox_SetItemData(m_hwnd, index, data); }
      int SetItemHeight(int index, int cyItem)
      { return ComboBox_SetItemHeight(m_hwnd, index, cyItem); }
      BOOL SetMinVisible(int iMinVisible)
      { return ComboBox_SetMinVisible(m_hwnd, iMinVisible); }
      int SetText(LPCTSTR lpsz)
      { return SetWindowText(lpsz); }
      BOOL ShowDropdown(BOOL fShow = TRUE)
      { return ComboBox_ShowDropdown(m_hwnd, fShow); }

    };

    class Static : public Control
    {
    public:
      virtual LPCTSTR GetClassName() { return TEXT("STATIC"); }
  
      HBITMAP GetBitmap()
      { return (HBITMAP)SendMessage(STM_GETIMAGE, IMAGE_BITMAP); }
      HCURSOR GetCursor()
      { return (HCURSOR)SendMessage(STM_GETIMAGE, IMAGE_CURSOR); }
      HMETAFILE GetMetaFile()
      { return (HMETAFILE)SendMessage(STM_GETIMAGE, IMAGE_ENHMETAFILE); }
      HICON GetIcon()
      { return (HICON)SendMessage(STM_GETIMAGE, IMAGE_ICON); }
      HBITMAP SetBitmap(HBITMAP hbmp)
      { return (HBITMAP)SendMessage(STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hbmp); }
      HCURSOR SetCursor(HCURSOR hCursor)
      { return (HCURSOR)SendMessage(STM_SETIMAGE, IMAGE_CURSOR, (LPARAM)hCursor); }
      HMETAFILE SetMetaFile(HMETAFILE hbmp)
      { return (HMETAFILE)SendMessage(STM_SETIMAGE, IMAGE_ENHMETAFILE, (LPARAM)hbmp); }
      HICON SetIcon(HICON hIcon)
      { return (HICON)SendMessage(STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon); }
    };

    class Animate : public Control
    {
    public:
      virtual LPCTSTR GetClassName(){ return ANIMATE_CLASS; }

    public:
      BOOL Create(DWORD dwStyle, HWND hwndParent, UINT nID, HINSTANCE hInst);
      BOOL Close()
      { return Animate_Close(m_hwnd); }
      BOOL IsPlaying()
      { return Animate_IsPlaying(m_hwnd); }
      BOOL Open(LPCTSTR lpszName)
      { return Animate_Open(m_hwnd, lpszName); }
      BOOL Open(LPCTSTR lpszName, HINSTANCE hInst)
      { return Animate_OpenEx(m_hwnd, hInst, lpszName); }
      BOOL Play(UINT wFrom, UINT wTo, UINT cRepeat)
      { return Animate_Play(m_hwnd, wFrom, wTo, cRepeat); }
      BOOL Seek(UINT wFrame)
      { return Animate_Seek(m_hwnd, wFrame); }
      BOOL Stop()
      { return Animate_Stop(m_hwnd); }

    };

    class ScrollBar : public Control
    {

    public:
      virtual LPCTSTR GetClassName(){ return TEXT("SCROLLBAR"); }

    public:
      BOOL EnableArrows(UINT flag)
      { return SendMessage(SBM_ENABLE_ARROWS, flag); }
      int GetPos()
      { return SendMessage(SBM_GETPOS); }
      void GetRange(int *pmin, int *pmax)
      { SendMessage(SBM_GETRANGE, (WPARAM)pmin, (LPARAM)pmax); }
      int GetScrollBarInfo(SCROLLBARINFO *ps)
      { return SendMessage(SBM_GETSCROLLBARINFO, 0, (LPARAM)ps); }
      BOOL GetScrollInfo(SCROLLINFO *ps)
      { return SendMessage(SBM_GETSCROLLINFO, (LPARAM)ps); }
      int SetPos(int pos, BOOL fRedraw = TRUE)
      { return SendMessage(SBM_GETSCROLLINFO, pos, fRedraw); }
      int SetRange(int min, int max)
      { return SendMessage(SBM_SETRANGE, min, max); }
      int SetRangeRedraw(int min, int max)
      { return SendMessage(SBM_SETRANGEREDRAW, min, max); }
      int SetScrollInfo(UINT flags, BOOL fRedraw = TRUE)
      { return SendMessage(SBM_SETSCROLLINFO, fRedraw, flags); }

    };

  } // namespace Controls
} // namespace GWT

#endif // GWTCTL_H
