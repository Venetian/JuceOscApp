

/*
 ==============================================================================
 
 OSCfinder.cpp
 Created: 13 Sep 2011 7:03:15pm
 Author:  Hemmer / Andrew Robertson modifier
 
 ==============================================================================
 */

//#include "PluginProcessor.h"
//#include "OSCfinder.h"
#include "OSCfinder.h"


//, mlrVSTAudioProcessor * const owner)




/*if you get an error:
libc++abi.dylib: terminating with uncaught exception of type std::runtime_error: unable to bind udp socket
it may be something else is bound to this port alread
 //when compiling this, if you have a std:runtime exception, check there's not something running binding to osc in background
 //http://new-supercollider-mailing-lists-forums-use-these.2681727.n2.nabble.com/unable-to-bind-udp-socket-resolve-on-OS-X-td7604288.html
 //http://forum.openframeworks.cc/t/ofxosc-unable-to-connect-udp-socket-crashes-of-app-when-in-login-items-mac-osx-mavericks/14943/5

*/

// Constructor
OSCfinder:: OSCfinder(const String &prefix):
            Thread("OscListener Thread"),
            incomingPort(OSC_RECEIVE_PORT), //incomingPort can be changed later ...
            s(IpEndpointName("localhost", incomingPort), this),
            buffer(), p(buffer, 1536),
            transmitSocket(IpEndpointName("localhost", OSC_SENDER_PORT)),
            OSCPrefix(prefix)
{
    setPrefix(prefix);

}


void OSCfinder::ProcessMessage(const osc::ReceivedMessage& m, const IpEndpointName& /*remoteEndpoint*/)
{
    try
    {
        String msgPattern = m.AddressPattern();
        const int numArgs = m.ArgumentCount();
    
        if (msgPattern.equalsIgnoreCase(OSCPrefix))
            std::cout << "osc message matches address: ";
        else
            std::cout << "receive something else: ";
        std::cout << msgPattern;
        
        std::cout << " and " << numArgs << " argument(s)" << std::endl;

        osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
        
        for (int i = 0; i < numArgs; i++){
            //this isnt great - it assumes one argument which is a float
            
            if (arg->IsFloat())
                std::cout << "float[" << i << "] : " << getFloatOSCArg(m, i) << std::endl;
            if (arg->IsInt32())
                std::cout << "int32[" << i << "] : " << getFloatOSCArg(m, i) << std::endl;
            else if (arg->IsString())
                std::cout << "string" << std::endl;
            else
                std::cout << "arg type unknown (easy to add your desired type to the code)" << std::endl;
            
            //advance iterator to the index we want
            //might not be very efficient for large osc messages
            //but we progress sequentially here through message
            arg++;
        }
    }
    catch (osc::Exception& e)
    {
        DBG("error while parsing message: " << m.AddressPattern() << ": " << e.what() << "\n");
    }
}




float OSCfinder::getFloatOSCArg(const osc::ReceivedMessage& m, int index)
{
    try
    {
        // we assume only one argument
        const int numArgs = m.ArgumentCount();
        if (numArgs < index) throw osc::MissingArgumentException();
        
        osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();

        //advance iterator to the index we want
        //probably not very efficient for large osc messages!
        while (index > 0){
            arg ++;
            --index;
        }
        
        const float floatArg = arg->AsFloat();
        
        return floatArg;
        
    }
    catch (osc::WrongArgumentTypeException& )
    {
        // if the argument is an int pretending to be a float, then we
        // need to consider that too so we cast to float
        try
        {
            osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
            const float floatArg = (float) arg->AsInt32();
            
            return floatArg;
        }
        catch (osc::Exception &e)
        {
            throw e;
        }
    }
    catch (osc::Exception &e)
    {
        // pass exception on
        throw e;
    }
    
}


int OSCfinder::getIntOSCArg(const osc::ReceivedMessage& m, int index)
{
    try
    {
        // we assume only one argument
        const int numArgs = m.ArgumentCount();
        if (numArgs < index) throw osc::MissingArgumentException();
        
        osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
        //advance iterator to the index we want
        //probably not very efficient for large osc messages!
        while (index > 0){
            arg ++;
            --index;
        }
        
        const int intArg = arg->AsInt32();
        
        return intArg;
        
    }
    catch (osc::Exception &e)
    {
        // pass exception on
        throw e;
    }
}

