using namespace std;

#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <bits/stdc++.h>

#include "game.hpp"
#include "texturemanager.hpp"
#include "hsl.hpp"
#include "ball.cpp"
#include "fft.cpp"

Game::Game() {
    this->window.create(sf::VideoMode(1920, 1080), "Raveforms", sf::Style::Fullscreen);
    this->window.setVerticalSyncEnabled(true);
    this->window.setFramerateLimit(60);

    this->textures = TextureManager();

    this->loadTextures();
};

Game::~Game() {

}

void Game::loadTextures() {
}

void Game::loop() {
    sf::Clock clock;
    sf::Event event;

    Mic mic;
    mic.start(88200);

    while (window.isOpen()) {



        sf::Vector2f mouse = sf::Vector2f(sf::Mouse::getPosition(window));

        // Event loop
        while (window.pollEvent(event)) {
            switch(event.type) {
                case sf::Event::Closed:
                    window.close();
                    mic.stop();
                    return;
                    break;

                case sf::Event::KeyPressed:
                    if(event.key.code == sf::Keyboard::Escape) {
                        window.close();
                        mic.stop();
                        return;
                        break;
                    }
                case sf::Event::KeyReleased:
                    break;

                case sf::Event::MouseButtonPressed:
                    break;

                default:
                    break;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Shift)) {
            int grey = (float)mic.max/65536.0*255.0;
            window.clear(sf::Color(grey, grey, grey));
        } else {
            window.clear(sf::Color::Black);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            valarray<Complex> raw;
            raw.resize(mic.samples.size());
            for (int i = 0; i < mic.samples.size(); i++) {
                raw[i] = std::complex<double>(mic.samples[i] + 0.0, 0.0);
            }

            fft(raw);

            vector<sf::Vertex> waveform;

            for (int i = 0; i < mic.samples.size(); i++) {
                sf::Vertex v;
                double y = (raw[i/8].real() + raw[i/8].imag())/3000+540;

                v.position = sf::Vector2f(i, y);
                v.color = HSL(50 + y, 100, 50).TurnToRGB();
                waveform.push_back(v);

                if (i >= 1920) {
                    break;
                }
            }


            window.draw(&waveform[0], waveform.size(), sf::LinesStrip);
            cout << "drew fourier" << endl;

        } else {
            vector<sf::Vertex> waveform;
            for (int i = 0; i < mic.samples.size(); i++) {
                sf::Vertex v;
                v.position = sf::Vector2f(i, (mic.samples[i]+32768)/80+128);
                v.color = HSL(50+(mic.samples[i]+32768)/150, 100, 50).TurnToRGB();
                waveform.push_back(v);
                if (i >= 1920) {
                    break;
                }
            }
            cout << "finished waveform" << endl;

            window.draw(&waveform[0], waveform.size(), sf::LinesStrip);

            cout << "drew wave" << endl;
        }



        sf::VertexArray amp(sf::Quads, 4);
        amp[0].position = sf::Vector2f(0, 1040);
        amp[1].position = sf::Vector2f(0, 1080);
        amp[2].position = sf::Vector2f(mic.max/40, 1040);
        amp[3].position = sf::Vector2f(mic.max/40, 1080);

        amp[0].color = sf::Color::White;
        amp[1].color = sf::Color::Black;
        amp[2].color = sf::Color::White;
        amp[3].color = sf::Color::Black;

        window.draw(amp);
        cout << "drew amp" << endl;

        sf::VertexArray peaks(sf::Quads, 4);
        peaks[0].position = sf::Vector2f(1920, 1040);
        peaks[1].position = sf::Vector2f(1920, 1080);
        peaks[2].position = sf::Vector2f(1920 - mic.peaks*3, 1040);
        peaks[3].position = sf::Vector2f(1920 - mic.peaks*3, 1080);

        peaks[0].color = sf::Color::Black;
        peaks[1].color = sf::Color::White;
        peaks[2].color = sf::Color::Black;
        peaks[3].color = sf::Color::White;

        window.draw(peaks);
        cout << "drew peaks" << endl;

        window.display();
    }

}
