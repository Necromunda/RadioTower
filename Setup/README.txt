IMPORTANT: Always check the config with some json validator after editing!

1. Add these to cfgignorelist.xml
<type name="RTServer"></type>
<type name="CaptureArea"></type>
<type name="RTLootcrate_Grey"></type>
<type name="RTLootcrate_White"></type>
<type name="RTLootcrate_Red"></type>
<type name="RTLootcrate_Yellow"></type>

2. Settings explanation
<------------------------------------------RTSettings.json------------------------------------------->
Type: int
Description: Duration in seconds between event spawns
"eventSpawnInterval": 2700

Type: int
Description: Duration in seconds during which event remains capturable after spawn, preferebly higher than "eventCapturetime"
"eventLifetime": 1800

Type: int
Description: Duration in seconds players need to be inside the capture area for event to become captured, must be lower than "eventLifetime"
"eventCapturetime": 300

Type: int
Description: Duration in seconds it takes for the server to get hacked and start the capturing process
"eventHacktime": 4

Type: string
Description: Default lootcrate, Use any container classname you wish but i recommend the use of one of these: "RTLootcrate_White", "RTLootcrate_Gray", "RTLootcrate_Red" or "RTLootcrate_Yellow"
"eventDefaultLootcrate": "RTLootcrate_White"

Type: bool
Description: Set 1 to create log files in $profile:RadioTower/Logs, otherwise 0
"enableLogging": 1

Type: bool
Description: Set 1 to allow event spawning to same location back to back, otherwise 0
"allowSameEventSpawnInARow": 0

Type: bool
Description: Set 1 to allow more than one event at the same time, otherwise 0. Only set to 1 if "prioritizeOldEvent" is 0 or vice versa
"enableConcurrentEvents": 0

Type: bool
Description: Set 1 to allow chat and popup notifications, otherwise 0
"enableNotifications": 1

Type: bool
Description: Set 1 to allow event creation notification, otherwise 0. Requires "enableNotifications" to be 1 to work
"enableEventCreateNotification": 1

Type: bool
Description: Set 1 to allow event capture notification, otherwise 0. Requires "enableNotifications" to be 1 to work
"enableEventCaptureNotification": 1

Type: bool
Description: Set 1 to allow event end notification, otherwise 0. Requires "enableNotifications" to be 1 to work
"enableEventEndNotification": 1

Type: bool
Description: Set 1 to not allow new event to spawn if old event is not captured or has lifetime left, otherwise 0. Only set to 1 if "enableConcurrentEvents" is 0 or vice versa
"prioritizeOldEvent": 1

Type: bool
Description: Set 1 to allow zombie spawning to capture area, otherwise 0
"spawnZombies": 0

Type: bool
Description: Set 1 to show colored smoke indicating capture progress, otherwise 0
"showCaptureStatusSmoke": 1

Type: bool
Description: Set 1 to show a marker on the map if your server has the LBMaster group mod, otherwise 0
"enableLBMapMarker": 0

Type: int
Description: Minimum players needed for the event to spawn
"minimumPlayerCount": 5


<------------------------------------------RTLocations.json------------------------------------------->
Type: float
Description: Capture area radius, Cylinder shape. Middle point is on server object
"captureAreaRadius": 10.0

Type: float
Description: Capture area height, Cylinder shape
"captureAreaHeight": 10.0

Type: float
Description: Capture area offset for Y axis, bottom of the capture area is on the same level as bottom of the server object. Use negative values to lower the cylinder and positive values to raise it
"captureAreaYAxisOffset": 0

Type: string
Description: Name for your location, Used in notifications. Must be unique between other location names!
"locationTitle": "Vybor Military Complex"

Type: string
Description: Set to override the default lootcrate, otherwise leave empty. Options: "RTLootcrate_White", "RTLootcrate_Gray", "RTLootcrate_Red" or "RTLootcrate_Yellow". Set "" if not using
"lootcrateClassName": ""

Type: vector
Description: Coordinates for the server object. Recommended to use RTDebug_Server object in offline mode to get coordinates
"locationCoordinatesXYZ": [ 4520.569824, 316.013977, 8273.550781 ]

Type: vector
Description: Orientation for the server object. Recommended to use RTDebug_Server object in offline mode to get orientation
"locationOrientationYPR": [ 12.657836, 2.585529, -0.357249 ]

Type: vector
Description: Coordinates for the lootcrate object. Recommended to use RTDebug_Lootcrate object in offline mode to get coordinates
"lootcrateCoordinatesXYZ": [ 4524.421875, 316.124512, 8273.518555]

Type: vector
Description: Orientation for the lootcrate object. Recommended to use RTDebug_Lootcrate object in offline mode to get orientation
"lootcrateOrientationYPR": [-77.772049, -2.510887, -2.104894 ]

Type: vector
Description: Optional. Coordinates for the loot vehicle. Set [] if not using
"vehicleCoordinatesXYZ": [ 4530.252930, 316.095093, 8273.514648 ]

Type: vector
Description: Optional. Orientation for the loot vehicle. Set [] if not using
"vehicleOrientationYPR": [ 15.248120, 0, 0 ]

Type: float
Description: Optional. Probability for the loot vehicle, 1.0 = 100% chance. Set 0 if not using
"vehicleProbability": 1.0

Type: string
Description: Optional. Loot vehicle classname. Set "" if not using
"vehicleClassName": "OffroadHatchback"

Type: array<string>
Description: Optional. Loot vehicle attachments. Use classnames. Set [] if not using
"vehicleAttachments": ["HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackWheel", "HatchbackDoors_Driver" ]

Type: array<object>
Description: Loot array. Set [] if not using
"loot": []

Type: string
Description: Loot object classname
"lootClassName": "M4A1"

Type: int
Description: Only set for items that can be stacked!
"quantity": 0

Type: array<string>
Description: Item attachment classnames
"attachments": [ "M4Suppressor" ]


<------------------------------------------RTProps.json------------------------------------------->
Type: array<object>
Description: Location prop object array. Set [] if not using
"eventProps": []

Type: string
Description: Must match with "locationTitle" in RTLocations.json
"locationTitle": "Green mountain"

Type: array<object>
Description: Prop object array. Set [] if not using
"locationProps": []

Type: string
Description: Use classname for whatever object you want to spawn
"propClassName": "Wreck_UH1Y",

Type: vector
Description: Coordinates for the prop
"propCoordinatesXYZ": [ 3707.654541, 403.856934, 5970.361328 ]

Type: vector
Description: Orientation for the prop
"propOrientationYPR": [ 85.999680, 0.000000, 0.000000 ]
