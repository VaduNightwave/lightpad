CXX = `wx-config --cxx`
CXXFLAGS = `wx-config --cxxflags` -O3
LIBS = `wx-config --libs` -lmagic -lrtmidi

ifeq ($(OS),Windows_NT)
	#CCFLAGS += -D WIN32
	CXXFLAGS += -D__NO_INLINE__
	ifeq ($(PROCESSOR_ARCHITEW6432),AMD64)
		#CCFLAGS += -D AMD64
	else
		ifeq ($(PROCESSOR_ARCHITECTURE),AMD64)
			#CCFLAGS += -D AMD64
		endif
		ifeq ($(PROCESSOR_ARCHITECTURE),x86)
			#CCFLAGS += -D IA32
		endif
	endif
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		# CCFLAGS += -D LINUX
	endif
	ifeq ($(UNAME_S),Darwin)
		# CCFLAGS += -D OSX
		CXXFLAGS += -D__MACOSX_CORE__
		LIBS += -framework CoreMIDI -framework CoreAudio -framework CoreFoundation
	endif
	UNAME_P := $(shell uname -p)
	ifeq ($(UNAME_P),x86_64)
		# CCFLAGS += -D AMD64
	endif
	ifneq ($(filter %86,$(UNAME_P)),)
		# CCFLAGS += -D IA32
	endif
	ifneq ($(filter arm%,$(UNAME_P)),)
		# CCFLAGS += -D ARM
	endif
endif

lightpad: main.o FilePanel.o Colors.o
	$(CXX) $(CXXFLAGS) -o lightpad main.o FilePanel.o Colors.o $(LIBS)

main.o: main.cpp IDs.h
FilePanel.o: FilePanel.cpp FilePanel.h IDs.h
Colors.cpp: Colors_template.cpp graphics/references/rgbcolors1.png graphics/references/rgbcolors2.png sample_colors.py
	python sample_colors.py > Colors.cpp
Colors.o: Colors.cpp Colors.h

colors: Colors.o
	$(CXX) $(CXXFLAGS) -o colors Colors.o $(LIBS)

midiprobe: midiprobe.cpp
	$(CXX) $(CXXFLAGS) -o midi midiprobe.cpp $(LIBS)

midiout: midiout.cpp
	$(CXX) $(CXXFLAGS) -o midi midiout.cpp $(LIBS)

clean:
	rm *.o
	rm lightpad
	rm colors
	rm midi