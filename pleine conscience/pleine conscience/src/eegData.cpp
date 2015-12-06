#include "eegData.h"

#include "ofMain.h"

eegData::eegData(const std::vector<std::string> values){
	if(values.size() < NUMBER_OF_TERMS){
		ofLogError("eegData::eegData") << "not enough values supplied";
		return;
	}

	SignalQuality = stoi(values[0]);
	Attention = stoi(values[1]);
	Meditation = stoi(values[2]);
	Delta = stoi(values[3]);
	Theta = stoi(values[4]);
	LowAlpha = stoi(values[5]);
	HighAlpha = stoi(values[6]);
	LowBeta = stoi(values[7]);
	HighBeta = stoi(values[8]);
	LowGamma = stoi(values[9]);
	HighGamma = stoi(values[10]);
}

#ifdef SIMULATION_MODE
	eegData::eegData(){
		SignalQuality = rand() % 1000;
		Attention = rand() % 1000;
		Meditation = rand() % 1000;
		Delta = rand() % 1000;
		Theta = rand() % 1000;
		LowAlpha = rand() % 1000;
		HighAlpha = rand() % 1000;
		LowBeta = rand() % 1000;
		HighBeta = rand() % 1000;
		LowGamma = rand() % 1000;
		HighGamma = rand() % 1000;
	}
#endif


eegData::~eegData(){
}

std::istream& operator>>(std::istream& is, eegData& eeg){
	is >> eeg.SignalQuality;
	is >> eeg.Attention;
	is >> eeg.Meditation;
	is >> eeg.Delta;
	is >> eeg.Theta;
	is >> eeg.LowAlpha;
	is >> eeg.HighAlpha;
	is >> eeg.LowBeta;
	is >> eeg.HighBeta;
	is >> eeg.LowGamma;
	is >> eeg.HighGamma;

	return is;
}

std::ostream& operator<<(std::ostream& os, const eegData& eeg){
	os << eeg.SignalQuality << " ";
	os << eeg.Attention << " ";
	os << eeg.Meditation << " ";
	os << eeg.Delta << " ";
	os << eeg.Theta << " ";
	os << eeg.LowAlpha << " ";
	os << eeg.HighAlpha << " ";
	os << eeg.LowBeta << " ";
	os << eeg.HighBeta << " ";
	os << eeg.LowGamma << " ";
	os << eeg.HighGamma << " ";

	return os;
}