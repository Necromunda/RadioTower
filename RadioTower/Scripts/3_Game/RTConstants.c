class RTConstants
{
	/* ACTION CONDITION DEFAULTS */
	const float RT_DISTANCE_DEFAULT = 2.0;
	/* EVENT DEFAULTS */
	const float RT_EVENT_CAPTURETIME_DEFAULT = 600;
	const float RT_EVENT_LIFETIME_DEFAULT = 1800;
	const float RT_EVENT_TOTAL_CAPTURE_PCT_DEFAULT = 100;
	const float RT_EVENT_UPDATE_PROGRESS_INTERVAL_DEFAULT = 1;
    const float RT_EVENT_TRIGGER_HEIGHT_DEFAULT = 25;
	const float RT_EVENT_TRIGGER_RADIUS_DEFAULT = 25;
	const float RT_TIMETOHACK_DEFAULT = 3;
	/* PATHS */
	const string RT_DIRPATH = "$profile:RadioTower/";
    const string RT_CONFIGPATH = "$profile:RadioTower/RadioTowerSettings.json";
	const string RT_LOGPATH = "$profile:RadioTower/Logs/";
	/* LOGGING */
	const string RT_LOG_PREFIX = "[RadioTower]";
};