#pragma once

#include "rtos.hpp"
#include "hwlib.hpp"

enum Sounds
{
    GAMESTART,
    GAMEOVER,
    HITSOUND,
    SHOOT,
    STOP

};

class note
{
public:
    // from https://www.seventhstring.com/resources/notefrequencies.html
    static const int A4 = 440;
    static const int A4s = 466;
    static const int B4 = 494;
    static const int C5 = 523;
    static const int C5s = 554;
    static const int D5 = 587;
    static const int D5s = 622;
    static const int E5 = 659;
    static const int F5 = 698;
    static const int F5s = 740;
    static const int G5 = 784;
    static const int G5s = 830;
    static const int A5 = 880;
    static const int A5s = 932;
    static const int B5 = 987;

    const int frequency;

    static const int dF = 1'000'000; // full measure
    static const int dH = dF / 2;    // half measure
    static const int dQ = dF / 4;    // quarter measure

    const int duration;
};

class Speaker
{
private:
    hwlib::target::pin_out &speaker;

public:
    Speaker(hwlib::target::pin_out &speaker)
        : speaker(speaker)
    {
    }

    void turnOn()
    {
        speaker.write(1);
        speaker.flush();
    }
    void turnOff()
    {
        speaker.write(0);
        speaker.flush();
    }
};
class note_player_gpio
{
private:
    Speaker &lsp;

public:
    note_player_gpio(Speaker &lsp) : lsp(lsp)
    {
    }

    void play(const note &n)
    {
        if (n.frequency == 0)
        {
            hwlib::wait_us(n.duration);
        }
        else
        {
            auto half_period = 1'000'000 / (2 * n.frequency);
            auto end = hwlib::now_us() + n.duration;
            do
            {
                lsp.turnOn();
                hwlib::wait_us(half_period);
                lsp.turnOff();
                hwlib::wait_us(half_period);
            } while (end > hwlib::now_us());
        }
    }
};

class SpeakerController : public rtos::task<>
{

    enum state_t
    {
        IDLE,
        PLAY_SOUND
    };

private:
    state_t state = IDLE;
    Sounds soundID;

    rtos::channel<Sounds, 1024> soundIDChannel;

    Speaker speaker;
    note_player_gpio player;
    // Note note;

public:
    SpeakerController(hwlib::target::pin_out &speaker1, unsigned int priority)
        : rtos::task<>(priority, "SPEAKERCONTROLLER_TASK"), soundIDChannel(this, "SOUNDS_CHANNEL"), speaker(speaker1), player(speaker)
    {
    }

    void addSound(Sounds soundID)
    {
        soundIDChannel.write(soundID);
    }

private:
    void gameover()
    {
        player.play(note{note::D5, 90000});
        player.play(note{200, 90000});
        player.play(note{note::F5, 90000});
        player.play(note{200, 200000});
        player.play(note{note::F5, 90000});
        player.play(note{200, 90000});
        player.play(note{note::F5, 90000});
        player.play(note{200, 90000});
        player.play(note{note::E5, 90000});
        player.play(note{200, 150000});
        player.play(note{note::D5, 90000});
        player.play(note{200, 90000});
        player.play(note{note::C5, 90000});
        player.play(note{200, 90000});
    }
    void gamestart()
    {
        player.play(note{698, 178571});
        player.play(note{1046, 89285});
        player.play(note{987, 44642});
        player.play(note{1046, 44642});
        player.play(note{987, 44642});
        player.play(note{1046, 44642});
        player.play(note{987, 89285});
        player.play(note{1046, 89285});
        player.play(note{830, 178571});
        player.play(note{698, 178571});
        player.play(note{698, 89285});
        player.play(note{830, 89285});
        player.play(note{1046, 89285});
        player.play(note{1108, 178571});
        player.play(note{830, 178571});
        player.play(note{1108, 178571});
        player.play(note{1244, 178571});
        player.play(note{1046, 89285});
        player.play(note{1108, 89285});
        player.play(note{1046, 89285});
        player.play(note{1108, 89285});
        player.play(note{1046, 357136});
    }

    void hitsound()
    {
        player.play(note{650, 25000});
        player.play(note{600, 25000});
        player.play(note{550, 25000});
        player.play(note{500, 25000});
        player.play(note{450, 25000});
        player.play(note{400, 25000});
        player.play(note{350, 25000});
        player.play(note{300, 25000});
        player.play(note{250, 25000});
        player.play(note{200, 25000});
    }

    void shoot()
    {
        player.play(note{note::E5, 50000});
        player.play(note{note::F5, 50000});
        player.play(note{note::G5, 50000});
        player.play(note{note::A5, 50000});
    }
    void main()
    {
        //        Sounds tempSoundID;
        for (;;)
        {
            switch (state)
            {
            case IDLE:
                wait(soundIDChannel);

                soundID = soundIDChannel.read();

                if (soundID >= 0)
                {
                    state = PLAY_SOUND;
                }
                break;

            case PLAY_SOUND:
                switch (soundID)
                {
                case HITSOUND:
                    hitsound();
                    state = IDLE;
                    break;

                case SHOOT:
                    shoot();
                    state = IDLE;
                    break;

                case GAMESTART:
                    gamestart();
                    state = IDLE;
                    break;

                case GAMEOVER:
                    gameover();
                    state = IDLE;
                    break;
                case STOP:

                    break;
                };

                break;
            };
        }
    }
};