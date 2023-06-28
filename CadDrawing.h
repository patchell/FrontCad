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
	virtual CLexer::Tokens GetDefaultToken() { return CLexer::Tokens::DRAWFILE; }
	int CheckSelected(DOUBLEPOINT p,CCadObject **ppSelList,int n,int flag= CADDRAWUBG_CHECKSEL_FLAG_ALL);
	void Print(CDC *pDC,MODE mode,DOUBLEPOINT ULHC,CScale Scale);
	virtual CLexer::Tokens Parse(CLexer::Tokens LookAHeadToken, CFileParser* pParser);
	virtual void Save(FILE *pO,  int Indent);
	virtual void Draw(CDC *pDC,MODE mode, DOUBLEPOINT ULHC,CScale Scale);
	CLexer::Tokens DrawObjects(CLexer::Tokens LookAHeadToken, CFileParser* pParser, CCadObject* pParent);
	void SetBkColor(COLORREF c){m_BkColor = c;}
	COLORREF& GetBkColor(void){return m_BkColor;}
	CCadPrintRect *GetPrintRect();
};
