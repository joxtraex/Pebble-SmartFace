var Verbose = 1;
var Location = "Omsk";
var Hourly_Vibe = "YES";
var BT_Vibe = "YES";
var Weather_Updates_Frequency = 3;

var Verbose_key                     = 1;
var Location_key                    = 2;
var Hourly_Vibe_key                 = 3;
var BT_Vibe_key                     = 4;
var Weather_Updates_Frequency_key   = 5;

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
	
	if (Verbose)
		console.log('SmartFace [phone]: Location - ' + Location + '; Hourly vibration - ' + Hourly_Vibe + '; Weather refresh - ' + Weather_Updates_Frequency + '; BT vibration - ' + BT_Vibe);
}

function SendSettings(){
	Pebble.sendAppMessage({'LOCATION': parseInt(Location),
							'HOURLY_VIBE': parseInt(Hourly_Vibe),
							'BT_VIBE': parseInt(BT_Vibe),
							'WEATHER_UPDATES_FREQUENCY': parseInt(Weather_Updates_Frequency)
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
  
    Pebble.openURL("http://grakovne.org/pebble/SmartFace/AppConfig.php?Location=" + Location + "&Weather_Updates_Frequency=" + Weather_Updates_Frequency + "&Hourly_Vibe=" + Hourly_Vibe + "&BT_Vibe=" + BT_Vibe);
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
  
	localStorage.setItem(Location_key, configuration.Location);
	
	localStorage.setItem(Hourly_Vibe_key, configuration.Hourly_Vibe);

	localStorage.setItem(BT_Vibe_key, configuration.BT_Vibe);  
	 
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
