#ifdef BASICMAP
modded class BasicMapMenu extends UIScriptedMenu 
{
	override void UpdateMarkers()
	{
		super.UpdateMarkers();
		ref RTLocation currentLocation = g_Game.GetLatestEventLocation();
		ref RTSettings settings = g_Game.GetRTSettings();
		if (currentLocation)
		{
			if (settings && settings.mapMarkers)
			{
				string mapMarkerText = settings.mapMarkers.mapMarkerText
				if (mapMarkerText.Contains("%1"))
				{
					mapMarkerText = string.Format(mapMarkerText, currentLocation.locationTitle);
				}
				m_Map.AddUserMark(currentLocation.locationCoordinatesXYZ, mapMarkerText, ARGB(255, 255, 0, 0), "BasicMap\\gui\\images\\marker.paa");
				m_Map.AddUserMark(currentLocation.locationCoordinatesXYZ, mapMarkerText, ARGB(255, 255, 0, 0), "BasicMap\\gui\\images\\marker.paa");
				RefreshMarkerList();
			}
		}
	}
}
#endif