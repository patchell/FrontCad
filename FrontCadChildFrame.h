#pragma once

// CFrontCadChildFrame frame

class CFrontCadChildFrame : public CMDIChildWnd
{
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
	void ShowRulers(int nShow);
};


