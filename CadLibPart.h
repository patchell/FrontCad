#pragma once

class CCadLibPart : public CCadObject
{
public:
	CCadLibPart();
	virtual ~CCadLibPart();
	BOOL Create(CCadObject* pParent, SubTypes type);
	virtual void UpdateEnclosure();
	virtual BOOL IsPointEnclosed(DOUBLEPOINT p);
	virtual Token GetDefaultToken() { return Token::LIBPART; }
	//--------------------------------------------
	// Parse (LoaD) and Save Methods
	//--------------------------------------------
	virtual void Parse(
		CParser* pParser,	// pointer to parser
		Token TypeToken = Token::DEFAULT // Token type to save object as
	);
	void ParseLibPartObjects(
		CParser* pParser
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

