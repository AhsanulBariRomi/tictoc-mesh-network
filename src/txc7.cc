/*
 * txc7.cc
 *
 *  Created on: Jan 13, 2026
 *      Author: Romi
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc7 : public cSimpleModule
{
  private:
    cMessage *event = nullptr;
    cMessage *tictocMsg = nullptr;

  public:
    virtual ~Txc7();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc7);

Txc7::~Txc7()
{
    cancelAndDelete(event);
    delete tictocMsg;
}

void Txc7::initialize()
{
    event = new cMessage("waitTimer");

    // For this step, we just hardcode "tic" to start again for simplicity
    if (strcmp("tic", getName()) == 0) {
        EV << "Tic starting the process...\n";
        tictocMsg = new cMessage("tictocMsg");

        // We schedule the first event to happen immediately (delay = 0 for start)
        scheduleAt(simTime(), event);
    }
}

void Txc7::handleMessage(cMessage *msg)
{
    if (msg == event) {
        EV << "Wait period is over, sending message out!\n";
        if (tictocMsg != nullptr) {
             send(tictocMsg, "out");
             tictocMsg = nullptr;
        }
    }
    else {
        // A message arrived from the network
        EV << "Message arrived.\n";

        // 1. Simulate Packet Loss (10% chance)
        // uniform(0, 1) returns a random decimal between 0.0 and 1.0
        if (uniform(0, 1) < 0.1) {
            EV << "Oops! The message was lost.\n";
            delete msg; // We drop it and DO NOT schedule the event. The game ends here.
            return;
        }

        // 2. If not lost, calculate random delay
        // We read the volatile parameter to get a fresh random number
        simtime_t delay = par("delayTime");

        EV << "Processing... will reply in " << delay << " seconds.\n";

        tictocMsg = msg;
        scheduleAt(simTime() + delay, event);
    }
}


