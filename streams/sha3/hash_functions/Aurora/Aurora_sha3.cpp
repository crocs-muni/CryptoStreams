#include "Aurora_sha3.h"


/*--- compile options ---*/
/*
#define _USE_ROT
#define _LOOPED
*/


/*
 * Usage
 *
 * input  : data (databitlen [bits])
 *          databitlen (bit length of data)
 * output : hashval
 *
 * ----- AURORA-224 -----
 * hashState st;
 * Init(&st, 224); -> Update(&st, data, databitlen); -> Final(&st, hashval);
 * or
 * Hash(224, data, databitlen, hashval);
 *
 * ----- AURORA-256 -----
 * hashState st;
 * Init(&st, 256); -> Update(&st, data, databitlen); -> Final(&st, hashval);
 * or
 * Hash(256, data, databitlen, hashval);
 *
 * ----- AURORA-384 -----
 * hashState st;
 * Init(&st, 384); -> Update(&st, data, databitlen); -> Final(&st, hashval);
 * or
 * Hash(384, data, databitlen, hashval);
 *
 * ----- AURORA-512 -----
 * hashState st;
 * Init(&st, 512); -> Update(&st, data, databitlen); -> Final(&st, hashval);
 * or
 * Hash(512, data, databitlen, hashval);
 */

/* T-box for AURORA */

/* AURORA S-box, cir(1223), pol = 1b */
const AURORA_UINT32 aurora_sm00[256] = {
  0xd970a9a9U,0xdc7fa3a3U,0xd36ebdbdU,0x69bbd2d2U,0xbddc6161U,0x00000000U,0x4dd79a9aU,0xeb26cdcdU,
  0x02060404U,0x246c4848U,0x57f9aeaeU,0xc25d9f9fU,0xb8d36b6bU,0x5de7babaU,0xb7c27575U,0x6db7dadaU,
  0xf504f1f1U,0x40c08080U,0x37596e6eU,0x4ed29c9cU,0x192b3232U,0xd873ababU,0x64acc8c8U,0x62a6c4c4U,
  0x9dbc2121U,0x345c6868U,0x0f111e1eU,0x7c84f8f8U,0xec2fc3c3U,0xce498787U,0x94a73333U,0x040c0808U,
  0xd168b9b9U,0x8a850f0fU,0x749ce8e8U,0xfb16ededU,0xe732d5d5U,0x87921515U,0x12362424U,0x23654646U,
  0xb5c47171U,0x5ce4b8b8U,0x1a2e3434U,0xbbd66d6dU,0x42c68484U,0x49db9292U,0x18283030U,0x85941111U,
  0x11332222U,0x46ca8c8cU,0x0d171a1aU,0x7193e2e2U,0x67a9ceceU,0x8f8a0505U,0xc6519797U,0x50f0a0a0U,
  0x58e8b0b0U,0xfd1ce1e1U,0x4bdd9696U,0xa4f75353U,0xcd4c8181U,0x8e890707U,0x99b02929U,0x1f213e3eU,
  0xadec4141U,0x63a5c6c6U,0xc9408989U,0x6bbdd6d6U,0xf702f5f5U,0x28785050U,0x9fba2525U,0x65afcacaU,
  0x2f715e5eU,0x5fe1bebeU,0x61a3c2c2U,0x7395e6e6U,0x3d477a7aU,0x8b860d0dU,0x0e121c1cU,0x1b2d3636U,
  0x33556666U,0xe03bdbdbU,0xacef4343U,0x266a4c4cU,0xa1f85959U,0xe33eddddU,0xf30efdfdU,0x829d1f1fU,
  0x839e1d1dU,0x759feaeaU,0x44cc8888U,0x90ab3b3bU,0x13352626U,0xafea4545U,0xf00bfbfbU,0x07090e0eU,
  0x96a13737U,0x21634242U,0xf813ebebU,0x3f417e7eU,0xa2fd5f5fU,0x98b32b2bU,0x9ab52f2fU,0xa3fe5d5dU,
  0x91a83939U,0x4cd49898U,0x7f81fefeU,0x92ad3f3fU,0x97a23535U,0xea25cfcfU,0x01030202U,0x1c243838U,
  0x1e223c3cU,0x2d775a5aU,0x89800909U,0x394b7272U,0xe631d7d7U,0x9cbf2323U,0x0a1e1414U,0x54fca8a8U,
  0x0c141818U,0x51f3a2a2U,0x6cb4d8d8U,0x43c58686U,0xaee94747U,0xdb76adadU,0x53f5a6a6U,0x59ebb2b2U,
  0xa6f15757U,0xf407f3f3U,0x060a0c0cU,0xda75afafU,0xe23ddfdfU,0x7888f0f0U,0x1d273a3aU,0x297b5252U,
  0x30506060U,0xe138d9d9U,0x355f6a6aU,0xfc1fe3e3U,0xed2cc1c1U,0xbcdf6363U,0x47c98e8eU,0xd564b1b1U,
  0xc05b9b9bU,0xabe64d4dU,0xcc4f8383U,0xa8e34b4bU,0x809b1b1bU,0x2b7d5656U,0x091b1212U,0xb0cb7b7bU,
  0x93ae3d3dU,0xd467b3b3U,0xc5549191U,0xb3ce7d7dU,0xd06bbbbbU,0xdf7aa5a5U,0xa9e04949U,0xaae54f4fU,
  0x7a8ef4f4U,0x365a6c6cU,0x2a7e5454U,0xd661b7b7U,0xb2cd7f7fU,0xfa15efefU,0xe823cbcbU,0xb1c87979U,
  0xa0fb5b5bU,0x68b8d0d0U,0x5aeeb4b4U,0x81981919U,0x48d89090U,0x08181010U,0x17392e2eU,0xc7529595U,
  0xfe19e7e7U,0x769aececU,0xbfda6565U,0xc4579393U,0xf20dffffU,0x3e427c7cU,0x4ade9494U,0x0b1d1616U,
  0x10302020U,0x143c2828U,0xf108f9f9U,0xef2ac5c5U,0xa7f25555U,0x27694e4eU,0xe534d1d1U,0xc8438b8bU,
  0xde79a7a7U,0x9bb62d2dU,0x8d8c0101U,0x3c447878U,0x56faacacU,0xd762b5b5U,0x8c8f0303U,0x60a0c0c0U,
  0x6abed4d4U,0x798bf2f2U,0xee29c7c7U,0xa5f45151U,0x31536262U,0x2e725c5cU,0x7799eeeeU,0x41c38282U,
  0xff1ae5e5U,0x95a43131U,0xdd7ca1a1U,0x256f4a4aU,0x3b4d7676U,0x55ffaaaaU,0xca458f8fU,0x52f6a4a4U,
  0x9eb92727U,0x2c745858U,0x153f2a2aU,0x4fd19e9eU,0xe437d3d3U,0x163a2c2cU,0x7090e0e0U,0x7d87fafaU,
  0x7296e4e4U,0x3a4e7474U,0x7b8df6f6U,0x84971313U,0xf601f7f7U,0x32566464U,0x86911717U,0x03050606U,
  0xb4c77373U,0x38487070U,0x6fb1dedeU,0xb9d06969U,0xc1589999U,0x45cf8a8aU,0x88830b0bU,0xe920c9c9U,
  0xbad56f6fU,0xb6c17777U,0x6eb2dcdcU,0x5ee2bcbcU,0xbed96767U,0x7e82fcfcU,0x20604040U,0xf910e9e9U,
  0x22664444U,0x66aaccccU,0x050f0a0aU,0xd26dbfbfU,0xcb468d8dU,0xc35e9d9dU,0xcf4a8585U,0x5bedb6b6U
};

#ifndef _USE_ROT

const AURORA_UINT32 aurora_sm01[256] = {
  0xa9d970a9U,0xa3dc7fa3U,0xbdd36ebdU,0xd269bbd2U,0x61bddc61U,0x00000000U,0x9a4dd79aU,0xcdeb26cdU,
  0x04020604U,0x48246c48U,0xae57f9aeU,0x9fc25d9fU,0x6bb8d36bU,0xba5de7baU,0x75b7c275U,0xda6db7daU,
  0xf1f504f1U,0x8040c080U,0x6e37596eU,0x9c4ed29cU,0x32192b32U,0xabd873abU,0xc864acc8U,0xc462a6c4U,
  0x219dbc21U,0x68345c68U,0x1e0f111eU,0xf87c84f8U,0xc3ec2fc3U,0x87ce4987U,0x3394a733U,0x08040c08U,
  0xb9d168b9U,0x0f8a850fU,0xe8749ce8U,0xedfb16edU,0xd5e732d5U,0x15879215U,0x24123624U,0x46236546U,
  0x71b5c471U,0xb85ce4b8U,0x341a2e34U,0x6dbbd66dU,0x8442c684U,0x9249db92U,0x30182830U,0x11859411U,
  0x22113322U,0x8c46ca8cU,0x1a0d171aU,0xe27193e2U,0xce67a9ceU,0x058f8a05U,0x97c65197U,0xa050f0a0U,
  0xb058e8b0U,0xe1fd1ce1U,0x964bdd96U,0x53a4f753U,0x81cd4c81U,0x078e8907U,0x2999b029U,0x3e1f213eU,
  0x41adec41U,0xc663a5c6U,0x89c94089U,0xd66bbdd6U,0xf5f702f5U,0x50287850U,0x259fba25U,0xca65afcaU,
  0x5e2f715eU,0xbe5fe1beU,0xc261a3c2U,0xe67395e6U,0x7a3d477aU,0x0d8b860dU,0x1c0e121cU,0x361b2d36U,
  0x66335566U,0xdbe03bdbU,0x43acef43U,0x4c266a4cU,0x59a1f859U,0xdde33eddU,0xfdf30efdU,0x1f829d1fU,
  0x1d839e1dU,0xea759feaU,0x8844cc88U,0x3b90ab3bU,0x26133526U,0x45afea45U,0xfbf00bfbU,0x0e07090eU,
  0x3796a137U,0x42216342U,0xebf813ebU,0x7e3f417eU,0x5fa2fd5fU,0x2b98b32bU,0x2f9ab52fU,0x5da3fe5dU,
  0x3991a839U,0x984cd498U,0xfe7f81feU,0x3f92ad3fU,0x3597a235U,0xcfea25cfU,0x02010302U,0x381c2438U,
  0x3c1e223cU,0x5a2d775aU,0x09898009U,0x72394b72U,0xd7e631d7U,0x239cbf23U,0x140a1e14U,0xa854fca8U,
  0x180c1418U,0xa251f3a2U,0xd86cb4d8U,0x8643c586U,0x47aee947U,0xaddb76adU,0xa653f5a6U,0xb259ebb2U,
  0x57a6f157U,0xf3f407f3U,0x0c060a0cU,0xafda75afU,0xdfe23ddfU,0xf07888f0U,0x3a1d273aU,0x52297b52U,
  0x60305060U,0xd9e138d9U,0x6a355f6aU,0xe3fc1fe3U,0xc1ed2cc1U,0x63bcdf63U,0x8e47c98eU,0xb1d564b1U,
  0x9bc05b9bU,0x4dabe64dU,0x83cc4f83U,0x4ba8e34bU,0x1b809b1bU,0x562b7d56U,0x12091b12U,0x7bb0cb7bU,
  0x3d93ae3dU,0xb3d467b3U,0x91c55491U,0x7db3ce7dU,0xbbd06bbbU,0xa5df7aa5U,0x49a9e049U,0x4faae54fU,
  0xf47a8ef4U,0x6c365a6cU,0x542a7e54U,0xb7d661b7U,0x7fb2cd7fU,0xeffa15efU,0xcbe823cbU,0x79b1c879U,
  0x5ba0fb5bU,0xd068b8d0U,0xb45aeeb4U,0x19819819U,0x9048d890U,0x10081810U,0x2e17392eU,0x95c75295U,
  0xe7fe19e7U,0xec769aecU,0x65bfda65U,0x93c45793U,0xfff20dffU,0x7c3e427cU,0x944ade94U,0x160b1d16U,
  0x20103020U,0x28143c28U,0xf9f108f9U,0xc5ef2ac5U,0x55a7f255U,0x4e27694eU,0xd1e534d1U,0x8bc8438bU,
  0xa7de79a7U,0x2d9bb62dU,0x018d8c01U,0x783c4478U,0xac56faacU,0xb5d762b5U,0x038c8f03U,0xc060a0c0U,
  0xd46abed4U,0xf2798bf2U,0xc7ee29c7U,0x51a5f451U,0x62315362U,0x5c2e725cU,0xee7799eeU,0x8241c382U,
  0xe5ff1ae5U,0x3195a431U,0xa1dd7ca1U,0x4a256f4aU,0x763b4d76U,0xaa55ffaaU,0x8fca458fU,0xa452f6a4U,
  0x279eb927U,0x582c7458U,0x2a153f2aU,0x9e4fd19eU,0xd3e437d3U,0x2c163a2cU,0xe07090e0U,0xfa7d87faU,
  0xe47296e4U,0x743a4e74U,0xf67b8df6U,0x13849713U,0xf7f601f7U,0x64325664U,0x17869117U,0x06030506U,
  0x73b4c773U,0x70384870U,0xde6fb1deU,0x69b9d069U,0x99c15899U,0x8a45cf8aU,0x0b88830bU,0xc9e920c9U,
  0x6fbad56fU,0x77b6c177U,0xdc6eb2dcU,0xbc5ee2bcU,0x67bed967U,0xfc7e82fcU,0x40206040U,0xe9f910e9U,
  0x44226644U,0xcc66aaccU,0x0a050f0aU,0xbfd26dbfU,0x8dcb468dU,0x9dc35e9dU,0x85cf4a85U,0xb65bedb6U
};

