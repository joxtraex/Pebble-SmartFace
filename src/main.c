#include <pebble.h>
#include "window.h"

#define MILLS_IN_HOUR               3600000
	
#define NON_INVERTED_WINDOW         0
#define INVERTED_WINDOW             1
	
#define ENGLISH_LANG                0
#define RUSSIAN_LANG                1

#define Hourly_Vibe_key             1
#define BT_Vibe_key                 2
#define Info_Updates_Frequency_key  3
#define Add_String_key              4
#define Language_key                5
#define Inverted_key                6
#define Charge_Vibe_key             7
#define Hide_Battery_key            8
#define Hide_BT_key                 9
#define Weather_Text_key            10
#define AddInfo_Text_key            11
#define Hide_Weather_key            12
	
Window *MainWindow;
TextLayer *Time_Text;
TextLayer *Week_Text;
TextLayer *Date_Text;
TextLayer *CWeather_Text;
TextLayer *Battery_Text;
TextLayer *Connection_Text;
TextLayer *AddInfo_Text;

BitmapLayer *BT_Image;
BitmapLayer *BAT_Image;

GBitmap *BT;
GBitmap *BAT;

GFont Bar_Font;
GFont Time_Font;
GFont Date_Font;
GFont CWeather_Font;

static bool JustRun_Flag = 1;
static bool IsConnected_Flag = 1;

time_t now;
struct tm *current_time;

static char Time[] = "00:00";
static char Date[] = "26.06.1996";

static char Buffer_Weather    [24];
static char Buffer_Add_String [24];

static inline void Process_Received_Data(DictionaryIterator *iter, void *context);
static inline void send_int(uint8_t key, uint8_t cmd);
static inline void UpdateWeather();
static inline void UpdateConnection(bool Connected);
static inline void UpdateBattery(BatteryChargeState State);
static inline void UpdateTime(struct tm* CurrentTime, TimeUnits units_changed);
static inline void UpdateDate(struct tm* CurrentTime, TimeUnits units_changed);
static inline void Init_Display(void);

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

static const uint32_t BT_Icons[] = {
	RESOURCE_ID_NO_BT_ICON,
	RESOURCE_ID_BT_ICON
};

static const char DayNames[2][7][24] = {
	{
		"SUNDAY",
		"MONDAY",
		"TUESDAY",
		"WEDNESDAY",
		"THURSDAY",
		"FRIDAY",
		"SATURDAY"
	},
	{
		"BOCKPECEHЬE",
		"ПOHEДEЛЬHИK",
		"BTOPHИK",
		"CPEДA",
		"ЧETBЕPГ",
		"ПЯTHИЦA",
		"CYББOTA"
	}
};

static const char OfflineNames[2][16] = {
	"OFFLINE",
	"HET CETИ"
};

static const char BTNames[2][2][7] = {
	{
		"LOST",
		"OK"
	},
	{
		"HET",
		"OK"
	}
};

enum {
	CURRENT_WEATHER          = 0,
	HOURLY_VIBE              = 1,
	BT_VIBE                  = 2,
	INFO_UPDATES_FREQUENCY   = 3,
	ADD_INFO                 = 4,
	LANGUAGE                 = 5,
	INVERTED                 = 6,
	CHARGE_VIBE              = 7,
	HIDE_BATTERY             = 8,
	HIDE_BT                  = 9,
	HIDE_WEATHER             = 10,
};

static struct {
	char Info_Updates_Frequency;
	char Weather_Text [24];
	char AddInfo_Text [32];
	bool Hourly_Vibe;
	bool BT_Vibe;
	bool Charge_Vibe;
	bool Language;
	bool Inverted;
	bool Hide_Battery;
	bool Hide_BT;
	bool Hide_Weather;
} Settings;

