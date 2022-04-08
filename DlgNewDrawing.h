#pragma once


// CDlgNewDrawing dialog

class CDlgNewDrawing : public CDialogEx
{
	CString m_csDocName;
	double m_PaperSizeX;
	double m_PaperSizeY;
	double m_DrawingSizeX;
	double m_DrawingSizeY;
	CEditDecimal m_Edit_DrawingSizeH;
	CEditDecimal m_Edit_DrawingSizeW;
	CEditDecimal m_Edit_PaperSizeH;
	CEditDecimal m_Edit_PaperSizeW;
	CEdit m_Edit_DrawingName;
	DECLARE_DYNAMIC(CDlgNewDrawing)

public:
	CDlgNewDrawing(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgNewDrawing();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnBnClickedButtonOpenExisting();
	void SetDialogTitle(CString csTitle){}
	void SetDocumentName(CString& csDocName) { m_csDocName = csDocName; }
	CString& GetDocumentName() { return m_csDocName; }
	void SetPaperSizeX(double sz) { m_PaperSizeX = sz; }
	double GetPaperSizeX() { return m_PaperSizeX; }
	void SetPaperSizeY(double sz) { m_PaperSizeY = sz; }
	double GetPaperSizeY() { return m_PaperSizeY; }
	void SetDrawingSizeX(double sz) { m_DrawingSizeX = sz; }
	double GetDrawingSizeX() { return m_DrawingSizeX; }
	void SetDrawingSizeY(double sz) { m_DrawingSizeY = sz; }
	double GetDrawingSizeY() { return m_DrawingSizeY; }
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_NEW_DRAWING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
};
