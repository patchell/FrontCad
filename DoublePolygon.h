#pragma once

class CDoublePolygon
{
	CDoublePoint* m_pointsPolygon;
	UINT m_numPoints;
	UINT m_PointAddedCount;
	CDoublePoint m_pointPivot;
	double Angle;
public:
	CDoublePolygon();
	~CDoublePolygon();
	void Destroy() {
		if (m_pointsPolygon) delete[]m_pointsPolygon;
		m_pointsPolygon = 0;
		m_numPoints = 0;
		m_PointAddedCount = 0;
	}
	void Create(UINT numberOfPoints, CDoublePoint* pPoints = NULL);
	void Fill(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale, COLORREF color);
	void ConnectTheDots(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	void Draw(CDC* pDC, MODE mode, CDoublePoint& ULHC, CScale& Scale);
	void Copy(CDoublePolygon* pPoly) {
		m_numPoints = pPoly->GetSize();
		for (size_t i = 0; i < m_numPoints; i++)
			m_pointsPolygon[i] = pPoly->GetPoints()[i];
	}
	BOOL PointInPolygon(CDoublePoint Point);
	CDoublePoint* GetPoints() { return m_pointsPolygon; }
	void SetSize(UINT size);
	UINT GetSize() { return m_numPoints; }
	UINT GetPointCount() { return m_PointAddedCount; }
	BOOL AddPoint(CDoublePoint p);
	void SetPoint(CDoublePoint p) { m_pointsPolygon[m_PointAddedCount] = p; }
	void SetPoint(int v, CDoublePoint p) { m_pointsPolygon[v] = p; }
	CDoublePoint GetPoint(int v) { return m_pointsPolygon[v]; }
	CDoublePoint GetLastPoint() { return m_pointsPolygon[m_PointAddedCount - 1]; }
	UINT DeleteLastPoint() { 
		if (m_PointAddedCount)--m_PointAddedCount; 
		return m_PointAddedCount;
	}
	CDoublePoint GetCenter();
};

