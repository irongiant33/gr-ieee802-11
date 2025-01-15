/*
 * Copyright (C) 2013 Bastian Bloessl <bloessl@ccs-labs.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef INCLUDED_IEEE802_11_UTILS_H
#define INCLUDED_IEEE802_11_UTILS_H

#include <gnuradio/config.h>
#include <ieee802_11/api.h>
#include <ieee802_11/mapper.h>
#include <ieee802_11/constellations.h>
#include <cinttypes>
#include <iostream>

using gr::ieee802_11::Encoding;

#define SAMPLES_PER_OFDM_SYMBOL 32 //there are 32 HaLow subcarriers
#define CODED_BITS_PER_OFDM_SYMBOL 24 // there are 26 effective HaLow subcarriers. 2 are pilots, bringing the number of data subcarriers to 24 
#define NUM_PILOTS 2 //2 HaLow pilots. p.3253 of spec
#define PILOT1_INDEX 9 //technically -7 in the spec, but that is a range of [-16, 15). We're operating in range of [0, 31). p.3253 of spec
#define PILOT2_INDEX 23 //technically +7 in the spec, but see above ^. p.3253 of spec.
#define TRAVELING_PILOT_POSITIONS 13 //number of travelling pilot positions. Table 23-21, p.3254.
#define NUM_OFDM_SYMBOLS_IN_LTF1 4 //this is described in Figure 23-3
#define NUM_OFDM_SYMBOLS_IN_SIG_FIELD 6 //this is described in Figure 23-3
#define NUM_BITS_UNREPEATED_SIG_SYMBOL 12 // p. 3251
#define NUM_BITS_DECODED_SIG_SYMBOL 6 // p. 3251
#define NUM_BITS_IN_HALOW_SIG_FIELD 36 //p.3246 of spec
#define NUM_SIG_FIELD_REPETITIONS 2 //p.3246 of spec
#define NUM_BITS_PER_REPETITION 6 //p.3246 of spec
#define MCS_FIRST_BIT_INDEX 7 //p.3246 of spec
#define MCS_LAST_BIT_INDEX 10 //p.3246 of spec, inclusive
#define SAMPLES_PER_GI 8 //for ieee802.11a/g, the GI is 0.8us. For 802.11ah, GI is 8us

#define MAX_PAYLOAD_SIZE 1500
#define MAX_PSDU_SIZE 511 //MAX_PSDU_SIZE is the maximum number of octets (or ofdm symbols) per Halow frames (length field in SIG is coded on 9 bits, see Table 23-18)
#define MAX_SYM ((8 * MAX_PSDU_SIZE + 8 + 6) / 6)
#define MAX_BITS_PER_SYM 160 //is this true for HaLow?
#define MAX_ENCODED_BITS MAX_BITS_PER_SYM * MAX_PSDU_SIZE //is 16 samples per GI?

#define dout d_debug&& std::cout
#define mylog(...)                      \
    do {                                \
        if (d_log) {                    \
            d_logger->info(__VA_ARGS__); \
        }                               \
    } while (0);

#pragma pack(push, 1)
struct mac_header {
    // protocol version, type, subtype, to_ds, from_ds, ...
    uint16_t frame_control;
    uint16_t duration;
    uint8_t addr1[6];
    uint8_t addr2[6];
    uint8_t addr3[6];
    uint16_t seq_nr;
};
#pragma pack(pop)

/**
 * WIFI parameters
 */
class ofdm_param
{
public:
    ofdm_param(Encoding e);

    // data rate
    Encoding encoding;
    // rate field of the SIGNAL header
    char rate_field;
    // number of coded bits per sub carrier
    int n_bpsc;
    // number of coded bits per OFDM symbol
    int n_cbps;
    // number of data bits per OFDM symbol
    int n_dbps;
    // constellation
    std::shared_ptr<gr::digital::constellation> constellation;

    void print();
};

/**
 * packet specific parameters
 */
class frame_param
{
public:
    frame_param(ofdm_param& ofdm, int psdu_length);//for DATA field
    frame_param(ofdm_param& ofdm);//for SIG field
    // PSDU size in bytes
    int psdu_size;
    // number of OFDM symbols (17-11)
    int n_sym;
    // number of padding bits in the DATA field (17-13)
    int n_pad;
    int n_encoded_bits;
    // number of data bits, including service and padding (17-12)
    uint16_t n_data_bits;

    void print();
};

/**
 * Given a payload, generates a MAC data frame (i.e., a PSDU) to be given
 * to the physical layer for encoding.
 *
 * \param msdu the payload for the MAC frame
 * \param msdu_size the size of the msdu in bytes
 * \param psdu pointer to a byte array where to store the MAC frame. Memory
 * will be alloced by the function
 * \param psdu_size pointer to an integer where the size of the psdu in bytes
 * will be stored
 * \param seq sequence number of the frame
 */
void generate_mac_data_frame(
    const char* msdu, int msdu_size, char** psdu, int* psdu_size, char seq);

void scramble(const char* input, char* out, frame_param& frame, char initial_state);

void reset_tail_bits(char* scrambled_data, frame_param& frame);

void convolutional_encoding(const char* input, char* out, frame_param& frame);

void puncturing(const char* input, char* out, frame_param& frame, ofdm_param& ofdm);

void interleave(const char* input,
                char* out,
                frame_param& frame,
                ofdm_param& ofdm,
                bool reverse = false);

void split_symbols(const char* input, char* out, frame_param& frame, ofdm_param& ofdm);

void generate_bits(const char* psdu, char* data_bits, frame_param& frame);


/**
 * Variables and functions related to frame decoding
 * 
 * 
 */

void deinterleave(gr_complex* deinterleaved, const gr_complex* rx_symbols);

void unrepeat(gr_complex* unrepeated, gr_complex* deinterleaved);

const int interleaver_pattern[CODED_BITS_PER_OFDM_SYMBOL] = {
    0, 3, 6, 9,  12, 15, 18, 21,
    1, 4, 7, 10, 13, 16, 19, 22,
    2, 5, 8, 11, 14, 17, 20, 23
}; //table 23-20 and table 23-41

#endif /* INCLUDED_IEEE802_11_UTILS_H */
