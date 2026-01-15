/*
 * txc15.cc
 *
 *  Created on: Jan 15, 2026
 *      Author: Romi
 */

#include <stdio.h>
#include <string.h>
#include <omnetpp.h>
#include "tictoc15_m.h"

using namespace omnetpp;

class Txc15 : public cSimpleModule
{
  private:
    long numSent;
    long numReceived;

    // STATISTICS OBJECTS
    cHistogram hopCountStats;
    cOutVector hopCountVector;

  protected:
    virtual TicTocMsg15 *generateMessage();
    virtual void forwardMessage(TicTocMsg15 *msg);
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

Define_Module(Txc15);

void Txc15::initialize()
{
    numSent = 0;
    numReceived = 0;
    WATCH(numSent);
    WATCH(numReceived);

    // Name the statistics so they appear clearly in the output file
    hopCountStats.setName("hopCountStats");
    hopCountVector.setName("HopCount");

    if (getIndex() == 0) {
        TicTocMsg15 *msg = generateMessage();
        scheduleAt(0.0, msg);
    }
}

void Txc15::handleMessage(cMessage *msg)
{
    TicTocMsg15 *ttmsg = check_and_cast<TicTocMsg15 *>(msg);

    if (ttmsg->getDestination() == getIndex()) {
        int hops = ttmsg->getHopCount();
        EV << "Message arrived after " << hops << " hops.\n";

        // UPDATE STATS
        numReceived++;
        hopCountVector.record(hops);
        hopCountStats.collect(hops);

        delete ttmsg;

        TicTocMsg15 *newMsg = generateMessage();
        forwardMessage(newMsg);
    }
    else {
        forwardMessage(ttmsg);
    }
}

TicTocMsg15 *Txc15::generateMessage()
{
    int src = getIndex();
    int n = getVectorSize();
    int dest = intuniform(0, n-2);
    if (dest >= src) dest++;

    char msgname[20];
    sprintf(msgname, "tic-%d-to-%d", src, dest);

    TicTocMsg15 *msg = new TicTocMsg15(msgname);
    msg->setSource(src);
    msg->setDestination(dest);
    return msg;
}

void Txc15::forwardMessage(TicTocMsg15 *msg)
{
    msg->setHopCount(msg->getHopCount() + 1);
    numSent++;
    int n = gateSize("gate");
    int k = intuniform(0, n-1);
    send(msg, "gate$o", k);
}

void Txc15::finish()
{
    recordScalar("#sent", numSent);
    recordScalar("#received", numReceived);
    hopCountStats.recordAs("hop count");
}
