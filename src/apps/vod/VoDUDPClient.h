// 
//                           SimuLTE
// Copyright (C) 2012 Antonio Virdis, Daniele Migliorini, Giovanni
// Accongiagioco, Generoso Pagano, Vincenzo Pii.
// 
// This file is part of a software released under the license included in file
// "license.pdf". This license can be also found at http://www.ltesimulator.com/
// The above file and the present reference are part of the software itself, 
// and cannot be removed from it.
// 
//

#ifndef VODUDPCLIENT
#define VODUDPCLIENT

#include <omnetpp.h>
#include "VoDPacket_m.h"
#include <string.h>
#include "VoDUDPStruct.h"
#include <fstream>
#include "UDPControlInfo_m.h"
#include "UDPSocket.h"
#include "IPvXAddressResolver.h"


using namespace std;
class TaggedSample: public cObject {
public:
    double sample;
    unsigned int id;
    // the emitting cComponent (module)
    cComponent* module;
};
class VoDUDPClient : public cSimpleModule
{
    UDPSocket socket;
    fstream outfile;

public:
    simsignal_t tptLayer0_;
    simsignal_t tptLayer1_;
    simsignal_t tptLayer2_;
    simsignal_t tptLayer3_;
    TaggedSample* tSample_;
    simsignal_t delayLayer0_;
    simsignal_t delayLayer1_;
    simsignal_t delayLayer2_;
    simsignal_t delayLayer3_;

  protected:

    virtual void initialize(int stage);
    virtual int numInitStages() const {return 4;}
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
    virtual void receiveStream(VoDPacket *msg);
};


#endif
