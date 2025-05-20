// CadDrawing.cpp: implementation of the CCadDrawing class.
// This is a top level object for a CAD drawing
//////////////////////////////////////////////////////////////////////

#include "pch.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCadDrawing::CCadDrawing():CCadObject(CCadObject::ObjectType::DRAWING)
{
	m_BkColor = RGB(255,255,255);
}

CCadDrawing::~CCadDrawing()
{
	if(GetHead())
	{
		CCadObject *pO = GetHead();
		while(pO)
		{
			CCadObject *pOd = pO;
			pO = pO->GetNext();
			delete pOd;
		}
	}
}

BOOL CCadDrawing::Create(CCadObject* pParent, CCadObject::SubTypes type)
{
	CCadOrigin* pOrigin;

	pOrigin = new CCadOrigin();
	pOrigin->Create(this, CCadObject::SubTypes::DEFAULT);
	return TRUE;
}

void CCadDrawing::UpdateEnclosure()
{
}

// this is a test
// this is a test
//
void CCadDrawing::Draw(CDC *pDC, MODE mode, DOUBLEPOINT ULHC, CScale Scale)
{
	///-----------------------------------------
	/// Draw
	///		This is the function that draws the
	/// Drawing onto the specified device
	/// context.
	///	parameters:
	///		pDC.....pointer to display context
	///		mode....Ddrawing Mode
	///		ULHC....Offset to add to objects
	///		Scale...Ammount to scale objects by
	///-----------------------------------------
	CCadObject *pCO = GetHead();
	
	while(pCO)
	{
		pCO->Draw(pDC,mode,ULHC,Scale);
		pCO = pCO->GetNext();
	}
}


void CCadDrawing::Print(CDC *pDC, MODE mode, DOUBLEPOINT ULHC, CScale Scale)
{
	//-----------------------------------------
	// Print
	//		This is the draw function for drawing
	//	on the printer.
	//
	//	parameters:
	//		pDC....device context to draw to
	//		mode...drawing mode
	//		Offset.offset to add to objects
	//		Scale.amount to Scale drawing by
	//-----------------------------------------
	CCadObject *pCO = GetHead();
	while(pCO)
	{
		//---------------------------------
		// we really don't give a hang
		// what the mode is
		//---------------------------------
		pCO->Draw(pDC,mode,ULHC,Scale);
		pCO = pCO->GetNext();
	}
}

void CCadDrawing::Parse(
	CParser* pParser,
	Token TypeToken // Token type to save object as

)
{
}


