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

CLexer::Tokens CCadLibPart::Parse(CLexer::Tokens LookAHeadToken, CFileParser* pParser)
{
	CString csName;
	BOOL Loop = TRUE;
	CCadObject* pObj;

	LookAHeadToken = pParser->Expect(CLexer::Tokens::LIBPART, LookAHeadToken);
	LookAHeadToken = pParser->Expect(CLexer::Tokens('('), LookAHeadToken);
	while (Loop)
	{
		switch (LookAHeadToken)
		{
		case CLexer::Tokens::STRING:
			csName = CString((pParser->GetLexer()->GetLexBuff()));
			SetName(csName);
			LookAHeadToken = pParser->Expect(CLexer::Tokens::STRING, LookAHeadToken);
			break;
		case CLexer::Tokens::REFERENCE:
			pObj = FindObject(ObjectType::POINT, SubTypes::REFERENCE_POINT, 0);
			if (pObj)
				LookAHeadToken = pObj->Parse(CLexer::Tokens::REFERENCE, pParser);
			else
			{
				Exception.ErrorString = _T("Could Not Find Object");
				Exception.badToken = CLexer::Tokens::REFERENCE;
				throw Exception;
			}
			break;
		case CLexer::Tokens::POINT:
			pObj = FindObject(ObjectType::POINT, SubTypes::ANY, 1);
			if (pObj)
				LookAHeadToken = pObj->Parse(CLexer::Tokens::REFERENCE, pParser);
			else
			{
				Exception.ErrorString = _T("Could Not Find Object");
				Exception.badToken = CLexer::Tokens::REFERENCE;
				throw Exception;
			}
			break;
		case CLexer::Tokens(','):
			LookAHeadToken = pParser->Expect(CLexer::Tokens(','), LookAHeadToken);
			break;
		case CLexer::Tokens(')'):
			LookAHeadToken = pParser->Expect(CLexer::Tokens(')'), LookAHeadToken);
			break;
			break;
		}

	}
	LookAHeadToken = ParseLibPartObjects(LookAHeadToken, pParser);
	return LookAHeadToken;
}

CLexer::Tokens CCadLibPart::ParseLibPartObjects(
	CLexer::Tokens LookAHeadToken, 
	CFileParser* pParser
)
{
	BOOL Loop = TRUE;
	CADObjectTypes DrawingObjects;

	LookAHeadToken = pParser->Expect(CLexer::Tokens('{'), LookAHeadToken);
	memset(&DrawingObjects, 0, sizeof(CADObjectTypes));
	while (Loop)
	{
		switch (LookAHeadToken)
		{
		case CLexer::Tokens::DIMENSION:
			DrawingObjects.pCadDimension = new CCadDimension;
			DrawingObjects.pCadDimension->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadDimension->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::TEXT:
			DrawingObjects.pCadText = new CCadText;
			DrawingObjects.pCadText->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadText->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::ARC:
			DrawingObjects.pCadArc = new CCadArc;
			DrawingObjects.pCadArc->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArc->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::ARC_CENTER:
			DrawingObjects.pCadArcCent = new CCadArcCent;
			DrawingObjects.pCadArcCent->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArcCent->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::CIRCLE:
			DrawingObjects.pCadElispe = new CCadEllipse;
			DrawingObjects.pCadElispe->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadElispe->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::RECT:
			DrawingObjects.pCadRect = new CCadRect;
			DrawingObjects.pCadRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadRect->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::RNDRECT:
			DrawingObjects.pCadRndRect = new CCadRndRect;
			DrawingObjects.pCadRndRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadRndRect->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::POLY:
			DrawingObjects.pCadPolygon = new CCadPolygon;
			DrawingObjects.pCadPolygon->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadPolygon->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::LINE:
			DrawingObjects.pCadLine = new CCadLine;
			DrawingObjects.pCadLine->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadLine->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::HOLERND:
			DrawingObjects.pCadHoleRound = new CCadHoleRound;
			DrawingObjects.pCadHoleRound->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRound->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::HOLERECT:
			DrawingObjects.pCadHoleRect = new CCadHoleRect;
			DrawingObjects.pCadHoleRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRect->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::HOLE_1FLAT:
			DrawingObjects.pCadHoleRnd1Flat = new CCadHoleRnd1Flat;
			DrawingObjects.pCadHoleRnd1Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRnd1Flat->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::HOLE_2FLAT:
			DrawingObjects.pCadHoleRnd2Flat = new CCadHoleRnd2Flat;
			DrawingObjects.pCadHoleRnd2Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRnd2Flat->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
//		case CLexer::Tokens::LIBPART:
//			DrawingObjects.pCadLibPart = new CCadLibPart;
//			DrawingObjects.pCadLibPart->Create(this, NULL);
//			LookAHeadToken = DrawingObjects.pCadLibPart->Parse(LookAHeadToken, pParser);
//			AddObjectAtTail(DrawingObjects.pCadObject);
//			break;
		case CLexer::Tokens::BITMAP:
			DrawingObjects.pCadBitmap = new CCadBitmap;
			DrawingObjects.pCadBitmap->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadBitmap->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::ARROW:
			DrawingObjects.pCadArrow = new CCadArrow;
			DrawingObjects.pCadArrow->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArrow->Parse(LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case '}':
			LookAHeadToken = pParser->Expect(CLexer::Tokens('}'), LookAHeadToken);
			Loop = 0;
			break;
		}//end of switch(LookAHeadToken)
	}	//end of while loop
	return LookAHeadToken;
}

void CCadLibPart::Save(FILE* pO, int Indent)
{
}





