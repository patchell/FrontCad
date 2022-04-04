// FrontCadDoc.cpp : implementation file
//

#include "pch.h"

// CFrontCadDoc

IMPLEMENT_DYNCREATE(CFrontCadDoc, CBaseDocument)

CFrontCadDoc::CFrontCadDoc()
{
	m_DocSize = CDoubleSize(11.0, 8.5);
}

CFrontCadDoc::~CFrontCadDoc()
{
}

BOOL CFrontCadDoc::OnNewDocument()
{
	static int NewFileCount = 0;
	CString csName;
	CString csTitle;
	CDlgNewName Dlg;

	csName.Format(_T("FrontCadDoc%d"), ++NewFileCount);
	csTitle.Format(_T("New FrontCad Doc"));
	Dlg.SetDialogTitle(csTitle);
	Dlg.SetNameString(csName);
	Dlg.DoModal();
	SetTitle(Dlg.GetNameString());
	return TRUE;
}


BEGIN_MESSAGE_MAP(CFrontCadDoc, CBaseDocument)
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
	DocFileParseToken Token;

	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		CLexer Lex;
		CADObjectTypes Obj;
		memset(&Obj, 0, sizeof(CADObjectTypes));
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

		Token = Lex.Lex();
		if (Token > DocFileParseToken::ERRORToken)
		{
			try
			{
				switch (Token)
				{
				case DocFileParseToken::ARC:
					Obj.pCadArc = new CCadArc;
					Token = Obj.pCadArc->Parse(Token, &Lex);
					break;
				case DocFileParseToken::ARCANGLE:
					break;
				case DocFileParseToken::ARCCENTERED:
					break;
				case DocFileParseToken::ARROW:
					break;
				case DocFileParseToken::BITMAP:
					break;
				case DocFileParseToken::LIBPART:
					break;
				case DocFileParseToken::LINE:
					break;
				case DocFileParseToken::POLYGON:
					break;
				case DocFileParseToken::RECT:
					break;
				case DocFileParseToken::ROUNDRECT:
					break;
				case DocFileParseToken::HOLERECT:
					break;
				case DocFileParseToken::HOLERND1FLAT:
					break;
				case DocFileParseToken::HOLERND2FLAT:
					break;
				case DocFileParseToken::HOLEROUND:
					break;
				case DocFileParseToken::DIMENSION:
					break;
				case DocFileParseToken::ORIGIN:
					break;
				}
			}
			catch (CString csError)
			{
				MessageBoxW(NULL, csError, _T("Bad FrontCad Document"), MB_ICONHAND | MB_OK);
			}
		}
	}
}
#endif


// CFrontCadDoc commands
UINT CFrontCadDoc::PointInObjectAndSelect(
	CDoublePoint p, 
	CCadObject** ppSelList,
	int n,
	DrawingCheckSelectFlags flag
)
{
	///--------------------------------------
	/// PointInObjectAndSelect
	///
	/// This Method is used to see if
	/// there are any objects under the
	/// point.  This Method returns the
	/// first 'n' objects it finds.  It does
	/// not actually select anything.
	///
	/// parameters:
	///		p......point of interest
	///		ppSelList...pointer to an array of selected objects
	///		n.....maximum number of objects to check for
	///		flag..what kind of objects are inlcuded in list
	///				0-All Objects
	///				1-Unselected Objects
	///				2-Selected Objects
	///	returns:
	///		number of objects that are under the point
	///--------------------------------------------
	int count = 0;
	CCadObject* pObject = GetHead();

	while (pObject && ((count < n) || !n))
	{
		if (pObject->PointInObjectAndSelect(p))
		{
			switch (flag)
			{
			case DrawingCheckSelectFlags::FLAG_ALL:
				if (ppSelList) ppSelList[count] = pObject;
				count++;	///increment count
				break;
			case DrawingCheckSelectFlags::FLAG_UNSEL:
				if (!pObject->IsSelected())
				{
					if (ppSelList) ppSelList[count] = pObject;
					count++;	///increment count
				}
				break;
			case DrawingCheckSelectFlags::FLAG_SEL:
				if (pObject->IsSelected())
				{
					if (ppSelList) ppSelList[count] = pObject;
					count++;	///increment count
				}
				break;
			}
		}
		pObject = pObject->GetNext();
	}
	return count;
}

void CFrontCadDoc::SetDirty(UINT flag)
{
}
