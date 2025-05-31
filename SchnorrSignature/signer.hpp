#ifndef SIGNER_HPP
#define SIGNER_HPP

#include <string>
#include <utility>
#include <gmpxx.h>
#include "signature.h"
#include "prng_c_interface.h"
#include "../src/prng.hpp"  

std::pair<mpz_class, mpz_class> get_signature(const std::string& message, 
                                             const mpz_class& x, 
                                             const mpz_class& g, 
                                             const mpz_class& p);

#endif // SIGNER_HPP
