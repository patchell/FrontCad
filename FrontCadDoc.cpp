// FrontCadDoc.cpp : implementation file
//

#include "pch.h"

// CFrontCadDoc

IMPLEMENT_DYNCREATE(CFrontCadDoc, CBaseDocument)

CFrontCadDoc::CFrontCadDoc()
{
	m_DocSize = CDoubleSize(DOCUMENT_DRAWING_X, DOCUMENT_DRAWING_Y);
	m_PaperSize = CDoubleSize(DOCUMENT_PAPER_X, DOCUMENT_PAPER_Y);
}

CFrontCadDoc::~CFrontCadDoc()
{
}

BOOL CFrontCadDoc::OnNewDocument()
{
	static int NewFileCount = 0;
	CString csName;
	CString csTitle;
	CDlgNewDrawing Dlg;
	int Id;
	CCadDrawing* pDrawing;
	BOOL rV = FALSE;

	csName.Format(_T("FrontCadDoc%d"), ++NewFileCount);
	csTitle.Format(_T("New FrontCad Drawuing"));
	Dlg.SetDialogTitle(csTitle);
	Dlg.SetDocumentName(csName);
	Dlg.SetPaperSizeX(m_PaperSize.dCX);
	Dlg.SetPaperSizeY(m_PaperSize.dCY);
	Dlg.SetDrawingSizeX(m_DocSize.dCX);
	Dlg.SetDrawingSizeY(m_DocSize.dCY);

	Id = Dlg.DoModal();
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


BEGIN_MESSAGE_MAP(CFrontCadDoc, CBaseDocument)
	ON_COMMAND(ID_MODIFY_SWITCHREFERENCE, &CFrontCadDoc::OnModifySwitchreference)
END_MESSAGE_MAP()


// CFrontCadDoc diagnostics

#ifdef _DEBUG
void CFrontCadDoc::AssertValid() const
{
	CBaseDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CFrontCadDoc::Dump(CDumpContext& dc) const
{
	CBaseDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CFrontCadDoc serialization

void CFrontCadDoc::Serialize(CArchive& ar)
{
	CLexer::Tokens Token;

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		//-----------------------------
		// Open Input file
		//-----------------------------
		CFileParser Parser;

		CFileDialog Dlg(
			TRUE, 
			_T("frontCad"), 
			NULL, 
			6UL,
			_T("*.frontCad"),
			NULL,
			0,
			1
		);
		CString FileName;
		FileName = Dlg.GetPathName();
		Parser.Create(FileName);
	}
}
#endif


// CFrontCadDoc commands
int CFrontCadDoc::PointInObjectAndSelect(
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
	//		nKinds..what kind of objects are inlcuded in list
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

void CFrontCadDoc::SetDirty(UINT flag)
{
}


void CFrontCadDoc::OnModifySwitchreference()
{
	// TODO: Add your command handler code here
}
