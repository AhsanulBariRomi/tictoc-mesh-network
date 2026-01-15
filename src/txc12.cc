/*
 * txc12.cc
 *
 *  Created on: Jan 14, 2026
 *      Author: Romi
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc12 : public cSimpleModule
{
  protected:
    virtual void forwardMessage(cMessage *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc12);

void Txc12::initialize()
{
    if (getIndex() == 0) {
        char msgname[20];
        sprintf(msgname, "tic-%d", getIndex());
        cMessage *msg = new cMessage(msgname);
        scheduleAt(0.0, msg);
    }
}

void Txc12::handleMessage(cMessage *msg)
{
    if (getIndex() == 3) {
        EV << "Message " << msg << " arrived at destination node 3!\n";
        delete msg;
    }
    else {
        forwardMessage(msg);
    }
}

void Txc12::forwardMessage(cMessage *msg)
{
    // 1. Ask: How many connections do I have?
    int n = gateSize("gate");

    // 2. Pick a random one
    int k = intuniform(0, n - 1);

    EV << "Forwarding message " << msg << " on gate[" << k << "]\n";

    // 3. SEND command
    // CRITICAL CHANGE: We must use "gate$o" to specify the output half
    // $o and $i suffix is used to identify the input/output part of a two way gate
    send(msg, "gate$o", k);
}


