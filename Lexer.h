#pragma once

class CLexer
{
	int m_UnGetBuff;
	FILE *m_pIN;
	int m_LexIndex;
	char m_LexBuff[256];
	int m_Line;
	int m_Col;
	int m_LexValueInt;
	double m_LexValueDouble;
	//---------------------------------------
	// Tokens
	//---------------------------------------
public:
	enum Tokens {
		DEFAULT = -2,
		EOFToken = -1,
		BASE_OBJ,
		MATCHED,
		NUMINT = 256,
		NUMDOUBLE,
		STRING,			//1
		//--------------------------------
		// Drawing Objects
		//--------------------------------
		ARC,				//2
		ARC_CENTER,		//3
		ARROW,
		BITMAP,
		CIRCLE,			//4
		DIMENSION,
		RECT,				//5
		RNDRECT,			//6
		POLY,				//7
		LINE,				//9
		HOLERND,			//10
		HOLERECT,			//11
		HOLE_1FLAT,		//12
		HOLE_2FLAT,		//13
		LIBPART,			//14
		ORG,	//token for origin object
		POINT,
		PRINTRECT,
		TEXT,				//15
		VERTEX_POINT,
		START_POINT,
		END_POINT,
		//colors
		BK_TRANSPARENT,
		BACKGROUND_COLOR,	//17
		LINE_COLOR,
		TEXT_COLOR,
		FILL_COLOR,
		SELECTED_COLOR,
		RED,
		GREEN,
		BLUE,
		//-------
		FONT,
		FONT_WEIGHT,
		FONTHEIGHT,
		FONTWIDTH,
		FONT_FORMAT,
		FONT_NAME,
		//--------------------
		RADIUS,
		FLATDIST,
		VERTEX,
		RECTWIDTH,
		RECTHIEGHT,
		ANGLE,
		REFERENCE,
		FILENAME,
		LENGTH,
		SIZE,
		SCALE,
		LINE_WIDTH,
		ARROW_LENGTH,
		ARROW_WIDTH,
		SELECTED_UNITS,
		//-------------------
		DRAWFILE,
		LIBFILE,
		// --- Attributes ---
		ATTRIB_ARC,
		ATTRIB_ARCCENTER,
		ATTRIB_ARROW,
		ATTRIB_BITMAP,
		ATTRIB_DIMENSION,
		ATTRIB_ELLIPSE,
		ATTRIB_GRID,
		ATTRIB_HOLERECT,
		ATTRIB_HOLERND1F,
		ATTRIB_HOLERND2F,
		ATTRIB_HOLERND,
		ATTRIB_LINE,
		ATTRIB_ORIGIN,
		ATTRIB_POINT,
		ATTRIB_POLYGON,
		ATTRIB_RECT,
		ATTRIB_RNDRECT,
		ATTRIB_RULERS,
		ATTRIB_TEXT,
		ERRORToken
	};
private:
	struct KEYWORD {
		Tokens m_Token;
		const char* m_pName;
	};
	static inline KEYWORD KeyWords[] = {
		{BASE_OBJ,"BASE_OBJ"},
		{ARC,"ARC"},
		{ARC_CENTER,"ARC_CENTER"},
		{CIRCLE,"CIRCLE"},
		{RECT,"RECT"},
		{RNDRECT,"RNDRECT"},
		{POLY,"POLY"},
		{LINE,"LINE"},
		{HOLERND,"HOLERND"},
		{HOLERECT,"HOLERECT"},
		{HOLE_1FLAT,"HOLE_1FLAT"},
		{HOLE_2FLAT,"HOLE_2FLAT"},
		{LIBPART,"LIBPART"},
		{TEXT,"TEXT"},
		{POINT,"POINT"},
		{VERTEX_POINT,"VERTEX_POINT"},
		{START_POINT,"START_POINT"},
		{END_POINT,"END_POINT"},
		{BACKGROUND_COLOR,"BACKGROUND_COLOR"},
		{LINE_COLOR,"LINE_COLOR"},
		{TEXT_COLOR,"TEXT_COLOR"},
		{FILL_COLOR,"FILL_COLOR"},
		{SELECTED_COLOR,"SELECTED_COLOR"},
		{RED,"RED"},
		{GREEN,"GREEN"},
		{BLUE,"BLUE"},
		{FONT,"FONT"},
		{FONT_WEIGHT,"FONT_WEIGHT"},
		{FONTHEIGHT,"FONTHIEGHT"},
		{FONTWIDTH,"FONTWIDTH"},
		{FONT_FORMAT,"FONT_FORMAT"},
		{FONT_NAME,"FONT_NAME"},
		{RADIUS,"RADIUS"},
		{FLATDIST,"FLATDIST"},
		{DRAWFILE,"DRAWFILE"},
		{VERTEX,"VERTEX"},
		{LIBFILE,"LIBFILE"},
		{RECTWIDTH,"RECTWIDTH"},
		{RECTHIEGHT,"RECTHIEGHT"},
		{ANGLE,"ANGLE"},
		{BK_TRANSPARENT,"TRANSPARENT"},
		{REFERENCE,"REFERENCE"},
		{BITMAP,"BITMAP"},
		{FILENAME,"FILENAME"},
		{ARROW,"ARROW"},
		{LENGTH,"LENGTH"},
		{ORG,"ORG"},
		{DIMENSION,"DIMENSION"},
		{PRINTRECT,"PRINTRECT"},
		{SIZE,"SIZE"},
		{SCALE,"SCALE"},
		{LINE_WIDTH,"LINE_WIDTH"},
		{ARROW_LENGTH,"ARROW_LENGTH"},
		{ARROW_WIDTH,"ARROW_WIDTH"},
		{SELECTED_UNITS,"SELECTED_UNITS"},
		{ATTRIB_ARC,"ATTRIB_ARC"},
		{ATTRIB_ARCCENTER,"ATTRIB_ARCCENTER"},
		{ATTRIB_ARROW,"ATTRIB_ARROW"},
		{ATTRIB_BITMAP,"ATTRIB_BITMAP"},
		{ATTRIB_DIMENSION,"ATTRIB_DIMENSION"},
		{ATTRIB_ELLIPSE,"ATTRIB_ELLIPSE"},
		{ATTRIB_GRID,"ATTRIB_GRID"},
		{ATTRIB_HOLERECT,"ATTRIB_HOLERECT"},
		{ATTRIB_HOLERND1F,"ATTRIB_HOLERND1F"},
		{ATTRIB_HOLERND2F,"ATTRIB_HOLERND2F"},
		{ATTRIB_HOLERND,"ATTRIB_HOLERND"},
		{ATTRIB_LINE,"ATTRIB_LINE"},
		{ATTRIB_ORIGIN,"ATTRIB_ORIGIN"},
		{ATTRIB_POINT,"ATTRIB_POINT"},
		{ATTRIB_POLYGON,"ATTRIB_POLYGON"},
		{ATTRIB_RECT,"ATTRIB_RECT"},
		{ATTRIB_RNDRECT,"ATTRIB_RNDRECT"},
		{ATTRIB_RULERS,"ATTRIB_RULERS"},
		{ATTRIB_TEXT,"ATTRIB_TEXT"},
		{NUMINT,"<int number>"},
		{NUMDOUBLE,"<double number>"},
		{STRING,"<string>"},
		{ERRORToken,"<Error>"},
		{Tokens(0),0}
	};
public:
	CLexer();
	virtual ~CLexer();
	BOOL Create(CString& path);
	Tokens Lex();
	int GetChar();
	int UnGetChar(int c);
	Tokens LookUp(const char *pS);
	static const char* TokenLookup(Tokens TokenVal);
	BOOL IsValidDigit(int c);
	BOOL IsDoubleValue(int c);
	const char* GetLexBuff() { return m_LexBuff; }

	int GetLine() { return m_Line; }
	int GetCol() { return m_Col; }
	int GetIntLexValue() { return m_LexValueInt; }
	double GetDoubleLexValue() { return m_LexValueDouble; }
};