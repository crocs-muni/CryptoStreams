// Header files
#include <stdlib.h>   
#include <stdio.h> 
#include "Shamata_sha3.h"

#pragma warning(disable:4715)
#pragma warning(disable:4244)

// MDS boxes
unsigned int SHAMATA_MHH[256]={0x00000000,0x02010103,0x04020206,0x06030305,0x0804040c,0x0a05050f,0x0c06060a,0x0e070709,
                       0x10080818,0x1209091b,0x140a0a1e,0x160b0b1d,0x180c0c14,0x1a0d0d17,0x1c0e0e12,0x1e0f0f11,
					   0x20101030,0x22111133,0x24121236,0x26131335,0x2814143c,0x2a15153f,0x2c16163a,0x2e171739,
					   0x30181828,0x3219192b,0x341a1a2e,0x361b1b2d,0x381c1c24,0x3a1d1d27,0x3c1e1e22,0x3e1f1f21,
					   0x40202060,0x42212163,0x44222266,0x46232365,0x4824246c,0x4a25256f,0x4c26266a,0x4e272769,
					   0x50282878,0x5229297b,0x542a2a7e,0x562b2b7d,0x582c2c74,0x5a2d2d77,0x5c2e2e72,0x5e2f2f71,
					   0x60303050,0x62313153,0x64323256,0x66333355,0x6834345c,0x6a35355f,0x6c36365a,0x6e373759,
					   0x70383848,0x7239394b,0x743a3a4e,0x763b3b4d,0x783c3c44,0x7a3d3d47,0x7c3e3e42,0x7e3f3f41,
					   0x804040c0,0x824141c3,0x844242c6,0x864343c5,0x884444cc,0x8a4545cf,0x8c4646ca,0x8e4747c9,
					   0x904848d8,0x924949db,0x944a4ade,0x964b4bdd,0x984c4cd4,0x9a4d4dd7,0x9c4e4ed2,0x9e4f4fd1,
					   0xa05050f0,0xa25151f3,0xa45252f6,0xa65353f5,0xa85454fc,0xaa5555ff,0xac5656fa,0xae5757f9,
					   0xb05858e8,0xb25959eb,0xb45a5aee,0xb65b5bed,0xb85c5ce4,0xba5d5de7,0xbc5e5ee2,0xbe5f5fe1,
					   0xc06060a0,0xc26161a3,0xc46262a6,0xc66363a5,0xc86464ac,0xca6565af,0xcc6666aa,0xce6767a9,
					   0xd06868b8,0xd26969bb,0xd46a6abe,0xd66b6bbd,0xd86c6cb4,0xda6d6db7,0xdc6e6eb2,0xde6f6fb1,
					   0xe0707090,0xe2717193,0xe4727296,0xe6737395,0xe874749c,0xea75759f,0xec76769a,0xee777799,
					   0xf0787888,0xf279798b,0xf47a7a8e,0xf67b7b8d,0xf87c7c84,0xfa7d7d87,0xfc7e7e82,0xfe7f7f81,
					   0x1b80809b,0x19818198,0x1f82829d,0x1d83839e,0x13848497,0x11858594,0x17868691,0x15878792,
					   0x0b888883,0x09898980,0x0f8a8a85,0x0d8b8b86,0x038c8c8f,0x018d8d8c,0x078e8e89,0x058f8f8a,
					   0x3b9090ab,0x399191a8,0x3f9292ad,0x3d9393ae,0x339494a7,0x319595a4,0x379696a1,0x359797a2,
					   0x2b9898b3,0x299999b0,0x2f9a9ab5,0x2d9b9bb6,0x239c9cbf,0x219d9dbc,0x279e9eb9,0x259f9fba,
					   0x5ba0a0fb,0x59a1a1f8,0x5fa2a2fd,0x5da3a3fe,0x53a4a4f7,0x51a5a5f4,0x57a6a6f1,0x55a7a7f2,
					   0x4ba8a8e3,0x49a9a9e0,0x4faaaae5,0x4dababe6,0x43acacef,0x41adadec,0x47aeaee9,0x45afafea,
					   0x7bb0b0cb,0x79b1b1c8,0x7fb2b2cd,0x7db3b3ce,0x73b4b4c7,0x71b5b5c4,0x77b6b6c1,0x75b7b7c2,
					   0x6bb8b8d3,0x69b9b9d0,0x6fbabad5,0x6dbbbbd6,0x63bcbcdf,0x61bdbddc,0x67bebed9,0x65bfbfda,
					   0x9bc0c05b,0x99c1c158,0x9fc2c25d,0x9dc3c35e,0x93c4c457,0x91c5c554,0x97c6c651,0x95c7c752,
					   0x8bc8c843,0x89c9c940,0x8fcaca45,0x8dcbcb46,0x83cccc4f,0x81cdcd4c,0x87cece49,0x85cfcf4a,
					   0xbbd0d06b,0xb9d1d168,0xbfd2d26d,0xbdd3d36e,0xb3d4d467,0xb1d5d564,0xb7d6d661,0xb5d7d762,
					   0xabd8d873,0xa9d9d970,0xafdada75,0xaddbdb76,0xa3dcdc7f,0xa1dddd7c,0xa7dede79,0xa5dfdf7a,
					   0xdbe0e03b,0xd9e1e138,0xdfe2e23d,0xdde3e33e,0xd3e4e437,0xd1e5e534,0xd7e6e631,0xd5e7e732,
					   0xcbe8e823,0xc9e9e920,0xcfeaea25,0xcdebeb26,0xc3ecec2f,0xc1eded2c,0xc7eeee29,0xc5efef2a,
					   0xfbf0f00b,0xf9f1f108,0xfff2f20d,0xfdf3f30e,0xf3f4f407,0xf1f5f504,0xf7f6f601,0xf5f7f702,
					   0xebf8f813,0xe9f9f910,0xeffafa15,0xedfbfb16,0xe3fcfc1f,0xe1fdfd1c,0xe7fefe19,0xe5ffff1a};

unsigned int SHAMATA_MHL[256]={0x00000000,0x03020101,0x06040202,0x05060303,0x0c080404,0x0f0a0505,0x0a0c0606,0x090e0707,
                       0x18100808,0x1b120909,0x1e140a0a,0x1d160b0b,0x14180c0c,0x171a0d0d,0x121c0e0e,0x111e0f0f,
                       0x30201010,0x33221111,0x36241212,0x35261313,0x3c281414,0x3f2a1515,0x3a2c1616,0x392e1717,
                       0x28301818,0x2b321919,0x2e341a1a,0x2d361b1b,0x24381c1c,0x273a1d1d,0x223c1e1e,0x213e1f1f,
                       0x60402020,0x63422121,0x66442222,0x65462323,0x6c482424,0x6f4a2525,0x6a4c2626,0x694e2727,
                       0x78502828,0x7b522929,0x7e542a2a,0x7d562b2b,0x74582c2c,0x775a2d2d,0x725c2e2e,0x715e2f2f,
                       0x50603030,0x53623131,0x56643232,0x55663333,0x5c683434,0x5f6a3535,0x5a6c3636,0x596e3737,
                       0x48703838,0x4b723939,0x4e743a3a,0x4d763b3b,0x44783c3c,0x477a3d3d,0x427c3e3e,0x417e3f3f,
                       0xc0804040,0xc3824141,0xc6844242,0xc5864343,0xcc884444,0xcf8a4545,0xca8c4646,0xc98e4747,
                       0xd8904848,0xdb924949,0xde944a4a,0xdd964b4b,0xd4984c4c,0xd79a4d4d,0xd29c4e4e,0xd19e4f4f,
                       0xf0a05050,0xf3a25151,0xf6a45252,0xf5a65353,0xfca85454,0xffaa5555,0xfaac5656,0xf9ae5757,
                       0xe8b05858,0xebb25959,0xeeb45a5a,0xedb65b5b,0xe4b85c5c,0xe7ba5d5d,0xe2bc5e5e,0xe1be5f5f,
                       0xa0c06060,0xa3c26161,0xa6c46262,0xa5c66363,0xacc86464,0xafca6565,0xaacc6666,0xa9ce6767,
                       0xb8d06868,0xbbd26969,0xbed46a6a,0xbdd66b6b,0xb4d86c6c,0xb7da6d6d,0xb2dc6e6e,0xb1de6f6f,
                       0x90e07070,0x93e27171,0x96e47272,0x95e67373,0x9ce87474,0x9fea7575,0x9aec7676,0x99ee7777,
                       0x88f07878,0x8bf27979,0x8ef47a7a,0x8df67b7b,0x84f87c7c,0x87fa7d7d,0x82fc7e7e,0x81fe7f7f,
                       0x9b1b8080,0x98198181,0x9d1f8282,0x9e1d8383,0x97138484,0x94118585,0x91178686,0x92158787,
                       0x830b8888,0x80098989,0x850f8a8a,0x860d8b8b,0x8f038c8c,0x8c018d8d,0x89078e8e,0x8a058f8f,
                       0xab3b9090,0xa8399191,0xad3f9292,0xae3d9393,0xa7339494,0xa4319595,0xa1379696,0xa2359797,
                       0xb32b9898,0xb0299999,0xb52f9a9a,0xb62d9b9b,0xbf239c9c,0xbc219d9d,0xb9279e9e,0xba259f9f,
                       0xfb5ba0a0,0xf859a1a1,0xfd5fa2a2,0xfe5da3a3,0xf753a4a4,0xf451a5a5,0xf157a6a6,0xf255a7a7,
                       0xe34ba8a8,0xe049a9a9,0xe54faaaa,0xe64dabab,0xef43acac,0xec41adad,0xe947aeae,0xea45afaf,
                       0xcb7bb0b0,0xc879b1b1,0xcd7fb2b2,0xce7db3b3,0xc773b4b4,0xc471b5b5,0xc177b6b6,0xc275b7b7,
                       0xd36bb8b8,0xd069b9b9,0xd56fbaba,0xd66dbbbb,0xdf63bcbc,0xdc61bdbd,0xd967bebe,0xda65bfbf,
                       0x5b9bc0c0,0x5899c1c1,0x5d9fc2c2,0x5e9dc3c3,0x5793c4c4,0x5491c5c5,0x5197c6c6,0x5295c7c7,
                       0x438bc8c8,0x4089c9c9,0x458fcaca,0x468dcbcb,0x4f83cccc,0x4c81cdcd,0x4987cece,0x4a85cfcf,
                       0x6bbbd0d0,0x68b9d1d1,0x6dbfd2d2,0x6ebdd3d3,0x67b3d4d4,0x64b1d5d5,0x61b7d6d6,0x62b5d7d7,
                       0x73abd8d8,0x70a9d9d9,0x75afdada,0x76addbdb,0x7fa3dcdc,0x7ca1dddd,0x79a7dede,0x7aa5dfdf,
                       0x3bdbe0e0,0x38d9e1e1,0x3ddfe2e2,0x3edde3e3,0x37d3e4e4,0x34d1e5e5,0x31d7e6e6,0x32d5e7e7,
                       0x23cbe8e8,0x20c9e9e9,0x25cfeaea,0x26cdebeb,0x2fc3ecec,0x2cc1eded,0x29c7eeee,0x2ac5efef,
                       0x0bfbf0f0,0x08f9f1f1,0x0dfff2f2,0x0efdf3f3,0x07f3f4f4,0x04f1f5f5,0x01f7f6f6,0x02f5f7f7,
					   0x13ebf8f8,0x10e9f9f9,0x15effafa,0x16edfbfb,0x1fe3fcfc,0x1ce1fdfd,0x19e7fefe,0x1ae5ffff};

unsigned int SHAMATA_MLH[256]={0x00000000,0x01030201,0x02060402,0x03050603,0x040c0804,0x050f0a05,0x060a0c06,0x07090e07,
                       0x08181008,0x091b1209,0x0a1e140a,0x0b1d160b,0x0c14180c,0x0d171a0d,0x0e121c0e,0x0f111e0f,
                       0x10302010,0x11332211,0x12362412,0x13352613,0x143c2814,0x153f2a15,0x163a2c16,0x17392e17,
                       0x18283018,0x192b3219,0x1a2e341a,0x1b2d361b,0x1c24381c,0x1d273a1d,0x1e223c1e,0x1f213e1f,
                       0x20604020,0x21634221,0x22664422,0x23654623,0x246c4824,0x256f4a25,0x266a4c26,0x27694e27,
                       0x28785028,0x297b5229,0x2a7e542a,0x2b7d562b,0x2c74582c,0x2d775a2d,0x2e725c2e,0x2f715e2f,
                       0x30506030,0x31536231,0x32566432,0x33556633,0x345c6834,0x355f6a35,0x365a6c36,0x37596e37,
                       0x38487038,0x394b7239,0x3a4e743a,0x3b4d763b,0x3c44783c,0x3d477a3d,0x3e427c3e,0x3f417e3f,
                       0x40c08040,0x41c38241,0x42c68442,0x43c58643,0x44cc8844,0x45cf8a45,0x46ca8c46,0x47c98e47,
                       0x48d89048,0x49db9249,0x4ade944a,0x4bdd964b,0x4cd4984c,0x4dd79a4d,0x4ed29c4e,0x4fd19e4f,
                       0x50f0a050,0x51f3a251,0x52f6a452,0x53f5a653,0x54fca854,0x55ffaa55,0x56faac56,0x57f9ae57,
                       0x58e8b058,0x59ebb259,0x5aeeb45a,0x5bedb65b,0x5ce4b85c,0x5de7ba5d,0x5ee2bc5e,0x5fe1be5f,
                       0x60a0c060,0x61a3c261,0x62a6c462,0x63a5c663,0x64acc864,0x65afca65,0x66aacc66,0x67a9ce67,
                       0x68b8d068,0x69bbd269,0x6abed46a,0x6bbdd66b,0x6cb4d86c,0x6db7da6d,0x6eb2dc6e,0x6fb1de6f,
                       0x7090e070,0x7193e271,0x7296e472,0x7395e673,0x749ce874,0x759fea75,0x769aec76,0x7799ee77,
                       0x7888f078,0x798bf279,0x7a8ef47a,0x7b8df67b,0x7c84f87c,0x7d87fa7d,0x7e82fc7e,0x7f81fe7f,
                       0x809b1b80,0x81981981,0x829d1f82,0x839e1d83,0x84971384,0x85941185,0x86911786,0x87921587,
                       0x88830b88,0x89800989,0x8a850f8a,0x8b860d8b,0x8c8f038c,0x8d8c018d,0x8e89078e,0x8f8a058f,
                       0x90ab3b90,0x91a83991,0x92ad3f92,0x93ae3d93,0x94a73394,0x95a43195,0x96a13796,0x97a23597,
                       0x98b32b98,0x99b02999,0x9ab52f9a,0x9bb62d9b,0x9cbf239c,0x9dbc219d,0x9eb9279e,0x9fba259f,
                       0xa0fb5ba0,0xa1f859a1,0xa2fd5fa2,0xa3fe5da3,0xa4f753a4,0xa5f451a5,0xa6f157a6,0xa7f255a7,
                       0xa8e34ba8,0xa9e049a9,0xaae54faa,0xabe64dab,0xacef43ac,0xadec41ad,0xaee947ae,0xafea45af,
                       0xb0cb7bb0,0xb1c879b1,0xb2cd7fb2,0xb3ce7db3,0xb4c773b4,0xb5c471b5,0xb6c177b6,0xb7c275b7,
                       0xb8d36bb8,0xb9d069b9,0xbad56fba,0xbbd66dbb,0xbcdf63bc,0xbddc61bd,0xbed967be,0xbfda65bf,
                       0xc05b9bc0,0xc15899c1,0xc25d9fc2,0xc35e9dc3,0xc45793c4,0xc55491c5,0xc65197c6,0xc75295c7,
                       0xc8438bc8,0xc94089c9,0xca458fca,0xcb468dcb,0xcc4f83cc,0xcd4c81cd,0xce4987ce,0xcf4a85cf,
                       0xd06bbbd0,0xd168b9d1,0xd26dbfd2,0xd36ebdd3,0xd467b3d4,0xd564b1d5,0xd661b7d6,0xd762b5d7,
                       0xd873abd8,0xd970a9d9,0xda75afda,0xdb76addb,0xdc7fa3dc,0xdd7ca1dd,0xde79a7de,0xdf7aa5df,
                       0xe03bdbe0,0xe138d9e1,0xe23ddfe2,0xe33edde3,0xe437d3e4,0xe534d1e5,0xe631d7e6,0xe732d5e7,
                       0xe823cbe8,0xe920c9e9,0xea25cfea,0xeb26cdeb,0xec2fc3ec,0xed2cc1ed,0xee29c7ee,0xef2ac5ef,
                       0xf00bfbf0,0xf108f9f1,0xf20dfff2,0xf30efdf3,0xf407f3f4,0xf504f1f5,0xf601f7f6,0xf702f5f7,
					   0xf813ebf8,0xf910e9f9,0xfa15effa,0xfb16edfb,0xfc1fe3fc,0xfd1ce1fd,0xfe19e7fe,0xff1ae5ff};

unsigned int SHAMATA_MLL[256]={0x00000000,0x01010302,0x02020604,0x03030506,0x04040c08,0x05050f0a,0x06060a0c,0x0707090e,
					   0x08081810,0x09091b12,0x0a0a1e14,0x0b0b1d16,0x0c0c1418,0x0d0d171a,0x0e0e121c,0x0f0f111e,
					   0x10103020,0x11113322,0x12123624,0x13133526,0x14143c28,0x15153f2a,0x16163a2c,0x1717392e,
					   0x18182830,0x19192b32,0x1a1a2e34,0x1b1b2d36,0x1c1c2438,0x1d1d273a,0x1e1e223c,0x1f1f213e,
					   0x20206040,0x21216342,0x22226644,0x23236546,0x24246c48,0x25256f4a,0x26266a4c,0x2727694e,
					   0x28287850,0x29297b52,0x2a2a7e54,0x2b2b7d56,0x2c2c7458,0x2d2d775a,0x2e2e725c,0x2f2f715e,
					   0x30305060,0x31315362,0x32325664,0x33335566,0x34345c68,0x35355f6a,0x36365a6c,0x3737596e,
					   0x38384870,0x39394b72,0x3a3a4e74,0x3b3b4d76,0x3c3c4478,0x3d3d477a,0x3e3e427c,0x3f3f417e,
					   0x4040c080,0x4141c382,0x4242c684,0x4343c586,0x4444cc88,0x4545cf8a,0x4646ca8c,0x4747c98e,
					   0x4848d890,0x4949db92,0x4a4ade94,0x4b4bdd96,0x4c4cd498,0x4d4dd79a,0x4e4ed29c,0x4f4fd19e,
					   0x5050f0a0,0x5151f3a2,0x5252f6a4,0x5353f5a6,0x5454fca8,0x5555ffaa,0x5656faac,0x5757f9ae,
					   0x5858e8b0,0x5959ebb2,0x5a5aeeb4,0x5b5bedb6,0x5c5ce4b8,0x5d5de7ba,0x5e5ee2bc,0x5f5fe1be,
					   0x6060a0c0,0x6161a3c2,0x6262a6c4,0x6363a5c6,0x6464acc8,0x6565afca,0x6666aacc,0x6767a9ce,
					   0x6868b8d0,0x6969bbd2,0x6a6abed4,0x6b6bbdd6,0x6c6cb4d8,0x6d6db7da,0x6e6eb2dc,0x6f6fb1de,
					   0x707090e0,0x717193e2,0x727296e4,0x737395e6,0x74749ce8,0x75759fea,0x76769aec,0x777799ee,
					   0x787888f0,0x79798bf2,0x7a7a8ef4,0x7b7b8df6,0x7c7c84f8,0x7d7d87fa,0x7e7e82fc,0x7f7f81fe,
					   0x80809b1b,0x81819819,0x82829d1f,0x83839e1d,0x84849713,0x85859411,0x86869117,0x87879215,
					   0x8888830b,0x89898009,0x8a8a850f,0x8b8b860d,0x8c8c8f03,0x8d8d8c01,0x8e8e8907,0x8f8f8a05,
					   0x9090ab3b,0x9191a839,0x9292ad3f,0x9393ae3d,0x9494a733,0x9595a431,0x9696a137,0x9797a235,
					   0x9898b32b,0x9999b029,0x9a9ab52f,0x9b9bb62d,0x9c9cbf23,0x9d9dbc21,0x9e9eb927,0x9f9fba25,
					   0xa0a0fb5b,0xa1a1f859,0xa2a2fd5f,0xa3a3fe5d,0xa4a4f753,0xa5a5f451,0xa6a6f157,0xa7a7f255,
					   0xa8a8e34b,0xa9a9e049,0xaaaae54f,0xababe64d,0xacacef43,0xadadec41,0xaeaee947,0xafafea45,
					   0xb0b0cb7b,0xb1b1c879,0xb2b2cd7f,0xb3b3ce7d,0xb4b4c773,0xb5b5c471,0xb6b6c177,0xb7b7c275,
					   0xb8b8d36b,0xb9b9d069,0xbabad56f,0xbbbbd66d,0xbcbcdf63,0xbdbddc61,0xbebed967,0xbfbfda65,
					   0xc0c05b9b,0xc1c15899,0xc2c25d9f,0xc3c35e9d,0xc4c45793,0xc5c55491,0xc6c65197,0xc7c75295,
					   0xc8c8438b,0xc9c94089,0xcaca458f,0xcbcb468d,0xcccc4f83,0xcdcd4c81,0xcece4987,0xcfcf4a85,
					   0xd0d06bbb,0xd1d168b9,0xd2d26dbf,0xd3d36ebd,0xd4d467b3,0xd5d564b1,0xd6d661b7,0xd7d762b5,
					   0xd8d873ab,0xd9d970a9,0xdada75af,0xdbdb76ad,0xdcdc7fa3,0xdddd7ca1,0xdede79a7,0xdfdf7aa5,
					   0xe0e03bdb,0xe1e138d9,0xe2e23ddf,0xe3e33edd,0xe4e437d3,0xe5e534d1,0xe6e631d7,0xe7e732d5,
					   0xe8e823cb,0xe9e920c9,0xeaea25cf,0xebeb26cd,0xecec2fc3,0xeded2cc1,0xeeee29c7,0xefef2ac5,
					   0xf0f00bfb,0xf1f108f9,0xf2f20dff,0xf3f30efd,0xf4f407f3,0xf5f504f1,0xf6f601f7,0xf7f702f5,
					   0xf8f813eb,0xf9f910e9,0xfafa15ef,0xfbfb16ed,0xfcfc1fe3,0xfdfd1ce1,0xfefe19e7,0xffff1ae5};

