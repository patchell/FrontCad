#pragma once

class CCadLibPart : public CCadObject
{
public:
	CCadLibPart();
	virtual ~CCadLibPart();
	BOOL Create(CCadObject* pParent, SubTypes type);
	virtual BOOL IsEnclosedShapeIntrinsic() { return FALSE; }
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual int GetDefaultToken() { return TOKEN_LIBPART; }
	//--------------------------------------------
	// Parse (LoaD) and Save Methods
	//--------------------------------------------
	virtual int Parse(
		CFile* pIn,
		int Token,	// Lookahead Token
		CFileParser* pParser,	// pointer to parser
		int TypeToken = TOKEN_DEFAULT // Token type to save object as
	);
	int ParseLibPartObjects(
		CFile* pcfInFile,
		int LookAHeadToken, 
		CFileParser* pParser
	);
	virtual void Save(
		CFile* pcfFile,
		int Indent,
		int flags
	);
	//--------------------------------------------
	// Copy Object Methods
	//--------------------------------------------
	virtual CCadObject* Copy();
	virtual void CopyAttributes(CCadObject* pToObj);
};

