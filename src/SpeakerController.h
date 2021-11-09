#pragma once

enum Sounds
{
    GAMESTART,
    GAMEOVER,
    HITSOUND,
    SHOOT

};

class Note
{
public:
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

class SpeakerController : public rtos::task<>
{

    enum state_t
    {
        IDLE,
        PLAY_SOUND
    };

private:
    state_t state = IDLE;
    Sounds sound;

    rtos::channel<Sounds, 1024> soundIDChannel;

    Speaker speaker;
    // Note note;

public:
    SpeakerController(hwlib::target::pin_out &speaker1, unsigned int priority)
        : rtos::task<>(priority, "SPEAKERCONTROLLER_TASK"), soundIDChannel(this, "SOUNDS_CHANNEL"), speaker(speaker1)
    {
    }

    void addSound(Sounds soundID)
    {
        soundIDChannel.write(soundID);
    }

private:
    void playNote(const Note &n)
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
                speaker.turnOn();
                hwlib::wait_us(half_period);
                speaker.turnOff();
                hwlib::wait_us(half_period);
            } while (end > hwlib::now_us());
        }
    }

    void hitSound()
    {

        speaker.turnOn();
        hwlib::wait_ms(500);
        speaker.turnOff();
        hwlib::wait_ms(500);
        // playNote(note{note::E5, note::dQ});
        // playNote(note{note::D5s, note::dQ});
        // playNote(note{note::E5, note::dQ});
        // playNote(note{note::D5s, note::dQ});
        // playNote(note{note::E5, note::dQ});
        // playNote(note{note::B4, note::dQ});
        // playNote(note{note::D5, note::dQ});
        // playNote(note{note::C5, note::dQ});
        // playNote(note{note::A4, note::dH});
    }

    void shoot()
    {
    }

    void main()
    {
        Sounds soundID;
        for (;;)
        {
            switch (state)
            {
            case IDLE:
                wait(soundIDChannel);

                soundID = soundIDChannel.read();

                if (soundID)
                {
                    state = PLAY_SOUND;
                }

                break;

            case PLAY_SOUND:
                switch (soundID)
                {
                case HITSOUND:
                    hitSound();
                    state = IDLE;
                    break;

                case SHOOT:
                    shoot();
                    state = IDLE;
                    break;

                case GAMESTART:
                    hitSound();
                    state = IDLE;
                    break;

                case GAMEOVER:
                    shoot();
                    state = IDLE;
                    break;
                };

                break;
            };
        }
    }
};