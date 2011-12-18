#ifndef STRINGINTERFACE_H
#define STRINGINTERFACE_H

class CBEmu;

class StringInterface {
	public:
		StringInterface();
		
		void functionStr(void);
		void functionLeft(void);
		void functionRight(void);
		void functionMid(void);
		void functionReplace(void);
		void functionInStr(void);
		void functionUpper(void);
		void functionLower(void);
		void functionTrim(void);
		void functionLSet(void);
		void functionRSet(void);
		void functionChr(void);
		void functionAsc(void);
		void functionLen(void);
		void functionHex(void);
		void functionBin(void);
		void functionString(void);
		void functionFlip(void);
		void functionStrInsert(void);
		void functionStrRemove(void);
		void functionStrMove(void);
		void functionCountWords(void);
		void functionGetWord(void);
	private:
		CBEmu *cb;

};

#endif
