/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "OSCfinder.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component, private Timer
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void paint (Graphics&);
    void resized();

private:
    
    void timerCallback() override
    {
    int64 timenow = juce::Time::currentTimeMillis();
    std::cout << "timer callback " << timenow << std::endl;
    
    /*
    DatagramSocket* dataSocket;
    std::string* buffer;
    int result = dataSocket->waitUntilReady(true, 0);
    if (result) {
        int sizeRead = dataSocket->read(buffer, 512, false);
        std::cout << "reading " << std::endl;
    } else {
        std::cout << "nothing" << std::endl;
    }
    */
    }
    OSCfinder finder;
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
