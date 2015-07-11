var Verbose = 1;
var Location = "Omsk";
var Hourly_Vibe = "YES";
var BT_Vibe = "YES";
var Weather_Updates_Frequency = 3;
var Battery_Percents_Visibility = "YES";
var BT_State_Visibility = "YES";

var Verbose_key                     = 1;
var Location_key                    = 2;
var Hourly_Vibe_key                 = 3;
var BT_Vibe_key                     = 4;
var Weather_Updates_Frequency_key   = 5;
var Battery_Percents_Visibility_key = 6;
var BT_State_Visibility_key         = 7;



function HTTPGET(url) {
    var req = new XMLHttpRequest();
    req.open("GET", url, false);
    req.send(null);
    return req.responseText;
}

function ReadSettings(){
	Verbose = 1;
	Location = localStorage.getItem(Location_key);
	Hourly_Vibe = localStorage.getItem(Hourly_Vibe_key);
	BT_Vibe = localStorage.getItem(BT_Vibe_key);
	Weather_Updates_Frequency = localStorage.getItem(Weather_Updates_Frequency_key);
	Battery_Percents_Visibility = localStorage.getItem(Battery_Percents_Visibility_key);
	BT_State_Visibility = localStorage.getItem(BT_State_Visibility_key);
	
	if (Verbose)
		console.log('SmartFace [phone]: Location - ' + Location + ' Hourly vibration - ' + Hourly_Vibe + ' Weather refresh - ' + Weather_Updates_Frequency + ' BT vibration - ' + BT_Vibe + ' Battery percents visible: ' + Battery_Percents_Visibility + ' BT state visible: ' + BT_State_Visibility);
}

function SendSettings(){
	Pebble.sendAppMessage({'LOCATION': parseInt(Location),
						   'HOURLY_VIBE': parseInt(Hourly_Vibe),
						   'BT_VIBE': parseInt(BT_Vibe),
						   'WEATHER_UPDATES_FREQUENCY': parseInt(Weather_Updates_Frequency),
						   'BATTERY_PERSENTS_VISIBILITY': parseInt(Battery_Percents_Visibility),
						   'BT_STATE_VISIBILITY': parseInt(BT_State_Visibility)
						  });
}

function Update_Weather(){
	var CurrentWeather = '';
	var response = HTTPGET("http://api.openweathermap.org/data/2.5/weather?q=" + Location);

 	var json = JSON.parse(response);
	var temperature = (json.main.temp - 273.15);
	
	/*Just adding "+" sign for good temperature*/
	var sign = "";
	if (temperature > 0)
		sign = "+";
	
	temperature = temperature.toFixed(1);	
  
	var state = json.weather[0].main;

	if (state == "Thunderstorm")
		state = "Storm";
	if (state == "Atmosphere")
		state = "Smoke";
	
	/*Sending All data to Pebble*/
	CurrentWeather = state + ', ' + sign + temperature + "C";
	
	var dict = {"CURRENT_WEATHER" : CurrentWeather};

	Pebble.sendAppMessage(dict);
	
	if (Verbose)
		console.log('SmartFace [phone]: Weather updated: ' + CurrentWeather + ' ' + Location);
}


Pebble.addEventListener("showConfiguration",
  function(e) {
    if (Verbose)
			console.log('SmartFace [phone]: Configuration is open');
	  
    Pebble.openURL("http://grakovne.org/pebble/SmartFace/AppConfig.php?Location=" + Location + "&Weather_Updates_Frequency=" + Weather_Updates_Frequency + "&Hourly_Vibe=" + Hourly_Vibe + "&BT_Vibe=" + BT_Vibe + "&Battery_Percents_Visibility=" + Battery_Percents_Visibility + "&BT_State_Visibility=" + BT_State_Visibility);
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
	console.log("Configuration window returned: " + JSON.stringify(configuration));
	  
	localStorage.setItem(Location_key, configuration.Location);
	  
	localStorage.setItem(Hourly_Vibe_key, configuration.Hourly_Vibe);

	localStorage.setItem(BT_Vibe_key, configuration.BT_Vibe);  

	localStorage.setItem(Battery_Percents_Visibility_key, configuration.Battery_Persents_Visibility);

	localStorage.setItem(BT_State_Visibility_key, configuration.BT_State_Visibility);
	  
	localStorage.setItem(Weather_Updates_Frequency_key, configuration.Weather_Updates_Frequency);
	  
	ReadSettings();
	SendSettings();
	Update_Weather();
	
  }
);

Pebble.addEventListener("appmessage",
  function(e) {
	  if (Verbose)
			console.log('SmartFace [phone]: New message Received!');
	  
	  if ('CURRENT_WEATHER' in e.payload)
    	Update_Weather();
  }
);

Pebble.addEventListener("ready",
  function(e) {
	ReadSettings();
	SendSettings();
	Update_Weather();  
	  if (Verbose)
			console.log('SmartFace [phone]: App running OK...');
  }
);
