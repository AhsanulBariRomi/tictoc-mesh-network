/*
 * txc6.cc
 *
 *  Created on: Jan 13, 2026
 *      Author: Romi
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc6 : public cSimpleModule
{
  private:
    cMessage *event = nullptr;      // The "Alarm Clock"
    cMessage *tictocMsg = nullptr;  // The message we hold onto while waiting

  public:
    virtual ~Txc6();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Txc6);

Txc6::~Txc6()
{
    cancelAndDelete(event);
    delete tictocMsg;
}

void Txc6::initialize()
{
    // Create the timer object
    event = new cMessage("waitTimer");

    // Use the parameter to decide if we start (Inheritance style!)
    if (par("sendMsgOnInit").boolValue() == true) {
        EV << "Initialized: I am configured to start the process.\n";

        // We schedule the FIRST message to go out immediately (or after 1s if you prefer)
        tictocMsg = new cMessage("tictocMsg");

        // Let's create an immediate "wait" so Tic also simulates processing
        scheduleAt(simTime() + 1.0, event);
    }
}

void Txc6::handleMessage(cMessage *msg)
{
    // SCENARIO 1: The Alarm Clock rang
    if (msg == event) {
        EV << "Wait period is over, sending message out!\n";

        // If we have a stored message, send it
        if (tictocMsg != nullptr) {
            send(tictocMsg, "out");
            tictocMsg = nullptr; // We don't own it anymore
        } else {
             // Use case: Initial startup
             tictocMsg = new cMessage("tictocMsg");
             send(tictocMsg, "out");
             tictocMsg = nullptr;
        }
    }
    // SCENARIO 2: A network packet arrived
    else {
        EV << "Message arrived. Holding it for 1 second...\n";

        // 1. Keep the message
        tictocMsg = msg;

        // 2. Schedule the alarm for NOW + 1.0s
        scheduleAt(simTime() + 1.0, event);
    }
}


