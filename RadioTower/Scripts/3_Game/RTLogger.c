// TODO. Create logs into Profiles/RadioTower/Logs
class RTLogger
{
	static const string m_RTPrefix = "[RadioTower]";
	
	static void Log(string msg, bool timestamp = true, bool prefix = true)
	{
		/*
		string date = CF_Date.Now().Format("DD-MM-YYYY hh:mm:ss");
		string message = "";
		
		if (timestamp && prefix)
			message = date + " " + m_RTPrefix;
		
		if (!timestamp && prefix)
			message = m_RTPrefix;
		
		if (GetGame().IsDedicatedServer())
			message += " Server: " + msg;
		
		if (GetGame().IsClient())
			message += " Client: " + msg;
		
		Print(message);
		*/
	}
}