var Location;
var Hourly_Vibe;
var BT_Vibe ;
var Info_Updates_Frequency;
var Add_String;
var Language;
var Inverted;
var Hide_Weather;
var Charge_Vibe;
var Hide_Battery;
var Hide_BT;

var Location_key                    = 1;
var Hourly_Vibe_key                 = 2;
var BT_Vibe_key                     = 3;
var Info_Updates_Frequency_key      = 4;
var Add_String_key                  = 5;
var Language_key                    = 6;
var Inverted_key                    = 7;
var Hide_Weather_key                = 8;
var Charge_Vibe_key                 = 9;
var Hide_Battery_key                =10;
var Hide_BT_key                     =11;

function HTTPGET(url) {
    var req = new XMLHttpRequest();
	for (var i = 0; i < 2; i++){
		try {
			req.open("GET", url, false);
			req.send(null);
            }
		catch (err) {}
		if(req.status == 200)
			return req.responseText;
	}
	return 0;
}

function ReadSettings(){
	Location = localStorage.getItem(Location_key);
	Hourly_Vibe = localStorage.getItem(Hourly_Vibe_key);
	BT_Vibe = localStorage.getItem(BT_Vibe_key);
	Info_Updates_Frequency = localStorage.getItem(Info_Updates_Frequency_key);
	Add_String = localStorage.getItem(Add_String_key);
	Language = localStorage.getItem(Language_key);
	Inverted = localStorage.getItem(Inverted_key);
	Hide_Weather = localStorage.getItem(Hide_Weather_key);
	Charge_Vibe = localStorage.getItem(Charge_Vibe_key);
	Hide_Battery = localStorage.getItem(Hide_Battery_key);
	Hide_BT = localStorage.getItem(Hide_BT_key);
	
	if (!Location)
		Location = "London";
	if (!Hourly_Vibe)
		Hourly_Vibe = 1;
	if (!BT_Vibe)
		BT_Vibe = 1;
	if (!Info_Updates_Frequency)
		Info_Updates_Frequency = 1;
	if (!Add_String)
		Add_String = "Empty";
	if (!Language)
		Language = 0;
	if (!Inverted)
		Inverted = 0;
	if (!Hide_Weather)
		Hide_Weather = 0;
	if (!Charge_Vibe)
		Charge_Vibe = 1;
	if (!Hide_Battery)
		Hide_Battery = 0;
	if (!Hide_BT)
		Hide_BT = 0;
		
	//console.log('SmartFace [phone]: Location - ' + Location + '; Hourly vibration - ' + Hourly_Vibe + '; Info refresh - ' + Info_Updates_Frequency + '; BT vibration - ' + BT_Vibe + '; Language - ' + Language + '; Inverted - ' + Inverted + "; Hide Weather - " + Hide_Weather + "; Charging Vibe - " + Charge_Vibe + "; Hiding Battery text - " + Hide_Battery + "; Hiding BT-state text - " + Hide_BT);
}

function SendSettings(){
	Pebble.sendAppMessage({
							'HOURLY_VIBE'             : parseInt(Hourly_Vibe),
							'BT_VIBE'                 : parseInt(BT_Vibe),
							'INFO_UPDATES_FREQUENCY'  : parseInt(Info_Updates_Frequency),
							'LANGUAGE'                : parseInt(Language),
							'INVERTED'                : parseInt (Inverted),
							'CHARGE_VIBE'             : parseInt(Charge_Vibe),
							'HIDE_BATTERY'            : parseInt(Hide_Battery),
							'HIDE_BT'                 : parseInt(Hide_BT),
							'HIDE_WEATHER'            : parseInt(Hide_Weather),
							}); 
}

