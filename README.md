guitareffectVST
------------------------------------------------------------------------------------------
https://github.com/jinjuyu/guitareffectvst

For help of each effect: http://rakarrack.sourceforge.net/effects.html

Help to this particular program: https://github.com/jinjuyu/guitareffectvst/wiki 

Download it Here: https://github.com/jinjuyu/guitareffectvst/blob/master/ExtraReleaseFiles/guitareffectVST-0.2.7.rar?raw=true

Demo MP3: https://github.com/jinjuyu/guitareffectvst/blob/master/Demo.mp3?raw=true

![screenshot](https://github.com/jinjuyu/guitareffectvst/raw/master/Screenshot.jpg)

New in 0.2
------------------------------------------------------------------------------------------
Added Parameter Automation Support.
Just right click on Slider and select parameter slot on the popup!


About
-------------------------------------------------------------------------------------------
This VST plugin is Win32 VST port of Rakkarack(http://rakarrack.sourceforge.net/)

About from Rakarrack website:
Rakarrack is a richly featured multi-effects processor emulating a guitar effects pedalboard.  Effects include compressor, expander, noise gate, graphic equalizer, parametric equalizer, exciter, shuffle, convolotron, valve, flanger, dual flange, chorus, musicaldelay, arpie, echo with reverse playback, musical delay, reverb, digital phaser, analogic phaser, synthfilter, varyband, ring, wah-wah, alien-wah, mutromojo, harmonizer, looper and four flexible distortion modules including sub-octave modulation and dirty octave up.  Most of the effects engine is built from modules found in the excellent software synthesizer ZynAddSubFX.  Presets and user interface are optimized for guitar, but Rakarrack processes signals in stereo while it does not apply internal band-limiting filtering, and thus is well suited to all musical instruments and vocals.  Rakarrack is designed for Linux distributions with Jack Audio Connection Kit.




INSTALL
-------------------------------------------------------------------------------------------
- You need to put all the other dlls in the Dependencies folder(libfftw3-3.dll, libfftw3f-3.dll, libfftw3l-3.dll, libsamplerate-0.dll, libsndfile-1.dll, DevIL.dll, ILU.dll) in the folder where the your DAW or Host's EXE is.
- For example: where the Podium.exe(REAPER.exe, Cubase.exe, Pedalboard2.exe, etc) is. System32 and VSTPlugins folder doesn't work.
- Put guitareffectVST.dll and guitareffectVST folder into VSTPlugins folder. 
- Load it into your favorite host and DONE! 

- To make this more convenient, I need to make a bridge dll for that but it's a lot of work so until then..



Working DAWs
-------------------------------------------------------------------------------------------
Tested on REAPER, Zynewave Podium, Minihost, Pedalboard2, FL Studio.



Future Plans
-------------------------------------------------------------------------------------------

- Bridge DLL between plugin DLL and host. (This will make this plugin work on every DAWs and Hosts)
- And also nice installer



Known Issues
------------------------------------------------------------------------------------------
- Dual Flange does not work due to a crashing bug I could not find the cause.(Removed from list)
- There is no Looper and Vocoder.
- StereoHarm and Harmonizer acts wierdly if MIDI and SELECT is on.
- Ring sometimes creates some noises, don't know if it's normal behavior.



About Source Code
----------------------------------------------------------------------------------------
- Code is for Visual C++ 2010(Express). Project file is placed under VSTGL directory.
- Code guideline:
    -> it usually retains original code
    -> it adds processReplacing() for every out().
    -> you need to reset PERIOD and fPERIOD for every processReplacing call in the processReplacing func.
    -> thus functions that uses PERIOD or fPERIOD needs to be updated too.
    -> for EffectLFO you need to call update() in processReplacing() like this:
    void APhaser::processReplacing()
    {
        ...

        PERIOD = sampleFrames;
        fPERIOD = PERIOD;
        invperiod = 1.0f/fPERIOD;
        lfo.update();
        (from APhaser.cpp) 

        ...
    }

