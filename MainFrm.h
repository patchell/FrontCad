
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
protected:  // control bar embedded members
	CToolBar        m_wndToolBar;
	CStatusBar      m_wndStatusBar;
public:
	CMainFrame() noexcept;

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
	void SetStatusText(CString csStatusText) { m_wndStatusBar.SetWindowTextW(csStatusText); }
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// Generated message map Methods
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

	afx_msg LRESULT OnMainframeFinishupdate(WPARAM wParam, LPARAM lParam);
};


