// -----------------------------------------------------------------------------
//    ASPiK Plugin Kernel File:  plugincore.cpp
//
/**
    \file   plugincore.cpp
    \author Will Pirkle
    \date   17-September-2018
    \brief  Implementation file for PluginCore object
    		- http://www.aspikplugins.com
    		- http://www.willpirkle.com
*/
// -----------------------------------------------------------------------------
#include "plugincore.h"
#include "plugindescription.h"
#include "customviews.h"
#include "bankwaveviews.h"

/**
\brief PluginCore constructor is launching pad for object initialization

Operations:
- initialize the plugin description (strings, codes, numbers, see initPluginDescriptors())
- setup the plugin's audio I/O channel support
- create the PluginParameter objects that represent the plugin parameters (see FX book if needed)
- create the presets
*/
PluginCore::PluginCore()
{
    // --- describe the plugin; call the helper to init the static parts you setup in plugindescription.h
    initPluginDescriptors();

	//enum class egType { kADSR, kAHDSR, kAHR };

    // --- default I/O combinations
	// --- for FX plugins
	if (getPluginType() == kFXPlugin)
	{
		addSupportedIOCombination({ kCFMono, kCFMono });
		addSupportedIOCombination({ kCFMono, kCFStereo });
		addSupportedIOCombination({ kCFStereo, kCFStereo });
	}
	else // --- synth plugins have no input, only output
	{
		addSupportedIOCombination({ kCFNone, kCFMono });
		addSupportedIOCombination({ kCFNone, kCFStereo });
	}

	// --- for sidechaining, we support mono and stereo inputs; auxOutputs reserved for future use
	addSupportedAuxIOCombination({ kCFMono, kCFNone });
	addSupportedAuxIOCombination({ kCFStereo, kCFNone });

	// --- create the parameters
    initPluginParameters();

    // --- create the presets
    initPluginPresets();
}

