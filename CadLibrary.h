#pragma once

class CCadLibrary : public CCadObject
{
public:
	CCadLibrary();
	virtual ~CCadLibrary();
	BOOL Create();
	virtual CLexer::Tokens GetDefaultToken() { return CLexer::Tokens::LIBFILE; }
	virtual CLexer::Tokens Parse(CLexer::Tokens LookAHeadToken, CFileParser* pParser);
	virtual void Save(FILE* pO, int Indent);
};

