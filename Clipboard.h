#pragma once

class CClipboard : public CCadObject
{
public:
	CClipboard();
	~CClipboard();
	void RemoveAndDestroyALL();
	virtual BOOL IsEnclosedShapeIntrinsic() { return FALSE; }
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p) {
		return FALSE;
	}
	//--------------------------------------------
	// Copy Object Methods
	//--------------------------------------------
	virtual CCadObject* Copy() { return NULL; }
	virtual void CopyAttributes(CCadObject* pToObj) {}
};

