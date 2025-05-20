#pragma once

class CCadLibrary : public CCadObject
{
public:
	CCadLibrary();
	virtual ~CCadLibrary();
	BOOL Create();
	virtual void UpdateEnclosure();
	virtual Token GetDefaultToken() { return Token::LIBFILE; }
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	//--------------------------------------------
	// Parse (LoaD) and Save Methods
	//--------------------------------------------
	virtual void Parse(
		CParser* pParser,	// pointer to parser
		Token TypeToken = Token::DEFAULT // Token type to save object as
	);
	virtual void Save(
		CFile* pcfFile,
		int Indent
	);
	//--------------------------------------------
	// Copy Object Methods
	//--------------------------------------------
	virtual CCadObject* Copy();
	virtual void CopyAttributes(CCadObject* pToObj);
};

