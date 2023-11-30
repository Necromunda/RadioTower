// class ExampleUI: UIScriptedMenu
// {
    // protected Widget m_RootWidget;
    // protected TextWidget m_TextWidget;

    // void ExampleUI(PlayerBase player)
    // {
        // m_RootWidget = GetGame().GetWorkspace().CreateWidgets("MyModName/GUI/Layouts/MyLayout.layout");
        // m_TextWidget = TextWidget.Cast( m_RootWidget.FindAnyWidget("m_TextWidget") );
        // m_RootWidget.Show(false);

        // ExampleFunction();
    // }

    // void ~ExampleUI()
    // {        
        // if (m_RootWidget != null)
        // {
            // m_RootWidget.Show(false);
            // m_RootWidget.Unlink();
        // }
    // }

    // void ExampleFunction()
    // {
        // m_TextWidget.SetColor(-16777216);
        // m_TextWidget.SetText("Hello World ! I was able to load my layout :)");

        // m_RootWidget.Show(true);
    // }
// };