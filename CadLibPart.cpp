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

BOOL CCadLibPart::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

int CCadLibPart::Parse(
	CFile* pcfInFile,
	int LookAHeadToken, 
	CFileParser* pParser,
	int TypeToken // Token type to save object as
)
{
	CString csName;
	BOOL Loop = TRUE;
	CCadObject* pObj;

	LookAHeadToken = pParser->Expect(pcfInFile, TOKEN_LIBPART, LookAHeadToken);
	LookAHeadToken = pParser->Expect(pcfInFile, int('('), LookAHeadToken);
	while (Loop)
	{
		switch (LookAHeadToken)
		{
		case TOKEN_STRING:
			csName = CString((pParser->GetLexBuff()));
			SetName(csName);
			LookAHeadToken = pParser->Expect(pcfInFile, TOKEN_STRING, LookAHeadToken);
			break;
		case TOKEN_REFERENCE:
			pObj = FindObject(ObjectType::POINT, SubTypes::REFERENCE_POINT, 0);
			if (pObj)
				LookAHeadToken = pObj->Parse(pcfInFile, TOKEN_REFERENCE, pParser);
			else
			{
				Exception.ErrorString = _T("Could Not Find Object");
				Exception.badToken = TOKEN_REFERENCE;
				throw Exception;
			}
			break;
		case TOKEN_POINT:
			pObj = FindObject(ObjectType::POINT, SubTypes::ANY, 1);
			if (pObj)
				LookAHeadToken = pObj->Parse(pcfInFile, TOKEN_REFERENCE, pParser);
			else
			{
				Exception.ErrorString = _T("Could Not Find Object");
				Exception.badToken = TOKEN_REFERENCE;
				throw Exception;
			}
			break;
		case int(','):
			LookAHeadToken = pParser->Expect(pcfInFile, int(','), LookAHeadToken);
			break;
		case int(')'):
			LookAHeadToken = pParser->Expect(pcfInFile, int(')'), LookAHeadToken);
			break;
		}

	}
	LookAHeadToken = ParseLibPartObjects(pcfInFile, LookAHeadToken, pParser);
	return LookAHeadToken;
}

int CCadLibPart::ParseLibPartObjects(
	CFile* pcfInFile,
	int LookAHeadToken, 
	CFileParser* pParser
)
{
	BOOL Loop = TRUE;
	CADObjectTypes DrawingObjects;

	LookAHeadToken = pParser->Expect(pcfInFile, int('{'), LookAHeadToken);
	memset(&DrawingObjects, 0, sizeof(CADObjectTypes));
	while (Loop)
	{
		switch (LookAHeadToken)
		{
		case TOKEN_DIMENSION:
			DrawingObjects.pCadDimension = new CCadDimension;
			DrawingObjects.pCadDimension->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadDimension->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_TEXT:
			DrawingObjects.pCadText = new CCadText;
			DrawingObjects.pCadText->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadText->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_ARC:
			DrawingObjects.pCadArc = new CCadArc;
			DrawingObjects.pCadArc->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArc->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_ARC_CENTER:
			DrawingObjects.pCadArcCent = new CCadArcCent;
			DrawingObjects.pCadArcCent->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArcCent->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_CIRCLE:
			DrawingObjects.pCadElispe = new CCadEllipse;
			DrawingObjects.pCadElispe->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadElispe->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_RECT:
			DrawingObjects.pCadRect = new CCadRect;
			DrawingObjects.pCadRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadRect->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_RNDRECT:
			DrawingObjects.pCadRndRect = new CCadRndRect;
			DrawingObjects.pCadRndRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadRndRect->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_POLY:
			DrawingObjects.pCadPolygon = new CCadPolygon;
			DrawingObjects.pCadPolygon->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadPolygon->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_LINE:
			DrawingObjects.pCadLine = new CCadLine;
			DrawingObjects.pCadLine->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadLine->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_HOLERND:
			DrawingObjects.pCadHoleRound = new CCadHoleRound;
			DrawingObjects.pCadHoleRound->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRound->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_HOLERECT:
			DrawingObjects.pCadHoleRect = new CCadHoleRect;
			DrawingObjects.pCadHoleRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRect->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_HOLE_1FLAT:
			DrawingObjects.pCadHoleRnd1Flat = new CCadHoleRnd1Flat;
			DrawingObjects.pCadHoleRnd1Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRnd1Flat->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_HOLE_2FLAT:
			DrawingObjects.pCadHoleRnd2Flat = new CCadHoleRnd2Flat;
			DrawingObjects.pCadHoleRnd2Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRnd2Flat->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
//		case TOKEN_LIBPART:
//			DrawingObjects.pCadLibPart = new CCadLibPart;
//			DrawingObjects.pCadLibPart->Create(this, NULL);
//			LookAHeadToken = DrawingObjects.pCadLibPart->Parse(LookAHeadToken, pParser);
//			AddObjectAtTail(DrawingObjects.pCadObject);
//			break;
		case TOKEN_BITMAP:
			DrawingObjects.pCadBitmap = new CCadBitmap;
			DrawingObjects.pCadBitmap->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadBitmap->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case TOKEN_ARROW:
			DrawingObjects.pCadArrow = new CCadArrow;
			DrawingObjects.pCadArrow->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArrow->Parse(pcfInFile, LookAHeadToken, pParser);
			AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case '}':
			LookAHeadToken = pParser->Expect(pcfInFile, int('}'), LookAHeadToken);
			Loop = 0;
			break;
		}//end of switch(LookAHeadToken)
	}	//end of while loop
	return LookAHeadToken;
}

void CCadLibPart::Save(
	CFile* pcfFile,
	int Indent, 
	int flags
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





