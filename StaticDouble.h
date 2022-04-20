#pragma once


// CStaticDouble

class CStaticDouble : public CStatic
{
	DECLARE_DYNAMIC(CStaticDouble)

public:
	CStaticDouble();
	virtual ~CStaticDouble();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetDoubleValue(double v);
};


