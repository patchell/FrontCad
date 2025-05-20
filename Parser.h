#pragma once

class CParser
{
	friend class CParserFrontCad;
	Token m_LookaHeadToken;
	CLexer* m_pLexer;
public:
	CParser();
	virtual ~CParser();
	BOOL Create(CArchive& ar);
	virtual BOOL Parse(CCadObject* pCadObj) = 0;
	void Expect(Token ExpectedToken) ;
	BOOL Accept(Token ExpectedToken) const;
	CLexer* GetLexer() { return m_pLexer; }
	Token GetLookaHead() const { return m_LookaHeadToken; }
	void Color(
		Token TargetToken,	//type of color object 
		COLORREF& ColorParam		//Color Value to set 
	);
	void IntValue(
		Token TargetToken,	//type of color object 
		int& IntegerValue			//Integer Value to set 
	);
	void UINTValue(
		Token TargetToken,
		UINT& UINTValue
	);
	void DoubleValue(
		Token TargetToken,
		double& DoubleValue
	);
	void StringValue(
		Token TargetToken,
		CString& csS
	);
	//----------------------------------
	// Save Util Methods
	//----------------------------------
	static int SaveString(
		char* s,
		int n,
		Token TokenValue,
		CString& csString
	);
	static char* SaveStringS(
		char* s,
		int n,
		Token TokenValue,
		CString& csString
	);
	static int SaveSize(
		char* s,
		int n,
		Token TokenValue,
		CDoubleSize& size
	);
	static int SaveColor(
		char* s,
		int n,
		Token TokenValue,
		COLORREF color
	);
	static char* SaveColorS(
		char* s,
		int n,
		Token TokenValue,
		COLORREF color
	);
	static int SaveIntValue(
		char* s,
		int n,
		Token  TokenValue,
		int& value
	);
	static char* SaveIntValueS(
		char* s,
		int n,
		Token  TokenValue,
		int& value
	);
	static int SaveUINTValue(
		char* s, 
		int n, 
		Token TokenValue, 
		UINT& value
	);
	static char* SaveUINTValueS(
		char* s,
		int n,
		Token TokenValue,
		UINT& value
	);
	static int SaveDoubleValue(
		char* s,
		int n,
		Token TokenValue,
		double& value
	);
	static char* SaveDoubleValueS(
		char* s,
		int n,
		Token TokenValue,
		double& value
	);
	static int SaveDoubleValue(char* s, int n, double& v);
	static char* SaveDoubleValueS(char* s, int n, double& v);
};

