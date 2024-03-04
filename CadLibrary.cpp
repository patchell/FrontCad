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

BOOL CCadLibrary::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

int CCadLibrary::Parse(
	CFile* pcfInFile,
	int LookAHeadToken,	// Lookahead Token
	CFileParser* pParser,	// pointer to parser
	int TypeToken // Token type to save object as
)
{
	BOOL Loop = TRUE;
	CString csStr;
	CCadLibPart* pPart = 0;

	LookAHeadToken = pParser->Expect(pcfInFile, TOKEN_LIBFILE, LookAHeadToken);
	LookAHeadToken = pParser->Expect(pcfInFile, int('('), LookAHeadToken);
	csStr = CString(pParser->GetLexBuff());
	SetName(csStr);
	LookAHeadToken = pParser->Expect(pcfInFile, TOKEN_STRING, LookAHeadToken);
	LookAHeadToken = pParser->Expect(pcfInFile, int(')'), LookAHeadToken);
	LookAHeadToken = pParser->Expect(pcfInFile, int('{'), LookAHeadToken);
	while (Loop)
	{
		switch (LookAHeadToken)
		{
		case TOKEN_LIBPART:
			pPart = new CCadLibPart;
			pPart->Create(this,CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = pPart->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(pPart);
			break;
		case int('}'):
			LookAHeadToken = pParser->Expect(pcfInFile, int('}'), LookAHeadToken);
			Loop = FALSE;
			break;
		default:
			break;
		}
	}
	return LookAHeadToken;
}

void CCadLibrary::Save(
	CFile* pcfFile,
	int Indent, 
	int flags
)
{
}

CCadObject* CCadLibrary::Copy()
{
	return nullptr;
}

void CCadLibrary::CopyAttributes(CCadObject* pToObj)
{
//	((CCadLibrary*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}
