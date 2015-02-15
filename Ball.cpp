using namespace std;

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <bits/stdc++.h>



class Mic : public sf::SoundRecorder {
public:
	int max;
	int average;
	vector<int> samples;
	int peaks;


private:
	virtual bool onStart() {
		cout << "Recording starting" << endl;
		this->setProcessingInterval(sf::milliseconds(16.67));
		return true;
	}

	virtual bool onProcessSamples(const short* rawsamples, std::size_t sampleCount) {
		this->samples.clear();
		this->peaks = 0;
		this->max = 0;

		for (int i = 0; i < sampleCount-2 && i < 1920; i++) {
            int sample = *(rawsamples+i);
            int next = *(rawsamples+i+1);
			this->samples.push_back(sample);

			if (sample*next < 0) {
                this->peaks++;
			}

			if (sample > this->max) {
                this->max = sample;
			}
		}


		return true;
	}

	virtual void onStop() {
		cout << "Recording stopped" << endl;
		return;
	}
};