// MDS boxes of AES with SubByte
unsigned int SHAMATA_Shh[256]={0xc66363a5,0xf87c7c84,0xee777799,0xf67b7b8d,0xfff2f20d,0xd66b6bbd,0xde6f6fb1,0x91c5c554,
                       0x60303050,0x02010103,0xce6767a9,0x562b2b7d,0xe7fefe19,0xb5d7d762,0x4dababe6,0xec76769a,
					   0x8fcaca45,0x1f82829d,0x89c9c940,0xfa7d7d87,0xeffafa15,0xb25959eb,0x8e4747c9,0xfbf0f00b,
					   0x41adadec,0xb3d4d467,0x5fa2a2fd,0x45afafea,0x239c9cbf,0x53a4a4f7,0xe4727296,0x9bc0c05b,
					   0x75b7b7c2,0xe1fdfd1c,0x3d9393ae,0x4c26266a,0x6c36365a,0x7e3f3f41,0xf5f7f702,0x83cccc4f,
					   0x6834345c,0x51a5a5f4,0xd1e5e534,0xf9f1f108,0xe2717193,0xabd8d873,0x62313153,0x2a15153f,
					   0x0804040c,0x95c7c752,0x46232365,0x9dc3c35e,0x30181828,0x379696a1,0x0a05050f,0x2f9a9ab5,
					   0x0e070709,0x24121236,0x1b80809b,0xdfe2e23d,0xcdebeb26,0x4e272769,0x7fb2b2cd,0xea75759f,
					   0x1209091b,0x1d83839e,0x582c2c74,0x341a1a2e,0x361b1b2d,0xdc6e6eb2,0xb45a5aee,0x5ba0a0fb,
					   0xa45252f6,0x763b3b4d,0xb7d6d661,0x7db3b3ce,0x5229297b,0xdde3e33e,0x5e2f2f71,0x13848497,
					   0xa65353f5,0xb9d1d168,0x00000000,0xc1eded2c,0x40202060,0xe3fcfc1f,0x79b1b1c8,0xb65b5bed,
					   0xd46a6abe,0x8dcbcb46,0x67bebed9,0x7239394b,0x944a4ade,0x984c4cd4,0xb05858e8,0x85cfcf4a,
					   0xbbd0d06b,0xc5efef2a,0x4faaaae5,0xedfbfb16,0x864343c5,0x9a4d4dd7,0x66333355,0x11858594,
					   0x8a4545cf,0xe9f9f910,0x04020206,0xfe7f7f81,0xa05050f0,0x783c3c44,0x259f9fba,0x4ba8a8e3,
					   0xa25151f3,0x5da3a3fe,0x804040c0,0x058f8f8a,0x3f9292ad,0x219d9dbc,0x70383848,0xf1f5f504,
					   0x63bcbcdf,0x77b6b6c1,0xafdada75,0x42212163,0x20101030,0xe5ffff1a,0xfdf3f30e,0xbfd2d26d,
					   0x81cdcd4c,0x180c0c14,0x26131335,0xc3ecec2f,0xbe5f5fe1,0x359797a2,0x884444cc,0x2e171739,
					   0x93c4c457,0x55a7a7f2,0xfc7e7e82,0x7a3d3d47,0xc86464ac,0xba5d5de7,0x3219192b,0xe6737395,
					   0xc06060a0,0x19818198,0x9e4f4fd1,0xa3dcdc7f,0x44222266,0x542a2a7e,0x3b9090ab,0x0b888883,
					   0x8c4646ca,0xc7eeee29,0x6bb8b8d3,0x2814143c,0xa7dede79,0xbc5e5ee2,0x160b0b1d,0xaddbdb76,
					   0xdbe0e03b,0x64323256,0x743a3a4e,0x140a0a1e,0x924949db,0x0c06060a,0x4824246c,0xb85c5ce4,
					   0x9fc2c25d,0xbdd3d36e,0x43acacef,0xc46262a6,0x399191a8,0x319595a4,0xd3e4e437,0xf279798b,
					   0xd5e7e732,0x8bc8c843,0x6e373759,0xda6d6db7,0x018d8d8c,0xb1d5d564,0x9c4e4ed2,0x49a9a9e0,
					   0xd86c6cb4,0xac5656fa,0xf3f4f407,0xcfeaea25,0xca6565af,0xf47a7a8e,0x47aeaee9,0x10080818,
					   0x6fbabad5,0xf0787888,0x4a25256f,0x5c2e2e72,0x381c1c24,0x57a6a6f1,0x73b4b4c7,0x97c6c651,
					   0xcbe8e823,0xa1dddd7c,0xe874749c,0x3e1f1f21,0x964b4bdd,0x61bdbddc,0x0d8b8b86,0x0f8a8a85,
					   0xe0707090,0x7c3e3e42,0x71b5b5c4,0xcc6666aa,0x904848d8,0x06030305,0xf7f6f601,0x1c0e0e12,
					   0xc26161a3,0x6a35355f,0xae5757f9,0x69b9b9d0,0x17868691,0x99c1c158,0x3a1d1d27,0x279e9eb9,
					   0xd9e1e138,0xebf8f813,0x2b9898b3,0x22111133,0xd26969bb,0xa9d9d970,0x078e8e89,0x339494a7,
					   0x2d9b9bb6,0x3c1e1e22,0x15878792,0xc9e9e920,0x87cece49,0xaa5555ff,0x50282878,0xa5dfdf7a,
					   0x038c8c8f,0x59a1a1f8,0x09898980,0x1a0d0d17,0x65bfbfda,0xd7e6e631,0x844242c6,0xd06868b8,
					   0x824141c3,0x299999b0,0x5a2d2d77,0x1e0f0f11,0x7bb0b0cb,0xa85454fc,0x6dbbbbd6,0x2c16163a};

unsigned int SHAMATA_Shl[256]={0xa5c66363,0x84f87c7c,0x99ee7777,0x8df67b7b,0x0dfff2f2,0xbdd66b6b,0xb1de6f6f,0x5491c5c5,
                       0x50603030,0x03020101,0xa9ce6767,0x7d562b2b,0x19e7fefe,0x62b5d7d7,0xe64dabab,0x9aec7676,
					   0x458fcaca,0x9d1f8282,0x4089c9c9,0x87fa7d7d,0x15effafa,0xebb25959,0xc98e4747,0x0bfbf0f0,
					   0xec41adad,0x67b3d4d4,0xfd5fa2a2,0xea45afaf,0xbf239c9c,0xf753a4a4,0x96e47272,0x5b9bc0c0,
					   0xc275b7b7,0x1ce1fdfd,0xae3d9393,0x6a4c2626,0x5a6c3636,0x417e3f3f,0x02f5f7f7,0x4f83cccc,
					   0x5c683434,0xf451a5a5,0x34d1e5e5,0x08f9f1f1,0x93e27171,0x73abd8d8,0x53623131,0x3f2a1515,
					   0x0c080404,0x5295c7c7,0x65462323,0x5e9dc3c3,0x28301818,0xa1379696,0x0f0a0505,0xb52f9a9a,
					   0x090e0707,0x36241212,0x9b1b8080,0x3ddfe2e2,0x26cdebeb,0x694e2727,0xcd7fb2b2,0x9fea7575,
					   0x1b120909,0x9e1d8383,0x74582c2c,0x2e341a1a,0x2d361b1b,0xb2dc6e6e,0xeeb45a5a,0xfb5ba0a0,
					   0xf6a45252,0x4d763b3b,0x61b7d6d6,0xce7db3b3,0x7b522929,0x3edde3e3,0x715e2f2f,0x97138484,
					   0xf5a65353,0x68b9d1d1,0x00000000,0x2cc1eded,0x60402020,0x1fe3fcfc,0xc879b1b1,0xedb65b5b,
					   0xbed46a6a,0x468dcbcb,0xd967bebe,0x4b723939,0xde944a4a,0xd4984c4c,0xe8b05858,0x4a85cfcf,
					   0x6bbbd0d0,0x2ac5efef,0xe54faaaa,0x16edfbfb,0xc5864343,0xd79a4d4d,0x55663333,0x94118585,
					   0xcf8a4545,0x10e9f9f9,0x06040202,0x81fe7f7f,0xf0a05050,0x44783c3c,0xba259f9f,0xe34ba8a8,
					   0xf3a25151,0xfe5da3a3,0xc0804040,0x8a058f8f,0xad3f9292,0xbc219d9d,0x48703838,0x04f1f5f5,
					   0xdf63bcbc,0xc177b6b6,0x75afdada,0x63422121,0x30201010,0x1ae5ffff,0x0efdf3f3,0x6dbfd2d2,
					   0x4c81cdcd,0x14180c0c,0x35261313,0x2fc3ecec,0xe1be5f5f,0xa2359797,0xcc884444,0x392e1717,
					   0x5793c4c4,0xf255a7a7,0x82fc7e7e,0x477a3d3d,0xacc86464,0xe7ba5d5d,0x2b321919,0x95e67373,
					   0xa0c06060,0x98198181,0xd19e4f4f,0x7fa3dcdc,0x66442222,0x7e542a2a,0xab3b9090,0x830b8888,
					   0xca8c4646,0x29c7eeee,0xd36bb8b8,0x3c281414,0x79a7dede,0xe2bc5e5e,0x1d160b0b,0x76addbdb,
					   0x3bdbe0e0,0x56643232,0x4e743a3a,0x1e140a0a,0xdb924949,0x0a0c0606,0x6c482424,0xe4b85c5c,
					   0x5d9fc2c2,0x6ebdd3d3,0xef43acac,0xa6c46262,0xa8399191,0xa4319595,0x37d3e4e4,0x8bf27979,
					   0x32d5e7e7,0x438bc8c8,0x596e3737,0xb7da6d6d,0x8c018d8d,0x64b1d5d5,0xd29c4e4e,0xe049a9a9,
					   0xb4d86c6c,0xfaac5656,0x07f3f4f4,0x25cfeaea,0xafca6565,0x8ef47a7a,0xe947aeae,0x18100808,
					   0xd56fbaba,0x88f07878,0x6f4a2525,0x725c2e2e,0x24381c1c,0xf157a6a6,0xc773b4b4,0x5197c6c6,
					   0x23cbe8e8,0x7ca1dddd,0x9ce87474,0x213e1f1f,0xdd964b4b,0xdc61bdbd,0x860d8b8b,0x850f8a8a,
					   0x90e07070,0x427c3e3e,0xc471b5b5,0xaacc6666,0xd8904848,0x05060303,0x01f7f6f6,0x121c0e0e,
					   0xa3c26161,0x5f6a3535,0xf9ae5757,0xd069b9b9,0x91178686,0x5899c1c1,0x273a1d1d,0xb9279e9e,
					   0x38d9e1e1,0x13ebf8f8,0xb32b9898,0x33221111,0xbbd26969,0x70a9d9d9,0x89078e8e,0xa7339494,
					   0xb62d9b9b,0x223c1e1e,0x92158787,0x20c9e9e9,0x4987cece,0xffaa5555,0x78502828,0x7aa5dfdf,
					   0x8f038c8c,0xf859a1a1,0x80098989,0x171a0d0d,0xda65bfbf,0x31d7e6e6,0xc6844242,0xb8d06868,
					   0xc3824141,0xb0299999,0x775a2d2d,0x111e0f0f,0xcb7bb0b0,0xfca85454,0xd66dbbbb,0x3a2c1616};

unsigned int SHAMATA_Slh[256]={0x63a5c663,0x7c84f87c,0x7799ee77,0x7b8df67b,0xf20dfff2,0x6bbdd66b,0x6fb1de6f,0xc55491c5,
                       0x30506030,0x01030201,0x67a9ce67,0x2b7d562b,0xfe19e7fe,0xd762b5d7,0xabe64dab,0x769aec76,
					   0xca458fca,0x829d1f82,0xc94089c9,0x7d87fa7d,0xfa15effa,0x59ebb259,0x47c98e47,0xf00bfbf0,
					   0xadec41ad,0xd467b3d4,0xa2fd5fa2,0xafea45af,0x9cbf239c,0xa4f753a4,0x7296e472,0xc05b9bc0,
					   0xb7c275b7,0xfd1ce1fd,0x93ae3d93,0x266a4c26,0x365a6c36,0x3f417e3f,0xf702f5f7,0xcc4f83cc,
					   0x345c6834,0xa5f451a5,0xe534d1e5,0xf108f9f1,0x7193e271,0xd873abd8,0x31536231,0x153f2a15,
					   0x040c0804,0xc75295c7,0x23654623,0xc35e9dc3,0x18283018,0x96a13796,0x050f0a05,0x9ab52f9a,
					   0x07090e07,0x12362412,0x809b1b80,0xe23ddfe2,0xeb26cdeb,0x27694e27,0xb2cd7fb2,0x759fea75,
					   0x091b1209,0x839e1d83,0x2c74582c,0x1a2e341a,0x1b2d361b,0x6eb2dc6e,0x5aeeb45a,0xa0fb5ba0,
					   0x52f6a452,0x3b4d763b,0xd661b7d6,0xb3ce7db3,0x297b5229,0xe33edde3,0x2f715e2f,0x84971384,
					   0x53f5a653,0xd168b9d1,0x00000000,0xed2cc1ed,0x20604020,0xfc1fe3fc,0xb1c879b1,0x5bedb65b,
					   0x6abed46a,0xcb468dcb,0xbed967be,0x394b7239,0x4ade944a,0x4cd4984c,0x58e8b058,0xcf4a85cf,
					   0xd06bbbd0,0xef2ac5ef,0xaae54faa,0xfb16edfb,0x43c58643,0x4dd79a4d,0x33556633,0x85941185,
					   0x45cf8a45,0xf910e9f9,0x02060402,0x7f81fe7f,0x50f0a050,0x3c44783c,0x9fba259f,0xa8e34ba8,
					   0x51f3a251,0xa3fe5da3,0x40c08040,0x8f8a058f,0x92ad3f92,0x9dbc219d,0x38487038,0xf504f1f5,
					   0xbcdf63bc,0xb6c177b6,0xda75afda,0x21634221,0x10302010,0xff1ae5ff,0xf30efdf3,0xd26dbfd2,
					   0xcd4c81cd,0x0c14180c,0x13352613,0xec2fc3ec,0x5fe1be5f,0x97a23597,0x44cc8844,0x17392e17,
					   0xc45793c4,0xa7f255a7,0x7e82fc7e,0x3d477a3d,0x64acc864,0x5de7ba5d,0x192b3219,0x7395e673,
					   0x60a0c060,0x81981981,0x4fd19e4f,0xdc7fa3dc,0x22664422,0x2a7e542a,0x90ab3b90,0x88830b88,
					   0x46ca8c46,0xee29c7ee,0xb8d36bb8,0x143c2814,0xde79a7de,0x5ee2bc5e,0x0b1d160b,0xdb76addb,
					   0xe03bdbe0,0x32566432,0x3a4e743a,0x0a1e140a,0x49db9249,0x060a0c06,0x246c4824,0x5ce4b85c,
					   0xc25d9fc2,0xd36ebdd3,0xacef43ac,0x62a6c462,0x91a83991,0x95a43195,0xe437d3e4,0x798bf279,
					   0xe732d5e7,0xc8438bc8,0x37596e37,0x6db7da6d,0x8d8c018d,0xd564b1d5,0x4ed29c4e,0xa9e049a9,
					   0x6cb4d86c,0x56faac56,0xf407f3f4,0xea25cfea,0x65afca65,0x7a8ef47a,0xaee947ae,0x08181008,
					   0xbad56fba,0x7888f078,0x256f4a25,0x2e725c2e,0x1c24381c,0xa6f157a6,0xb4c773b4,0xc65197c6,
					   0xe823cbe8,0xdd7ca1dd,0x749ce874,0x1f213e1f,0x4bdd964b,0xbddc61bd,0x8b860d8b,0x8a850f8a,
					   0x7090e070,0x3e427c3e,0xb5c471b5,0x66aacc66,0x48d89048,0x03050603,0xf601f7f6,0x0e121c0e,
					   0x61a3c261,0x355f6a35,0x57f9ae57,0xb9d069b9,0x86911786,0xc15899c1,0x1d273a1d,0x9eb9279e,
					   0xe138d9e1,0xf813ebf8,0x98b32b98,0x11332211,0x69bbd269,0xd970a9d9,0x8e89078e,0x94a73394,
					   0x9bb62d9b,0x1e223c1e,0x87921587,0xe920c9e9,0xce4987ce,0x55ffaa55,0x28785028,0xdf7aa5df,
					   0x8c8f038c,0xa1f859a1,0x89800989,0x0d171a0d,0xbfda65bf,0xe631d7e6,0x42c68442,0x68b8d068,
					   0x41c38241,0x99b02999,0x2d775a2d,0x0f111e0f,0xb0cb7bb0,0x54fca854,0xbbd66dbb,0x163a2c16};

unsigned int SHAMATA_Sll[256]={0x6363a5c6,0x7c7c84f8,0x777799ee,0x7b7b8df6,0xf2f20dff,0x6b6bbdd6,0x6f6fb1de,0xc5c55491,
                       0x30305060,0x01010302,0x6767a9ce,0x2b2b7d56,0xfefe19e7,0xd7d762b5,0xababe64d,0x76769aec,
					   0xcaca458f,0x82829d1f,0xc9c94089,0x7d7d87fa,0xfafa15ef,0x5959ebb2,0x4747c98e,0xf0f00bfb,
					   0xadadec41,0xd4d467b3,0xa2a2fd5f,0xafafea45,0x9c9cbf23,0xa4a4f753,0x727296e4,0xc0c05b9b,
					   0xb7b7c275,0xfdfd1ce1,0x9393ae3d,0x26266a4c,0x36365a6c,0x3f3f417e,0xf7f702f5,0xcccc4f83,
					   0x34345c68,0xa5a5f451,0xe5e534d1,0xf1f108f9,0x717193e2,0xd8d873ab,0x31315362,0x15153f2a,
					   0x04040c08,0xc7c75295,0x23236546,0xc3c35e9d,0x18182830,0x9696a137,0x05050f0a,0x9a9ab52f,
					   0x0707090e,0x12123624,0x80809b1b,0xe2e23ddf,0xebeb26cd,0x2727694e,0xb2b2cd7f,0x75759fea,
					   0x09091b12,0x83839e1d,0x2c2c7458,0x1a1a2e34,0x1b1b2d36,0x6e6eb2dc,0x5a5aeeb4,0xa0a0fb5b,
					   0x5252f6a4,0x3b3b4d76,0xd6d661b7,0xb3b3ce7d,0x29297b52,0xe3e33edd,0x2f2f715e,0x84849713,
					   0x5353f5a6,0xd1d168b9,0x00000000,0xeded2cc1,0x20206040,0xfcfc1fe3,0xb1b1c879,0x5b5bedb6,
					   0x6a6abed4,0xcbcb468d,0xbebed967,0x39394b72,0x4a4ade94,0x4c4cd498,0x5858e8b0,0xcfcf4a85,
					   0xd0d06bbb,0xefef2ac5,0xaaaae54f,0xfbfb16ed,0x4343c586,0x4d4dd79a,0x33335566,0x85859411,
					   0x4545cf8a,0xf9f910e9,0x02020604,0x7f7f81fe,0x5050f0a0,0x3c3c4478,0x9f9fba25,0xa8a8e34b,
					   0x5151f3a2,0xa3a3fe5d,0x4040c080,0x8f8f8a05,0x9292ad3f,0x9d9dbc21,0x38384870,0xf5f504f1,
					   0xbcbcdf63,0xb6b6c177,0xdada75af,0x21216342,0x10103020,0xffff1ae5,0xf3f30efd,0xd2d26dbf,
					   0xcdcd4c81,0x0c0c1418,0x13133526,0xecec2fc3,0x5f5fe1be,0x9797a235,0x4444cc88,0x1717392e,
					   0xc4c45793,0xa7a7f255,0x7e7e82fc,0x3d3d477a,0x6464acc8,0x5d5de7ba,0x19192b32,0x737395e6,
					   0x6060a0c0,0x81819819,0x4f4fd19e,0xdcdc7fa3,0x22226644,0x2a2a7e54,0x9090ab3b,0x8888830b,
					   0x4646ca8c,0xeeee29c7,0xb8b8d36b,0x14143c28,0xdede79a7,0x5e5ee2bc,0x0b0b1d16,0xdbdb76ad,
					   0xe0e03bdb,0x32325664,0x3a3a4e74,0x0a0a1e14,0x4949db92,0x06060a0c,0x24246c48,0x5c5ce4b8,
					   0xc2c25d9f,0xd3d36ebd,0xacacef43,0x6262a6c4,0x9191a839,0x9595a431,0xe4e437d3,0x79798bf2,
					   0xe7e732d5,0xc8c8438b,0x3737596e,0x6d6db7da,0x8d8d8c01,0xd5d564b1,0x4e4ed29c,0xa9a9e049,
					   0x6c6cb4d8,0x5656faac,0xf4f407f3,0xeaea25cf,0x6565afca,0x7a7a8ef4,0xaeaee947,0x08081810,
					   0xbabad56f,0x787888f0,0x25256f4a,0x2e2e725c,0x1c1c2438,0xa6a6f157,0xb4b4c773,0xc6c65197,
					   0xe8e823cb,0xdddd7ca1,0x74749ce8,0x1f1f213e,0x4b4bdd96,0xbdbddc61,0x8b8b860d,0x8a8a850f,
					   0x707090e0,0x3e3e427c,0xb5b5c471,0x6666aacc,0x4848d890,0x03030506,0xf6f601f7,0x0e0e121c,
					   0x6161a3c2,0x35355f6a,0x5757f9ae,0xb9b9d069,0x86869117,0xc1c15899,0x1d1d273a,0x9e9eb927,
					   0xe1e138d9,0xf8f813eb,0x9898b32b,0x11113322,0x6969bbd2,0xd9d970a9,0x8e8e8907,0x9494a733,
					   0x9b9bb62d,0x1e1e223c,0x87879215,0xe9e920c9,0xcece4987,0x5555ffaa,0x28287850,0xdfdf7aa5,
					   0x8c8c8f03,0xa1a1f859,0x89898009,0x0d0d171a,0xbfbfda65,0xe6e631d7,0x4242c684,0x6868b8d0,
					   0x4141c382,0x9999b029,0x2d2d775a,0x0f0f111e,0xb0b0cb7b,0x5454fca8,0xbbbbd66d,0x16163a2c};