/*
 
 float OSCfinder::getFloatOSCArg(const osc::ReceivedMessage& m)
 {
 //routine designed for just one argument
 try
 {
 // we assume only one argument
 const int numArgs = m.ArgumentCount();
 if (numArgs != 1) throw osc::MissingArgumentException();
 
 osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
 
 const float floatArg = arg->AsFloat();
 
 return floatArg;
 
 }
 catch (osc::WrongArgumentTypeException& )
 {
 // if the argument is an int pretending to be a float, then we
 // need to consider that too so we cast to float
 try
 {
 osc::ReceivedMessage::const_iterator arg = m.ArgumentsBegin();
 const float floatArg = (float) arg->AsInt32();
 
 return floatArg;
 }
 catch (osc::Exception &e)
 {
 throw e;
 }
 }
 catch (osc::Exception &e)
 {
 // pass exception on
 throw e;
 }
 
 }

 */

/*
 void OSCfinder::buttonPressCallback(const int &monomeCol, const int &monomeRow, const bool &state)
 {
 if (state) { DBG("button down " << monomeRow << " " << monomeCol); }
 else       { DBG("button up " << monomeRow << " " << monomeCol); }
 
 parent->processOSCKeyPress(monomeCol, monomeRow, state);
 }
 
 void OSCfinder::setLED(const int &row, const int &col, const int &val)
 {
 p.Clear();
 const char * msg = ledStr.getCharPointer();
 p << osc::BeginMessage(msg) << row << col << val << osc::EndMessage;
 transmitSocket.Send(p.Data(), p.Size());
 }
 
 void OSCfinder::setRow(const int &row, const int &val)
 {
 p.Clear();
 const char * msg = ledRowStr.getCharPointer();
 p << osc::BeginMessage(msg) << row << 0 << val << osc::EndMessage;
 transmitSocket.Send(p.Data(), p.Size());
 }
 
 void OSCfinder::clearGrid()
 {
 p.Clear();
 const char * msg = ledClearStr.getCharPointer();
 p << osc::BeginMessage(msg) << osc::EndMessage;
 transmitSocket.Send(p.Data(), p.Size());
 }
 
 const String stripWildcard = OSCPrefix+ "strip/*";
 if (numArgs != 3) throw osc::Exception();
 
 osc::ReceivedMessageArgumentStream args = m.ArgumentStream();
 
 // unpack the monome button, row and state (button up or down)
 osc::int32 row, col, state;
 args >> row >> col >> state >> osc::EndMessage;
 //buttonPressCallback(row, col, state == 1);
 

 else if (msgPattern.matchesWildcard(stripWildcard, false))
 {
 // strip off the /mlrvst/strip/ part of the message
 msgPattern = msgPattern.substring(stripWildcard.length() - 1);
 
 // and extract the SampleStrip rowID from the message
 const String rowIDStr = msgPattern.upToFirstOccurrenceOf("/", false, false);
 
 // const int stripID = rowIDStr.getIntValue();
 // handleStripMessage(stripID, m);
 }
 */

/*
 void OSCfinder::handleStripMessage(const int &stripID, const osc::ReceivedMessage& m)
 {
 const String msgPattern = m.AddressPattern();
 const String OSCcommand = msgPattern.fromFirstOccurrenceOf("/" + String(stripID) + "/", false, false);
 
 DBG("strip " << stripID << " sends command: " << OSCcommand);
 
 if (OSCcommand == "vol")
 {
 try
 {
 const float newVol = getFloatOSCArg(m);
 parent->setSampleStripParameter(SampleStrip::pStripVolume, &newVol, stripID);
 }
 catch (osc::Exception &) { DBG("Couldn't process volume message"); }
 }
 
 
 else if (OSCcommand == "speed")
 {
 try
 {
 const float newSpeed = getFloatOSCArg(m);
 parent->setSampleStripParameter(SampleStrip::pPlaySpeed, &newSpeed, stripID);
 }
 catch (osc::Exception &) { DBG("Couldn't process speed message"); }
 }
 
 else if (OSCcommand == "chan")
 {
 try
 {
 const int newChannel = getIntOSCArg(m);
 parent->setSampleStripParameter(SampleStrip::pCurrentChannel, &newChannel, stripID);
 }
 catch (osc::Exception &) { DBG("Couldn't process channel message"); }
 
 }
 }
 */