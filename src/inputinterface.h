#ifndef INPUTINTERFACE_H
#define INPUTINTERFACE_H

class CBEnchanted;

class InputInterface {
	public:
		InputInterface();
		
		void commandCloseInput(void);
		void commandWaitKey(void);
		void commandClearKeys(void);
		void commandPositionMouse(void);
		void commandWaitMouse(void);
		void commandShowMouse(void);
		void commandClearMouse(void);
		void commandSAFEEXIT(void);
		
		void functionInput(void);
		void functionKeyDown(void);
		void functionKeyHit(void);
		void functionKeyUp(void);
		void functionGetKey(void);
		void functionWaitKey(void);
		void functionMouseDown(void);
		void functionMouseHit(void);
		void functionMouseUp(void);
		void functionGetMouse(void);
		void functionWaitMouse(void);
		void functionMouseX(void);
		void functionMouseY(void);
		void functionMouseWX(void);
		void functionMouseWY(void);
		void functionMouseZ(void);
		void functionMouseMoveX(void);
		void functionMouseMoveY(void);
		void functionMouseMoveZ(void);
		void functionLeftKey(void);
		void functionRightKey(void);
		void functionUpKey(void);
		void functionDownKey(void);
		void functionEscapeKey(void);
	private:
		CBEnchanted *cb;
};

#endif
