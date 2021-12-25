#include <iostream>
#include "gtest/gtest.h"
#include "GameObjects.h"
#include "GlobalEnvironment.h"

class TestConsistentSearchFixture : public testing::Test {
protected:
    size_t num_of_tests = 4;
    const char *map[4] = {"abababab", "1223334444", "\n\n\n\n", ""};
    size_t size_map[4] = {8, 10, 4, 0};
    const char *symbols[4] = {"a", "12", "abdl", "df"};
    const size_t result[4] = {4, 3, 0, 0};
};

TEST()

TEST_F(TestConsistentSearchFixture, FindSymbolInMap) {
    size_t number_found;
    for (size_t i = 0; i < num_of_tests; i++){
        number_found = 0;
        FindSymbolInMap(&number_found, map[i], size_map[i], symbols[i], 1);
        EXPECT_EQ(number_found, result[i]);
    }

    // test conding not 1
    number_found = 0;
    FindSymbolInMap(&number_found, map[1], size_map[1], symbols[1], 2);
    EXPECT_EQ(number_found, 1);
    number_found = 0;
    FindSymbolInMap(&number_found, map[0], size_map[0], symbols[2], 2);
    EXPECT_EQ(number_found, 4);
}


class TestConsistentMapFixture : public testing::Test {
protected:
    void SetUp() override {
        fd1 = open(valid_file_name.c_str(), O_RDONLY);
        fd2 = open("file2.txt", O_RDONLY);
        if (fd2 == -1){
            close(fd1);
            exit(1);
        }
        if (fd1 == -1)
            exit(1);

        struct stat st;
        stat(valid_file_name.c_str(), &st);
        f1_len = st.st_size;
        stat(+ "file2.txt", &st);
        f2_len = st.st_size;

        page = getpagesize();
        valid_memory = page * 4;
    }
    void TearDown() override {
        close(fd1);
        close(fd2);
    }
    std::string valid_file_name = "file1.txt";
    std::string invalid_file_name = "invalid_file.txt";

    size_t valid_coding = 1;
    size_t invalid_coding = 3;

    size_t valid_memory;
    size_t invalid_memory = 3;

    const char *symbols = "bb";
    const char *symbols_for_mmap = "b";

    int fd1, fd2;
    size_t f1_len, f2_len;

    size_t page;
};


TEST_F(TestConsistentMapFixture, FindNumSymbols) {
    size_t num_of_symbols = 0;
    EXPECT_EQ(ERROR_OPEN_FILE, FindNumSymbols(&num_of_symbols, invalid_file_name.c_str(),
                                              symbols, valid_coding, 0));
    EXPECT_EQ(ERROR_RAM, FindNumSymbols(&num_of_symbols, valid_file_name.c_str(),
                                        symbols, valid_coding, invalid_memory));
    EXPECT_EQ(BAD_CODING, FindNumSymbols(&num_of_symbols, valid_file_name.c_str(),
                                         symbols, invalid_coding, 0));
    EXPECT_EQ(0, FindNumSymbols(&num_of_symbols, valid_file_name.c_str(),
                                symbols, 0, 0));
}


TEST_F(TestConsistentMapFixture, MmapAndSearch){
    size_t num_of_symbols = 0;
    EXPECT_EQ(ERROR_MAP, MmapAndSearch(&num_of_symbols, fd1, f1_len, symbols, valid_coding, invalid_memory));
    num_of_symbols = 0;
    EXPECT_EQ(0, MmapAndSearch(&num_of_symbols, fd2, f2_len, symbols_for_mmap, valid_coding, valid_memory));
    EXPECT_EQ(num_of_symbols, 4);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}