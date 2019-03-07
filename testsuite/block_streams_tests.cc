#include <gtest/gtest.h>
#include <testsuite/test_utils/block_test_case.h>

TEST(aes, test_vectors) {
    testsuite::block_test_case("AES", 10)();
}

TEST(aria, test_vectors) {
    testsuite::block_test_case("ARIA", 1)();
    testsuite::block_test_case("ARIA", 2)();
    testsuite::block_test_case("ARIA", 6)();
    testsuite::block_test_case("ARIA", 7)();
    testsuite::block_test_case("ARIA", 11)();
    testsuite::block_test_case("ARIA", 12)();
}

TEST(blowfish, test_vectors) {
    testsuite::block_test_case("BLOWFISH", 16)();
}

TEST(mars, test_vectors) {
    testsuite::block_test_case("MARS", 32)();
}

TEST(rc6, test_vectors) {
    testsuite::block_test_case("RC6", 20)();
}

TEST(serpent, test_vectors) {
    testsuite::block_test_case("SERPENT", 32)();
}

TEST(twofish, test_vectors) {
    testsuite::block_test_case("TWOFISH", 16)();
}

TEST(camellia, test_vectors) {
    testsuite::block_test_case("CAMELLIA", 18)();
    testsuite::block_test_case("CAMELLIA", 24)();
}

TEST(seed, test_vectors) {
    testsuite::block_test_case("SEED", 16)();
}

TEST(simon, test_vectors) {
    testsuite::block_test_case("SIMON", 32)();
    testsuite::block_test_case("SIMON", 36)();
    testsuite::block_test_case("SIMON", 42)();
    testsuite::block_test_case("SIMON", 44)();
    testsuite::block_test_case("SIMON", 52)();
    testsuite::block_test_case("SIMON", 54)();
    testsuite::block_test_case("SIMON", 68)();
    testsuite::block_test_case("SIMON", 69)();
    testsuite::block_test_case("SIMON", 72)();
}

TEST(speck, test_vectors) {
    testsuite::block_test_case("SPECK", 22)();
    testsuite::block_test_case("SPECK", 23)();
    testsuite::block_test_case("SPECK", 26)();
    testsuite::block_test_case("SPECK", 28)();
    testsuite::block_test_case("SPECK", 29)();
    testsuite::block_test_case("SPECK", 32)();
    testsuite::block_test_case("SPECK", 33)();
    testsuite::block_test_case("SPECK", 34)();
}

TEST(single_des, test_vectors) {
    testsuite::block_test_case("SINGLE-DES", 16)();
}

TEST(triple_des, test_vectors) {
    testsuite::block_test_case("TRIPLE-DES", 16)();
}

TEST(tea, test_vectors) {
    testsuite::block_test_case("TEA", 32)();
}

TEST(gost_block, test_vectors) {
    testsuite::block_test_case("GOST", 32)();
}

TEST(idea, test_vectors) {
    testsuite::block_test_case("IDEA", 8)();
}

TEST(cast, test_vectors) {
    testsuite::block_test_case("CAST", 16)();
}

TEST(kasumi, test_vectors) {
    testsuite::block_test_case("KASUMI", 8)();
}

TEST(kuznyechik, test_vectors) {
    testsuite::block_test_case("KUZNYECHIK", 10)();
}

TEST(misty1, test_vectors) {
    testsuite::block_test_case("MISTY1", 4)();
}

TEST(noekeon, test_vectors) {
    testsuite::block_test_case("NOEKEON", 16)();
}

TEST(shacal2, test_vectors) {
    testsuite::block_test_case("SHACAL2", 64)();
}

TEST(xtea, test_vectors) {
    testsuite::block_test_case("XTEA", 32)();
}

TEST(chaskey, test_vectors) {
    testsuite::block_test_case("CHASKEY", 8)();
    testsuite::block_test_case("CHASKEY", 16)();
}

TEST(chaskey, round_reduction) {
    testsuite::block_test_case util("CHASKEY", 16);

    for(auto i = 1; i <= 16; i++) {
        util.testRoundReducedEncryptDecrypt(16, 16, i);
    }
}

TEST(fantomas, test_vectors) {
    testsuite::block_test_case("FANTOMAS", 12)();
}

TEST(fantomas, round_reduction) {
    testsuite::block_test_case util("FANTOMAS", 12);

    for(auto i = 1; i <= 12; i++) {
        util.testRoundReducedEncryptDecrypt(16, 16, i);
    }
}

TEST(hight, test_vectors) {
    testsuite::block_test_case("HIGHT", 32)();
}

//TEST(hight, round_reduction) { doesn't work
//    testsuite::block_test_case util("HIGHT", 32);
//
//    for(auto i = 1; i <= 32; i++) {
//        util.testRoundReducedEncryptDecrypt(8, 16, i);
//    }
//}

TEST(lblock, test_vectors) {
    testsuite::block_test_case("LBLOCK", 32)();
}

//TEST(lblock, round_reduction) {
//    testsuite::block_test_case util("LBLOCK", 32);
//
//    for(auto i = 1; i <= 32; i++) {
//        util.testRoundReducedEncryptDecrypt(8, 10, i);
//    }
//}

TEST(lea, test_vectors) {
    testsuite::block_test_case("LEA", 24)();
}

