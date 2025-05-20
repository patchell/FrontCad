#pragma once

class CParserFrontCad : public CParser
{
public:
	CParserFrontCad();
	virtual ~CParserFrontCad();
	virtual BOOL Create(CArchive& ar);
	virtual BOOL Parse(CCadObject* pCadObj);
	void DrawFileAttributes(
		CCadObject* pO
	);
	void DrawFileAttributes2(
		CCadObject* pO
	);
	void DrawFileAttributes3(CCadObject* pO);
};

