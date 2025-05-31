#include "signer.hpp"

std::pair<mpz_class, mpz_class> get_signature(const std::string& message,
                                             const mpz_class& x,
                                             const mpz_class& g,
                                             const mpz_class& p) {
    mpz_t r, s;
    mpz_inits(r, s, NULL);
    
    mpz_t x_tmp, g_tmp, p_tmp;
    mpz_inits(x_tmp, g_tmp, p_tmp, NULL);
    mpz_set(x_tmp, x.get_mpz_t());
    mpz_set(g_tmp, g.get_mpz_t());
    mpz_set(p_tmp, p.get_mpz_t());
    
    PRNG prng;
    PRNG_Handle_t prng_handle = PRNG_create_from_existing(&prng);
    struct stribog_ctx_t ctx;
    
    sign_message(message.c_str(), r, s, x_tmp, g_tmp, p_tmp, prng_handle, &ctx);
    
    // Создаем mpz_class из r и s
    mpz_class r_class, s_class;
    mpz_set(r_class.get_mpz_t(), r);
    mpz_set(s_class.get_mpz_t(), s);
    
    PRNG_destroy(prng_handle);
    mpz_clears(r, s, x_tmp, g_tmp, p_tmp, NULL);
    
    return std::make_pair(r_class, s_class);
}