/*
 * txc10.cc
 *
 *  Created on: Jan 14, 2026
 *      Author: Romi
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc10 : public cSimpleModule
{
  protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc10);

void Txc10::initialize()
{
    // Am I Node 0? If so, start the traffic.
    if (getIndex() == 0) {
        // Boot the process
        char msgname[20];
        sprintf(msgname, "tic-%d", getIndex());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg); // Use a self-message to start "now"
    }
}

void Txc10::handleMessage(cMessage *msg)
{
    // 1. Check if I am the destination (Node 3)
    if (getIndex() == 3) {
        EV << "Message " << msg << " arrived at destination node 3!\n";
        delete msg; // Mission Accomplished
    }
    // 2. If not, forward it to a random neighbor
    else {
        forwardMessage(msg);
    }
}

void Txc10::forwardMessage(cMessage *msg)
{
    // Pick a random gate
    int n = gateSize("out");      // How many connections do I have?
    int k = intuniform(0, n - 1); // Pick a random index between 0 and (n-1)

    EV << "Forwarding message " << msg << " on port out[" << k << "]\n";

    // SEND command with 3 arguments: Message, Gate Name, Gate Index
    send(msg, "out", k);
}