int Shamata::Hash (int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
	//hashState *state = (hashState *)malloc(sizeof(hashState));

	if (hashbitlen != 224 && hashbitlen != 256 && hashbitlen != 384 && hashbitlen != 512){
		printf("Bad Hash Bit Length Error!\n");
		return BAD_HASHBITLEN;
	}

	Shamata::Init(hashbitlen);
	Shamata::Update(data, databitlen);
	Shamata::Final(hashval);

	return SUCCESS;
}

int Shamata::Init(int hashbitlen)
{
	switch (hashbitlen){
		case 224 :  return Init224(&shamataState,hashbitlen);
		case 256 :	return Init256(&shamataState,hashbitlen);
		case 384 :	return Init384(&shamataState,hashbitlen);
		case 512 :  return Init512(&shamataState,hashbitlen);
	}
}

int Shamata::Update(const BitSequence *data, DataLength databitlen)
{
	if(shamataState.hashbitlen==224 || shamataState.hashbitlen==256)
		return Update224_256(&shamataState,data,databitlen);
	else
		return Update384_512(&shamataState,data,databitlen);
}


int Shamata::Final(BitSequence *hashval)
{
	if(shamataState.hashbitlen==224 || shamataState.hashbitlen==256)
		return Final224_256(&shamataState,hashval);
	else
		return Final384_512(&shamataState,hashval);
}

int Shamata::Init224(hashState *state, int hashbitlen)
{
	unsigned int i;
    
	state->hashbitlen          = hashbitlen; 	
	state->databitlen          = 0;	
	state->remainingdatabitlen = 0;    
	
	for(i=0;i<8;i++)
		state->remainingdata[i] = 0;
	
	state->B[0][0] = 0x7de3745c;  state->B[0][1] = 0x828fde52;  state->B[0][2] = 0x7723f777;  state->B[0][3] = 0xa25c964d;
	state->B[1][0] = 0x1cab2590;  state->B[1][1] = 0xba2e18d5;  state->B[1][2] = 0x93f1c47d;  state->B[1][3] = 0xb02b3c6a;
	state->B[2][0] = 0x61870632;  state->B[2][1] = 0x7128e776;  state->B[2][2] = 0x3a7202f8;  state->B[2][3] = 0x0fe41b8e;
	state->B[3][0] = 0xd5373fbc;  state->B[3][1] = 0xdff6c880;  state->B[3][2] = 0xb9b1fe4b;  state->B[3][3] = 0x44baf80c;
	state->K[0][0] = 0xffe8161e;  state->K[0][1] = 0x1d027b15;  state->K[0][2] = 0x4b1e08ce;  state->K[0][3] = 0xaafb46e5;
	state->K[1][0] = 0x9ca445fb;  state->K[1][1] = 0x5f5423cd;  state->K[1][2] = 0x373f0dc6;  state->K[1][3] = 0x543e299f;
	state->K[2][0] = 0x69d5fe36;  state->K[2][1] = 0x7e2546e4;  state->K[2][2] = 0x164ea4fa;  state->K[2][3] = 0xef7e10bc;
	state->K[3][0] = 0xd3fe07c9;  state->K[3][1] = 0x7f107d7c;  state->K[3][2] = 0x02cd637d;  state->K[3][3] = 0xf7f0bd90;
	state->K[4][0] = 0x9b99c05f;  state->K[4][1] = 0x43c31eac;  state->K[4][2] = 0xf53b9b6c;  state->K[4][3] = 0xd3571cc3;
	state->K[5][0] = 0xb0255583;  state->K[5][1] = 0x6983c94a;  state->K[5][2] = 0x1eed4efd;  state->K[5][3] = 0x03f344f5;
	state->K[6][0] = 0xf3dc1186;  state->K[6][1] = 0x6d4ff29d;  state->K[6][2] = 0xc92c8aa2;  state->K[6][3] = 0x3c4dba34;
	state->K[7][0] = 0x297d8238;  state->K[7][1] = 0x92a3e322;  state->K[7][2] = 0x481bf50f;  state->K[7][3] = 0x0376308c;
	state->K[8][0] = 0x9a911ed4;  state->K[8][1] = 0x09f76dfa;  state->K[8][2] = 0x97b2586b;  state->K[8][3] = 0xdd4e334e;
	state->K[9][0] = 0xb8200b1d;  state->K[9][1] = 0xd57aab71;  state->K[9][2] = 0xa4a1af3e;  state->K[9][3] = 0x10675c73;
	state->K[10][0]= 0xdb607493;  state->K[10][1]= 0x84e15ff6;  state->K[10][2]= 0x762d6903;  state->K[10][3]= 0x849729cb;
	state->K[11][0]= 0xdc3cd1f1;  state->K[11][1]= 0xb16bfed8;  state->K[11][2]= 0x2a4738d4;  state->K[11][3]= 0x11f1c98b;

	return SUCCESS;
}

int Shamata::Init256(hashState *state, int hashbitlen)
{
	unsigned int i;
    
	state->hashbitlen          = hashbitlen; 	
	state->databitlen          = 0;	
	state->remainingdatabitlen = 0;    
	
	for(i=0;i<8;i++)
		state->remainingdata[i] = 0;
	
	state->B[0][0] =0xd5c22302;		state->B[0][1] =0x027a5584;		state->B[0][2] =0xace91bf2;		state->B[0][3] =0x28c6a604;
	state->B[1][0] =0xc23373e3;		state->B[1][1] =0x679c6f7f;		state->B[1][2] =0xbff15374;		state->B[1][3] =0x09c35325;
	state->B[2][0] =0xfb476749;		state->B[2][1] =0xec3fe4f7;		state->B[2][2] =0x0a3e6aee;		state->B[2][3] =0x4088cd1d;
	state->B[3][0] =0x2f2fb8f2;		state->B[3][1] =0x14410207;		state->B[3][2] =0xa736090a;		state->B[3][3] =0x78f9cbab;
	state->K[0][0] =0x26c07b25;		state->K[0][1] =0xe6e6c2e7;		state->K[0][2] =0xd8cd2a86;		state->K[0][3] =0xef48bc5c;
	state->K[1][0] =0x4521b2d8;		state->K[1][1] =0x7e84bc15;		state->K[1][2] =0x8b77a3d2;		state->K[1][3] =0x99874bf8;
	state->K[2][0] =0x6d4b221b;		state->K[2][1] =0x56ba7c53;		state->K[2][2] =0x7e080610;		state->K[2][3] =0x1b070843;
	state->K[3][0] =0xe749ecd9;		state->K[3][1] =0x64da6c25;		state->K[3][2] =0x18cae13e;		state->K[3][3] =0x58304dcc;
	state->K[4][0] =0x0554faf7;		state->K[4][1] =0x81823b0c;		state->K[4][2] =0x5ba4aca0;		state->K[4][3] =0xdbe5a2ce;
	state->K[5][0] =0x72e62021;		state->K[5][1] =0xc334436e;		state->K[5][2] =0xb7cada8c;		state->K[5][3] =0xf3aab56e;
	state->K[6][0] =0x820555b2;		state->K[6][1] =0x17aa959c;		state->K[6][2] =0xd948d6a2;		state->K[6][3] =0xb07693bc;
	state->K[7][0] =0xb521a748;		state->K[7][1] =0xd5de56f4;		state->K[7][2] =0x53855ee2;		state->K[7][3] =0xcb6dcc32;
	state->K[8][0] =0xc4476040;		state->K[8][1] =0xb6115d9d;		state->K[8][2] =0x5070ba05;		state->K[8][3] =0x86106f20;
	state->K[9][0] =0x24625414;		state->K[9][1] =0x3368849e;		state->K[9][2] =0x3bc5f994;		state->K[9][3] =0xcfc19ddb;
	state->K[10][0]=0x4bf828e4;		state->K[10][1]=0xec2041f4;		state->K[10][2]=0x4754bc1a;		state->K[10][3]=0x9aaeacfb;
	state->K[11][0]=0x51e4269d;		state->K[11][1]=0x40beb903;		state->K[11][2]=0x372dbc44;		state->K[11][3]=0x776648ad;

	return SUCCESS;
}

int Shamata::Init384(hashState *state, int hashbitlen)
{
	unsigned int i;
    
	state->hashbitlen          = hashbitlen; 	
	state->databitlen          = 0;	
	state->remainingdatabitlen = 0;    
	
	for(i=0;i<8;i++)
		state->remainingdata[i] = 0;

	state->B[0][0] = 0x9b9d14ee; 	state->B[0][1] = 0x916bd8af; 	state->B[0][2] = 0x73e6862d; 	state->B[0][3] = 0x25e66be8;
	state->B[1][0] = 0x2f4d32bc; 	state->B[1][1] = 0x87ba416e; 	state->B[1][2] = 0xa26dda6e; 	state->B[1][3] = 0x1a5227b4;
	state->B[2][0] = 0x2504e360; 	state->B[2][1] = 0x8d355f33; 	state->B[2][2] = 0xc0bb6655; 	state->B[2][3] = 0xf91930bb;
	state->B[3][0] = 0x5dd70d70; 	state->B[3][1] = 0xe7102fbf; 	state->B[3][2] = 0xe8e46aeb; 	state->B[3][3] = 0x6ea675cb;
	state->K[0][0] = 0x725570e8; 	state->K[0][1] = 0xefe5baa0; 	state->K[0][2] = 0x79f89ed5; 	state->K[0][3] = 0x1f5ad312;
	state->K[1][0] = 0x68867fde; 	state->K[1][1] = 0xb1f89c16; 	state->K[1][2] = 0x9bf14d5a; 	state->K[1][3] = 0xd0e0543f;
	state->K[2][0] = 0x1f1629c9; 	state->K[2][1] = 0xd90b662b; 	state->K[2][2] = 0xcce98616; 	state->K[2][3] = 0x01026c2e;
	state->K[3][0] = 0xa1321835; 	state->K[3][1] = 0xbf39808f; 	state->K[3][2] = 0x3fe5fa42; 	state->K[3][3] = 0x23a338c7;
	state->K[4][0] = 0x188c1d97; 	state->K[4][1] = 0x6f8ce863; 	state->K[4][2] = 0x3660d9e4; 	state->K[4][3] = 0x4951c6e7;
	state->K[5][0] = 0x9cbc8b0c; 	state->K[5][1] = 0xd872929c; 	state->K[5][2] = 0x5c24c0da; 	state->K[5][3] = 0xe34c6da6;
	state->K[6][0] = 0xd2ac1371; 	state->K[6][1] = 0xcd928d97; 	state->K[6][2] = 0xcfaef6ae; 	state->K[6][3] = 0x6f6c268a;
	state->K[7][0] = 0xb5643fde; 	state->K[7][1] = 0x154399dd; 	state->K[7][2] = 0xd8188adc; 	state->K[7][3] = 0xc4a2718d;
	state->K[8][0] = 0xf4017a09; 	state->K[8][1] = 0x53af8e5f; 	state->K[8][2] = 0x53803c0a; 	state->K[8][3] = 0xdd09039c;
	state->K[9][0] = 0xa5d3f617; 	state->K[9][1] = 0xf882cb2a; 	state->K[9][2] = 0x3372bf04; 	state->K[9][3] = 0xff5a3596;
	state->K[10][0]= 0x7258b952; 	state->K[10][1]= 0xcbca7681; 	state->K[10][2]= 0x30b3eded; 	state->K[10][3]= 0x65888c2a;
	state->K[11][0]= 0x17f7e3f3; 	state->K[11][1]= 0xa2da995e; 	state->K[11][2]= 0x7e649930; 	state->K[11][3]= 0xaf17f943;

	return SUCCESS;
}

int Shamata::Init512(hashState *state, int hashbitlen)
{
	unsigned int i;
    
	state->hashbitlen          = hashbitlen; 	
	state->databitlen          = 0;	
	state->remainingdatabitlen = 0;    

	for(i=0;i<8;i++)
		state->remainingdata[i] = 0;
	
	state->B[0][0] = 0xdacb8994;  state->B[0][1] = 0x4cc9075c;  state->B[0][2] = 0x563281d1;  state->B[0][3] = 0xec1b3cd7;
	state->B[1][0] = 0xa43ebd40;  state->B[1][1] = 0xa8ce36b0;  state->B[1][2] = 0xc5523168;  state->B[1][3] = 0xa0244cac;
	state->B[2][0] = 0x2271fa1a;  state->B[2][1] = 0x6ef492ce;  state->B[2][2] = 0x2dd77c6f;  state->B[2][3] = 0xb66ff8ed;
	state->B[3][0] = 0x8ccf92bf;  state->B[3][1] = 0x55d5248f;  state->B[3][2] = 0x5097b0ec;  state->B[3][3] = 0x1faa2654;
	state->K[0][0] = 0x4f38f232;  state->K[0][1] = 0xaf1786e6;  state->K[0][2] = 0x35ad689f;  state->K[0][3] = 0x9b896774;
	state->K[1][0] = 0xe9b93253;  state->K[1][1] = 0xf8d9a03b;  state->K[1][2] = 0xf93e4e60;  state->K[1][3] = 0xb9a6fea2;
	state->K[2][0] = 0x6cc16a67;  state->K[2][1] = 0xc92ce0f3;  state->K[2][2] = 0xbbe83c26;  state->K[2][3] = 0x884064c4;
	state->K[3][0] = 0x6e40f5e3;  state->K[3][1] = 0xe8878669;  state->K[3][2] = 0x19af74c8;  state->K[3][3] = 0x03774940;
	state->K[4][0] = 0xe266d439;  state->K[4][1] = 0x0a77a018;  state->K[4][2] = 0x64b00dc1;  state->K[4][3] = 0xcd40033a;
	state->K[5][0] = 0x08223e3e;  state->K[5][1] = 0xfb96af1b;  state->K[5][2] = 0xea32c232;  state->K[5][3] = 0x14e937db;
	state->K[6][0] = 0x849981d6;  state->K[6][1] = 0xf1824d33;  state->K[6][2] = 0xe80f693e;  state->K[6][3] = 0x3f6e1dd1;
	state->K[7][0] = 0x54d5cf30;  state->K[7][1] = 0xeb7ae9b0;  state->K[7][2] = 0x7744f2f2;  state->K[7][3] = 0x9fbef64b;
	state->K[8][0] = 0xd6da88fb;  state->K[8][1] = 0xc57dddd2;  state->K[8][2] = 0x7343bea8;  state->K[8][3] = 0xf0721d1e;
	state->K[9][0] = 0x0731f644;  state->K[9][1] = 0x14841398;  state->K[9][2] = 0x9bcb015e;  state->K[9][3] = 0x618422c2;
	state->K[10][0]= 0x7ef65555;  state->K[10][1]= 0x606d9fe2;  state->K[10][2]= 0x781e6959;  state->K[10][3]= 0xffa4d2fe;
	state->K[11][0]= 0x02ed18f5;  state->K[11][1]= 0x817073e3;  state->K[11][2]= 0xf39be5e0;  state->K[11][3]= 0x6865e21c;

	return SUCCESS;
}

