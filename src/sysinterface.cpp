#include "precomp.h"
#include "cbenchanted.h"
#include "gfxinterface.h"
#include "sysinterface.h"
#include "errorsystem.h"
#include "meminterface.h"
#include <time.h>
#include <iostream>
#include <boost/crc.hpp>
#include <allegro5/allegro_memfile.h>
#ifdef WIN32
	#include <allegro5/allegro_windows.h>
	#include "utf8.h"
#else
	#include <unistd.h>
	#include <sys/time.h>
#endif
#include <allegro5/allegro_native_dialog.h>
#include "util.h"

#ifndef CBE_LIB
SysInterface::SysInterface() : windowTitle(""), confirmationStr("") {
	cb = CBEnchanted::instance();
}

SysInterface::~SysInterface() { }

void SysInterface::initializeSysInterface() {
	srand(mtimer());
}

void SysInterface::commandWait(void) {
	al_rest(double(cb->popValue().getInt()) / 1000.0);
}

void SysInterface::commandMakeError(void) {
	cb->errors->createFatalError(cb->popValue().toString().getUtf8Encoded(), "", "Error!");
}

void SysInterface::commandSaveProgram(void) {
	STUB;
}

void SysInterface::commandLoadProgram(void) {
	STUB;
}

void SysInterface::commandGotoSavedLocation(void) {
	STUB;
}

void SysInterface::commandFrameLimit(void) {
	int32_t fps_limit = cb->popValue().toInt();
	cb->setFrameLimit(fps_limit);
}

void SysInterface::commandEncrypt(void) {
	const string pass = cb->popValue().toString().getRef();
	const Any &dest = cb->popValue();
	const Any &src = cb->popValue();
	uint32_t srcSize;
	ALLEGRO_FILE *srcFile;
	ALLEGRO_FILE *destFile;
	if (src.type() == Any::Int) { //Memblock
		uint8_t *memBlock = cb->memInterface->getMemblock(src.getInt());
		srcSize = MemInterface::getMEMBlockSize(memBlock);
		srcFile = al_open_memfile(MemInterface::getMEMBlockData(memBlock), srcSize, "r");
	}
	else { //File
		ISString filePath = src.toString();
		srcFile = al_fopen(filePath.getRef().c_str(), "rb");
		if (srcFile == 0) {
			cb->errors->createError("Can't open file","Can't open source file " + filePath.getRef() + " given to Encrypt");
			return;
		}
		ALLEGRO_FS_ENTRY *file = al_create_fs_entry(filePath.getRef().c_str());
		srcSize = al_get_fs_entry_size(file);
	}

	if (dest.type() == Any::Int) {
		uint8_t *memBlock = cb->memInterface->getMemblock(src.getInt());
		if (srcSize > MemInterface::getMEMBlockSize(memBlock)) {
			cb->errors->createError("Memblock is too small", "Memblock given to Encrypt is too small to encrypt whole source data.");
			al_fclose(srcFile);
		}
		destFile = al_open_memfile(MemInterface::getMEMBlockData(memBlock), MemInterface::getMEMBlockSize(memBlock), "w");
	}
	else {
		ISString filePath = dest.toString();
		destFile = al_fopen(filePath.getRef().c_str(), "wb");
		if (destFile == 0) {
			cb->errors->createError("Can't write to file","Can't write to destination file " + filePath.getRef() + " given to Encrypt");
			al_fclose(srcFile);
			return;
		}
	}

	const unsigned char *passPtr = (const unsigned char*)pass.c_str();
	uint32_t passLength = pass.size();
	for (uint32_t i = 0; i < srcSize; i++) {
		uint8_t pc = passPtr[i % passLength];
		int c = al_fgetc(srcFile);
		uint8_t d = (unsigned char)c + pc;
		al_fputc(destFile, d);
	}

	al_fclose(srcFile);
	al_fclose(destFile);
}

