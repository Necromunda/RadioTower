modded class ActionConstructor
{
    override void RegisterActions (TTypenameArray actions)
    {
        super.RegisterActions(actions);
        actions.Insert(ActionOpenRTServerConsole);
        actions.Insert(ActionCloseRTServerConsole);
        actions.Insert(ActionHackRTServerConsole);
    };
};