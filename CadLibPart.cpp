#include "pch.h"

CCadLibPart::CCadLibPart() :CCadObject(ObjectType::LIBPART)
{
}

CCadLibPart::~CCadLibPart()
{
}

BOOL CCadLibPart::Create(CCadObject* pParent, SubTypes type)
{
	CCadPoint* pRefPoint;

	CCadObject::Create(pParent, type);
	pRefPoint = new CCadPoint();
	pRefPoint->Create(this, CCadObject::SubTypes::REFERENCE_POINT);
	AddObjectAtTail(pRefPoint);
	return 0;
}

void CCadLibPart::UpdateEnclosure()
{
}

BOOL CCadLibPart::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

void CCadLibPart::Parse(
	CParser* pParser,
	Token TypeToken // Token type to save object as
)
{
	CString csName;
	BOOL Loop = TRUE;
	CCadObject* pObj;

	pParser->Expect(Token::LIBPART );
	pParser->Expect(Token('(') );
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::STRING:
			csName = CString((pParser->GetLexer()->GetLexBuffer()));
			SetName(csName);
			pParser->Expect(Token::STRING);
			break;
		case Token::REFERENCE:
			pObj = FindObject(ObjectType::POINT, SubTypes::REFERENCE_POINT, 0);
			if (pObj)
				pObj->Parse(pParser, Token::REFERENCE);
			else
			{
				Exception.ErrorString = _T("Could Not Find Object");
				Exception.badToken = Token::REFERENCE;
				throw Exception;
			}
			break;
		case Token::POINT:
			pObj = FindObject(ObjectType::POINT, SubTypes::ANY, 1);
			if (pObj)
				pObj->Parse(pParser, Token::REFERENCE);
			else
			{
				Exception.ErrorString = _T("Could Not Find Object");
				Exception.badToken = Token::REFERENCE;
				throw Exception;
			}
			break;
		case Token(','):
			pParser->Expect(Token(','));
			break;
		case Token(')'):
			pParser->Expect(Token(')'));
			break;
		}

	}
	ParseLibPartObjects(pParser);
}

void CCadLibPart::ParseLibPartObjects(
	CParser* pParser
)
{
	BOOL Loop = TRUE;
	CADObjectTypes DrawingObjects;

	pParser->Expect(Token('{'));
	memset(&DrawingObjects, 0, sizeof(CADObjectTypes));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::DIMENSION:
			DrawingObjects.pCadDimension = new CCadDimension;
			DrawingObjects.pCadDimension->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadDimension->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::TEXT:
			DrawingObjects.pCadText = new CCadText;
			DrawingObjects.pCadText->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadText->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::ARC:
			DrawingObjects.pCadArc = new CCadArc;
			DrawingObjects.pCadArc->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadArc->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::ARC_CENTER:
			DrawingObjects.pCadArcCent = new CCadArcCent;
			DrawingObjects.pCadArcCent->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadArcCent->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::CIRCLE:
			DrawingObjects.pCadElispe = new CCadEllipse;
			DrawingObjects.pCadElispe->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadElispe->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::RECT:
			DrawingObjects.pCadRect = new CCadRect;
			DrawingObjects.pCadRect->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadRect->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::RNDRECT:
			DrawingObjects.pCadRndRect = new CCadRndRect;
			DrawingObjects.pCadRndRect->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadRndRect->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::POLY:
			DrawingObjects.pCadPolygon = new CCadPolygon;
			DrawingObjects.pCadPolygon->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadPolygon->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::LINE:
			DrawingObjects.pCadLine = new CCadLine;
			DrawingObjects.pCadLine->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadLine->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::HOLERND:
			DrawingObjects.pCadHoleRound = new CCadHoleRound;
			DrawingObjects.pCadHoleRound->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadHoleRound->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::HOLERECT:
			DrawingObjects.pCadHoleRect = new CCadHoleRect;
			DrawingObjects.pCadHoleRect->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadHoleRect->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::HOLE_1FLAT:
			DrawingObjects.pCadHoleRnd1Flat = new CCadHoleRnd1Flat;
			DrawingObjects.pCadHoleRnd1Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadHoleRnd1Flat->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::HOLE_2FLAT:
			DrawingObjects.pCadHoleRnd2Flat = new CCadHoleRnd2Flat;
			DrawingObjects.pCadHoleRnd2Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadHoleRnd2Flat->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
//		case Token::LIBPART:
//			DrawingObjects.pCadLibPart = new CCadLibPart;
//			DrawingObjects.pCadLibPart->Create(this, NULL);
//			LookAHeadToken = DrawingObjects.pCadLibPart->Parse(LookAHeadToken, pParser);
//			AddObjectAtTail(DrawingObjects.pCadObject);
//			break;
		case Token::BITMAP:
			DrawingObjects.pCadBitmap = new CCadBitmap;
			DrawingObjects.pCadBitmap->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadBitmap->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::ARROW:
			DrawingObjects.pCadArrow = new CCadArrow;
			DrawingObjects.pCadArrow->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadArrow->Parse(pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token('}'):
			pParser->Expect(Token('}'));
			Loop = 0;
			break;
		}//end of switch(LookAHeadToken)
	}	//end of while loop
}

void CCadLibPart::Save(
	CFile* pcfFile,
	int Indent
)
{
}

CCadObject* CCadLibPart::Copy()
{
	return nullptr;
}

void CCadLibPart::CopyAttributes(CCadObject* pToObj)
{
//	((CCadLibPart*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}





