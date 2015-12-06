#pragma once

#define SIMULATION_MODE true

#include <vector>

/// <summary>
/// Represent a sample of data from the headset, serialized
/// </summary>
class eegData {
	public:
		/// <summary>
		/// Parse a string of values as given by the arduino device
		/// </summary>
		/// <param name="values">values sorted and seperated by ','</param>
		eegData(const std::vector<std::string> values);
		#ifdef SIMULATION_MODE
				eegData();
		#endif
		~eegData();

		unsigned int SignalQuality;
		unsigned int Attention;
		unsigned int Meditation;
		unsigned int Delta;
		unsigned int Theta;
		unsigned int LowAlpha;
		unsigned int HighAlpha;
		unsigned int LowBeta;
		unsigned int HighBeta;
		unsigned int LowGamma;
		unsigned int HighGamma;

		const static unsigned int NUMBER_OF_TERMS = 11;
};

std::istream& operator>>(std::istream& is, eegData& eeg);
std::ostream& operator<<(std::ostream& os, const eegData& eeg);

