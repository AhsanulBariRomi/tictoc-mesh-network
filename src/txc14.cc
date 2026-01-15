/*
 * txc14.cc
 *
 *  Created on: Jan 15, 2026
 *      Author: Romi
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "tictoc14_m.h" // Don't forget this!

using namespace omnetpp;

class Txc14 : public cSimpleModule
{
  private:
    long numSent;
    long numReceived;

  protected:
    virtual TicTocMsg14 *generateMessage();
    virtual void forwardMessage(TicTocMsg14 *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void refreshDisplay() const override; // <--- The new magic function
};

Define_Module(Txc14);

void Txc14::initialize()
{
    numSent = 0;
    numReceived = 0;

    // WATCH makes these variables visible in the Object Inspector
    WATCH(numSent);
    WATCH(numReceived);

    if (getIndex() == 0) {
        TicTocMsg14 *msg = generateMessage();
        scheduleAt(0.0, msg);
    }
}

void Txc14::handleMessage(cMessage *msg)
{
    TicTocMsg14 *ttmsg = check_and_cast<TicTocMsg14 *>(msg);

    if (ttmsg->getDestination() == getIndex()) {
        // Message Arrived
        numReceived++;
        delete ttmsg;

        // Send a new one
        bubble("ARRIVED, starting new one!");
        TicTocMsg14 *newMsg = generateMessage();
        forwardMessage(newMsg);
    }
    else {
        // Forward it
        forwardMessage(ttmsg);
    }
}

void Txc14::refreshDisplay() const
{
    // Update the "t=" tag (text) in the display string
    char buf[40];
    sprintf(buf, "rcvd: %ld sent: %ld", numReceived, numSent);

    // This is safe to call constantly because it only updates the GUI
    getDisplayString().setTagArg("t", 0, buf);
}

TicTocMsg14 *Txc14::generateMessage()
{
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src) dest++;

    char msgname[20];
    sprintf(msgname, "tic-%d-to-%d", src, dest);

    TicTocMsg14 *msg = new TicTocMsg14(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc14::forwardMessage(TicTocMsg14 *msg)
{
    msg->setHopCount(msg->getHopCount() + 1);
    numSent++; // Increment send counter

    int n = gateSize("gate");
    int k = intuniform(0, n-1);

    send(msg, "gate$o", k);
}


