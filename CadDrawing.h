// CadDrawing.h: interface for the CCadDrawing class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define CADDRAWUBG_CHECKSEL_FLAG_ALL	0
#define CADDRAWUBG_CHECKSEL_FLAG_UNSEL	1
#define CADDRAWUBG_CHECKSEL_FLAG_SEL	2


class CFileParser;
class CCadPrintRect;

class CCadDrawing : public CCadObject  
{
	friend CFileParser;
	COLORREF m_BkColor;
public:
	CCadDrawing();
	virtual ~CCadDrawing();
	virtual BOOL Create(
		CCadObject* pParent, 
		SubTypes type
	);
	CCadLibPart * CreatePartFromSelected(CString& csName);
	virtual int GetDefaultToken() { return TOKEN_DRAWFILE; }
	int CheckSelected(DOUBLEPOINT p,CCadObject **ppSelList,int n,int flag= CADDRAWUBG_CHECKSEL_FLAG_ALL);
	void Print(CDC *pDC,MODE mode,DOUBLEPOINT ULHC,CScale Scale);
	virtual BOOL IsEnclosedShapeIntrinsic() { return FALSE; }
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual void Draw(CDC *pDC,MODE mode, DOUBLEPOINT ULHC,CScale Scale);
	int DrawObjects(
		CFile* pcfFile,
		int LookAHeadToken, 
		CFileParser* pParser, 
		CCadObject* pParent
	);
	void SetBkColor(COLORREF c){m_BkColor = c;}
	COLORREF& GetBkColor(){return m_BkColor;}
	CCadPrintRect *GetPrintRect();
	//--------------------------------------------
	// Parse (LoaD) and Save Methods
	//--------------------------------------------
	virtual int Parse(
		CFile* pcfInFile,
		int Token,	// Lookahead Token
		CFileParser* pParser,	// pointer to parser
		int TypeToken = TOKEN_DEFAULT // Token type to save object as
	);
	virtual void Save(
		CFile* cfFile,
		int Indent,
		int flags
	);
	//--------------------------------------------
	// Copy Object Methods
	//--------------------------------------------
	virtual CCadObject* Copy();
	virtual void CopyAttributes(CCadObject* pToObj);
};
