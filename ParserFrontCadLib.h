#pragma once

class CParserFrontCadLib : public CParser
{
public:
	CParserFrontCadLib();
	virtual ~CParserFrontCadLib();
	BOOL Create(CArchive& ar);
	BOOL Parse(CCadLibrary** pLib);
};

