#ifndef __SOUND_H_1438443855__
#define __SOUND_H_1438443855__
#include "framework/fwcommon.hpp"
namespace strangeio {
namespace routine {
namespace sound {

	
	/**
	 * @brief Deinterleave 2 channels
	 * 
	 * Deinterleave two channels from LRLRLR to LLLRRR
	 * format; it splits the channels around the period size.
	 * 
	 * The function assumes standard L R L R interleaving
	 * where the first index (block[0]) is always the left
	 * channel. It also assumes both channels have a complete
	 * number of samples, given the period size. So if the
	 * period is 256, each channel has 256 samples in the block.
	 * 
	 * @param block Pointer to the block of interleaved samples
	 * @param out Pointer to destination of samples
	 * @param period_size The number of samples per channel
	 */
	void deinterleave2(const PcmSample* block, PcmSample *out, unsigned int numSamples);
	/**
	 * @brief Interleave 2 channels
	 * 
	 * Interleave two channels from LLLRRR to LRLRLR format;
	 * it assumes the channel split is around the period size.
	 * 
	 * This function will assume the standard L R L R interleaving
	 * format, where the first index (block[0]) is always the left
	 * channel. It also assumes that both channels have a complete
	 * number of samples, given the period size. So if the period
	 * is 256, each channel has 256 samples in the block.
	 * 
	 * @param block Pointer to the block of channel split samples
	 * @param out Pointer to destination of samples
	 * @param period_size The number of samples per channel
	 */
	void interleave2(const PcmSample* block, PcmSample* out, unsigned int numSamples);
}
} // helpers
} // StrangeIO
#endif
