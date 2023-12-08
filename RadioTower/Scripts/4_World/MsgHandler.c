class RTMsgHandler
{
	static void RTSendChatMessage(string message)
	{
		ref array<Man> players = new array<Man>;
		// Get array of active players
		GetGame().GetPlayers(players);
		// Build message parameter
		Param1<string> m_MessageParam = new Param1<string>(message);
		// Loop through player array
		foreach(auto player : players)
		{
			// Send player text message
			GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity());
		}
	}
	
	static void RTSendPrivateChatMessage(string message, PlayerBase player )
	{
		// Build message parameter
		Param1<string> m_MessageParam = new Param1<string>(message);
		// Loop through player array

		// Send player text message
		GetGame().RPCSingleParam(player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity());
	}
	
	static void RTSendClientAlert(string icon, string message, int duration) {
		// If icon is null then use default
		if (!icon) {
			icon = RTConstants.RT_ICON;
		}
		NotificationSystem.SendNotificationToPlayerIdentityExtended(null, duration, "RadioTower...", message, icon);
	}
}