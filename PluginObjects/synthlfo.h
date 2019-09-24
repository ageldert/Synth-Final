#pragma once

#include "synthdefs.h"
#include "guiconstants.h"

// HOMEWORK: generate image (custom structure and object diagram) for the synthlfo
// look through the documentation, code to look at all it goes; look at will's doxygen stuff
// should definitely have some modulation input, a core, containing an output object
// styling in draw.io should be set up to look consistent and good!

// --- LFO may have very diff waveforms from pitched output

/**
\enum LFOWaveform
\ingroup SynthDefs
\brief LFO Oscillation Type

- Triangle: triangle wave
- Sin: sine wave
- Saw: saw wave
- RSH: random sample-and-hold
- QRSH: quasi-random sample-and-hold
- Noise: random noise
- QRNoise: quasi-random noise

*/
enum class LFOWaveform { kTriangle, kSin, kSaw, kRSH, kQRSH, kNoise, kQRNoise };

/**
\enum LFOMode
\ingroup SynthDefs
\brief LFO Mode of operation

- Sync: LFO restarts with each new note event
- OneShot: LFO completes exactly one cycle
- FreeRun: LFO free runs after first note event

*/
enum class LFOMode { kSync, kOneShot, kFreeRun };


/**
\enum LFOOutput
\ingroup SynthDefs
\brief Determines type of LFO Modulation Output

- LFONormalOutput: Output modulates about 0.0 from MIN to MAX
- LFONormalOutputInverted: Inverted normal
- LFOQuadPhaseOutput: Output modulates with 90 degree phase offset)
- LFOQuadPhaseOutputInverted: Inverted quad phase
- UnipolarOutputFromMax: Modulation from MAX to 0.0 (mimics inverted EG)
- UnipolarOutputFromMin: Modulation from 0.0 to MAX (mimics EG)
*/
enum {
	kLFONormalOutput,			// 0
	kLFONormalOutputInverted,	// 1 etc...
	kLFOQuadPhaseOutput,
	kLFOQuadPhaseOutputInverted,
	kUnipolarOutputFromMax,
	kUnipolarOutputFromMin
};

/**
\struct SynthLFOParameters
\ingroup SynthStructures
\brief Encapsulates LFO parameters:
- waveform: oscillation type, see enum LFOWaveform
- mode: mode of operation, see enum LFOMode
- frequency_Hz: oscillation frequency
- outputAmplitude: amplitude of LFO output (default 1.0)
*/

struct SynthLFOParameters
{
	SynthLFOParameters() {}
	SynthLFOParameters& operator=(const SynthLFOParameters& params)	// need this override for collections to work
	{
		if (this == &params)
			return *this;

		waveform = params.waveform;
		mode = params.mode;

		frequency_Hz = params.frequency_Hz;
		outputAmplitude = params.outputAmplitude;

		delay_mSec = params.delay_mSec;
		rampTime_mSec = params.rampTime_mSec;

		return *this;
	}

	// --- individual parameters
	LFOWaveform waveform = LFOWaveform::kRSH;
	LFOMode mode = LFOMode::kSync;

	double frequency_Hz = 9.0;
	double outputAmplitude = 1.0;
	double delay_mSec = 0.0;
	double rampTime_mSec = 0.0;
};


/**
\class SynthLFO
\ingroup SynthClasses
\brief Encapsulates a synth LFO, inherits from ISynthModulator.<br>
Contains SynthLFOParameters.<br>
Outputs: contains 6 outputs (see LFOOutput)
- LFONormalOutput
- LFONormalOutputInverted
- LFOQuadPhaseOutput
- LFOQuadPhaseOutputInverted
- UnipolarOutputFromMax
- UnipolarOutputFromMin
*/
class SynthLFO : public ISynthModulator//ISynthOscillator
{
public:
	SynthLFO(const std::shared_ptr<MidiInputData> _midiInputData, std::shared_ptr<SynthLFOParameters> _parameters)
		: midiInputData(_midiInputData) 
	, parameters(_parameters){
		srand(time(NULL)); // --- seed random number generator

		// --- randomize the PN register
		pnRegister = rand();

	}	/* C-TOR */
	virtual ~SynthLFO() {}				/* D-TOR */