int Shamata::Update224_256 (hashState *state, const BitSequence *data, DataLength databitlen)
{
	DataLength messageblockcount = databitlen/128;  
	DataLength processedmessageblockcount = state->databitlen/128;
	unsigned long long i; 
	unsigned int j, tmp10, tmp11, tmp12, tmp13, Q0, Q1, Q2, Q3, P0, P1, P2, P3,
		               B00 = state->B[0][0],	B01 = state->B[0][1],	B02 = state->B[0][2],	B03 = state->B[0][3], 
					   B10 = state->B[1][0],	B11 = state->B[1][1],	B12 = state->B[1][2],	B13 = state->B[1][3], 
					   B20 = state->B[2][0],	B21 = state->B[2][1],	B22 = state->B[2][2],	B23 = state->B[2][3], 
					   B30 = state->B[3][0],	B31 = state->B[3][1],	B32 = state->B[3][2],	B33 = state->B[3][3], 
				       K00 = state->K[0][0],	K01 = state->K[0][1],	K02 = state->K[0][2],	K03 = state->K[0][3], 
					   K10 = state->K[1][0],	K11 = state->K[1][1],	K12 = state->K[1][2],	K13 = state->K[1][3], 
					   K20 = state->K[2][0],	K21 = state->K[2][1],	K22 = state->K[2][2],	K23 = state->K[2][3], 
					   K30 = state->K[3][0],	K31 = state->K[3][1],	K32 = state->K[3][2],	K33 = state->K[3][3], 
					   K40 = state->K[4][0],	K41 = state->K[4][1],	K42 = state->K[4][2],	K43 = state->K[4][3], 
					   K50 = state->K[5][0],	K51 = state->K[5][1],	K52 = state->K[5][2],	K53 = state->K[5][3], 
					   K60 = state->K[6][0],	K61 = state->K[6][1],	K62 = state->K[6][2],	K63 = state->K[6][3], 
					   K70 = state->K[7][0],	K71 = state->K[7][1],	K72 = state->K[7][2],	K73 = state->K[7][3],
					   K80 = state->K[8][0],	K81 = state->K[8][1],	K82 = state->K[8][2],	K83 = state->K[8][3], 
					   K90 = state->K[9][0],	K91 = state->K[9][1],	K92 = state->K[9][2],	K93 = state->K[9][3], 
					   K100 = state->K[10][0],	K101 = state->K[10][1], K102 = state->K[10][2], K103 = state->K[10][3], 
					   K110 = state->K[11][0],	K111 = state->K[11][1], K112 = state->K[11][2], K113 = state->K[11][3];
	
	// Check : Update function must be called with a databitlen which is a multiple of 128 if it is not a final call
	if (state->remainingdatabitlen != 0 && databitlen%128 != 0){
        printf("\n\n !!! Update function must be called with a databitlen which is a multiple of 128 if it is not a final call... \n");	
		return FAIL;
	}
	
	// Update state remaining data bitlen
	state->databitlen += databitlen;
	state->remainingdatabitlen = databitlen%128;

	for(j=0;j<(state->remainingdatabitlen/8 + (state->remainingdatabitlen%8 == 0 ? 0:1));j++)
		state->remainingdata[j/4] |= data[16*messageblockcount+j] << ((3-j%4)*8);
	
	i = 1;
	while(1){
		//LOAD #6*t+1 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^((i+processedmessageblockcount)>>32);   B23 ^=P3^(i+processedmessageblockcount);  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^((i+processedmessageblockcount)>>32);   B33 ^=Q3^(i+processedmessageblockcount);  
		K30 ^=P2;	K31 ^=P3;	K32 ^=Q0;			K33 ^=Q1;
		K50 ^=Q0;	K51 ^=Q1;	K52 ^=Q2;			K53 ^=Q3;
		K70 ^=P0;	K71 ^=P1;	K72 ^=P2;			K73 ^=P3;
		K110^=Q2;	K111^=Q3;	K112^=P0;			K113^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        // Calculate Feed K
		tmp10^=B00;	tmp11^=B01;	tmp12^=B02;	tmp13^=B03;
        // Calculate Feed B and write it to the register
		B00=tmp10^K90^K00;	B01=tmp11^K91^K01;	B02=tmp12^K92^K02;	B03=tmp13^K93^K03;
		// Write Feed K to the register
		K00=tmp10;	K01=tmp11;	K02=tmp12;	K03=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        // Calculate Feed K
		tmp10^=B10;	tmp11^=B11;	tmp12^=B12;	tmp13^=B13;
        // Calculate Feed B and write it to the register
		B10=tmp10^K100^K10;	B11=tmp11^K101^K11;	B12=tmp12^K102^K12;	B13=tmp13^K103^K13;
        // Write Feed K to the register
		K10=tmp10;	K11=tmp11;	K12=tmp12;	K13=tmp13;

		//LOAD #6*t+2 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^((i+processedmessageblockcount)>>32);   B03 ^=P3^(i+processedmessageblockcount);  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^((i+processedmessageblockcount)>>32);   B13 ^=Q3^(i+processedmessageblockcount);  
		K50 ^=P2;	K51 ^=P3;	K52 ^=Q0;			K53 ^=Q1;
		K70 ^=Q0;	K71 ^=Q1;	K72 ^=Q2;			K73 ^=Q3;
		K90 ^=P0;	K91 ^=P1;	K92 ^=P2;			K93 ^=P3;
		K10 ^=Q2;	K11 ^=Q3;	K12 ^=P0;			K13 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        // Calculate Feed K
		tmp10^=B20;	tmp11^=B21;	tmp12^=B22;	tmp13^=B23;
        // Calculate Feed B and write it to the register
		B20=tmp10^K110^K20;	B21=tmp11^K111^K21;	B22=tmp12^K112^K22;	B23=tmp13^K113^K23;
		// Write Feed K to the register
		K20=tmp10;	K21=tmp11;	K22=tmp12;	K23=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        // Calculate Feed K
		tmp10^=B30;	tmp11^=B31;	tmp12^=B32;	tmp13^=B33;
        // Calculate Feed B and write it to the register
		B30=tmp10^K00^K30;	B31=tmp11^K01^K31;	B32=tmp12^K02^K32;	B33=tmp13^K03^K33;
        // Write Feed K to the register
		K30=tmp10;	K31=tmp11;	K32=tmp12;	K33=tmp13;

		//LOAD #6*t+3 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^((i+processedmessageblockcount)>>32);   B23 ^=P3^(i+processedmessageblockcount);  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^((i+processedmessageblockcount)>>32);   B33 ^=Q3^(i+processedmessageblockcount);  
		K70 ^=P2;	K71 ^=P3;	K72 ^=Q0;			K73 ^=Q1;
		K90 ^=Q0;	K91 ^=Q1;	K92 ^=Q2;			K93 ^=Q3;
		K110^=P0;	K111^=P1;	K112^=P2;			K113^=P3;
		K30 ^=Q2;	K31 ^=Q3;	K32 ^=P0;			K33 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        // Calculate Feed K
		tmp10^=B00;	tmp11^=B01;	tmp12^=B02;	tmp13^=B03;
        // Calculate Feed B and write it to the register
		B00=tmp10^K10^K40;	B01=tmp11^K11^K41;	B02=tmp12^K12^K42;	B03=tmp13^K13^K43;
		// Write Feed K to the register
		K40=tmp10;	K41=tmp11;	K42=tmp12;	K43=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        // Calculate Feed K
		tmp10^=B10;	tmp11^=B11;	tmp12^=B12;	tmp13^=B13;
        // Calculate Feed B and write it to the register
		B10=tmp10^K20^K50;	B11=tmp11^K21^K51;	B12=tmp12^K22^K52;	B13=tmp13^K23^K53;
        // Write Feed K to the register
		K50=tmp10;	K51=tmp11;	K52=tmp12;	K53=tmp13;

		//LOAD #6*t+4 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^((i+processedmessageblockcount)>>32);   B03 ^=P3^(i+processedmessageblockcount);  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^((i+processedmessageblockcount)>>32);   B13 ^=Q3^(i+processedmessageblockcount);  
		K90 ^=P2;	K91 ^=P3;	K92 ^=Q0;			K93 ^=Q1;
		K110^=Q0;	K111^=Q1;	K112^=Q2;			K113^=Q3;
		K10 ^=P0;	K11 ^=P1;	K12 ^=P2;			K13 ^=P3;
		K50 ^=Q2;	K51 ^=Q3;	K52 ^=P0;			K53 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        // Calculate Feed K
		tmp10^=B20;	tmp11^=B21;	tmp12^=B22;	tmp13^=B23;
        // Calculate Feed B and write it to the register
		B20=tmp10^K30^K60;	B21=tmp11^K31^K61;	B22=tmp12^K32^K62;	B23=tmp13^K33^K63;
		// Write Feed K to the register
		K60=tmp10;	K61=tmp11;	K62=tmp12;	K63=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        // Calculate Feed K
		tmp10^=B30;	tmp11^=B31;	tmp12^=B32;	tmp13^=B33;
        // Calculate Feed B and write it to the register
		B30=tmp10^K40^K70;	B31=tmp11^K41^K71;	B32=tmp12^K42^K72;	B33=tmp13^K43^K73;
        // Write Feed K to the register
		K70=tmp10;	K71=tmp11;	K72=tmp12;	K73=tmp13;

		//LOAD #6*t+5 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^((i+processedmessageblockcount)>>32);   B23 ^=P3^(i+processedmessageblockcount);  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^((i+processedmessageblockcount)>>32);   B33 ^=Q3^(i+processedmessageblockcount);  
		K110^=P2;	K111^=P3;	K112^=Q0;			K113^=Q1;
		K10 ^=Q0;	K11 ^=Q1;	K12 ^=Q2;			K13 ^=Q3;
		K30 ^=P0;	K31 ^=P1;	K32 ^=P2;			K33 ^=P3;
		K70 ^=Q2;	K71 ^=Q3;	K72 ^=P0;			K73 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        // Calculate Feed K
		tmp10^=B00;	tmp11^=B01;	tmp12^=B02;	tmp13^=B03;
        // Calculate Feed B and write it to the register
		B00=tmp10^K50^K80;	B01=tmp11^K51^K81;	B02=tmp12^K52^K82;	B03=tmp13^K53^K83;
		// Write Feed K to the register
		K80=tmp10;	K81=tmp11;	K82=tmp12;	K83=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        // Calculate Feed K
		tmp10^=B10;	tmp11^=B11;	tmp12^=B12;	tmp13^=B13;
        // Calculate Feed B and write it to the register
		B10=tmp10^K60^K90;	B11=tmp11^K61^K91;	B12=tmp12^K62^K92;	B13=tmp13^K63^K93;
        // Write Feed K to the register
		K90=tmp10;	K91=tmp11;	K92=tmp12;	K93=tmp13;

		//LOAD #6*t+6 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^((i+processedmessageblockcount)>>32);   B03 ^=P3^(i+processedmessageblockcount);  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^((i+processedmessageblockcount)>>32);   B13 ^=Q3^(i+processedmessageblockcount);  
		K10 ^=P2;	K11 ^=P3;	K12 ^=Q0;			K13 ^=Q1;
		K30 ^=Q0;	K31 ^=Q1;	K32 ^=Q2;			K33 ^=Q3;
		K50 ^=P0;	K51 ^=P1;	K52 ^=P2;			K53 ^=P3;
		K90 ^=Q2;	K91 ^=Q3;	K92 ^=P0;			K93 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        // Calculate Feed K
		tmp10^=B20;	tmp11^=B21;	tmp12^=B22;	tmp13^=B23;
        // Calculate Feed B and write it to the register
		B20=tmp10^K70^K100;	B21=tmp11^K71^K101;	B22=tmp12^K72^K102;	B23=tmp13^K73^K103;
		// Write Feed K to the register
		K100=tmp10;	K101=tmp11;	K102=tmp12;	K103=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        // Calculate Feed K
		tmp10^=B30;	tmp11^=B31;	tmp12^=B32;	tmp13^=B33;
        // Calculate Feed B and write it to the register
		B30=tmp10^K80^K110;	B31=tmp11^K81^K111;	B32=tmp12^K82^K112;	B33=tmp13^K83^K113;
        // Write Feed K to the register
		K110=tmp10;	K111=tmp11;	K112=tmp12;	K113=tmp13;
	}

	// Write calculate register values to the state register
	state->B[0][0] = B00;	state->B[0][1] = B01;	state->B[0][2] = B02;	state->B[0][3] = B03;	
	state->B[1][0] = B10;	state->B[1][1] = B11;	state->B[1][2] = B12;	state->B[1][3] = B13;	
	state->B[2][0] = B20;	state->B[2][1] = B21;	state->B[2][2] = B22;	state->B[2][3] = B23;	
	state->B[3][0] = B30;	state->B[3][1] = B31;	state->B[3][2] = B32;	state->B[3][3] = B33;	
	state->K[0][0] = K00;	state->K[0][1] = K01;	state->K[0][2] = K02;	state->K[0][3] = K03;	
	state->K[1][0] = K10;	state->K[1][1] = K11;	state->K[1][2] = K12;	state->K[1][3] = K13;	
	state->K[2][0] = K20;	state->K[2][1] = K21;	state->K[2][2] = K22;	state->K[2][3] = K23;	
	state->K[3][0] = K30;	state->K[3][1] = K31;	state->K[3][2] = K32;	state->K[3][3] = K33;	
	state->K[4][0] = K40;	state->K[4][1] = K41;	state->K[4][2] = K42;	state->K[4][3] = K43;	
	state->K[5][0] = K50;	state->K[5][1] = K51;	state->K[5][2] = K52;	state->K[5][3] = K53;	
	state->K[6][0] = K60;	state->K[6][1] = K61;	state->K[6][2] = K62;	state->K[6][3] = K63;	
	state->K[7][0] = K70;	state->K[7][1] = K71;	state->K[7][2] = K72;	state->K[7][3] = K73;	
	state->K[8][0] = K80;	state->K[8][1] = K81;	state->K[8][2] = K82;	state->K[8][3] = K83;	
	state->K[9][0] = K90;	state->K[9][1] = K91;	state->K[9][2] = K92;	state->K[9][3] = K93;	
	state->K[10][0]= K100;	state->K[10][1]= K101;	state->K[10][2] = K102;	state->K[10][3] = K103;	
	state->K[11][0]= K110;	state->K[11][1]= K111;	state->K[11][2] = K112;	state->K[11][3] = K113;	

	for(i=0;i<((messageblockcount%2)*2);i++){
		tmp10 = state->B[0][0];		tmp11 = state->B[0][1];		
		tmp12 = state->B[0][2];		tmp13 = state->B[0][3];
		for(j=0;j<3;j++){
			state->B[j][0] = state->B[j+1][0];		state->B[j][1] = state->B[j+1][1];		
			state->B[j][2] = state->B[j+1][2];		state->B[j][3] = state->B[j+1][3];
		}
		state->B[3][0] = tmp10;		state->B[3][1] = tmp11;		
		state->B[3][2] = tmp12;		state->B[3][3] = tmp13;
	}
	for(i=0;i<((messageblockcount%6)*2);i++){	
		tmp10 = state->K[0][0];		tmp11 = state->K[0][1];		
		tmp12 = state->K[0][2];		tmp13 = state->K[0][3];
		for(j=0;j<11;j++){
			state->K[j][0] = state->K[j+1][0];		state->K[j][1] = state->K[j+1][1];		
			state->K[j][2] = state->K[j+1][2];		state->K[j][3] = state->K[j+1][3];
		}
		state->K[11][0] = tmp10;		state->K[11][1] = tmp11;		
		state->K[11][2] = tmp12;		state->K[11][3] = tmp13;
	}

	return SUCCESS;
}

int Shamata::Update384_512 (hashState *state, const BitSequence *data, DataLength databitlen)
{
	DataLength messageblockcount = databitlen/128; 
	DataLength processedmessageblockcount = state->databitlen/128;
	unsigned long long i; 
	unsigned int j, tmp10, tmp11, tmp12, tmp13, tmp20, tmp21, tmp22, tmp23, Q0, Q1, Q2, Q3, P0, P1, P2, P3,
		               B00 = state->B[0][0],	B01 = state->B[0][1],	B02 = state->B[0][2],	B03 = state->B[0][3], 
					   B10 = state->B[1][0],	B11 = state->B[1][1],	B12 = state->B[1][2],	B13 = state->B[1][3], 
					   B20 = state->B[2][0],	B21 = state->B[2][1],	B22 = state->B[2][2],	B23 = state->B[2][3], 
					   B30 = state->B[3][0],	B31 = state->B[3][1],	B32 = state->B[3][2],	B33 = state->B[3][3], 
				       K00 = state->K[0][0],	K01 = state->K[0][1],	K02 = state->K[0][2],	K03 = state->K[0][3], 
					   K10 = state->K[1][0],	K11 = state->K[1][1],	K12 = state->K[1][2],	K13 = state->K[1][3], 
					   K20 = state->K[2][0],	K21 = state->K[2][1],	K22 = state->K[2][2],	K23 = state->K[2][3], 
					   K30 = state->K[3][0],	K31 = state->K[3][1],	K32 = state->K[3][2],	K33 = state->K[3][3], 
					   K40 = state->K[4][0],	K41 = state->K[4][1],	K42 = state->K[4][2],	K43 = state->K[4][3], 
					   K50 = state->K[5][0],	K51 = state->K[5][1],	K52 = state->K[5][2],	K53 = state->K[5][3], 
					   K60 = state->K[6][0],	K61 = state->K[6][1],	K62 = state->K[6][2],	K63 = state->K[6][3], 
					   K70 = state->K[7][0],	K71 = state->K[7][1],	K72 = state->K[7][2],	K73 = state->K[7][3],
					   K80 = state->K[8][0],	K81 = state->K[8][1],	K82 = state->K[8][2],	K83 = state->K[8][3], 
					   K90 = state->K[9][0],	K91 = state->K[9][1],	K92 = state->K[9][2],	K93 = state->K[9][3], 
					   K100 = state->K[10][0],	K101 = state->K[10][1], K102 = state->K[10][2], K103 = state->K[10][3], 
					   K110 = state->K[11][0],	K111 = state->K[11][1], K112 = state->K[11][2], K113 = state->K[11][3];
	
	// Check : Update function must be called with a databitlen which is a multiple of 128 if it is not a final call
	if (state->remainingdatabitlen != 0 && databitlen%128 != 0){
        printf("\n\n !!! Update function must be called with a databitlen which is a multiple of 128 if it is not a final call... \n");	
		return FAIL;
	}
	
	// Update state remaining data bitlen
	state->databitlen += databitlen;
	state->remainingdatabitlen = databitlen%128;

	for(j=0;j<(state->remainingdatabitlen/8 + (state->remainingdatabitlen%8 == 0 ? 0:1));j++)
		state->remainingdata[j/4] |= data[16*messageblockcount+j] << ((3-j%4)*8);
	
	i = 1;
	while(1){
		//LOAD #6*t+1 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^((i+processedmessageblockcount)>>32);   B23 ^=P3^(i+processedmessageblockcount);  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^((i+processedmessageblockcount)>>32);   B33 ^=Q3^(i+processedmessageblockcount);  
		K30 ^=P2;	K31 ^=P3;	K32 ^=Q0;			K33 ^=Q1;
		K50 ^=Q0;	K51 ^=Q1;	K52 ^=Q2;			K53 ^=Q3;
		K70 ^=P0;	K71 ^=P1;	K72 ^=P2;			K73 ^=P3;
		K110^=Q2;	K111^=Q3;	K112^=P0;			K113^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
		
		tmp20 = B00^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B01^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B02^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B03^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B00=tmp20^K90^K00;	B01=tmp21^K91^K01;	B02=tmp22^K92^K02;	B03=tmp23^K93^K03;
		// Write Feed K to the register
		K00=tmp20;	K01=tmp21;	K02=tmp22;	K03=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        
		tmp20 = B10^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B11^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B12^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B13^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
	
        // Calculate Feed B and write it to the register
		B10=tmp20^K100^K10;	B11=tmp21^K101^K11;	B12=tmp22^K102^K12;	B13=tmp23^K103^K13;
        // Write Feed K to the register
		K10=tmp20;	K11=tmp21;	K12=tmp22;	K13=tmp23;

		//LOAD #6*t+2 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^((i+processedmessageblockcount)>>32);   B03 ^=P3^(i+processedmessageblockcount);  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^((i+processedmessageblockcount)>>32);   B13 ^=Q3^(i+processedmessageblockcount);  
		K50 ^=P2;	K51 ^=P3;	K52 ^=Q0;			K53 ^=Q1;
		K70 ^=Q0;	K71 ^=Q1;	K72 ^=Q2;			K73 ^=Q3;
		K90 ^=P0;	K91 ^=P1;	K92 ^=P2;			K93 ^=P3;
		K10 ^=Q2;	K11 ^=Q3;	K12 ^=P0;			K13 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        
		tmp20 = B20^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B21^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B22^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B23^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B20=tmp20^K110^K20;	B21=tmp21^K111^K21;	B22=tmp22^K112^K22;	B23=tmp23^K113^K23;
		// Write Feed K to the register
		K20=tmp20;	K21=tmp21;	K22=tmp22;	K23=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        
		tmp20 = B30^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B31^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B32^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B33^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
        // Calculate Feed B and write it to the register
		B30=tmp20^K00^K30;	B31=tmp21^K01^K31;	B32=tmp22^K02^K32;	B33=tmp23^K03^K33;
        // Write Feed K to the register
		K30=tmp20;	K31=tmp21;	K32=tmp22;	K33=tmp23;

		//LOAD #6*t+3 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^((i+processedmessageblockcount)>>32);   B23 ^=P3^(i+processedmessageblockcount);  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^((i+processedmessageblockcount)>>32);   B33 ^=Q3^(i+processedmessageblockcount);  
		K70 ^=P2;	K71 ^=P3;	K72 ^=Q0;			K73 ^=Q1;
		K90 ^=Q0;	K91 ^=Q1;	K92 ^=Q2;			K93 ^=Q3;
		K110^=P0;	K111^=P1;	K112^=P2;			K113^=P3;
		K30 ^=Q2;	K31 ^=Q3;	K32 ^=P0;			K33 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        
		tmp20 = B00^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B01^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B02^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B03^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
        // Calculate Feed B and write it to the register
		B00=tmp20^K10^K40;	B01=tmp21^K11^K41;	B02=tmp22^K12^K42;	B03=tmp23^K13^K43;
		// Write Feed K to the register
		K40=tmp20;	K41=tmp21;	K42=tmp22;	K43=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        
		tmp20 = B10^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B11^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B12^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B13^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
        // Calculate Feed B and write it to the register
		B10=tmp20^K20^K50;	B11=tmp21^K21^K51;	B12=tmp22^K22^K52;	B13=tmp23^K23^K53;
        // Write Feed K to the register
		K50=tmp20;	K51=tmp21;	K52=tmp22;	K53=tmp23;

		//LOAD #6*t+4 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^((i+processedmessageblockcount)>>32);   B03 ^=P3^(i+processedmessageblockcount);  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^((i+processedmessageblockcount)>>32);   B13 ^=Q3^(i+processedmessageblockcount);  
		K90 ^=P2;	K91 ^=P3;	K92 ^=Q0;			K93 ^=Q1;
		K110^=Q0;	K111^=Q1;	K112^=Q2;			K113^=Q3;
		K10 ^=P0;	K11 ^=P1;	K12 ^=P2;			K13 ^=P3;
		K50 ^=Q2;	K51 ^=Q3;	K52 ^=P0;			K53 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        
		tmp20 = B20^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B21^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B22^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B23^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B20=tmp20^K30^K60;	B21=tmp21^K31^K61;	B22=tmp22^K32^K62;	B23=tmp23^K33^K63;
		// Write Feed K to the register
		K60=tmp20;	K61=tmp21;	K62=tmp22;	K63=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        
		tmp20 = B30^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B31^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B32^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B33^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
        // Calculate Feed B and write it to the register
		B30=tmp20^K40^K70;	B31=tmp21^K41^K71;	B32=tmp22^K42^K72;	B33=tmp23^K43^K73;
        // Write Feed K to the register
		K70=tmp20;	K71=tmp21;	K72=tmp22;	K73=tmp23;

		//LOAD #6*t+5 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^((i+processedmessageblockcount)>>32);   B23 ^=P3^(i+processedmessageblockcount);  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^((i+processedmessageblockcount)>>32);   B33 ^=Q3^(i+processedmessageblockcount);  
		K110^=P2;	K111^=P3;	K112^=Q0;			K113^=Q1;
		K10 ^=Q0;	K11 ^=Q1;	K12 ^=Q2;			K13 ^=Q3;
		K30 ^=P0;	K31 ^=P1;	K32 ^=P2;			K33 ^=P3;
		K70 ^=Q2;	K71 ^=Q3;	K72 ^=P0;			K73 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        
		tmp20 = B00^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B01^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B02^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B03^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B00=tmp20^K50^K80;	B01=tmp21^K51^K81;	B02=tmp22^K52^K82;	B03=tmp23^K53^K83;
		// Write Feed K to the register
		K80=tmp20;	K81=tmp21;	K82=tmp22;	K83=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        
		tmp20 = B10^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B11^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B12^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B13^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
        // Calculate Feed B and write it to the register
		B10=tmp20^K60^K90;	B11=tmp21^K61^K91;	B12=tmp22^K62^K92;	B13=tmp23^K63^K93;
        // Write Feed K to the register
		K90=tmp20;	K91=tmp21;	K92=tmp22;	K93=tmp23;

		//LOAD #6*t+6 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Encode
		Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
		Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
		Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
		Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
		P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
		P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
		P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
		P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^((i+processedmessageblockcount)>>32);   B03 ^=P3^(i+processedmessageblockcount);  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^((i+processedmessageblockcount)>>32);   B13 ^=Q3^(i+processedmessageblockcount);  
		K10 ^=P2;	K11 ^=P3;	K12 ^=Q0;			K13 ^=Q1;
		K30 ^=Q0;	K31 ^=Q1;	K32 ^=Q2;			K33 ^=Q3;
		K50 ^=P0;	K51 ^=P1;	K52 ^=P2;			K53 ^=P3;
		K90 ^=Q2;	K91 ^=Q3;	K92 ^=P0;			K93 ^=P1;
		// Point next block
		data += 16;
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        
		tmp20 = B20^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B21^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B22^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B23^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
		// Calculate Feed B and write it to the register
		B20=tmp20^K70^K100;	B21=tmp21^K71^K101;	B22=tmp22^K72^K102;	B23=tmp23^K73^K103;
		// Write Feed K to the register
		K100=tmp20;	K101=tmp21;	K102=tmp22;	K103=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        
		tmp20 = B30^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B31^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B32^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B33^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B30=tmp20^K80^K110;	B31=tmp21^K81^K111;	B32=tmp22^K82^K112;	B33=tmp23^K83^K113;
        // Write Feed K to the register
		K110=tmp20;	K111=tmp21;	K112=tmp22;	K113=tmp23;
	}

	// Write calculate register values to the state register
	state->B[0][0] = B00;	state->B[0][1] = B01;	state->B[0][2] = B02;	state->B[0][3] = B03;	
	state->B[1][0] = B10;	state->B[1][1] = B11;	state->B[1][2] = B12;	state->B[1][3] = B13;	
	state->B[2][0] = B20;	state->B[2][1] = B21;	state->B[2][2] = B22;	state->B[2][3] = B23;	
	state->B[3][0] = B30;	state->B[3][1] = B31;	state->B[3][2] = B32;	state->B[3][3] = B33;	
	state->K[0][0] = K00;	state->K[0][1] = K01;	state->K[0][2] = K02;	state->K[0][3] = K03;	
	state->K[1][0] = K10;	state->K[1][1] = K11;	state->K[1][2] = K12;	state->K[1][3] = K13;	
	state->K[2][0] = K20;	state->K[2][1] = K21;	state->K[2][2] = K22;	state->K[2][3] = K23;	
	state->K[3][0] = K30;	state->K[3][1] = K31;	state->K[3][2] = K32;	state->K[3][3] = K33;	
	state->K[4][0] = K40;	state->K[4][1] = K41;	state->K[4][2] = K42;	state->K[4][3] = K43;	
	state->K[5][0] = K50;	state->K[5][1] = K51;	state->K[5][2] = K52;	state->K[5][3] = K53;	
	state->K[6][0] = K60;	state->K[6][1] = K61;	state->K[6][2] = K62;	state->K[6][3] = K63;	
	state->K[7][0] = K70;	state->K[7][1] = K71;	state->K[7][2] = K72;	state->K[7][3] = K73;	
	state->K[8][0] = K80;	state->K[8][1] = K81;	state->K[8][2] = K82;	state->K[8][3] = K83;	
	state->K[9][0] = K90;	state->K[9][1] = K91;	state->K[9][2] = K92;	state->K[9][3] = K93;	
	state->K[10][0]= K100;	state->K[10][1]= K101;	state->K[10][2] = K102;	state->K[10][3] = K103;	
	state->K[11][0]= K110;	state->K[11][1]= K111;	state->K[11][2] = K112;	state->K[11][3] = K113;	

	for(i=0;i<((messageblockcount%2)*2);i++){
		tmp10 = state->B[0][0];		tmp11 = state->B[0][1];		
		tmp12 = state->B[0][2];		tmp13 = state->B[0][3];
		for(j=0;j<3;j++){
			state->B[j][0] = state->B[j+1][0];		state->B[j][1] = state->B[j+1][1];		
			state->B[j][2] = state->B[j+1][2];		state->B[j][3] = state->B[j+1][3];
		}
		state->B[3][0] = tmp10;		state->B[3][1] = tmp11;		
		state->B[3][2] = tmp12;		state->B[3][3] = tmp13;
	}
	for(i=0;i<((messageblockcount%6)*2);i++){	
		tmp10 = state->K[0][0];		tmp11 = state->K[0][1];		
		tmp12 = state->K[0][2];		tmp13 = state->K[0][3];
		for(j=0;j<11;j++){
			state->K[j][0] = state->K[j+1][0];		state->K[j][1] = state->K[j+1][1];		
			state->K[j][2] = state->K[j+1][2];		state->K[j][3] = state->K[j+1][3];
		}
		state->K[11][0] = tmp10;		state->K[11][1] = tmp11;		
		state->K[11][2] = tmp12;		state->K[11][3] = tmp13;
	}
	return SUCCESS;
}

