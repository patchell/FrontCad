#pragma once

class CCadLibPart : public CCadObject
{
public:
	CCadLibPart();
	virtual ~CCadLibPart();
	BOOL Create(CCadObject* pParent, SubTypes type);
	virtual CLexer::Tokens GetDefaultToken() { return CLexer::Tokens::LIBPART; }
	virtual CLexer::Tokens Parse(CLexer::Tokens LookAHeadToken, CFileParser* pParser);
	virtual CLexer::Tokens ParseLibPartObjects(
		CLexer::Tokens LookAHeadToken,
		CFileParser* pParser
	);
	virtual void Save(FILE* pO, int Indent);
};

