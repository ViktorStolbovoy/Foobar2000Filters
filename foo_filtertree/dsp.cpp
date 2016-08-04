#include "stdafx.h"
#include "DspFilters/Dsp.h"
#include <FilterConfig.h>
#include <FilterControls.h>
#include <ConfigItem.h>
#include <MainComponent.h>
#include <ProcessorHelpers.h>
#include <Processor.h>
#include "stdafx.h"

static void RunDSPConfigPopup(const dsp_preset & p_data, HWND p_parent, dsp_preset_edit_callback & p_callback);

static GUID g_get_guid() {
	//This is FilterTree specific GUID. 
	static const GUID guid = { 0xb57033db, 0xd36d, 0x4115, { 0x95, 0x8f, 0xa3, 0xaa, 0xf1, 0x51, 0x5d, 0x1b } };
	return guid;
}

class dsp_config
{
public:
	std::vector<ConfigItemBase*> Items;
	std::vector<ConfigItemBase*> OrigItems;

	dsp_config()
	{
		
	}

	~dsp_config()
	{
		// Clear(); elements can be deleted by a treeview
	}

	void Clear()
	{
		for (size_t i = 0; i < Items.size(); i++)
		{
			auto i1 = Items[i];
			auto i2 = i < OrigItems.size() ? OrigItems[i] : nullptr;
			if (i2 && i1 != i2) delete i2;
			delete i1;
		}
		Items.clear();
		OrigItems.clear();
	}

	void parse(const dsp_preset &in)
	{
		for (size_t i = 0; i < Items.size(); i++) delete Items[i];
		Items.clear();
		t_size len = in.get_data_size();
		uint8 * data = (uint8*)in.get_data();
		ConfigItemBase::LoadConfig(data, len, Items);
		OrigItems = Items;
	}

	void trimInputChannels(uint32 inputChannelMask)
	{
		for (size_t i = 0; i < OrigItems.size(); i++)
		{
			auto inp = dynamic_cast<InputConfigItem *>(OrigItems[i]);
			if (inp && !inp->IsVirtual)
			{
				uint32 inpMask = inp->InputMask;
				inpMask &= inputChannelMask;
				if (inpMask != inp->InputMask)
				{
					inp = new InputConfigItem(inp);
					inp->InputMask = inpMask;
				}

				if (inp->InputMask == 0)
				{
					if (i == 0 && ConfigItemBase::GetChannelCount(inputChannelMask) == 1)
					{
						inp->InputMask = inputChannelMask; //Mono source
					}
					else
					{
						inp = nullptr; //If teher is no channels - remove that branch
					}
				}
				
				if (Items[i] != OrigItems[i]) delete Items[i];
				Items[i] = inp;
			}
		}
	}



	void setPreset(dsp_preset_edit_callback & p_callback)
	{
		dsp_preset_impl preset;
		preset.set_owner(::g_get_guid());

		size_t totalSize = 0;
		for (size_t i = 0; i < Items.size(); i++)
		{
			totalSize += Items[i]->GetSize();
		}

		uint8* data = new uint8[totalSize];
		uint8* d = data;
		for (size_t i = 0; i < Items.size(); i++)
		{
			Items[i]->WriteToBuffer(&d);
		}
		preset.set_data(data, totalSize);
		delete[] data;

		p_callback.on_preset_changed(preset);
	}


};

class dsp_filtertree : public dsp_impl_base
{
public:
	dsp_filtertree(dsp_preset const & in) : m_processor(nullptr), m_buffer(nullptr), m_bufferLen(0), m_sampleRate(0), m_channelMap(0)
	{
		m_processorCfg.parse(in);
	}

	virtual ~dsp_filtertree()
	{
		delete m_processor;
		delete[]m_buffer;
		m_processorCfg.Clear();
	}
	
	static GUID g_get_guid() {
		//This is FilterTree specific GUID. 
		return ::g_get_guid();
	}

	static void g_get_name(pfc::string_base & p_out) { p_out = "Filter Tree"; }

	