const AURORA_UINT32 aurora_sm02[256] = {
  0xa9a9d970U,0xa3a3dc7fU,0xbdbdd36eU,0xd2d269bbU,0x6161bddcU,0x00000000U,0x9a9a4dd7U,0xcdcdeb26U,
  0x04040206U,0x4848246cU,0xaeae57f9U,0x9f9fc25dU,0x6b6bb8d3U,0xbaba5de7U,0x7575b7c2U,0xdada6db7U,
  0xf1f1f504U,0x808040c0U,0x6e6e3759U,0x9c9c4ed2U,0x3232192bU,0xababd873U,0xc8c864acU,0xc4c462a6U,
  0x21219dbcU,0x6868345cU,0x1e1e0f11U,0xf8f87c84U,0xc3c3ec2fU,0x8787ce49U,0x333394a7U,0x0808040cU,
  0xb9b9d168U,0x0f0f8a85U,0xe8e8749cU,0xededfb16U,0xd5d5e732U,0x15158792U,0x24241236U,0x46462365U,
  0x7171b5c4U,0xb8b85ce4U,0x34341a2eU,0x6d6dbbd6U,0x848442c6U,0x929249dbU,0x30301828U,0x11118594U,
  0x22221133U,0x8c8c46caU,0x1a1a0d17U,0xe2e27193U,0xcece67a9U,0x05058f8aU,0x9797c651U,0xa0a050f0U,
  0xb0b058e8U,0xe1e1fd1cU,0x96964bddU,0x5353a4f7U,0x8181cd4cU,0x07078e89U,0x292999b0U,0x3e3e1f21U,
  0x4141adecU,0xc6c663a5U,0x8989c940U,0xd6d66bbdU,0xf5f5f702U,0x50502878U,0x25259fbaU,0xcaca65afU,
  0x5e5e2f71U,0xbebe5fe1U,0xc2c261a3U,0xe6e67395U,0x7a7a3d47U,0x0d0d8b86U,0x1c1c0e12U,0x36361b2dU,
  0x66663355U,0xdbdbe03bU,0x4343acefU,0x4c4c266aU,0x5959a1f8U,0xdddde33eU,0xfdfdf30eU,0x1f1f829dU,
  0x1d1d839eU,0xeaea759fU,0x888844ccU,0x3b3b90abU,0x26261335U,0x4545afeaU,0xfbfbf00bU,0x0e0e0709U,
  0x373796a1U,0x42422163U,0xebebf813U,0x7e7e3f41U,0x5f5fa2fdU,0x2b2b98b3U,0x2f2f9ab5U,0x5d5da3feU,
  0x393991a8U,0x98984cd4U,0xfefe7f81U,0x3f3f92adU,0x353597a2U,0xcfcfea25U,0x02020103U,0x38381c24U,
  0x3c3c1e22U,0x5a5a2d77U,0x09098980U,0x7272394bU,0xd7d7e631U,0x23239cbfU,0x14140a1eU,0xa8a854fcU,
  0x18180c14U,0xa2a251f3U,0xd8d86cb4U,0x868643c5U,0x4747aee9U,0xadaddb76U,0xa6a653f5U,0xb2b259ebU,
  0x5757a6f1U,0xf3f3f407U,0x0c0c060aU,0xafafda75U,0xdfdfe23dU,0xf0f07888U,0x3a3a1d27U,0x5252297bU,
  0x60603050U,0xd9d9e138U,0x6a6a355fU,0xe3e3fc1fU,0xc1c1ed2cU,0x6363bcdfU,0x8e8e47c9U,0xb1b1d564U,
  0x9b9bc05bU,0x4d4dabe6U,0x8383cc4fU,0x4b4ba8e3U,0x1b1b809bU,0x56562b7dU,0x1212091bU,0x7b7bb0cbU,
  0x3d3d93aeU,0xb3b3d467U,0x9191c554U,0x7d7db3ceU,0xbbbbd06bU,0xa5a5df7aU,0x4949a9e0U,0x4f4faae5U,
  0xf4f47a8eU,0x6c6c365aU,0x54542a7eU,0xb7b7d661U,0x7f7fb2cdU,0xefeffa15U,0xcbcbe823U,0x7979b1c8U,
  0x5b5ba0fbU,0xd0d068b8U,0xb4b45aeeU,0x19198198U,0x909048d8U,0x10100818U,0x2e2e1739U,0x9595c752U,
  0xe7e7fe19U,0xecec769aU,0x6565bfdaU,0x9393c457U,0xfffff20dU,0x7c7c3e42U,0x94944adeU,0x16160b1dU,
  0x20201030U,0x2828143cU,0xf9f9f108U,0xc5c5ef2aU,0x5555a7f2U,0x4e4e2769U,0xd1d1e534U,0x8b8bc843U,
  0xa7a7de79U,0x2d2d9bb6U,0x01018d8cU,0x78783c44U,0xacac56faU,0xb5b5d762U,0x03038c8fU,0xc0c060a0U,
  0xd4d46abeU,0xf2f2798bU,0xc7c7ee29U,0x5151a5f4U,0x62623153U,0x5c5c2e72U,0xeeee7799U,0x828241c3U,
  0xe5e5ff1aU,0x313195a4U,0xa1a1dd7cU,0x4a4a256fU,0x76763b4dU,0xaaaa55ffU,0x8f8fca45U,0xa4a452f6U,
  0x27279eb9U,0x58582c74U,0x2a2a153fU,0x9e9e4fd1U,0xd3d3e437U,0x2c2c163aU,0xe0e07090U,0xfafa7d87U,
  0xe4e47296U,0x74743a4eU,0xf6f67b8dU,0x13138497U,0xf7f7f601U,0x64643256U,0x17178691U,0x06060305U,
  0x7373b4c7U,0x70703848U,0xdede6fb1U,0x6969b9d0U,0x9999c158U,0x8a8a45cfU,0x0b0b8883U,0xc9c9e920U,
  0x6f6fbad5U,0x7777b6c1U,0xdcdc6eb2U,0xbcbc5ee2U,0x6767bed9U,0xfcfc7e82U,0x40402060U,0xe9e9f910U,
  0x44442266U,0xcccc66aaU,0x0a0a050fU,0xbfbfd26dU,0x8d8dcb46U,0x9d9dc35eU,0x8585cf4aU,0xb6b65bedU
};

const AURORA_UINT32 aurora_sm03[256] = {
  0x70a9a9d9U,0x7fa3a3dcU,0x6ebdbdd3U,0xbbd2d269U,0xdc6161bdU,0x00000000U,0xd79a9a4dU,0x26cdcdebU,
  0x06040402U,0x6c484824U,0xf9aeae57U,0x5d9f9fc2U,0xd36b6bb8U,0xe7baba5dU,0xc27575b7U,0xb7dada6dU,
  0x04f1f1f5U,0xc0808040U,0x596e6e37U,0xd29c9c4eU,0x2b323219U,0x73ababd8U,0xacc8c864U,0xa6c4c462U,
  0xbc21219dU,0x5c686834U,0x111e1e0fU,0x84f8f87cU,0x2fc3c3ecU,0x498787ceU,0xa7333394U,0x0c080804U,
  0x68b9b9d1U,0x850f0f8aU,0x9ce8e874U,0x16ededfbU,0x32d5d5e7U,0x92151587U,0x36242412U,0x65464623U,
  0xc47171b5U,0xe4b8b85cU,0x2e34341aU,0xd66d6dbbU,0xc6848442U,0xdb929249U,0x28303018U,0x94111185U,
  0x33222211U,0xca8c8c46U,0x171a1a0dU,0x93e2e271U,0xa9cece67U,0x8a05058fU,0x519797c6U,0xf0a0a050U,
  0xe8b0b058U,0x1ce1e1fdU,0xdd96964bU,0xf75353a4U,0x4c8181cdU,0x8907078eU,0xb0292999U,0x213e3e1fU,
  0xec4141adU,0xa5c6c663U,0x408989c9U,0xbdd6d66bU,0x02f5f5f7U,0x78505028U,0xba25259fU,0xafcaca65U,
  0x715e5e2fU,0xe1bebe5fU,0xa3c2c261U,0x95e6e673U,0x477a7a3dU,0x860d0d8bU,0x121c1c0eU,0x2d36361bU,
  0x55666633U,0x3bdbdbe0U,0xef4343acU,0x6a4c4c26U,0xf85959a1U,0x3edddde3U,0x0efdfdf3U,0x9d1f1f82U,
  0x9e1d1d83U,0x9feaea75U,0xcc888844U,0xab3b3b90U,0x35262613U,0xea4545afU,0x0bfbfbf0U,0x090e0e07U,
  0xa1373796U,0x63424221U,0x13ebebf8U,0x417e7e3fU,0xfd5f5fa2U,0xb32b2b98U,0xb52f2f9aU,0xfe5d5da3U,
  0xa8393991U,0xd498984cU,0x81fefe7fU,0xad3f3f92U,0xa2353597U,0x25cfcfeaU,0x03020201U,0x2438381cU,
  0x223c3c1eU,0x775a5a2dU,0x80090989U,0x4b727239U,0x31d7d7e6U,0xbf23239cU,0x1e14140aU,0xfca8a854U,
  0x1418180cU,0xf3a2a251U,0xb4d8d86cU,0xc5868643U,0xe94747aeU,0x76adaddbU,0xf5a6a653U,0xebb2b259U,
  0xf15757a6U,0x07f3f3f4U,0x0a0c0c06U,0x75afafdaU,0x3ddfdfe2U,0x88f0f078U,0x273a3a1dU,0x7b525229U,
  0x50606030U,0x38d9d9e1U,0x5f6a6a35U,0x1fe3e3fcU,0x2cc1c1edU,0xdf6363bcU,0xc98e8e47U,0x64b1b1d5U,
  0x5b9b9bc0U,0xe64d4dabU,0x4f8383ccU,0xe34b4ba8U,0x9b1b1b80U,0x7d56562bU,0x1b121209U,0xcb7b7bb0U,
  0xae3d3d93U,0x67b3b3d4U,0x549191c5U,0xce7d7db3U,0x6bbbbbd0U,0x7aa5a5dfU,0xe04949a9U,0xe54f4faaU,
  0x8ef4f47aU,0x5a6c6c36U,0x7e54542aU,0x61b7b7d6U,0xcd7f7fb2U,0x15efeffaU,0x23cbcbe8U,0xc87979b1U,
  0xfb5b5ba0U,0xb8d0d068U,0xeeb4b45aU,0x98191981U,0xd8909048U,0x18101008U,0x392e2e17U,0x529595c7U,
  0x19e7e7feU,0x9aecec76U,0xda6565bfU,0x579393c4U,0x0dfffff2U,0x427c7c3eU,0xde94944aU,0x1d16160bU,
  0x30202010U,0x3c282814U,0x08f9f9f1U,0x2ac5c5efU,0xf25555a7U,0x694e4e27U,0x34d1d1e5U,0x438b8bc8U,
  0x79a7a7deU,0xb62d2d9bU,0x8c01018dU,0x4478783cU,0xfaacac56U,0x62b5b5d7U,0x8f03038cU,0xa0c0c060U,
  0xbed4d46aU,0x8bf2f279U,0x29c7c7eeU,0xf45151a5U,0x53626231U,0x725c5c2eU,0x99eeee77U,0xc3828241U,
  0x1ae5e5ffU,0xa4313195U,0x7ca1a1ddU,0x6f4a4a25U,0x4d76763bU,0xffaaaa55U,0x458f8fcaU,0xf6a4a452U,
  0xb927279eU,0x7458582cU,0x3f2a2a15U,0xd19e9e4fU,0x37d3d3e4U,0x3a2c2c16U,0x90e0e070U,0x87fafa7dU,
  0x96e4e472U,0x4e74743aU,0x8df6f67bU,0x97131384U,0x01f7f7f6U,0x56646432U,0x91171786U,0x05060603U,
  0xc77373b4U,0x48707038U,0xb1dede6fU,0xd06969b9U,0x589999c1U,0xcf8a8a45U,0x830b0b88U,0x20c9c9e9U,
  0xd56f6fbaU,0xc17777b6U,0xb2dcdc6eU,0xe2bcbc5eU,0xd96767beU,0x82fcfc7eU,0x60404020U,0x10e9e9f9U,
  0x66444422U,0xaacccc66U,0x0f0a0a05U,0x6dbfbfd2U,0x468d8dcbU,0x5e9d9dc3U,0x4a8585cfU,0xedb6b65bU
};

#endif

