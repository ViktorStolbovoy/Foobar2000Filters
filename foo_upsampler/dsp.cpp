#include "stdafx.h"
#include "DspFilters/Dsp.h"
#include <FilterConfig.h>
#include <Config.h>
#include <FilterControls.h>
#include <MainPanel.h>
#include "stdafx.h"

static void RunDSPConfigPopup(const dsp_preset & p_data, HWND p_parent, dsp_preset_edit_callback & p_callback);

static GUID g_get_guid() {
	//This is Upsampler specific GUID. 
	static const GUID guid = { 0x39dd7a18, 0xc3e0, 0x48b4, { 0xae, 0x31, 0xab, 0x5f, 0xdf, 0xd6, 0x63, 0x9f } };
	return guid;
}

class dsp_config
{
public:
	Config Data;

	dsp_config()
	{
		
	}

	void parse(const dsp_preset &in)
	{
		int size = in.get_data_size();
		if (size == sizeof(Data))
		{
			Data = *((Config *)in.get_data());
		}
		else
		{
			Config cNew;
			Data = cNew;
		}
	}

	void setPreset(dsp_preset_edit_callback & p_callback)
	{
		dsp_preset_impl preset;
		preset.set_owner(::g_get_guid());
		preset.set_data(&Data, sizeof(Data));
		p_callback.on_preset_changed(preset);
	}
};

class dsp_upsampler : public dsp_impl_base
{
public:
	dsp_upsampler(dsp_preset const & in) {
		m_filterCfg.parse(in);
		m_buffer = nullptr;
		m_buffer_len = 0;
		m_filter = nullptr;
		m_sampleRate = 0;
		m_rateMultiplier = 1; 
		m_channelCount = 0;
	}

	
	static GUID g_get_guid() {
		//This is Upsampler specific GUID. 
		return ::g_get_guid();
	}

	static void g_get_name(pfc::string_base & p_out) { p_out = "Upsampler"; }

	audio_sample * m_buffer;
	t_size m_buffer_len;

	bool on_chunk(audio_chunk * chunk, abort_callback &) {
		// Perform any operations on the chunk here.
		// The most simple DSPs can just alter the chunk in-place here and skip the following functions.

		//chunk->grow_data_size();
		const int sr = chunk->get_sample_rate();
		const unsigned int chCount = chunk->get_channel_count();
		if (m_sampleRate != sr || m_channelCount != chCount)
		{
			auto cfg = m_filterCfg.Data.getConfigForSourceSampleRate(sr);
			if (cfg)
			{
				m_rateMultiplier = cfg->FMultiplier;
				m_filter = m_filterCfg.Data.createFilter(sr, chCount);
			}
			else
			{
				m_filter = nullptr;
				m_rateMultiplier = 1;
			}
			m_channelCount = chCount;
			m_sampleRate = sr;
			m_multiplier = (m_filter) ? ((m_rateMultiplier == 4) ? 1.41 : 1.2) : 1;
		}

		//unsigned int cconfig = chunk->get_channel_config(); - don't really care
		audio_sample* data = chunk->get_data();
		const t_size size = chunk->get_data_size();
		const t_size sampleCnt = chunk->get_sample_count();


		if (m_rateMultiplier < 2 || m_rateMultiplier > 8)
		{
			//Just run the the limiter
			smart_limiter(size, data);
			return true;
		}

		if (size > m_buffer_len)
		{
			if (m_buffer != NULL) delete[]m_buffer;
			m_buffer = new audio_sample[size];
			m_buffer_len = size;
		}
		memcpy(m_buffer, data, size * sizeof(audio_sample));
		
		const int numPoints = sampleCnt * m_rateMultiplier;
		const int numPointsTimesChannels = numPoints * chCount;
		chunk->grow_data_size(numPointsTimesChannels);
		chunk->set_silence(numPoints);
		data = chunk->get_data();


		for (unsigned int iTarget = 0, iSource = 0; iSource < sampleCnt * chCount; iTarget += m_rateMultiplier * chCount, iSource += chCount )
		{
			for(unsigned int j = 0; j < chCount; j ++)
			{
				data[iTarget + j] = (audio_sample)(m_buffer[iSource + j] * m_multiplier);
			}
		}

		if (m_filter) m_filter->processInterleavedSamples(numPoints, chunk->get_data());

		smart_limiter(numPointsTimesChannels, data);

		chunk->set_sample_rate(sr * m_rateMultiplier);

		return true; //Return true to keep the chunk or false to drop it from the chain.
	}

	void on_endofplayback(abort_callback &) {
		// The end of playlist has been reached, we've already received the last decoded audio chunk.
		// We need to finish any pending processing and output any buffered data through insert_chunk().
	}
	void on_endoftrack(abort_callback &) {
		// Should do nothing except for special cases where your DSP performs special operations when changing tracks.
		// If this function does anything, you must change need_track_change_mark() to return true.
		// If you have pending audio data that you wish to output, you can use insert_chunk() to do so.		
	}

	void flush() {
		if (m_filter) m_filter->reset();
		// If you have any audio data buffered, you should drop it immediately and reset the DSP to a freshly initialized state.
		// Called after a seek etc.
	}

	double get_latency() {
		// If the DSP buffers some amount of audio data, it should return the duration of buffered data (in seconds) here.
		return 0;
	}

	bool need_track_change_mark() {
		// Return true if you need on_endoftrack() or need to accurately know which track we're currently processing
		// WARNING: If you return true, the DSP manager will fire on_endofplayback() at DSPs that are before us in the chain on track change to ensure that we get an accurate mark, so use it only when needed.
		return false;
	}
	static bool g_get_default_preset(dsp_preset & p_out)
	{
		Config c;
		p_out.set_owner(::g_get_guid());
		p_out.set_data(&c, sizeof(c));
		return true;
	}
	static void g_show_config_popup(const dsp_preset & p_data, HWND p_parent, dsp_preset_edit_callback & p_callback) {
		::RunDSPConfigPopup(p_data, p_parent, p_callback);
	}
	static bool g_have_config_popup() { return true; }
private:
	dsp_config m_filterCfg;
	Dsp::Filter * m_filter;
	int m_sampleRate;
	int m_rateMultiplier;
	int m_channelCount;
	double m_multiplier;

	void smart_limiter(int numSamplesTimesChannels, audio_sample* data)
	{
		for (int i = 0; i < numSamplesTimesChannels; i++)
		{
			audio_sample d = *(data);
			d *= m_multiplier;

			if (d > 1) m_multiplier /= d;
			if (d < -1) m_multiplier /= -d;

			*(data++) = d;
		}
	}
};


// Use dsp_factory_nopreset_t<> instead of dsp_factory_t<> if your DSP does not provide preset/configuration functionality.
static dsp_factory_t<dsp_upsampler> g_dsp_sample_factory;



static void RunDSPConfigPopup(const dsp_preset & p_data,HWND p_parent,dsp_preset_edit_callback & p_callback) {
	
	{
		dsp_config cfg;
		cfg.parse(p_data);
		MainPanel mp(&cfg.Data);
		int result = DialogWindow::showModalDialog(TRANS("Upsampler Configuration"), &mp, NULL, Colours::lightgrey, false, true);
		if (result == 1)
		{
			cfg.setPreset(p_callback);
		}
	}
	//MainPanel should be deleted defore shutdown
	shutdownJuce_GUI();
	
}