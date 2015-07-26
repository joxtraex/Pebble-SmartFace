#include <pebble.h>
#include "window.h"

#define MILLS_IN_HOUR               3600000
	
#define NON_INVERTED_WINDOW         0
#define INVERTED_WINDOW             1
	
#define ENGLISH_LANG                0
#define RUSSIAN_LANG                1

#define Verbose_key                 1
#define Location_key                2
#define Hourly_Vibe_key             3
#define BT_Vibe_key                 4
#define Info_Updates_Frequency_key  5
#define Add_String_key              6
#define Language_key                7
#define Inverted_key                8
#define Charge_Vibe_key             9
#define Hide_Battery_key            10
#define Hide_BT_key                 11
	
Window *MainWindow;
TextLayer *Time_Text;
TextLayer *Week_Text;
TextLayer *Date_Text;
TextLayer *CWeather_Text;
TextLayer *Battery_Text;
TextLayer *Connection_Text;
TextLayer *AddString_Text;

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

char Buffer_Weather [32];
char Buffer_Add_String [32];
char Buffer_Temp[32];

static void Process_Received_Data(DictionaryIterator *iter, void *context);
void handle_init(void);
void handle_deinit(void);
void send_int(uint8_t key, uint8_t cmd);
void UpdateWeather();
static void UpdateConnection(bool Connected);
static void UpdateBattery(BatteryChargeState State);
static void BatteryTimer();
static void UpdateTime(struct tm* CurrentTime, TimeUnits units_changed);
void UpdateTime_Force(void);

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

static const char DayNames[2][7][32] = {
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
		"ВОСКРЕСЕНЬЕ",
		"ПОНЕДЕЛЬНИК",
		"ВТОРНИК",
		"СРЕДА",
		"ЧЕТВЕРГ",
		"ПЯТНИЦА",
		"СУББОТА"
	}
};

static const char OfflineNames[2][32] = {
	"OFFLINE",
	"НЕТ СЕТИ"
};

static const char BTNames[2][2][32] = {
	{
		"LOST",
		"OK"
	},
	{
		"НЕТ",
		"ОК"
	}
};

enum {
	CURRENT_WEATHER          = 0,
	LOCATION                 = 1,
	HOURLY_VIBE              = 2,
	BT_VIBE                  = 3,
	INFO_UPDATES_FREQUENCY   = 4,
	ADD_INFO                 = 5,
	LANGUAGE                 = 6,
	INVERTED                 = 7,
	CHARGE_VIBE              = 8,
	HIDE_BATTERY             = 9,
	HIDE_BT                  = 10,
	
};

struct {
	int Info_Updates_Frequency;
	char Location [32];
	bool Hourly_Vibe;
	bool BT_Vibe;
	bool Charge_Vibe;
	bool Verbose;
	bool Language;
	bool Inverted;
	bool Hide_Battery;
	bool Hide_BT;
} Settings;

