enum RTEventState
{
	DELETED,
	ACTIVE,
	CAPTURING,
	CAPTURED
}

enum RTNotificationState
{
	DISABLED = 0,
	ENABLED = 1
}

enum RTNotificationType
{
	CREATE,
	CAPTURE,
	END,
	PLAYER_ENTER,
	PLAYER_LEAVE
}

enum RTEventType
{
	NORMAL = 0,
	GAS = 1
}