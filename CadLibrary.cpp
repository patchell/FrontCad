#include "pch.h"

CCadLibrary::CCadLibrary():CCadObject(ObjectType::LIBRARY)
{
}

CCadLibrary::~CCadLibrary()
{
}

BOOL CCadLibrary::Create()
{
	return 0;
}

CLexer::Tokens CCadLibrary::Parse(CLexer::Tokens LookAHeadToken, CFileParser* pParser)
{
	BOOL Loop = TRUE;
	CString csStr;
	CCadLibPart* pPart = 0;

	LookAHeadToken = pParser->Expect(CLexer::Tokens::LIBFILE, LookAHeadToken);
	LookAHeadToken = pParser->Expect(CLexer::Tokens('('), LookAHeadToken);
	csStr = CString(pParser->GetLexer()->GetLexBuff());
	SetName(csStr);
	LookAHeadToken = pParser->Expect(CLexer::Tokens::STRING, LookAHeadToken);
	LookAHeadToken = pParser->Expect(CLexer::Tokens(')'), LookAHeadToken);
	LookAHeadToken = pParser->Expect(CLexer::Tokens('{'), LookAHeadToken);
	while (Loop)
	{
		switch (LookAHeadToken)
		{
		case CLexer::Tokens::LIBPART:
			pPart = new CCadLibPart;
			pPart->Create(this,CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = pPart->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(pPart);
			break;
		case CLexer::Tokens('}'):
			LookAHeadToken = pParser->Expect(CLexer::Tokens('}'), LookAHeadToken);
			Loop = FALSE;
			break;
		default:
			break;
		}
	}
	return LookAHeadToken;
}

void CCadLibrary::Save(FILE* pO, int Indent)
{
}
