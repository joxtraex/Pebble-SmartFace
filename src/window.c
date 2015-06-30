#include <pebble.h>
#include "window.h"

extern Window *MainWindow;
extern TextLayer *Time_Text;
extern TextLayer *Week_Text;
extern TextLayer *Date_Text;
extern TextLayer *Battery_Text;
extern TextLayer *Connection_Text;
extern TextLayer *CWeather_Text;

extern BitmapLayer *BT_Image;
extern BitmapLayer *BAT_Image;

extern GBitmap *BT;
extern GBitmap *BAT;

extern GFont Bar_Font;
extern GFont Time_Font;
extern GFont Date_Font;
extern GFont CWeather_Font;
	
void BuildWindow(){
	MainWindow = window_create();
	window_set_background_color(MainWindow, GColorWhite);
	
	/*BT Icon*/
	BT = gbitmap_create_with_resource(RESOURCE_ID_BT_ICON);
	BT_Image = bitmap_layer_create(GRect(0, 2, 16, 16));
	bitmap_layer_set_bitmap(BT_Image, BT);
  	layer_add_child(window_get_root_layer(MainWindow), bitmap_layer_get_layer(BT_Image));
	
	/*Battery Icon*/
	BAT = gbitmap_create_with_resource(RESOURCE_ID_BAT_ICON_100);
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
	CWeather_Text = text_layer_create(GRect(0, 147, 144, 25)); //105
	text_layer_set_text_color(CWeather_Text, GColorBlack);
	text_layer_set_text_alignment(CWeather_Text, GTextAlignmentCenter);
	text_layer_set_font(CWeather_Text, CWeather_Font);
	text_layer_set_text(CWeather_Text, "LOADING...");
	layer_add_child(window_get_root_layer(MainWindow), text_layer_get_layer(CWeather_Text));
	
	
}

void DestroyWindow(){
	text_layer_destroy(Time_Text);
	text_layer_destroy(Week_Text);
	text_layer_destroy(CWeather_Text);
	text_layer_destroy(Battery_Text);
	text_layer_destroy(Connection_Text);
	gbitmap_destroy(BT); 
	gbitmap_destroy(BAT); 
	bitmap_layer_destroy(BT_Image);
	bitmap_layer_destroy(BAT_Image);
	window_destroy(MainWindow);
}
					   