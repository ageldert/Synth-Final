#include "WankelRotor.h"

/**
\brief runs priority modulators and updates
*/
bool WankelRotor::update(bool updateAllModRoutings)
{
	// --- Run priority modulators 
	
	lfo1->update(updateAllModRoutings);
	lfo2->update(updateAllModRoutings);
	return true;
}

/**
\ModOutputData renderModulatorOutput()
\ingroup SynthFunctions
\brief returns ModOutputData struct containing Rotor output
Checks and wraps modulo first
Returns output of all modulators, processing based on LFOWaveform and LFOOutput
Advances modulo every function call
*/
const ModOutputData WankelRotor::renderModulatorOutput()
{
	// --- setup for output
	ModOutputData rotorOutputData; // should auto-zero on instantiation
	rotorOutputData.clear();

	double x = lfo1->renderModulatorOutput().modulationOutputs[kLFONormalOutput];
	double y = lfo2->renderModulatorOutput().modulationOutputs[kLFOQuadPhaseOutput];

	// Rotor output is a pair of LFO outputs
	if (parameters->mode == 0)
	{
		rotorOutputData.modulationOutputs[0] = 0.0;
		rotorOutputData.modulationOutputs[1] = 0.0;
	}
	else if (parameters->mode == 1)
	{
		rotorOutputData.modulationOutputs[0] = x;
		rotorOutputData.modulationOutputs[1] = y;
	}
	else if (parameters->mode == 2)
	{
		rotorOutputData.modulationOutputs[0] = doWhiteNoise();
		rotorOutputData.modulationOutputs[1] = doWhiteNoise();
	}
	
	return rotorOutputData;
}
