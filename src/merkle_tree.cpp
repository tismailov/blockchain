#include "merkle_tree.hpp"

MerkleTree::MerkleTree(const std::vector<std::string>& files_paths){     
    struct stribog_ctx_t ctx;

    //Чтение файлов и вычисление их хэша
    for(int i = 0; i < files_paths.size(); i++){

        hash_file(&ctx, files_paths[i].c_str());
        mpz_class hash = stribog_ctx_to_mpz_256(ctx);
        files_hashes.push_back(hash);
    }

    //Если количество транзакций нечетное, последняя транзакция дублируется
    if(files_hashes.size() % 2 != 0){
        files_hashes.push_back(files_hashes[files_hashes.size()-1]);
    }

    fades.push_back(files_hashes);
}   

void MerkleTree::calc_top_hash(){
    struct stribog_ctx_t ctx;
    init(&ctx, HASH256);

    size_t last_fade_ind;
    while(true){
        last_fade_ind = fades.size() - 1;

        if(fades[last_fade_ind].size() == 1){
            break;
        }

        std::vector<mpz_class> fade;
        for(int i = 0; i < fades[last_fade_ind].size() - 1; i+=2){

            mpz_class leaf = fades[last_fade_ind][i] + fades[last_fade_ind][i + 1]; // Складываем 2 соседних хэша
            
            char* tmp = mpz_get_str(nullptr, 10, leaf.get_mpz_t()); 
            const char* str = tmp; // Конвертируем сумму в строку
            
            stribog(&ctx, (u8*)str, strlen(str)); // Вычисляем хэш из полученной строки 
            mpz_class leaf_hash = stribog_ctx_to_mpz_256(ctx); // Конвертируем полученный хэш в mpz_class  
            fade.push_back(leaf_hash);
        }

        if(fade.size() % 2 != 0 && fade.size() != 1){
            fade.push_back(fade[fade.size() - 1]);
        }
        
        fades.push_back(fade);
    }

    top_hash = fades[last_fade_ind][0];
}

mpz_class MerkleTree::get_top_hash(){
    return top_hash;
}