	bool on_chunk(audio_chunk * chunk, abort_callback &) {
		// Perform any operations on the chunk here.
		// The most simple DSPs can just alter the chunk in-place here and skip the following functions.

		//chunk->grow_data_size();
		const int sr = chunk->get_sample_rate();
		const unsigned int chMap = chunk->get_channel_config();
		const unsigned int chCount = chunk->get_channel_count();
		if (m_sampleRate != sr || m_channelMap != chMap || !m_processor)
		{
			if (m_processor) delete m_processor;

			m_processorCfg.trimInputChannels(chMap);
			
			m_channelMap = chMap;
			m_sampleRate = sr;
			m_processor = new Processor(m_processorCfg.Items);
			m_processor->Setup(sr, chMap);
			m_scale = 1.0;
			m_outMask = m_processor->FindOutputChannelsConfig(&m_outChCount);
		}

		int * iptr = new int[10];

		//unsigned int cconfig = chunk->get_channel_config(); - don't really care
		audio_sample* data = chunk->get_data();
		
		delete[] iptr;

		//const t_size origDataSize = chunk->get_data_size();	 //Buffer size in samples * channels

		const t_size sampleCnt = chunk->get_sample_count();
		const t_size numInpSamplesByChannels = sampleCnt * chCount;


		if (numInpSamplesByChannels > m_bufferLen)
		{
			if (m_buffer != NULL) delete[]m_buffer;
			m_buffer = new audio_sample[numInpSamplesByChannels];
			m_bufferLen = numInpSamplesByChannels;
		}

		m_bufferBeyond = m_buffer + numInpSamplesByChannels;
		
		memcpy(m_buffer, data, numInpSamplesByChannels * sizeof(audio_sample));
		
		const int numOutSamplesTimesChannels = sampleCnt * m_outChCount;
		chunk->set_channels(m_outChCount, m_outMask);
		chunk->grow_data_size(numOutSamplesTimesChannels);
		
		data = chunk->get_data(); //After resizing it might be different adderss

		float *in = m_buffer;
		float *out = data;
		while (in < m_bufferBeyond)
		{
			m_processor->SetInput(in);
			m_processor->GetOutput(out);
			in += chCount;
			out += m_outChCount;
		} 

		//"Smart" limiter -- use only in upsampler
		/*for (int i = 0; i < numOutSamplesTimesChannels; i++)
		{
			audio_sample d = *(data);
			d *= m_scale;

			if (d > 1) m_scale /= d;
			if (d < -1) m_scale /= -d;

			*(data++) = d;
		} */
		
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
		if (m_processor) m_processor->Reset();
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
		p_out.set_owner(::g_get_guid());
		//p_out.set_data(&c, sizeof(c));
		return true;
	}
	static void g_show_config_popup(const dsp_preset & p_data, HWND p_parent, dsp_preset_edit_callback & p_callback) {
		::RunDSPConfigPopup(p_data, p_parent, p_callback);
	}
	static bool g_have_config_popup() { return true; }
private:
	dsp_config m_processorCfg;
	Processor * m_processor;
	int m_sampleRate;
	int m_channelMap;
	double m_scale;
	uint8 m_outChCount;
	uint32 m_outMask;
	audio_sample * m_buffer;
	t_size m_bufferLen;
	float *m_bufferBeyond;
};


// Use dsp_factory_nopreset_t<> instead of dsp_factory_t<> if your DSP does not provide preset/configuration functionality.
static dsp_factory_t<dsp_filtertree> g_dsp_sample_factory;



static void RunDSPConfigPopup(const dsp_preset & p_data,HWND p_parent,dsp_preset_edit_callback & p_callback) {
	
	{
		dsp_config cfg;
		cfg.parse(p_data);
		MainContentComponent mp;
		auto libraryInitialiser = new ScopedJuceInitialiser_GUI();
		mp.LoadConfig(cfg.Items);
		cfg.Clear();
		int result = DialogWindow::showModalDialog(TRANS("Filters Configuration"), &mp, NULL, Colours::lightgrey, false, true);
		if (result == 1)
		{
			mp.ExportConfig(cfg.Items);
			cfg.setPreset(p_callback);
		}
		delete libraryInitialiser;
	}
	//MainPanel should be deleted defore shutdown
	shutdownJuce_GUI();
	
}