/* AURORA S-box, cir(1682), pol = 1b */
const AURORA_UINT32 aurora_sm10[256] = {
  0xd9a992e0U,0xdca3bafeU,0xd3bdc2dcU,0x69d2656dU,0xbd619fa3U,0x00000000U,0x4d9a5eb5U,0xebcd194cU,
  0x0204100cU,0x24483bd8U,0x57ae8ee9U,0xc29f4abaU,0xb86bb7bdU,0x5dbaded5U,0xb775cf9fU,0x6dda4575U,
  0xf5f1e908U,0x4080369bU,0x376ea3b2U,0x4e9c46bfU,0x1932c856U,0xd8ab9ae6U,0x64c80d43U,0x62c43d57U,
  0x9d218463U,0x3468bbb8U,0x0f1e7822U,0x7cf8cd13U,0xecc3215eU,0xce872a92U,0x9433cc55U,0x04082018U,
  0xd1b9d2d0U,0x8a0f3c11U,0x74e88d23U,0xfbed992cU,0xe7d57964U,0x8715543fU,0x1224906cU,0x234603caU,
  0xb571df93U,0x5cb8d6d3U,0x1a34d05cU,0xbb6dafb7U,0x42842697U,0x49927eadU,0x1830c050U,0x85114433U,
  0x11228866U,0x468c068fU,0x0d1a682eU,0x71e2a53dU,0x67ce1549U,0x8f05140fU,0xc6976aa2U,0x50a0b6fbU,
  0x58b0f6cbU,0xfde1a938U,0x4b966ea1U,0xa45357f5U,0xcd813298U,0x8e071c09U,0x9929a47bU,0x1f3ef842U,
  0xad411fc3U,0x63c63551U,0xc9891280U,0x6bd67561U,0xf7f5f904U,0x28505bf0U,0x9f25946fU,0x65ca0545U,
  0x2f5e63e2U,0x5fbeced9U,0x61c2255dU,0x73e6b531U,0x3d7af38eU,0x8b0d3417U,0x0e1c7024U,0x1b36d85aU,
  0x336683aaU,0xe0db4176U,0xac4317c5U,0x264c2bd4U,0xa1597febU,0xe3dd597cU,0xf3fdd91cU,0x821f7c21U,
  0x831d7427U,0x75ea8525U,0x44881683U,0x903bec4dU,0x1326986aU,0xaf450fcfU,0xf0fbc116U,0x070e3812U,
  0x9637dc59U,0x214213c6U,0xf8eb8126U,0x3f7ee382U,0xa25f67e1U,0x982bac7dU,0x9a2fbc71U,0xa35d6fe7U,
  0x9139e44bU,0x4c9856b3U,0x7ffed519U,0x923ffc41U,0x9735d45fU,0xeacf114aU,0x01020806U,0x1c38e048U,
  0x1e3cf044U,0x2d5a73eeU,0x8909241bU,0x3972d396U,0xe6d77162U,0x9c238c65U,0x0a14503cU,0x54a896e3U,
  0x0c186028U,0x51a2befdU,0x6cd84d73U,0x43862e91U,0xae4707c9U,0xdbad82ecU,0x53a6aef1U,0x59b2fecdU,
  0xa65747f9U,0xf4f3e10eU,0x060c3014U,0xdaaf8aeaU,0xe2df517aU,0x78f0ed0bU,0x1d3ae84eU,0x295253f6U,
  0x30609ba0U,0xe1d94970U,0x356ab3beU,0xfce3a13eU,0xedc12958U,0xbc6397a5U,0x478e0e89U,0xd5b1f2c8U,
  0xc09b5ab6U,0xab4d2fd7U,0xcc833a9eU,0xa84b37ddU,0x801b6c2dU,0x2b5643faU,0x09124836U,0xb07bf78dU,
  0x933df447U,0xd4b3faceU,0xc59172a8U,0xb37def87U,0xd0bbdad6U,0xdfa5a2f4U,0xa9493fdbU,0xaa4f27d1U,
  0x7af4fd07U,0x366cabb4U,0x2a544bfcU,0xd6b7eac2U,0xb27fe781U,0xfaef912aU,0xe8cb0146U,0xb179ff8bU,
  0xa05b77edU,0x68d06d6bU,0x5ab4e6c7U,0x8119642bU,0x489076abU,0x08104030U,0x172eb872U,0xc79562a4U,
  0xfee7b132U,0x76ec9d2fU,0xbf658fafU,0xc4937aaeU,0xf2ffd11aU,0x3e7ceb84U,0x4a9466a7U,0x0b16583aU,
  0x10208060U,0x1428a078U,0xf1f9c910U,0xefc53954U,0xa7554fffU,0x274e23d2U,0xe5d16968U,0xc88b1a86U,
  0xdea7aaf2U,0x9b2db477U,0x8d010403U,0x3c78fb88U,0x56ac86efU,0xd7b5e2c4U,0x8c030c05U,0x60c02d5bU,
  0x6ad47d67U,0x79f2e50dU,0xeec73152U,0xa5515ff3U,0x316293a6U,0x2e5c6be4U,0x77ee9529U,0x41823e9dU,
  0xffe5b934U,0x9531c453U,0xdda1b2f8U,0x254a33deU,0x3b76c39aU,0x55aa9ee5U,0xca8f0a8aU,0x52a4a6f7U,
  0x9e279c69U,0x2c587be8U,0x152aa87eU,0x4f9e4eb9U,0xe4d3616eU,0x162cb074U,0x70e0ad3bU,0x7dfac515U,
  0x72e4bd37U,0x3a74cb9cU,0x7bf6f501U,0x84134c35U,0xf6f7f102U,0x32648bacU,0x86175c39U,0x0306180aU,
  0xb473d795U,0x3870db90U,0x6fde5579U,0xb969bfbbU,0xc19952b0U,0x458a1e85U,0x880b2c1dU,0xe9c90940U,
  0xba6fa7b1U,0xb677c799U,0x6edc5d7fU,0x5ebcc6dfU,0xbe6787a9U,0x7efcdd1fU,0x20401bc0U,0xf9e98920U,
  0x22440bccU,0x66cc1d4fU,0x050a281eU,0xd2bfcadaU,0xcb8d028cU,0xc39d42bcU,0xcf852294U,0x5bb6eec1U
};

#ifndef _USE_ROT

const AURORA_UINT32 aurora_sm11[256] = {
  0xe0d9a992U,0xfedca3baU,0xdcd3bdc2U,0x6d69d265U,0xa3bd619fU,0x00000000U,0xb54d9a5eU,0x4cebcd19U,
  0x0c020410U,0xd824483bU,0xe957ae8eU,0xbac29f4aU,0xbdb86bb7U,0xd55dbadeU,0x9fb775cfU,0x756dda45U,
  0x08f5f1e9U,0x9b408036U,0xb2376ea3U,0xbf4e9c46U,0x561932c8U,0xe6d8ab9aU,0x4364c80dU,0x5762c43dU,
  0x639d2184U,0xb83468bbU,0x220f1e78U,0x137cf8cdU,0x5eecc321U,0x92ce872aU,0x559433ccU,0x18040820U,
  0xd0d1b9d2U,0x118a0f3cU,0x2374e88dU,0x2cfbed99U,0x64e7d579U,0x3f871554U,0x6c122490U,0xca234603U,
  0x93b571dfU,0xd35cb8d6U,0x5c1a34d0U,0xb7bb6dafU,0x97428426U,0xad49927eU,0x501830c0U,0x33851144U,
  0x66112288U,0x8f468c06U,0x2e0d1a68U,0x3d71e2a5U,0x4967ce15U,0x0f8f0514U,0xa2c6976aU,0xfb50a0b6U,
  0xcb58b0f6U,0x38fde1a9U,0xa14b966eU,0xf5a45357U,0x98cd8132U,0x098e071cU,0x7b9929a4U,0x421f3ef8U,
  0xc3ad411fU,0x5163c635U,0x80c98912U,0x616bd675U,0x04f7f5f9U,0xf028505bU,0x6f9f2594U,0x4565ca05U,
  0xe22f5e63U,0xd95fbeceU,0x5d61c225U,0x3173e6b5U,0x8e3d7af3U,0x178b0d34U,0x240e1c70U,0x5a1b36d8U,
  0xaa336683U,0x76e0db41U,0xc5ac4317U,0xd4264c2bU,0xeba1597fU,0x7ce3dd59U,0x1cf3fdd9U,0x21821f7cU,
  0x27831d74U,0x2575ea85U,0x83448816U,0x4d903becU,0x6a132698U,0xcfaf450fU,0x16f0fbc1U,0x12070e38U,
  0x599637dcU,0xc6214213U,0x26f8eb81U,0x823f7ee3U,0xe1a25f67U,0x7d982bacU,0x719a2fbcU,0xe7a35d6fU,
  0x4b9139e4U,0xb34c9856U,0x197ffed5U,0x41923ffcU,0x5f9735d4U,0x4aeacf11U,0x06010208U,0x481c38e0U,
  0x441e3cf0U,0xee2d5a73U,0x1b890924U,0x963972d3U,0x62e6d771U,0x659c238cU,0x3c0a1450U,0xe354a896U,
  0x280c1860U,0xfd51a2beU,0x736cd84dU,0x9143862eU,0xc9ae4707U,0xecdbad82U,0xf153a6aeU,0xcd59b2feU,
  0xf9a65747U,0x0ef4f3e1U,0x14060c30U,0xeadaaf8aU,0x7ae2df51U,0x0b78f0edU,0x4e1d3ae8U,0xf6295253U,
  0xa030609bU,0x70e1d949U,0xbe356ab3U,0x3efce3a1U,0x58edc129U,0xa5bc6397U,0x89478e0eU,0xc8d5b1f2U,
  0xb6c09b5aU,0xd7ab4d2fU,0x9ecc833aU,0xdda84b37U,0x2d801b6cU,0xfa2b5643U,0x36091248U,0x8db07bf7U,
  0x47933df4U,0xced4b3faU,0xa8c59172U,0x87b37defU,0xd6d0bbdaU,0xf4dfa5a2U,0xdba9493fU,0xd1aa4f27U,
  0x077af4fdU,0xb4366cabU,0xfc2a544bU,0xc2d6b7eaU,0x81b27fe7U,0x2afaef91U,0x46e8cb01U,0x8bb179ffU,
  0xeda05b77U,0x6b68d06dU,0xc75ab4e6U,0x2b811964U,0xab489076U,0x30081040U,0x72172eb8U,0xa4c79562U,
  0x32fee7b1U,0x2f76ec9dU,0xafbf658fU,0xaec4937aU,0x1af2ffd1U,0x843e7cebU,0xa74a9466U,0x3a0b1658U,
  0x60102080U,0x781428a0U,0x10f1f9c9U,0x54efc539U,0xffa7554fU,0xd2274e23U,0x68e5d169U,0x86c88b1aU,
  0xf2dea7aaU,0x779b2db4U,0x038d0104U,0x883c78fbU,0xef56ac86U,0xc4d7b5e2U,0x058c030cU,0x5b60c02dU,
  0x676ad47dU,0x0d79f2e5U,0x52eec731U,0xf3a5515fU,0xa6316293U,0xe42e5c6bU,0x2977ee95U,0x9d41823eU,
  0x34ffe5b9U,0x539531c4U,0xf8dda1b2U,0xde254a33U,0x9a3b76c3U,0xe555aa9eU,0x8aca8f0aU,0xf752a4a6U,
  0x699e279cU,0xe82c587bU,0x7e152aa8U,0xb94f9e4eU,0x6ee4d361U,0x74162cb0U,0x3b70e0adU,0x157dfac5U,
  0x3772e4bdU,0x9c3a74cbU,0x017bf6f5U,0x3584134cU,0x02f6f7f1U,0xac32648bU,0x3986175cU,0x0a030618U,
  0x95b473d7U,0x903870dbU,0x796fde55U,0xbbb969bfU,0xb0c19952U,0x85458a1eU,0x1d880b2cU,0x40e9c909U,
  0xb1ba6fa7U,0x99b677c7U,0x7f6edc5dU,0xdf5ebcc6U,0xa9be6787U,0x1f7efcddU,0xc020401bU,0x20f9e989U,
  0xcc22440bU,0x4f66cc1dU,0x1e050a28U,0xdad2bfcaU,0x8ccb8d02U,0xbcc39d42U,0x94cf8522U,0xc15bb6eeU
};

