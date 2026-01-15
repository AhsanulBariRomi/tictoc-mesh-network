/*
 * txc4.cc
 *
 *  Created on: Jan 13, 2026
 *      Author: Romi
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc4 : public cSimpleModule
{
  private:
    int counter;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc4);

void Txc4::initialize()
{
    // 1. Initialize the counter from the NED parameter
    counter = par("limit");

    // Watch the counter so you can see it in the runtime inspector
    WATCH(counter);

    // 2. Decide whether to send the first message based on the parameter
    // Notice we NO LONGER check if (name == "tic")!
    if (par("sendMsgOnInit").boolValue() == true) {
        EV << "Sending initial message\n";
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "out");
    }
}

void Txc4::handleMessage(cMessage *msg)
{
    counter--;

    if (counter == 0) {
        EV << getName() << "'s counter reached zero, deleting message\n";
        delete msg;
    }
    else {
        EV << getName() << "'s counter is " << counter << ", sending back message\n";
        send(msg, "out");
    }
}