static void Process_Received_Data(DictionaryIterator *iter, void *context){
	
	Tuple *t = dict_read_first(iter);
	 while(t != NULL){
		char key = t->key;
        char value = t->value->int32;
		static char string_value[32];
		strcpy(string_value, t->value->cstring);	 
		 switch (key){
			 case CURRENT_WEATHER:
			 	
			 		if (strcmp(string_value, Settings.Weather_Text)){
			 			strcpy(Settings.Weather_Text, string_value);
			 			persist_write_string(Weather_Text_key, string_value);
 						text_layer_set_text(CWeather_Text, Settings.Weather_Text); 
				
			 			//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Weather was updated!");
					}
			 
			 	break; 
			 
			 case ADD_INFO:
			 
			 	if (strcmp(string_value, Settings.AddInfo_Text)){
			 			strcpy(Settings.AddInfo_Text, string_value);
			 			persist_write_string(AddInfo_Text_key, string_value);
 						text_layer_set_text(AddInfo_Text, Settings.AddInfo_Text); 
					
			 			//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Additional info was updated!");
				}
			 
			 	break;
			
			 case HOURLY_VIBE:
			 	
			 	Settings.Hourly_Vibe = value;
			 	persist_write_bool(Hourly_Vibe_key, Settings.Hourly_Vibe);
			 
			 	//APP_LOG(APP_LOG_LEVEL_INFO, "Hourly vibe applied");
			 	
			 	break;
			 
			 case BT_VIBE:
			 	
			 	Settings.BT_Vibe = value;
				persist_write_bool(BT_Vibe_key, Settings.BT_Vibe);
			 
			 	//APP_LOG(APP_LOG_LEVEL_INFO, "BT vibe applied");
			 
				 break;
			 
			 case LANGUAGE:
			 	
			 	if (value < 2){
				 	Settings.Language = value;
					persist_write_bool(Language_key, Settings.Language);
				}
			 	Init_Display();
					
			 	//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Language applied");
			 
			 	break;
				
			 case INVERTED:
			 	
			 	if (value < 2){
					Settings.Inverted = value;
					persist_write_bool(Inverted_key, Settings.Inverted);
				}
			 	SetColors(Settings.Inverted);
					
			 	//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Window color mode applied");
			 
			 	break;
			 
			 case CHARGE_VIBE:
			 	
			 	if (value < 2){
					Settings.Charge_Vibe = value;
					persist_write_bool(Charge_Vibe_key, Settings.Charge_Vibe);
				}
			 
			 	//APP_LOG(APP_LOG_LEVEL_INFO, "Charge vibe applied");
			 
			 	break;
			 
			 case HIDE_BATTERY:
			 	if (value < 2){
					Settings.Hide_Battery = value;
					persist_write_bool(Hide_Battery_key, Settings.Hide_Battery);
					SetBarText(Settings.Hide_Battery, Settings.Hide_BT, Settings.Hide_Weather);
					
					//APP_LOG(APP_LOG_LEVEL_INFO, "Hide battery state applied");
				}
			 	
			 	break;
			 
			 case HIDE_BT:
			 	if (value < 2){
					Settings.Hide_BT = value;
					persist_write_bool(Hide_BT_key, Settings.Hide_BT);
					SetBarText(Settings.Hide_Battery, Settings.Hide_BT, Settings.Hide_Weather);
					
					//APP_LOG(APP_LOG_LEVEL_INFO, "Hide BT state applied");
				}
			 	
			 	break;
			 
			 case HIDE_WEATHER:
			 
				if (value < 2){
					Settings.Hide_Weather = value;
					persist_write_bool(Hide_Weather_key, Settings.Hide_Weather);
					SetBarText(Settings.Hide_Battery, Settings.Hide_BT, Settings.Hide_Weather);
					
					//APP_LOG(APP_LOG_LEVEL_INFO, "Hide weather state applied");
				}
			 
			 case INFO_UPDATES_FREQUENCY:
			 	
			 	Settings.Info_Updates_Frequency = value;
			 	persist_write_int(Info_Updates_Frequency_key, Settings.Info_Updates_Frequency);
			 	if (Settings.Info_Updates_Frequency < 10)
					app_timer_register(MILLS_IN_HOUR / Settings.Info_Updates_Frequency, UpdateWeather, 0);
				else app_timer_register(MILLS_IN_HOUR, UpdateWeather, 0);
		 		
				//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Info Refresh applied");
			 
		 }	 
        t = dict_read_next(iter);
    }	
}

