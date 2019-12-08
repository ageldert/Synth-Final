// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.h
//
/**
    \file   plugincore.h
    \author Will Pirkle
    \date   17-September-2018
    \brief  base class interface file for ASPiK plugincore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------

/*
HOMEWORK FOR THURSDAY:

ROTOR Object: 2 LFOs with cos and sin, adjustable amps and freqs; puts out X and Y values
Inheriting from ISynthModulator - basically all functions are abstract
Look at SynthLFO as an example

How about we push a note and it plays all 32 wavetables in a bank per a wavesequencer
- plays them, cross fading and holding through them, 0->31 (and reverse back to 0?)

things for joystick loc: 
- sin/cos good to start with
- ramp waves, any type of signal...
- really LPFed noise??

Korg Wavesequencing:
"get the incense out, get the masseuse out..."
"Joe is a cheap bastard!"

*/

#ifndef __pluginCore_h__
#define __pluginCore_h__

#include "pluginbase.h"
#include "synthcore.h"

// **--0x7F1F--**

// --- Plugin Variables controlID Enumeration 

enum controlID {
	masterPitchBend = 9,
	masterTune = 19,
	masterVolume_dB = 29,
	lfo1Waveform = 40,
	lfo1Mode = 41,
	lfo1Frequency_Hz = 42,
	lfo1DelayTime_mSec = 43,
	lfo1RampTime_mSec = 44,
	lfo2Waveform = 50,
	lfo2Mode = 51,
	lfo2Frequency_Hz = 52,
	lfo2Amp = 53,
	lfo2_shape = 54,
	lfo2_shapeY = 55,
	lfo1_pitchMode = 45,
	osc1_waveForm = 8,
	osc1_BankIndex = 7,
	osc2_waveForm = 18,
	osc2_BankIndex = 17,
	osc3_BankIndex = 27,
	osc4_BankIndex = 37,
	osc3_waveForm = 28,
	osc4_waveForm = 38,
	osc1_detune = 6,
	osc2_detune = 16,
	rotorShape = 58,
	rotorMode = 57,
	rotorFreq = 59,
	eg1AttackTime_mSec = 62,
	eg1DecayTime_mSec = 64,
	eg1SustainLevel = 65,
	eg1ReleaseTime_mSec = 66,
	eg1DelayTime_mSec = 61,
	eg1HoldTime_mSec = 63,
	eg1Mode = 60,
	eg1_retrigger = 67,
	eg1_attackFromZero = 68,
	filter1_fc = 20,
	filter1_q = 21,
	eg2AttackTime_mSec = 72,
	eg2DecayTime_mSec = 74,
	eg2SustainLevel = 75,
	eg2ReleaseTime_mSec = 76,
	eg1_sourceInt = 90,
	eg2_sourceInt = 100,
	lfo1_sourceInt = 110,
	lfo2_sourceInt = 120,
	osc1_fo_destInt = 81,
	osc2_fo_destInt = 82,
	filt1_fo_destInt = 83,
	lfo1_X_destInt = 84,
	r1c1 = 91,
	r1c2 = 92,
	r1c3 = 93,
	r1c4 = 94,
	r2c1 = 101,
	r2c2 = 102,
	r2c3 = 103,
	r2c4 = 104,
	r3c1 = 111,
	r3c2 = 112,
	r3c3 = 113,
	r3c4 = 114,
	r4c1 = 121,
	r4c2 = 122,
	r4c3 = 123,
	r4c4 = 124,
	filter1_enableKeyTrack = 10,
	filter1_keyTrackRatio = 11,
	osc3_detune = 26,
	osc4_detune = 36,
	synthMode = 0,
	unisonDetune_cents = 1
};

	// **--0x0F1F--**

/**
\class PluginCore
\ingroup ASPiK-Core
\brief
The PluginCore object is the default PluginBase derived object for ASPiK projects.
Note that you are fre to change the name of this object (as long as you change it in the compiler settings, etc...)


PluginCore Operations:
- overrides the main processing functions from the base class
- performs reset operation on sub-modules
- processes audio
- processes messages for custom views
- performs pre and post processing functions on parameters and audio (if needed)

\author Will Pirkle http://www.willpirkle.com
\remark This object is included in Designing Audio Effects Plugins in C++ 2nd Ed. by Will Pirkle
\version Revision : 1.0
\date Date : 2018 / 09 / 7
*/
class PluginCore : public PluginBase
{
public:
    PluginCore();

