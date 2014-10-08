///
/// \file SoapySDR/Types.hpp
///
/// Misc data type definitions used in the API.
///
/// \copyright
/// Copyright (c) 2014-2014 Josh Blum
/// SPDX-License-Identifier: BSL-1.0
///

#pragma once
#include <SoapySDR/Config.hpp>
#include <SoapySDR/Types.h>
#include <vector>
#include <string>
#include <map>

namespace SoapySDR
{

//! Direction type: transmit or receive
typedef SoapySDRDirection Direction;

//! TX direction constant
static const SoapySDRDirection TX = SOAPY_SDR_TX;

//! RX direction constant
static const SoapySDRDirection RX = SOAPY_SDR_RX;

//! Typedef for a dictionary of key-value string arguments
typedef std::map<std::string, std::string> Kwargs;

/*!
 * A simple min/max numeric range pair
 */
class SOAPY_SDR_API Range
{
public:

    //! Create an empty range (0.0, 0.0)
    Range(void);

    //! Create a min/max range
    Range(const double minimum, const double maximum);

    //! Get the range minimum
    double minimum(void) const;

    //! Get the range maximum
    double maximum(void) const;

private:
    double _min, _max;
};

/*!
 * Typedef for a list of min/max range pairs.
 * Overall minimum: rl.front().minimum();
 * Overall maximum: rl.back().maximum();
 */
typedef std::vector<Range> RangeList;

}

inline double SoapySDR::Range::minimum(void) const
{
    return _min;
}

inline double SoapySDR::Range::maximum(void) const
{
    return _max;
}