int Shamata::Final224_256(hashState *state, BitSequence *hashval)
{
	// The following is looping parameter
	unsigned int i, j,tmp10,tmp11,tmp12,tmp13,tmp20,tmp21,tmp22,tmp23,Q0,Q1,Q2,Q3,P0,P1,P2,P3;
	unsigned char RData[16];
	DataLength processedmessageblockcount = state->databitlen/128;


    // Pad the message and process the calculated block(s)
	if(state->remainingdatabitlen <= 63){   // if remainindatabitlen <= 63 only 1 block will be filled, otherwise 2 blocks is needed
		
		// Add one '1'
		if (state->remainingdatabitlen%32 == 0)
			state->remainingdata[state->remainingdatabitlen/32] &= 0;
		else
			state->remainingdata[state->remainingdatabitlen/32] &= (0xffffffff << (32-(state->remainingdatabitlen%32)));
		state->remainingdata[state->remainingdatabitlen/32] |= (0x80000000 >> (state->remainingdatabitlen%32));

		// Add '0's 
		for(i=state->remainingdatabitlen/32+1;i<2;i++)
			state->remainingdata[i] = 0;
		
		// Add databitlen 
		state->remainingdata[2] = state->databitlen>>32;
		state->remainingdata[3] = state->databitlen;

		// Copy remainingdata to RData int the form of byte array
		for(i=0;i<16;i++)
			RData[i]=(state->remainingdata[i/4]>>(24-8*(i%4)))&0xff;

		// Calculate the current block index
		processedmessageblockcount++;

		Q0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[4]]  ^ SHAMATA_MLH[RData[8]]   ^ SHAMATA_MLL[RData[12]]; 
		Q1 = SHAMATA_MHH[RData[1]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[9]]   ^ SHAMATA_MLL[RData[13]];
		Q2 = SHAMATA_MHH[RData[2]]  ^ SHAMATA_MHL[RData[6]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[14]];
		Q3 = SHAMATA_MHH[RData[3]]  ^ SHAMATA_MHL[RData[7]]  ^ SHAMATA_MLH[RData[11]]  ^ SHAMATA_MLL[RData[15]];
		P0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[1]]  ^ SHAMATA_MLH[RData[2]]   ^ SHAMATA_MLL[RData[3]];
		P1 = SHAMATA_MHH[RData[4]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[6]]   ^ SHAMATA_MLL[RData[7]];
		P2 = SHAMATA_MHH[RData[8]]  ^ SHAMATA_MHL[RData[9]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[11]];
		P3 = SHAMATA_MHH[RData[12]] ^ SHAMATA_MHL[RData[13]] ^ SHAMATA_MLH[RData[14]]  ^ SHAMATA_MLL[RData[15]];
        // Load
		state->B[2][0] ^=P0;	state->B[2][1] ^=P1;	state->B[2][2] ^=P2^(processedmessageblockcount>>32);   state->B[2][3] ^=P3^processedmessageblockcount;  
		state->B[3][0] ^=Q0;	state->B[3][1] ^=Q1;	state->B[3][2] ^=Q2^(processedmessageblockcount>>32);   state->B[3][3] ^=Q3^processedmessageblockcount;  
		state->K[3][0] ^=P2;	state->K[3][1] ^=P3;	state->K[3][2] ^=Q0;									state->K[3][3] ^=Q1;
		state->K[5][0] ^=Q0;	state->K[5][1] ^=Q1;	state->K[5][2] ^=Q2;									state->K[5][3] ^=Q3;
		state->K[7][0] ^=P0;	state->K[7][1] ^=P1;	state->K[7][2] ^=P2;									state->K[7][3] ^=P3;
		state->K[11][0]^=Q2;	state->K[11][1]^=Q3;	state->K[11][2]^=P0;									state->K[11][3]^=P1;
		
		tmp10 = state->B[0][0]^SHAMATA_Shh[(state->B[2][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][3])&0xff];
        tmp11 = state->B[0][1]^SHAMATA_Shh[(state->B[2][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][0])&0xff];
 		tmp12 = state->B[0][2]^SHAMATA_Shh[(state->B[2][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][1])&0xff];
		tmp13 = state->B[0][3]^SHAMATA_Shh[(state->B[2][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][2])&0xff];

		tmp20 = state->B[1][0]^SHAMATA_Shh[(state->B[3][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][3])&0xff];
        tmp21 = state->B[1][1]^SHAMATA_Shh[(state->B[3][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][0])&0xff];
 		tmp22 = state->B[1][2]^SHAMATA_Shh[(state->B[3][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][1])&0xff];
		tmp23 = state->B[1][3]^SHAMATA_Shh[(state->B[3][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][2])&0xff];
        
		state->B[0][0]=state->B[2][0]; state->B[0][1]=state->B[2][1]; state->B[0][2]=state->B[2][2]; state->B[0][3]=state->B[2][3]; 
		state->B[1][0]=state->B[3][0]; state->B[1][1]=state->B[3][1]; state->B[1][2]=state->B[3][2]; state->B[1][3]=state->B[3][3]; 
		state->B[2][0]=tmp10 ^ state->K[9][0] ^ state->K[0][0]; state->B[2][1]=tmp11 ^ state->K[9][1] ^ state->K[0][1]; state->B[2][2]=tmp12 ^ state->K[9][2] ^ state->K[0][2]; state->B[2][3]=tmp13 ^ state->K[9][3] ^ state->K[0][3];
		state->B[3][0]=tmp20 ^ state->K[10][0] ^ state->K[1][0]; state->B[3][1]=tmp21 ^ state->K[10][1] ^ state->K[1][1]; state->B[3][2]=tmp22 ^ state->K[10][2] ^ state->K[1][2]; state->B[3][3]=tmp23 ^ state->K[10][3] ^ state->K[1][3];
		
		state->K[0][0]=state->K[2][0]; state->K[0][1]=state->K[2][1]; state->K[0][2]=state->K[2][2]; state->K[0][3]=state->K[2][3];
		state->K[1][0]=state->K[3][0]; state->K[1][1]=state->K[3][1]; state->K[1][2]=state->K[3][2]; state->K[1][3]=state->K[3][3];
		state->K[2][0]=state->K[4][0]; state->K[2][1]=state->K[4][1]; state->K[2][2]=state->K[4][2]; state->K[2][3]=state->K[4][3];
		state->K[3][0]=state->K[5][0]; state->K[3][1]=state->K[5][1]; state->K[3][2]=state->K[5][2]; state->K[3][3]=state->K[5][3];
		state->K[4][0]=state->K[6][0]; state->K[4][1]=state->K[6][1]; state->K[4][2]=state->K[6][2]; state->K[4][3]=state->K[6][3];
		state->K[5][0]=state->K[7][0]; state->K[5][1]=state->K[7][1]; state->K[5][2]=state->K[7][2]; state->K[5][3]=state->K[7][3];
		state->K[6][0]=state->K[8][0]; state->K[6][1]=state->K[8][1]; state->K[6][2]=state->K[8][2]; state->K[6][3]=state->K[8][3];
		state->K[7][0]=state->K[9][0]; state->K[7][1]=state->K[9][1]; state->K[7][2]=state->K[9][2]; state->K[7][3]=state->K[9][3];
		state->K[8][0]=state->K[10][0]; state->K[8][1]=state->K[10][1]; state->K[8][2]=state->K[10][2]; state->K[8][3]=state->K[10][3];
		state->K[9][0]=state->K[11][0]; state->K[9][1]=state->K[11][1]; state->K[9][2]=state->K[11][2]; state->K[9][3]=state->K[11][3];
		state->K[10][0]=tmp10; state->K[10][1]=tmp11; state->K[10][2]=tmp12; state->K[10][3]=tmp13; 
		state->K[11][0]=tmp20; state->K[11][1]=tmp21; state->K[11][2]=tmp22; state->K[11][3]=tmp23; 


	}else{

		// Add one '1'
		if (state->remainingdatabitlen%32 == 0)
			state->remainingdata[state->remainingdatabitlen/32] &= 0;
		else
			state->remainingdata[state->remainingdatabitlen/32] &= (0xffffffff << (32-(state->remainingdatabitlen%32)));
		state->remainingdata[state->remainingdatabitlen/32] |= (0x80000000 >> (state->remainingdatabitlen%32));

		// Add '0's
		for(i=state->remainingdatabitlen/32+1;i<6;i++)
			state->remainingdata[i] = 0;

		// Add databitlen 
		state->remainingdata[6] = state->databitlen>>32;
		state->remainingdata[7] = state->databitlen;

		// Copy first 128 bits of remainingdata to RData int the form of byte array
        for(i=0;i<16;i++)
			RData[i]=(state->remainingdata[i/4]>>(24-8*(i%4)))&0xff;

		// Calculate the current block index
		processedmessageblockcount++;

		Q0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[4]]  ^ SHAMATA_MLH[RData[8]]   ^ SHAMATA_MLL[RData[12]]; 
		Q1 = SHAMATA_MHH[RData[1]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[9]]   ^ SHAMATA_MLL[RData[13]];
		Q2 = SHAMATA_MHH[RData[2]]  ^ SHAMATA_MHL[RData[6]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[14]];
		Q3 = SHAMATA_MHH[RData[3]]  ^ SHAMATA_MHL[RData[7]]  ^ SHAMATA_MLH[RData[11]]  ^ SHAMATA_MLL[RData[15]];
		P0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[1]]  ^ SHAMATA_MLH[RData[2]]   ^ SHAMATA_MLL[RData[3]];
		P1 = SHAMATA_MHH[RData[4]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[6]]   ^ SHAMATA_MLL[RData[7]];
		P2 = SHAMATA_MHH[RData[8]]  ^ SHAMATA_MHL[RData[9]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[11]];
		P3 = SHAMATA_MHH[RData[12]] ^ SHAMATA_MHL[RData[13]] ^ SHAMATA_MLH[RData[14]]  ^ SHAMATA_MLL[RData[15]];
        // Load
		state->B[2][0] ^=P0;	state->B[2][1] ^=P1;	state->B[2][2] ^=P2^(processedmessageblockcount>>32);   state->B[2][3] ^=P3^processedmessageblockcount;  
		state->B[3][0] ^=Q0;	state->B[3][1] ^=Q1;	state->B[3][2] ^=Q2^(processedmessageblockcount>>32);   state->B[3][3] ^=Q3^processedmessageblockcount;  
		state->K[3][0] ^=P2;	state->K[3][1] ^=P3;	state->K[3][2] ^=Q0;									state->K[3][3] ^=Q1;
		state->K[5][0] ^=Q0;	state->K[5][1] ^=Q1;	state->K[5][2] ^=Q2;									state->K[5][3] ^=Q3;
		state->K[7][0] ^=P0;	state->K[7][1] ^=P1;	state->K[7][2] ^=P2;									state->K[7][3] ^=P3;
		state->K[11][0]^=Q2;	state->K[11][1]^=Q3;	state->K[11][2]^=P0;									state->K[11][3]^=P1;
		
		tmp10 = state->B[0][0]^SHAMATA_Shh[(state->B[2][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][3])&0xff];
        tmp11 = state->B[0][1]^SHAMATA_Shh[(state->B[2][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][0])&0xff];
 		tmp12 = state->B[0][2]^SHAMATA_Shh[(state->B[2][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][1])&0xff];
		tmp13 = state->B[0][3]^SHAMATA_Shh[(state->B[2][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][2])&0xff];

		tmp20 = state->B[1][0]^SHAMATA_Shh[(state->B[3][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][3])&0xff];
        tmp21 = state->B[1][1]^SHAMATA_Shh[(state->B[3][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][0])&0xff];
 		tmp22 = state->B[1][2]^SHAMATA_Shh[(state->B[3][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][1])&0xff];
		tmp23 = state->B[1][3]^SHAMATA_Shh[(state->B[3][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][2])&0xff];
        
		state->B[0][0]=state->B[2][0]; state->B[0][1]=state->B[2][1]; state->B[0][2]=state->B[2][2]; state->B[0][3]=state->B[2][3]; 
		state->B[1][0]=state->B[3][0]; state->B[1][1]=state->B[3][1]; state->B[1][2]=state->B[3][2]; state->B[1][3]=state->B[3][3]; 
		state->B[2][0]=tmp10 ^ state->K[9][0] ^ state->K[0][0]; state->B[2][1]=tmp11 ^ state->K[9][1] ^ state->K[0][1]; state->B[2][2]=tmp12 ^ state->K[9][2] ^ state->K[0][2]; state->B[2][3]=tmp13 ^ state->K[9][3] ^ state->K[0][3];
		state->B[3][0]=tmp20 ^ state->K[10][0] ^ state->K[1][0]; state->B[3][1]=tmp21 ^ state->K[10][1] ^ state->K[1][1]; state->B[3][2]=tmp22 ^ state->K[10][2] ^ state->K[1][2]; state->B[3][3]=tmp23 ^ state->K[10][3] ^ state->K[1][3];
		
		state->K[0][0]=state->K[2][0]; state->K[0][1]=state->K[2][1]; state->K[0][2]=state->K[2][2]; state->K[0][3]=state->K[2][3];
		state->K[1][0]=state->K[3][0]; state->K[1][1]=state->K[3][1]; state->K[1][2]=state->K[3][2]; state->K[1][3]=state->K[3][3];
		state->K[2][0]=state->K[4][0]; state->K[2][1]=state->K[4][1]; state->K[2][2]=state->K[4][2]; state->K[2][3]=state->K[4][3];
		state->K[3][0]=state->K[5][0]; state->K[3][1]=state->K[5][1]; state->K[3][2]=state->K[5][2]; state->K[3][3]=state->K[5][3];
		state->K[4][0]=state->K[6][0]; state->K[4][1]=state->K[6][1]; state->K[4][2]=state->K[6][2]; state->K[4][3]=state->K[6][3];
		state->K[5][0]=state->K[7][0]; state->K[5][1]=state->K[7][1]; state->K[5][2]=state->K[7][2]; state->K[5][3]=state->K[7][3];
		state->K[6][0]=state->K[8][0]; state->K[6][1]=state->K[8][1]; state->K[6][2]=state->K[8][2]; state->K[6][3]=state->K[8][3];
		state->K[7][0]=state->K[9][0]; state->K[7][1]=state->K[9][1]; state->K[7][2]=state->K[9][2]; state->K[7][3]=state->K[9][3];
		state->K[8][0]=state->K[10][0]; state->K[8][1]=state->K[10][1]; state->K[8][2]=state->K[10][2]; state->K[8][3]=state->K[10][3];
		state->K[9][0]=state->K[11][0]; state->K[9][1]=state->K[11][1]; state->K[9][2]=state->K[11][2]; state->K[9][3]=state->K[11][3];
		state->K[10][0]=tmp10; state->K[10][1]=tmp11; state->K[10][2]=tmp12; state->K[10][3]=tmp13; 
		state->K[11][0]=tmp20; state->K[11][1]=tmp21; state->K[11][2]=tmp22; state->K[11][3]=tmp23; 

		// Copy last 128 bits of remainingdata to RData int the form of byte array
		for(i=16;i<32;i++)
			RData[i-16]=(state->remainingdata[i/4]>>(24-8*(i%4)))&0xff;
     
		// Calculate the current block index
		processedmessageblockcount++;

		// Call the compression function for the calculated blocks
		Q0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[4]]  ^ SHAMATA_MLH[RData[8]]   ^ SHAMATA_MLL[RData[12]]; 
		Q1 = SHAMATA_MHH[RData[1]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[9]]   ^ SHAMATA_MLL[RData[13]];
		Q2 = SHAMATA_MHH[RData[2]]  ^ SHAMATA_MHL[RData[6]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[14]];
		Q3 = SHAMATA_MHH[RData[3]]  ^ SHAMATA_MHL[RData[7]]  ^ SHAMATA_MLH[RData[11]]  ^ SHAMATA_MLL[RData[15]];
		P0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[1]]  ^ SHAMATA_MLH[RData[2]]   ^ SHAMATA_MLL[RData[3]];
		P1 = SHAMATA_MHH[RData[4]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[6]]   ^ SHAMATA_MLL[RData[7]];
		P2 = SHAMATA_MHH[RData[8]]  ^ SHAMATA_MHL[RData[9]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[11]];
		P3 = SHAMATA_MHH[RData[12]] ^ SHAMATA_MHL[RData[13]] ^ SHAMATA_MLH[RData[14]]  ^ SHAMATA_MLL[RData[15]];
        // Load
		state->B[2][0] ^=P0;	state->B[2][1] ^=P1;	state->B[2][2] ^=P2^(processedmessageblockcount>>32);   state->B[2][3] ^=P3^processedmessageblockcount;  
		state->B[3][0] ^=Q0;	state->B[3][1] ^=Q1;	state->B[3][2] ^=Q2^(processedmessageblockcount>>32);   state->B[3][3] ^=Q3^processedmessageblockcount;  
		state->K[3][0] ^=P2;	state->K[3][1] ^=P3;	state->K[3][2] ^=Q0;									state->K[3][3] ^=Q1;
		state->K[5][0] ^=Q0;	state->K[5][1] ^=Q1;	state->K[5][2] ^=Q2;									state->K[5][3] ^=Q3;
		state->K[7][0] ^=P0;	state->K[7][1] ^=P1;	state->K[7][2] ^=P2;									state->K[7][3] ^=P3;
		state->K[11][0]^=Q2;	state->K[11][1]^=Q3;	state->K[11][2]^=P0;									state->K[11][3]^=P1;
		
		tmp10 = state->B[0][0]^SHAMATA_Shh[(state->B[2][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][3])&0xff];
        tmp11 = state->B[0][1]^SHAMATA_Shh[(state->B[2][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][0])&0xff];
 		tmp12 = state->B[0][2]^SHAMATA_Shh[(state->B[2][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][1])&0xff];
		tmp13 = state->B[0][3]^SHAMATA_Shh[(state->B[2][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][2])&0xff];

		tmp20 = state->B[1][0]^SHAMATA_Shh[(state->B[3][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][3])&0xff];
        tmp21 = state->B[1][1]^SHAMATA_Shh[(state->B[3][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][0])&0xff];
 		tmp22 = state->B[1][2]^SHAMATA_Shh[(state->B[3][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][1])&0xff];
		tmp23 = state->B[1][3]^SHAMATA_Shh[(state->B[3][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][2])&0xff];
        
		state->B[0][0]=state->B[2][0]; state->B[0][1]=state->B[2][1]; state->B[0][2]=state->B[2][2]; state->B[0][3]=state->B[2][3]; 
		state->B[1][0]=state->B[3][0]; state->B[1][1]=state->B[3][1]; state->B[1][2]=state->B[3][2]; state->B[1][3]=state->B[3][3]; 
		state->B[2][0]=tmp10 ^ state->K[9][0] ^ state->K[0][0]; state->B[2][1]=tmp11 ^ state->K[9][1] ^ state->K[0][1]; state->B[2][2]=tmp12 ^ state->K[9][2] ^ state->K[0][2]; state->B[2][3]=tmp13 ^ state->K[9][3] ^ state->K[0][3];
		state->B[3][0]=tmp20 ^ state->K[10][0] ^ state->K[1][0]; state->B[3][1]=tmp21 ^ state->K[10][1] ^ state->K[1][1]; state->B[3][2]=tmp22 ^ state->K[10][2] ^ state->K[1][2]; state->B[3][3]=tmp23 ^ state->K[10][3] ^ state->K[1][3];
		
		state->K[0][0]=state->K[2][0]; state->K[0][1]=state->K[2][1]; state->K[0][2]=state->K[2][2]; state->K[0][3]=state->K[2][3];
		state->K[1][0]=state->K[3][0]; state->K[1][1]=state->K[3][1]; state->K[1][2]=state->K[3][2]; state->K[1][3]=state->K[3][3];
		state->K[2][0]=state->K[4][0]; state->K[2][1]=state->K[4][1]; state->K[2][2]=state->K[4][2]; state->K[2][3]=state->K[4][3];
		state->K[3][0]=state->K[5][0]; state->K[3][1]=state->K[5][1]; state->K[3][2]=state->K[5][2]; state->K[3][3]=state->K[5][3];
		state->K[4][0]=state->K[6][0]; state->K[4][1]=state->K[6][1]; state->K[4][2]=state->K[6][2]; state->K[4][3]=state->K[6][3];
		state->K[5][0]=state->K[7][0]; state->K[5][1]=state->K[7][1]; state->K[5][2]=state->K[7][2]; state->K[5][3]=state->K[7][3];
		state->K[6][0]=state->K[8][0]; state->K[6][1]=state->K[8][1]; state->K[6][2]=state->K[8][2]; state->K[6][3]=state->K[8][3];
		state->K[7][0]=state->K[9][0]; state->K[7][1]=state->K[9][1]; state->K[7][2]=state->K[9][2]; state->K[7][3]=state->K[9][3];
		state->K[8][0]=state->K[10][0]; state->K[8][1]=state->K[10][1]; state->K[8][2]=state->K[10][2]; state->K[8][3]=state->K[10][3];
		state->K[9][0]=state->K[11][0]; state->K[9][1]=state->K[11][1]; state->K[9][2]=state->K[11][2]; state->K[9][3]=state->K[11][3];
		state->K[10][0]=tmp10; state->K[10][1]=tmp11; state->K[10][2]=tmp12; state->K[10][3]=tmp13; 
		state->K[11][0]=tmp20; state->K[11][1]=tmp21; state->K[11][2]=tmp22; state->K[11][3]=tmp23; 


	}

	// Copy number of message blokc count to RData in the form of byte array
	for(i=0;i<16;i++)
		RData[i]=(processedmessageblockcount>>(120-8*(i%16)));

	// Call UpdateRegister 32 times using number of message blocks as block
	FinalUpdate224_256(state,RData);	

	// Produce hash value
	ProduceOutput(state,hashval);	

	return SUCCESS;
}
int Shamata::Final384_512(hashState *state, BitSequence *hashval)
{
	// The following is looping parameter
	unsigned int i, j,tmp10,tmp11,tmp12,tmp13,tmp20,tmp21,tmp22,tmp30,tmp31,tmp32,tmp33,tmp40,tmp41,tmp42,tmp43,tmp23,Q0,Q1,Q2,Q3,P0,P1,P2,P3;
	unsigned char RData[16];
	DataLength processedmessageblockcount = state->databitlen/128;


    // Pad the message and process the calculated block(s)
	if(state->remainingdatabitlen <= 63){   // if remainindatabitlen <= 63 only 1 block will be filled, otherwise 2 blocks is needed
		
		// Add one '1'
		if (state->remainingdatabitlen%32 == 0)
			state->remainingdata[state->remainingdatabitlen/32] &= 0;
		else
			state->remainingdata[state->remainingdatabitlen/32] &= (0xffffffff << (32-(state->remainingdatabitlen%32)));
		state->remainingdata[state->remainingdatabitlen/32] |= (0x80000000 >> (state->remainingdatabitlen%32));

		// Add '0's 
		for(i=state->remainingdatabitlen/32+1;i<2;i++)
			state->remainingdata[i] = 0;
		
		// Add databitlen 
		state->remainingdata[2] = state->databitlen>>32;
		state->remainingdata[3] = state->databitlen;

		// Copy remainingdata to RData int the form of byte array
		for(i=0;i<16;i++)
			RData[i]=(state->remainingdata[i/4]>>(24-8*(i%4)))&0xff;

		// Calculate the current block index
		processedmessageblockcount++;

		Q0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[4]]  ^ SHAMATA_MLH[RData[8]]   ^ SHAMATA_MLL[RData[12]]; 
		Q1 = SHAMATA_MHH[RData[1]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[9]]   ^ SHAMATA_MLL[RData[13]];
		Q2 = SHAMATA_MHH[RData[2]]  ^ SHAMATA_MHL[RData[6]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[14]];
		Q3 = SHAMATA_MHH[RData[3]]  ^ SHAMATA_MHL[RData[7]]  ^ SHAMATA_MLH[RData[11]]  ^ SHAMATA_MLL[RData[15]];
		P0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[1]]  ^ SHAMATA_MLH[RData[2]]   ^ SHAMATA_MLL[RData[3]];
		P1 = SHAMATA_MHH[RData[4]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[6]]   ^ SHAMATA_MLL[RData[7]];
		P2 = SHAMATA_MHH[RData[8]]  ^ SHAMATA_MHL[RData[9]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[11]];
		P3 = SHAMATA_MHH[RData[12]] ^ SHAMATA_MHL[RData[13]] ^ SHAMATA_MLH[RData[14]]  ^ SHAMATA_MLL[RData[15]];
        // Load
		state->B[2][0] ^=P0;	state->B[2][1] ^=P1;	state->B[2][2] ^=P2^(processedmessageblockcount>>32);   state->B[2][3] ^=P3^processedmessageblockcount;  
		state->B[3][0] ^=Q0;	state->B[3][1] ^=Q1;	state->B[3][2] ^=Q2^(processedmessageblockcount>>32);   state->B[3][3] ^=Q3^processedmessageblockcount;  
		state->K[3][0] ^=P2;	state->K[3][1] ^=P3;	state->K[3][2] ^=Q0;									state->K[3][3] ^=Q1;
		state->K[5][0] ^=Q0;	state->K[5][1] ^=Q1;	state->K[5][2] ^=Q2;									state->K[5][3] ^=Q3;
		state->K[7][0] ^=P0;	state->K[7][1] ^=P1;	state->K[7][2] ^=P2;									state->K[7][3] ^=P3;
		state->K[11][0]^=Q2;	state->K[11][1]^=Q3;	state->K[11][2]^=P0;									state->K[11][3]^=P1;
		
		tmp10 = SHAMATA_Shh[(state->B[2][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][3])&0xff];
        tmp11 = SHAMATA_Shh[(state->B[2][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][0])&0xff];
 		tmp12 = SHAMATA_Shh[(state->B[2][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][1])&0xff];
		tmp13 = SHAMATA_Shh[(state->B[2][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][2])&0xff];

		tmp30 = state->B[0][0]^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp31 = state->B[0][1]^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp32 = state->B[0][2]^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp33 = state->B[0][3]^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
		
		tmp20 = SHAMATA_Shh[(state->B[3][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][3])&0xff];
        tmp21 = SHAMATA_Shh[(state->B[3][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][0])&0xff];
 		tmp22 = SHAMATA_Shh[(state->B[3][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][1])&0xff];
		tmp23 = SHAMATA_Shh[(state->B[3][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][2])&0xff];
        
		tmp40 = state->B[1][0]^SHAMATA_Shh[(tmp20>>24)&0xff] ^ SHAMATA_Shl[(tmp21>>16)&0xff] ^ SHAMATA_Slh[(tmp22>>8)&0xff] ^ SHAMATA_Sll[(tmp23)&0xff];
        tmp41 = state->B[1][1]^SHAMATA_Shh[(tmp21>>24)&0xff] ^ SHAMATA_Shl[(tmp22>>16)&0xff] ^ SHAMATA_Slh[(tmp23>>8)&0xff] ^ SHAMATA_Sll[(tmp20)&0xff];
 		tmp42 = state->B[1][2]^SHAMATA_Shh[(tmp22>>24)&0xff] ^ SHAMATA_Shl[(tmp23>>16)&0xff] ^ SHAMATA_Slh[(tmp20>>8)&0xff] ^ SHAMATA_Sll[(tmp21)&0xff];
		tmp43 = state->B[1][3]^SHAMATA_Shh[(tmp23>>24)&0xff] ^ SHAMATA_Shl[(tmp20>>16)&0xff] ^ SHAMATA_Slh[(tmp21>>8)&0xff] ^ SHAMATA_Sll[(tmp22)&0xff];

		state->B[0][0]=state->B[2][0]; state->B[0][1]=state->B[2][1]; state->B[0][2]=state->B[2][2]; state->B[0][3]=state->B[2][3]; 
		state->B[1][0]=state->B[3][0]; state->B[1][1]=state->B[3][1]; state->B[1][2]=state->B[3][2]; state->B[1][3]=state->B[3][3]; 
		state->B[2][0]=tmp30 ^ state->K[9][0] ^ state->K[0][0]; state->B[2][1]=tmp31 ^ state->K[9][1] ^ state->K[0][1]; state->B[2][2]=tmp32 ^ state->K[9][2] ^ state->K[0][2]; state->B[2][3]=tmp33 ^ state->K[9][3] ^ state->K[0][3];
		state->B[3][0]=tmp40 ^ state->K[10][0] ^ state->K[1][0]; state->B[3][1]=tmp41 ^ state->K[10][1] ^ state->K[1][1]; state->B[3][2]=tmp42 ^ state->K[10][2] ^ state->K[1][2]; state->B[3][3]=tmp43 ^ state->K[10][3] ^ state->K[1][3];
		
		state->K[0][0]=state->K[2][0]; state->K[0][1]=state->K[2][1]; state->K[0][2]=state->K[2][2]; state->K[0][3]=state->K[2][3];
		state->K[1][0]=state->K[3][0]; state->K[1][1]=state->K[3][1]; state->K[1][2]=state->K[3][2]; state->K[1][3]=state->K[3][3];
		state->K[2][0]=state->K[4][0]; state->K[2][1]=state->K[4][1]; state->K[2][2]=state->K[4][2]; state->K[2][3]=state->K[4][3];
		state->K[3][0]=state->K[5][0]; state->K[3][1]=state->K[5][1]; state->K[3][2]=state->K[5][2]; state->K[3][3]=state->K[5][3];
		state->K[4][0]=state->K[6][0]; state->K[4][1]=state->K[6][1]; state->K[4][2]=state->K[6][2]; state->K[4][3]=state->K[6][3];
		state->K[5][0]=state->K[7][0]; state->K[5][1]=state->K[7][1]; state->K[5][2]=state->K[7][2]; state->K[5][3]=state->K[7][3];
		state->K[6][0]=state->K[8][0]; state->K[6][1]=state->K[8][1]; state->K[6][2]=state->K[8][2]; state->K[6][3]=state->K[8][3];
		state->K[7][0]=state->K[9][0]; state->K[7][1]=state->K[9][1]; state->K[7][2]=state->K[9][2]; state->K[7][3]=state->K[9][3];
		state->K[8][0]=state->K[10][0]; state->K[8][1]=state->K[10][1]; state->K[8][2]=state->K[10][2]; state->K[8][3]=state->K[10][3];
		state->K[9][0]=state->K[11][0]; state->K[9][1]=state->K[11][1]; state->K[9][2]=state->K[11][2]; state->K[9][3]=state->K[11][3];
		state->K[10][0]=tmp30; state->K[10][1]=tmp31; state->K[10][2]=tmp32; state->K[10][3]=tmp33; 
		state->K[11][0]=tmp40; state->K[11][1]=tmp41; state->K[11][2]=tmp42; state->K[11][3]=tmp43; 


	}else{

		// Add one '1'
		if (state->remainingdatabitlen%32 == 0)
			state->remainingdata[state->remainingdatabitlen/32] &= 0;
		else
			state->remainingdata[state->remainingdatabitlen/32] &= (0xffffffff << (32-(state->remainingdatabitlen%32)));
		state->remainingdata[state->remainingdatabitlen/32] |= (0x80000000 >> (state->remainingdatabitlen%32));

		// Add '0's
		for(i=state->remainingdatabitlen/32+1;i<6;i++)
			state->remainingdata[i] = 0;

		// Add databitlen 
		state->remainingdata[6] = state->databitlen>>32;
		state->remainingdata[7] = state->databitlen;

		// Copy first 128 bits of remainingdata to RData int the form of byte array
        for(i=0;i<16;i++)
			RData[i]=(state->remainingdata[i/4]>>(24-8*(i%4)))&0xff;

		// Calculate the current block index
		processedmessageblockcount++;

		Q0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[4]]  ^ SHAMATA_MLH[RData[8]]   ^ SHAMATA_MLL[RData[12]]; 
		Q1 = SHAMATA_MHH[RData[1]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[9]]   ^ SHAMATA_MLL[RData[13]];
		Q2 = SHAMATA_MHH[RData[2]]  ^ SHAMATA_MHL[RData[6]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[14]];
		Q3 = SHAMATA_MHH[RData[3]]  ^ SHAMATA_MHL[RData[7]]  ^ SHAMATA_MLH[RData[11]]  ^ SHAMATA_MLL[RData[15]];
		P0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[1]]  ^ SHAMATA_MLH[RData[2]]   ^ SHAMATA_MLL[RData[3]];
		P1 = SHAMATA_MHH[RData[4]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[6]]   ^ SHAMATA_MLL[RData[7]];
		P2 = SHAMATA_MHH[RData[8]]  ^ SHAMATA_MHL[RData[9]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[11]];
		P3 = SHAMATA_MHH[RData[12]] ^ SHAMATA_MHL[RData[13]] ^ SHAMATA_MLH[RData[14]]  ^ SHAMATA_MLL[RData[15]];
        // Load
		state->B[2][0] ^=P0;	state->B[2][1] ^=P1;	state->B[2][2] ^=P2^(processedmessageblockcount>>32);   state->B[2][3] ^=P3^processedmessageblockcount;  
		state->B[3][0] ^=Q0;	state->B[3][1] ^=Q1;	state->B[3][2] ^=Q2^(processedmessageblockcount>>32);   state->B[3][3] ^=Q3^processedmessageblockcount;  
		state->K[3][0] ^=P2;	state->K[3][1] ^=P3;	state->K[3][2] ^=Q0;									state->K[3][3] ^=Q1;
		state->K[5][0] ^=Q0;	state->K[5][1] ^=Q1;	state->K[5][2] ^=Q2;									state->K[5][3] ^=Q3;
		state->K[7][0] ^=P0;	state->K[7][1] ^=P1;	state->K[7][2] ^=P2;									state->K[7][3] ^=P3;
		state->K[11][0]^=Q2;	state->K[11][1]^=Q3;	state->K[11][2]^=P0;									state->K[11][3]^=P1;
		
		tmp10 = SHAMATA_Shh[(state->B[2][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][3])&0xff];
        tmp11 = SHAMATA_Shh[(state->B[2][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][0])&0xff];
 		tmp12 = SHAMATA_Shh[(state->B[2][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][1])&0xff];
		tmp13 = SHAMATA_Shh[(state->B[2][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][2])&0xff];

		tmp30 = state->B[0][0]^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp31 = state->B[0][1]^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp32 = state->B[0][2]^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp33 = state->B[0][3]^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
		tmp20 = SHAMATA_Shh[(state->B[3][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][3])&0xff];
        tmp21 = SHAMATA_Shh[(state->B[3][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][0])&0xff];
 		tmp22 = SHAMATA_Shh[(state->B[3][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][1])&0xff];
		tmp23 = SHAMATA_Shh[(state->B[3][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][2])&0xff];
        
		tmp40 = state->B[1][0]^SHAMATA_Shh[(tmp20>>24)&0xff] ^ SHAMATA_Shl[(tmp21>>16)&0xff] ^ SHAMATA_Slh[(tmp22>>8)&0xff] ^ SHAMATA_Sll[(tmp23)&0xff];
        tmp41 = state->B[1][1]^SHAMATA_Shh[(tmp21>>24)&0xff] ^ SHAMATA_Shl[(tmp22>>16)&0xff] ^ SHAMATA_Slh[(tmp23>>8)&0xff] ^ SHAMATA_Sll[(tmp20)&0xff];
 		tmp42 = state->B[1][2]^SHAMATA_Shh[(tmp22>>24)&0xff] ^ SHAMATA_Shl[(tmp23>>16)&0xff] ^ SHAMATA_Slh[(tmp20>>8)&0xff] ^ SHAMATA_Sll[(tmp21)&0xff];
		tmp43 = state->B[1][3]^SHAMATA_Shh[(tmp23>>24)&0xff] ^ SHAMATA_Shl[(tmp20>>16)&0xff] ^ SHAMATA_Slh[(tmp21>>8)&0xff] ^ SHAMATA_Sll[(tmp22)&0xff];

		state->B[0][0]=state->B[2][0]; state->B[0][1]=state->B[2][1]; state->B[0][2]=state->B[2][2]; state->B[0][3]=state->B[2][3]; 
		state->B[1][0]=state->B[3][0]; state->B[1][1]=state->B[3][1]; state->B[1][2]=state->B[3][2]; state->B[1][3]=state->B[3][3]; 
		state->B[2][0]=tmp30 ^ state->K[9][0] ^ state->K[0][0]; state->B[2][1]=tmp31 ^ state->K[9][1] ^ state->K[0][1]; state->B[2][2]=tmp32 ^ state->K[9][2] ^ state->K[0][2]; state->B[2][3]=tmp33 ^ state->K[9][3] ^ state->K[0][3];
		state->B[3][0]=tmp40 ^ state->K[10][0] ^ state->K[1][0]; state->B[3][1]=tmp41 ^ state->K[10][1] ^ state->K[1][1]; state->B[3][2]=tmp42 ^ state->K[10][2] ^ state->K[1][2]; state->B[3][3]=tmp43 ^ state->K[10][3] ^ state->K[1][3];
		
		state->K[0][0]=state->K[2][0]; state->K[0][1]=state->K[2][1]; state->K[0][2]=state->K[2][2]; state->K[0][3]=state->K[2][3];
		state->K[1][0]=state->K[3][0]; state->K[1][1]=state->K[3][1]; state->K[1][2]=state->K[3][2]; state->K[1][3]=state->K[3][3];
		state->K[2][0]=state->K[4][0]; state->K[2][1]=state->K[4][1]; state->K[2][2]=state->K[4][2]; state->K[2][3]=state->K[4][3];
		state->K[3][0]=state->K[5][0]; state->K[3][1]=state->K[5][1]; state->K[3][2]=state->K[5][2]; state->K[3][3]=state->K[5][3];
		state->K[4][0]=state->K[6][0]; state->K[4][1]=state->K[6][1]; state->K[4][2]=state->K[6][2]; state->K[4][3]=state->K[6][3];
		state->K[5][0]=state->K[7][0]; state->K[5][1]=state->K[7][1]; state->K[5][2]=state->K[7][2]; state->K[5][3]=state->K[7][3];
		state->K[6][0]=state->K[8][0]; state->K[6][1]=state->K[8][1]; state->K[6][2]=state->K[8][2]; state->K[6][3]=state->K[8][3];
		state->K[7][0]=state->K[9][0]; state->K[7][1]=state->K[9][1]; state->K[7][2]=state->K[9][2]; state->K[7][3]=state->K[9][3];
		state->K[8][0]=state->K[10][0]; state->K[8][1]=state->K[10][1]; state->K[8][2]=state->K[10][2]; state->K[8][3]=state->K[10][3];
		state->K[9][0]=state->K[11][0]; state->K[9][1]=state->K[11][1]; state->K[9][2]=state->K[11][2]; state->K[9][3]=state->K[11][3];
		state->K[10][0]=tmp30; state->K[10][1]=tmp31; state->K[10][2]=tmp32; state->K[10][3]=tmp33; 
		state->K[11][0]=tmp40; state->K[11][1]=tmp41; state->K[11][2]=tmp42; state->K[11][3]=tmp43; 

		// Copy last 128 bits of remainingdata to RData int the form of byte array
		for(i=16;i<32;i++)
			RData[i-16]=(state->remainingdata[i/4]>>(24-8*(i%4)))&0xff;
     
		// Calculate the current block index
		processedmessageblockcount++;

		// Call the compression function for the calculated blocks
		Q0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[4]]  ^ SHAMATA_MLH[RData[8]]   ^ SHAMATA_MLL[RData[12]]; 
		Q1 = SHAMATA_MHH[RData[1]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[9]]   ^ SHAMATA_MLL[RData[13]];
		Q2 = SHAMATA_MHH[RData[2]]  ^ SHAMATA_MHL[RData[6]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[14]];
		Q3 = SHAMATA_MHH[RData[3]]  ^ SHAMATA_MHL[RData[7]]  ^ SHAMATA_MLH[RData[11]]  ^ SHAMATA_MLL[RData[15]];
		P0 = SHAMATA_MHH[RData[0]]  ^ SHAMATA_MHL[RData[1]]  ^ SHAMATA_MLH[RData[2]]   ^ SHAMATA_MLL[RData[3]];
		P1 = SHAMATA_MHH[RData[4]]  ^ SHAMATA_MHL[RData[5]]  ^ SHAMATA_MLH[RData[6]]   ^ SHAMATA_MLL[RData[7]];
		P2 = SHAMATA_MHH[RData[8]]  ^ SHAMATA_MHL[RData[9]]  ^ SHAMATA_MLH[RData[10]]  ^ SHAMATA_MLL[RData[11]];
		P3 = SHAMATA_MHH[RData[12]] ^ SHAMATA_MHL[RData[13]] ^ SHAMATA_MLH[RData[14]]  ^ SHAMATA_MLL[RData[15]];
        // Load
		state->B[2][0] ^=P0;	state->B[2][1] ^=P1;	state->B[2][2] ^=P2^(processedmessageblockcount>>32);   state->B[2][3] ^=P3^processedmessageblockcount;  
		state->B[3][0] ^=Q0;	state->B[3][1] ^=Q1;	state->B[3][2] ^=Q2^(processedmessageblockcount>>32);   state->B[3][3] ^=Q3^processedmessageblockcount;  
		state->K[3][0] ^=P2;	state->K[3][1] ^=P3;	state->K[3][2] ^=Q0;									state->K[3][3] ^=Q1;
		state->K[5][0] ^=Q0;	state->K[5][1] ^=Q1;	state->K[5][2] ^=Q2;									state->K[5][3] ^=Q3;
		state->K[7][0] ^=P0;	state->K[7][1] ^=P1;	state->K[7][2] ^=P2;									state->K[7][3] ^=P3;
		state->K[11][0]^=Q2;	state->K[11][1]^=Q3;	state->K[11][2]^=P0;									state->K[11][3]^=P1;
		
		tmp10 = SHAMATA_Shh[(state->B[2][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][3])&0xff];
        tmp11 = SHAMATA_Shh[(state->B[2][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][0])&0xff];
 		tmp12 = SHAMATA_Shh[(state->B[2][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][1])&0xff];
		tmp13 = SHAMATA_Shh[(state->B[2][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[2][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[2][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[2][2])&0xff];

		tmp30 = state->B[0][0]^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp31 = state->B[0][1]^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp32 = state->B[0][2]^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp33 = state->B[0][3]^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
		
		tmp20 = SHAMATA_Shh[(state->B[3][0]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][1]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][2]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][3])&0xff];
        tmp21 = SHAMATA_Shh[(state->B[3][1]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][2]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][3]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][0])&0xff];
 		tmp22 = SHAMATA_Shh[(state->B[3][2]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][3]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][0]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][1])&0xff];
		tmp23 = SHAMATA_Shh[(state->B[3][3]>>24)&0xff] ^ SHAMATA_Shl[(state->B[3][0]>>16)&0xff] ^ SHAMATA_Slh[(state->B[3][1]>>8)&0xff] ^ SHAMATA_Sll[(state->B[3][2])&0xff];
        
		tmp40 = state->B[1][0]^SHAMATA_Shh[(tmp20>>24)&0xff] ^ SHAMATA_Shl[(tmp21>>16)&0xff] ^ SHAMATA_Slh[(tmp22>>8)&0xff] ^ SHAMATA_Sll[(tmp23)&0xff];
        tmp41 = state->B[1][1]^SHAMATA_Shh[(tmp21>>24)&0xff] ^ SHAMATA_Shl[(tmp22>>16)&0xff] ^ SHAMATA_Slh[(tmp23>>8)&0xff] ^ SHAMATA_Sll[(tmp20)&0xff];
 		tmp42 = state->B[1][2]^SHAMATA_Shh[(tmp22>>24)&0xff] ^ SHAMATA_Shl[(tmp23>>16)&0xff] ^ SHAMATA_Slh[(tmp20>>8)&0xff] ^ SHAMATA_Sll[(tmp21)&0xff];
		tmp43 = state->B[1][3]^SHAMATA_Shh[(tmp23>>24)&0xff] ^ SHAMATA_Shl[(tmp20>>16)&0xff] ^ SHAMATA_Slh[(tmp21>>8)&0xff] ^ SHAMATA_Sll[(tmp22)&0xff];

		state->B[0][0]=state->B[2][0]; state->B[0][1]=state->B[2][1]; state->B[0][2]=state->B[2][2]; state->B[0][3]=state->B[2][3]; 
		state->B[1][0]=state->B[3][0]; state->B[1][1]=state->B[3][1]; state->B[1][2]=state->B[3][2]; state->B[1][3]=state->B[3][3]; 
		state->B[2][0]=tmp30 ^ state->K[9][0] ^ state->K[0][0]; state->B[2][1]=tmp31 ^ state->K[9][1] ^ state->K[0][1]; state->B[2][2]=tmp32 ^ state->K[9][2] ^ state->K[0][2]; state->B[2][3]=tmp33 ^ state->K[9][3] ^ state->K[0][3];
		state->B[3][0]=tmp40 ^ state->K[10][0] ^ state->K[1][0]; state->B[3][1]=tmp41 ^ state->K[10][1] ^ state->K[1][1]; state->B[3][2]=tmp42 ^ state->K[10][2] ^ state->K[1][2]; state->B[3][3]=tmp43 ^ state->K[10][3] ^ state->K[1][3];
		
		state->K[0][0]=state->K[2][0]; state->K[0][1]=state->K[2][1]; state->K[0][2]=state->K[2][2]; state->K[0][3]=state->K[2][3];
		state->K[1][0]=state->K[3][0]; state->K[1][1]=state->K[3][1]; state->K[1][2]=state->K[3][2]; state->K[1][3]=state->K[3][3];
		state->K[2][0]=state->K[4][0]; state->K[2][1]=state->K[4][1]; state->K[2][2]=state->K[4][2]; state->K[2][3]=state->K[4][3];
		state->K[3][0]=state->K[5][0]; state->K[3][1]=state->K[5][1]; state->K[3][2]=state->K[5][2]; state->K[3][3]=state->K[5][3];
		state->K[4][0]=state->K[6][0]; state->K[4][1]=state->K[6][1]; state->K[4][2]=state->K[6][2]; state->K[4][3]=state->K[6][3];
		state->K[5][0]=state->K[7][0]; state->K[5][1]=state->K[7][1]; state->K[5][2]=state->K[7][2]; state->K[5][3]=state->K[7][3];
		state->K[6][0]=state->K[8][0]; state->K[6][1]=state->K[8][1]; state->K[6][2]=state->K[8][2]; state->K[6][3]=state->K[8][3];
		state->K[7][0]=state->K[9][0]; state->K[7][1]=state->K[9][1]; state->K[7][2]=state->K[9][2]; state->K[7][3]=state->K[9][3];
		state->K[8][0]=state->K[10][0]; state->K[8][1]=state->K[10][1]; state->K[8][2]=state->K[10][2]; state->K[8][3]=state->K[10][3];
		state->K[9][0]=state->K[11][0]; state->K[9][1]=state->K[11][1]; state->K[9][2]=state->K[11][2]; state->K[9][3]=state->K[11][3];
		state->K[10][0]=tmp30; state->K[10][1]=tmp31; state->K[10][2]=tmp32; state->K[10][3]=tmp33; 
		state->K[11][0]=tmp40; state->K[11][1]=tmp41; state->K[11][2]=tmp42; state->K[11][3]=tmp43; 


	}

	// Copy number of message blokc count to RData in the form of byte array
	for(i=0;i<16;i++)
		RData[i]=(processedmessageblockcount>>(120-8*(i%16)));

	// Call UpdateRegister 32 times using number of message blocks as block
	FinalUpdate384_512(state,RData);
	// Produce hash value
	ProduceOutput(state,hashval);	

	return SUCCESS;
}