	/** Destructor: empty in default version */
    virtual ~PluginCore(){}

	// --- PluginBase Overrides ---
	//
	/** this is the creation function for all plugin parameters */
	bool initPluginParameters();

	/** called when plugin is loaded, a new audio file is playing or sample rate changes */
	virtual bool reset(ResetInfo& resetInfo);

	/** one-time post creation init function; pluginInfo contains path to this plugin */
	virtual bool initialize(PluginInfo& _pluginInfo);

	// --- preProcess: sync GUI parameters here; override if you don't want to use automatic variable-binding
	virtual bool preProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** process frames of data */
	virtual bool processAudioFrame(ProcessFrameInfo& processFrameInfo);

	// --- uncomment and override this for buffer processing; see base class implementation for
	//     help on breaking up buffers and getting info from processBufferInfo
	//virtual bool processAudioBuffers(ProcessBufferInfo& processBufferInfo);

	/** preProcess: do any post-buffer processing required; default operation is to send metering data to GUI  */
	virtual bool postProcessAudioBuffers(ProcessBufferInfo& processInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** called by host plugin at top of buffer proccess; this alters parameters prior to variable binding operation  */
	virtual bool updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo);

	/** this can be called: 1) after bound variable has been updated or 2) after smoothing occurs  */
	virtual bool postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo);

	/** this is ony called when the user makes a GUI control change */
	virtual bool guiParameterChanged(int32_t controlID, double actualValue);

	/** processMessage: messaging system; currently used for custom/special GUI operations */
	virtual bool processMessage(MessageInfo& messageInfo);

	/** processMIDIEvent: MIDI event processing */
	virtual bool processMIDIEvent(midiEvent& event);

	/** specialized joystick servicing (currently not used) */
	virtual bool setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData);

	/** create the presets */
	bool initPluginPresets();

	// --- BEGIN USER VARIABLES AND FUNCTIONS -------------------------------------- //
	//	   Add your variables and methods here
	SynthEngine synthEngine;

	void updateParameters();

	midiEvent lastEvent;
	
	// --- for custom GUI
	ICustomView* bankAndWaveGroup_0 = nullptr;

	// --- END USER VARIABLES AND FUNCTIONS -------------------------------------- //