const AURORA_UINT32 aurora_sm12[256] = {
  0x92e0d9a9U,0xbafedca3U,0xc2dcd3bdU,0x656d69d2U,0x9fa3bd61U,0x00000000U,0x5eb54d9aU,0x194cebcdU,
  0x100c0204U,0x3bd82448U,0x8ee957aeU,0x4abac29fU,0xb7bdb86bU,0xded55dbaU,0xcf9fb775U,0x45756ddaU,
  0xe908f5f1U,0x369b4080U,0xa3b2376eU,0x46bf4e9cU,0xc8561932U,0x9ae6d8abU,0x0d4364c8U,0x3d5762c4U,
  0x84639d21U,0xbbb83468U,0x78220f1eU,0xcd137cf8U,0x215eecc3U,0x2a92ce87U,0xcc559433U,0x20180408U,
  0xd2d0d1b9U,0x3c118a0fU,0x8d2374e8U,0x992cfbedU,0x7964e7d5U,0x543f8715U,0x906c1224U,0x03ca2346U,
  0xdf93b571U,0xd6d35cb8U,0xd05c1a34U,0xafb7bb6dU,0x26974284U,0x7ead4992U,0xc0501830U,0x44338511U,
  0x88661122U,0x068f468cU,0x682e0d1aU,0xa53d71e2U,0x154967ceU,0x140f8f05U,0x6aa2c697U,0xb6fb50a0U,
  0xf6cb58b0U,0xa938fde1U,0x6ea14b96U,0x57f5a453U,0x3298cd81U,0x1c098e07U,0xa47b9929U,0xf8421f3eU,
  0x1fc3ad41U,0x355163c6U,0x1280c989U,0x75616bd6U,0xf904f7f5U,0x5bf02850U,0x946f9f25U,0x054565caU,
  0x63e22f5eU,0xced95fbeU,0x255d61c2U,0xb53173e6U,0xf38e3d7aU,0x34178b0dU,0x70240e1cU,0xd85a1b36U,
  0x83aa3366U,0x4176e0dbU,0x17c5ac43U,0x2bd4264cU,0x7feba159U,0x597ce3ddU,0xd91cf3fdU,0x7c21821fU,
  0x7427831dU,0x852575eaU,0x16834488U,0xec4d903bU,0x986a1326U,0x0fcfaf45U,0xc116f0fbU,0x3812070eU,
  0xdc599637U,0x13c62142U,0x8126f8ebU,0xe3823f7eU,0x67e1a25fU,0xac7d982bU,0xbc719a2fU,0x6fe7a35dU,
  0xe44b9139U,0x56b34c98U,0xd5197ffeU,0xfc41923fU,0xd45f9735U,0x114aeacfU,0x08060102U,0xe0481c38U,
  0xf0441e3cU,0x73ee2d5aU,0x241b8909U,0xd3963972U,0x7162e6d7U,0x8c659c23U,0x503c0a14U,0x96e354a8U,
  0x60280c18U,0xbefd51a2U,0x4d736cd8U,0x2e914386U,0x07c9ae47U,0x82ecdbadU,0xaef153a6U,0xfecd59b2U,
  0x47f9a657U,0xe10ef4f3U,0x3014060cU,0x8aeadaafU,0x517ae2dfU,0xed0b78f0U,0xe84e1d3aU,0x53f62952U,
  0x9ba03060U,0x4970e1d9U,0xb3be356aU,0xa13efce3U,0x2958edc1U,0x97a5bc63U,0x0e89478eU,0xf2c8d5b1U,
  0x5ab6c09bU,0x2fd7ab4dU,0x3a9ecc83U,0x37dda84bU,0x6c2d801bU,0x43fa2b56U,0x48360912U,0xf78db07bU,
  0xf447933dU,0xfaced4b3U,0x72a8c591U,0xef87b37dU,0xdad6d0bbU,0xa2f4dfa5U,0x3fdba949U,0x27d1aa4fU,
  0xfd077af4U,0xabb4366cU,0x4bfc2a54U,0xeac2d6b7U,0xe781b27fU,0x912afaefU,0x0146e8cbU,0xff8bb179U,
  0x77eda05bU,0x6d6b68d0U,0xe6c75ab4U,0x642b8119U,0x76ab4890U,0x40300810U,0xb872172eU,0x62a4c795U,
  0xb132fee7U,0x9d2f76ecU,0x8fafbf65U,0x7aaec493U,0xd11af2ffU,0xeb843e7cU,0x66a74a94U,0x583a0b16U,
  0x80601020U,0xa0781428U,0xc910f1f9U,0x3954efc5U,0x4fffa755U,0x23d2274eU,0x6968e5d1U,0x1a86c88bU,
  0xaaf2dea7U,0xb4779b2dU,0x04038d01U,0xfb883c78U,0x86ef56acU,0xe2c4d7b5U,0x0c058c03U,0x2d5b60c0U,
  0x7d676ad4U,0xe50d79f2U,0x3152eec7U,0x5ff3a551U,0x93a63162U,0x6be42e5cU,0x952977eeU,0x3e9d4182U,
  0xb934ffe5U,0xc4539531U,0xb2f8dda1U,0x33de254aU,0xc39a3b76U,0x9ee555aaU,0x0a8aca8fU,0xa6f752a4U,
  0x9c699e27U,0x7be82c58U,0xa87e152aU,0x4eb94f9eU,0x616ee4d3U,0xb074162cU,0xad3b70e0U,0xc5157dfaU,
  0xbd3772e4U,0xcb9c3a74U,0xf5017bf6U,0x4c358413U,0xf102f6f7U,0x8bac3264U,0x5c398617U,0x180a0306U,
  0xd795b473U,0xdb903870U,0x55796fdeU,0xbfbbb969U,0x52b0c199U,0x1e85458aU,0x2c1d880bU,0x0940e9c9U,
  0xa7b1ba6fU,0xc799b677U,0x5d7f6edcU,0xc6df5ebcU,0x87a9be67U,0xdd1f7efcU,0x1bc02040U,0x8920f9e9U,
  0x0bcc2244U,0x1d4f66ccU,0x281e050aU,0xcadad2bfU,0x028ccb8dU,0x42bcc39dU,0x2294cf85U,0xeec15bb6U
};

const AURORA_UINT32 aurora_sm13[256] = {
  0xa992e0d9U,0xa3bafedcU,0xbdc2dcd3U,0xd2656d69U,0x619fa3bdU,0x00000000U,0x9a5eb54dU,0xcd194cebU,
  0x04100c02U,0x483bd824U,0xae8ee957U,0x9f4abac2U,0x6bb7bdb8U,0xbaded55dU,0x75cf9fb7U,0xda45756dU,
  0xf1e908f5U,0x80369b40U,0x6ea3b237U,0x9c46bf4eU,0x32c85619U,0xab9ae6d8U,0xc80d4364U,0xc43d5762U,
  0x2184639dU,0x68bbb834U,0x1e78220fU,0xf8cd137cU,0xc3215eecU,0x872a92ceU,0x33cc5594U,0x08201804U,
  0xb9d2d0d1U,0x0f3c118aU,0xe88d2374U,0xed992cfbU,0xd57964e7U,0x15543f87U,0x24906c12U,0x4603ca23U,
  0x71df93b5U,0xb8d6d35cU,0x34d05c1aU,0x6dafb7bbU,0x84269742U,0x927ead49U,0x30c05018U,0x11443385U,
  0x22886611U,0x8c068f46U,0x1a682e0dU,0xe2a53d71U,0xce154967U,0x05140f8fU,0x976aa2c6U,0xa0b6fb50U,
  0xb0f6cb58U,0xe1a938fdU,0x966ea14bU,0x5357f5a4U,0x813298cdU,0x071c098eU,0x29a47b99U,0x3ef8421fU,
  0x411fc3adU,0xc6355163U,0x891280c9U,0xd675616bU,0xf5f904f7U,0x505bf028U,0x25946f9fU,0xca054565U,
  0x5e63e22fU,0xbeced95fU,0xc2255d61U,0xe6b53173U,0x7af38e3dU,0x0d34178bU,0x1c70240eU,0x36d85a1bU,
  0x6683aa33U,0xdb4176e0U,0x4317c5acU,0x4c2bd426U,0x597feba1U,0xdd597ce3U,0xfdd91cf3U,0x1f7c2182U,
  0x1d742783U,0xea852575U,0x88168344U,0x3bec4d90U,0x26986a13U,0x450fcfafU,0xfbc116f0U,0x0e381207U,
  0x37dc5996U,0x4213c621U,0xeb8126f8U,0x7ee3823fU,0x5f67e1a2U,0x2bac7d98U,0x2fbc719aU,0x5d6fe7a3U,
  0x39e44b91U,0x9856b34cU,0xfed5197fU,0x3ffc4192U,0x35d45f97U,0xcf114aeaU,0x02080601U,0x38e0481cU,
  0x3cf0441eU,0x5a73ee2dU,0x09241b89U,0x72d39639U,0xd77162e6U,0x238c659cU,0x14503c0aU,0xa896e354U,
  0x1860280cU,0xa2befd51U,0xd84d736cU,0x862e9143U,0x4707c9aeU,0xad82ecdbU,0xa6aef153U,0xb2fecd59U,
  0x5747f9a6U,0xf3e10ef4U,0x0c301406U,0xaf8aeadaU,0xdf517ae2U,0xf0ed0b78U,0x3ae84e1dU,0x5253f629U,
  0x609ba030U,0xd94970e1U,0x6ab3be35U,0xe3a13efcU,0xc12958edU,0x6397a5bcU,0x8e0e8947U,0xb1f2c8d5U,
  0x9b5ab6c0U,0x4d2fd7abU,0x833a9eccU,0x4b37dda8U,0x1b6c2d80U,0x5643fa2bU,0x12483609U,0x7bf78db0U,
  0x3df44793U,0xb3faced4U,0x9172a8c5U,0x7def87b3U,0xbbdad6d0U,0xa5a2f4dfU,0x493fdba9U,0x4f27d1aaU,
  0xf4fd077aU,0x6cabb436U,0x544bfc2aU,0xb7eac2d6U,0x7fe781b2U,0xef912afaU,0xcb0146e8U,0x79ff8bb1U,
  0x5b77eda0U,0xd06d6b68U,0xb4e6c75aU,0x19642b81U,0x9076ab48U,0x10403008U,0x2eb87217U,0x9562a4c7U,
  0xe7b132feU,0xec9d2f76U,0x658fafbfU,0x937aaec4U,0xffd11af2U,0x7ceb843eU,0x9466a74aU,0x16583a0bU,
  0x20806010U,0x28a07814U,0xf9c910f1U,0xc53954efU,0x554fffa7U,0x4e23d227U,0xd16968e5U,0x8b1a86c8U,
  0xa7aaf2deU,0x2db4779bU,0x0104038dU,0x78fb883cU,0xac86ef56U,0xb5e2c4d7U,0x030c058cU,0xc02d5b60U,
  0xd47d676aU,0xf2e50d79U,0xc73152eeU,0x515ff3a5U,0x6293a631U,0x5c6be42eU,0xee952977U,0x823e9d41U,
  0xe5b934ffU,0x31c45395U,0xa1b2f8ddU,0x4a33de25U,0x76c39a3bU,0xaa9ee555U,0x8f0a8acaU,0xa4a6f752U,
  0x279c699eU,0x587be82cU,0x2aa87e15U,0x9e4eb94fU,0xd3616ee4U,0x2cb07416U,0xe0ad3b70U,0xfac5157dU,
  0xe4bd3772U,0x74cb9c3aU,0xf6f5017bU,0x134c3584U,0xf7f102f6U,0x648bac32U,0x175c3986U,0x06180a03U,
  0x73d795b4U,0x70db9038U,0xde55796fU,0x69bfbbb9U,0x9952b0c1U,0x8a1e8545U,0x0b2c1d88U,0xc90940e9U,
  0x6fa7b1baU,0x77c799b6U,0xdc5d7f6eU,0xbcc6df5eU,0x6787a9beU,0xfcdd1f7eU,0x401bc020U,0xe98920f9U,
  0x440bcc22U,0xcc1d4f66U,0x0a281e05U,0xbfcadad2U,0x8d028ccbU,0x9d42bcc3U,0x852294cfU,0xb6eec15bU
};

#endif



/* Constant values for AURORA-224/256 */

/* CONC */
const AURORA_UINT32 aurora256conc[] = {
  0x2883f695U,0xca509844U,0x096a4c18U,0xcf76858fU,
  0x9698ed2bU,0xf89c5476U,0x12d4f203U,0x5713b743U,
  0x429feaffU,0xe1fa326fU,0x15002604U,0x9b21ae25U,
  0x42a0d5ffU,0xed498163U,0x2a00263bU,0xfd38a296U,
  0x42deabffU,0x3f08c0b1U,0x54002645U,0x7e9c70d7U,
  0x422257ffU,0x8230f80cU,0xa80026b9U,0x0fe6cdefU,
  0x43daaffeU,0xdcac6452U,0x50012741U,0x375b9373U,
  0x402a5ffdU,0xf3e22a7dU,0xa00224b1U,0xab05bc3dU,
  0x47cabffaU,0xe4458d6aU,0x40052351U,0xe52aab9aU,
  0x480a7ff5U,0x3b8e46b5U,0x800a2c91U,0x72957451U,
  0x578affeaU,0x8073bb0eU,0x00153311U,0x89e2cfacU,
  0x688affd5U,0x09955d87U,0x002a0c11U,0x44f1464aU,
  0x168affabU,0x4d66aec3U,0x00547211U,0xa27802b9U,
  0xea8aff57U,0xbb07cf35U,0x00a88e11U,0x6194f4d8U,
  0xbabbce07U,0x142fe79aU,0x31f8de20U,0x30ca5bf0U,
  0x1ad9aca7U,0x43bb73cdU,0x53587e42U,0x18650c64U,
  0xf22c594fU,0x6871b9e6U,0xa6b096b7U,0x8c3227aeU,
  0x00000000U,0x00000000U,0x00000000U,0x00000000U
};