/**
\brief create all of your plugin parameters here

\return true if parameters were created, false if they already existed
*/
bool PluginCore::initPluginParameters()
{
	if (pluginParameterMap.size() > 0)
		return false;

    // --- Add your plugin parameter instantiation code bewtween these hex codes
	// **--0xDEA7--**


	// --- Declaration of Plugin Parameter Objects 
	PluginParameter* piParam = nullptr;

	// --- continuous control: PB Range
	piParam = new PluginParameter(controlID::masterPitchBend, "PB Range", "semi", controlVariableType::kDouble, 1.000000, 24.000000, 7.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&masterPitchBend, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Master Tune
	piParam = new PluginParameter(controlID::masterTune, "Master Tune", "semi", controlVariableType::kDouble, -12.000000, 12.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&masterTune, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Master Volume
	piParam = new PluginParameter(controlID::masterVolume_dB, "Master Volume", "dB", controlVariableType::kDouble, -60.000000, 12.000000, -3.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&masterVolume_dB, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: LFO1 Wave
	piParam = new PluginParameter(controlID::lfo1Waveform, "LFO1 Wave", "Triangle,Sin,Saw,RSH,QRSH,Noise,QRNoise", "Triangle");
	piParam->setBoundVariable(&lfo1Waveform, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO1 Mode
	piParam = new PluginParameter(controlID::lfo1Mode, "LFO1 Mode", "Sync,One Shot,Free Run", "Sync");
	piParam->setBoundVariable(&lfo1Mode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 Fo
	piParam = new PluginParameter(controlID::lfo1Frequency_Hz, "LFO1 Fo", "Hz", controlVariableType::kDouble, 0.020000, 20.000000, 0.500000, taper::kVoltOctaveTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1Frequency_Hz, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 Dly
	piParam = new PluginParameter(controlID::lfo1DelayTime_mSec, "LFO1 Dly", "mSec", controlVariableType::kDouble, 0.000000, 2000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1DelayTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 Ramp
	piParam = new PluginParameter(controlID::lfo1RampTime_mSec, "LFO1 Ramp", "mSec", controlVariableType::kDouble, 0.000000, 2000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1RampTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: LFO2 Wave
	piParam = new PluginParameter(controlID::lfo2Waveform, "LFO2 Wave", "Triangle,Sin,Saw,RSH,QRSH,Noise,QRNoise", "Triangle");
	piParam->setBoundVariable(&lfo2Waveform, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO2 Mode
	piParam = new PluginParameter(controlID::lfo2Mode, "LFO2 Mode", "Sync,One Shot,Free Run", "Sync");
	piParam->setBoundVariable(&lfo2Mode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: LFO2 Fo
	piParam = new PluginParameter(controlID::lfo2Frequency_Hz, "LFO2 Fo", "Hz", controlVariableType::kDouble, 0.020000, 20.000000, 0.500000, taper::kVoltOctaveTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo2Frequency_Hz, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO2 Amp
	piParam = new PluginParameter(controlID::lfo2Amp, "LFO2 Amp", "%", controlVariableType::kDouble, 0.000000, 100.000000, 100.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo2Amp, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO2 Shape1
	piParam = new PluginParameter(controlID::lfo2_shape, "LFO2 Shape1", "", controlVariableType::kDouble, 0.200000, 0.800000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo2_shape, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO2 Shape2
	piParam = new PluginParameter(controlID::lfo2_shapeY, "LFO2 Shape2", "", controlVariableType::kDouble, 0.200000, 0.800000, 0.500000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo2_shapeY, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Pitch Mode
	piParam = new PluginParameter(controlID::lfo1_pitchMode, "Pitch Mode", "None,Ionian,Dorian,Phrygian,Lydian,Mixolydian,Aeolian,Locrian", "None");
	piParam->setBoundVariable(&lfo1_pitchMode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc1 Wave
	piParam = new PluginParameter(controlID::osc1_waveForm, "Osc1 Wave", "wv0,wv1,wv2,wv3,wv4,wv5,wv6,wv7,wv8,wv9,wv10,wv11,wv12,wv13,wv14,wv15,wv16,wv17,wv18,wv19,wv20,wv21,wv22,wv23,wv24,wv25,wv26,wv27,wv28,wv29,wv30,wv31", "wv0");
	piParam->setBoundVariable(&osc1_waveForm, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc1 Bank
	piParam = new PluginParameter(controlID::osc1_BankIndex, "Osc1 Bank", "wv bank0,wv bank1,wv bank2,wv bank3", "wv bank0");
	piParam->setBoundVariable(&osc1_BankIndex, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc2 Wave
	piParam = new PluginParameter(controlID::osc2_waveForm, "Osc2 Wave", "wv0,wv1,wv2,wv3,wv4,wv5,wv6,wv7,wv8,wv9,wv10,wv11,wv12,wv13,wv14,wv15,wv16,wv17,wv18,wv19,wv20,wv21,wv22,wv23,wv24,wv25,wv26,wv27,wv28,wv29,wv30,wv31", "wv1");
	piParam->setBoundVariable(&osc2_waveForm, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc2 Bank
	piParam = new PluginParameter(controlID::osc2_BankIndex, "Osc2 Bank", "wv bank0,wv bank1,wv bank2,wv bank3", "wv bank1");
	piParam->setBoundVariable(&osc2_BankIndex, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc3 Bank
	piParam = new PluginParameter(controlID::osc3_BankIndex, "Osc3 Bank", "wv bank0,wv bank1,wv bank2,wv bank3", "wv bank0");
	piParam->setBoundVariable(&osc3_BankIndex, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc4 Bank
	piParam = new PluginParameter(controlID::osc4_BankIndex, "Osc4 Bank", "wv bank0,wv bank1,wv bank2,wv bank3", "wv bank1");
	piParam->setBoundVariable(&osc4_BankIndex, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc3 Wave
	piParam = new PluginParameter(controlID::osc3_waveForm, "Osc3 Wave", "wv0,wv1,wv2,wv3,wv4,wv5,wv6,wv7,wv8,wv9,wv10,wv11,wv12,wv13,wv14,wv15,wv16,wv17,wv18,wv19,wv20,wv21,wv22,wv23,wv24,wv25,wv26,wv27,wv28,wv29,wv30,wv31", "wv2");
	piParam->setBoundVariable(&osc3_waveForm, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Osc4 Wave
	piParam = new PluginParameter(controlID::osc4_waveForm, "Osc4 Wave", "wv0,wv1,wv2,wv3,wv4,wv5,wv6,wv7,wv8,wv9,wv10,wv11,wv12,wv13,wv14,wv15,wv16,wv17,wv18,wv19,wv20,wv21,wv22,wv23,wv24,wv25,wv26,wv27,wv28,wv29,wv30,wv31", "wv3");
	piParam->setBoundVariable(&osc4_waveForm, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Detune Osc1
	piParam = new PluginParameter(controlID::osc1_detune, "Detune Osc1", "cents", controlVariableType::kDouble, -100.000000, 100.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&osc1_detune, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Detune Osc2
	piParam = new PluginParameter(controlID::osc2_detune, "Detune Osc2", "cents", controlVariableType::kDouble, -100.000000, 100.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&osc2_detune, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Rotor Shape
	piParam = new PluginParameter(controlID::rotorShape, "Rotor Shape", "", controlVariableType::kDouble, 0.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&rotorShape, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Rotor Mode
	piParam = new PluginParameter(controlID::rotorMode, "Rotor Mode", "Disabled,Elliptical,Noise", "Disabled");
	piParam->setBoundVariable(&rotorMode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Rotor Freq
	piParam = new PluginParameter(controlID::rotorFreq, "Rotor Freq", "hZ", controlVariableType::kDouble, 0.010000, 10.000000, 0.010000, taper::kVoltOctaveTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&rotorFreq, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Attack
	piParam = new PluginParameter(controlID::eg1AttackTime_mSec, "EG1 Attack", "ms", controlVariableType::kDouble, 0.000000, 1000.000000, 5.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg1AttackTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Decay
	piParam = new PluginParameter(controlID::eg1DecayTime_mSec, "EG1 Decay", "ms", controlVariableType::kDouble, 0.000000, 5000.000000, 100.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg1DecayTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Sustain
	piParam = new PluginParameter(controlID::eg1SustainLevel, "EG1 Sustain", "", controlVariableType::kDouble, 0.000000, 1.000000, 0.707000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg1SustainLevel, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Release
	piParam = new PluginParameter(controlID::eg1ReleaseTime_mSec, "EG1 Release", "ms", controlVariableType::kDouble, 0.000000, 10000.000000, 500.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg1ReleaseTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Delay
	piParam = new PluginParameter(controlID::eg1DelayTime_mSec, "EG1 Delay", "ms", controlVariableType::kDouble, 0.000000, 1000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg1DelayTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Hold
	piParam = new PluginParameter(controlID::eg1HoldTime_mSec, "EG1 Hold", "ms", controlVariableType::kDouble, 0.000000, 3000.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg1HoldTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: EG1 Mode
	piParam = new PluginParameter(controlID::eg1Mode, "EG1 Mode", "ADSR,AHDSR,AHR,AHR RT", "ADSR");
	piParam->setBoundVariable(&eg1Mode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG1 Retrigger
	piParam = new PluginParameter(controlID::eg1_retrigger, "EG1 Retrigger", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&eg1_retrigger, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG1 Attack Zero
	piParam = new PluginParameter(controlID::eg1_attackFromZero, "EG1 Attack Zero", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&eg1_attackFromZero, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Filter 1 Fc
	piParam = new PluginParameter(controlID::filter1_fc, "Filter 1 Fc", "Hz", controlVariableType::kDouble, 20.000000, 18000.000000, 880.000000, taper::kVoltOctaveTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&filter1_fc, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Filter 1 Q
	piParam = new PluginParameter(controlID::filter1_q, "Filter 1 Q", "", controlVariableType::kDouble, 1.000000, 10.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&filter1_q, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG2 Attack
	piParam = new PluginParameter(controlID::eg2AttackTime_mSec, "EG2 Attack", "ms", controlVariableType::kDouble, 0.000000, 1000.000000, 5.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg2AttackTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG2 Decay
	piParam = new PluginParameter(controlID::eg2DecayTime_mSec, "EG2 Decay", "ms", controlVariableType::kDouble, 0.000000, 5000.000000, 100.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg2DecayTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG2 Sustain
	piParam = new PluginParameter(controlID::eg2SustainLevel, "EG2 Sustain", "", controlVariableType::kDouble, 0.000000, 1.000000, 0.707000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg2SustainLevel, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG2 Release
	piParam = new PluginParameter(controlID::eg2ReleaseTime_mSec, "EG2 Release", "ms", controlVariableType::kDouble, 0.000000, 10000.000000, 500.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg2ReleaseTime_mSec, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG1 Int
	piParam = new PluginParameter(controlID::eg1_sourceInt, "EG1 Int", "", controlVariableType::kDouble, -1.000000, 1.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg1_sourceInt, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: EG2 Int
	piParam = new PluginParameter(controlID::eg2_sourceInt, "EG2 Int", "", controlVariableType::kDouble, -1.000000, 1.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&eg2_sourceInt, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 Int
	piParam = new PluginParameter(controlID::lfo1_sourceInt, "LFO1 Int", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1_sourceInt, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO2 Int
	piParam = new PluginParameter(controlID::lfo2_sourceInt, "LFO2 Int", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo2_sourceInt, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Osc1 fo Int
	piParam = new PluginParameter(controlID::osc1_fo_destInt, "Osc1 fo Int", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&osc1_fo_destInt, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Osc2 fo Int
	piParam = new PluginParameter(controlID::osc2_fo_destInt, "Osc2 fo Int", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&osc2_fo_destInt, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Filt1 fo Int
	piParam = new PluginParameter(controlID::filt1_fo_destInt, "Filt1 fo Int", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&filt1_fo_destInt, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: LFO1 ShpX Int
	piParam = new PluginParameter(controlID::lfo1_X_destInt, "LFO1 ShpX Int", "", controlVariableType::kDouble, -1.000000, 1.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&lfo1_X_destInt, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: EG1
	piParam = new PluginParameter(controlID::r1c1, "EG1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r1c1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG1
	piParam = new PluginParameter(controlID::r1c2, "EG1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r1c2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG1
	piParam = new PluginParameter(controlID::r1c3, "EG1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r1c3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG1
	piParam = new PluginParameter(controlID::r1c4, "EG1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r1c4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG2
	piParam = new PluginParameter(controlID::r2c1, "EG2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r2c1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG2
	piParam = new PluginParameter(controlID::r2c2, "EG2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r2c2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG2
	piParam = new PluginParameter(controlID::r2c3, "EG2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r2c3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: EG2
	piParam = new PluginParameter(controlID::r2c4, "EG2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r2c4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO1
	piParam = new PluginParameter(controlID::r3c1, "LFO1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r3c1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO1
	piParam = new PluginParameter(controlID::r3c2, "LFO1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r3c2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO1
	piParam = new PluginParameter(controlID::r3c3, "LFO1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r3c3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO1
	piParam = new PluginParameter(controlID::r3c4, "LFO1", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r3c4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO2
	piParam = new PluginParameter(controlID::r4c1, "LFO2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r4c1, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO2
	piParam = new PluginParameter(controlID::r4c2, "LFO2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r4c2, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO2
	piParam = new PluginParameter(controlID::r4c3, "LFO2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r4c3, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: LFO2
	piParam = new PluginParameter(controlID::r4c4, "LFO2", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&r4c4, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- discrete control: Filter Key Track
	piParam = new PluginParameter(controlID::filter1_enableKeyTrack, "Filter Key Track", "SWITCH OFF,SWITCH ON", "SWITCH OFF");
	piParam->setBoundVariable(&filter1_enableKeyTrack, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Key Ratio
	piParam = new PluginParameter(controlID::filter1_keyTrackRatio, "Key Ratio", "", controlVariableType::kDouble, 0.250000, 4.000000, 1.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&filter1_keyTrackRatio, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Detune Osc3
	piParam = new PluginParameter(controlID::osc3_detune, "Detune Osc3", "cents", controlVariableType::kDouble, -100.000000, 100.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&osc3_detune, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- continuous control: Detune Osc4
	piParam = new PluginParameter(controlID::osc4_detune, "Detune Osc4", "cents", controlVariableType::kDouble, -100.000000, 100.000000, 0.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&osc4_detune, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- discrete control: Synth Mode
	piParam = new PluginParameter(controlID::synthMode, "Synth Mode", "Poly,Mono,Unison", "Poly");
	piParam->setBoundVariable(&synthMode, boundVariableType::kInt);
	piParam->setIsDiscreteSwitch(true);
	addPluginParameter(piParam);

	// --- continuous control: Unison
	piParam = new PluginParameter(controlID::unisonDetune_cents, "Unison", "%", controlVariableType::kDouble, 0.000000, 100.000000, 20.000000, taper::kLinearTaper);
	piParam->setParameterSmoothing(false);
	piParam->setSmoothingTimeMsec(100.00);
	piParam->setBoundVariable(&unisonDetune_cents, boundVariableType::kDouble);
	addPluginParameter(piParam);

	// --- Aux Attributes
	AuxParameterAttribute auxAttribute;

	// --- RAFX GUI attributes
	// --- controlID::masterPitchBend
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::masterPitchBend, auxAttribute);

	// --- controlID::masterTune
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::masterTune, auxAttribute);

	// --- controlID::masterVolume_dB
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::masterVolume_dB, auxAttribute);

	// --- controlID::lfo1Waveform
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::lfo1Waveform, auxAttribute);

	// --- controlID::lfo1Mode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::lfo1Mode, auxAttribute);

	// --- controlID::lfo1Frequency_Hz
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo1Frequency_Hz, auxAttribute);

	// --- controlID::lfo1DelayTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo1DelayTime_mSec, auxAttribute);

	// --- controlID::lfo1RampTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo1RampTime_mSec, auxAttribute);

	// --- controlID::lfo2Waveform
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::lfo2Waveform, auxAttribute);

	// --- controlID::lfo2Mode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::lfo2Mode, auxAttribute);

	// --- controlID::lfo2Frequency_Hz
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo2Frequency_Hz, auxAttribute);

	// --- controlID::lfo2Amp
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo2Amp, auxAttribute);

	// --- controlID::lfo2_shape
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo2_shape, auxAttribute);

	// --- controlID::lfo2_shapeY
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo2_shapeY, auxAttribute);

	// --- controlID::lfo1_pitchMode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::lfo1_pitchMode, auxAttribute);

	// --- controlID::osc1_waveForm
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc1_waveForm, auxAttribute);

	// --- controlID::osc1_BankIndex
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc1_BankIndex, auxAttribute);

	// --- controlID::osc2_waveForm
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc2_waveForm, auxAttribute);

	// --- controlID::osc2_BankIndex
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc2_BankIndex, auxAttribute);

	// --- controlID::osc3_BankIndex
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc3_BankIndex, auxAttribute);

	// --- controlID::osc4_BankIndex
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc4_BankIndex, auxAttribute);

	// --- controlID::osc3_waveForm
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc3_waveForm, auxAttribute);

	// --- controlID::osc4_waveForm
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::osc4_waveForm, auxAttribute);

	// --- controlID::osc1_detune
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483658);
	setParamAuxAttribute(controlID::osc1_detune, auxAttribute);

	// --- controlID::osc2_detune
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483658);
	setParamAuxAttribute(controlID::osc2_detune, auxAttribute);

	// --- controlID::rotorShape
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::rotorShape, auxAttribute);

	// --- controlID::rotorMode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::rotorMode, auxAttribute);

	// --- controlID::rotorFreq
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::rotorFreq, auxAttribute);

	// --- controlID::eg1AttackTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg1AttackTime_mSec, auxAttribute);

	// --- controlID::eg1DecayTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg1DecayTime_mSec, auxAttribute);

	// --- controlID::eg1SustainLevel
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg1SustainLevel, auxAttribute);

	// --- controlID::eg1ReleaseTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg1ReleaseTime_mSec, auxAttribute);

	// --- controlID::eg1DelayTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg1DelayTime_mSec, auxAttribute);

	// --- controlID::eg1HoldTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg1HoldTime_mSec, auxAttribute);

	// --- controlID::eg1Mode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::eg1Mode, auxAttribute);

	// --- controlID::eg1_retrigger
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1610612736);
	setParamAuxAttribute(controlID::eg1_retrigger, auxAttribute);

	// --- controlID::eg1_attackFromZero
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::eg1_attackFromZero, auxAttribute);

	// --- controlID::filter1_fc
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::filter1_fc, auxAttribute);

	// --- controlID::filter1_q
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::filter1_q, auxAttribute);

	// --- controlID::eg2AttackTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg2AttackTime_mSec, auxAttribute);

	// --- controlID::eg2DecayTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg2DecayTime_mSec, auxAttribute);

	// --- controlID::eg2SustainLevel
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg2SustainLevel, auxAttribute);

	// --- controlID::eg2ReleaseTime_mSec
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg2ReleaseTime_mSec, auxAttribute);

	// --- controlID::eg1_sourceInt
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg1_sourceInt, auxAttribute);

	// --- controlID::eg2_sourceInt
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::eg2_sourceInt, auxAttribute);

	// --- controlID::lfo1_sourceInt
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo1_sourceInt, auxAttribute);

	// --- controlID::lfo2_sourceInt
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo2_sourceInt, auxAttribute);

	// --- controlID::osc1_fo_destInt
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::osc1_fo_destInt, auxAttribute);

	// --- controlID::osc2_fo_destInt
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::osc2_fo_destInt, auxAttribute);

	// --- controlID::filt1_fo_destInt
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::filt1_fo_destInt, auxAttribute);

	// --- controlID::lfo1_X_destInt
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::lfo1_X_destInt, auxAttribute);

	// --- controlID::r1c1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r1c1, auxAttribute);

	// --- controlID::r1c2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r1c2, auxAttribute);

	// --- controlID::r1c3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r1c3, auxAttribute);

	// --- controlID::r1c4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r1c4, auxAttribute);

	// --- controlID::r2c1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r2c1, auxAttribute);

	// --- controlID::r2c2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r2c2, auxAttribute);

	// --- controlID::r2c3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r2c3, auxAttribute);

	// --- controlID::r2c4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r2c4, auxAttribute);

	// --- controlID::r3c1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r3c1, auxAttribute);

	// --- controlID::r3c2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r3c2, auxAttribute);

	// --- controlID::r3c3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r3c3, auxAttribute);

	// --- controlID::r3c4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r3c4, auxAttribute);

	// --- controlID::r4c1
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r4c1, auxAttribute);

	// --- controlID::r4c2
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r4c2, auxAttribute);

	// --- controlID::r4c3
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r4c3, auxAttribute);

	// --- controlID::r4c4
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741827);
	setParamAuxAttribute(controlID::r4c4, auxAttribute);

	// --- controlID::filter1_enableKeyTrack
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(1073741824);
	setParamAuxAttribute(controlID::filter1_enableKeyTrack, auxAttribute);

	// --- controlID::filter1_keyTrackRatio
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::filter1_keyTrackRatio, auxAttribute);

	// --- controlID::osc3_detune
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483658);
	setParamAuxAttribute(controlID::osc3_detune, auxAttribute);

	// --- controlID::osc4_detune
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483658);
	setParamAuxAttribute(controlID::osc4_detune, auxAttribute);

	// --- controlID::synthMode
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(805306368);
	setParamAuxAttribute(controlID::synthMode, auxAttribute);

	// --- controlID::unisonDetune_cents
	auxAttribute.reset(auxGUIIdentifier::guiControlData);
	auxAttribute.setUintAttribute(2147483648);
	setParamAuxAttribute(controlID::unisonDetune_cents, auxAttribute);


	// **--0xEDA5--**
   
    // --- BONUS Parameter
    // --- SCALE_GUI_SIZE
    PluginParameter* piParamBonus = new PluginParameter(SCALE_GUI_SIZE, "Scale GUI", "tiny,small,medium,normal,large,giant", "normal");
    addPluginParameter(piParamBonus);

	// --- create the super fast access array
	initPluginParameterArray();

    return true;
}

/**
\brief initialize object for a new run of audio; called just before audio streams

Operation:
- store sample rate and bit depth on audioProcDescriptor - this information is globally available to all core functions
- reset your member objects here

\param resetInfo structure of information about current audio format

\return true if operation succeeds, false otherwise
*/
bool PluginCore::reset(ResetInfo& resetInfo)
{
    // --- save for audio processing
    audioProcDescriptor.sampleRate = resetInfo.sampleRate;
    audioProcDescriptor.bitDepth = resetInfo.bitDepth;

    // --- other reset inits

	synthEngine.reset(resetInfo.sampleRate);

    return PluginBase::reset(resetInfo);
}

/**
\brief one-time initialize function called after object creation and before the first reset( ) call

Operation:
- saves structure for the plugin to use; you can also load WAV files or state information here
*/
bool PluginCore::initialize(PluginInfo& pluginInfo)
{
	// --- add one-time init stuff here

	return true;
}

/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- syncInBoundVariables when preProcessAudioBuffers is called, it is *guaranteed* that all GUI control change information
  has been applied to plugin parameters; this binds parameter changes to your underlying variables
- NOTE: postUpdatePluginParameter( ) will be called for all bound variables that are acutally updated; if you need to process
  them individually, do so in that function
- use this function to bulk-transfer the bound variable data into your plugin's member object variables

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::preProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
    // --- sync internal variables to GUI parameters; you can also do this manually if you don't
    //     want to use the auto-variable-binding
    syncInBoundVariables();

	// we only update parameters ONCE per buffer
	updateParameters();

    return true;
}

void PluginCore::updateParameters() 
{
	SynthEngineParameters engineParams = synthEngine.getParameters();

	// --- collect GUI control update values
	engineParams.masterPitchBendSensCoarse = (unsigned int)masterPitchBend; // --- this is pitch bend max range in semitones
	engineParams.masterPitchBendSensFine = (unsigned int)(100.0*(masterPitchBend - engineParams.masterPitchBendSensCoarse)); // this is pitch bend max range in semitones

	// --- create two tuning offsets from one master tune value
	engineParams.masterTuningCoarse = (int)masterTune;
	engineParams.masterTuningFine = (int)(100.0*(masterTune - engineParams.masterTuningCoarse)); // --- get fraction and convert to cents (1/100th of a semitone)

	// --- Mode
	engineParams.mode = convertIntToEnum(synthMode,SynthMode);
	engineParams.masterUnisonDetune_Cents = unisonDetune_cents;

	// --- Master Volume
	engineParams.masterVolume_dB = masterVolume_dB;

	// --- LFO 1 Parameters
	engineParams.voiceParameters->lfo1Parameters->frequency_Hz = lfo1Frequency_Hz;
	engineParams.voiceParameters->lfo1Parameters->waveform = convertIntToEnum(lfo1Waveform, LFOWaveform);
	engineParams.voiceParameters->lfo1Parameters->mode = convertIntToEnum(lfo1Mode, LFOMode);
	engineParams.voiceParameters->lfo1Parameters->delay_mSec = lfo1DelayTime_mSec;
	engineParams.voiceParameters->lfo1Parameters->rampTime_mSec = lfo1RampTime_mSec;

	// --- LFO 2 Parameters
	engineParams.voiceParameters->lfo2Parameters->frequency_Hz = lfo2Frequency_Hz;
	engineParams.voiceParameters->lfo2Parameters->waveform = convertIntToEnum(lfo2Waveform, LFOWaveform);
	engineParams.voiceParameters->lfo2Parameters->mode = convertIntToEnum(lfo2Mode, LFOMode);
	engineParams.voiceParameters->lfo2Parameters->outputAmplitude = lfo2Amp / 100.0;
	engineParams.voiceParameters->lfo2Parameters->shape = lfo2_shape;
	engineParams.voiceParameters->lfo2Parameters->shapey = lfo2_shapeY;

	// --- wavetable oscillator parameters
	engineParams.voiceParameters->osc1Parameters->oscillatorBankIndex = osc1_BankIndex;
	engineParams.voiceParameters->osc1Parameters->oscillatorWaveformIndex = osc1_waveForm;
	engineParams.voiceParameters->osc1Parameters->pitchMode = lfo1_pitchMode;
	engineParams.voiceParameters->osc1Parameters->detuneCents = osc1_detune;

	engineParams.voiceParameters->osc2Parameters->oscillatorBankIndex = osc2_BankIndex;
	engineParams.voiceParameters->osc2Parameters->oscillatorWaveformIndex = osc2_waveForm;
	engineParams.voiceParameters->osc2Parameters->pitchMode = lfo1_pitchMode;
	engineParams.voiceParameters->osc2Parameters->detuneCents = osc2_detune;

	engineParams.voiceParameters->osc3Parameters->oscillatorBankIndex = osc3_BankIndex;
	engineParams.voiceParameters->osc3Parameters->oscillatorWaveformIndex = osc3_waveForm;
	engineParams.voiceParameters->osc3Parameters->pitchMode = lfo1_pitchMode;
	engineParams.voiceParameters->osc3Parameters->detuneCents = osc3_detune;

	engineParams.voiceParameters->osc4Parameters->oscillatorBankIndex = osc4_BankIndex;
	engineParams.voiceParameters->osc4Parameters->oscillatorWaveformIndex = osc4_waveForm;
	engineParams.voiceParameters->osc4Parameters->pitchMode = lfo1_pitchMode;
	engineParams.voiceParameters->osc4Parameters->detuneCents = osc4_detune;

	// --- Rotor Parameters
	engineParams.voiceParameters->rotorParameters->mode = rotorMode;
	engineParams.voiceParameters->rotorParameters->shape = rotorShape;
	engineParams.voiceParameters->rotorParameters->freq = rotorFreq;
	engineParams.voiceParameters->rotorParameters->lfo1Parameters->frequency_Hz = rotorFreq;
	engineParams.voiceParameters->rotorParameters->lfo2Parameters->frequency_Hz = rotorFreq;

	// --- Amp EG Parameters
	engineParams.voiceParameters->ampEGParameters->delayTime_mSec = eg1DelayTime_mSec;
	engineParams.voiceParameters->ampEGParameters->attackTime_mSec = eg1AttackTime_mSec;
	engineParams.voiceParameters->ampEGParameters->holdTime_mSec = eg1HoldTime_mSec;
	engineParams.voiceParameters->ampEGParameters->decayTime_mSec = eg1DecayTime_mSec;
	engineParams.voiceParameters->ampEGParameters->sustainLevel = eg1SustainLevel;
	engineParams.voiceParameters->ampEGParameters->releaseTime_mSec = eg1ReleaseTime_mSec;
	engineParams.voiceParameters->ampEGParameters->egContourType = convertIntToEnum(eg1Mode, egType);
	engineParams.voiceParameters->ampEGParameters->resetToZero = eg1_attackFromZero;

	// --- EG2 Parameters
	engineParams.voiceParameters->EG2Parameters->attackTime_mSec = eg2AttackTime_mSec;
	engineParams.voiceParameters->EG2Parameters->decayTime_mSec = eg2DecayTime_mSec;
	engineParams.voiceParameters->EG2Parameters->sustainLevel = eg2SustainLevel;
	engineParams.voiceParameters->EG2Parameters->releaseTime_mSec = eg2ReleaseTime_mSec;

	// --- Filter Parameters
	engineParams.voiceParameters->moogFilterParameters->fc = filter1_fc;
	engineParams.voiceParameters->moogFilterParameters->Q = filter1_q;
	engineParams.voiceParameters->moogFilterParameters->enableKeyTrack = (filter1_enableKeyTrack == 1);
	engineParams.voiceParameters->moogFilterParameters->keyTrackRatio = filter1_keyTrackRatio;

	// --- setting MM source intensities
	engineParams.setMM_SourceMasterIntensity(kEG1_Normal, eg1_sourceInt);
	engineParams.setMM_SourceMasterIntensity(kEG2_Normal, eg2_sourceInt);
	engineParams.setMM_SourceMasterIntensity(kLFO1_Normal, lfo1_sourceInt);
	engineParams.setMM_SourceMasterIntensity(kLFO2_Normal, lfo2_sourceInt);

	// ---- MM switches
	engineParams.setMM_ChannelEnable(kEG1_Normal, kOsc1_fo, (r1c1 == 1));
	engineParams.setMM_ChannelEnable(kEG1_Normal, kOsc2_fo, (r1c2 == 1));
	engineParams.setMM_ChannelEnable(kEG1_Normal, kFilter1_fc, (r1c3 == 1));
	engineParams.setMM_ChannelEnable(kEG1_Normal, kShapeX, (r1c4 == 1));
	engineParams.setMM_ChannelEnable(kEG2_Normal, kOsc1_fo, (r2c1 == 1));
	engineParams.setMM_ChannelEnable(kEG2_Normal, kOsc2_fo, (r2c2 == 1));
	engineParams.setMM_ChannelEnable(kEG2_Normal, kFilter1_fc, (r2c3 == 1));
	engineParams.setMM_ChannelEnable(kEG2_Normal, kShapeX, (r2c4 == 1));
	engineParams.setMM_ChannelEnable(kLFO1_Normal, kOsc1_fo, (r3c1 == 1));
	engineParams.setMM_ChannelEnable(kLFO1_Normal, kOsc2_fo, (r3c2 == 1));
	engineParams.setMM_ChannelEnable(kLFO1_Normal, kFilter1_fc, (r3c3 == 1));
	engineParams.setMM_ChannelEnable(kLFO1_Normal, kShapeX, (r3c4 == 1));
	engineParams.setMM_ChannelEnable(kLFO2_Normal, kOsc1_fo, (r4c1 == 1));
	engineParams.setMM_ChannelEnable(kLFO2_Normal, kOsc2_fo, (r4c2 == 1));
	engineParams.setMM_ChannelEnable(kLFO2_Normal, kFilter1_fc, (r4c3 == 1));
	engineParams.setMM_ChannelEnable(kLFO2_Normal, kShapeX, (r4c4 == 1));

	// --- setting MM destination intensities
	engineParams.setMM_DestMasterIntensity(kOsc1_fo, osc1_fo_destInt);
	engineParams.setMM_DestMasterIntensity(kOsc2_fo, osc2_fo_destInt);
	engineParams.setMM_DestMasterIntensity(kFilter1_fc, filt1_fo_destInt);
	engineParams.setMM_DestMasterIntensity(kShapeX, lfo1_X_destInt);

   // --- THE update - this trickles all param updates
   //                  via the setParameters( ) of each  synthEngine.setParameters(engineParams); 

	synthEngine.setParameters(engineParams);
}

/**
\brief frame-processing method

Operation:
- decode the plugin type - for synth plugins, fill in the rendering code; for FX plugins, delete the if(synth) portion and add your processing code
- note that MIDI events are fired for each sample interval so that MIDI is tightly sunk with audio
- doSampleAccurateParameterUpdates will perform per-sample interval smoothing

\param processFrameInfo structure of information about *frame* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processAudioFrame(ProcessFrameInfo& processFrameInfo)
{
    // --- fire any MIDI events for this sample interval
    processFrameInfo.midiEventQueue->fireMidiEvents(processFrameInfo.currentFrame);
	if (eg1_retrigger) processMIDIEvent(lastEvent);

	// --- do per-frame updates; VST automation and parameter smoothing
	doSampleAccurateParameterUpdates();

	// processFrameInfo.hostInfo->dBPM // oooh something with this

    // --- decode the channelIOConfiguration and process accordingly
    //
	// --- Synth Plugin:
	// --- Synth Plugin --- remove for FX plugins
	if (getPluginType() == kSynthPlugin)
	{
		// --- do the synth render  
		const SynthRenderData render = synthEngine.renderAudioOutput(); 

		// --- write left channel  
		processFrameInfo.audioOutputFrame[0] = render.synthOutputs[0]; 

		// --- write right channel  
		if (processFrameInfo.channelIOConfig.outputChannelFormat ==  kCFStereo)   
			processFrameInfo.audioOutputFrame[1] =  render.synthOutputs[1]; 

		return true;	/// processed
	}

    // --- FX Plugin:
    if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFMono)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Mono-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFMono &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = processFrameInfo.audioInputFrame[0];

        return true; /// processed
    }

    // --- Stereo-In/Stereo-Out
    else if(processFrameInfo.channelIOConfig.inputChannelFormat == kCFStereo &&
       processFrameInfo.channelIOConfig.outputChannelFormat == kCFStereo)
    {
		// --- pass through code: change this with your signal processing
        processFrameInfo.audioOutputFrame[0] = processFrameInfo.audioInputFrame[0];
        processFrameInfo.audioOutputFrame[1] = processFrameInfo.audioInputFrame[1];

        return true; /// processed
    }

    return false; /// NOT processed
}


/**
\brief do anything needed prior to arrival of audio buffers

Operation:
- updateOutBoundVariables sends metering data to the GUI meters

\param processInfo structure of information about *buffer* processing

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postProcessAudioBuffers(ProcessBufferInfo& processInfo)
{
	// --- update outbound variables; currently this is meter data only, but could be extended
	//     in the future
	updateOutBoundVariables();

    return true;
}

/**
\brief update the PluginParameter's value based on GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- use base class helper
    setPIParamValue(controlID, controlValue);

    // --- do any post-processing
    postUpdatePluginParameter(controlID, controlValue, paramInfo);

    return true; /// handled
}

/**
\brief update the PluginParameter's value based on *normlaized* GUI control, preset, or data smoothing (thread-safe)

Operation:
- update the parameter's value (with smoothing this initiates another smoothing process)
- call postUpdatePluginParameter to do any further processing

\param controlID the control ID value of the parameter being updated
\param normalizedValue the new control value in normalized form
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::updatePluginParameterNormalized(int32_t controlID, double normalizedValue, ParameterUpdateInfo& paramInfo)
{
	// --- use base class helper, returns actual value
	double controlValue = setPIParamValueNormalized(controlID, normalizedValue, paramInfo.applyTaper);

	// --- do any post-processing
	postUpdatePluginParameter(controlID, controlValue, paramInfo);

	return true; /// handled
}

/**
\brief perform any operations after the plugin parameter has been updated; this is one paradigm for
	   transferring control information into vital plugin variables or member objects. If you use this
	   method you can decode the control ID and then do any cooking that is needed. NOTE: do not
	   overwrite bound variables here - this is ONLY for any extra cooking that is required to convert
	   the GUI data to meaninful coefficients or other specific modifiers.

\param controlID the control ID value of the parameter being updated
\param controlValue the new control value
\param paramInfo structure of information about why this value is being udpated (e.g as a result of a preset being loaded vs. the top of a buffer process cycle)

\return true if operation succeeds, false otherwise
*/
bool PluginCore::postUpdatePluginParameter(int32_t controlID, double controlValue, ParameterUpdateInfo& paramInfo)
{
    // --- now do any post update cooking; be careful with VST Sample Accurate automation
    //     If enabled, then make sure the cooking functions are short and efficient otherwise disable it
    //     for the Parameter involved
    /*switch(controlID)
    {
        case 0:
        {
            return true;    /// handled
        }

        default:
            return false;   /// not handled
    }*/

    return false;
}

/**
\brief has nothing to do with actual variable or updated variable (binding)

CAUTION:
- DO NOT update underlying variables here - this is only for sending GUI updates or letting you
  know that a parameter was changed; it should not change the state of your plugin.

WARNING:
- THIS IS NOT THE PREFERRED WAY TO LINK OR COMBINE CONTROLS TOGETHER. THE PROPER METHOD IS
  TO USE A CUSTOM SUB-CONTROLLER THAT IS PART OF THE GUI OBJECT AND CODE.
  SEE http://www.willpirkle.com for more information

\param controlID the control ID value of the parameter being updated
\param actualValue the new control value

\return true if operation succeeds, false otherwise
*/
bool PluginCore::guiParameterChanged(int32_t controlID, double actualValue)
{
	/*
	switch (controlID)
	{
		case controlID::<your control here>
		{

			return true; // handled
		}

		default:
			break;
	}*/

	return false; /// not handled
}

/**
\brief For Custom View and Custom Sub-Controller Operations

NOTES:
- this is for advanced users only to implement custom view and custom sub-controllers
- see the SDK for examples of use
- function gets called on load/unload, every animation (50 ms), other scenarios

\param messageInfo a structure containing information about the incoming message

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMessage(MessageInfo& messageInfo)
{
	// --- decode message
	switch (messageInfo.message)
	{
		// --- add customization appearance here
	case PLUGINGUI_DIDOPEN:
	{
		if (bankAndWaveGroup_0) bankAndWaveGroup_0->updateView();
		return false;
	}

	// --- NULL pointers so that we don't accidentally use them
	case PLUGINGUI_WILLCLOSE:
	{
		return false;
	}

	// --- update view; this will only be called if the GUI is actually open
	case PLUGINGUI_TIMERPING:
	{
		return false;
	}

	// --- register the custom view, grab the ICustomView interface
	case PLUGINGUI_REGISTER_CUSTOMVIEW:
	{

		return false;
	}

	case PLUGINGUI_REGISTER_SUBCONTROLLER:
	{
		// --- decode name string
		if (messageInfo.inMessageString.compare("BankWaveController_0") == 0)
		{
			// --- (1) get the custom view interface via incoming message data*
			//         can use it for communication
			if (bankAndWaveGroup_0 != static_cast<ICustomView*>(messageInfo.inMessageData))
				bankAndWaveGroup_0 = static_cast<ICustomView*>(messageInfo.inMessageData);

			if (!bankAndWaveGroup_0) return false;

			// --- need to tell the subcontroller the bank names
			VSTGUI::BankWaveMessage subcontrollerMessage;
			subcontrollerMessage.message = VSTGUI::UPDATE_BANK_NAMES;

			subcontrollerMessage.bankNames = synthEngine.getBankNames(0, 0); // voice, voice-oscillator 
			bankAndWaveGroup_0->sendMessage(&subcontrollerMessage);

			// --- now add the wave names for each bank
			subcontrollerMessage.message = VSTGUI::ADD_BANK_WAVENAMES;

			// --- bank 0
			subcontrollerMessage.bankIndex = 0;
			subcontrollerMessage.waveformNames = synthEngine.getOscWaveformNames(0, 0, 0); // voice, voice-oscillator, bank 

			bankAndWaveGroup_0->sendMessage(&subcontrollerMessage);

			// --- bank 1
			subcontrollerMessage.bankIndex = 1;
			subcontrollerMessage.waveformNames = synthEngine.getOscWaveformNames(0, 0, 1); // voice,  voice-oscillator, bank 

			bankAndWaveGroup_0->sendMessage(&subcontrollerMessage);

			// --- registered!
			return true;
		}
	}

	case PLUGINGUI_QUERY_HASUSERCUSTOM:
	case PLUGINGUI_USER_CUSTOMOPEN:
	case PLUGINGUI_USER_CUSTOMCLOSE:
	case PLUGINGUI_EXTERNAL_SET_NORMVALUE:
	case PLUGINGUI_EXTERNAL_SET_ACTUALVALUE:
	{

		return false;
	}

	default:
		break;
	}

	return false; /// not handled
}


/**
\brief process a MIDI event

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param event a structure containing the MIDI event data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::processMIDIEvent(midiEvent& event)
{
	synthEngine.processMIDIEvent(event);
	lastEvent = event;
	return true;
}

/**
\brief (for future use)

NOTES:
- MIDI events are 100% sample accurate; this function will be called repeatedly for every MIDI message
- see the SDK for examples of use

\param vectorJoysickData a structure containing joystick data

\return true if operation succeeds, false otherwise
*/
bool PluginCore::setVectorJoystickParameters(const VectorJoystickData& vectorJoysickData)
{
	// asynchronous
	SynthEngineParameters engineParams = synthEngine.getParameters();
	engineParams.voiceParameters->vectorJSData = vectorJoysickData;
	synthEngine.setParameters(engineParams);
	return true;
}

/**
\brief use this method to add new presets to the list

NOTES:
- see the SDK for examples of use
- for non RackAFX users that have large paramter counts, there is a secret GUI control you
  can enable to write C++ code into text files, one per preset. See the SDK or http://www.willpirkle.com for details

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginPresets()
{
	// **--0xFF7A--**

	// --- Plugin Presets 
	int index = 0;
	PresetInfo* preset = nullptr;

	// --- Preset: Factory Preset
	preset = new PresetInfo(index++, "Factory Preset");
	initPresetParameters(preset->presetParameters);
	setPresetParameter(preset->presetParameters, controlID::masterPitchBend, 7.000000);
	setPresetParameter(preset->presetParameters, controlID::masterTune, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::masterVolume_dB, -3.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1Waveform, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1Mode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1Frequency_Hz, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::lfo1DelayTime_mSec, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1RampTime_mSec, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo2Waveform, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo2Mode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo2Frequency_Hz, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::lfo2Amp, 100.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo2_shape, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::lfo2_shapeY, 0.500000);
	setPresetParameter(preset->presetParameters, controlID::lfo1_pitchMode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc1_waveForm, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc1_BankIndex, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc2_waveForm, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::osc2_BankIndex, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::osc3_BankIndex, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc4_BankIndex, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::osc3_waveForm, 2.000000);
	setPresetParameter(preset->presetParameters, controlID::osc4_waveForm, 3.000000);
	setPresetParameter(preset->presetParameters, controlID::osc1_detune, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc2_detune, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::rotorShape, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::rotorMode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::rotorFreq, 0.010000);
	setPresetParameter(preset->presetParameters, controlID::eg1AttackTime_mSec, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1DecayTime_mSec, 100.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1SustainLevel, 0.707000);
	setPresetParameter(preset->presetParameters, controlID::eg1ReleaseTime_mSec, 500.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1DelayTime_mSec, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1HoldTime_mSec, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1Mode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1_retrigger, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1_attackFromZero, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::filter1_fc, 880.000000);
	setPresetParameter(preset->presetParameters, controlID::filter1_q, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::eg2AttackTime_mSec, 5.000000);
	setPresetParameter(preset->presetParameters, controlID::eg2DecayTime_mSec, 100.000000);
	setPresetParameter(preset->presetParameters, controlID::eg2SustainLevel, 0.707000);
	setPresetParameter(preset->presetParameters, controlID::eg2ReleaseTime_mSec, 500.000000);
	setPresetParameter(preset->presetParameters, controlID::eg1_sourceInt, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::eg2_sourceInt, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1_sourceInt, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo2_sourceInt, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc1_fo_destInt, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc2_fo_destInt, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::filt1_fo_destInt, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::lfo1_X_destInt, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::r1c1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r1c2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r1c3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r1c4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r2c1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r2c2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r2c3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r2c4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r3c1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r3c2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r3c3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r3c4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r4c1, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r4c2, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r4c3, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::r4c4, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::filter1_enableKeyTrack, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::filter1_keyTrackRatio, 1.000000);
	setPresetParameter(preset->presetParameters, controlID::osc3_detune, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::osc4_detune, 0.000000);
	setPresetParameter(preset->presetParameters, controlID::synthMode, -0.000000);
	setPresetParameter(preset->presetParameters, controlID::unisonDetune_cents, 0.000000);
	addPreset(preset);


	// **--0xA7FF--**

    return true;
}

/**
\brief setup the plugin description strings, flags and codes; this is ordinarily done through the ASPiKreator or CMake

\return true if operation succeeds, false otherwise
*/
bool PluginCore::initPluginDescriptors()
{
    pluginDescriptor.pluginName = PluginCore::getPluginName();
    pluginDescriptor.shortPluginName = PluginCore::getShortPluginName();
    pluginDescriptor.vendorName = PluginCore::getVendorName();
    pluginDescriptor.pluginTypeCode = PluginCore::getPluginType();

	// --- describe the plugin attributes; set according to your needs
	pluginDescriptor.hasSidechain = kWantSidechain;
	pluginDescriptor.latencyInSamples = kLatencyInSamples;
	pluginDescriptor.tailTimeInMSec = kTailTimeMsec;
	pluginDescriptor.infiniteTailVST3 = kVSTInfiniteTail;

    // --- AAX
    apiSpecificInfo.aaxManufacturerID = kManufacturerID;
    apiSpecificInfo.aaxProductID = kAAXProductID;
    apiSpecificInfo.aaxBundleID = kAAXBundleID;  /* MacOS only: this MUST match the bundle identifier in your info.plist file */
    apiSpecificInfo.aaxEffectID = "aaxDeveloper.";
    apiSpecificInfo.aaxEffectID.append(PluginCore::getPluginName());
    apiSpecificInfo.aaxPluginCategoryCode = kAAXCategory;

    // --- AU
    apiSpecificInfo.auBundleID = kAUBundleID;   /* MacOS only: this MUST match the bundle identifier in your info.plist file */

    // --- VST3
    apiSpecificInfo.vst3FUID = PluginCore::getVSTFUID(); // OLE string format
    apiSpecificInfo.vst3BundleID = kVST3BundleID;/* MacOS only: this MUST match the bundle identifier in your info.plist file */
	apiSpecificInfo.enableVST3SampleAccurateAutomation = kVSTSAA;
	apiSpecificInfo.vst3SampleAccurateGranularity = kVST3SAAGranularity;

    // --- AU and AAX
    apiSpecificInfo.fourCharCode = PluginCore::getFourCharCode();

    return true;
}

// --- static functions required for VST3/AU only --------------------------------------------- //
const char* PluginCore::getPluginBundleName() { return kAUBundleName; }
const char* PluginCore::getPluginName(){ return kPluginName; }
const char* PluginCore::getShortPluginName(){ return kShortPluginName; }
const char* PluginCore::getVendorName(){ return kVendorName; }
const char* PluginCore::getVendorURL(){ return kVendorURL; }
const char* PluginCore::getVendorEmail(){ return kVendorEmail; }
const char* PluginCore::getAUCocoaViewFactoryName(){ return AU_COCOA_VIEWFACTORY_STRING; }
pluginType PluginCore::getPluginType(){ return kPluginType; }
const char* PluginCore::getVSTFUID(){ return kVSTFUID; }
int32_t PluginCore::getFourCharCode(){ return kFourCharCode; }
