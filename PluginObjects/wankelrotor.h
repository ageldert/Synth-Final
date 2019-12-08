#pragma once

#include "synthdefs.h"
#include "guiconstants.h"
#include "synthlfo.h"

/**
\struct WankelRotorParameters
\ingroup SynthStructures
\brief Encapsulates Rotor parameters:

- Item 1
- Item 2
*/

struct WankelRotorParameters
{
	WankelRotorParameters() {}
	WankelRotorParameters& operator=(const WankelRotorParameters& params)	// need this override for collections to work
	{
		if (this == &params)
			return *this;
		mode = params.mode;
		shape = params.shape;
		freq = params.freq;

		lfo1Parameters = params.lfo1Parameters;
		lfo2Parameters = params.lfo2Parameters;

		return *this;
	}

	// --- individual parameters
	int mode = 0;
	double shape = 0.0;
	double freq = 0.0;

	std::shared_ptr<SynthLFOParameters> lfo1Parameters = std::make_shared<SynthLFOParameters>();
	std::shared_ptr<SynthLFOParameters> lfo2Parameters = std::make_shared<SynthLFOParameters>();
};


/**
\class WankelRotor
\ingroup SynthClasses
\brief Encapsulates 2 synth LFOs, inherits from ISynthModulator.<br>
Contains WankelRotorParameters.<br>
Outputs: contains 2 rendered LFOOutputs

*/
class WankelRotor : public ISynthModulator//ISynthOscillator
{
public:
	WankelRotor(const std::shared_ptr<MidiInputData> _midiInputData, std::shared_ptr<WankelRotorParameters> _parameters)
		: midiInputData(_midiInputData) 
	, parameters(_parameters){
		srand(time(NULL)); // --- seed random number generator

		// --- randomize the PN register
		pnRegister = rand();

		// --- create subcomponents
		lfo1.reset(new SynthLFO(midiInputData, parameters->lfo1Parameters));
		lfo2.reset(new SynthLFO(midiInputData, parameters->lfo2Parameters));

	}	/* C-TOR */
	virtual ~WankelRotor() {}				/* D-TOR */

	/**
	\brief Implements reset: 
	updates sample rate and phaseInc, and resets modCounters
	*/
	virtual bool reset(double _sampleRate)
	{
		sampleRate = _sampleRate;

		lfo1->reset(sampleRate);
		lfo2->reset(sampleRate);

		return true;
	}

	// --- ISynthModulator cont'd
	virtual bool update(bool updateAllModRoutings = true);

	virtual bool doNoteOn(double midiPitch, uint32_t _midiNoteNumber, uint32_t midiNoteVelocity) 
	{ 
		lfo1->doNoteOn(midiPitch, _midiNoteNumber, midiNoteVelocity);
		lfo2->doNoteOn(midiPitch, _midiNoteNumber, midiNoteVelocity);
		randomSHCounter = -1; // -1 = reset
		return true; 
	}


	virtual bool doNoteOff(double midiPitch, uint32_t _midiNoteNumber, uint32_t midiNoteVelocity)
	{
		return true; 
	}

	// --- the oscillator function
	const ModOutputData renderModulatorOutput();

	// --- get our modulators
	virtual std::shared_ptr<ModInputData> getModulators() {
		return modulators;
	}

	// --- not used here;
	virtual void setModulators(std::shared_ptr<ModInputData> _modulators) {
		modulators = _modulators;
	}

protected:
	// --- MIDI Data Interface
	const std::shared_ptr<MidiInputData> midiInputData = nullptr;
	
	// --- modulators
	std::shared_ptr<ModInputData> modulators = std::make_shared<ModInputData>();

	// --- parameters
	std::shared_ptr<WankelRotorParameters> parameters = nullptr;

	// --- sample rate
	double sampleRate = 0.0;			///< sample rate

	std::shared_ptr<SynthLFO> lfo1;
	std::shared_ptr<SynthLFO> lfo2;

	// --- 32-bit register for RS&H
	uint32_t pnRegister = 0;			///< 32 bit register for PN oscillator
	int randomSHCounter = -1;			///< random sample/hold counter;  -1 is reset condition
	double randomSHValue = 0.0;			///< current output, needed because we hold this output for some number of samples = (sampleRate / oscFrequency)

};

