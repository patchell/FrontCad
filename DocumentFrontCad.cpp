#include "pch.h"


IMPLEMENT_DYNCREATE(CDocumentFrontCad, CBaseDocument)


BEGIN_MESSAGE_MAP(CDocumentFrontCad, CBaseDocument)
	ON_COMMAND(ID_MODIFY_SWITCHREFERENCE, &CDocumentFrontCad::OnModifySwitchreference)
END_MESSAGE_MAP()


CDocumentFrontCad::CDocumentFrontCad()
{
	m_pLexer = 0;
	m_DocSize = CDoubleSize(DOCUMENT_DRAWING_X, DOCUMENT_DRAWING_Y);
	m_PaperSize = CDoubleSize(DOCUMENT_PAPER_X, DOCUMENT_PAPER_Y);
}

CDocumentFrontCad::~CDocumentFrontCad()
{
}

BOOL CDocumentFrontCad::Create(CArchive& ar)
{
	BOOL rV = TRUE;

	return rV;
}

BOOL CDocumentFrontCad::OnNewDocument()
{
	static int NewFileCount = 0;
	CString csName;
	CString csTitle;
	CDlgNewDrawing Dlg;
	int Id;
	CCadDrawing* pDrawing;
	BOOL rV = FALSE;

	csName.Format(_T("FrontCadDoc%d"), ++NewFileCount);
	csTitle.Format(_T("New FrontCad Drawing"));
	Dlg.SetDialogTitle(csTitle);
	Dlg.SetDocumentName(csName);
	Dlg.SetPaperSizeX(m_PaperSize.dCX);
	Dlg.SetPaperSizeY(m_PaperSize.dCY);
	Dlg.SetDrawingSizeX(m_DocSize.dCX);
	Dlg.SetDrawingSizeY(m_DocSize.dCY);

	Id = (INT_PTR)Dlg.DoModal();
	if (IDOK == Id)
	{
		rV = TRUE;
		SetPaperSize(CDoubleSize(Dlg.GetPaperSizeX(), Dlg.GetPaperSizeY()));
		SetDocSize(CDoubleSize(Dlg.GetDrawingSizeX(), Dlg.GetDrawingSizeY()));
		SetDocumentName(Dlg.GetDocumentName());
		SetTitle(GetDocumentName());
	}
	pDrawing = new CCadDrawing;
	pDrawing->Create(NULL, CCadObject::SubTypes::DEFAULT);
	SetDrawing(pDrawing);
	return rV;
}

void CDocumentFrontCad::Serialize(CArchive& ar)
{
	CFile* pFile = 0;
	CString csFname;
	UINT FileSize = 0;

	printf("Serialize\n");

	if (ar.IsStoring())
	{
		pFile = ar.GetFile();
		csFname = pFile->GetFilePath();
		SetFileName(csFname);
		MessageBoxW(NULL, csFname, _T("Save"), MB_OK);
		GetDrawing()->Save(pFile, 0);
	}
	else
	{
		//-----------------------------
		// Open Input file
		//-----------------------------
		CParserFrontCad* pParser = 0;
		CCadDrawing* pCadDrawing = 0;

		pCadDrawing = new CCadDrawing;
		pCadDrawing->Create(NULL, CCadObject::SubTypes::NONE);
		csFname = ar.GetFile()->GetFilePath();
		SetFileName(csFname);
		pParser = new CParserFrontCad;
		pParser->Create(ar);
		pParser->Parse(GetDrawing());
	}
}

// CDocumentFrontCad commands
int CDocumentFrontCad::PointInObjectAndSelect(
	DOUBLEPOINT p,
	CCadObject* pExcludeObject,
	CCadObject** ppSelList,
	int n,
	UINT nKinds		//kinds of object to select
)
{
	//--------------------------------------
	// PointInObjectAndSelect
	//
	// This Method is used to see if
	// there are any objects under the
	// point.  This Method returns the
	// first 'n' objects it finds.  It does
	// not actually select anything.
	//
	// parameters:
	//		p......point of interest
	//		ppSelList...pointer to an array of selected objects
	//		n.....maximum number of objects to check for
	//		nKinds..what kind of objects are included in list
	//	returns:
	//		number of objects that are under the point
	//--------------------------------------------
	CCadObject* pObject = GetDrawing()->GetHead();
	int index = 0;

	while (pObject && (index < n))
	{
		index = pObject->PointInObjectAndSelect(p, pExcludeObject, ppSelList, index, n, nKinds);
		pObject = pObject->GetNext();
	}
	printf("FrontCadDoc Objects Found:%d\n", index);
	return index;
}

void CDocumentFrontCad::SetDirty(UINT flag)
{
}


void CDocumentFrontCad::OnModifySwitchreference()
{
	// TODO: Add your command handler code here
}