/* CONM_L */
const AURORA_UINT32 aurora256conml[] = {
  0x5107ed2aU,0x94a13089U,0x12d49830U,0x9eed0b1fU,
  0x853fd5feU,0xc3f464dfU,0x2a004c08U,0x36435c4bU,
  0x85bd57feU,0x7e118162U,0xa8004c8aU,0xfd38e1aeU,
  0x87b55ffcU,0xb958c8a5U,0xa0024e82U,0x6eb726e6U,
  0x8f957ff4U,0xc88b1ad5U,0x800a46a2U,0xca555735U,
  0xaf15ffd4U,0x00e7761dU,0x002a6622U,0x13c59f59U,
  0x2d15ff56U,0x9acd5d86U,0x00a8e422U,0x44f00573U,
  0x75779c0fU,0x285fcf34U,0x63f1bc40U,0x6194b7e0U,
  0x00000000U,0x00000000U,0x00000000U,0x00000000U
};

/* CONM_R */
const AURORA_UINT32 aurora256conmr[] = {
  0xcb4c7695U,0x7c4e2a3bU,0x896a7901U,0xab89dba1U,
  0xa1506affU,0xf6a4c0b1U,0x9500131dU,0x7e9c514bU,
  0xa1112bffU,0x41187c06U,0xd400135cU,0x87f366f7U,
  0xa0152ffeU,0xf9f1153eU,0xd0011258U,0xd582de1eU,
  0xa4053ffaU,0x9dc7235aU,0xc0051648U,0xb94aba28U,
  0xb4457feaU,0x84caaec3U,0x80150608U,0x2278a325U,
  0xf5457fabU,0xdd83e79aU,0x80544708U,0x30ca7a6cU,
  0x8d6cd653U,0xa1ddb9e6U,0x29ac3f21U,0x0c328632U,
  0x00000000U,0x00000000U,0x00000000U,0x00000000U
};

/* Constant values for AURORA-384/512 */

/* CONC_L */
const AURORA_UINT32 aurora512concl[] = {
  0x6858f1aeU,0xc2f4fa64U,0x0e51cc84U,0x0b363092U,
  0x9b4ae35dU,0xc06b6566U,0x1ca23f96U,0x3533320dU,
  0xd55ff613U,0x153c32b3U,0x09ec7183U,0x9a99e75aU,
  0x4975dc8fU,0xab8f810dU,0x2370eda9U,0xfde459e9U,
  0xd910b91fU,0x20cec086U,0x46e07dccU,0x7ef2d2a8U,
  0x51eb4297U,0xb1767817U,0xbd68f537U,0x0fd14310U,
  0xe82c852eU,0xf9aaa45fU,0x7ad14cf0U,0xb7400bccU,
  0x33933af5U,0xddc4ca7bU,0xc50a974fU,0x6b082fa2U,
  0x2cdc75eaU,0xcff3fd69U,0x8a158800U,0x052c3d95U,
  0x1242ebd4U,0x12f0feb4U,0x142bb69eU,0x0296e096U,
  0x6f7ed7a9U,0xa869670eU,0x2856cba2U,0x31e35a0fU,
  0x9506af53U,0x213d3387U,0x50ac31daU,0x98f1d35bU,
  0xc9c76e0fU,0x659799c3U,0x91f06d1bU,0xcc7897f1U,
  0x7045ecb6U,0x47c2cce1U,0x1349d499U,0x663cb5a4U,
  0xab70d96dU,0x82f0fe24U,0x26920facU,0x03b67096U,
  0xb52b8273U,0xe0696746U,0x7d8c11f7U,0x3173120fU,
  0x899d344fU,0x053d33a3U,0xcbb02d41U,0x98b9f75bU,  
  0x00000000U,0x00000000U,0x00000000U,0x00000000U
};

/* CONC_R */
const AURORA_UINT32 aurora512concr[] = {
  0x42c78d73U,0x17a7d326U,0x728e6420U,0x59b18490U,
  0xda571aecU,0x035b2b36U,0xe511fcb0U,0xa9999069U,
  0xaaffb09eU,0xa9e19598U,0x4f638c18U,0xd4cf3ad4U,
  0x4baee47aU,0x5c7c086dU,0x1b876d49U,0xef22cf4fU,
  0xc885c8feU,0x06760431U,0x3703ee62U,0xf7969543U,
  0x8f5a14baU,0x8bb3c0bdU,0xeb47a9bdU,0x7e8a1880U,
  0x41642977U,0xcd5522ffU,0xd68a6783U,0xba005e65U,
  0x9c99d7a9U,0xee2653deU,0x2854ba7eU,0x58417d13U,
  0x66e3af51U,0x7f9feb4eU,0x50ac4004U,0x2961eca8U,
  0x92175ea0U,0x9787f5a0U,0xa15db4f0U,0x14b704b0U,
  0x7bf6bd4bU,0x434b3875U,0x42b65d11U,0x8f1ad079U,
  0xa8357a9cU,0x09e99c39U,0x85618ed2U,0xc78e9adcU,
  0x4e3b707eU,0x2cbcce1bU,0x8f8368dcU,0x63c4bf8eU,
  0x822f65b3U,0x3e16670aU,0x9a4ea4c8U,0x31e5ad23U,
  0x5b86cb6dU,0x1787f124U,0x34907d61U,0x1db384b0U,
  0xa95c139dU,0x034b3a37U,0xec608fbbU,0x8b989079U,
  0x4ce9a27cU,0x29e99d18U,0x5d816a0eU,0xc5cfbadcU,
  0x00000000U,0x00000000U,0x00000000U,0x00000000U
};

/* CONM_L */
const AURORA_UINT32 aurora512conml[] = {
  0xd0b1e35cU,0x85e9f4c9U,0x1ca39908U,0x166c6124U,
  0xaabfec27U,0x2a786566U,0x13d8e306U,0x3533ceb5U,
  0xb221723fU,0x419d810cU,0x8dc0fb98U,0xfde5a550U,
  0xd0590a5dU,0xf35548bfU,0xf5a299e0U,0x6e801799U,
  0x59b8ebd4U,0x9fe7fad3U,0x142b1001U,0x0a587b2aU,
  0xdefdaf52U,0x50d2ce1dU,0x50ad9744U,0x63c6b41eU,
  0x938edc1fU,0xcb2f3386U,0x23e0da37U,0x98f12fe3U,
  0x56e1b2dbU,0x05e1fc49U,0x4d241f58U,0x076ce12cU,
  0x00000000U,0x00000000U,0x00000000U,0x00000000U
};

/* CONM_R */
const AURORA_UINT32 aurora512conmr[] = {
  0xcda571aeU,0x6035b2b3U,0x0e511fcbU,0x9a999906U,
  0xa4baee47U,0xd5c7c086U,0x91b876d4U,0xfef22cf4U,
  0xa8f5a14bU,0xd8bb3c0bU,0xdeb47a9bU,0x07e8a188U,
  0x99c99d7aU,0xeee2653dU,0xe2854ba7U,0x358417d1U,
  0x092175eaU,0x09787f5aU,0x0a15db4fU,0x014b704bU,
  0xca8357a9U,0x909e99c3U,0x285618edU,0xcc78e9adU,
  0x3822f65bU,0xa3e16670U,0x89a4ea4cU,0x331e5ad2U,
  0xda95c139U,0x7034b3a3U,0xbec608fbU,0x98b98907U,
  0x00000000U,0x00000000U,0x00000000U,0x00000000U
};

/* Macro functions for AURORA */

/* uchar to ulong (1-word = 32-bit) */
#define AURORA_READ32BIT(_dst, _src)\
{ (_dst) =\
    (((AURORA_UINT32) *((_src) + 0)) << 24) |\
    (((AURORA_UINT32) *((_src) + 1)) << 16) |\
    (((AURORA_UINT32) *((_src) + 2)) << 8)  |\
    ( (AURORA_UINT32) *((_src) + 3));\
}

/* ulong to uchar (1-word = 32-bit) */
#define AURORA_WRITE32BIT(_dst, _src)\
{ *((_dst) + 0) = (BitSequence) ((_src) >> 24);        \
  *((_dst) + 1) = (BitSequence) ((_src) >> 16) & 0xffU;\
  *((_dst) + 2) = (BitSequence) ((_src) >> 8)  & 0xffU;\
  *((_dst) + 3) = (BitSequence)  (_src)        & 0xffU;\
}

/* ulonglong to uchar (1-word = 64-bit) */
#define AURORA_WRITE64BIT(_dst, _src)\
{ *((_dst) + 0) = (BitSequence) ((_src) >> 56);        \
  *((_dst) + 1) = (BitSequence) ((_src) >> 48) & 0xffU;\
  *((_dst) + 2) = (BitSequence) ((_src) >> 40) & 0xffU;\
  *((_dst) + 3) = (BitSequence) ((_src) >> 32) & 0xffU;\
  *((_dst) + 4) = (BitSequence) ((_src) >> 24) & 0xffU;\
  *((_dst) + 5) = (BitSequence) ((_src) >> 16) & 0xffU;\
  *((_dst) + 6) = (BitSequence) ((_src) >> 8)  & 0xffU;\
  *((_dst) + 7) = (BitSequence)  (_src)        & 0xffU;\
}

/* ulong to uchar (4-word = 128-bit) */
#define AURORA_WRITE128BIT(_dst, _src0, _src1, _src2, _src3)\
{ AURORA_WRITE32BIT((_dst) + 0,  (_src0));\
  AURORA_WRITE32BIT((_dst) + 4,  (_src1));\
  AURORA_WRITE32BIT((_dst) + 8,  (_src2));\
  AURORA_WRITE32BIT((_dst) + 12, (_src3));\
}

/* ulong to uchar (8-word = 256-bit) */
#define AURORA_WRITE256BIT(_dst, _src0, _src1, _src2, _src3, _src4, _src5, _src6, _src7)\
{ AURORA_WRITE128BIT((_dst) + 0,  (_src0), (_src1), (_src2), (_src3)); \
  AURORA_WRITE128BIT((_dst) + 16, (_src4), (_src5), (_src6), (_src7)); \
}

#ifndef _USE_ROT

#define F0_0(_x) aurora_sm00[(_x)]
#define F0_1(_x) aurora_sm01[(_x)]
#define F0_2(_x) aurora_sm02[(_x)]
#define F0_3(_x) aurora_sm03[(_x)]

#define F1_0(_x) aurora_sm10[(_x)]
#define F1_1(_x) aurora_sm11[(_x)]
#define F1_2(_x) aurora_sm12[(_x)]
#define F1_3(_x) aurora_sm13[(_x)]

#else /* _USE_ROT */

/* n-bit right rotation for 32-bit processor (x: unsigned long) */
#define AURORA_ROTR32(_x, _n) (((_x) >> (_n)) | ((_x) << (32 - (_n))))

#define F0_0(_x) aurora_sm00[(_x)]
#define F0_1(_x) AURORA_ROTR32(aurora_sm00[(_x)], 8)
#define F0_2(_x) AURORA_ROTR32(aurora_sm00[(_x)], 16)
#define F0_3(_x) AURORA_ROTR32(aurora_sm00[(_x)], 24)

#define F1_0(_x) aurora_sm10[(_x)]
#define F1_1(_x) AURORA_ROTR32(aurora_sm10[(_x)], 8)
#define F1_2(_x) AURORA_ROTR32(aurora_sm10[(_x)], 16)
#define F1_3(_x) AURORA_ROTR32(aurora_sm10[(_x)], 24)

#endif /* ?_USE_ROT */


#define AURORA_F32(_dst, _s0, _s1, _s2, _s3, _tbx0, _tbx1, _tbx2, _tbx3) \
{ (_dst) = _tbx0( (_s0) >> 24         ) ^ \
           _tbx1(((_s1) >> 16) & 0xffU) ^ \
           _tbx2(((_s2) >> 8)  & 0xffU) ^ \
           _tbx3( (_s3)        & 0xffU);\
}


/*=============================================================================
 * Aurora256CF
 *
 * AURORA-224/256 compression function
 *===========================================================================*/

