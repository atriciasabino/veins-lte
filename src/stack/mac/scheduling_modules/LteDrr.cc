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
#include <LteDrr.h>
#include <LteSchedulerEnb.h>


void LteDrr::prepareSchedule (){

    activeTempList_ = activeList_;
    drrTempMap_ = drrMap_;

    bool terminateFlag = false, activeFlag = true, eligibleFlag = true;
    unsigned int eligible = activeTempList_.size();
    // Loop until the active list is not empty and there is spare room.
    while ( ! activeTempList_.empty() && eligible > 0 ) {
        // Get the current CID.
        MacCid cid = activeTempList_.current();

        // Get the current DRR descriptor.
        DrrDesc& desc = drrTempMap_[cid];

        // Check for connection eligibility. If not, skip it.
        if ( ! desc.eligible_ ) {
            activeTempList_.move();
            eligible--;
            continue;
        }
        
        // Update the deficit counter.
        desc.deficit_ += desc.quantum_;
        // Clear the flags passed to the grant() function.
        terminateFlag = false;
        activeFlag = true;
        eligibleFlag = true;
        // Try to schedule as many PDUs as possible (or fragments thereof).
        unsigned int scheduled = eNbScheduler_->scheduleGrant (cid, desc.deficit_, terminateFlag, activeFlag, eligibleFlag);

        if(desc.deficit_ - scheduled < 0)
        {
            throw cRuntimeError("LteDrr::execSchedule CID:%d unexpected deficit value of %d",cid,desc.deficit_);
        }
        // Update the deficit counter.
        desc.deficit_ -= scheduled;

        // Update the number of eligible connections.
        if ( ! eligibleFlag || ! activeFlag ) {
            eligible--;              // decrement the number of eligible conns
            desc.eligible_ = false;  // this queue is not eligible for service
        }

        // Remove the queue if it has become inactiveList_.
        if ( ! activeFlag ) {
            activeTempList_.erase();          // remove from the active list
            activeConnectionTempSet_.erase(cid);
            desc.deficit_ = 0;       // reset the deficit to zero
            desc.active_  = false;   // set this descriptor as inactive
        
        // If scheduling is going to stop and the current queue has not
        // been served entirely, then the RR pointer should NOT move to
        // the next element of the active list. Instead, the deficit
        // is decremented by a quantum, which will be added at
        // the beginning of the next round. Note that this step is only
        // performed if the deficit counter is greater than a quantum so
        // as not to give the queue more bandwidth than its fair share.
        } else if ( terminateFlag && desc.deficit_ >= desc.quantum_ ){
            desc.deficit_ -= desc.quantum_;
            
        // Otherwise, move the round-robin pointer to the next element.
        } else {
            activeTempList_.move ();
        }

        // Terminate scheduling, if the grant function specified so.
        if ( terminateFlag ) break;
    }
}

void LteDrr::commitSchedule ()
{
    activeList_ = activeTempList_;
    activeConnectionSet_=activeConnectionTempSet_;
    drrMap_ = drrTempMap_;
}

void
LteDrr::updateSchedulingInfo ()
{
    // Get connections.
    LteMacBufferMap* conn;

    if (direction_==DL)
    {
        conn = eNbScheduler_->mac_->getMacBuffers();
    } else if (direction_==UL)
    {
        conn = eNbScheduler_->mac_->getBsrVirtualBuffers();
    }

    //    // Iterators to cycle through the maps of connection descriptors.

    LteMacBufferMap::iterator it=conn->begin(),et=conn->end();

    // Select the minimum rate and MAC SDU size.
    double minSize = 0;
    double minRate = 0;
    for ( it = conn->begin() ; it != et ; ++it )
    {
//            ConnectionParameters& pars = jt->second.parameters_;
        MacCid cid = it->first;
        MacNodeId nodeId=MacCidToNodeId(cid);
        bool eligible=true;
        const UserTxParams& info = eNbScheduler_->mac_->getAmc()->computeTxParams(nodeId,direction_);
        unsigned int codeword=info.getLayers().size();
        if (eNbScheduler_->allocatedCws(nodeId)==codeword)
            eligible=false;

        for (unsigned int i=0;i<codeword;i++){
            if (info.readCqiVector()[i]==0)
                eligible=false;
        }
        if ( minRate == 0  /* || pars.minReservedRate_ < minRate*/ )
//                TODO add connections parameters and fix this value
            minRate = 500 ;
        if ( minSize == 0 /*|| pars.maxBurst_ < minSize */)
            minSize = 160; /*pars.maxBurst_;*/

        // Compute the quanta. If descriptors do not exist they are created.
        // The values of the other fields, e.g. active status, are not changed.

        drrMap_[cid].quantum_ = (unsigned int) ( ceil (  ( /*pars.minReservedRate_*/ 500 / minRate ) * minSize ) );
        drrMap_[cid].eligible_=eligible;

    }

}

void 
LteDrr::notifyActiveConnection(MacCid cid)
{
    EV << NOW <<"LteDrr::notify CID: " << cid<<endl;
    //this is a mirror structure of activelist, used by all the modules that want to know the list of active users
    activeConnectionSet_.insert (cid);

    bool alreadyIn=false;    
    activeList_.find(cid,alreadyIn);
    if (!alreadyIn)
    {
        activeList_.insert(cid);
        (drrMap_[cid]).active_=true;
    }
    
    (drrMap_[cid]).eligible_=true;

    EV << NOW << "LteSchedulerEnb::notifyDrr active: " << drrMap_[cid].active_ << endl;
}

void 
LteDrr::removeActiveConnection(MacCid cid){
    activeList_.eraseElem(cid);
    activeConnectionSet_.erase(cid);
}

