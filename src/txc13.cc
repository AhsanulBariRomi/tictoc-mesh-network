/*
 * txc13.cc
 *
 *  Created on: Jan 14, 2026
 *      Author: Romi
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

// CRITICAL: Include the file generated from your .msg file!
#include "tictoc13_m.h"

using namespace omnetpp;

class Txc13 : public cSimpleModule
{
  protected:
    virtual TicTocMsg13 *generateMessage();
    virtual void forwardMessage(TicTocMsg13 *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc13);

void Txc13::initialize()
{
    // Node 0 starts the process
    if (getIndex() == 0) {
        TicTocMsg13 *msg = generateMessage();
        scheduleAt(0.0, msg);
    }
}

void Txc13::handleMessage(cMessage *msg)
{
    // SAFETY CHECK: Convert generic message to our custom type
    TicTocMsg13 *ttmsg = check_and_cast<TicTocMsg13 *>(msg);

    // 1. Is this message for me?
    if (ttmsg->getDestination() == getIndex()) {
        EV << "Message " << ttmsg << " arrived after " << ttmsg->getHopCount() << " hops!\n";
        bubble("ARRIVED, starting new one!");

        // Delete the old message
        delete ttmsg;

        // Generate a new random message to keep the simulation going
        TicTocMsg13 *newMsg = generateMessage();
        forwardMessage(newMsg);
    }
    // 2. If not, forward it
    else {
        forwardMessage(ttmsg);
    }
}

TicTocMsg13 *Txc13::generateMessage()
{
    // Pick a random destination (that isn't me)
    int src = getIndex();
    int n = getVectorSize(); // Total number of modules (6)
    int dest = intuniform(0, n-2);
    if (dest >= src) dest++;

    char msgname[20];
    sprintf(msgname, "tic-%d-to-%d", src, dest);

    // Create the CUSTOM message
    TicTocMsg13 *msg = new TicTocMsg13(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc13::forwardMessage(TicTocMsg13 *msg)
{
    // Increment hop count
    msg->setHopCount(msg->getHopCount() + 1);

    // Pick random gate
    int n = gateSize("gate");
    int k = intuniform(0, n-1);

    EV << "Forwarding message " << msg << " on gate[" << k << "]\n";
    send(msg, "gate$o", k);
}



