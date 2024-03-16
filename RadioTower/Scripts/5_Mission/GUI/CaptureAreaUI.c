class CaptureAreaUI: UIScriptedMenu
{
    protected Widget m_RootWidget;
    protected Widget m_CaptureFrameWidget;
    protected TextWidget m_PlayersInCaptureZoneTextWidget;
	protected TextWidget m_CaptureTextWidget;
	protected ProgressBarWidget m_ProgressBarWidget;
	protected bool m_IsVisible;
	protected float m_X;
	protected float m_Y;

    void CaptureAreaUI()
    {
		m_IsVisible = false;
        m_RootWidget = GetGame().GetWorkspace().CreateWidgets("RadioTower/RadioTower/GUI/Capture.layout");
        m_CaptureFrameWidget = Widget.Cast( m_RootWidget.FindAnyWidget("CaptureFrame"));
		m_CaptureFrameWidget.GetScreenPos(m_X, m_Y);
        m_PlayersInCaptureZoneTextWidget = TextWidget.Cast( m_RootWidget.FindAnyWidget("PlayersInCaptureZoneValueText"));
		m_CaptureTextWidget = TextWidget.Cast(m_RootWidget.FindAnyWidget("CaptureText"));
		m_ProgressBarWidget = ProgressBarWidget.Cast(m_RootWidget.FindAnyWidget("CaptureProgressBar"));
        m_RootWidget.Show(false);
    }

    void ~CaptureAreaUI()
    {        
        if (m_RootWidget != null)
        {
            m_RootWidget.Show(false);
            m_RootWidget.Unlink();
        }
    }
	
	void SetYOffset(float offset)
	{
		m_CaptureFrameWidget.SetScreenPos(m_X, m_Y + offset);
	}
	
	void ToggleCaptureUI()
	{
		m_IsVisible = !m_IsVisible;
		m_RootWidget.Show(m_IsVisible);
	}
	
	bool IsCaptureUIVisible()
	{
		return m_IsVisible;
	}
	
	void SetInsiderCount(string text)
	{
		m_PlayersInCaptureZoneTextWidget.SetText(text);
	}
	
	void SetCaptureProgress(float value)
	{
		m_ProgressBarWidget.SetCurrent(value);
		SetCaptureText(value);
	}
	
	void SetCaptureText(float value)
	{
		string text = MiscGameplayFunctions.TruncateToS(value, 2) + "% captured";
		//string text = value.ToString() + "% captured";
		m_CaptureTextWidget.SetText(text);
	}
};