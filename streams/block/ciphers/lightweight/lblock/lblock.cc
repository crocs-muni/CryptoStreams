//
// Created by mhajas on 7/8/18.
//

#include "lblock.h"

void block::lblock::keysetup(const std::uint8_t *key, const std::uint64_t keysize) {
    uint16_t shiftedKey[2];
    uint8_t keyCopy[LBLOCK_KEY_SIZE];


    uint16_t *Key = (uint16_t *)key;
    uint32_t *RoundKeys = (uint32_t *)_key;


    uint16_t *KeyCopy = (uint16_t *)keyCopy;


    KeyCopy[4] = Key[4];
    KeyCopy[3] = Key[3];
    KeyCopy[2] = Key[2];
    KeyCopy[1] = Key[1];
    KeyCopy[0] = Key[0];


    /* Set round subkey K(1) */
    RoundKeys[0] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];


    /* Set round subkey K(2) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x00;
    keyCopy[5] = keyCopy[5] ^ 0x40;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[1] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(2) - End */


    /* Set round subkey K(3) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x00;
    keyCopy[5] = keyCopy[5] ^ 0x80;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[2] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(3) - End */


    /* Set round subkey K(4) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x00;
    keyCopy[5] = keyCopy[5] ^ 0xC0;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[3] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(4) - End */


    /* Set round subkey K(5) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x01;
    keyCopy[5] = keyCopy[5] ^ 0x00;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[4] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(5) - End */


    /* Set round subkey K(6) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x01;
    keyCopy[5] = keyCopy[5] ^ 0x40;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[5] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(6) - End */


    /* Set round subkey K(7) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x01;
    keyCopy[5] = keyCopy[5] ^ 0x80;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[6] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(7) - End */


    /* Set round subkey K(8) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x01;
    keyCopy[5] = keyCopy[5] ^ 0xC0;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[7] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(8) - End */


    /* Set round subkey K(9) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];;


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x02;
    keyCopy[5] = keyCopy[5] ^ 0x00;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[8] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(9) - End */


    /* Set round subkey K(10) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x02;
    keyCopy[5] = keyCopy[5] ^ 0x40;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[9] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(10) - End */


    /* Set round subkey K(11) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x02;
    keyCopy[5] = keyCopy[5] ^ 0x80;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[10] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(11) - End */


    /* Set round subkey K(12) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x02;
    keyCopy[5] = keyCopy[5] ^ 0xC0;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[11] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(12) - End */


    /* Set round subkey K(13) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x03;
    keyCopy[5] = keyCopy[5] ^ 0x00;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[12] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(13) - End */


    /* Set round subkey K(14) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x03;
    keyCopy[5] = keyCopy[5] ^ 0x40;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[13] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(14) - End */


    /* Set round subkey K(15) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x03;
    keyCopy[5] = keyCopy[5] ^ 0x80;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[14] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(15) - End */


    /* Set round subkey K(16) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x03;
    keyCopy[5] = keyCopy[5] ^ 0xC0;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[15] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(16) - End */


    /* Set round subkey K(17) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x04;
    keyCopy[5] = keyCopy[5] ^ 0x00;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[16] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(17) - End */


    /* Set round subkey K(18) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x04;
    keyCopy[5] = keyCopy[5] ^ 0x40;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[17] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(18) - End */


    /* Set round subkey K(19) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x04;
    keyCopy[5] = keyCopy[5] ^ 0x80;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[18] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(19) - End */


    /* Set round subkey K(20) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x04;
    keyCopy[5] = keyCopy[5] ^ 0xC0;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[19] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(20) - End */


    /* Set round subkey K(21) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x05;
    keyCopy[5] = keyCopy[5] ^ 0x00;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[20] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(21) - End */


    /* Set round subkey K(22) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x05;
    keyCopy[5] = keyCopy[5] ^ 0x40;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[21] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(22) - End */


    /* Set round subkey K(23) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x05;
    keyCopy[5] = keyCopy[5] ^ 0x80;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[22] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(23) - End */


    /* Set round subkey K(24) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x05;
    keyCopy[5] = keyCopy[5] ^ 0xC0;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[23] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(24) - End */


    /* Set round subkey K(25) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x06;
    keyCopy[5] = keyCopy[5] ^ 0x00;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[24] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(25) - End */


    /* Set round subkey K(26) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x06;
    keyCopy[5] = keyCopy[5] ^ 0x40;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[25] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(26) - End */


    /* Set round subkey K(27) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x06;
    keyCopy[5] = keyCopy[5] ^ 0x80;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[26] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(27) - End */


    /* Set round subkey K(28) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x06;
    keyCopy[5] = keyCopy[5] ^ 0xC0;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[27] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(28) - End */


    /* Set round subkey K(29) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x07;
    keyCopy[5] = keyCopy[5] ^ 0x00;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[28] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(29) - End */


    /* Set round subkey K(30) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x07;
    keyCopy[5] = keyCopy[5] ^ 0x40;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[29] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(30) - End */


    /* Set round subkey K(31) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x07;
    keyCopy[5] = keyCopy[5] ^ 0x80;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[30] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(31) - End */


    /* Set round subkey K(32) - Begin */
    /* (a) K <<< 29 */
    shiftedKey[1] = KeyCopy[4];
    shiftedKey[0] = KeyCopy[3];


    KeyCopy[4] = (KeyCopy[3] << 13) ^ (KeyCopy[2] >> 3);
    KeyCopy[3] = (KeyCopy[2] << 13) ^ (KeyCopy[1] >> 3);
    KeyCopy[2] = (KeyCopy[1] << 13) ^ (KeyCopy[0] >> 3);
    KeyCopy[1] = (KeyCopy[0] << 13) ^ (shiftedKey[1] >> 3);
    KeyCopy[0] = (shiftedKey[1] << 13) ^ (shiftedKey[0] >> 3);


    /* (b) S-boxes */
    keyCopy[9] = (READ_SBOX_BYTE(S9[keyCopy[9] >> 4]) << 4) ^ READ_SBOX_BYTE(S8[(keyCopy[9] & 0x0F)]);


    /* (c) XOR */
    keyCopy[6] = keyCopy[6] ^ 0x07;
    keyCopy[5] = keyCopy[5] ^ 0xC0;


    /* (d) Set the round subkey K(i+1) */
    RoundKeys[31] = (((uint32_t)KeyCopy[4]) << 16) ^ (uint32_t)KeyCopy[3];
    /* Set round subkey K(32) - End */}

