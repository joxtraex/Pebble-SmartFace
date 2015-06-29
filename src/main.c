#include <pebble.h>

Window *MainWindow;
TextLayer *Time_Text;
TextLayer *Week_Text;
TextLayer *Date_Text;
TextLayer *CWeater_Text;
TextLayer *Battery_Text;
TextLayer *Connection_Text;

static BitmapLayer *BT_Image;
static BitmapLayer *BAT_Image;

static GBitmap *BT;
static GBitmap *BAT;

GFont Bar_Font;
GFont Time_Font;
GFont Date_Font;
GFont CWeather_Font;

int JustRun_Flag = 1;

AppTimer *BatteryUpdate_Timer;

static char Time[] = "00:00";
static char Date[]="26.06.1996";
static char Week[]="wednesday";

char Buffer [] = "CLOUDS, 20C [RESERVED]";

enum {
	CURRENT_WEATHER = 0
};


const int Battery_Update_Period = 60 * 30 * 1000;
const int Weather_Update_Period = 60 * 30 * 1000;

static void Process_Received_Data(DictionaryIterator *iter, void *context){
	Tuple *t = dict_read_first(iter);
	int value = t->value->int32;
	char string_value[32];
	strcpy(string_value, t->value->cstring);
	snprintf(Buffer, sizeof(Buffer), "%s", string_value);
  text_layer_set_text(CWeater_Text, Buffer); 
}

void handle_init(void) {
  MainWindow = window_create();
	window_set_background_color(MainWindow, GColorWhite);
	
	/*BT Icon*/
	BT = gbitmap_create_with_resource(RESOURCE_ID_BT_ICON);
	BT_Image = bitmap_layer_create(GRect(0, 2, 16, 16));
	bitmap_layer_set_bitmap(BT_Image, BT);
  layer_add_child(window_get_root_layer(MainWindow), bitmap_layer_get_layer(BT_Image));
	
	/*Battery Icon*/
	BAT = gbitmap_create_with_resource(RESOURCE_ID_BAT_ICON);
	BAT_Image = bitmap_layer_create(GRect(127, 2, 16, 16));
	bitmap_layer_set_bitmap(BAT_Image, BAT);
  layer_add_child(window_get_root_layer(MainWindow), bitmap_layer_get_layer(BAT_Image));
	
	/*Battery percent Text*/
	Bar_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_12));
  Battery_Text = text_layer_create(GRect(16, 2, 112, 16));
	text_layer_set_text_color(Battery_Text, GColorBlack);
	text_layer_set_background_color(Battery_Text, GColorClear);
	text_layer_set_text_alignment(Battery_Text, GTextAlignmentRight);
	text_layer_set_font(Battery_Text, Bar_Font);
	text_layer_set_text(Battery_Text, "100%");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Battery_Text));
	
	/*Bluetooth connection percent Text*/
  Connection_Text = text_layer_create(GRect(17, 2, 124, 16));
	text_layer_set_text_color(Connection_Text, GColorBlack);
	text_layer_set_background_color(Connection_Text, GColorClear);
	text_layer_set_text_alignment(Connection_Text, GTextAlignmentLeft);
	text_layer_set_font(Connection_Text, Bar_Font);
	text_layer_set_text(Connection_Text, "NONE");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Connection_Text));
	
	/*Week Day Text*/
	Date_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_18));
  Week_Text = text_layer_create(GRect(0, 40, 144, 30)); //20
	text_layer_set_text_color(Week_Text, GColorBlack);
	text_layer_set_background_color(Week_Text, GColorClear);
	text_layer_set_text_alignment(Week_Text, GTextAlignmentCenter);
	text_layer_set_font(Week_Text, Date_Font);
	text_layer_set_text(Week_Text, "");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Week_Text));
	
	/*Time Text*/
	Time_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_36));
  Time_Text = text_layer_create(GRect(0, 54, 144, 50)); //34
	text_layer_set_text_color(Time_Text, GColorBlack);
	text_layer_set_background_color(Time_Text, GColorClear);
	text_layer_set_text_alignment(Time_Text, GTextAlignmentCenter);
	text_layer_set_font(Time_Text, Time_Font);
	text_layer_set_text(Time_Text, "");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Time_Text));
	
	/*Date Text*/
	Date_Text = text_layer_create(GRect(0, 95, 144, 25)); //75
	text_layer_set_text_color(Date_Text, GColorBlack);
	text_layer_set_text_alignment(Date_Text, GTextAlignmentCenter);
	text_layer_set_font(Date_Text, Date_Font);
	text_layer_set_text(Date_Text, "");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Date_Text));
	
	/*Current weather Text*/
	CWeather_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_14));
	CWeater_Text = text_layer_create(GRect(0, 147, 144, 25)); //105
	text_layer_set_text_color(CWeater_Text, GColorBlack);
	text_layer_set_text_alignment(CWeater_Text, GTextAlignmentCenter);
	text_layer_set_font(CWeater_Text, CWeather_Font);
	text_layer_set_text(CWeater_Text, "CLOUDY, +26C");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(CWeater_Text));

	app_message_register_inbox_received(Process_Received_Data);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
	
}

void handle_deinit(void) {
  text_layer_destroy(Time_Text);
	tick_timer_service_unsubscribe(); 
	battery_state_service_unsubscribe();
	bluetooth_connection_service_unsubscribe();
  window_destroy(MainWindow);
}

void send_int(uint8_t key, uint8_t cmd)
{
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
 
    Tuplet value = TupletInteger(key, cmd);
    dict_write_tuplet(iter, &value);
 
    app_message_outbox_send();
	
	  APP_LOG(APP_LOG_LEVEL_INFO, "Data sended");
}

void UpdateWeather(){
      send_int(5, 5);
			APP_LOG(APP_LOG_LEVEL_INFO, "Weather will update...");
			app_timer_register(Weather_Update_Period, UpdateWeather, 0);
}

static void UpdateConnection(bool Connected){
	if (!JustRun_Flag)
		vibes_long_pulse();
	
	if (Connected)
		text_layer_set_text(Connection_Text, "OK");
	else
		text_layer_set_text(Connection_Text, "LOST");
}

static void UpdateBattery(BatteryChargeState State){
	static char Percents[] = "100%";
	snprintf(Percents, sizeof(Percents), "%d%%", State.charge_percent);
	text_layer_set_text(Battery_Text, Percents);
}

static void BatteryTimer(){
	UpdateBattery(battery_state_service_peek());
	BatteryUpdate_Timer = app_timer_register(Battery_Update_Period, BatteryTimer, 0);
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
	
	tick_timer_service_subscribe(MINUTE_UNIT, &UpdateTime);
	UpdateConnection(bluetooth_connection_service_peek());
	
	UpdateBattery(battery_state_service_peek());
	battery_state_service_subscribe(UpdateBattery);
	
	bluetooth_connection_service_subscribe(UpdateConnection);
	BatteryTimer();
	UpdateWeather();
	
	window_stack_push(MainWindow, true);
	JustRun_Flag = 0;
	
  app_event_loop();
  handle_deinit();
}