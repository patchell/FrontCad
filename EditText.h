#pragma once


// CEditText

class CEditText : public CEdit
{
	BOOL m_bDirty;
	DECLARE_DYNAMIC(CEditText)
public:
	CEditText() {
		m_bDirty = FALSE;

	}
	virtual ~CEditText() {};
	afx_msg void OnEnChange();
	BOOL IsDirty() { return m_bDirty; }
protected:
	DECLARE_MESSAGE_MAP()
};