static inline void ReadSettings(){

	if (persist_exists(Weather_Text_key))
		persist_read_string(Weather_Text_key, Settings.Weather_Text, sizeof(Settings.Weather_Text));
	else
		strcpy(Settings.Weather_Text, "Set Location!");
		
	
	if (persist_exists(AddInfo_Text_key))
		persist_read_string(AddInfo_Text_key, Settings.AddInfo_Text, sizeof(Settings.AddInfo_Text));
	else
		strcpy(Settings.AddInfo_Text, "Add something!");
	
	if (persist_exists(Hourly_Vibe_key)) 
		Settings.Hourly_Vibe = persist_read_int(Hourly_Vibe_key);
	else
		Settings.Hourly_Vibe = 1;

	if (persist_exists(BT_Vibe_key)) 
		Settings.BT_Vibe = persist_read_int(BT_Vibe_key);
	else
		Settings.BT_Vibe = 1;
	
	if (persist_exists(Info_Updates_Frequency_key)) 
		Settings.Info_Updates_Frequency = persist_read_int(Info_Updates_Frequency_key);
	else
		Settings.Info_Updates_Frequency = 1;
	
	if (persist_exists(Language_key)) 
		Settings.Language = persist_read_int(Language_key);
	else
		Settings.Language = ENGLISH_LANG;
	
	if (persist_exists(Inverted_key)) 
		Settings.Inverted = persist_read_int(Inverted_key);
	else
		Settings.Inverted = NON_INVERTED_WINDOW;
	
	if (persist_exists(Charge_Vibe_key)) 
		Settings.Charge_Vibe = persist_read_int(Charge_Vibe_key);
	else
		Settings.Charge_Vibe = 1;
	
	if (persist_exists(Hide_Battery_key)) 
		Settings.Hide_Battery = persist_read_int(Hide_Battery_key);
	else
		Settings.Hide_Battery = 0;
	
	if (persist_exists(Hide_BT_key)) 
		Settings.Hide_BT = persist_read_int(Hide_BT_key);
	else
		Settings.Hide_BT = 0;
	
	if (persist_exists(Hide_Weather_key)) 
		Settings.Hide_Weather = persist_read_int(Hide_Weather_key);
	else
		Settings.Hide_Weather = 0;
}

static inline void send_int(uint8_t key, uint8_t cmd){
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
 
    app_message_outbox_send();
	
	//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Data was sended!");
}

static inline void UpdateWeather(){
	//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Info is updated");
	
	if (!IsConnected_Flag){
		text_layer_set_text(CWeather_Text, OfflineNames[Settings.Language]);
	}
	
	else if (!JustRun_Flag) {
		psleep(1000);
		send_int(CURRENT_WEATHER, 1);
	}
	
	if (Settings.Info_Updates_Frequency)
		app_timer_register(MILLS_IN_HOUR / Settings.Info_Updates_Frequency, UpdateWeather, 0);
	else 
		app_timer_register(MILLS_IN_HOUR, UpdateWeather, 0);
}

static inline void UpdateConnection(bool Connected){
	IsConnected_Flag = Connected;
	
	if ( (!JustRun_Flag)&&(Settings.BT_Vibe) )
		vibes_long_pulse();
	
	if (Connected)
		UpdateWeather();
	
	gbitmap_destroy(BT); 
	BT = gbitmap_create_with_resource(BT_Icons[Connected]); 
	bitmap_layer_set_bitmap(BT_Image, BT);
	
	text_layer_set_text(Connection_Text, BTNames[Settings.Language][Connected]);
	
	//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: BT-Connection was updated!");
}

