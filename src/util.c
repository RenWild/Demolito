/*
 * Demolito, a UCI chess engine.
 * Copyright 2015 lucasart.
 *
 * Demolito is free software: you can redistribute it and/or modify it under the terms of the GNU
 * General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Demolito is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program. If
 * not, see <http://www.gnu.org/licenses/>.
*/
#include <assert.h>
#include "util.h"

// Simple hash function I derived from SplitMix64. Known limitations:
// - alignment: 'buffer' must be 8-byte aligned.
// - length: must be a multiple of 8 bytes.
// - endianness: big endian is long dead, who cares?
// - seedless: no seed argument.
// All these limitations could easily be lifted, but it's overkill in Demolito.
uint64_t hash(const void *buffer, size_t length)
{
    assert((uintptr_t)buffer % 8 == 0 && length % 8 == 0);
    const uint64_t *blocks = (const uint64_t *)buffer;
    uint64_t result = 0, seed = 0;

    for (size_t i = 0; i < length / 8; i++) {
        seed ^= blocks[i];
        result ^= prng(&seed);
    }

    return result;
}

// SplitMix64 PRNG, based on http://xoroshiro.di.unimi.it/splitmix64.c
uint64_t prng(uint64_t *state)
{
    uint64_t rnd = (*state += 0x9E3779B97F4A7C15);
    rnd = (rnd ^ (rnd >> 30)) * 0xBF58476D1CE4E5B9;
    rnd = (rnd ^ (rnd >> 27)) * 0x94D049BB133111EB;
    rnd ^= rnd >> 31;
    return rnd;
}
