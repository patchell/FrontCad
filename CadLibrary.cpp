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

void CCadLibrary::UpdateEnclosure()
{
}

BOOL CCadLibrary::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

void CCadLibrary::Parse(
	CParser* pParser,	// pointer to parser
	Token TypeToken // Token type to save object as
)
{
	BOOL Loop = TRUE;
	CString csStr;
	CCadLibPart* pPart = 0;

	pParser->Expect( Token::LIBFILE);
	pParser->Expect( Token('('));
	csStr = CString(pParser->GetLexer()->GetLexBuffer());
	SetName(csStr);
	pParser->Expect( Token::STRING);
	pParser->Expect( Token(')'));
	pParser->Expect( Token('{'));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::LIBPART:
			pPart = new CCadLibPart;
			pPart->Create(this,CCadObject::SubTypes::DEFAULT);
			pPart->Parse(pParser);
			AddObjectAtTail(pPart);
			break;
		case Token('}'):
			pParser->Expect( Token('}'));
			Loop = FALSE;
			break;
		default:
			break;
		}
	}
}

void CCadLibrary::Save(
	CFile* pcfFile,
	int Indent
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
