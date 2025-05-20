// CadDrawing.h: interface for the CCadDrawing class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#define CADDRAWUBG_CHECKSEL_FLAG_ALL	0
#define CADDRAWUBG_CHECKSEL_FLAG_UNSEL	1
#define CADDRAWUBG_CHECKSEL_FLAG_SEL	2


class CParser;
class CCadPrintRect;

class CCadDrawing : public CCadObject  
{
	friend class CParser;
	COLORREF m_BkColor;
public:
	CCadDrawing();
	virtual ~CCadDrawing();
	virtual BOOL Create(
		CCadObject* pParent, 
		SubTypes type
	);
	virtual void UpdateEnclosure();
	CCadLibPart * CreatePartFromSelected(CString& csName);
	virtual Token GetDefaultToken() { return Token::DRAWFILE; }
	int CheckSelected(DOUBLEPOINT p,CCadObject **ppSelList,int n,int flag= CADDRAWUBG_CHECKSEL_FLAG_ALL);
	void Print(CDC *pDC,MODE mode,DOUBLEPOINT ULHC,CScale Scale);
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual void Draw(CDC *pDC,MODE mode, DOUBLEPOINT ULHC,CScale Scale);
	void ParseDrawObjects(
		CParser* pParser,
		CCadObject* pParent
	);
	void SetBkColor(COLORREF c){m_BkColor = c;}
	COLORREF& GetBkColor(){return m_BkColor;}
	CCadPrintRect *GetPrintRect();
	//--------------------------------------------
	// Parse (LoaD) and Save Methods
	//--------------------------------------------
	virtual void Parse(
		CParser* pParser,	// pointer to parser
		Token TypeToken = Token::DEFAULT // Token type to save object as
	);
	virtual void Save(
		CFile* cfFile,
		int Indent
	);
	//--------------------------------------------
	// Copy Object Methods
	//--------------------------------------------
	virtual CCadObject* Copy();
	virtual void CopyAttributes(CCadObject* pToObj);
};
