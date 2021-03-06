#ifndef TEXTINTERFACE_H
#define TEXTINTERFACE_H

class CBEnchanted;
#include "precomp.h"
#include "rendertarget.h"
#include "isstring.h"

struct AddText{
	ISString txt;
	ALLEGRO_FONT *font;
	int32_t txtX;
	int32_t txtY;
	ALLEGRO_COLOR col;
};

class TextInterface {
	public:
#ifndef CBE_LIB
		TextInterface();
		~TextInterface();

		void commandSetFont(void);
		void commandDeleteFont(void);
		void commandText(void);
		void commandCenterText(void);
		void commandVerticalText(void);
		void commandPrint(void);
		void commandWrite(void);
		void commandLocate(void);
		void commandAddText(void);
		void commandClearText(void);

		void functionLoadFont(void);
		void functionTextWidth(void);
		void functionTextHeight(void);
#endif

		void renderAddTexts(RenderTarget &r);
		bool initializeFonts();
		ALLEGRO_FONT *getCurrentFont() { return currentFont; }
		int32_t getLocationX() { return locationX; }
		int32_t getLocationY() { return locationY; }
	private:
		CBEnchanted *cb;
		map<int32_t, ALLEGRO_FONT*> fontMap;
		ALLEGRO_FONT *currentFont;
		int32_t locationX;
		int32_t locationY;
		std::vector<AddText*> texts;
		inline int32_t nextfontid(){static int32_t fontID = 0; return ++fontID;}
};

#endif