void SysInterface::commandDecrypt(void) {
	const string pass = cb->popValue().toString().getRef();
	const Any &dest = cb->popValue();
	const Any &src = cb->popValue();
	uint32_t srcSize;
	ALLEGRO_FILE *srcFile;
	ALLEGRO_FILE *destFile;
	if (src.type() == Any::Int) { //Memblock
		uint8_t *memBlock = cb->memInterface->getMemblock(src.getInt());
		srcSize = MemInterface::getMEMBlockSize(memBlock);
		srcFile = al_open_memfile(MemInterface::getMEMBlockData(memBlock), srcSize, "r");
	}
	else { //File
		ISString filePath = src.toString();
		srcFile = al_fopen(filePath.getRef().c_str(), "rb");
		if (srcFile == 0) {
			cb->errors->createError("Can't open file","Can't open source file " + filePath.getRef() + " given to Decrypt");
			return;
		}
		ALLEGRO_FS_ENTRY * file = al_create_fs_entry(filePath.getRef().c_str());
		srcSize = al_get_fs_entry_size(file);
	}

	if (dest.type() == Any::Int) {
		uint8_t *memBlock = cb->memInterface->getMemblock(src.getInt());
		if (srcSize > MemInterface::getMEMBlockSize(memBlock)) {
			cb->errors->createError("Memblock is too small", "Memblock given to Decrypt is too small to decrypt whole source data.");
			al_fclose(srcFile);
		}
		destFile = al_open_memfile(MemInterface::getMEMBlockData(memBlock), MemInterface::getMEMBlockSize(memBlock), "w");
	}
	else {
		ISString filePath = dest.toString();
		destFile = al_fopen(filePath.getRef().c_str(), "wb");
		if (destFile == 0) {
			cb->errors->createError("Can't write to file","Can't write to destination file " + filePath.getRef() + " given to Decrypt");
			al_fclose(srcFile);
			return;
		}
	}

	const unsigned char *passPtr = (const unsigned char*)pass.c_str();
	uint32_t passLength = pass.size();
	for (uint32_t i = 0; i < srcSize; i++) {
		uint8_t pc = passPtr[i % passLength];
		int c = al_fgetc(srcFile);
		uint8_t d = (unsigned char)c - pc;
		al_fputc(destFile, d);
	}

	al_fclose(srcFile);
	al_fclose(destFile);
}

void SysInterface::commandCallDLL(void) {
	int32_t memblockOutId = cb->popValue().getInt();
	int32_t memblockInId = cb->popValue().getInt();
	const ISString& funcName = cb->popValue().getString();
	const ISString& dllName = cb->popValue().getString();
	CBDLL *dll = dlls[dllName.getRef()];
	if (dll == 0) {
		dll = new CBDLL;
		if (!dll->load(dllName)) {
			cb->errors->createError("Can't open dll file.", "Can't open "+dllName.getRef());
		}
		dlls[dllName.getRef()] = dll;
	}
	void *memIn = 0;
	void *memOut = 0;
	uint32_t memInSize = 0;
	uint32_t memOutSize = 0;
	if (memblockOutId) {
		uint8_t *memblockOut = cb->memInterface->getMemblock(memblockOutId);
		memOut = MemInterface::getMEMBlockData(memblockOut);
		memOutSize = MemInterface::getMEMBlockSize(memblockOut);
	}
	if (memblockInId) {
		uint8_t *memblockIn = cb->memInterface->getMemblock(memblockInId);
		memIn = MemInterface::getMEMBlockData(memblockIn);
		memInSize = MemInterface::getMEMBlockSize(memblockIn);
	}
	dll->call(funcName.getRef(), memIn, memInSize, memOut, memOutSize);
}

void SysInterface::commandErrors(void) {
	bool enabled = cb->popValue().toBool();
	cb->errors->setErrorMessages(enabled);
}

