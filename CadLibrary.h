#pragma once

class CCadLibrary : public CCadObject
{
public:
	CCadLibrary();
	virtual ~CCadLibrary();
	BOOL Create();
	virtual int GetDefaultToken() { return TOKEN_LIBFILE; }
	virtual BOOL IsEnclosedShapeIntrinsic() { return FALSE; }
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
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

