#include "create_transactions.hpp"

void create_transactions(std::string name, PRNG& prng){
    std::ofstream first_transaction("transactions/transaction0.txt");
    first_transaction << name << std::endl;
    first_transaction.close();

    for(int i = 1; i < 5; i++){
        std::string file_name = "transactions/transaction" +  std::to_string(i) + ".txt";
        std::ofstream outFile(file_name);
        std::string content = prng.generate_num().get_str();
        outFile << content;
        std::cout << "Transaction number " << i << " contents: " << content << "\n";
        
        std::string sign = "exp";
        outFile << sign;
        std::cout << "Transaction number " << i << " signature: " << sign <<"\n";

        outFile.close();
    }
}