//TEST(lea, round_reduction) {
//    testsuite::block_test_case util("LEA", 24);
//
//    for(auto i = 1; i <= 24; i++) {
//        util.testRoundReducedEncryptDecrypt(16, 16, i);
//    }
//}

TEST(led, test_vectors) {
    testsuite::block_test_case("LED", 48)();
}

TEST(led, round_reduction) {
    testsuite::block_test_case util("LED", 48);

    for(auto i = 1; i <= 48; i++) {
        util.testRoundReducedEncryptDecrypt(8, 10, i);
    }
}

TEST(piccolo, test_vectors) {
    testsuite::block_test_case("PICCOLO", 25)();
}

//TEST(piccolo, round_reduction) {
//    testsuite::block_test_case util("PICCOLO", 25);
//
//    for(auto i = 1; i <= 25; i++) {
//        util.testRoundReducedEncryptDecrypt(8, 10, i);
//    }
//}

TEST(pride, test_vectors) {
    testsuite::block_test_case("PRIDE", 20)();
}

//TEST(pride, round_reduction) {
//    testsuite::block_test_case util("PRIDE", 20);
//
//    for(auto i = 1; i <= 20; i++) {
//        util.testRoundReducedEncryptDecrypt(8, 16, i);
//    }
//}

TEST(prince, test_vectors) {
    testsuite::block_test_case("PRINCE", 12)();
}

TEST(prince, round_reduction) {
    testsuite::block_test_case util("PRINCE", 12);

    for(auto i = 1; i <= 24; i++) {
        util.testRoundReducedEncryptDecrypt(8, 16, i);
    }
}

TEST(rc5_20, test_vectors) {
    testsuite::block_test_case("RC5-20", 20)();
}

TEST(rc5_20, round_reduction) {
    testsuite::block_test_case util("RC5-20", 20);

    for(auto i = 1; i <= 20; i++) {
        util.testRoundReducedEncryptDecrypt(8, 16, i);
    }
}

TEST(rectangle_k80, test_vectors) {
    testsuite::block_test_case("RECTANGLE-K80", 25)();
}

TEST(rectangle_k128, test_vectors) {
    testsuite::block_test_case("RECTANGLE-K128", 25)();
}


//TEST(rectangle_k80, round_reduction) {
//    testsuite::block_test_case util("RECTANGLE-K80", 25);
//
//    for(auto i = 1; i <= 25; i++) {
//        util.testRoundReducedEncryptDecrypt(8, 10, i);
//    }
//}
//
//TEST(rectangle_k128, round_reduction) {
//    testsuite::block_test_case util("RECTANGLE-K128", 25);
//
//    for(auto i = 1; i <= 25; i++) {
//        util.testRoundReducedEncryptDecrypt(8, 16, i);
//    }
//}

TEST(road_runner_k128, test_vectors) {
    testsuite::block_test_case("ROAD-RUNNER-K128", 12)();
}

TEST(road_runner_k80, test_vectors) {
    testsuite::block_test_case("ROAD-RUNNER-K80", 10)();
}


TEST(road_runner_k128, round_reduction) {
    testsuite::block_test_case util("ROAD-RUNNER-K128", 12);

    for(auto i = 1; i <= 12; i++) {
        util.testRoundReducedEncryptDecrypt(8, 16, i);
    }
}

TEST(road_runner_k80, round_reduction) {

    testsuite::block_test_case util("ROAD-RUNNER-K80", 10);

    for(auto i = 1; i <= 10; i++) {
        util.testRoundReducedEncryptDecrypt(8, 10, i);
    }
}

TEST(robin, test_vectors) {
    testsuite::block_test_case("ROBIN", 16)();
}

TEST(robin, round_reduction) {

    testsuite::block_test_case util("ROBIN", 16);

    for(auto i = 1; i <= 16; i++) {
        util.testRoundReducedEncryptDecrypt(16, 16, i);
    }
}

TEST(robin_star, test_vectors) {
    testsuite::block_test_case("ROBIN-STAR", 16)();
}

TEST(robin_star, round_reduction) {

    testsuite::block_test_case util("ROBIN-STAR", 16);

    for(auto i = 1; i <= 16; i++) {
        util.testRoundReducedEncryptDecrypt(16, 16, i);
    }
}

TEST(sparx_b64, test_vectors) {
    testsuite::block_test_case("SPARX-B64", 8)();
}

TEST(sparx_b128, test_vectors) {
    testsuite::block_test_case("SPARX-B128", 8)();
}

TEST(sparx_b64, round_reduction) {
    testsuite::block_test_case util("SPARX-B64", 8);

    for(auto i = 1; i <= 8; i++) {
        util.testRoundReducedEncryptDecrypt(8, 16, i);
    }
}

TEST(sparx_b128, round_reduction) {

    testsuite::block_test_case util("SPARX-B128", 8);

    for(auto i = 1; i <= 8; i++) {
        util.testRoundReducedEncryptDecrypt(16, 16, i);
    }
}

TEST(twine, test_vectors) {
    testsuite::block_test_case("TWINE", 35)();
}

TEST(twine, round_reduction) {

    testsuite::block_test_case util("TWINE", 35);

    for(auto i = 1; i <= 35; i++) {
        util.testRoundReducedEncryptDecrypt(8, 10, i);
    }
}
