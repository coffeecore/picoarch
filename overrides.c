#include "overrides.h"
#include "overrides/arduous.h"
#include "overrides/beetle-pce-fast.h"
#include "overrides/bluemsx.h"
#include "overrides/dosbox-pure.h"
#include "overrides/fake-08.h"
#include "overrides/fbalpha2012.h"
#include "overrides/fceumm.h"
#include "overrides/fmsx.h"
#include "overrides/gambatte.h"
#include "overrides/gme.h"
#include "overrides/gpsp.h"
#include "overrides/mame2000.h"
#include "overrides/mednafen_ngp.h"
#include "overrides/mednafen_wswan.h"
#include "overrides/pcsx_rearmed.h"
#include "overrides/picodrive.h"
#include "overrides/pokemini.h"
#include "overrides/prboom.h"
#include "overrides/quicknes.h"
#include "overrides/scummvm.h"
#include "overrides/smsplus.h"
#include "overrides/snes9x2002.h"
#include "overrides/snes9x2005.h"
#include "overrides/snes9x2005_plus.h"
#include "overrides/snes9x2010.h"
#include "overrides/stella2014.h"
#include "overrides/tyrquake.h"
#include "overrides/vitaquake2.h"
#include "util.h"

static const struct core_override overrides[] = {
	arduous_overrides,
	beetle_pce_fast_overrides,
	bluemsx_overrides,
	dosbox_pure_overrides,
	fake08_overrides,
	fbalpha2012_overrides,
	fceumm_overrides,
	fmsx_overrides,
	gambatte_overrides,
	gme_overrides,
	gpsp_overrides,
	mame2000_overrides,
	mednafen_ngp_overrides,
	mednafen_wswan_overrides,
	pcsx_rearmed_overrides,
	picodrive_overrides,
	pokemini_overrides,
	prboom_overrides,
	quicknes_overrides,
	scummvm_overrides,
	smsplus_overrides,
	snes9x2002_overrides,
	snes9x2005_overrides,
	snes9x2005_plus_overrides,
	snes9x2010_overrides,
	stella2014_overrides,
	tyrquake_overrides,
	vitaquake2_overrides,
};

static const struct core_override *override;

const struct core_override *get_overrides(void) {
	return override;
}

void set_overrides(const char *core_name) {
	override = NULL;

	for (int i = 0; i < array_size(overrides); i++) {
		if (!strcmp(core_name, overrides[i].core_name)) {
			override = &overrides[i];
			break;
		}
	}
}