int Shamata::FinalUpdate224_256 (hashState *state, const BitSequence *data)
{ 
	unsigned long long i,messageblockcount=32; 
	unsigned int j, tmp10, tmp11, tmp12, tmp13, Q0, Q1, Q2, Q3, P0, P1, P2, P3,
		               B00 = state->B[0][0],	B01 = state->B[0][1],	B02 = state->B[0][2],	B03 = state->B[0][3], 
					   B10 = state->B[1][0],	B11 = state->B[1][1],	B12 = state->B[1][2],	B13 = state->B[1][3], 
					   B20 = state->B[2][0],	B21 = state->B[2][1],	B22 = state->B[2][2],	B23 = state->B[2][3], 
					   B30 = state->B[3][0],	B31 = state->B[3][1],	B32 = state->B[3][2],	B33 = state->B[3][3], 
				       K00 = state->K[0][0],	K01 = state->K[0][1],	K02 = state->K[0][2],	K03 = state->K[0][3], 
					   K10 = state->K[1][0],	K11 = state->K[1][1],	K12 = state->K[1][2],	K13 = state->K[1][3], 
					   K20 = state->K[2][0],	K21 = state->K[2][1],	K22 = state->K[2][2],	K23 = state->K[2][3], 
					   K30 = state->K[3][0],	K31 = state->K[3][1],	K32 = state->K[3][2],	K33 = state->K[3][3], 
					   K40 = state->K[4][0],	K41 = state->K[4][1],	K42 = state->K[4][2],	K43 = state->K[4][3], 
					   K50 = state->K[5][0],	K51 = state->K[5][1],	K52 = state->K[5][2],	K53 = state->K[5][3], 
					   K60 = state->K[6][0],	K61 = state->K[6][1],	K62 = state->K[6][2],	K63 = state->K[6][3], 
					   K70 = state->K[7][0],	K71 = state->K[7][1],	K72 = state->K[7][2],	K73 = state->K[7][3],
					   K80 = state->K[8][0],	K81 = state->K[8][1],	K82 = state->K[8][2],	K83 = state->K[8][3], 
					   K90 = state->K[9][0],	K91 = state->K[9][1],	K92 = state->K[9][2],	K93 = state->K[9][3], 
					   K100 = state->K[10][0],	K101 = state->K[10][1], K102 = state->K[10][2], K103 = state->K[10][3], 
					   K110 = state->K[11][0],	K111 = state->K[11][1], K112 = state->K[11][2], K113 = state->K[11][3];
	
	i = 1;
	
	Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
	Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
	Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
	Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
	P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
	P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
	P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
	P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];

	while(1){
		//LOAD #6*t+1 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		// Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^(i>>32);   B23 ^=P3^i;  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^(i>>32);   B33 ^=Q3^i;  
		K30 ^=P2;	K31 ^=P3;	K32 ^=Q0;			K33 ^=Q1;
		K50 ^=Q0;	K51 ^=Q1;	K52 ^=Q2;			K53 ^=Q3;
		K70 ^=P0;	K71 ^=P1;	K72 ^=P2;			K73 ^=P3;
		K110^=Q2;	K111^=Q3;	K112^=P0;			K113^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        // Calculate Feed K
		tmp10^=B00;	tmp11^=B01;	tmp12^=B02;	tmp13^=B03;
        // Calculate Feed B and write it to the register
		B00=tmp10^K90^K00;	B01=tmp11^K91^K01;	B02=tmp12^K92^K02;	B03=tmp13^K93^K03;
		// Write Feed K to the register
		K00=tmp10;	K01=tmp11;	K02=tmp12;	K03=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        // Calculate Feed K
		tmp10^=B10;	tmp11^=B11;	tmp12^=B12;	tmp13^=B13;
        // Calculate Feed B and write it to the register
		B10=tmp10^K100^K10;	B11=tmp11^K101^K11;	B12=tmp12^K102^K12;	B13=tmp13^K103^K13;
        // Write Feed K to the register
		K10=tmp10;	K11=tmp11;	K12=tmp12;	K13=tmp13;

		//LOAD #6*t+2 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^(i>>32);   B03 ^=P3^i;  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^(i>>32);   B13 ^=Q3^i;  
		K50 ^=P2;	K51 ^=P3;	K52 ^=Q0;			K53 ^=Q1;
		K70 ^=Q0;	K71 ^=Q1;	K72 ^=Q2;			K73 ^=Q3;
		K90 ^=P0;	K91 ^=P1;	K92 ^=P2;			K93 ^=P3;
		K10 ^=Q2;	K11 ^=Q3;	K12 ^=P0;			K13 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        // Calculate Feed K
		tmp10^=B20;	tmp11^=B21;	tmp12^=B22;	tmp13^=B23;
        // Calculate Feed B and write it to the register
		B20=tmp10^K110^K20;	B21=tmp11^K111^K21;	B22=tmp12^K112^K22;	B23=tmp13^K113^K23;
		// Write Feed K to the register
		K20=tmp10;	K21=tmp11;	K22=tmp12;	K23=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        // Calculate Feed K
		tmp10^=B30;	tmp11^=B31;	tmp12^=B32;	tmp13^=B33;
        // Calculate Feed B and write it to the register
		B30=tmp10^K00^K30;	B31=tmp11^K01^K31;	B32=tmp12^K02^K32;	B33=tmp13^K03^K33;
        // Write Feed K to the register
		K30=tmp10;	K31=tmp11;	K32=tmp12;	K33=tmp13;

		//LOAD #6*t+3 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^(i>>32);   B23 ^=P3^i;  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^(i>>32);   B33 ^=Q3^i;  
		K70 ^=P2;	K71 ^=P3;	K72 ^=Q0;			K73 ^=Q1;
		K90 ^=Q0;	K91 ^=Q1;	K92 ^=Q2;			K93 ^=Q3;
		K110^=P0;	K111^=P1;	K112^=P2;			K113^=P3;
		K30 ^=Q2;	K31 ^=Q3;	K32 ^=P0;			K33 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        // Calculate Feed K
		tmp10^=B00;	tmp11^=B01;	tmp12^=B02;	tmp13^=B03;
        // Calculate Feed B and write it to the register
		B00=tmp10^K10^K40;	B01=tmp11^K11^K41;	B02=tmp12^K12^K42;	B03=tmp13^K13^K43;
		// Write Feed K to the register
		K40=tmp10;	K41=tmp11;	K42=tmp12;	K43=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        // Calculate Feed K
		tmp10^=B10;	tmp11^=B11;	tmp12^=B12;	tmp13^=B13;
        // Calculate Feed B and write it to the register
		B10=tmp10^K20^K50;	B11=tmp11^K21^K51;	B12=tmp12^K22^K52;	B13=tmp13^K23^K53;
        // Write Feed K to the register
		K50=tmp10;	K51=tmp11;	K52=tmp12;	K53=tmp13;

		//LOAD #6*t+4 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^(i>>32);   B03 ^=P3^i;  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^(i>>32);   B13 ^=Q3^i;  
		K90 ^=P2;	K91 ^=P3;	K92 ^=Q0;			K93 ^=Q1;
		K110^=Q0;	K111^=Q1;	K112^=Q2;			K113^=Q3;
		K10 ^=P0;	K11 ^=P1;	K12 ^=P2;			K13 ^=P3;
		K50 ^=Q2;	K51 ^=Q3;	K52 ^=P0;			K53 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        // Calculate Feed K
		tmp10^=B20;	tmp11^=B21;	tmp12^=B22;	tmp13^=B23;
        // Calculate Feed B and write it to the register
		B20=tmp10^K30^K60;	B21=tmp11^K31^K61;	B22=tmp12^K32^K62;	B23=tmp13^K33^K63;
		// Write Feed K to the register
		K60=tmp10;	K61=tmp11;	K62=tmp12;	K63=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        // Calculate Feed K
		tmp10^=B30;	tmp11^=B31;	tmp12^=B32;	tmp13^=B33;
        // Calculate Feed B and write it to the register
		B30=tmp10^K40^K70;	B31=tmp11^K41^K71;	B32=tmp12^K42^K72;	B33=tmp13^K43^K73;
        // Write Feed K to the register
		K70=tmp10;	K71=tmp11;	K72=tmp12;	K73=tmp13;

		//LOAD #6*t+5 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^(i>>32);   B23 ^=P3^i;  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^(i>>32);   B33 ^=Q3^i;  
		K110^=P2;	K111^=P3;	K112^=Q0;			K113^=Q1;
		K10 ^=Q0;	K11 ^=Q1;	K12 ^=Q2;			K13 ^=Q3;
		K30 ^=P0;	K31 ^=P1;	K32 ^=P2;			K33 ^=P3;
		K70 ^=Q2;	K71 ^=Q3;	K72 ^=P0;			K73 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        // Calculate Feed K
		tmp10^=B00;	tmp11^=B01;	tmp12^=B02;	tmp13^=B03;
        // Calculate Feed B and write it to the register
		B00=tmp10^K50^K80;	B01=tmp11^K51^K81;	B02=tmp12^K52^K82;	B03=tmp13^K53^K83;
		// Write Feed K to the register
		K80=tmp10;	K81=tmp11;	K82=tmp12;	K83=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        // Calculate Feed K
		tmp10^=B10;	tmp11^=B11;	tmp12^=B12;	tmp13^=B13;
        // Calculate Feed B and write it to the register
		B10=tmp10^K60^K90;	B11=tmp11^K61^K91;	B12=tmp12^K62^K92;	B13=tmp13^K63^K93;
        // Write Feed K to the register
		K90=tmp10;	K91=tmp11;	K92=tmp12;	K93=tmp13;

		//LOAD #6*t+6 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^(i>>32);   B03 ^=P3^i;  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^(i>>32);   B13 ^=Q3^i;  
		K10 ^=P2;	K11 ^=P3;	K12 ^=Q0;			K13 ^=Q1;
		K30 ^=Q0;	K31 ^=Q1;	K32 ^=Q2;			K33 ^=Q3;
		K50 ^=P0;	K51 ^=P1;	K52 ^=P2;			K53 ^=P3;
		K90 ^=Q2;	K91 ^=Q3;	K92 ^=P0;			K93 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        // Calculate Feed K
		tmp10^=B20;	tmp11^=B21;	tmp12^=B22;	tmp13^=B23;
        // Calculate Feed B and write it to the register
		B20=tmp10^K70^K100;	B21=tmp11^K71^K101;	B22=tmp12^K72^K102;	B23=tmp13^K73^K103;
		// Write Feed K to the register
		K100=tmp10;	K101=tmp11;	K102=tmp12;	K103=tmp13;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        // Calculate Feed K
		tmp10^=B30;	tmp11^=B31;	tmp12^=B32;	tmp13^=B33;
        // Calculate Feed B and write it to the register
		B30=tmp10^K80^K110;	B31=tmp11^K81^K111;	B32=tmp12^K82^K112;	B33=tmp13^K83^K113;
        // Write Feed K to the register
		K110=tmp10;	K111=tmp11;	K112=tmp12;	K113=tmp13;
	}

	// Write calculate register values to the state register
	state->B[0][0] = B00;	state->B[0][1] = B01;	state->B[0][2] = B02;	state->B[0][3] = B03;	
	state->B[1][0] = B10;	state->B[1][1] = B11;	state->B[1][2] = B12;	state->B[1][3] = B13;	
	state->B[2][0] = B20;	state->B[2][1] = B21;	state->B[2][2] = B22;	state->B[2][3] = B23;	
	state->B[3][0] = B30;	state->B[3][1] = B31;	state->B[3][2] = B32;	state->B[3][3] = B33;	
	state->K[0][0] = K00;	state->K[0][1] = K01;	state->K[0][2] = K02;	state->K[0][3] = K03;	
	state->K[1][0] = K10;	state->K[1][1] = K11;	state->K[1][2] = K12;	state->K[1][3] = K13;	
	state->K[2][0] = K20;	state->K[2][1] = K21;	state->K[2][2] = K22;	state->K[2][3] = K23;	
	state->K[3][0] = K30;	state->K[3][1] = K31;	state->K[3][2] = K32;	state->K[3][3] = K33;	
	state->K[4][0] = K40;	state->K[4][1] = K41;	state->K[4][2] = K42;	state->K[4][3] = K43;	
	state->K[5][0] = K50;	state->K[5][1] = K51;	state->K[5][2] = K52;	state->K[5][3] = K53;	
	state->K[6][0] = K60;	state->K[6][1] = K61;	state->K[6][2] = K62;	state->K[6][3] = K63;	
	state->K[7][0] = K70;	state->K[7][1] = K71;	state->K[7][2] = K72;	state->K[7][3] = K73;	
	state->K[8][0] = K80;	state->K[8][1] = K81;	state->K[8][2] = K82;	state->K[8][3] = K83;	
	state->K[9][0] = K90;	state->K[9][1] = K91;	state->K[9][2] = K92;	state->K[9][3] = K93;	
	state->K[10][0]= K100;	state->K[10][1]= K101;	state->K[10][2] = K102;	state->K[10][3] = K103;	
	state->K[11][0]= K110;	state->K[11][1]= K111;	state->K[11][2] = K112;	state->K[11][3] = K113;	

	for(i=0;i<((messageblockcount%2)*2);i++){
		tmp10 = state->B[0][0];		tmp11 = state->B[0][1];		
		tmp12 = state->B[0][2];		tmp13 = state->B[0][3];
		for(j=0;j<3;j++){
			state->B[j][0] = state->B[j+1][0];		state->B[j][1] = state->B[j+1][1];		
			state->B[j][2] = state->B[j+1][2];		state->B[j][3] = state->B[j+1][3];
		}
		state->B[3][0] = tmp10;		state->B[3][1] = tmp11;		
		state->B[3][2] = tmp12;		state->B[3][3] = tmp13;
	}
	for(i=0;i<((messageblockcount%6)*2);i++){	
		tmp10 = state->K[0][0];		tmp11 = state->K[0][1];		
		tmp12 = state->K[0][2];		tmp13 = state->K[0][3];
		for(j=0;j<11;j++){
			state->K[j][0] = state->K[j+1][0];		state->K[j][1] = state->K[j+1][1];		
			state->K[j][2] = state->K[j+1][2];		state->K[j][3] = state->K[j+1][3];
		}
		state->K[11][0] = tmp10;		state->K[11][1] = tmp11;		
		state->K[11][2] = tmp12;		state->K[11][3] = tmp13;
	}

	return SUCCESS;
}


