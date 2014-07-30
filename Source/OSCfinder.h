//
//  OSCfinder.h
//  MidiSyncTracker
//
//  Created by Hemmer / modified by Andrew Robertson on 30/07/2014.
//
//

//#include "OSCfinder.h"

#ifndef __OSCFINDER__
#define __OSCFINDER__

#include "../JuceLibraryCode/JuceHeader.h"

// OSC send includes
#include "osc/OscOutboundPacketStream.h"
#include "ip/IpEndpointName.h"
// OSC receive includes
#include "osc/OscReceivedElements.h"
#include "osc/OscPacketListener.h"
// OSC shared includes
#include "ip/UdpSocket.h"

//#include "AudioSample.h"

// forward declaration
//class MainContentComponent;//mlrVSTAudioProcessor;

#define OSC_RECEIVE_PORT 7500//change for alternative port
#define OSC_SENDER_PORT 8080//NOT USING?

class OSCfinder :  public osc::OscPacketListener,
public Thread
{
    
public:
    
    // Constructor
    OSCfinder(const String &prefix);//, MainContentComponent* const owner);//, mlrVSTAudioProcessor * const owner);
    
    ~OSCfinder()
    {
        // stop the OSC Listener thread running
        s.AsynchronousBreak();
        
        // allow the thread 2 seconds to stop cleanly - should be plenty of time.
        stopThread(2000);
    }
    
    // Start the oscpack OSC Listener Thread
    // NOTE: s.Run() won't return unless we force it to with
    // s.AsynchronousBreak() as is done in the destructor
    void run()
    {
        s.Run();
    }
    /*
     void buttonPressCallback(const int &monomeCol, const int &monomeRow, const bool &state);
     void setLED(const int &row, const int &col, const int &val);
     void setRow(const int &row, const int &val);
     void clearGrid();
     */
    void setPrefix(const String &prefix)
    {
        OSCPrefix = "/" + prefix;
        
        DBG("prefix now: " << OSCPrefix);
    }
    
private:
    //Hemmer needed the component info so included a pointer - see his code for how
  //  MainContentComponent* const parent;
    
    int incomingPort;
    UdpListeningReceiveSocket s;
    
    
    // outgoing messages ////////////////////
    char buffer[1536];                  // to store message data
    osc::OutboundPacketStream p;
    UdpTransmitSocket transmitSocket;
    
    // strings ////////////////////////////
    String OSCPrefix;                       // main prefix (/mlrvst/ by default)
    //   String ledStr, ledRowStr, ledClearStr;  // + "led", + "led_row", + "clear"
    //   String buttonPressMask;                 // + "press"
    
    //   void handleStripMessage(const int &stripID, const osc::ReceivedMessage& m);
    
    //float getFloatOSCArg(const osc::ReceivedMessage& m);//defunkt as using iterators now
    float getFloatOSCArg(const osc::ReceivedMessage& m, int index);
    int getIntOSCArg(const osc::ReceivedMessage& m, int index);
    
    JUCE_LEAK_DETECTOR(OSCfinder);
    
protected:
    
    void ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& /*remoteEndpoint*/);
};


#endif





