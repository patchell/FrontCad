#pragma once

// CFrontCadChildFrame frame

class CFrontCadChildFrame : public CMDIChildWnd
{
	CMyToolBarSplitter m_ToolBarSplitter;
	CRulerSplitterWnd m_RulerSplitter;
	DECLARE_DYNCREATE(CFrontCadChildFrame)
protected:
	CFrontCadChildFrame();           // protected constructor used by dynamic creation
	virtual ~CFrontCadChildFrame();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	DECLARE_MESSAGE_MAP()
public:
	//------------------------------------------
	// Ruler View Methods
	//------------------------------------------
	void ShowRulers(int nShow);
	CRulerSplitterWnd* GetRulerSpliterWnd() { 
		CRulerSplitterWnd* pRSW;

		pRSW = &m_RulerSplitter;
		return pRSW; 
	}
	//------------------------------------------
	// Tool Bar Methods
	//------------------------------------------
	CMyToolBarSplitter* GetToolBarSplitterWnd() {
		CMyToolBarSplitter* pTBSW;
		pTBSW = &m_ToolBarSplitter;
		return pTBSW;
	}
	CMyToolBarView* GetToolBarView() {
		CMyToolBarView* pMTBV;
		pMTBV = (CMyToolBarView*)GetToolBarSplitterWnd()->GetPane(TOOLBAR_PANE);
		return pMTBV;
	}
	void SetToolbarDebug(CString& csString)
	{
		GetToolBarView()->SetDebug(csString);
	}
	void ToolBarSetPosition(CDoublePoint pos)
	{
		GetToolBarView()->SetToolBarPosition(pos);
	}
	void InitToolBar(CWnd* pWnd, CRulerInfo *pRI);
	void ShowToolBar(int nShow);
};