private:
	//  **--0x07FD--**

	// --- Continuous Plugin Variables 
	double masterPitchBend = 0.0;
	double masterTune = 0.0;
	double masterVolume_dB = 0.0;
	double lfo1Frequency_Hz = 0.0;
	double lfo1DelayTime_mSec = 0.0;
	double lfo1RampTime_mSec = 0.0;
	double lfo2Frequency_Hz = 0.0;
	double lfo2Amp = 0.0;
	double lfo2_shape = 0.0;
	double lfo2_shapeY = 0.0;
	double osc1_detune = 0.0;
	double osc2_detune = 0.0;
	double rotorShape = 0.0;
	double rotorFreq = 0.0;
	double eg1AttackTime_mSec = 0.0;
	double eg1DecayTime_mSec = 0.0;
	double eg1SustainLevel = 0.0;
	double eg1ReleaseTime_mSec = 0.0;
	double eg1DelayTime_mSec = 0.0;
	double eg1HoldTime_mSec = 0.0;
	double filter1_fc = 0.0;
	double filter1_q = 0.0;
	double eg2AttackTime_mSec = 0.0;
	double eg2DecayTime_mSec = 0.0;
	double eg2SustainLevel = 0.0;
	double eg2ReleaseTime_mSec = 0.0;
	double eg1_sourceInt = 0.0;
	double eg2_sourceInt = 0.0;
	double lfo1_sourceInt = 0.0;
	double lfo2_sourceInt = 0.0;
	double osc1_fo_destInt = 0.0;
	double osc2_fo_destInt = 0.0;
	double filt1_fo_destInt = 0.0;
	double lfo1_X_destInt = 0.0;
	double filter1_keyTrackRatio = 0.0;
	double osc3_detune = 0.0;
	double osc4_detune = 0.0;
	double unisonDetune_cents = 0.0;

	// --- Discrete Plugin Variables 
	int lfo1Waveform = 0;
	enum class lfo1WaveformEnum { Triangle,Sin,Saw,RSH,QRSH,Noise,QRNoise };	// to compare: if(compareEnumToInt(lfo1WaveformEnum::Triangle, lfo1Waveform)) etc... 

	int lfo1Mode = 0;
	enum class lfo1ModeEnum { Sync,One_Shot,Free_Run };	// to compare: if(compareEnumToInt(lfo1ModeEnum::Sync, lfo1Mode)) etc... 

	int lfo2Waveform = 0;
	enum class lfo2WaveformEnum { Triangle,Sin,Saw,RSH,QRSH,Noise,QRNoise };	// to compare: if(compareEnumToInt(lfo2WaveformEnum::Triangle, lfo2Waveform)) etc... 

	int lfo2Mode = 0;
	enum class lfo2ModeEnum { Sync,One_Shot,Free_Run };	// to compare: if(compareEnumToInt(lfo2ModeEnum::Sync, lfo2Mode)) etc... 

	int lfo1_pitchMode = 0;
	enum class lfo1_pitchModeEnum { None,Ionian,Dorian,Phrygian,Lydian,Mixolydian,Aeolian,Locrian };	// to compare: if(compareEnumToInt(lfo1_pitchModeEnum::None, lfo1_pitchMode)) etc... 

	int osc1_waveForm = 0;
	enum class osc1_waveFormEnum { wv0,wv1,wv2,wv3,wv4,wv5,wv6,wv7,wv8,wv9,wv10,wv11,wv12,wv13,wv14,wv15,wv16,wv17,wv18,wv19,wv20,wv21,wv22,wv23,wv24,wv25,wv26,wv27,wv28,wv29,wv30,wv31 };	// to compare: if(compareEnumToInt(osc1_waveFormEnum::wv0, osc1_waveForm)) etc... 

	int osc1_BankIndex = 0;
	enum class osc1_BankIndexEnum { wv_bank0,wv_bank1,wv_bank2,wv_bank3 };	// to compare: if(compareEnumToInt(osc1_BankIndexEnum::wv_bank0, osc1_BankIndex)) etc... 

	int osc2_waveForm = 0;
	enum class osc2_waveFormEnum { wv0,wv1,wv2,wv3,wv4,wv5,wv6,wv7,wv8,wv9,wv10,wv11,wv12,wv13,wv14,wv15,wv16,wv17,wv18,wv19,wv20,wv21,wv22,wv23,wv24,wv25,wv26,wv27,wv28,wv29,wv30,wv31 };	// to compare: if(compareEnumToInt(osc2_waveFormEnum::wv0, osc2_waveForm)) etc... 

	int osc2_BankIndex = 0;
	enum class osc2_BankIndexEnum { wv_bank0,wv_bank1,wv_bank2,wv_bank3 };	// to compare: if(compareEnumToInt(osc2_BankIndexEnum::wv_bank0, osc2_BankIndex)) etc... 

	int osc3_BankIndex = 0;
	enum class osc3_BankIndexEnum { wv_bank0,wv_bank1,wv_bank2,wv_bank3 };	// to compare: if(compareEnumToInt(osc3_BankIndexEnum::wv_bank0, osc3_BankIndex)) etc... 

	int osc4_BankIndex = 0;
	enum class osc4_BankIndexEnum { wv_bank0,wv_bank1,wv_bank2,wv_bank3 };	// to compare: if(compareEnumToInt(osc4_BankIndexEnum::wv_bank0, osc4_BankIndex)) etc... 

	int osc3_waveForm = 0;
	enum class osc3_waveFormEnum { wv0,wv1,wv2,wv3,wv4,wv5,wv6,wv7,wv8,wv9,wv10,wv11,wv12,wv13,wv14,wv15,wv16,wv17,wv18,wv19,wv20,wv21,wv22,wv23,wv24,wv25,wv26,wv27,wv28,wv29,wv30,wv31 };	// to compare: if(compareEnumToInt(osc3_waveFormEnum::wv0, osc3_waveForm)) etc... 

	int osc4_waveForm = 0;
	enum class osc4_waveFormEnum { wv0,wv1,wv2,wv3,wv4,wv5,wv6,wv7,wv8,wv9,wv10,wv11,wv12,wv13,wv14,wv15,wv16,wv17,wv18,wv19,wv20,wv21,wv22,wv23,wv24,wv25,wv26,wv27,wv28,wv29,wv30,wv31 };	// to compare: if(compareEnumToInt(osc4_waveFormEnum::wv0, osc4_waveForm)) etc... 

	int rotorMode = 0;
	enum class rotorModeEnum { Disabled,Elliptical,Noise };	// to compare: if(compareEnumToInt(rotorModeEnum::Disabled, rotorMode)) etc... 

	int eg1Mode = 0;
	enum class eg1ModeEnum { ADSR,AHDSR,AHR,AHR_RT };	// to compare: if(compareEnumToInt(eg1ModeEnum::ADSR, eg1Mode)) etc... 

	int eg1_retrigger = 0;
	enum class eg1_retriggerEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(eg1_retriggerEnum::SWITCH_OFF, eg1_retrigger)) etc... 

	int eg1_attackFromZero = 0;
	enum class eg1_attackFromZeroEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(eg1_attackFromZeroEnum::SWITCH_OFF, eg1_attackFromZero)) etc... 

	int r1c1 = 0;
	enum class r1c1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r1c1Enum::SWITCH_OFF, r1c1)) etc... 

	int r1c2 = 0;
	enum class r1c2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r1c2Enum::SWITCH_OFF, r1c2)) etc... 

	int r1c3 = 0;
	enum class r1c3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r1c3Enum::SWITCH_OFF, r1c3)) etc... 

	int r1c4 = 0;
	enum class r1c4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r1c4Enum::SWITCH_OFF, r1c4)) etc... 

	int r2c1 = 0;
	enum class r2c1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r2c1Enum::SWITCH_OFF, r2c1)) etc... 

	int r2c2 = 0;
	enum class r2c2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r2c2Enum::SWITCH_OFF, r2c2)) etc... 

	int r2c3 = 0;
	enum class r2c3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r2c3Enum::SWITCH_OFF, r2c3)) etc... 

	int r2c4 = 0;
	enum class r2c4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r2c4Enum::SWITCH_OFF, r2c4)) etc... 

	int r3c1 = 0;
	enum class r3c1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r3c1Enum::SWITCH_OFF, r3c1)) etc... 

	int r3c2 = 0;
	enum class r3c2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r3c2Enum::SWITCH_OFF, r3c2)) etc... 

	int r3c3 = 0;
	enum class r3c3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r3c3Enum::SWITCH_OFF, r3c3)) etc... 

	int r3c4 = 0;
	enum class r3c4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r3c4Enum::SWITCH_OFF, r3c4)) etc... 

	int r4c1 = 0;
	enum class r4c1Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r4c1Enum::SWITCH_OFF, r4c1)) etc... 

	int r4c2 = 0;
	enum class r4c2Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r4c2Enum::SWITCH_OFF, r4c2)) etc... 

	int r4c3 = 0;
	enum class r4c3Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r4c3Enum::SWITCH_OFF, r4c3)) etc... 

	int r4c4 = 0;
	enum class r4c4Enum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(r4c4Enum::SWITCH_OFF, r4c4)) etc... 

	int filter1_enableKeyTrack = 0;
	enum class filter1_enableKeyTrackEnum { SWITCH_OFF,SWITCH_ON };	// to compare: if(compareEnumToInt(filter1_enableKeyTrackEnum::SWITCH_OFF, filter1_enableKeyTrack)) etc... 

	int synthMode = 0;
	enum class synthModeEnum { Poly,Mono,Unison };	// to compare: if(compareEnumToInt(synthModeEnum::Poly, synthMode)) etc... 

	// **--0x1A7F--**
    // --- end member variables

public:
    /** static description: bundle folder name

	\return bundle folder name as a const char*
	*/
    static const char* getPluginBundleName();

    /** static description: name

	\return name as a const char*
	*/
    static const char* getPluginName();

	/** static description: short name

	\return short name as a const char*
	*/
	static const char* getShortPluginName();

	/** static description: vendor name

	\return vendor name as a const char*
	*/
	static const char* getVendorName();

	/** static description: URL

	\return URL as a const char*
	*/
	static const char* getVendorURL();

	/** static description: email

	\return email address as a const char*
	*/
	static const char* getVendorEmail();

	/** static description: Cocoa View Factory Name

	\return Cocoa View Factory Name as a const char*
	*/
	static const char* getAUCocoaViewFactoryName();

	/** static description: plugin type

	\return type (FX or Synth)
	*/
	static pluginType getPluginType();

	/** static description: VST3 GUID

	\return VST3 GUID as a const char*
	*/
	static const char* getVSTFUID();

	/** static description: 4-char code

	\return 4-char code as int
	*/
	static int32_t getFourCharCode();

	/** initalizer */
	bool initPluginDescriptors();

};


#endif /* defined(__pluginCore_h__) */
