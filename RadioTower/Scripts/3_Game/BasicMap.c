#ifdef BASICMAP
modded class BasicMapMenu extends UIScriptedMenu 
{
	ref BasicMapCircleMarker cMarker = null;
	
	override void UpdateMarkers()
	{
		if (cMarker) 
		{		
			BasicMap().RemoveMarker(BasicMapController.SERVER_KEY, cMarker);
			cMarker = null;
		}
		
		super.UpdateMarkers();
		ref RTLocation currentLocation = g_Game.GetLatestEventLocation();
		ref RTSettings settings = g_Game.GetRTSettings();
		if (currentLocation && settings && settings.mapMarkers && settings.mapMarkers.enableBasicMapMarker)
		{
			string mapMarkerText = settings.mapMarkers.mapMarkerText
			if (mapMarkerText.Contains("%1"))
			{
				mapMarkerText = string.Format(mapMarkerText, currentLocation.locationTitle);
			}

			cMarker = BasicMapCircleMarker(mapMarkerText, currentLocation.locationCoordinatesXYZ);
			cMarker.SetRadius(currentLocation.captureAreaRadius);
			cMarker.SetHideOnPanel(1);
			cMarker.SetARGB(255, 255, 0, 0);
			cMarker.SetCanEdit(false);
			
			BasicMap().AddMarker(BasicMapController.SERVER_KEY, cMarker);
			m_Map.AddUserMark(currentLocation.locationCoordinatesXYZ, mapMarkerText, ARGB(255, 255, 0, 0), "BasicMap\\gui\\images\\marker.paa");
			RefreshMarkerList();
		}
	}
}
#endif