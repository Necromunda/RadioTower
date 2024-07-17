enum RTLogType
{
	INFO,
	DEBUG,
	WARNING,
	ERROR
}

class RTLogger
{
	protected static ref RTLogger instance; 
	
	protected string configPath = RTConstants.RT_LOGPATH + RTConstants.RT_LOG_PREFIX; 
	protected string logFileName; 
	protected string logFilePath;
	protected bool createLogs;
	
	void RTLogger(bool doLogs)
	{
		Print("[RadioTower] Logger created!");
		
		logFileName = GetCurrentDateAndTimePathFriendly() + ".txt";
		logFilePath = configPath + logFileName;
		createLogs = doLogs;
		
		Print("[RadioTower] Session logfile: " + RTConstants.RT_LOG_PREFIX + logFileName);
	}
	
	string GetLogTypePrefix(RTLogType type)
	{
		string prefix = "[INFO]";
		switch (type)
		{
			case RTLogType.DEBUG:
				prefix = "[DEBUG]";
				break;
			case RTLogType.WARNING:
				prefix = "[WARNING]";
				break;
			case RTLogType.ERROR:
				prefix = "[ERROR]";
				break;
		}
		
		return prefix;
	}
	
	string GetCurrentDateAndTimePretty()
	{
		return "[" + GetCurrentDateAndTime() + "]";
	}
	
	void LogMessage(RTLogType type, string message)
	{
		if (!createLogs) return;
		
		FileHandle fileHandle = OpenFile(logFilePath, FileMode.APPEND);
		
		if (fileHandle != 0)
		{
			string msg = GetCurrentDateAndTimePretty() + " " + GetLogTypePrefix(type) + " " + message;
			FPrintln(fileHandle, msg);
			CloseFile(fileHandle);
		}
	}
	
	void LogCaptureEvent()
	{
		if (!createLogs) return;
		
		FileHandle fileHandle = OpenFile(logFilePath, FileMode.APPEND);
		
		if (fileHandle != 0)
		{
			//FPrintln(fileHandle, "[" + GetCurrentDateAndTime() + "]	" + message);
			CloseFile(fileHandle);
		}
	}

	string GetCurrentTime()
	{
		int hours, minutes, seconds;
		string h, m, s; 
		GetHourMinuteSecond(hours, minutes, seconds);
		
		h = hours.ToString();
		m = minutes.ToString();
		s = seconds.ToString();
		if(hours < 10)
		{
			h = "0" + h;
		}
		
		if(minutes < 10)
		{
			m = "0" + m;
		}
		
		if(seconds < 10)
		{
			s = "0" + s;
		}
		
		return h + "-" + m + "-" + s;
	}
	
	string GetCurrentDate()
	{	
		int days, months, years;
		string d, m, y;
		GetYearMonthDay(years, months, days);
		
		d = days.ToString();
		m = months.ToString();
		y = years.ToString();
		if(days < 10)
		{
			d = "0" + d;
		}
		
		if(months < 10)
		{
			m = "0" + m;
		}
		
		if(years < 10)
		{
			y = "0" + y;
		}
		
		return d + "-" + m + "-" + y;
	}
	
	string GetCurrentDateAndTimePathFriendly()
	{		
		return GetCurrentDate() + "_" + GetCurrentTime();
	}
	
	string GetCurrentDateAndTime()
	{
		string date = GetCurrentDate();
		string time = GetCurrentTime();
		date.Replace("-", "/");
		time.Replace("-", ":");
		
		return date + "-" + time;
	}
	
	static RTLogger GetInstance()
	{	
		return instance;
	}
	
	static void ClearInstance()
	{
		delete instance; 
	}
	
	static RTLogger CreateInstance(bool doLogs = false)
	{
		ClearInstance();
		instance = new RTLogger(doLogs); 
		return instance;
	}
}