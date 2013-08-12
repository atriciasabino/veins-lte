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


#ifndef REMOTEANTENNASET_H_
#define REMOTEANTENNASET_H_

#include <vector>
#include "Coord.h"

/**
 * @class RemoteAntennaSet
 * @brief Class containing all physical antenna infos
 *
 * This class stores all informations regarding the
 * Remote Antenna Set (RAS) of an eNB, that is, the
 * physical information of all its remotes.
 *
 * The RAS is a vector of remotes, and for each remote
 * we store the couple <position, transmit power>.
 *
 */
class RemoteAntennaSet {
    private:
        /// Remote Antenna physical informations
        struct RemoteAntenna {
            /// Position
            Coord ruPosition_;

            /// Transmit power
            double txPower_;
        };

        /// Vector storing all remote antennas
        std::vector<RemoteAntenna> remoteAntennaSet_;

    public:
        /// Constructor
        RemoteAntennaSet() {;}

        /// Destructor
        ~RemoteAntennaSet() {;}

        /**
         * addRemoteAntenna() is called by das to add a new
         * antenna for the eNB with all its physical properties
         *
         * @param ruX Remote Antenna coordinate along X
         * @param ruX Remote Antenna coordinate along Y
         * @param ruPow Remote Antenna transmit power
         */
        void addRemoteAntenna(double ruX, double ruY, double ruPow);

        /**
         * getAntennaCoord() is used by the phy to find
         * the Coordinates of a given antenna
         *
         * @param remote i-th physical antenna index
         * @return i-th antenna coordinates
         */
        Coord getAntennaCoord(unsigned int remote);

        /**
         * getAntennaTxPower() is used by the phy to find
         * the TxPower of a given antenna
         *
         * @param remote i-th physical antenna index
         * @return i-th antenna tx power
         */
        double getAntennaTxPower(unsigned int remote);

        /**
         * getAntennaSetSize() returns the number of physical
         * antennas (MACRO included)
         *
         * @return number of antennas
         */
        unsigned int getAntennaSetSize();

        /**
         * Debugging: prints a line with all coordinates and tx powers of
         * all antennas (MACRO included)
         */
        friend std::ostream &operator<<(std::ostream &stream, const RemoteAntennaSet* ruSet_);
};

#endif /* REMOTEANTENNASET_H_ */
