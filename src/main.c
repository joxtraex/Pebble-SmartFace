#include <pebble.h>
#include "window.h"

#define MILLS_IN_HOUR 3600000
	
Window *MainWindow;
TextLayer *Time_Text;
TextLayer *Week_Text;
TextLayer *Date_Text;
TextLayer *CWeather_Text;
TextLayer *Battery_Text;
TextLayer *Connection_Text;
TextLayer *WeatherTime_Text;


BitmapLayer *BT_Image;
BitmapLayer *BAT_Image;

GBitmap *BT;
GBitmap *BAT;

GFont Bar_Font;
GFont Time_Font;
GFont Date_Font;
GFont CWeather_Font;

int JustRun_Flag = 1;

time_t now;
struct tm *current_time;

static char Time[] = "00:00";
static char Date[] = "26.06.1996";
static char Week[] = "wednesday";
static char UpdTime[] = "00:00:00";

char Buffer [32];
char Buffer_0 [32];
char Buffer_1 [32];

static const uint32_t Battery_Icons[] = {
	RESOURCE_ID_BAT_ICON_0, 
	RESOURCE_ID_BAT_ICON_10, 
	RESOURCE_ID_BAT_ICON_20, 
	RESOURCE_ID_BAT_ICON_30, 
	RESOURCE_ID_BAT_ICON_40, 
	RESOURCE_ID_BAT_ICON_50, 
	RESOURCE_ID_BAT_ICON_60, 
	RESOURCE_ID_BAT_ICON_70, 
	RESOURCE_ID_BAT_ICON_80, 
	RESOURCE_ID_BAT_ICON_90, 
	RESOURCE_ID_BAT_ICON_100
	};

enum {
	CURRENT_WEATHER = 0,
	LOCATION = 1,
	HOURLY_VIBE = 2,
	BT_VIBE = 3,
	WEATHER_UPDATES_FREQUENCY = 4,
	BATTERY_PERSENTS_VISIBILITY = 5,
	BT_STATE_VISIBILITY = 6
};

struct {
	bool Verbose;
	int Weather_Updates_Frequency;
	char Location [32];
	bool Hourly_Vibe;
	bool BT_Vibe;
	bool Battery_Percents_Visibility;
	bool BT_State_Visibility;
} Settings;

static void Process_Received_Data(DictionaryIterator *iter, void *context){
	Tuple *t = dict_read_first(iter);
	 while(t != NULL){
		int key = t->key;
        int value = t->value->int32;
		char string_value[32];
		strcpy(string_value, t->value->cstring);	 
		 switch (key){
			 case CURRENT_WEATHER:
			 	snprintf(Buffer, sizeof(Buffer), "%s", string_value);
 				text_layer_set_text(CWeather_Text, Buffer); 
			 	if (Settings.Verbose)
					APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Weather was updated!");
			 
			 	break; 
			 case LOCATION:
			 	strcpy(Settings.Location, string_value);
			 	if (Settings.Verbose){
					APP_LOG(APP_LOG_LEVEL_INFO, "Location changed");
				}
			 
			 	break;
			
			 case HOURLY_VIBE:
			 	Settings.Hourly_Vibe = value;
			 	if (Settings.Verbose){
			 		if (value)
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Hourly vibration is enabled");
			 		else
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Hourly vibration is disabled");
				}
			 	
			 	break;
			 
			 case BT_VIBE:
			 	Settings.BT_Vibe = value;
			 
			 	if (Settings.Verbose){
			 		if (value)
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Bluetooth vibration is enabled");
			 		else
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Bluetooth vibration is disabled");
				}
				 break;
			 
			 case WEATHER_UPDATES_FREQUENCY:
			 	Settings.Weather_Updates_Frequency = value;
		 		if (Settings.Verbose){
					APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Weather Refresh applied");
				}
			 
			 	break;
			 
			 case BATTERY_PERSENTS_VISIBILITY:
			 	Settings.Battery_Percents_Visibility = 1; // Plug. Will Used if need
			 
			 	if (Settings.Verbose){
			 		if (value)
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Battery percents is shown");
			 		else
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Battery percents is not shown");
				}
			 	
				 break;
			 
			 case BT_STATE_VISIBILITY:
			 	Settings.BT_State_Visibility = 1; // Plug. Will Used if need
		
			 	if (Settings.Verbose){
			 		if (value)
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Battery percents is shown");
			 		else
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Battery percents is not shown");
				}	
		 }
		 
        t = dict_read_next(iter);
    }

	
}

