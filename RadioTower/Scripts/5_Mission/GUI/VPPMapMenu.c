#ifdef VanillaPPMap
modded class VPPMapMenu extends UIScriptedMenu {
   override void DisplayServerMarkers() 
   {
		super.DisplayServerMarkers();
		if (m_MapWidget)
		{	
			ref RTLocation currentLocation = g_Game.GetLatestEventLocation();
			ref RTSettings settings = g_Game.GetRTSettings();
			if (currentLocation)
			{
				if (settings && settings.mapMarkers)
				{
					/*
					string mapMarkerText = settings.mapMarkers.mapMarkerText
					if (!mapMarkerText.Contains("%"))
					{
						mapMarkerText = RTConstants.RT_MAP_MARKER_TEXT;
					}
					string title = string.Format(mapMarkerText, currentLocation.locationTitle);
					*/
					string mapMarkerText = settings.mapMarkers.mapMarkerText
					if (mapMarkerText.Contains("%1"))
					{
						mapMarkerText = string.Format(mapMarkerText, currentLocation.locationTitle);
					}
					m_MapWidget.AddUserMark(currentLocation.locationCoordinatesXYZ, mapMarkerText, ARGB(255, 255, 0, 0), "VanillaPPMap\\GUI\\Textures\\CustomMapIcons\\waypoint_CA.paa");
				}
			}
		}
		m_Adapter.UpdateContent();
    }
}
#endif