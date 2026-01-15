/*
 * txc2.cc
 *
 *  Created on: Jan 13, 2026
 *      Author: Romi
 */

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

class Txc2 : public cSimpleModule
{
  private:
    int counter;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

// IMPORTANT: We register this as Txc2 now!
Define_Module(Txc2);

void Txc2::initialize()
{
    // Initialize the counter to 10
        counter = 10;

        // The WATCH() statement tells the GUI to display this variable
        // inside the "Objects" inspector when you debug.
        WATCH(counter);

    if (strcmp("tic", getName()) == 0) {
        // The EV command works just like cout in standard C++
        EV << "Sending initial message\n";
        cMessage *msg = new cMessage("tictocMsg");
        send(msg, "out");
    }
}

void Txc2::handleMessage(cMessage *msg)
{
    // 1. Decrease the counter
    counter--;
    // Log a message every time we receive a packet
    // 2. Check if we should stop
    if (counter == 0) {
        EV << getName() << "'s counter reached zero, deleting message\n";
        delete msg;  // <--- CRITICAL: If we don't send it, we must delete it!
    }
    else {
        // Log a message every time we receive a packet
        EV << getName() << "'s counter is " << counter << ", sending back message\n";
        send(msg, "out");
    }
}

