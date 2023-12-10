class RTConstants
{
	/* ACTION CONDITION DEFAULTS */
	const float RT_DISTANCE_DEFAULT = 2.0;
	/* EVENT DEFAULTS */
	const float RT_EVENT_SPAWN_INTERVAL_DEFAULT = 60;
	const float RT_EVENT_CAPTURETIME_DEFAULT = 600;
	const float RT_EVENT_LIFETIME_DEFAULT = 1800;
	const float RT_EVENT_TOTAL_CAPTURE_PCT_DEFAULT = 100;
	const float RT_EVENT_UPDATE_PROGRESS_INTERVAL_DEFAULT = 1;
    const float RT_EVENT_TRIGGER_HEIGHT_DEFAULT = 25;
	const float RT_EVENT_TRIGGER_RADIUS_DEFAULT = 25;
	const float RT_TIMETOHACK_DEFAULT = 1;
	const bool RT_ALLOW_SAME_EVENT_SPAWN_IN_A_ROW = true;
	const bool RT_ENABLE_LOGGING = false;
	/* PATHS */
	const string RT_ROOTPATH = "$profile:RadioTower/";
	const string RT_LOGPATH = "$profile:RadioTower/Logs/";
    const string RT_SETTINGS_CONFIGPATH = "$profile:RadioTower/RTSettings.json";
	const string RT_LOCATIONS_CONFIGPATH = "$profile:RadioTower/RTLocations.json";
	const string RT_PROPS_CONFIGPATH = "$profile:RadioTower/RTProps.json";
	/* LOGGING */
	const string RT_LOG_PREFIX = "RT_";
	/* ICONS */
	const string RT_ICON = "RadioTower/RadioTower/Data/Icons/rt_logo.paa";
};