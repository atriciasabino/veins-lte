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


#ifndef __LTE_FEEDBACKTESTER_H_
#define __LTE_FEEDBACKTESTER_H_

#include <omnetpp.h>
#include "LteDlFeedbackGenerator.h"

/**
 * TODO - Generated class
 */
class FeedbackTester : public cSimpleModule
{
    simtime_t interval_;
    cMessage *aperiodic_;
    LteDlFeedbackGenerator *generator_;

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
};

#endif
