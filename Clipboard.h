#pragma once

class CClipboard : public CCadObject
{
public:
	CClipboard();
	virtual ~CClipboard();
	virtual void UpdateEnclosure();
	void RemoveAndDestroyALL();
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p) {
		return FALSE;
	}
	//--------------------------------------------
	// Copy Object Methods
	//--------------------------------------------
	virtual CCadObject* Copy() { return NULL; }
	virtual void CopyAttributes(CCadObject* pToObj) {}
};