void block::lblock::Encrypt(uint8_t *block) {
    uint8_t temp[4];
    uint8_t p[4];


    uint8_t *x = block;
    uint8_t *k = _key;


    uint32_t *X = (uint32_t *)x;
    uint32_t *K = (uint32_t *)k;

    uint32_t *Temp = (uint32_t *)temp;


    /* Save a copy of the left half of X */
    Temp[0] = X[1];


    /* Round 1 - Begin */
    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[0]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 1 - End */

    /* Round 2 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[1]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 2 - End */


    /* Round 3 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[2]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 3 - End */

    /* Round 4 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[3]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 4 - End */


    /* Round 5 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[4]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 5 - End */

    /* Round 6 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[5]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 6 - End */


    /* Round 7 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[6]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 7 - End */

    /* Round 8 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[7]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 8 - End */


    /* Round 9 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[8]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 9 - End */

    /* Round 10 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[9]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 10 - End */


    /* Round 11 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[10]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 11 - End */

    /* Round 12 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[11]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 12 - End */


    /* Round 13 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[12]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 13 - End */

    /* Round 14 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[13]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 14 - End */


    /* Round 15 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[14]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 15 - End */

    /* Round 16 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[15]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 16 - End */


    /* Round 17 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */;
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[16]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 17 - End */

    /* Round 18 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[17]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 18 - End */


    /* Round 19 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[18]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 19 - End */

    /* Round 20 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[19]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 20 - End */


    /* Round 21 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[20]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 21 - End */

    /* Round 22 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[21]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 22 - End */


    /* Round 23 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[22]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 23 - End */

    /* Round 24 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[23]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 24 - End */


    /* Round 25 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[24]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 25 - End */

    /* Round 26 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[25]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 26 - End */


    /* Round 27 Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[26]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 27 - End */

    /* Round 28 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[27]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 28 - End */


    /* Round 29 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[28]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 29 - End */

    /* Round 30 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[29]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 30 - End */


    /* Round 31 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];

    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[30]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[2];
    temp[2] = p[2] ^ x[1];
    temp[1] = p[1] ^ x[0];
    temp[0] = p[0] ^ x[3];
    /* Round 31 - End */

    /* Round 32 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];

    /* XOR X left half with the round key: X XOR K(i) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[31]);


    /* (2) Confusion function S: S(X XOR K(i)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(i))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(i-1), K(i-1)) XOR (X(i-2) <<< 8) */
    temp[3] = p[3] ^ x[6];
    temp[2] = p[2] ^ x[5];
    temp[1] = p[1] ^ x[4];
    temp[0] = p[0] ^ x[7];
    /* Round 32 - End */


    X[1] = X[0];

    X[0] = Temp[0];
}

