#ifndef SOUNDINTERFACE_H
#define SOUNDINTERFACE_H

#include "precomp.h"
#include <set>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include "cbsound.h"
#include "cbchannel.h"
class CBEnchanted;

class SoundInterface {
	public:
#ifndef CBE_LIB
		SoundInterface();
		~SoundInterface();

		void commandPlaySound(void);
		void commandSetSound(void);
		void commandStopSound(void);
		void commandDeleteSound(void);

		void functionLoadSound(void);
		void functionPlaySound(void);
		void functionSoundPlaying(void);

		bool initializeSounds();
		void cleanupSoundInterface(void);
#endif

		void updateAudio(void);
		inline ALLEGRO_MIXER* getMixer() {return cbMixer;}
		CBSound* getSound(int32_t id);
		CBChannel* getChannel(int32_t id);
	private:
		inline int32_t nextSampleId(void){static int32_t idsamples = 0; return ++idsamples;}
		inline int32_t nextChannelId(void){static int32_t idchannels = 0; return ++idchannels;}
		std::map<int32_t, CBSound*> sounds;
		std::map<int32_t, CBChannel*> channels;

		CBEnchanted *cb;
		ALLEGRO_MIXER* cbMixer;
};

#endif