	/**
	\brief Implements reset: 
	updates sample rate and phaseInc, and resets modCounters
	*/
	virtual bool reset(double _sampleRate)
	{
		sampleRate = _sampleRate;
		phaseInc = parameters->frequency_Hz / sampleRate;

		// --- timebase variables
		modCounter = 0.0;			///< modulo counter [0.0, +1.0]
		modCounterQP = 0.25;		///<Quad Phase modulo counter [0.0, +1.0]

		lfo1_timer.resetTimer();

		return true;
	}

	// --- ISynthModulator cont'd
	virtual bool update(bool updateAllModRoutings = true);

	virtual bool doNoteOn(double midiPitch, uint32_t _midiNoteNumber, uint32_t midiNoteVelocity) 
	{ 
		renderComplete = false;
		if (parameters->mode == LFOMode::kSync || parameters->mode == LFOMode::kOneShot)
		{
			modCounter = 0.0;			///< modulo counter [0.0, +1.0]
			modCounterQP = 0.25;		///< Quad Phase modulo counter [0.0, +1.0]
		}
		
		lfo1_timer.resetTimer();
		lfo1_timer.setTargetValueInSamples(msecToSamples(sampleRate,parameters->delay_mSec));
		rampAmp = 0.0;
		rampInc = 1.0 / msecToSamples(sampleRate, parameters->rampTime_mSec);
	
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
	std::shared_ptr<SynthLFOParameters> parameters = nullptr;

	// --- sample rate
	double sampleRate = 0.0;			///< sample rate

	// --- timebase variables
	double modCounter = 0.0;			///< modulo counter [0.0, +1.0]
	double phaseInc = 0.0;				///< phase inc = fo/fs
	double modCounterQP = 0.25;			///< Quad Phase modulo counter [0.0, +1.0]
	bool renderComplete = false;		///< flag for one-shot
	double rampAmp = 1.0;				///< amplitude of LFO1
	double rampInc = 0.0;				///< increment based on rampTime_mSec

	// --- 32-bit register for RS&H
	uint32_t pnRegister = 0;			///< 32 bit register for PN oscillator
	int randomSHCounter = -1;			///< random sample/hold counter;  -1 is reset condition
	double randomSHValue = 0.0;			///< current output, needed because we hold this output for some number of samples = (sampleRate / oscFrequency)

	/**
	\brief Checks the moduloCounter and wraps value if needed 

	*/
	inline bool checkAndWrapModulo(double& moduloCounter, double phaseInc)
	{
		// --- for positive frequencies
		if (phaseInc > 0 && moduloCounter >= 1.0)
		{
			moduloCounter -= 1.0;
			return true;
		}

		// --- for negative frequencies
		if (phaseInc < 0 && moduloCounter <= 0.0)
		{
			moduloCounter += 1.0;
			return true;
		}

		return false;
	}

	/**
	\brief Advances moduloCounter, then checks and wraps value if needed
	*/
	inline bool advanceAndCheckWrapModulo(double& moduloCounter, double phaseInc)
	{
		// --- advance counter
		moduloCounter += phaseInc;

		// --- for positive frequencies
		if (phaseInc > 0 && moduloCounter >= 1.0)
		{
			moduloCounter -= 1.0;
			return true;
		}

		// --- for negative frequencies
		if (phaseInc < 0 && moduloCounter <= 0.0)
		{
			moduloCounter += 1.0;
			return true;
		}

		return false;
	}

	/**
	\brief Advances moduloCounter by phaseInc
	*/
	inline void advanceModulo(double& moduloCounter, double phaseInc) { moduloCounter += phaseInc; }

	// --- sine approximation with parabolas
	const double B = 4.0 / kPi;
	const double C = -4.0 / (kPi* kPi);
	const double P = 0.225;
	// http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine
	//
	// --- angle is -pi to +pi

	/**
	\brief Implements parabolas to approximate sine of a given angle. 
	Source: http://devmaster.net/posts/9648/fast-and-accurate-sine-cosine
	*/
	inline double parabolicSine(double angle)
	{
		double y = B * angle + C * angle * fabs(angle);
		y = P * (y * fabs(y) - y) + y;
		return y;
	}
/*
	inline double msecToSamples(double sampleRate, double timeMSec)
	{
		return sampleRate * (timeMSec / 1000.0);;
	}
*/
	Timer lfo1_timer;
};

