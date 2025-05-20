#pragma once


class CLexerFrontCad : public CLexer
{
public:
	CLexerFrontCad();
	~CLexerFrontCad();
	virtual BOOL Create(CArchive& ar);
	virtual Token Lex();
};