void SysInterface::commandSetWindow(void) {
	confirmationStr = cb->popValue().toString().getUtf8Encoded();
	uint32_t mode = cb->popValue().toInt();
	windowTitle = cb->popValue().toString().getUtf8Encoded();

	if (windowTitle.empty()) {
		al_set_window_title(cb->gfxInterface->getWindow(), "");
		return;
	}
#ifdef _WIN32
	// Windows is fucked up and doesn't use UTF-8
	HWND win = al_get_win_window_handle(cb->gfxInterface->getWindow());
	wstring widestr = utf8ToUtf16(windowTitle);

	if (!SetWindowTextW(win, &widestr[0])) {
		cb->errors->createError("Failed to set window title!");
	}

#else
	// Oh dear Linux, why do you so kindly accept UTF-8 <3
	al_set_window_title(cb->gfxInterface->getWindow(), windowTitle.c_str());
#endif

	if (mode != 0) {
		// Window mode changes!
#ifdef _WIN32
		int action = 0;
		switch (mode) {
			case 1:
				action = SW_RESTORE;
				break;
			case 2:
				action = SW_MINIMIZE;
				break;
			case 3:
				action = SW_MAXIMIZE;
				break;
		}
		// Minimizing window and then returning does not work yet.
		if (action && action != SW_MINIMIZE) {
			ShowWindow(win, action);
		}
// #elif something something, X11 here? OSX has to be handled separately as well
#endif
	}
}

void SysInterface::commandEnd(void) {
	cb->stop();
}

void SysInterface::functionDate(void) {
	time_t rtime;
	struct tm * timei;
	time (&rtime);
	timei = localtime(&rtime);

	string d(ctime(&rtime));

	short day  = timei->tm_mday;

	stringstream ssdate;
	ssdate << day << " " << d.substr(4, 3) << " " << d.substr(d.length() - 5, 4);

	cb->pushValue(ssdate.str());
}

void SysInterface::functionTime(void) {
	time_t rtime;
	time (&rtime);
	string t(ctime(&rtime));

	cb->pushValue(t.substr(11, 8));
}

void SysInterface::functionTimer(void) {
	cb->pushValue((int32_t)mtimer());
}

void SysInterface::functionCommandLine(void) {
	cb->pushValue(cb->getCommandLine());
}

void SysInterface::functionGetEXEName(void) {
	ALLEGRO_PATH* exePath = al_get_standard_path(ALLEGRO_EXENAME_PATH);
	cb->pushValue(string(al_path_cstr(exePath, ALLEGRO_NATIVE_PATH_SEP)));
	al_destroy_path(exePath);
}

void SysInterface::functionFPS(void) {
	cb->pushValue(cb->gfxInterface->getFPS());
}

void SysInterface::functionCrc32(void) {
	const Any &any = cb->popValue();
	boost::crc_32_type result;
	if (any.type() == Any::String) {
		const size_t bufferSize = 1024;
		char buffer[bufferSize];
		ALLEGRO_FILE *file = al_fopen(any.getString().getRef().c_str(),"rb");
		size_t readSize;
		do {
			readSize = al_fread(file, buffer, bufferSize);
			result.process_bytes(buffer, readSize);
		} while (readSize == bufferSize);
		al_fclose(file);
	}
	else if (any.type() == Any::Int) {
		uint8_t *memblock = cb->memInterface->getMemblock(any.getInt());
		result.process_bytes(MemInterface::getMEMBlockData(memblock), MemInterface::getMEMBlockSize(memblock));
	}
	cb->pushValue((int32_t)result.checksum());
}
#endif

/** Asks for user confirmation before closing the program, returns true if program should be terminated. */
bool SysInterface::askForExit() {
	if (confirmationStr.empty()) {
		return true;
	}

#ifdef _WIN32
	// Windows is fucked up and doesn't use UTF-8

	// Convert message and title to utf-16 with the amazing UTF8-CPP library
	wstring wideMsg = utf8ToUtf16(confirmationStr);
	wstring wideTitle = utf8ToUtf16(windowTitle);

	int ret = MessageBoxW(al_get_win_window_handle(cb->gfxInterface->getWindow()), &wideMsg[0], &wideTitle[0], MB_OKCANCEL | MB_ICONWARNING);

	if (ret == IDCANCEL) {
		return false;
	}

#else

	int pressed = al_show_native_message_box(cb->gfxInterface->getWindow(), windowTitle.c_str(), confirmationStr.c_str(), "", NULL, ALLEGRO_MESSAGEBOX_OK_CANCEL);
	if (pressed == 2 || pressed == 0) {
		// Pressed "Cancel" or closed the messagebox
		return false;
	}
#endif

	return true;
}
