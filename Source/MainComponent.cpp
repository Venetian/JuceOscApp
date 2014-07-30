/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

#define PORT 7500
const String fixedString = "water";
//==============================================================================
MainContentComponent::MainContentComponent() : finder(fixedString)//, this)
{
    setSize (500, 400);
    
    startTimer(1000.);
    
    finder.startThread();
    DBG("OSC thread started");
    /*
    
    dataSocket = new DatagramSocket(7500, false);
    //7500, false);
    dataSocket->bindToPort(7500);
    
    const std::string empty = "";
    
    dataSocket->createListener(PORT ,empty);
    
    int sizeRead = 0;
    int result = dataSocket->waitUntilReady(true, 0);
    
    std::string* buffer;
    
    if (result) {
        sizeRead = dataSocket->read(buffer, 512, false);
        std::cout << "reading " << std::endl;
    }
    
    if (sizeRead > 0) {
        return buffer;
    } else {
        return emptyBuffer;
    }*/
   
    
}


MainContentComponent::~MainContentComponent()
{
    stopTimer();
  //  delete dataSocket;
}

void MainContentComponent::paint (Graphics& g)
{
    g.fillAll (Colour (0xffeeddff));

    g.setFont (Font (16.0f));
    g.setColour (Colours::black);
    g.drawText ("Hello World!", getLocalBounds(), Justification::centred, true);
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
