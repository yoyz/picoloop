
VERSION=0.9.1
APP=reMID

all:	
	cd src && make && cp $(APP) ..

clean:
	rm -f $(APP)
	cd src && make clean

dist:	clean
	ln -s $(APP) ../$(APP)-$(VERSION)
	tar zchf $(APP)-$(VERSION).tar.gz --exclude '*.tar' --exclude '*.tar.gz' --exclude .git --exclude '*.swp' --exclude TODO ../$(APP)-$(VERSION)
	rm ../$(APP)-$(VERSION)

test:	all
	./$(APP) -s 0 -j system:playback_1 -j system:playback_2 -m 28:1
	#./$(APP) -s 0 -j system:playback_1 -j system:playback_2 -m system:midi_capture_7

dtest:	all
	./$(APP) -s 0 -j system:playback_1 -j system:playback_2 -m 28:1 -d

	