#define AURORA256_2ROUNDS(_idx) \
{ /* MS_L */ \
  AURORA_F32(fmsl[0], ml[4], ml[7], ml[6], ml[5], F0_0, F0_1, F0_2, F0_3); \
  AURORA_F32(fmsl[1], ml[5], ml[4], ml[7], ml[6], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fmsl[2], ml[6], ml[5], ml[4], ml[7], F0_0, F0_1, F0_2, F0_3); \
  AURORA_F32(fmsl[3], ml[7], ml[6], ml[5], ml[4], F1_0, F1_1, F1_2, F1_3); \
  /* XORing after F and XORing with CONM_L */ \
  ml[4] = fmsl[0] ^ ml[1] ^ aurora256conml[(_idx) * 4 + 0]; \
  ml[5] = fmsl[1] ^ ml[2] ^ aurora256conml[(_idx) * 4 + 1]; \
  ml[6] = fmsl[2] ^ ml[3] ^ aurora256conml[(_idx) * 4 + 2]; \
  ml[7] = fmsl[3] ^ ml[0] ^ aurora256conml[(_idx) * 4 + 3]; \
  ml[0] = fmsl[0]; \
  ml[1] = fmsl[1]; \
  ml[2] = fmsl[2]; \
  ml[3] = fmsl[3]; \
  /* CP (R1) */ \
  AURORA_F32(fcp[0], x[4], x[7], x[6], x[5], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fcp[1], x[5], x[4], x[7], x[6], F0_0, F0_1, F0_2, F0_3); \
  AURORA_F32(fcp[2], x[6], x[5], x[4], x[7], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fcp[3], x[7], x[6], x[5], x[4], F0_0, F0_1, F0_2, F0_3); \
  /* Xoring after F and XORing with CONC */ \
  x[4] = fcp[0] ^ x[1] ^ aurora256conc[(_idx) * 8 - 4] ^ (mr[4] >> 1) ^ (mr[5] << 31); \
  x[5] = fcp[1] ^ x[2] ^ aurora256conc[(_idx) * 8 - 3] ^ (mr[5] >> 1) ^ (mr[4] << 31); \
  x[6] = fcp[2] ^ x[3] ^ aurora256conc[(_idx) * 8 - 2] ^ mr[6]; \
  x[7] = fcp[3] ^ x[0] ^ aurora256conc[(_idx) * 8 - 1] ^ mr[7] ^ mask; \
  x[0] = fcp[0] ^ mr[0]; \
  x[1] = fcp[1] ^ mr[1]; \
  x[2] = fcp[2] ^ mr[2]; \
  x[3] = fcp[3] ^ mr[3]; \
  /* MS_R */ \
  AURORA_F32(fmsr[0], mr[4], mr[7], mr[6], mr[5], F0_3, F0_0, F0_1, F0_2); \
  AURORA_F32(fmsr[1], mr[5], mr[4], mr[7], mr[6], F1_1, F1_2, F1_3, F1_0); \
  AURORA_F32(fmsr[2], mr[6], mr[5], mr[4], mr[7], F0_3, F0_0, F0_1, F0_2); \
  AURORA_F32(fmsr[3], mr[7], mr[6], mr[5], mr[4], F1_1, F1_2, F1_3, F1_0); \
  /* XORing after F and XORing with CONM_R */ \
  mr[4] = fmsr[0] ^ mr[1] ^ aurora256conmr[(_idx) * 4 + 0]; \
  mr[5] = fmsr[1] ^ mr[2] ^ aurora256conmr[(_idx) * 4 + 1]; \
  mr[6] = fmsr[2] ^ mr[3] ^ aurora256conmr[(_idx) * 4 + 2]; \
  mr[7] = fmsr[3] ^ mr[0] ^ aurora256conmr[(_idx) * 4 + 3]; \
  mr[0] = fmsr[0]; \
  mr[1] = fmsr[1]; \
  mr[2] = fmsr[2]; \
  mr[3] = fmsr[3]; \
  /* CP (R2) */ \
  AURORA_F32(fcp[0], x[4], x[7], x[6], x[5], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fcp[1], x[5], x[4], x[7], x[6], F0_0, F0_1, F0_2, F0_3); \
  AURORA_F32(fcp[2], x[6], x[5], x[4], x[7], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fcp[3], x[7], x[6], x[5], x[4], F0_0, F0_1, F0_2, F0_3); \
  /* XORing after F and XORing with CONC */ \
  x[4] = fcp[0] ^ x[1] ^ aurora256conc[(_idx) * 8 + 0] ^ (ml[4] << 1) ^ (ml[5] >> 31); \
  x[5] = fcp[1] ^ x[2] ^ aurora256conc[(_idx) * 8 + 1] ^ (ml[5] << 1) ^ (ml[4] >> 31); \
  x[6] = fcp[2] ^ x[3] ^ aurora256conc[(_idx) * 8 + 2] ^ ml[6]; \
  x[7] = fcp[3] ^ x[0] ^ aurora256conc[(_idx) * 8 + 3] ^ ml[7] ^ mask; \
  x[0] = fcp[0] ^ ml[0]; \
  x[1] = fcp[1] ^ ml[1]; \
  x[2] = fcp[2] ^ ml[2]; \
  x[3] = fcp[3] ^ ml[3]; \
}

void Aurora::Aurora256CF(AURORA_CTX *hctx, const BitSequence *m, const AURORA_UINT32 mask, const int rounds)
{
  AURORA_UINT32 ml[8], mr[8], x[8];
  AURORA_UINT32 fmsl[4], fmsr[4], fcp[4];
  int i;

  x[0] = hctx->h[0]; x[1] = hctx->h[2];
  x[2] = hctx->h[4]; x[3] = hctx->h[6];
  x[4] = hctx->h[1]; x[5] = hctx->h[3];
  x[6] = hctx->h[5]; x[7] = hctx->h[7];

  for(i = 0; i < 4; i++){
    AURORA_READ32BIT(ml[i],     m + (i * 8));
    AURORA_READ32BIT(ml[i + 4], m + (i * 8 + 4));
    AURORA_READ32BIT(mr[i],     m + (i * 8 + 32));
    AURORA_READ32BIT(mr[i + 4], m + (i * 8 + 36));
  }

  /* whitening */
  for(i = 0; i < 4; i++){
    ml[i + 4] ^= aurora256conml[i];
    mr[i + 4] ^= aurora256conmr[i];
  }
  x[0] ^= ml[0];
  x[1] ^= ml[1];
  x[2] ^= ml[2];
  x[3] ^= ml[3];
  x[4] ^= (ml[4] << 1) ^ (ml[5] >> 31) ^ aurora256conc[0];
  x[5] ^= (ml[5] << 1) ^ (ml[4] >> 31) ^ aurora256conc[1];
  x[6] ^= ml[6] ^ aurora256conc[2];
  x[7] ^= ml[7] ^ aurora256conc[3] ^ mask;

//NASTAVENIE RUND + DALSIE DOLE:
#ifndef _LOOPED /* = _LOOP_UNROLL */
  if (rounds >= 1) AURORA256_2ROUNDS(1); /* R1,  R2  */
  if (rounds >= 3) AURORA256_2ROUNDS(2); /* R3,  R4  */
  if (rounds >= 5) AURORA256_2ROUNDS(3); /* R5,  R6  */
  if (rounds >= 7) AURORA256_2ROUNDS(4); /* R7,  R8  */
  if (rounds >= 9) AURORA256_2ROUNDS(5); /* R9,  R10 */
  if (rounds >= 11) AURORA256_2ROUNDS(6); /* R11, R12 */
  if (rounds >= 13) AURORA256_2ROUNDS(7); /* R13, R14 */
  if (rounds >= 15) AURORA256_2ROUNDS(8); /* R15, R16 */
#else /* _LOOPED */
  int realRounds = 8;
  if (rounds >= 1) realRounds = 1;
  if (rounds >= 3) realRounds = 2;
  if (rounds >= 5) realRounds = 3;
  if (rounds >= 7) realRounds = 4;
  if (rounds >= 9) realRounds = 5;
  if (rounds >= 11) realRounds = 6;
  if (rounds >= 13) realRounds = 7;
  if (rounds >= 15) realRounds = 8;
  for(i = 1; i <= realRounds; i++){
    AURORA256_2ROUNDS(i);
  }
#endif

  /* final round (R17) */

  /* CP */
  AURORA_F32(fcp[0], x[4], x[7], x[6], x[5], F1_0, F1_1, F1_2, F1_3);
  AURORA_F32(fcp[1], x[5], x[4], x[7], x[6], F0_0, F0_1, F0_2, F0_3);
  AURORA_F32(fcp[2], x[6], x[5], x[4], x[7], F1_0, F1_1, F1_2, F1_3);
  AURORA_F32(fcp[3], x[7], x[6], x[5], x[4], F0_0, F0_1, F0_2, F0_3);

  /* XORing after F + XORing after CP */
  hctx->h[1] ^= fcp[0] ^ x[1] ^ (mr[4] >> 1) ^ (mr[5] << 31);
  hctx->h[3] ^= fcp[1] ^ x[2] ^ (mr[5] >> 1) ^ (mr[4] << 31);
  hctx->h[5] ^= fcp[2] ^ x[3] ^ mr[6];
  hctx->h[7] ^= fcp[3] ^ x[0] ^ mr[7];

  hctx->h[0] ^= fcp[0] ^ mr[0];
  hctx->h[2] ^= fcp[1] ^ mr[1];
  hctx->h[4] ^= fcp[2] ^ mr[2];
  hctx->h[6] ^= fcp[3] ^ mr[3];
}


/*=============================================================================
 * AURORA512CF and AURORA512MF
 *
 * AURORA-384/512 compression function and mixing function
 *===========================================================================*/

#define AURORA512_2ROUNDS(_idx) \
{ /* MS_L */ \
  AURORA_F32(fmsl[0], ml[4], ml[7], ml[6], ml[5], F0_0, F0_1, F0_2, F0_3); \
  AURORA_F32(fmsl[1], ml[5], ml[4], ml[7], ml[6], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fmsl[2], ml[6], ml[5], ml[4], ml[7], F0_0, F0_1, F0_2, F0_3); \
  AURORA_F32(fmsl[3], ml[7], ml[6], ml[5], ml[4], F1_0, F1_1, F1_2, F1_3); \
  /* XORing after F and XORing with CONM_L */ \
  ml[4] = fmsl[0] ^ ml[1] ^ aurora512conml[(_idx) * 4 + 0]; \
  ml[5] = fmsl[1] ^ ml[2] ^ aurora512conml[(_idx) * 4 + 1]; \
  ml[6] = fmsl[2] ^ ml[3] ^ aurora512conml[(_idx) * 4 + 2]; \
  ml[7] = fmsl[3] ^ ml[0] ^ aurora512conml[(_idx) * 4 + 3]; \
  ml[0] = fmsl[0]; \
  ml[1] = fmsl[1]; \
  ml[2] = fmsl[2]; \
  ml[3] = fmsl[3]; \
  /* CP_L (R1) */ \
  AURORA_F32(fcpl[0], xl[4], xl[7], xl[6], xl[5], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fcpl[1], xl[5], xl[4], xl[7], xl[6], F0_0, F0_1, F0_2, F0_3); \
  AURORA_F32(fcpl[2], xl[6], xl[5], xl[4], xl[7], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fcpl[3], xl[7], xl[6], xl[5], xl[4], F0_0, F0_1, F0_2, F0_3); \
 /* XORing after F and XORing with CONC_L */ \
  xl[4] = fcpl[0] ^ xl[1] ^ aurora512concl[(_idx) * 8 - 4] ^ (mr[4] >> 1) ^ (mr[5] << 31); \
  xl[5] = fcpl[1] ^ xl[2] ^ aurora512concl[(_idx) * 8 - 3] ^ (mr[5] >> 1) ^ (mr[4] << 31); \
  xl[6] = fcpl[2] ^ xl[3] ^ aurora512concl[(_idx) * 8 - 2] ^ mr[6]; \
  xl[7] = fcpl[3] ^ xl[0] ^ aurora512concl[(_idx) * 8 - 1] ^ mr[7] ^ mask; \
  xl[0] = fcpl[0] ^ mr[0]; \
  xl[1] = fcpl[1] ^ mr[1]; \
  xl[2] = fcpl[2] ^ mr[2]; \
  xl[3] = fcpl[3] ^ mr[3]; \
  /* CP_R (R1) */ \
  AURORA_F32(fcpr[0], xr[4], xr[7], xr[6], xr[5], F1_1, F1_2, F1_3, F1_0); \
  AURORA_F32(fcpr[1], xr[5], xr[4], xr[7], xr[6], F0_3, F0_0, F0_1, F0_2); \
  AURORA_F32(fcpr[2], xr[6], xr[5], xr[4], xr[7], F1_1, F1_2, F1_3, F1_0); \
  AURORA_F32(fcpr[3], xr[7], xr[6], xr[5], xr[4], F0_3, F0_0, F0_1, F0_2); \
 /* XORing after F and XORing with CONC_R */ \
  xr[4] = fcpr[0] ^ xr[1] ^ aurora512concr[(_idx) * 8 - 4] ^ (mr[4] >> 1) ^ (mr[5] << 31); \
  xr[5] = fcpr[1] ^ xr[2] ^ aurora512concr[(_idx) * 8 - 3] ^ (mr[5] >> 1) ^ (mr[4] << 31); \
  xr[6] = fcpr[2] ^ xr[3] ^ aurora512concr[(_idx) * 8 - 2] ^ mr[6]; \
  xr[7] = fcpr[3] ^ xr[0] ^ aurora512concr[(_idx) * 8 - 1] ^ mr[7] ^ mask; \
  xr[0] = fcpr[0] ^ mr[0]; \
  xr[1] = fcpr[1] ^ mr[1]; \
  xr[2] = fcpr[2] ^ mr[2]; \
  xr[3] = fcpr[3] ^ mr[3]; \
  /* MS_R */ \
  AURORA_F32(fmsr[0], mr[4], mr[7], mr[6], mr[5], F0_3, F0_0, F0_1, F0_2); \
  AURORA_F32(fmsr[1], mr[5], mr[4], mr[7], mr[6], F1_1, F1_2, F1_3, F1_0); \
  AURORA_F32(fmsr[2], mr[6], mr[5], mr[4], mr[7], F0_3, F0_0, F0_1, F0_2); \
  AURORA_F32(fmsr[3], mr[7], mr[6], mr[5], mr[4], F1_1, F1_2, F1_3, F1_0); \
  /* XORing after F and XORing with CONM_R */ \
  mr[4] = fmsr[0] ^ mr[1] ^ aurora512conmr[(_idx) * 4 + 0]; \
  mr[5] = fmsr[1] ^ mr[2] ^ aurora512conmr[(_idx) * 4 + 1]; \
  mr[6] = fmsr[2] ^ mr[3] ^ aurora512conmr[(_idx) * 4 + 2]; \
  mr[7] = fmsr[3] ^ mr[0] ^ aurora512conmr[(_idx) * 4 + 3]; \
  mr[0] = fmsr[0]; \
  mr[1] = fmsr[1]; \
  mr[2] = fmsr[2]; \
  mr[3] = fmsr[3]; \
  /* CP_L (R2) */ \
  AURORA_F32(fcpl[0], xl[4], xl[7], xl[6], xl[5], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fcpl[1], xl[5], xl[4], xl[7], xl[6], F0_0, F0_1, F0_2, F0_3); \
  AURORA_F32(fcpl[2], xl[6], xl[5], xl[4], xl[7], F1_0, F1_1, F1_2, F1_3); \
  AURORA_F32(fcpl[3], xl[7], xl[6], xl[5], xl[4], F0_0, F0_1, F0_2, F0_3); \
  /* XORing after F and XORing with CONC_L */ \
  xl[4] = fcpl[0] ^ xl[1] ^ aurora512concl[(_idx) * 8 + 0] ^ (ml[4] << 1) ^ (ml[5] >> 31); \
  xl[5] = fcpl[1] ^ xl[2] ^ aurora512concl[(_idx) * 8 + 1] ^ (ml[5] << 1) ^ (ml[4] >> 31); \
  xl[6] = fcpl[2] ^ xl[3] ^ aurora512concl[(_idx) * 8 + 2] ^ ml[6]; \
  xl[7] = fcpl[3] ^ xl[0] ^ aurora512concl[(_idx) * 8 + 3] ^ ml[7] ^ mask; \
  xl[0] = fcpl[0] ^ ml[0]; \
  xl[1] = fcpl[1] ^ ml[1]; \
  xl[2] = fcpl[2] ^ ml[2]; \
  xl[3] = fcpl[3] ^ ml[3]; \
  /* CP_R (R2) */ \
  AURORA_F32(fcpr[0], xr[4], xr[7], xr[6], xr[5], F1_1, F1_2, F1_3, F1_0); \
  AURORA_F32(fcpr[1], xr[5], xr[4], xr[7], xr[6], F0_3, F0_0, F0_1, F0_2); \
  AURORA_F32(fcpr[2], xr[6], xr[5], xr[4], xr[7], F1_1, F1_2, F1_3, F1_0); \
  AURORA_F32(fcpr[3], xr[7], xr[6], xr[5], xr[4], F0_3, F0_0, F0_1, F0_2); \
  /* XORing after F and XORing with CONC_R */ \
  xr[4] = fcpr[0] ^ xr[1] ^ aurora512concr[(_idx) * 8 + 0] ^ (ml[4] << 1) ^ (ml[5] >> 31); \
  xr[5] = fcpr[1] ^ xr[2] ^ aurora512concr[(_idx) * 8 + 1] ^ (ml[5] << 1) ^ (ml[4] >> 31); \
  xr[6] = fcpr[2] ^ xr[3] ^ aurora512concr[(_idx) * 8 + 2] ^ ml[6]; \
  xr[7] = fcpr[3] ^ xr[0] ^ aurora512concr[(_idx) * 8 + 3] ^ ml[7] ^ mask; \
  xr[0] = fcpr[0] ^ ml[0]; \
  xr[1] = fcpr[1] ^ ml[1]; \
  xr[2] = fcpr[2] ^ ml[2]; \
  xr[3] = fcpr[3] ^ ml[3]; \
}

