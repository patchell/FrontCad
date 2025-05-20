#pragma once

constexpr auto LEXER_LEX_BUFF_SIZE = 256;

class CLexer
{
	struct KEYWORD {
		Token m_Token;
		const char* m_pName;
	};
	static inline KEYWORD KeyWords[] = {
		{Token::BASE_OBJ,"BASE_OBJ"},
		{Token::ARC,"ARC"},
		{Token::ARC_CENTER,"ARC_CENTER"},
		{Token::CIRCLE,"CIRCLE"},
		{Token::RECT,"RECT"},
		{Token::RNDRECT,"RNDRECT"},
		{Token::POLY,"POLY"},
		{Token::LINE,"LINE"},
		{Token::HOLERND,"HOLERND"},
		{Token::HOLERECT,"HOLERECT"},
		{Token::HOLE_1FLAT,"HOLE_1FLAT"},
		{Token::HOLE_2FLAT,"HOLE_2FLAT"},
		{Token::LIBPART,"LIBPART"},
		{Token::TEXT,"TEXT"},
		{Token::POINT,"POINT_1"},
		{Token::VERTEX_POINT,"VERTEX_POINT"},
		{Token::START_POINT,"START_POINT"},
		{Token::END_POINT,"END_POINT"},
		{Token::BACKGROUND_COLOR,"BACKGROUND_COLOR"},
		{Token::LINE_COLOR,"LINE_COLOR"},
		{Token::TEXT_COLOR,"TEXT_COLOR"},
		{Token::FILL_COLOR,"FILL_COLOR"},
		{Token::SELECTED_COLOR,"SELECTED_COLOR"},
		{Token::RED,"RED"},
		{Token::GREEN,"GREEN"},
		{Token::BLUE,"BLUE"},
		{Token::FONT,"FONT"},
		{Token::FONT_WEIGHT,"FONT_WEIGHT"},
		{Token::FONTHEIGHT,"FONTHIEGHT"},
		{Token::FONTWIDTH,"FONTWIDTH"},
		{Token::FONT_FORMAT,"FONT_FORMAT"},
		{Token::FONT_NAME,"FONT_NAME"},
		{Token::RADIUS,"RADIUS"},
		{Token::FLATDIST,"FLATDIST"},
		{Token::DRAWFILE,"DRAWFILE"},
		{Token::VERTEX,"VERTEX"},
		{Token::LIBFILE,"LIBFILE"},
		{Token::RECTWIDTH,"RECTWIDTH"},
		{Token::RECTHIEGHT,"RECTHIEGHT"},
		{Token::ANGLE,"ANGLE"},
		{Token::BK_TRANSPARENT,"TRANSPARENT"},
		{Token::REFERENCE,"REFERENCE"},
		{Token::BITMAP,"BITMAP"},
		{Token::FILENAME,"FILENAME"},
		{Token::ARROW,"ARROW"},
		{Token::LENGTH,"LENGTH"},
		{Token::ORG,"ORG"},
		{Token::DIMENSION,"DIMENSION"},
		{Token::PRINTRECT,"PRINTRECT"},
		{Token::SIZE,"SIZE"},
		{Token::SCALE,"SCALE"},
		{Token::LINE_WIDTH,"LINE_WIDTH"},
		{Token::ARROW_LENGTH,"ARROW_LENGTH"},
		{Token::ARROW_WIDTH,"ARROW_WIDTH"},
		{Token::SELECTED_UNITS,"SELECTED_UNITS"},
		{Token::ATTRIB_ARC,"ATTRIB_ARC"},
		{Token::ATTRIB_ARCCENTER,"ATTRIB_ARCCENTER"},
		{Token::ATTRIB_ARROW,"ATTRIB_ARROW"},
		{Token::ATTRIB_BITMAP,"ATTRIB_BITMAP"},
		{Token::ATTRIB_DIMENSION,"ATTRIB_DIMENSION"},
		{Token::ATTRIB_ELLIPSE,"ATTRIB_ELLIPSE"},
		{Token::ATTRIB_GRID,"ATTRIB_GRID"},
		{Token::ATTRIB_HOLERECT,"ATTRIB_HOLERECT"},
		{Token::ATTRIB_HOLERND1F,"ATTRIB_HOLERND1F"},
		{Token::ATTRIB_HOLERND2F,"ATTRIB_HOLERND2F"},
		{Token::ATTRIB_HOLERND,"ATTRIB_HOLERND"},
		{Token::ATTRIB_LINE,"ATTRIB_LINE"},
		{Token::ATTRIB_ORIGIN,"ATTRIB_ORIGIN"},
		{Token::ATTRIB_POINT,"ATTRIB_POINT"},
		{Token::ATTRIB_POLYGON,"ATTRIB_POLYGON"},
		{Token::ATTRIB_RECT,"ATTRIB_RECT"},
		{Token::ATTRIB_RNDRECT,"ATTRIB_RNDRECT"},
		{Token::ATTRIB_RULERS,"ATTRIB_RULERS"},
		{Token::ATTRIB_TEXT,"ATTRIB_TEXT"},
		{Token::NUMINT,"<int number>"},
		{Token::NUMDOUBLE,"<double number>"},
		{Token::STRING,"<string>"},
		{Token::ERRORToken,"<Error>"},
		{Token::ENDOFTOKENS,0}
	};
	int m_UngetBuffer;
	char* m_pLexBuff;	// Buffer for the current token string
	int m_LexBuffIndex;		// index into ^
	int m_Line;
	int m_Col;
	char* m_pFileBuffeer;	// Buffer the source file is stored in
	int m_InFileSize;	// Size of the buffer
	int m_FileIndex;	// Current index into file buffer
	double m_LexValueDouble;
	int m_LexValueInt;
public:
	CLexer();
	virtual ~CLexer();
	virtual BOOL Create(CArchive& ar);
	Token LookUp(const char* pKW);
	static const char* TokenLookup(Token TokenVal);
	BOOL IsValidDigit(int c);
	BOOL IsDoubleValue(int c);
	virtual Token Lex() = 0;
	static KEYWORD* GetKEYWORD(int i) { return &KeyWords[i]; }
	int GetCol() const { return m_Col; }
	int GetLine() const  { return m_Line; }
	char* GetLexBuffer() { return m_pLexBuff; }
	int LexGet();
	void LexUnGet(int c);
	double GetLexValueDouble() const { return m_LexValueDouble; }
	double GetLexValueInt() const { return m_LexValueInt; }
	friend class CLexerFrontCad;
};

