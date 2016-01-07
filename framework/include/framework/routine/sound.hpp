#ifndef __SOUND_H_1438443855__
#define __SOUND_H_1438443855__
#include "framework/fwcommon.hpp"
#include "framework/memory/cache_ptr.hpp"
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
	 * @param num_frames The number of samples per channel
	 */
	void deinterleave2(const PcmSample* block, PcmSample *out, unsigned int num_frames);
	
	/**
	 * @brief Deinterleave 2 channels (consumes input)
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
	 * This function will consume the input block
	 * 
	 * @param in cache_ptr of interleaved samples
	 * @param out Pointer to destination of samples
	 * @param num_frames The number of frames to process
	 */
	void deinterleave2(strangeio::memory::cache_ptr in, PcmSample *out, unsigned int num_frames);
	
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
	 * @param in Pointer to the block of channel split samples
	 * @param out Pointer to destination of samples
	 * @param period_size The number of samples per channel
	 */
	void interleave2(const PcmSample* in, PcmSample* out, unsigned int num_frames);
	/**
	 * @brief Interleave 2 channels (consume input)
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
	 * This function will consume the input block
	 *
	 * @param in Pointer to the block of channel split samples
	 * @param out Pointer to destination of samples
	 * @param period_size The number of samples per channel
	 */
	void interleave2(strangeio::memory::cache_ptr in, PcmSample* out, unsigned int num_frames);
	
	void sinewave_mono(PcmSample* out, unsigned int fc, unsigned int fs, unsigned int numSamples);
}
} // helpers
} // StrangeIO
#endif