static inline void UpdateBattery(BatteryChargeState State){
	static char Percents[] = "100%";
	static bool Vibe_Flag;
	
	if ( (!Vibe_Flag) && (State.is_plugged) && (Settings.Charge_Vibe) && (!JustRun_Flag)){
		vibes_short_pulse();
		Vibe_Flag = 1;
	}
	
	if ( (Vibe_Flag) && (!State.is_plugged) && (Settings.Charge_Vibe) && (!JustRun_Flag) ) {
		vibes_short_pulse();
		Vibe_Flag = 0;
	}
	
	snprintf(Percents, sizeof(Percents), "%d%%", State.charge_percent);
	text_layer_set_text(Battery_Text, Percents);
	gbitmap_destroy(BAT); 
	BAT = gbitmap_create_with_resource(Battery_Icons[State.charge_percent / 10]); 
	bitmap_layer_set_bitmap(BAT_Image, BAT); 
}

static inline void UpdateTime(struct tm* CurrentTime, TimeUnits units_changed){
	strftime(Time, sizeof(Time), "%H:%M", CurrentTime);
	text_layer_set_text(Time_Text, Time);
	
	if ( (!(CurrentTime -> tm_hour)) && (!(CurrentTime -> tm_min)) ){
		UpdateDate(CurrentTime, SECOND_UNIT);
	}
	
	if ( (!(CurrentTime -> tm_min)) && (!JustRun_Flag) && (Settings.Hourly_Vibe) )
		vibes_double_pulse();
}

static inline void UpdateDate(struct tm* CurrentTime, TimeUnits units_changed){
	strftime(Date, sizeof(Date), "%d.%m.%Y", CurrentTime);
	text_layer_set_text(Date_Text, Date);
	text_layer_set_text(Week_Text, DayNames[Settings.Language][CurrentTime->tm_wday]);
}

static inline void Init_Display(){
	
	/*Set current time*/
	now = time(NULL);
    current_time = localtime(&now);
    UpdateTime(current_time, SECOND_UNIT);
	UpdateDate(current_time, SECOND_UNIT);
	
	/*Set bottom strings*/
	snprintf(Buffer_Add_String, sizeof(Buffer_Add_String), "%s", Settings.AddInfo_Text);
 	text_layer_set_text(AddInfo_Text, Buffer_Add_String); 
	
	snprintf(Buffer_Weather, sizeof(Buffer_Weather), "%s", Settings.Weather_Text);
 	text_layer_set_text(CWeather_Text, Buffer_Weather); 
}

int main(void) {
	/*Interface building*/
	JustRun_Flag = 1;
	BuildWindow();
	
	/*communication with phone*/
	app_message_register_inbox_received(Process_Received_Data);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	//APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: App was opened!");
	
	/*Application Settings*/
	ReadSettings();
	SetColors(Settings.Inverted);
	SetBarText(Settings.Hide_Battery, Settings.Hide_BT, Settings.Hide_Weather);
	Init_Display();
	
	/*Checking Battery state & BT - connection*/
	UpdateBattery(battery_state_service_peek());
	UpdateConnection(bluetooth_connection_service_peek());
	if (!IsConnected_Flag)
		text_layer_set_text(CWeather_Text, OfflineNames[Settings.Language]);
	
	/*subcribing to services*/
	tick_timer_service_subscribe(MINUTE_UNIT, &UpdateTime);
	bluetooth_connection_service_subscribe(UpdateConnection);
	battery_state_service_subscribe(UpdateBattery);
	
	window_stack_push(MainWindow, true);
	JustRun_Flag = 0;
	
  	app_event_loop();
	
	/*unsubcribing from services*/
	//APP_LOG(APP_LOG_LEVEL_INFO, "SmaftFace: App is closed...");
  	tick_timer_service_unsubscribe(); 
	battery_state_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
	
	/*Closing phone connection*/
	app_message_deregister_callbacks();
	
	/*Inerface destroying*/
 	DestroyWindow();
}