function Update_Info(){
	var CurrentWeather = " ";
	var response = " ";
	if (Hide_Weather == 0){
		response = HTTPGET("http://api.openweathermap.org/data/2.5/weather?q=" + Location);
		var json = JSON.parse(response);
		var temperature = (json.main.temp - 273.15);
	
		/*Just adding "+" sign for good temperature*/
		var sign = "";
		if (temperature > 0)
			sign = "+";
	
		temperature = temperature.toFixed(1);	
  
		var state = json.weather[0].main;

		switch (state){
			case "Thunderstorm":
				state = "Storm";
				break;
			case "Atmosphere":
				state = "Smoke";
		}
	
		if (Language == 1)
			switch(state){
				case "Storm":
					state = "Гроза";
					break;
				case "Drizzle":
					state = "Морось";
					break;
				case "Rain":
					state = "Дождь";
					break;
				case "Snow":
					state = "Снег";
					break;
				case "Smoke":
					state = "Туман";
					break;
				case "Clouds":
					state = "Облачно";
					break;
				case "Extreme":
					state = "Шторм";
					break;
				case "Clear":
					state = "Ясно";
					break;
				case "Mist":
					state = "Туман";
			}
		CurrentWeather = state + ', ' + sign + temperature + "C";
	}
	
	else
		CurrentWeather = " ";
		
	switch (Add_String){
		case "Empty":
			response = " ";
			break;
		
		case "Location":
			response = Location;
			if (response.length > 10)
				response = response.substr(0, 10) + "...";
			break;
			
		default:
			response = HTTPGET("http://grakovne.org/pebble/SmartFace/CustomStings/" + Add_String + ".php");
	}

	Pebble.sendAppMessage({"CURRENT_WEATHER" : CurrentWeather, "ADD_INFO" : response});
	
	//console.log('SmartFace [phone]: Info updated: ' + CurrentWeather + ' ' + Location + '; ' + response);
}

Pebble.addEventListener("showConfiguration",
  function(e) {
    //console.log('SmartFace [phone]: Configuration is open');
    Pebble.openURL("http://grakovne.org/pebble/SmartFace/AppConfig.php?Location=" + Location + "&Info_Updates_Frequency=" + Info_Updates_Frequency + "&Hourly_Vibe=" + Hourly_Vibe + "&BT_Vibe=" + BT_Vibe + "&Add_String=" + Add_String + "&Language=" + Language + "&Inverted=" + Inverted + "&Hide_Weather=" + Hide_Weather + "&Charge_Vibe=" + Charge_Vibe + "&Hide_Battery=" + Hide_Battery + "&Hide_BT=" + Hide_BT);
  }
);

Pebble.addEventListener("webviewclosed",
  function(e) {
    var configuration = JSON.parse(decodeURIComponent(e.response));
  
	localStorage.setItem(Location_key, configuration.Location);
	
	localStorage.setItem(Hourly_Vibe_key, configuration.Hourly_Vibe);
	
	localStorage.setItem(BT_Vibe_key, configuration.BT_Vibe);  
	 
	localStorage.setItem(Info_Updates_Frequency_key, configuration.Info_Updates_Frequency);
  
	localStorage.setItem(Add_String_key, configuration.Add_String); 
	
	localStorage.setItem(Language_key, configuration.Language);
	 
	localStorage.setItem(Inverted_key, configuration.Inverted);
	
	localStorage.setItem(Hide_Weather_key, configuration.Hide_Weather);

	localStorage.setItem(Charge_Vibe_key, configuration.Charge_Vibe);

	localStorage.setItem(Hide_Battery_key, configuration.Hide_Battery);
	
	localStorage.setItem(Hide_BT_key, configuration.Hide_BT);
	 
	ReadSettings();
	SendSettings();
	Update_Info();
	
  }
);

Pebble.addEventListener("appmessage",
  function(e) {
	//console.log('SmartFace [phone]: New message Received!');
	 
	if ('CURRENT_WEATHER' in e.payload)
		Update_Info();
  }
);

Pebble.addEventListener("ready",
  function(e) {
	ReadSettings();
	SendSettings();
	Update_Info();  
	//console.log('SmartFace [phone]: App running OK...');
  }
);