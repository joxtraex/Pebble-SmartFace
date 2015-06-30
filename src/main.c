#include <pebble.h>
#include "window.h"
	
Window *MainWindow;
TextLayer *Time_Text;
TextLayer *Week_Text;
TextLayer *Date_Text;
TextLayer *CWeather_Text;
TextLayer *Battery_Text;
TextLayer *Connection_Text;

BitmapLayer *BT_Image;
BitmapLayer *BAT_Image;

GBitmap *BT;
GBitmap *BAT;

GFont Bar_Font;
GFont Time_Font;
GFont Date_Font;
GFont CWeather_Font;

int JustRun_Flag = 1;

AppTimer *BatteryUpdate_Timer;

static char Time[] = "00:00";
static char Date[]="26.06.1996";
static char Week[]="wednesday";

char Buffer [32];

enum {
	CURRENT_WEATHER = 0,
	VERBOSE = 1,
	WEATHER_UPD_PERIOD = 3,
	WEATHER_CITY = 4
};

struct {
	bool Verbose;
	int Weather_Update_Period;
	char City [32];
	bool Hourly_Vibe;
	bool BT_Vibe;
} Settings;


void GetSettings(){
	Settings.Verbose = 1;
	Settings.Weather_Update_Period = 60*30*1000;
	Settings.Hourly_Vibe = 1;
	Settings.BT_Vibe = 1;
	strcpy(Settings.City, "Omsk");
}

static void Process_Received_Data(DictionaryIterator *iter, void *context){
	Tuple *t = dict_read_first(iter);
	int value = t->value->int32;
	char string_value[32];
	strcpy(string_value, t->value->cstring);
	snprintf(Buffer, sizeof(Buffer), "%s", string_value);
 	text_layer_set_text(CWeather_Text, Buffer); 
	
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: Weather was updated!");
}

void handle_init(void) {
	BuildWindow();
	app_message_register_inbox_received(Process_Received_Data);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	GetSettings();
	
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
	send_int(5, 5);
	app_timer_register(Settings.Weather_Update_Period, UpdateWeather, 0);
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
}

static void BatteryTimer(){
	UpdateBattery(battery_state_service_peek());
	BatteryUpdate_Timer = app_timer_register(3600000, BatteryTimer, 0);
	
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
	
	if (CurrentTime -> tm_min == 0)
		vibes_double_pulse();
}

int main(void) {
	handle_init();
	
	time_t now = time(NULL);
    struct tm *current_time = localtime(&now);
    UpdateTime(current_time, SECOND_UNIT);
	
	UpdateConnection(bluetooth_connection_service_peek());
	BatteryTimer();
	
	tick_timer_service_subscribe(MINUTE_UNIT, &UpdateTime);
	battery_state_service_subscribe(UpdateBattery);
	bluetooth_connection_service_subscribe(UpdateConnection);
	
	window_stack_push(MainWindow, true);
	JustRun_Flag = 0;
	
	if (Settings.Verbose)
		APP_LOG(APP_LOG_LEVEL_INFO, "SmartFace: App running!");
	
  	app_event_loop();
  	handle_deinit();
}