#include <pebble.h>

Window *MainWindow;
TextLayer *Time_Text;
TextLayer *Week_Text;
TextLayer *Date_Text;
TextLayer *CWeater_Text;
TextLayer *Battery_Text;
TextLayer *Connection_Text;

GFont Bar_Font;
GFont Time_Font;
GFont Date_Font;
GFont CWeather_Font;

void handle_init(void) {
  MainWindow = window_create();
	window_set_background_color(MainWindow, GColorWhite);
	
	/*Battery percent Text*/
	Bar_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_12));
  Battery_Text = text_layer_create(GRect(0, 0, 124, 16));
	text_layer_set_text_color(Battery_Text, GColorBlack);
	text_layer_set_background_color(Battery_Text, GColorClear);
	text_layer_set_text_alignment(Battery_Text, GTextAlignmentRight);
	text_layer_set_font(Battery_Text, Bar_Font);
	text_layer_set_text(Battery_Text, "100%");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Battery_Text));
  window_stack_push(MainWindow, true);
	
	/*Bluetooth connection percent Text*/
  Connection_Text = text_layer_create(GRect(20, 0, 124, 16));
	text_layer_set_text_color(Connection_Text, GColorBlack);
	text_layer_set_background_color(Connection_Text, GColorClear);
	text_layer_set_text_alignment(Connection_Text, GTextAlignmentLeft);
	text_layer_set_font(Connection_Text, Bar_Font);
	text_layer_set_text(Connection_Text, "NONE");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Connection_Text));
  window_stack_push(MainWindow, true);
	
	/*Week Day Text*/
	Date_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_18));
  Week_Text = text_layer_create(GRect(0, 20, 144, 30));
	text_layer_set_text_color(Week_Text, GColorBlack);
	text_layer_set_background_color(Week_Text, GColorClear);
	text_layer_set_text_alignment(Week_Text, GTextAlignmentCenter);
	text_layer_set_font(Week_Text, Date_Font);
	text_layer_set_text(Week_Text, "");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Week_Text));
  window_stack_push(MainWindow, true);
	
	/*Time Text*/
	Time_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_36));
  Time_Text = text_layer_create(GRect(0, 34, 144, 50));
	text_layer_set_text_color(Time_Text, GColorBlack);
	text_layer_set_background_color(Time_Text, GColorClear);
	text_layer_set_text_alignment(Time_Text, GTextAlignmentCenter);
	text_layer_set_font(Time_Text, Time_Font);
	text_layer_set_text(Time_Text, "");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Time_Text));
	
	/*Date Text*/
	Date_Text = text_layer_create(GRect(0, 75, 144, 25));
	text_layer_set_text_color(Date_Text, GColorBlack);
	text_layer_set_text_alignment(Date_Text, GTextAlignmentCenter);
	text_layer_set_font(Date_Text, Date_Font);
	text_layer_set_text(Date_Text, "");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(Date_Text));
  window_stack_push(MainWindow, true);
	
	/*Current weather Text*/
	CWeather_Font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_IMAGINE_TIME_14));
	CWeater_Text = text_layer_create(GRect(0, 100, 144, 25));
	text_layer_set_text_color(CWeater_Text, GColorBlack);
	text_layer_set_text_alignment(CWeater_Text, GTextAlignmentCenter);
	text_layer_set_font(CWeater_Text, CWeather_Font);
	text_layer_set_text(CWeater_Text, "CLOUDY, +26C");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(CWeater_Text));
	
	
  window_stack_push(MainWindow, true);
}

void handle_deinit(void) {
  text_layer_destroy(Time_Text);
	tick_timer_service_unsubscribe(); 
  window_destroy(MainWindow);
}

static void UpdateConnection(bool Connected){
	vibes_double_pulse();
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

static void UpdateTime(struct tm* CurrentTime, TimeUnits units_changed){
	static char Time[] = "00:00";
	static char Date[]="26.06.1996";
	static char Week[]="wednesday";
	
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
	tick_timer_service_subscribe(MINUTE_UNIT, &UpdateTime);
	UpdateConnection(bluetooth_connection_service_peek());
	UpdateBattery(battery_state_service_peek());
	battery_state_service_subscribe(UpdateBattery);
	bluetooth_connection_service_subscribe(UpdateConnection);
  app_event_loop();
  handle_deinit();
}