#ifndef GFXINTERFACE_H
#define GFXINTERFACE_H

#include <SFML/Graphics.hpp>
#include "rendertarget.h"

class CBEnchanted;

class GfxInterface {
	public:
		GfxInterface();
		~GfxInterface();
		
		void commandScreen(void);
		void commandLock(void);
		void commandUnlock(void);
		void commandPutPixel(void);
		void commandPutPixel2(void);
		void commandCopyBox(void);
		void commandColor(void);
		void commandClsColor(void);
		void commandCls(void);
		void commandDot(void);
		void commandLine(void);
		void commandBox(void);
		void commandCircle(void);
		void commandEllipse(void);
		void commandPickColor(void);
		void commandScreenGamma(void);
		void commandDrawToImage(void);
		void commandDrawToScreen(void);
		void commandDrawToWorld(void);
		void commandSmooth2D(void);
		void commandScreenShot(void);
		void commandUpdateGame(void);
		void commandDrawGame(void);
		void commandDrawScreen(void);

		void functionSCREEN(void);
		void functionImage(void);
		void functionGetPixel(void);
		void functionGetPixel2(void);
		void functionGetRGB(void);
		void functionScreenWidth(void);
		void functionScreenHeight(void);
		void functionScreenDepth(void);
		void functionGFXModeExists(void);

        int32_t getFPS()const{return currentFPS;}

        sf::RenderWindow *getWindow(void) { return &this->window; }
		sf::Color getDrawColor() { return drawColor; }
        inline bool getDrawDrawCommandToWorld()const{return drawDrawCommandToWorld;}
        inline bool getDrawImageToWorld()const{return drawImageToWorld;}
        inline bool getDrawTextToWorld()const{return drawTextToWorld;}
        inline RenderTarget *getCurrentRenderTarget(){return currentRenderTarget;}
        void setCurrentRenderTarget(RenderTarget *t);

        void initializeGfx();
	private:
		CBEnchanted *cb;
		
		string windowTitle;
        sf::RenderWindow window;
		sf::Color clearColor;
		sf::Color drawColor;
        int32_t fpsCounter;
        int32_t currentFPS;
        clock_t lastSecTimer;
        RenderTarget *currentRenderTarget;

        RenderTarget windowRenderTarget;
        bool drawDrawCommandToWorld;
        bool drawImageToWorld;
        bool drawTextToWorld;

        std::map<int32_t,RenderTarget*> bufferMap;
};

#endif
