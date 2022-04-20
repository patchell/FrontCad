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
	CRulerSplitterWnd* GetRulerSpliterWnd() { 
		CRulerSplitterWnd* pRSW;

		pRSW = &m_RulerSplitter;
		return pRSW; 
	}
	CMyToolBarSplitter* GetToolBarSplitterWnd() {
		CMyToolBarSplitter* pTBSW;
		pTBSW = &m_ToolBarSplitter;
		return pTBSW;
	}
	void InitToolBar();
	void ShowRulers(int nShow);
	void ShowToolBar(int nShow);
};


