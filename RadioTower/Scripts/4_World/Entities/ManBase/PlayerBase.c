modded class PlayerBase
{
	bool m_IsInsideCaptureArea = false;
	
	void SetIsInsideCaptureArea(bool value)
	{
		m_IsInsideCaptureArea = value;
	}
	
	bool GetIsInsideCaptureArea()
	{
		return m_IsInsideCaptureArea;
	}
}