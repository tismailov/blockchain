#include "block_builder.hpp"

BlockBuilder::BlockBuilder(const std::vector<std::string>& files_paths, PRNG& prng){
    // 1. Размер блока (4 произвольных ненулевых байта)
    block_size = (prng.generate_num() & 0xFFFFFF) | 0x01010101; // Гарантируем ненулевые байты

    //Вычисление корня дерева Меркла
    MerkleTree merkle_tree(files_paths);
    merkle_tree.calc_top_hash();
    data_hash = merkle_tree.get_top_hash();

    //Хэш предыдущего блока (генерация псевдослучайного числа)
    pre_block_header_hash = prng.generate_num();

    //Запись текущей даты
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::setfill('0')
        << std::setw(2) << now_tm->tm_hour    // час
        << std::setw(2) << now_tm->tm_mday    // день
        << std::setw(2) << (now_tm->tm_mon + 1) // месяц
        << std::setw(2) << (now_tm->tm_year % 100); // год (2 цифры)
    timestamp = oss.str();
    
    nonce = 0;
}

void BlockBuilder::build_block() {
    std::ofstream block("block_header.txt");
    std::string header;

    struct stribog_ctx_t ctx;
    init(&ctx, HASH256);
    while (true) {
        // 1. Формируем заголовок блока
        header = 
            block_size.get_str() + 
            pre_block_header_hash.get_str() + 
            data_hash.get_str() + 
            timestamp + 
            nonce.get_str();

        // 2. Вычисляем хеш заголовка (пример для SHA-256)
        stribog(&ctx, (u8*)header.c_str(), strlen(header.c_str())); // Вычисляем хэш из полученной строки 
        block_header_hash = stribog_ctx_to_mpz_256(ctx); // Конвертируем полученный хэш в mpz_class 

        // 3. Проверяем условие Proof-of-Work (первые 5 бит = 0)
        if (check_pow_condition(block_header_hash, 5)) {
            break; // Условие выполнено
        }

        // 4. Увеличиваем nonce для следующей попытки
        nonce += 1;
    }

    // Сохраняем валидный блок
    block << "Block Size: " << std::hex << block_size.get_str() << std::endl;
    block << "Previous Hash: " << pre_block_header_hash.get_str(16) << std::endl;
    block << "Data Hash: " << data_hash.get_str(16) << std::endl;
    block << "Timestamp: " << std::hex << timestamp << std::endl;
    block << "Nonce: " << std::hex << nonce.get_str() << std::endl;
    block << "Block Hash: " << block_header_hash.get_str(16) << std::endl;

    block.close();
 

}

bool BlockBuilder::check_pow_condition(const mpz_class& hash, int zero_bits) {
    mpz_class shifted = hash >> (256 - zero_bits); // Сдвигаем, оставляя только старшие zero_bits
    return shifted == 0;
}

void BlockBuilder::print_pow_proof() {
    // Получаем двоичное представление (будет строка из 256 символов: '0' или '1')
    std::string bin = block_header_hash.get_str(2);
    
    // Дополняем нулями слева, если длина меньше 256
    while (bin.length() < 256)
        bin = "0" + bin;

    // Вывод хэша в битах
    std::cout << "Block hash in binary (first 32 bits shown):\n";
    for (int i = 0; i < 32; ++i) {
        if (i < 5)
            std::cout << "\033[1;32m" << bin[i] << "\033[0m";  // зелёный цвет для первых zero_bits
        else
            std::cout << bin[i];
    }
    std::cout << " ...\n";

    // Отдельно покажем первые N бит числом
    std::bitset<8> first_byte(bin.substr(0, 8));
    std::cout << "First 8 bits: " << first_byte << "\n";
    std::cout << "First " << 5 << " bits: ";
    for (int i = 0; i < 5; ++i) std::cout << bin[i];
    std::cout << " (should be all 0)\n";
}

void BlockBuilder::print_block(){
    std::cout << "Block Size: " << std::hex << block_size.get_str() << std::endl;
    std::cout << "Previous Hash: " << pre_block_header_hash.get_str(16) << std::endl;
    std::cout << "Data Hash: " << data_hash.get_str(16) << std::endl;
    std::cout << "Timestamp: " << std::hex << timestamp << std::endl;
    std::cout << "Nonce: " << std::hex << nonce.get_str() << std::endl;
    std::cout << "Block Hash: " << block_header_hash.get_str(16) << std::endl;
}