int Shamata::FinalUpdate384_512 (hashState *state, const BitSequence *data)
{ 
	unsigned long long i,messageblockcount=32; 
	unsigned int j, tmp10, tmp11, tmp12, tmp13, tmp20, tmp21, tmp22, tmp23, Q0, Q1, Q2, Q3, P0, P1, P2, P3,
		               B00 = state->B[0][0],	B01 = state->B[0][1],	B02 = state->B[0][2],	B03 = state->B[0][3], 
					   B10 = state->B[1][0],	B11 = state->B[1][1],	B12 = state->B[1][2],	B13 = state->B[1][3], 
					   B20 = state->B[2][0],	B21 = state->B[2][1],	B22 = state->B[2][2],	B23 = state->B[2][3], 
					   B30 = state->B[3][0],	B31 = state->B[3][1],	B32 = state->B[3][2],	B33 = state->B[3][3], 
				       K00 = state->K[0][0],	K01 = state->K[0][1],	K02 = state->K[0][2],	K03 = state->K[0][3], 
					   K10 = state->K[1][0],	K11 = state->K[1][1],	K12 = state->K[1][2],	K13 = state->K[1][3], 
					   K20 = state->K[2][0],	K21 = state->K[2][1],	K22 = state->K[2][2],	K23 = state->K[2][3], 
					   K30 = state->K[3][0],	K31 = state->K[3][1],	K32 = state->K[3][2],	K33 = state->K[3][3], 
					   K40 = state->K[4][0],	K41 = state->K[4][1],	K42 = state->K[4][2],	K43 = state->K[4][3], 
					   K50 = state->K[5][0],	K51 = state->K[5][1],	K52 = state->K[5][2],	K53 = state->K[5][3], 
					   K60 = state->K[6][0],	K61 = state->K[6][1],	K62 = state->K[6][2],	K63 = state->K[6][3], 
					   K70 = state->K[7][0],	K71 = state->K[7][1],	K72 = state->K[7][2],	K73 = state->K[7][3],
					   K80 = state->K[8][0],	K81 = state->K[8][1],	K82 = state->K[8][2],	K83 = state->K[8][3], 
					   K90 = state->K[9][0],	K91 = state->K[9][1],	K92 = state->K[9][2],	K93 = state->K[9][3], 
					   K100 = state->K[10][0],	K101 = state->K[10][1], K102 = state->K[10][2], K103 = state->K[10][3], 
					   K110 = state->K[11][0],	K111 = state->K[11][1], K112 = state->K[11][2], K113 = state->K[11][3];
	
	i = 1;
	
	Q0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[4]]  ^ SHAMATA_MLH[data[8]]   ^ SHAMATA_MLL[data[12]]; 
	Q1 = SHAMATA_MHH[data[1]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[9]]   ^ SHAMATA_MLL[data[13]];
	Q2 = SHAMATA_MHH[data[2]]  ^ SHAMATA_MHL[data[6]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[14]];
	Q3 = SHAMATA_MHH[data[3]]  ^ SHAMATA_MHL[data[7]]  ^ SHAMATA_MLH[data[11]]  ^ SHAMATA_MLL[data[15]];
	P0 = SHAMATA_MHH[data[0]]  ^ SHAMATA_MHL[data[1]]  ^ SHAMATA_MLH[data[2]]   ^ SHAMATA_MLL[data[3]];
	P1 = SHAMATA_MHH[data[4]]  ^ SHAMATA_MHL[data[5]]  ^ SHAMATA_MLH[data[6]]   ^ SHAMATA_MLL[data[7]];
	P2 = SHAMATA_MHH[data[8]]  ^ SHAMATA_MHL[data[9]]  ^ SHAMATA_MLH[data[10]]  ^ SHAMATA_MLL[data[11]];
	P3 = SHAMATA_MHH[data[12]] ^ SHAMATA_MHL[data[13]] ^ SHAMATA_MLH[data[14]]  ^ SHAMATA_MLL[data[15]];

	while(1){
		//LOAD #6*t+1 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^(i>>32);   B23 ^=P3^i;  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^(i>>32);   B33 ^=Q3^i;  
		K30 ^=P2;	K31 ^=P3;	K32 ^=Q0;			K33 ^=Q1;
		K50 ^=Q0;	K51 ^=Q1;	K52 ^=Q2;			K53 ^=Q3;
		K70 ^=P0;	K71 ^=P1;	K72 ^=P2;			K73 ^=P3;
		K110^=Q2;	K111^=Q3;	K112^=P0;			K113^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
		
		tmp20 = B00^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B01^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B02^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B03^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B00=tmp20^K90^K00;	B01=tmp21^K91^K01;	B02=tmp22^K92^K02;	B03=tmp23^K93^K03;
		// Write Feed K to the register
		K00=tmp20;	K01=tmp21;	K02=tmp22;	K03=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        
		tmp20 = B10^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B11^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B12^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B13^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
	
        // Calculate Feed B and write it to the register
		B10=tmp20^K100^K10;	B11=tmp21^K101^K11;	B12=tmp22^K102^K12;	B13=tmp23^K103^K13;
        // Write Feed K to the register
		K10=tmp20;	K11=tmp21;	K12=tmp22;	K13=tmp23;

		//LOAD #6*t+2 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^(i>>32);   B03 ^=P3^i;  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^(i>>32);   B13 ^=Q3^i;  
		K50 ^=P2;	K51 ^=P3;	K52 ^=Q0;			K53 ^=Q1;
		K70 ^=Q0;	K71 ^=Q1;	K72 ^=Q2;			K73 ^=Q3;
		K90 ^=P0;	K91 ^=P1;	K92 ^=P2;			K93 ^=P3;
		K10 ^=Q2;	K11 ^=Q3;	K12 ^=P0;			K13 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        
		tmp20 = B20^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B21^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B22^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B23^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B20=tmp20^K110^K20;	B21=tmp21^K111^K21;	B22=tmp22^K112^K22;	B23=tmp23^K113^K23;
		// Write Feed K to the register
		K20=tmp20;	K21=tmp21;	K22=tmp22;	K23=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        
		tmp20 = B30^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B31^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B32^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B33^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
        // Calculate Feed B and write it to the register
		B30=tmp20^K00^K30;	B31=tmp21^K01^K31;	B32=tmp22^K02^K32;	B33=tmp23^K03^K33;
        // Write Feed K to the register
		K30=tmp20;	K31=tmp21;	K32=tmp22;	K33=tmp23;

		//LOAD #6*t+3 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^(i>>32);   B23 ^=P3^i;  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^(i>>32);   B33 ^=Q3^i;  
		K70 ^=P2;	K71 ^=P3;	K72 ^=Q0;			K73 ^=Q1;
		K90 ^=Q0;	K91 ^=Q1;	K92 ^=Q2;			K93 ^=Q3;
		K110^=P0;	K111^=P1;	K112^=P2;			K113^=P3;
		K30 ^=Q2;	K31 ^=Q3;	K32 ^=P0;			K33 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        
		tmp20 = B00^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B01^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B02^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B03^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
        // Calculate Feed B and write it to the register
		B00=tmp20^K10^K40;	B01=tmp21^K11^K41;	B02=tmp22^K12^K42;	B03=tmp23^K13^K43;
		// Write Feed K to the register
		K40=tmp20;	K41=tmp21;	K42=tmp22;	K43=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        
		tmp20 = B10^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B11^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B12^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B13^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
        // Calculate Feed B and write it to the register
		B10=tmp20^K20^K50;	B11=tmp21^K21^K51;	B12=tmp22^K22^K52;	B13=tmp23^K23^K53;
        // Write Feed K to the register
		K50=tmp20;	K51=tmp21;	K52=tmp22;	K53=tmp23;

		//LOAD #6*t+4 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^(i>>32);   B03 ^=P3^i;  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^(i>>32);   B13 ^=Q3^i;  
		K90 ^=P2;	K91 ^=P3;	K92 ^=Q0;			K93 ^=Q1;
		K110^=Q0;	K111^=Q1;	K112^=Q2;			K113^=Q3;
		K10 ^=P0;	K11 ^=P1;	K12 ^=P2;			K13 ^=P3;
		K50 ^=Q2;	K51 ^=Q3;	K52 ^=P0;			K53 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        
		tmp20 = B20^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B21^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B22^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B23^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B20=tmp20^K30^K60;	B21=tmp21^K31^K61;	B22=tmp22^K32^K62;	B23=tmp23^K33^K63;
		// Write Feed K to the register
		K60=tmp20;	K61=tmp21;	K62=tmp22;	K63=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        
		tmp20 = B30^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B31^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B32^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B33^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
        // Calculate Feed B and write it to the register
		B30=tmp20^K40^K70;	B31=tmp21^K41^K71;	B32=tmp22^K42^K72;	B33=tmp23^K43^K73;
        // Write Feed K to the register
		K70=tmp20;	K71=tmp21;	K72=tmp22;	K73=tmp23;

		//LOAD #6*t+5 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
		
        // Load
		B20 ^=P0;	B21 ^=P1;	B22 ^=P2^(i>>32);   B23 ^=P3^i;  
		B30 ^=Q0;	B31 ^=Q1;	B32 ^=Q2^(i>>32);   B33 ^=Q3^i;  
		K110^=P2;	K111^=P3;	K112^=Q0;			K113^=Q1;
		K10 ^=Q0;	K11 ^=Q1;	K12 ^=Q2;			K13 ^=Q3;
		K30 ^=P0;	K31 ^=P1;	K32 ^=P2;			K33 ^=P3;
		K70 ^=Q2;	K71 ^=Q3;	K72 ^=P0;			K73 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B20>>24)&0xff] ^ SHAMATA_Shl[(B21>>16)&0xff] ^ SHAMATA_Slh[(B22>>8)&0xff] ^ SHAMATA_Sll[(B23)&0xff];
        tmp11 = SHAMATA_Shh[(B21>>24)&0xff] ^ SHAMATA_Shl[(B22>>16)&0xff] ^ SHAMATA_Slh[(B23>>8)&0xff] ^ SHAMATA_Sll[(B20)&0xff];
 		tmp12 = SHAMATA_Shh[(B22>>24)&0xff] ^ SHAMATA_Shl[(B23>>16)&0xff] ^ SHAMATA_Slh[(B20>>8)&0xff] ^ SHAMATA_Sll[(B21)&0xff];
		tmp13 = SHAMATA_Shh[(B23>>24)&0xff] ^ SHAMATA_Shl[(B20>>16)&0xff] ^ SHAMATA_Slh[(B21>>8)&0xff] ^ SHAMATA_Sll[(B22)&0xff];
        
		tmp20 = B00^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B01^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B02^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B03^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B00=tmp20^K50^K80;	B01=tmp21^K51^K81;	B02=tmp22^K52^K82;	B03=tmp23^K53^K83;
		// Write Feed K to the register
		K80=tmp20;	K81=tmp21;	K82=tmp22;	K83=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B30>>24)&0xff] ^ SHAMATA_Shl[(B31>>16)&0xff] ^ SHAMATA_Slh[(B32>>8)&0xff] ^ SHAMATA_Sll[(B33)&0xff];
        tmp11 = SHAMATA_Shh[(B31>>24)&0xff] ^ SHAMATA_Shl[(B32>>16)&0xff] ^ SHAMATA_Slh[(B33>>8)&0xff] ^ SHAMATA_Sll[(B30)&0xff];
 		tmp12 = SHAMATA_Shh[(B32>>24)&0xff] ^ SHAMATA_Shl[(B33>>16)&0xff] ^ SHAMATA_Slh[(B30>>8)&0xff] ^ SHAMATA_Sll[(B31)&0xff];
		tmp13 = SHAMATA_Shh[(B33>>24)&0xff] ^ SHAMATA_Shl[(B30>>16)&0xff] ^ SHAMATA_Slh[(B31>>8)&0xff] ^ SHAMATA_Sll[(B32)&0xff];
        
		tmp20 = B10^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B11^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B12^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B13^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
        // Calculate Feed B and write it to the register
		B10=tmp20^K60^K90;	B11=tmp21^K61^K91;	B12=tmp22^K62^K92;	B13=tmp23^K63^K93;
        // Write Feed K to the register
		K90=tmp20;	K91=tmp21;	K92=tmp22;	K93=tmp23;

		//LOAD #6*t+6 *************************************************************************
		if(i>messageblockcount) // if there are no block left exit from the while loop
			break;
        // Load
		B00 ^=P0;	B01 ^=P1;	B02 ^=P2^(i>>32);   B03 ^=P3^i;  
		B10 ^=Q0;	B11 ^=Q1;	B12 ^=Q2^(i>>32);   B13 ^=Q3^i;  
		K10 ^=P2;	K11 ^=P3;	K12 ^=Q0;			K13 ^=Q1;
		K30 ^=Q0;	K31 ^=Q1;	K32 ^=Q2;			K33 ^=Q3;
		K50 ^=P0;	K51 ^=P1;	K52 ^=P2;			K53 ^=P3;
		K90 ^=Q2;	K91 ^=Q3;	K92 ^=P0;			K93 ^=P1;
		// Point next block
		i++;
		// First Clock -------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B00>>24)&0xff] ^ SHAMATA_Shl[(B01>>16)&0xff] ^ SHAMATA_Slh[(B02>>8)&0xff] ^ SHAMATA_Sll[(B03)&0xff];
        tmp11 = SHAMATA_Shh[(B01>>24)&0xff] ^ SHAMATA_Shl[(B02>>16)&0xff] ^ SHAMATA_Slh[(B03>>8)&0xff] ^ SHAMATA_Sll[(B00)&0xff];
 		tmp12 = SHAMATA_Shh[(B02>>24)&0xff] ^ SHAMATA_Shl[(B03>>16)&0xff] ^ SHAMATA_Slh[(B00>>8)&0xff] ^ SHAMATA_Sll[(B01)&0xff];
		tmp13 = SHAMATA_Shh[(B03>>24)&0xff] ^ SHAMATA_Shl[(B00>>16)&0xff] ^ SHAMATA_Slh[(B01>>8)&0xff] ^ SHAMATA_Sll[(B02)&0xff];
        
		tmp20 = B20^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B21^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B22^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B23^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];
		
		// Calculate Feed B and write it to the register
		B20=tmp20^K70^K100;	B21=tmp21^K71^K101;	B22=tmp22^K72^K102;	B23=tmp23^K73^K103;
		// Write Feed K to the register
		K100=tmp20;	K101=tmp21;	K102=tmp22;	K103=tmp23;
		// Second Clock ------------------------------------------------------------------------
		// Run ARF
		tmp10 = SHAMATA_Shh[(B10>>24)&0xff] ^ SHAMATA_Shl[(B11>>16)&0xff] ^ SHAMATA_Slh[(B12>>8)&0xff] ^ SHAMATA_Sll[(B13)&0xff];
        tmp11 = SHAMATA_Shh[(B11>>24)&0xff] ^ SHAMATA_Shl[(B12>>16)&0xff] ^ SHAMATA_Slh[(B13>>8)&0xff] ^ SHAMATA_Sll[(B10)&0xff];
 		tmp12 = SHAMATA_Shh[(B12>>24)&0xff] ^ SHAMATA_Shl[(B13>>16)&0xff] ^ SHAMATA_Slh[(B10>>8)&0xff] ^ SHAMATA_Sll[(B11)&0xff];
		tmp13 = SHAMATA_Shh[(B13>>24)&0xff] ^ SHAMATA_Shl[(B10>>16)&0xff] ^ SHAMATA_Slh[(B11>>8)&0xff] ^ SHAMATA_Sll[(B12)&0xff];
        
		tmp20 = B30^SHAMATA_Shh[(tmp10>>24)&0xff] ^ SHAMATA_Shl[(tmp11>>16)&0xff] ^ SHAMATA_Slh[(tmp12>>8)&0xff] ^ SHAMATA_Sll[(tmp13)&0xff];
        tmp21 = B31^SHAMATA_Shh[(tmp11>>24)&0xff] ^ SHAMATA_Shl[(tmp12>>16)&0xff] ^ SHAMATA_Slh[(tmp13>>8)&0xff] ^ SHAMATA_Sll[(tmp10)&0xff];
 		tmp22 = B32^SHAMATA_Shh[(tmp12>>24)&0xff] ^ SHAMATA_Shl[(tmp13>>16)&0xff] ^ SHAMATA_Slh[(tmp10>>8)&0xff] ^ SHAMATA_Sll[(tmp11)&0xff];
		tmp23 = B33^SHAMATA_Shh[(tmp13>>24)&0xff] ^ SHAMATA_Shl[(tmp10>>16)&0xff] ^ SHAMATA_Slh[(tmp11>>8)&0xff] ^ SHAMATA_Sll[(tmp12)&0xff];

        // Calculate Feed B and write it to the register
		B30=tmp20^K80^K110;	B31=tmp21^K81^K111;	B32=tmp22^K82^K112;	B33=tmp23^K83^K113;
        // Write Feed K to the register
		K110=tmp20;	K111=tmp21;	K112=tmp22;	K113=tmp23;
	}

	// Write calculate register values to the state register
	state->B[0][0] = B00;	state->B[0][1] = B01;	state->B[0][2] = B02;	state->B[0][3] = B03;	
	state->B[1][0] = B10;	state->B[1][1] = B11;	state->B[1][2] = B12;	state->B[1][3] = B13;	
	state->B[2][0] = B20;	state->B[2][1] = B21;	state->B[2][2] = B22;	state->B[2][3] = B23;	
	state->B[3][0] = B30;	state->B[3][1] = B31;	state->B[3][2] = B32;	state->B[3][3] = B33;	
	state->K[0][0] = K00;	state->K[0][1] = K01;	state->K[0][2] = K02;	state->K[0][3] = K03;	
	state->K[1][0] = K10;	state->K[1][1] = K11;	state->K[1][2] = K12;	state->K[1][3] = K13;	
	state->K[2][0] = K20;	state->K[2][1] = K21;	state->K[2][2] = K22;	state->K[2][3] = K23;	
	state->K[3][0] = K30;	state->K[3][1] = K31;	state->K[3][2] = K32;	state->K[3][3] = K33;	
	state->K[4][0] = K40;	state->K[4][1] = K41;	state->K[4][2] = K42;	state->K[4][3] = K43;	
	state->K[5][0] = K50;	state->K[5][1] = K51;	state->K[5][2] = K52;	state->K[5][3] = K53;	
	state->K[6][0] = K60;	state->K[6][1] = K61;	state->K[6][2] = K62;	state->K[6][3] = K63;	
	state->K[7][0] = K70;	state->K[7][1] = K71;	state->K[7][2] = K72;	state->K[7][3] = K73;	
	state->K[8][0] = K80;	state->K[8][1] = K81;	state->K[8][2] = K82;	state->K[8][3] = K83;	
	state->K[9][0] = K90;	state->K[9][1] = K91;	state->K[9][2] = K92;	state->K[9][3] = K93;	
	state->K[10][0]= K100;	state->K[10][1]= K101;	state->K[10][2] = K102;	state->K[10][3] = K103;	
	state->K[11][0]= K110;	state->K[11][1]= K111;	state->K[11][2] = K112;	state->K[11][3] = K113;	

	for(i=0;i<((messageblockcount%2)*2);i++){
		tmp10 = state->B[0][0];		tmp11 = state->B[0][1];		
		tmp12 = state->B[0][2];		tmp13 = state->B[0][3];
		for(j=0;j<3;j++){
			state->B[j][0] = state->B[j+1][0];		state->B[j][1] = state->B[j+1][1];		
			state->B[j][2] = state->B[j+1][2];		state->B[j][3] = state->B[j+1][3];
		}
		state->B[3][0] = tmp10;		state->B[3][1] = tmp11;		
		state->B[3][2] = tmp12;		state->B[3][3] = tmp13;
	}
	for(i=0;i<((messageblockcount%6)*2);i++){	
		tmp10 = state->K[0][0];		tmp11 = state->K[0][1];		
		tmp12 = state->K[0][2];		tmp13 = state->K[0][3];
		for(j=0;j<11;j++){
			state->K[j][0] = state->K[j+1][0];		state->K[j][1] = state->K[j+1][1];		
			state->K[j][2] = state->K[j+1][2];		state->K[j][3] = state->K[j+1][3];
		}
		state->K[11][0] = tmp10;		state->K[11][1] = tmp11;		
		state->K[11][2] = tmp12;		state->K[11][3] = tmp13;
	}

	return SUCCESS;
}


int Shamata::ProduceOutput(hashState *state, BitSequence *hashval)
{
	// The following is looping parameter
	unsigned int i;

	for(i=0;i<state->hashbitlen/8;i++)
		hashval[(state->hashbitlen/8)-1-i]=(state->B[3-i/16][3-(i/4)%4]>>(8*i)%32)&0xff;

	return SUCCESS;
}