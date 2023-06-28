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

CLexer::Tokens CCadDrawing::Parse(CLexer::Tokens LookAHeadToken, CFileParser* pParser)
{
    return LookAHeadToken;
}


CLexer::Tokens CCadDrawing::DrawObjects(
	CLexer::Tokens LookAHeadToken, 
	CFileParser* pParser, 
	CCadObject* pParent
)
{
	BOOL Loop = TRUE;
	CADObjectTypes DrawingObjects;

	memset(&DrawingObjects, 0, sizeof(CADObjectTypes));
	while (Loop)
	{
		switch (LookAHeadToken)
		{
		case CLexer::Tokens::DIMENSION:
			DrawingObjects.pCadDimension = new CCadDimension;
			DrawingObjects.pCadDimension->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadDimension->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::ORG:
			DrawingObjects.pCadOrigin = new CCadOrigin;
			DrawingObjects.pCadOrigin->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadOrigin->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::TEXT:
			DrawingObjects.pCadText = new CCadText;
			DrawingObjects.pCadText->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadText->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::ARC:
			DrawingObjects.pCadArc = new CCadArc;
			DrawingObjects.pCadArc->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArc->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::ARC_CENTER:
			DrawingObjects.pCadArcCent = new CCadArcCent;
			DrawingObjects.pCadArcCent->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArcCent->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::CIRCLE:
			DrawingObjects.pCadElispe = new CCadEllipse;
			DrawingObjects.pCadElispe->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadElispe->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::RECT:
			DrawingObjects.pCadRect = new CCadRect;
			DrawingObjects.pCadRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadRect->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::RNDRECT:
			DrawingObjects.pCadRndRect = new CCadRndRect;
			DrawingObjects.pCadRndRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadRndRect->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::POLY:
			DrawingObjects.pCadPolygon = new CCadPolygon;
			DrawingObjects.pCadPolygon->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadPolygon->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::LINE:
			DrawingObjects.pCadLine = new CCadLine;
			DrawingObjects.pCadLine->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadLine->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::HOLERND:
			DrawingObjects.pCadHoleRound = new CCadHoleRound;
			DrawingObjects.pCadHoleRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRound->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::HOLERECT:
			DrawingObjects.pCadHoleRect = new CCadHoleRect;
			DrawingObjects.pCadHoleRect->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRect->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::HOLE_1FLAT:
			DrawingObjects.pCadHoleRnd1Flat = new CCadHoleRnd1Flat;
			DrawingObjects.pCadHoleRnd1Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRnd1Flat->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::HOLE_2FLAT:
			DrawingObjects.pCadHoleRnd2Flat = new CCadHoleRnd2Flat;
			DrawingObjects.pCadHoleRnd2Flat->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadHoleRnd2Flat->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::LIBPART:
			DrawingObjects.pCadLibPart = new CCadLibPart;
			DrawingObjects.pCadLibPart->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadLibPart->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::BITMAP:
			DrawingObjects.pCadBitmap = new CCadBitmap;
			DrawingObjects.pCadBitmap->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadBitmap->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case CLexer::Tokens::ARROW:
			DrawingObjects.pCadArrow = new CCadArrow;
			DrawingObjects.pCadArrow->Create(this, CCadObject::SubTypes::DEFAULT);
			LookAHeadToken = DrawingObjects.pCadArrow->Parse(LookAHeadToken, pParser);
			pParent->AddObjectAtTail(DrawingObjects.pCadObject);
			break;
		case '}':
			Loop = 0;
			break;
		}//end of switch(Token)
	}	//end of while loop
	return LookAHeadToken;
}

void CCadDrawing::Save(FILE* pO, int Indent)
{
	//-------------------------------------------
	//	Save
	//		This function is used to save the
	// drawing to a computer file
	//
	//	parameter:
	//		pO.....pointer to file tto save to
	//-------------------------------------------
	char* s = new char[256];
	char* s1 = new char[64];

	CCadObject* pCO = GetHead();;
	fprintf(pO, "%sDRAWFILE( %s )\n%s{\n",
		theApp.IndentString(s, 256, Indent),
		CFileParser::SaveColor(s1, 64, CLexer::Tokens::BACKGROUND_COLOR, GetBkColor()),
		s
	);
	while (pCO)
	{
		pCO->Save(pO, CLexer::Tokens::DEFAULT,  Indent + 4);
		pCO = pCO->GetNext();
	}
	fprintf(pO, "%s}\n", s);
	delete[]s1;
	delete[]s;
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
	//		flag..what kind of objects are inlcuded in list
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
				pNewObject = pObj->CopyObject();
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
