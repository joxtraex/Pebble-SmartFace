<!DOCTYPE html>
<html>
  <head>
	<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>SmartFace: Configurations</title>
  </head>
  <body>
	  <body background bgcolor = "dddddd">
	  
	  <h2>
		<p align = center>
		SmartFace: Configurations
		</p>
	   </h2>
	<hr>
	
	
	
	<p align  = center>
		Your Location: 
		<input type = "text" id = "Location" value = <?php echo $_GET["Location"]; ?> ></input>
	</p>
	
	<p align  = center>
		Refresh Data: 
		<select id="Info_Updates_Frequency" >
			<option value="1" <?php if ($_GET["Info_Updates_Frequency"] == '1') echo "selected"?> >Once per hour</option>
			<option value="2" <?php if ($_GET["Info_Updates_Frequency"] == '2') echo "selected"?> >Twice per hour</option>
			<option value="3" <?php if ($_GET["Info_Updates_Frequency"] == '3') echo "selected"?> >Three times per hour</option>
			<option value="4" <?php if ($_GET["Info_Updates_Frequency"] == '4') echo "selected"?> >Four times per hour</option>
			<option value="5" <?php if ($_GET["Info_Updates_Frequency"] == '5') echo "selected"?> >Five times per hour</option>
		</select>
    </p>
		
	<p align  = center>
		Hourly vibtation: 
		<select id="Hourly_Vibe">
			<option value="0"  <?php if ($_GET["Hourly_Vibe"] == '0') echo "selected"?> >No</option>
			<option value="1" <?php if ($_GET["Hourly_Vibe"] == '1') echo "selected"?> >Yes</option>
		</select>
    </p>
    
    <p align  = center>
		Bluetooth vibtation: 
		<select id="BT_Vibe">
			<option value="0"  <?php if ($_GET["BT_Vibe"] == '0') echo "selected"?> >No</option>
			<option value="1" <?php if ($_GET["BT_Vibe"] == '1') echo "selected"?> >Yes</option>
		</select>
    </p>
    
	<p align  = center>
		Additional info: 
		<select id="Add_String">
			<option value="Curr_Time"  <?php if ($_GET["Add_String"] == 'Curr_Time') echo "selected"?> >Server Time</option>
			<option value="Rates_Usd_Eur"  <?php if ($_GET["Add_String"] == 'Rates_Usd_Eur') echo "selected"?> >Exchange rates</option>
		</select>
    </p>
	
    <!-- <p align  = center>
		Visibility percent battery: 
		<select id="Battery_Persents_Visibility">
			<option value="0"  <?php if ($_GET["Battery_Percents_Visibility"] == '0') echo "selected"?> >No</option>
			<option value="1" <?php if ($_GET["Battery_Percents_Visibility"] == '1') echo "selected"?> >Yes</option>
		</select>
    </p>
    
    <p align  = center>
		Visibility Bluetooth state: 
		<select id="BT_State_Visibility">
			<option value="0"  <?php if ($_GET["BT_State_Visibility"] == '0') echo "selected"?> >No</option>
			<option value="1" <?php if ($_GET["BT_State_Visibility"] == '1') echo "selected"?> >Yes</option>
		</select>
    </p> -->
    
    <p align  = center>
		<button id="save_button">Save and close</button>
    </p>
    
    <hr>
     <p align  = center>
		Author: GrakovNe <br>
		E-Mail: grakovne@yandex.ru <br>
		Web: <a href = "http://grakovne.org/"> GrakovNe.org - Пробуя, создавать лучшее!</a>
    </p>
    
    <script>
 
    function saveOptions() {
		var Location = document.getElementById("Location");
		var Weather_Updates_Frequency = document.getElementById("Weather_Updates_Frequency");
		var Hourly_Vibe = document.getElementById("Hourly_Vibe");
		var BT_Vibe = document.getElementById("BT_Vibe");
		var Battery_Persents_Visibility = document.getElementById("Battery_Persents_Visibility");
		var BT_State_Visibility = document.getElementById("BT_State_Visibility");
		var Add_String = document.getElementById("Add_String");
		
		var options = {
			"Location": Location.value,
			"Info_Updates_Frequency": Info_Updates_Frequency.options[Info_Updates_Frequency.selectedIndex].value,
			"Hourly_Vibe": Hourly_Vibe.options[Hourly_Vibe.selectedIndex].value,
			"BT_Vibe": BT_Vibe.options[BT_Vibe.selectedIndex].value,
			"Add_String": Add_String.options[Add_String.selectedIndex].value
			/*"Battery_Persents_Visibility": Battery_Persents_Visibility.options[Battery_Persents_Visibility.selectedIndex].value,
			"BT_State_Visibility": BT_State_Visibility.options[BT_State_Visibility.selectedIndex].value*/
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
