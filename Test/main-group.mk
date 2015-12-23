all:
	gcc ../Source/tool.c main-group.c `xml2-config --cflags` -o main-group.do `xml2-config --libs`
warning:
	gcc ../Source/tool.c main-group.c `xml2-config --cflags` -o main-group.do -W all -W extra `xml2-config --libs`
clean:
	rm --force main-group.do