void Aurora::Aurora512CF(AURORA_CTX *hctx, const BitSequence *m, const AURORA_UINT32 mask, const int rounds)
{
  AURORA_UINT32 ml[8], mr[8], xl[8], xr[8];
  AURORA_UINT32 fmsl[4], fmsr[4], fcpl[4], fcpr[4];
  int i;

  xl[0] = hctx->h[0]; xl[1] = hctx->h[2];
  xl[2] = hctx->h[4]; xl[3] = hctx->h[6];
  xl[4] = hctx->h[1]; xl[5] = hctx->h[3];
  xl[6] = hctx->h[5]; xl[7] = hctx->h[7];

  xr[0] = hctx->h[8];  xr[1] = hctx->h[10];
  xr[2] = hctx->h[12]; xr[3] = hctx->h[14];
  xr[4] = hctx->h[9];  xr[5] = hctx->h[11];
  xr[6] = hctx->h[13]; xr[7] = hctx->h[15];

  for(i = 0; i < 4; i++){
    AURORA_READ32BIT(ml[i],     m + (i * 8));
    AURORA_READ32BIT(ml[i + 4], m + (i * 8 + 4));
    AURORA_READ32BIT(mr[i],     m + (i * 8 + 32));
    AURORA_READ32BIT(mr[i + 4], m + (i * 8 + 36));
  }

  /* whitening */
  for(i = 0; i < 4; i++){
    ml[i + 4]  ^= aurora512conml[i];
    mr[i + 4]  ^= aurora512conmr[i];
  }
  xl[0] ^= ml[0];
  xl[1] ^= ml[1];
  xl[2] ^= ml[2];
  xl[3] ^= ml[3];
  xl[4] ^= (ml[4] << 1) ^ (ml[5] >> 31) ^ aurora512concl[0];
  xl[5] ^= (ml[5] << 1) ^ (ml[4] >> 31) ^ aurora512concl[1];
  xl[6] ^= ml[6] ^ aurora512concl[2];
  xl[7] ^= ml[7] ^ aurora512concl[3] ^ mask;

  xr[0] ^= ml[0];
  xr[1] ^= ml[1];
  xr[2] ^= ml[2];
  xr[3] ^= ml[3];
  xr[4] ^= (ml[4] << 1) ^ (ml[5] >> 31) ^ aurora512concr[0];
  xr[5] ^= (ml[5] << 1) ^ (ml[4] >> 31) ^ aurora512concr[1];
  xr[6] ^= ml[6] ^ aurora512concr[2];
  xr[7] ^= ml[7] ^ aurora512concr[3] ^ mask;

//NASTAVENIE RUND:
#ifndef _LOOPED /* = _LOOP_UNROLL */
  if (rounds >= 1) AURORA512_2ROUNDS(1); /* R1,  R2  */
  if (rounds >= 3) AURORA512_2ROUNDS(2); /* R3,  R4  */
  if (rounds >= 5) AURORA512_2ROUNDS(3); /* R5,  R6  */
  if (rounds >= 7) AURORA512_2ROUNDS(4); /* R7,  R8  */
  if (rounds >= 9) AURORA512_2ROUNDS(5); /* R9,  R10 */
  if (rounds >= 11) AURORA512_2ROUNDS(6); /* R11, R12 */
  if (rounds >= 13) AURORA512_2ROUNDS(7); /* R13, R14 */
  if (rounds >= 15) AURORA512_2ROUNDS(8); /* R15, R16 */
#else /* _LOOPED */
  int realRounds = 8;
  if (rounds >= 1) realRounds = 1;
  if (rounds >= 3) realRounds = 2;
  if (rounds >= 5) realRounds = 3;
  if (rounds >= 7) realRounds = 4;
  if (rounds >= 9) realRounds = 5;
  if (rounds >= 11) realRounds = 6;
  if (rounds >= 13) realRounds = 7;
  if (rounds >= 15) realRounds = 8;
  for(i = 1; i <= realRounds; i++){
    AURORA512_2ROUNDS(i);
  }
#endif

  /* final round (R17) */

  /* CP_L */
  AURORA_F32(fcpl[0], xl[4], xl[7], xl[6], xl[5], F1_0, F1_1, F1_2, F1_3);
  AURORA_F32(fcpl[1], xl[5], xl[4], xl[7], xl[6], F0_0, F0_1, F0_2, F0_3);
  AURORA_F32(fcpl[2], xl[6], xl[5], xl[4], xl[7], F1_0, F1_1, F1_2, F1_3);
  AURORA_F32(fcpl[3], xl[7], xl[6], xl[5], xl[4], F0_0, F0_1, F0_2, F0_3);

  /* XORing after F + XORing after CP_L */
  hctx->h[1] ^= fcpl[0] ^ xl[1] ^ (mr[4] >> 1) ^ (mr[5] << 31);
  hctx->h[3] ^= fcpl[1] ^ xl[2] ^ (mr[5] >> 1) ^ (mr[4] << 31);
  hctx->h[5] ^= fcpl[2] ^ xl[3] ^ mr[6];
  hctx->h[7] ^= fcpl[3] ^ xl[0] ^ mr[7];

  hctx->h[0] ^= fcpl[0] ^ mr[0];
  hctx->h[2] ^= fcpl[1] ^ mr[1];
  hctx->h[4] ^= fcpl[2] ^ mr[2];
  hctx->h[6] ^= fcpl[3] ^ mr[3];

  /* CP_R */
  AURORA_F32(fcpr[0], xr[4], xr[7], xr[6], xr[5], F1_1, F1_2, F1_3, F1_0);
  AURORA_F32(fcpr[1], xr[5], xr[4], xr[7], xr[6], F0_3, F0_0, F0_1, F0_2);
  AURORA_F32(fcpr[2], xr[6], xr[5], xr[4], xr[7], F1_1, F1_2, F1_3, F1_0);
  AURORA_F32(fcpr[3], xr[7], xr[6], xr[5], xr[4], F0_3, F0_0, F0_1, F0_2);

  /* XORing after F + XORing after CP_R */
  hctx->h[9]  ^= fcpr[0] ^ xr[1] ^ (mr[4] >> 1) ^ (mr[5] << 31);
  hctx->h[11] ^= fcpr[1] ^ xr[2] ^ (mr[5] >> 1) ^ (mr[4] << 31);
  hctx->h[13] ^= fcpr[2] ^ xr[3] ^ mr[6];
  hctx->h[15] ^= fcpr[3] ^ xr[0] ^ mr[7];

  hctx->h[8]  ^= fcpr[0] ^ mr[0];
  hctx->h[10] ^= fcpr[1] ^ mr[1];
  hctx->h[12] ^= fcpr[2] ^ mr[2];
  hctx->h[14] ^= fcpr[3] ^ mr[3];
}

/*
 * Aurora512MF
 *
 * mixing function for AURORA-384/512
 */
void Aurora::Aurora512MF(AURORA_CTX *hctx, const AURORA_UINT32 mask, const int rounds)
{
  BitSequence m[64];
  int i;

  for(i = 0; i < 16; i++){
    AURORA_WRITE32BIT(m + i * 4, hctx->h[i]);
  }
  Aurora512CF(hctx, m, mask, rounds);
}


/*=============================================================================
 * AURORA-224/256 hash function
 *===========================================================================*/

/*
 * AuroraInit0()
 *
 * initialization function for AURORA-256 and AURORA-512
 * input and output: hctx
 */
void Aurora::AuroraInit0(AURORA_CTX *hctx)
{
  int i;

  for(i = 0; i < 16; i++){
    hctx->h[i] = 0U;
  }
  hctx->blk_num = 0U;
  hctx->cnt = 0;
  hctx->blk_idx = 0U;
}

/*
 * AuroraInit1()
 *
 * initialization function for AURORA-224 and AURORA-384
 * input and output: hctx
 */
void Aurora::AuroraInit1(AURORA_CTX *hctx)
{
  int i;

  for(i = 0; i < 16; i++){
    hctx->h[i] = 0xffffffffU;
  }
  hctx->blk_num = 0U;
  hctx->cnt = 0;
  hctx->blk_idx = 0;
}

/*
 * Aurora256Update()
 *
 * updating function for AURORA-256 and AURORA-224
 *
 * input  : msg (msg_bitlen [bits])
 *          msg_bitlen
 *          hctx
 * output : hctx (updated context)
 */
void Aurora::Aurora256Update(AURORA_CTX *hctx, const BitSequence *msg, DataLength msg_bitlen, const int rounds)
{
  int byte_cnt = hctx->cnt / 8;
  DataLength msg_bytelen = msg_bitlen >> 3;
  DataLength msg_blklen;
  int bit_ofs = hctx->cnt & 0x07;
  int msg_ofs = ((int) (msg_bitlen & 0x07U) + bit_ofs) & 0x07;
  BitSequence tmp;
  int i;

  if((hctx->cnt < 0) || (hctx->cnt >= 512)){
    return ;
  }

  if(bit_ofs){
    /* previous data is not byte aligned */
    tmp = hctx->buff[byte_cnt];
    hctx->cnt -= bit_ofs;
    while(msg_bytelen--){
      hctx->buff[byte_cnt++] = tmp | (*msg >> bit_ofs);
      tmp = *msg++ << (8 - bit_ofs);
      hctx->cnt += 8;
      if(hctx->cnt >= 64){
	hctx->cnt = 0;
	byte_cnt = 0;
	hctx->blk_num++;
	Aurora256CF(hctx, hctx->buff, 0x00000000U, rounds);
      }
    }
    hctx->buff[byte_cnt] = tmp & (0xffU << (8 - msg_ofs));
    hctx->cnt += msg_ofs;
  }else{
    /* previous data is byte aligned */
    /* computation for extra block */
    if((byte_cnt) && (msg_bytelen >= (DataLength) (AURORA_BLK_SIZE - byte_cnt))){
      msg_bytelen -= (DataLength) (AURORA_BLK_SIZE - byte_cnt);
      for(i = byte_cnt; i < AURORA_BLK_SIZE; i++){
	hctx->buff[i] = *msg++;
      }
      hctx->cnt = 0;
      byte_cnt = 0;
      hctx->blk_num++;
      Aurora256CF(hctx, hctx->buff, 0x00000000U, rounds);
    }
    
    /* main computation */
    msg_blklen = msg_bytelen / (DataLength) AURORA_BLK_SIZE;
    hctx->blk_num += msg_blklen;
    while(msg_blklen--){
      Aurora256CF(hctx, msg, 0x00000000U, rounds);
      msg += AURORA_BLK_SIZE;
    }
    
    msg_bytelen %= (DataLength) AURORA_BLK_SIZE; /* remaining bytes */
    
    /* copy remaining data to local buffer */
    hctx->cnt += (int) (msg_bytelen * 8);
    while(msg_bytelen--){
      hctx->buff[byte_cnt++] = *msg++;
    }

    if(msg_bitlen & 0x07U){
      /* input data is not aligned */
      hctx->buff[byte_cnt] = (*msg & (0xffU << (8 - (int) (msg_bitlen & 0x07U))));
      hctx->cnt += (int) (msg_bitlen & 0x07U);
    }
  }
}
  
