var Verbose = 1;

function HTTPGET(url) {
    var req = new XMLHttpRequest();
    req.open("GET", url, false);
    req.send(null);
    return req.responseText;
}

function Update_Weather(){
	var CurrentWeather = '';
	var response = HTTPGET("http://api.openweathermap.org/data/2.5/weather?q=omsk,ru");

 	var json = JSON.parse(response);
	var temperature = (json.main.temp - 273.15);
	
	/*Just adding "+" sign for good temperature*/
	var sign = "";
	if (temperature > 0)
		sign = "+";
	
	temperature = temperature.toFixed(1);	
  
	var state = json.weather[0].main;

	/*Sending All data to Pebble*/
	CurrentWeather = state + ', ' + sign + temperature + "C";
	
	var dict = {"CURRENT_WEATHER" : CurrentWeather};

	Pebble.sendAppMessage(dict);
	
	if (Verbose)
		console.log('SmartFace [phone]: Weather updated: ' + CurrentWeather);
}

Pebble.addEventListener("appmessage",
  function(e) {
    	Update_Weather();
	  
		if (Verbose)
			console.log('SmartFace [phone]: Message received!');
  }
);

Pebble.addEventListener("ready",
  function(e) {
    	Update_Weather();
		
	  if (Verbose)
			console.log('SmartFace [phone]: App running OK...');
  }
);
