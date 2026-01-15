/*
 * txc9.cc
 *
 *  Created on: Jan 13, 2026
 *      Author: Romi
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

/**
 * TOC Module: The clumsy receiver
 */
class Toc9 : public cSimpleModule
{
  protected:
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Toc9);

void Toc9::handleMessage(cMessage *msg)
{
    // 1. Roll the dice (30% chance to lose packet)
    if (uniform(0, 1) < 0.3) {
        EV << "\"Losing\" message " << msg << endl;

        // VISUALIZATION: Pop up a speech bubble over the node!
        bubble("message lost");

        delete msg; // Poof! Gone.
    }
    else {
        // 2. If successful, send it back (ACK)
        EV << "Message received, sending back acknowledgement.\n";
        send(msg, "out");
    }
}

/**
 * TIC Module: The smart sender with retransmission
 */
class Tic9 : public cSimpleModule
{
  private:
    simtime_t timeout;  // How long to wait before resending?
    cMessage *timeoutEvent = nullptr; // The "Alarm Clock"
    cMessage *message = nullptr;      // The ORIGINAL message (backup)
    int seq;                          // Sequence number (1, 2, 3...)

  public:
    virtual ~Tic9();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void generateNewMessage();
    virtual void sendCopyOf(cMessage *msg);
};

Define_Module(Tic9);

Tic9::~Tic9()
{
    cancelAndDelete(timeoutEvent);
    delete message;
}

void Tic9::initialize()
{
    // Initialize variables
    seq = 0;
    timeout = 1.0;
    timeoutEvent = new cMessage("timeoutEvent");

    // Create and send the very first message
    EV << "Sending initial message\n";
    generateNewMessage();
    scheduleAt(simTime() + timeout, timeoutEvent); // Set alarm
}

void Tic9::handleMessage(cMessage *msg)
{
    // SCENARIO 1: The Alarm Clock Rang (Timeout)
    if (msg == timeoutEvent) {
        EV << "Timeout expired, resending message and restarting timer\n";

        // Visualization: Make the node color Red for a moment
        bubble("Timeout!");

        // Resend the COPY (keep original in drawer)
        sendCopyOf(message);

        // Restart the alarm
        scheduleAt(simTime() + timeout, timeoutEvent);
    }
    // SCENARIO 2: We received an ACK from Toc
    else {
        EV << "Received: " << msg->getName() << "\n";

        // 1. Stop the alarm! We don't need to resend anymore.
        cancelEvent(timeoutEvent);

        // 2. Destroy the old ACK message
        delete msg;

        // 3. Destroy the ORIGINAL backup we were holding
        delete message;

        // 4. Start the next cycle
        EV << "Timer cancelled, generating new message...\n";
        generateNewMessage();

        // 5. Set alarm for the new message
        scheduleAt(simTime() + timeout, timeoutEvent);
    }
}

void Tic9::generateNewMessage()
{
    // Generate a name like "tic-1", "tic-2"...
    char msgname[20];
    sprintf(msgname, "tic-%d", ++seq);

    // Create the original message object
    message = new cMessage(msgname);

    // Send a COPY of it
    sendCopyOf(message);
}

void Tic9::sendCopyOf(cMessage *msg)
{
    // Create a perfect clone (duplicate) of the message
    cMessage *copy = (cMessage *)msg->dup();

    // Send the clone
    send(copy, "out");
}


