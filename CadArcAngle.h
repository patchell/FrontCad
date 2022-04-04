#pragma once


class CCadArcAngle :  public CCadObject
{
	inline static int m_ArcCount;
	inline static SArcAngleAttributes m_LastAttributes;
	inline static SArcAngleAttributes m_CurrentAttributes;
	inline static BOOL m_AttributesGood;
	inline static BOOL m_RenderEnable = TRUE;
	CPoint m_pointCenter;
	UINT m_Radius;
	FLOAT m_StartAngle;
	FLOAT m_EndAngle;
	SArcAngleAttributes m_Attributes;
public:
	CCadArcAngle();
	virtual ~CCadArcAngle();
	//--------------------------------------
	// Attributes Methods
	//------------------------------------
	SArcAngleAttributes& GetAttributes() { return m_Attributes; }
	virtual BOOL NeedsAttributes();
	virtual void ClearNeedsAttributes();
	//--------------------------------------------
	static void SetRenderEnable(BOOL e) { m_RenderEnable = e; }
	static BOOL IsRenderEnabled() { return m_RenderEnable; }
	SArcAngleAttributes* GetLastAttributes() const {
		return &m_LastAttributes;
	}
	//---------------------------------------------
	// Draw Object Methodes
	//---------------------------------------------
	virtual ObjectDrawState ProcessDrawMode(ObjectDrawState DrawState);
};