void block::lblock::Decrypt(uint8_t *block) {
    uint8_t temp[4];
    uint8_t p[4];


    uint8_t *x = block;
    uint8_t *k = _key;


    uint32_t *X = (uint32_t *)x;
    uint32_t *K = (uint32_t *)k;

    uint32_t *Temp = (uint32_t *)temp;


    /* Save a copy of the left half of X */
    Temp[0] = X[1];


    /* Round 1 - Begin */
    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[31]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 1 - End */

    /* Round 2 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[30]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 2 - End */


    /* Round 3 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[29]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 3 - End */

    /* Round 4 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[28]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 4 - End */


    /* Round 5 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[27]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 5 - End */

    /* Round 6 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[26]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 6 - End */


    /* Round 7 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[25]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 7 - End */

    /* Round 8 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[24]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 8 - End */


    /* Round 9 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[23]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 9 - End */

    /* Round 10 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[22]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 10 - End */


    /* Round 11 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[21]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 11 - End */

    /* Round 12 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[20]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 12 - End */


    /* Round 13 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[19]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 13 - End */

    /* Round 14 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[18]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 14 - End */


    /* Round 15 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[17]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 15 - End */

    /* Round 16 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[16]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 16 - End */


    /* Round 17 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[15]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 17 - End */

    /* Round 18 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[14]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 18 - End */


    /* Round 19 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[13]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 19 - End */

    /* Round 20 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[12]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 20 - End */


    /* Round 21 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[11]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 21 - End */

    /* Round 22 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[10]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 22 - End */


    /* Round 23 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[9]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 23 - End */

    /* Round 24 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[8]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 24 - End */


    /* Round 25 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[7]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 25 - End */

    /* Round 26 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[6]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 26 - End */


    /* Round 27 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[5]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 27 - End */

    /* Round 28 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[4]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 28 - End */


    /* Round 29 - Begin */
    /* Save a copy of the left half of X */
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[3]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 29 - End */

    /* Round 30 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[2]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 30 - End */


    /* Round 31 - Begin */
    /* Save a copy of the left half of X */;
    X[1] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[1]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[0] ^ p[0];
    temp[2] = x[3] ^ p[3];
    temp[1] = x[2] ^ p[2];
    temp[0] = x[1] ^ p[1];
    /* Round 31 - End */

    /* Round 32 - Begin */
    /* Save a copy of the left half of X */
    X[0] = Temp[0];


    /* XOR X left half with the round key: X XOR K(j) */
    Temp[0] = Temp[0] ^ READ_ROUND_KEY_DOUBLE_WORD(K[0]);


    /* (2) Confusion function S: S(X XOR K(j)) */
    temp[3] = (READ_SBOX_BYTE(S7[temp[3] >> 4]) << 4) ^ READ_SBOX_BYTE(S6[temp[3] & 0x0F]);
    temp[2] = (READ_SBOX_BYTE(S5[temp[2] >> 4]) << 4) ^ READ_SBOX_BYTE(S4[temp[2] & 0x0F]);
    temp[1] = (READ_SBOX_BYTE(S3[temp[1] >> 4]) << 4) ^ READ_SBOX_BYTE(S2[temp[1] & 0x0F]);
    temp[0] = (READ_SBOX_BYTE(S1[temp[0] >> 4]) << 4) ^ READ_SBOX_BYTE(S0[temp[0] & 0x0F]);


    /* (3) Diffusion function P: P(S(X XOR K(j))) */
    p[3] = (temp[3] << 4) ^ (temp[2] & 0x0F);
    p[2] = (temp[3] & 0xF0) ^ (temp[2] >> 4);
    p[1] = (temp[1] << 4) ^ (temp[0] & 0x0F);
    p[0] = (temp[1] & 0xF0) ^ (temp[0] >> 4);


    /* F(X(j+1), K(j+1)) XOR X(j+2)) >>> 8) */
    temp[3] = x[4] ^ p[0];
    temp[2] = x[7] ^ p[3];
    temp[1] = x[6] ^ p[2];
    temp[0] = x[5] ^ p[1];
    /* Round 32 - End */


    X[1] = X[0];

    X[0] = Temp[0];

}
