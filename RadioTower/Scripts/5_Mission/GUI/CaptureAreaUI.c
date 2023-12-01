class CaptureAreaUI: UIScriptedMenu
{
    protected Widget m_RootWidget;
    //protected TextWidget m_TextWidget;
	protected ProgressBarWidget m_ProgressBarWidget;
	protected bool m_IsVisible;

    void CaptureAreaUI()
    {
		m_IsVisible = false;
        m_RootWidget = GetGame().GetWorkspace().CreateWidgets("RadioTower/RadioTower/GUI/Capture.layout");
        //m_TextWidget = TextWidget.Cast( m_RootWidget.FindAnyWidget("m_TextWidget") );
		m_ProgressBarWidget = ProgressBarWidget.Cast(m_RootWidget.FindAnyWidget("CaptureProgressBar"));
        m_RootWidget.Show(false);

        //ExampleFunction();
    }

    void ~CaptureAreaUI()
    {        
        if (m_RootWidget != null)
        {
            m_RootWidget.Show(false);
            m_RootWidget.Unlink();
        }
    }
	
	void ToggleCaptureUI()
	{
		m_IsVisible = !m_IsVisible;
		m_RootWidget.Show(m_IsVisible);
		Print(RTConstants.RT_LOG_PREFIX + " Show menu: " + m_IsVisible);
	}
	
	bool IsCaptureUIVisible()
	{
		return m_IsVisible;
	}
	
    void ExampleFunction()
    {
        //m_TextWidget.SetColor(-16777216);
        //m_TextWidget.SetText("Hello World ! I was able to load my layout :)");

        m_RootWidget.Show(true);
    }
};