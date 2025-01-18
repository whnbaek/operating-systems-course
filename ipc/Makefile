all:
	gcc -o time_service time_service.c main_time_server.c `pkg-config --cflags --libs gio-2.0 glib-2.0 gio-unix-2.0`


clean:
	rm ./time_service
