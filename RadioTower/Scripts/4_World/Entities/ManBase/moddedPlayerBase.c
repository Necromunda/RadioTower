modded class PlayerBase extends ManBase
{
	protected bool m_IsInsideCaptureArea;
	
	override void Init()
	{
		super.Init();
		
		m_IsInsideCaptureArea = false;
	};
	
	void SetIsInsideCaptureArea(bool value)
	{
		m_IsInsideCaptureArea = value;
	}
	
	bool GetIsInsideCaptureArea()
	{
		return m_IsInsideCaptureArea;
	}
}