//
// Lightpad - DisplayPanel.cpp
// Created by Vinyl Darkscratch and Light Apacha, ©2017 Nightwave Studios.
//

// Attempt to load precompiled, if compiler doesn't support then load normal
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/sizer.h>

#include <sstream>
#include <algorithm>

#ifdef _WIN32
	//define something for Windows (32-bit and 64-bit, this part is common)
	#define WINDOWS
	#ifdef _WIN64
		//define something for Windows (64-bit only)
		#define WINDOWS_64
	#endif
#elif __APPLE__
	//define something for Mac
	#define MACOS
#else
	//#error "Unknown/unsupported compiler/operating system"
#endif

#include "DisplayPanel.h"
#include "Colors.h"
#include "Helpers.h"

// Initialize the file panel and it's elements
DisplayPanel::DisplayPanel(wxPanel *parent)
	   : wxPanel(parent, ID_Panel_Display, wxPoint(-1, -1), wxSize(-1, -1), wxBORDER_SUNKEN) {
	m_parent = parent;

	image_path = "graphics/launchpad_display/launchpad_display_4096.png";
	image_size = 4096;
	launchpad_image = new wxImage(image_path, wxBITMAP_TYPE_PNG);
	panel_width = image_size;
	panel_height = image_size;
	xpos = 0;
	ypos = 0;

	paintNow();
}

void DisplayPanel::paintEvent(wxPaintEvent &evt) {
	// depending on your system you may need to look at double-buffered dcs
	wxPaintDC dc(this);
	render(dc);
}

void DisplayPanel::paintNow() {
	// depending on your system you may need to look at double-buffered dcs
	wxClientDC dc(this);
	render(dc);
}

void DisplayPanel::OnSize(wxSizeEvent& event){
	Refresh();
	//skip the event.
	event.Skip();
}

void DisplayPanel::render(wxDC &dc) {
	int neww;
	int newh;
	std::stringstream ss;
	dc.GetSize(&neww, &newh);

	if( neww != panel_width || newh != panel_height ) {
		int newsize = std::min(neww, newh);
		if (newsize == 0) {
			newsize = image_size; // When first launching the app, this will make sure it doesn't crash
		}
		// int do_new_image = 0;

		// if (x <= 256 && image_size != 256) {
		// 	image_path.replace(45, 4, "0256");
		// 	image_size = 256;
		// } else if (x > 256 && x <= 512 && image_size != 512) {
		// 	image_path.replace(45, 4, "0512");
		// 	image_size = 512;
		// } else if (x > 512 && x <= 1024 && image_size != 1024) {
		// 	image_path.replace(45, 4, "1024");
		// 	image_size = 1024;
		// } else if (x > 1024 && x <= 2048 && image_size != 2048) {
		// 	image_path.replace(45, 4, "2048");
		// 	image_size = 2048;
		// } else if (x > 2048 && x <= 4096 && image_size != 4096) {
		// 	image_path.replace(45, 4, "4096");
		// 	image_size = 4096;
		// } else {
		// 	do_new_image = 0;
		// }

		// if (do_new_image == 1) launchpad_image->LoadFile(image_path, wxBITMAP_TYPE_PNG);
		resized = wxBitmap(launchpad_image->Scale(newsize, newsize));

		if ( neww > newh ) {
			xpos = (neww - newsize)/2;
			ypos = 0;
		} else {
			xpos = 0;
			ypos = (newh - newsize)/2;
		}

		dc.DrawBitmap(resized, xpos, ypos, false);

		panel_width = neww;
		panel_height = newh;
	} else {
		dc.DrawBitmap(resized, xpos, ypos, false);
	}
}

wxBEGIN_EVENT_TABLE(DisplayPanel, wxPanel)
	EVT_PAINT(DisplayPanel::paintEvent)
	EVT_SIZE(DisplayPanel::OnSize)
wxEND_EVENT_TABLE()