/*
 * Aurora256Final()
 *
 * finalization function for AURORA-256
 *
 * input  : hctx
 * output : digest
 */
void Aurora::Aurora256Final(BitSequence *digest, AURORA_CTX *hctx, const int rounds)
{
  int byte_cnt = hctx->cnt / 8;
  int i;

  if((hctx->cnt < 0) || (hctx->cnt >= 512)){
    return ;
  }

  if(0 != hctx->cnt){
    hctx->blk_num++;
  }

  if(hctx->cnt & 0x07){
    hctx->buff[byte_cnt++] |= 0x80U >> (hctx->cnt & 0x07);
  }else{
    hctx->buff[byte_cnt++] = 0x80U;
  }

  if(hctx->cnt > 447){
    /* computation for extra block */
    for(i = byte_cnt; i < AURORA_BLK_SIZE; i++){
      hctx->buff[i] = 0U;
    }
    Aurora256CF(hctx, hctx->buff, 0x00000000U, rounds);
    byte_cnt = 0;
  }

  /* computation for final block */
  for(i = byte_cnt; i < (AURORA_BLK_SIZE - 8); i++){
    hctx->buff[i] = 0U;
  }

  
  AURORA_WRITE64BIT(hctx->buff + AURORA_BLK_SIZE - 8, hctx->blk_num);

  Aurora256CF(hctx, hctx->buff, 0x01010101U, rounds);

  AURORA_WRITE256BIT(digest, hctx->h[0], hctx->h[1], hctx->h[2], hctx->h[3], hctx->h[4], hctx->h[5], hctx->h[6], hctx->h[7]);
}

/*
 * Aurora224Final()
 *
 * finalization function for AURORA-224
 *
 * input  : hctx
 * output : digest
 */
void Aurora::Aurora224Final(BitSequence *digest, AURORA_CTX *hctx, const int rounds)
{
  BitSequence digest256[AURORA256_DIGEST_SIZE];
  int cnt = 0;
  int i, j;

  Aurora256Final(digest256, hctx, rounds);
  for(i = 0; i < 4; i++){
    for(j = 0; j < 7; j++){
      *digest++ = digest256[cnt++];
    }
    cnt++;
  }
}


/*=============================================================================
 * AURORA-384/512 hash function
 *===========================================================================*/

/*
 * Aurora512Update()
 *
 * updating function for AURORA-512 and AURORA-384
 *
 * input  : msg (msg_bitlen [bits])
 *          msg_bitlen
 *          hctx
 * output : hctx (updated context)
 */
void Aurora::Aurora512Update(AURORA_CTX *hctx, const BitSequence *msg, DataLength msg_bitlen, const int rounds)
{
  int byte_cnt = hctx->cnt / 8;
  DataLength msg_bytelen = msg_bitlen >> 3;
  DataLength msg_blklen;
  int bit_ofs = hctx->cnt & 0x07;
  int msg_ofs = ((int) (msg_bitlen & 0x07U) + bit_ofs) & 0x07;
  BitSequence tmp;
  int i;

  if((hctx->cnt < 0) || (hctx->cnt >= 512)){
    return ;
  }

  if(bit_ofs){
    /* previous data is not byte aligned */
    tmp = hctx->buff[byte_cnt];
    hctx->cnt -= bit_ofs;
    while(msg_bytelen--){
      hctx->buff[byte_cnt++] = tmp | (*msg >> bit_ofs);
      tmp = *msg++ << (8 - bit_ofs);
      hctx->cnt += 8;
      if(hctx->cnt >= 64){
	hctx->cnt = 0;
	byte_cnt = 0;
	hctx->blk_num++;
	if(hctx->blk_idx >= 0x08080808U){
	  /* mixing function */
	  Aurora512MF(hctx, hctx->blk_idx, rounds);
	  hctx->blk_idx = 0U;
	}
	Aurora512CF(hctx, hctx->buff, hctx->blk_idx, rounds);
	hctx->blk_idx += 0x01010101U;
      }
    }
    hctx->buff[byte_cnt] = tmp & (0xffU << (8 - msg_ofs));
    hctx->cnt += msg_ofs;
  }else{
    /* previous data is byte aligned */
    /* computation for extra block */
    if((byte_cnt) && (msg_bytelen >= (DataLength) (AURORA_BLK_SIZE - byte_cnt))){
      msg_bytelen -= (DataLength) (AURORA_BLK_SIZE - byte_cnt);
      for(i = byte_cnt; i < AURORA_BLK_SIZE; i++){
	hctx->buff[i] = *msg++;
      }
      hctx->cnt = 0;
      byte_cnt = 0;
      hctx->blk_num++;
      if(hctx->blk_idx >= 0x08080808U){
	/* mixing function */
	Aurora512MF(hctx, hctx->blk_idx, rounds);
	hctx->blk_idx = 0U;
      }
      Aurora512CF(hctx, hctx->buff, hctx->blk_idx, rounds);
      hctx->blk_idx += 0x01010101U;
    }
    
    /* main computation */
    msg_blklen = msg_bytelen / (DataLength) AURORA_BLK_SIZE;
    hctx->blk_num += msg_blklen;
    while(msg_blklen--){
      if(hctx->blk_idx >= 0x08080808U){
	/* mixing function */
	Aurora512MF(hctx, hctx->blk_idx, rounds);
	hctx->blk_idx = 0U;
      }
      Aurora512CF(hctx, msg, hctx->blk_idx, rounds);
      hctx->blk_idx += 0x01010101U;
      msg += AURORA_BLK_SIZE;
    }
    
    msg_bytelen %= (DataLength) AURORA_BLK_SIZE; /* remaining bytes */
    
    /* copy remaining data to local buffer */
    hctx->cnt += (int) (msg_bytelen * 8);
    while(msg_bytelen--){
      hctx->buff[byte_cnt++] = *msg++;
    }

    if(msg_bitlen & 0x07U){
      /* input data is not byte aligned */
      hctx->buff[byte_cnt] = (*msg & (0xffU << (8 - (int) (msg_bitlen & 0x07U))));
      hctx->cnt += (int) (msg_bitlen & 0x07U);
    }
  }
}

/*
 * Aurora512Final()
 *
 * finalization function for AURORA-512
 *
 * input  : hctx
 * output : digest
 */
void Aurora::Aurora512Final(BitSequence *digest, AURORA_CTX *hctx, const int rounds)
{
  int byte_cnt = hctx->cnt / 8;
  int i;

  if((hctx->cnt < 0) || (hctx->cnt >= 512)){
    return ;
  }

  if(0 != hctx->cnt){
    hctx->blk_num++;
  }

  if(hctx->cnt & 0x07){
    hctx->buff[byte_cnt++] |= 0x80U >> (hctx->cnt & 0x07);
  }else{
    hctx->buff[byte_cnt++] = 0x80U;
  }

  if(hctx->cnt > 447){
    /* computation for extra block */
    for(i = byte_cnt; i < AURORA_BLK_SIZE; i++){
      hctx->buff[i] = 0U;
    }
    if(hctx->blk_idx >= 0x08080808U){
      /* mixing function */
      Aurora512MF(hctx, hctx->blk_idx, rounds);
      hctx->blk_idx = 0U;
    }
    Aurora512CF(hctx, hctx->buff, hctx->blk_idx, rounds);
    hctx->blk_idx += 0x01010101U;
    byte_cnt = 0;
  }

  /* computation for final block */
  for(i = byte_cnt; i < (AURORA_BLK_SIZE - 8); i++){
    hctx->buff[i] = 0U;
  }

  AURORA_WRITE64BIT(hctx->buff + AURORA_BLK_SIZE - 8, hctx->blk_num);

  /* final compression */
  if(hctx->blk_idx >= 0x08080808U){
    /* mixing function */
    Aurora512MF(hctx, hctx->blk_idx, rounds);
    hctx->blk_idx = 0U;
  }
  Aurora512CF(hctx, hctx->buff, hctx->blk_idx, rounds);
  hctx->blk_idx += 0x01010101U;

  /* final mixing */
  Aurora512MF(hctx, 0x09090909U, rounds);

  AURORA_WRITE256BIT(digest, hctx->h[0], hctx->h[1], hctx->h[2], hctx->h[3], hctx->h[4], hctx->h[5], hctx->h[6], hctx->h[7]);
  AURORA_WRITE256BIT(digest + 32, hctx->h[8], hctx->h[9], hctx->h[10], hctx->h[11], hctx->h[12], hctx->h[13], hctx->h[14], hctx->h[15]);
}

/*
 * Aurora384Final()
 *
 * finalization function for AURORA-384
 *
 * input  : hctx
 * output : digest
 */
void Aurora::Aurora384Final(BitSequence *digest, AURORA_CTX *hctx, const int rounds)
{
  BitSequence digest512[AURORA512_DIGEST_SIZE];
  int cnt = 0;
  int i, j;

  Aurora512Final(digest512, hctx, rounds);
  for(i = 0; i < 8; i++){
    for(j = 0; j < 6; j++){
      *digest++ = digest512[cnt++];
    }
    cnt += 2;
  }
}


/*=============================================================================
 * main
 *===========================================================================*/

/*
 * Init()
 *
 * input   : state
 *           hashbitlen
 * output  : state (updated)
 * returns : BAD_HASHBITLEN (hashbitlen is invalid) or SUCCESS
 */
int Aurora::Init(int hashbitlen)
{
  if(AURORA256_DIGEST_SIZE_BIT == hashbitlen){
    /* AURORA-256 */
    AuroraInit0(&auroraState.ctx);
    auroraState.HashUpdate = Aurora256Update;
    auroraState.HashFinal = Aurora256Final;
  }else if(AURORA224_DIGEST_SIZE_BIT == hashbitlen){
    /* AURORA-224 */
    AuroraInit1(&auroraState.ctx);
    auroraState.HashUpdate = Aurora256Update;
    auroraState.HashFinal = Aurora224Final;
  }else if(AURORA512_DIGEST_SIZE_BIT == hashbitlen){
    /* AURORA-512 */
    AuroraInit0(&auroraState.ctx);
    auroraState.HashUpdate = Aurora512Update;
    auroraState.HashFinal = Aurora512Final;
  }else if(AURORA384_DIGEST_SIZE_BIT == hashbitlen){
    /* AURORA-384 */
    AuroraInit1(&auroraState.ctx);
    auroraState.HashUpdate = Aurora512Update;
    auroraState.HashFinal = Aurora384Final;
  }else{
    /* invalid hashbitlen */
    return BAD_HASHBITLEN;
  }

  auroraState.hashbitlen = hashbitlen;

  return SUCCESS;
}

/*
 * Update()
 *
 * input   : state
 *           data
 *           databitlen
 * output  : state (updated)
 * returns : SUCCESS
 */
int Aurora::Update(const BitSequence *data, DataLength databitlen)
{
  auroraState.HashUpdate(&auroraState.ctx, data, databitlen, auroraNumRounds);

  return SUCCESS;
}

/*
 * Final()
 *
 * input   : state
 * output  : hashval
 * returns : SUCCESS
 */
int Aurora::Final(BitSequence *hashval)
{
	auroraState.HashFinal(hashval, &auroraState.ctx, auroraNumRounds);

  return SUCCESS;
}

/*
 * Hash()
 *
 * input   : hashbitlen
 *           data
 *           databitlen
 * output  : hashval
 * returns : BAD_HASHBITLEN, FAIL or SUCCESS
 */
int Aurora::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
  //hashState state;

  if(BAD_HASHBITLEN == Aurora::Init(hashbitlen)){
    return BAD_HASHBITLEN;
  }
  Aurora::Update(data, databitlen);
  Aurora::Final(hashval);

  return SUCCESS;
}

Aurora::Aurora(const int numRounds) {
	if (numRounds == -1) {
		auroraNumRounds = AURORA_ROUNDS;
	} else {
		auroraNumRounds = numRounds;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////