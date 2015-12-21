all:
	gcc tool.c main-passwd.c $(xml2-config --cflags) -o main-passwd.do $(xml2-config --libs)
warning:
	gcc tool.c main-passwd.c $(xml2-config --cflags) -o main-passwd.do -W all -W extra $(xml2-config --libs)
clean:
	rm --force main-passwd.do