void CCadDrawing::ParseDrawObjects(
	CParser* pParser,
	CCadObject* pParent
)
{
	BOOL Loop = TRUE;
	CADObjectTypes DrawingObjects;

	memset(&DrawingObjects, 0, sizeof(CADObjectTypes));
	while (Loop)
	{
		switch (pParser->GetLookaHead())
		{
		case Token::DIMENSION:
			DrawingObjects.pCadDimension = new CCadDimension;
			DrawingObjects.pCadDimension->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadDimension->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::ORG:
			DrawingObjects.pCadOrigin = new CCadOrigin;
			DrawingObjects.pCadOrigin->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadOrigin->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::TEXT:
			DrawingObjects.pCadText = new CCadText;
			DrawingObjects.pCadText->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadText->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::ARC:
			DrawingObjects.pCadArc = new CCadArc;
			DrawingObjects.pCadArc->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadArc->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::ARC_CENTER:
			DrawingObjects.pCadArcCent = new CCadArcCent;
			DrawingObjects.pCadArcCent->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadArcCent->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::CIRCLE:
			DrawingObjects.pCadElispe = new CCadEllipse;
			DrawingObjects.pCadElispe->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadElispe->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::RECT:
			DrawingObjects.pCadRect = new CCadRect;
			DrawingObjects.pCadRect->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadRect->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::RNDRECT:
			DrawingObjects.pCadRndRect = new CCadRndRect;
			DrawingObjects.pCadRndRect->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadRndRect->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::POLY:
			DrawingObjects.pCadPolygon = new CCadPolygon;
			DrawingObjects.pCadPolygon->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadPolygon->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::LINE:
			DrawingObjects.pCadLine = new CCadLine;
			DrawingObjects.pCadLine->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadLine->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::HOLERND:
			DrawingObjects.pCadHoleRound = new CCadHoleRound;
			DrawingObjects.pCadHoleRect->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadHoleRound->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::HOLERECT:
			DrawingObjects.pCadHoleRect = new CCadHoleRect;
			DrawingObjects.pCadHoleRect->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadHoleRect->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::HOLE_1FLAT:
			DrawingObjects.pCadHoleRnd1Flat = new CCadHoleRnd1Flat;
			DrawingObjects.pCadHoleRnd1Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadHoleRnd1Flat->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::HOLE_2FLAT:
			DrawingObjects.pCadHoleRnd2Flat = new CCadHoleRnd2Flat;
			DrawingObjects.pCadHoleRnd2Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadHoleRnd2Flat->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::LIBPART:
			DrawingObjects.pCadLibPart = new CCadLibPart;
			DrawingObjects.pCadLibPart->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadLibPart->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::BITMAP:
			DrawingObjects.pCadBitmap = new CCadBitmap;
			DrawingObjects.pCadBitmap->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadBitmap->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token::ARROW:
			DrawingObjects.pCadArrow = new CCadArrow;
			DrawingObjects.pCadArrow->Create(this, CCadObject::SubTypes::DEFAULT);
			DrawingObjects.pCadArrow->Parse(pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case Token('}'):
			Loop = 0;
			break;
		}//end of switch(Token)
	}	//end of while loop
}

void CCadDrawing::Save(
	CFile* pcfFile,
	int Indent
)
{
	//-------------------------------------------
	//	Save
	//		This function is used to save the
	// drawing to a computer file
	//
	//	parameter:
	//		pO.....pointer to file to save to
	//-------------------------------------------
	char* s = new char[256];
	char* s1 = new char[64];
	char* sO = new char[2048];
	CString csOut;
	CCadObject* pCO;

	csOut.Format( _T("%hsDRAWFILE(%hs)\n%hs{\n"),
		GETAPP.IndentString(s, 256, Indent,' '),
		CParserFrontCad::SaveColor(s1, 64, Token::BACKGROUND_COLOR, GetBkColor()),
		s
	);
	GETAPP.ConvertCStringToChar(sO, csOut);
	pcfFile->Write(sO, strlen(sO));
	pCO = GetHead();
	while (pCO)
	{
		pCO->Save(pcfFile,  Indent + 2);
		pCO = pCO->GetNext();
	}
	csOut.Format( _T("%hs}\n"), s);
	delete[]s1;
	delete[]s;
}

CCadObject* CCadDrawing::Copy()
{
	return nullptr;
}

void CCadDrawing::CopyAttributes(CCadObject* pToObj)
{
//	((CCadDrawing*)pToObj)->CopyAttributesFrom(GetPtrToAttributes());
}

BOOL CCadDrawing::IsPointEnclosed(DOUBLEPOINT p)
{
	return 0;
}

int CCadDrawing::CheckSelected(DOUBLEPOINT p, CCadObject **ppSelList, int n, int flag)
{
	//--------------------------------------
	// CheckSelected
	//
	// This function is used to see if
	// there are any objects under the
	// point.  This function returns the
	// first 'n' objects it finds.  It does
	// not actually select anything.
	//
	// parameters:
	//		p......point of interest
	//		ppSelList...pointer to an array of selected objects
	//		n.....maximum number of objects to check for
	//		flag..what kind of objects are included in list
	//				0-All Objects
	//				1-Unselected Objects
	//				2-Selected Objects
	//	returns:
	//		number of objects that are under the point
	//--------------------------------------------
	int count = 0;
	CCadObject *pCO = GetHead();

	while(pCO && ((count < n) || !n))
	{
		if(pCO->CheckSelected(p, ppSelList, n, flag))
		{
			switch (flag)
			{
				case CADDRAWUBG_CHECKSEL_FLAG_ALL:
					if(ppSelList) ppSelList[count] = pCO;
					count++;	///increment count
					break;
				case CADDRAWUBG_CHECKSEL_FLAG_UNSEL:
					if (!pCO->IsSelected())
					{
						if (ppSelList) ppSelList[count] = pCO;
						count++;	///increment count
					}
					break;
				case CADDRAWUBG_CHECKSEL_FLAG_SEL:
					if (pCO->IsSelected())
					{
						if (ppSelList) ppSelList[count] = pCO;
						count++;	///increment count
					}
					break;
			}
		}
		pCO = pCO->GetNext();
	}
	return count;
}

CCadLibPart * CCadDrawing::CreatePartFromSelected(CString& csName)
{
	///------------------------------------------------
	/// CreatePartFromSelected
	///		Thhis function is used to create a library
	/// part from all of the objects in the drawing that
	/// are selected.
	///
	/// parameters:
	///		name....name of the new part
	/// returns:
	///		pointer to the CCadLibPart that will
	///		represent the new poart.
	///------------------------------------------------
	CCadObject *pObj = 0;
	CCadLibPart *pPart;
	CCadObject* pNewObject;;

	pObj = GetHead();
	if(pObj == NULL)
		pPart = NULL;
	else
	{
		pPart = new CCadLibPart;
		pPart->Create(this,CCadObject::SubTypes::DEFAULT );
		pPart->SetName(csName);
		while (pObj)
		{
			if (pObj->IsSelected())
			{
				pNewObject = pObj->Copy();
				pPart->AddObjectAtTail(pNewObject);
			}
			pObj = pObj->GetNext();
		}
	}
	return pPart;
}

CCadPrintRect *CCadDrawing::GetPrintRect()
{
	CCadObject *pObj;
	pObj = GetHead();
	BOOL Loop = TRUE;
	while (Loop && pObj)
	{
		if (CCadObject::ObjectType::PRINTRECT == pObj->GetType())
		{
			Loop = FALSE;
		}
		else
			pObj = pObj->GetNext();
	}
	return (CCadPrintRect *)pObj;
}