static void Process_Received_Data(DictionaryIterator *iter, void *context){
	JustRun_Flag = 1;
	Tuple *t = dict_read_first(iter);
	 while(t != NULL){
		int key = t->key;
        int value = t->value->int32;
		char string_value[32];
		strcpy(string_value, t->value->cstring);	 
		 switch (key){
			 case CURRENT_WEATHER:
			 
			 	snprintf(Buffer_Weather, sizeof(Buffer_Weather), "%s", string_value);
 				text_layer_set_text(CWeather_Text, Buffer_Weather); 
			 	if (Settings.Verbose)
					APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Weather was updated!");
			 
			 	break; 
			 
			 case ADD_INFO:
			 
			 	snprintf(Buffer_Add_String, sizeof(Buffer_Add_String), "%s", string_value);
 				text_layer_set_text(AddString_Text, Buffer_Add_String); 
			 	if (Settings.Verbose)
					APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Additional info was updated!");
			 
			 	break;
			 
			 case LOCATION:
			 	
			 	strcpy(Settings.Location, string_value);
			 	persist_write_string(Location_key, Settings.Location);
			 	if (Settings.Verbose){
					APP_LOG(APP_LOG_LEVEL_INFO, "Location changed");
				}
			 
			 	break;
			
			 case HOURLY_VIBE:
			 	
			 	Settings.Hourly_Vibe = value;
			 	persist_write_bool(Hourly_Vibe_key, Settings.Hourly_Vibe);
			 	if (Settings.Verbose){
			 		if (value)
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Hourly vibration is enabled");
			 		else
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Hourly vibration is disabled");
				}
			 	
			 	break;
			 
			 case BT_VIBE:
			 	
			 	Settings.BT_Vibe = value;
				persist_write_bool(BT_Vibe_key, Settings.BT_Vibe);
			 	if (Settings.Verbose){
			 		if (value)
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Bluetooth vibration is enabled");
			 		else
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Bluetooth vibration is disabled");
				}
			 
				 break;
			 
			 case LANGUAGE:
			 	
			 	if (value < 2){
				 	Settings.Language = value;
					persist_write_bool(Language_key, Settings.Language);
				}
			 	UpdateTime_Force();
			 	if (Settings.Verbose)
					APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Language applied");
			 
			 	break;
				
			 case INVERTED:
			 	
			 	if (value < 2){
					Settings.Inverted = value;
					persist_write_bool(Inverted_key, Settings.Inverted);
				}
			 	SetColors(Settings.Inverted);
			 	if (Settings.Verbose)
					APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Window color mode applied");
			 
			 	break;
			 
			 case CHARGE_VIBE:
			 	
			 	if (value < 2){
					Settings.Charge_Vibe = value;
					persist_write_bool(Charge_Vibe_key, Settings.Charge_Vibe);
				}
			 
			 	if (Settings.Verbose){
					if (value)
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Vibe on charge turned on");
			 		else
						APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Vibe on charge turned off");
				}
			 
			 	break;
			 
			 case HIDE_BATTERY:
			 	if (value < 2){
					Settings.Hide_Battery = value;
					persist_write_bool(Hide_Battery_key, Settings.Hide_Battery);
					SetBarText(Settings.Hide_Battery, Settings.Hide_BT);
					
					if (Settings.Verbose){
						if (!value)
							APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Battery text shows");
						else
							APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Battery text hiding");
					}
				}
			 	
			 	break;
			 
			 case HIDE_BT:
			 	if (value < 2){
					Settings.Hide_BT = value;
					persist_write_bool(Hide_BT_key, Settings.Hide_BT);
					SetBarText(Settings.Hide_Battery, Settings.Hide_BT);
					
					if (Settings.Verbose){
						if (!value)
							APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: BT-state text shows");
						else
							APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: BT-state text hiding");
					}
				}
			 	
			 	break;
			 
			 case INFO_UPDATES_FREQUENCY:
			 	
			 	Settings.Info_Updates_Frequency = value;
			 	persist_write_int(Info_Updates_Frequency_key, Settings.Info_Updates_Frequency);
			 	if (Settings.Info_Updates_Frequency < 10)
					app_timer_register(MILLS_IN_HOUR / Settings.Info_Updates_Frequency, UpdateWeather, 0);
				else app_timer_register(MILLS_IN_HOUR, UpdateWeather, 0);
				 APP_LOG(APP_LOG_LEVEL_INFO, Buffer_Temp);
		 		
			 	if (Settings.Verbose){
					APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Info Refresh applied");
				}
			 
		 }	 
        t = dict_read_next(iter);
    }	
	JustRun_Flag = 0;
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

void ReadSettings(){
	Settings.Info_Updates_Frequency = 1;
	Settings.Hourly_Vibe            = 1;
	Settings.BT_Vibe                = 1;
	Settings.Verbose                = 1;
	Settings.Charge_Vibe            = 1;
	Settings.Hide_Battery           = 0;
	Settings.Hide_BT                = 0;
	
	Settings.Language               = ENGLISH_LANG;
	Settings.Inverted               = NON_INVERTED_WINDOW;
	strcpy(Settings.Location, "London");
	
	if (persist_exists(Location_key))
		persist_read_string(Location_key, Settings.Location, sizeof(Settings.Location));
	
	if (persist_exists(Hourly_Vibe_key)) 
		Settings.Hourly_Vibe = persist_read_int(Hourly_Vibe_key);

	if (persist_exists(BT_Vibe_key)) 
		Settings.BT_Vibe = persist_read_int(BT_Vibe_key);
	
	if (persist_exists(Info_Updates_Frequency_key)) 
		Settings.Info_Updates_Frequency = persist_read_int(Info_Updates_Frequency_key);
	
	if (persist_exists(Language_key)) 
		Settings.Language = persist_read_int(Language_key);
	
	if (persist_exists(Inverted_key)) 
		Settings.Inverted = persist_read_int(Inverted_key);
	
	if (persist_exists(Charge_Vibe_key)) 
		Settings.Charge_Vibe = persist_read_int(Charge_Vibe_key);
	
	if (persist_exists(Hide_Battery_key)) 
		Settings.Hide_Battery = persist_read_int(Hide_Battery_key);
	
	if (persist_exists(Hide_BT_key)) 
		Settings.Hide_BT = persist_read_int(Hide_BT_key);
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
	
	if (!bluetooth_connection_service_peek()){
		text_layer_set_text(CWeather_Text, OfflineNames[Settings.Language]);
	}
	else {
		send_int(CURRENT_WEATHER, 1);
	}
	
	if (Settings.Info_Updates_Frequency)
		app_timer_register(MILLS_IN_HOUR / Settings.Info_Updates_Frequency, UpdateWeather, 0);
	else app_timer_register(MILLS_IN_HOUR, UpdateWeather, 0);
}

static void UpdateConnection(bool Connected){
	if ( (!JustRun_Flag)&&(Settings.BT_Vibe) )
		vibes_long_pulse();
	
	if (Connected)
		UpdateWeather();
	
	gbitmap_destroy(BT); 
	BT = gbitmap_create_with_resource(BT_Icons[Connected]); 
	bitmap_layer_set_bitmap(BT_Image, BT);
	
	text_layer_set_text(Connection_Text, BTNames[Settings.Language][Connected]);
	
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: BT-Connection was updated!");
}

static void UpdateBattery(BatteryChargeState State){
	static char Percents[] = "100%";
	static int  Vibe_Flag;
	
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

	text_layer_set_text(Week_Text, DayNames[Settings.Language][CurrentTime->tm_wday]);
	
	if ( (CurrentTime -> tm_min == 0) && (!JustRun_Flag) && (Settings.Hourly_Vibe) )
		vibes_double_pulse();
}

void UpdateTime_Force(){
	now = time(NULL);
    current_time = localtime(&now);
    UpdateTime(current_time, SECOND_UNIT);
}

int main(void) {
	handle_init();
	
	ReadSettings();
	SetColors(Settings.Inverted);
	SetBarText(Settings.Hide_Battery, Settings.Hide_BT);
	JustRun_Flag = 1;
	UpdateTime_Force();
	
	UpdateConnection(bluetooth_connection_service_peek());
	
	if (!bluetooth_connection_service_peek())
		text_layer_set_text(CWeather_Text, OfflineNames[Settings.Language]);
	
	BatteryTimer();
	
	tick_timer_service_subscribe(MINUTE_UNIT, &UpdateTime);
	battery_state_service_subscribe(UpdateBattery);
	bluetooth_connection_service_subscribe(UpdateConnection);
	
	JustRun_Flag = 0;

	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: App running!");
	
	window_stack_push(MainWindow, true);
	
  	app_event_loop();
  	handle_deinit();
}