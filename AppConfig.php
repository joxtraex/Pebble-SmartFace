<!DOCTYPE html>
<html>
  <head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
	<title>SmartFace: Configurations</title>
	
		<style type="text/css">
			
			@font-face {
				font-family: Imagine; 
				src: url(fonts/imagine_font.ttf);
			}
			
			body {
				background-color: #000000;
			}
			
			.header { 
				width: 95%; 
				background: #ffffff;
				text-align: center;
				margin: 0 auto;
				padding: 10px 0px 0px 0px;
				font-size: 20px;
				font-family: Imagine;
			}
   
			.author {
				width: 95%; 
				background: #ffffff; 
				text-align: center;
				margin: 0 auto;
				padding: 0px 0px 10px 0px;
				font-size: 14px;
				font-family: Imagine;
			}
			
			.footer {
				width: 95%;
				background: #ffffff; 
				text-align: center;
				margin: 0 auto;
				font-size: 14px;
				padding: 10px 0px 10px 0px;
				font-family: Imagine;
			}
			
			.block {
				text-align: center;
				width: 95%;
				background: #ffffff; 
				margin: 0 auto;
				font-size: 14px;
				padding: 20px 0px 0px 0px;
				font-family: Imagine;
			}
			
			.block-text {
				display:inline-block;
				text-align: left;
				margin: 0 auto;
				padding: 0px 0px 0px 0px;
				font-size: 12px;
				width: 135px;
				
			}
			
			button {
				display:inline-block;
				margin: 0 auto;
				text-align: center;
				font-family: Imagine;
				width: 95%;
				background: #40c781;
				border: 1px;
				border-color: #25744C;
				padding: 1px;
				border-style: solid;
				font-size: 24px;
			}
			
		</style>	
	</head>
	
	
	<body>

	<br>

	<div class="header">
		SmartFace v 2.0: Settings
	</div>
		
	<div class= "author">
		by GrakovNe
	</div>
		
		<br/> <br/>
		
		
	<div class = "block">
			<div class = "block-text">
				LANGUAGE: 
			</div>
			<div class = "block-text">
				<select id="Language"  style="width: 135px">
					<option value="0" <?php if ($_GET["Language"] == '0') echo "selected"?> >English</option>
					<option value="1" <?php if ($_GET["Language"] == '1') echo "selected"?> >Русский</option>
				</select>
			</div>
	</div>
	
	<div class = "block">
			<div class = "block-text">
				SCREEN COLORS: 
			</div>
			<div class = "block-text">
				<select id="Inverted"  style="width: 135px">
					<option value="0" <?php if ($_GET["Inverted"] == '0') echo "selected"?> >Black on white</option>
					<option value="1" <?php if ($_GET["Inverted"] == '1') echo "selected"?> >White on black</option>
				</select>
			</div>
			
			<br/> <br/>
	</div>

	<br/> 

	<div class = "block">
			<div class = "block-text">
				Your location: 
			</div>
			<div class = "block-text">
				<input type = "text" id = "Location" style="width: 135px" value = <?php echo $_GET["Location"]; ?> ></input>
			</div>
			
		
	</div>
	
	<div class = "block">
			<div class = "block-text">
				Refresh Data:
			</div>
			<div class = "block-text">
				<select id="Info_Updates_Frequency"  style="width: 135px">
					<option value="1" <?php if ($_GET["Info_Updates_Frequency"] == '1') echo "selected"?> >Once per hour</option>
					<option value="2" <?php if ($_GET["Info_Updates_Frequency"] == '2') echo "selected"?> >Twice per hour</option>
					<option value="3" <?php if ($_GET["Info_Updates_Frequency"] == '3') echo "selected"?> >Three times per hour</option>
					<option value="4" <?php if ($_GET["Info_Updates_Frequency"] == '4') echo "selected"?> >Four times per hour</option>
					<option value="5" <?php if ($_GET["Info_Updates_Frequency"] == '5') echo "selected"?> >Five times per hour</option>
				</select>
			</div>
	</div>
	
	<div class = "block">
			<div class = "block-text">
				Additional info:
			</div>
			<div class = "block-text">
				<select id="Add_String"  style="width: 135px">
					<option value="Empty"  <?php if ($_GET["Add_String"] == 'Empty') echo "selected"?> >Nothing</option>
					<option value="Curr_Time"  <?php if ($_GET["Add_String"] == 'Curr_Time') echo "selected"?> >Server Time</option>
					<option value="Location"  <?php if ($_GET["Add_String"] == 'Location') echo "selected"?> >Your Location</option>
					<option value="Rates_Usd_Eur"  <?php if ($_GET["Add_String"] == 'Rates_Usd_Eur') echo "selected"?> >Exchange rates (RUB)</option>
				</select>
			</div>
			
			<br/> <br/>
	</div>
	
	<br/>
	
	<div class = "block">
			<div class = "block-text">
				Hourly vibtation:
			</div>
			<div class = "block-text">
				<select id="Hourly_Vibe"  style="width: 135px">
					<option value="0"  <?php if ($_GET["Hourly_Vibe"] == '0') echo "selected"?> >No</option>
					<option value="1" <?php if ($_GET["Hourly_Vibe"] == '1') echo "selected"?> >Yes</option>
				</select>
			</div>
	</div>
		
	<div class = "block">
			<div class = "block-text">
				Bluetooth vibtation:
			</div>
			<div class = "block-text">
				<select id="BT_Vibe"  style="width: 135px">
					<option value="0"  <?php if ($_GET["BT_Vibe"] == '0') echo "selected"?> >No</option>
					<option value="1" <?php if ($_GET["BT_Vibe"] == '1') echo "selected"?> >Yes</option>
				</select>
			</div>
	</div>
	
	<div class = "block">
			<div class = "block-text">
				Charging vibration:
			</div>
			<div class = "block-text">
				<select id="Charge_Vibe"  style="width: 135px">
					<option value="0"  <?php if ($_GET["Charge_Vibe"] == '0') echo "selected"?> >No</option>
					<option value="1" <?php if ($_GET["Charge_Vibe"] == '1') echo "selected"?> >Yes</option>
				</select>
			</div>
		
		<br/> <br/>
		
	</div>
	
	<br/>
	
	<div class = "block">
			<div class = "block-text">
				Hide weather:
			</div>
			<div class = "block-text">
				<select id="Hide_Weather"  style="width: 135px">
					<option value="0"  <?php if ($_GET["Hide_Weather"] == '0') echo "selected"?> >No</option>
					<option value="1" <?php if ($_GET["Hide_Weather"] == '1') echo "selected"?> >Yes</option>
				</select>
			</div>
	</div>
	
	<div class = "block">
			<div class = "block-text">
				Hide battery text:
			</div>
			<div class = "block-text">
				<select id="Hide_Battery"  style="width: 135px">
					<option value="0"  <?php if ($_GET["Hide_Battery"] == '0') echo "selected"?> >No</option>
					<option value="1" <?php if ($_GET["Hide_Battery"] == '1') echo "selected"?> >Yes</option>
				</select>
			</div>
	</div>
	
	<div class = "block">
			<div class = "block-text">
				Hide BT text:
			</div>
			<div class = "block-text">
				<select id="Hide_BT"  style="width: 135px">
					<option value="0"  <?php if ($_GET["Hide_BT"] == '0') echo "selected"?> >No</option>
					<option value="1" <?php if ($_GET["Hide_BT"] == '1') echo "selected"?> >Yes</option>
				</select>
			</div>
			
			<br/> <br/>
	</div>
	
	<br/> <br/>
		
		<div align = center>
			<button id="save_button">Save and close</button>
		</div>
		
	<br/> <br/>
	
	<div class = "footer">
		<b>Author</b>: <a href = "http://grakovne.org/"> GrakovNe</a><br>
		<b>E-Mail</b>: grakovne@yandex.ru <br>
		<a href = "http://grakovne.org/donate_to_grakovne/"><b>Donate</b></a>
	</div>
	
	<script>

	function saveOptions() {
		var Location = document.getElementById("Location");
		var Weather_Updates_Frequency = document.getElementById("Weather_Updates_Frequency");
		var Hourly_Vibe = document.getElementById("Hourly_Vibe");
		var BT_Vibe = document.getElementById("BT_Vibe");
		var Battery_Persents_Visibility = document.getElementById("Battery_Persents_Visibility");
		var BT_State_Visibility = document.getElementById("BT_State_Visibility");
		var Add_String = document.getElementById("Add_String");
		var Inverted = document.getElementById("Inverted");
		var Hide_Weather = document.getElementById("Hide_Weather");
		var Hide_Battery = document.getElementById("Hide_Battery");
		var Hide_BT = document.getElementById("Hide_BT");
		var Charge_Vibe = document.getElementById("Charge_Vibe");

		var options = {
			"Location": Location.value,
			"Info_Updates_Frequency": Info_Updates_Frequency.options[Info_Updates_Frequency.selectedIndex].value,
			"Hourly_Vibe": Hourly_Vibe.options[Hourly_Vibe.selectedIndex].value,
			"BT_Vibe": BT_Vibe.options[BT_Vibe.selectedIndex].value,
			"Add_String": Add_String.options[Add_String.selectedIndex].value,
			"Language": Language.options[Language.selectedIndex].value,
			"Inverted": Inverted.options[Inverted.selectedIndex].value,
			"Hide_Weather": Hide_Weather.options[Hide_Weather.selectedIndex].value,
			"Charge_Vibe": Charge_Vibe.options[Charge_Vibe.selectedIndex].value,
			"Hide_Battery": Hide_Battery.options[Hide_Battery.selectedIndex].value,
			"Hide_BT": Hide_BT.options[Hide_BT.selectedIndex].value,
		}

	return options;
	};

	var submitButton = document.getElementById("save_button");

	submitButton.addEventListener("click",
	function() {
	  console.log("Submit");

	  var options = saveOptions();
	  var location = "pebblejs://close#" + encodeURIComponent(JSON.stringify(options));

	  document.location = location;
	},
  false);

	</script>
	</body>
</html>