void handle_init(void) {
	BuildWindow();
	app_message_register_inbox_received(Process_Received_Data);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: App was opened!");
}

void handle_deinit(void) {
	tick_timer_service_unsubscribe(); 
	battery_state_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
	
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmaftFace: App is closed...");
	
	app_message_deregister_callbacks();
 	DestroyWindow();
}

void send_int(uint8_t key, uint8_t cmd)
{
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
 
    app_message_outbox_send();
	
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Data was sended!");
}


void UpdateWeather(){
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Weather is updated");
	
	text_layer_set_text(CWeather_Text, "Updating...");
	send_int(CURRENT_WEATHER, 1);
	now = time(NULL);
    current_time = localtime(&now);
	strftime(Buffer_0, sizeof(Buffer_0), "%H:%M:%S", current_time);
	text_layer_set_text(WeatherTime_Text, Buffer_0);
	app_timer_register(MILLS_IN_HOUR / Settings.Weather_Updates_Frequency, UpdateWeather, 0);
}

static void UpdateConnection(bool Connected){
	if ( (!JustRun_Flag)&&(Settings.BT_Vibe) )
		vibes_long_pulse();
	
	if (Connected)
		text_layer_set_text(Connection_Text, "OK");
	else
		text_layer_set_text(Connection_Text, "LOST");
	
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: BT-Connection was updated!");
}

static void UpdateBattery(BatteryChargeState State){
	static char Percents[] = "100%";
	snprintf(Percents, sizeof(Percents), "%d%%", State.charge_percent);
	text_layer_set_text(Battery_Text, Percents);
	gbitmap_destroy(BAT); 
	BAT = gbitmap_create_with_resource(Battery_Icons[State.charge_percent / 10]); 
	bitmap_layer_set_bitmap(BAT_Image, BAT); 
}

static void BatteryTimer(){
	UpdateBattery(battery_state_service_peek());
	app_timer_register(MILLS_IN_HOUR / 3, BatteryTimer, 0);
	
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Battery state was updated!");
}

static void UpdateTime(struct tm* CurrentTime, TimeUnits units_changed){
	strftime(Time, sizeof(Time), "%H:%M", CurrentTime);
	text_layer_set_text(Time_Text, Time);
	
	strftime(Date, sizeof(Date), "%d.%m.%Y", CurrentTime);
	text_layer_set_text(Date_Text, Date);
		
	strftime(Week, sizeof(Week), "%A", CurrentTime);
	text_layer_set_text(Week_Text, Week);
	
	if ( (CurrentTime -> tm_min == 0) && (!JustRun_Flag) && (Settings.Hourly_Vibe) )
		vibes_double_pulse();
}

int main(void) {
	handle_init();
	
	 /*Just Debug message in console*/
	Settings.Verbose = 1; 
	
	now = time(NULL);
    current_time = localtime(&now);
    UpdateTime(current_time, SECOND_UNIT);
	
	UpdateConnection(bluetooth_connection_service_peek());
	BatteryTimer();
	
	tick_timer_service_subscribe(MINUTE_UNIT, &UpdateTime);
	battery_state_service_subscribe(UpdateBattery);
	bluetooth_connection_service_subscribe(UpdateConnection);
	
	
	JustRun_Flag = 0;
	send_int(CURRENT_WEATHER, 1);
	UpdateWeather();
	
	window_stack_push(MainWindow, true);
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: App running!");
	
  	app_event_loop();
  	handle_deinit();
}