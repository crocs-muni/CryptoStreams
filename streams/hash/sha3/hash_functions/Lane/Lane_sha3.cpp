#include "Lane_sha3.h"

namespace sha3 {

#define LANE_T8(x) ((x) & 0xff)
#define LANE_B0(x) (LANE_T8((x)      ))
#define LANE_B1(x) (LANE_T8((x) >>  8))
#define LANE_B2(x) (LANE_T8((x) >> 16))
#define LANE_B3(x) (LANE_T8((x) >> 24))
#define LANE_MSB32(x) ((unsigned int)((((unsigned long long)(x))>>32) & 0xffffffff))
#define LANE_LSB32(x) ((unsigned int)((((unsigned long long)(x))    ) & 0xffffffff))
#ifdef LANE_BIG_ENDIAN
#define LANE_U8TO32_BIG(c)  (((unsigned int*)(c))[0])
#define LANE_U32TO8_BIG(c, v) ((unsigned int*)(c))[0]=v
#else
#define LANE_U8TO32_BIG(c)  (((unsigned int)LANE_T8(*((unsigned char*)(c))) << 24) | \
			((unsigned int)LANE_T8(*(((unsigned char*)(c)) + 1)) << 16) | \
			((unsigned int)LANE_T8(*(((unsigned char*)(c)) + 2)) << 8) | \
			((unsigned int)LANE_T8(*(((unsigned char*)(c)) + 3))))
#define LANE_U32TO8_BIG(c, v)    do { \
		unsigned int tmp_portable_h_x = (v); \
		unsigned char *tmp_portable_h_d = (c); \
		tmp_portable_h_d[0] = LANE_T8(tmp_portable_h_x >> 24); \
		tmp_portable_h_d[1] = LANE_T8(tmp_portable_h_x >> 16); \
		tmp_portable_h_d[2] = LANE_T8(tmp_portable_h_x >> 8); \
		tmp_portable_h_d[3] = LANE_T8(tmp_portable_h_x); \
	} while (0)
#endif /* LANE_BIG_ENDIAN */

static const unsigned int LANE_T0[256] = {
    0xc66363a5U, 0xf87c7c84U, 0xee777799U, 0xf67b7b8dU,
    0xfff2f20dU, 0xd66b6bbdU, 0xde6f6fb1U, 0x91c5c554U,
    0x60303050U, 0x02010103U, 0xce6767a9U, 0x562b2b7dU,
    0xe7fefe19U, 0xb5d7d762U, 0x4dababe6U, 0xec76769aU,
    0x8fcaca45U, 0x1f82829dU, 0x89c9c940U, 0xfa7d7d87U,
    0xeffafa15U, 0xb25959ebU, 0x8e4747c9U, 0xfbf0f00bU,
    0x41adadecU, 0xb3d4d467U, 0x5fa2a2fdU, 0x45afafeaU,
    0x239c9cbfU, 0x53a4a4f7U, 0xe4727296U, 0x9bc0c05bU,
    0x75b7b7c2U, 0xe1fdfd1cU, 0x3d9393aeU, 0x4c26266aU,
    0x6c36365aU, 0x7e3f3f41U, 0xf5f7f702U, 0x83cccc4fU,
    0x6834345cU, 0x51a5a5f4U, 0xd1e5e534U, 0xf9f1f108U,
    0xe2717193U, 0xabd8d873U, 0x62313153U, 0x2a15153fU,
    0x0804040cU, 0x95c7c752U, 0x46232365U, 0x9dc3c35eU,
    0x30181828U, 0x379696a1U, 0x0a05050fU, 0x2f9a9ab5U,
    0x0e070709U, 0x24121236U, 0x1b80809bU, 0xdfe2e23dU,
    0xcdebeb26U, 0x4e272769U, 0x7fb2b2cdU, 0xea75759fU,
    0x1209091bU, 0x1d83839eU, 0x582c2c74U, 0x341a1a2eU,
    0x361b1b2dU, 0xdc6e6eb2U, 0xb45a5aeeU, 0x5ba0a0fbU,
    0xa45252f6U, 0x763b3b4dU, 0xb7d6d661U, 0x7db3b3ceU,
    0x5229297bU, 0xdde3e33eU, 0x5e2f2f71U, 0x13848497U,
    0xa65353f5U, 0xb9d1d168U, 0x00000000U, 0xc1eded2cU,
    0x40202060U, 0xe3fcfc1fU, 0x79b1b1c8U, 0xb65b5bedU,
    0xd46a6abeU, 0x8dcbcb46U, 0x67bebed9U, 0x7239394bU,
    0x944a4adeU, 0x984c4cd4U, 0xb05858e8U, 0x85cfcf4aU,
    0xbbd0d06bU, 0xc5efef2aU, 0x4faaaae5U, 0xedfbfb16U,
    0x864343c5U, 0x9a4d4dd7U, 0x66333355U, 0x11858594U,
    0x8a4545cfU, 0xe9f9f910U, 0x04020206U, 0xfe7f7f81U,
    0xa05050f0U, 0x783c3c44U, 0x259f9fbaU, 0x4ba8a8e3U,
    0xa25151f3U, 0x5da3a3feU, 0x804040c0U, 0x058f8f8aU,
    0x3f9292adU, 0x219d9dbcU, 0x70383848U, 0xf1f5f504U,
    0x63bcbcdfU, 0x77b6b6c1U, 0xafdada75U, 0x42212163U,
    0x20101030U, 0xe5ffff1aU, 0xfdf3f30eU, 0xbfd2d26dU,
    0x81cdcd4cU, 0x180c0c14U, 0x26131335U, 0xc3ecec2fU,
    0xbe5f5fe1U, 0x359797a2U, 0x884444ccU, 0x2e171739U,
    0x93c4c457U, 0x55a7a7f2U, 0xfc7e7e82U, 0x7a3d3d47U,
    0xc86464acU, 0xba5d5de7U, 0x3219192bU, 0xe6737395U,
    0xc06060a0U, 0x19818198U, 0x9e4f4fd1U, 0xa3dcdc7fU,
    0x44222266U, 0x542a2a7eU, 0x3b9090abU, 0x0b888883U,
    0x8c4646caU, 0xc7eeee29U, 0x6bb8b8d3U, 0x2814143cU,
    0xa7dede79U, 0xbc5e5ee2U, 0x160b0b1dU, 0xaddbdb76U,
    0xdbe0e03bU, 0x64323256U, 0x743a3a4eU, 0x140a0a1eU,
    0x924949dbU, 0x0c06060aU, 0x4824246cU, 0xb85c5ce4U,
    0x9fc2c25dU, 0xbdd3d36eU, 0x43acacefU, 0xc46262a6U,
    0x399191a8U, 0x319595a4U, 0xd3e4e437U, 0xf279798bU,
    0xd5e7e732U, 0x8bc8c843U, 0x6e373759U, 0xda6d6db7U,
    0x018d8d8cU, 0xb1d5d564U, 0x9c4e4ed2U, 0x49a9a9e0U,
    0xd86c6cb4U, 0xac5656faU, 0xf3f4f407U, 0xcfeaea25U,
    0xca6565afU, 0xf47a7a8eU, 0x47aeaee9U, 0x10080818U,
    0x6fbabad5U, 0xf0787888U, 0x4a25256fU, 0x5c2e2e72U,
    0x381c1c24U, 0x57a6a6f1U, 0x73b4b4c7U, 0x97c6c651U,
    0xcbe8e823U, 0xa1dddd7cU, 0xe874749cU, 0x3e1f1f21U,
    0x964b4bddU, 0x61bdbddcU, 0x0d8b8b86U, 0x0f8a8a85U,
    0xe0707090U, 0x7c3e3e42U, 0x71b5b5c4U, 0xcc6666aaU,
    0x904848d8U, 0x06030305U, 0xf7f6f601U, 0x1c0e0e12U,
    0xc26161a3U, 0x6a35355fU, 0xae5757f9U, 0x69b9b9d0U,
    0x17868691U, 0x99c1c158U, 0x3a1d1d27U, 0x279e9eb9U,
    0xd9e1e138U, 0xebf8f813U, 0x2b9898b3U, 0x22111133U,
    0xd26969bbU, 0xa9d9d970U, 0x078e8e89U, 0x339494a7U,
    0x2d9b9bb6U, 0x3c1e1e22U, 0x15878792U, 0xc9e9e920U,
    0x87cece49U, 0xaa5555ffU, 0x50282878U, 0xa5dfdf7aU,
    0x038c8c8fU, 0x59a1a1f8U, 0x09898980U, 0x1a0d0d17U,
    0x65bfbfdaU, 0xd7e6e631U, 0x844242c6U, 0xd06868b8U,
    0x824141c3U, 0x299999b0U, 0x5a2d2d77U, 0x1e0f0f11U,
    0x7bb0b0cbU, 0xa85454fcU, 0x6dbbbbd6U, 0x2c16163aU,
};
static const unsigned int LANE_T1[256] = {
    0xa5c66363U, 0x84f87c7cU, 0x99ee7777U, 0x8df67b7bU,
    0x0dfff2f2U, 0xbdd66b6bU, 0xb1de6f6fU, 0x5491c5c5U,
    0x50603030U, 0x03020101U, 0xa9ce6767U, 0x7d562b2bU,
    0x19e7fefeU, 0x62b5d7d7U, 0xe64dababU, 0x9aec7676U,
    0x458fcacaU, 0x9d1f8282U, 0x4089c9c9U, 0x87fa7d7dU,
    0x15effafaU, 0xebb25959U, 0xc98e4747U, 0x0bfbf0f0U,
    0xec41adadU, 0x67b3d4d4U, 0xfd5fa2a2U, 0xea45afafU,
    0xbf239c9cU, 0xf753a4a4U, 0x96e47272U, 0x5b9bc0c0U,
    0xc275b7b7U, 0x1ce1fdfdU, 0xae3d9393U, 0x6a4c2626U,
    0x5a6c3636U, 0x417e3f3fU, 0x02f5f7f7U, 0x4f83ccccU,
    0x5c683434U, 0xf451a5a5U, 0x34d1e5e5U, 0x08f9f1f1U,
    0x93e27171U, 0x73abd8d8U, 0x53623131U, 0x3f2a1515U,
    0x0c080404U, 0x5295c7c7U, 0x65462323U, 0x5e9dc3c3U,
    0x28301818U, 0xa1379696U, 0x0f0a0505U, 0xb52f9a9aU,
    0x090e0707U, 0x36241212U, 0x9b1b8080U, 0x3ddfe2e2U,
    0x26cdebebU, 0x694e2727U, 0xcd7fb2b2U, 0x9fea7575U,
    0x1b120909U, 0x9e1d8383U, 0x74582c2cU, 0x2e341a1aU,
    0x2d361b1bU, 0xb2dc6e6eU, 0xeeb45a5aU, 0xfb5ba0a0U,
    0xf6a45252U, 0x4d763b3bU, 0x61b7d6d6U, 0xce7db3b3U,
    0x7b522929U, 0x3edde3e3U, 0x715e2f2fU, 0x97138484U,
    0xf5a65353U, 0x68b9d1d1U, 0x00000000U, 0x2cc1ededU,
    0x60402020U, 0x1fe3fcfcU, 0xc879b1b1U, 0xedb65b5bU,
    0xbed46a6aU, 0x468dcbcbU, 0xd967bebeU, 0x4b723939U,
    0xde944a4aU, 0xd4984c4cU, 0xe8b05858U, 0x4a85cfcfU,
    0x6bbbd0d0U, 0x2ac5efefU, 0xe54faaaaU, 0x16edfbfbU,
    0xc5864343U, 0xd79a4d4dU, 0x55663333U, 0x94118585U,
    0xcf8a4545U, 0x10e9f9f9U, 0x06040202U, 0x81fe7f7fU,
    0xf0a05050U, 0x44783c3cU, 0xba259f9fU, 0xe34ba8a8U,
    0xf3a25151U, 0xfe5da3a3U, 0xc0804040U, 0x8a058f8fU,
    0xad3f9292U, 0xbc219d9dU, 0x48703838U, 0x04f1f5f5U,
    0xdf63bcbcU, 0xc177b6b6U, 0x75afdadaU, 0x63422121U,
    0x30201010U, 0x1ae5ffffU, 0x0efdf3f3U, 0x6dbfd2d2U,
    0x4c81cdcdU, 0x14180c0cU, 0x35261313U, 0x2fc3ececU,
    0xe1be5f5fU, 0xa2359797U, 0xcc884444U, 0x392e1717U,
    0x5793c4c4U, 0xf255a7a7U, 0x82fc7e7eU, 0x477a3d3dU,
    0xacc86464U, 0xe7ba5d5dU, 0x2b321919U, 0x95e67373U,
    0xa0c06060U, 0x98198181U, 0xd19e4f4fU, 0x7fa3dcdcU,
    0x66442222U, 0x7e542a2aU, 0xab3b9090U, 0x830b8888U,
    0xca8c4646U, 0x29c7eeeeU, 0xd36bb8b8U, 0x3c281414U,
    0x79a7dedeU, 0xe2bc5e5eU, 0x1d160b0bU, 0x76addbdbU,
    0x3bdbe0e0U, 0x56643232U, 0x4e743a3aU, 0x1e140a0aU,
    0xdb924949U, 0x0a0c0606U, 0x6c482424U, 0xe4b85c5cU,
    0x5d9fc2c2U, 0x6ebdd3d3U, 0xef43acacU, 0xa6c46262U,
    0xa8399191U, 0xa4319595U, 0x37d3e4e4U, 0x8bf27979U,
    0x32d5e7e7U, 0x438bc8c8U, 0x596e3737U, 0xb7da6d6dU,
    0x8c018d8dU, 0x64b1d5d5U, 0xd29c4e4eU, 0xe049a9a9U,
    0xb4d86c6cU, 0xfaac5656U, 0x07f3f4f4U, 0x25cfeaeaU,
    0xafca6565U, 0x8ef47a7aU, 0xe947aeaeU, 0x18100808U,
    0xd56fbabaU, 0x88f07878U, 0x6f4a2525U, 0x725c2e2eU,
    0x24381c1cU, 0xf157a6a6U, 0xc773b4b4U, 0x5197c6c6U,
    0x23cbe8e8U, 0x7ca1ddddU, 0x9ce87474U, 0x213e1f1fU,
    0xdd964b4bU, 0xdc61bdbdU, 0x860d8b8bU, 0x850f8a8aU,
    0x90e07070U, 0x427c3e3eU, 0xc471b5b5U, 0xaacc6666U,
    0xd8904848U, 0x05060303U, 0x01f7f6f6U, 0x121c0e0eU,
    0xa3c26161U, 0x5f6a3535U, 0xf9ae5757U, 0xd069b9b9U,
    0x91178686U, 0x5899c1c1U, 0x273a1d1dU, 0xb9279e9eU,
    0x38d9e1e1U, 0x13ebf8f8U, 0xb32b9898U, 0x33221111U,
    0xbbd26969U, 0x70a9d9d9U, 0x89078e8eU, 0xa7339494U,
    0xb62d9b9bU, 0x223c1e1eU, 0x92158787U, 0x20c9e9e9U,
    0x4987ceceU, 0xffaa5555U, 0x78502828U, 0x7aa5dfdfU,
    0x8f038c8cU, 0xf859a1a1U, 0x80098989U, 0x171a0d0dU,
    0xda65bfbfU, 0x31d7e6e6U, 0xc6844242U, 0xb8d06868U,
    0xc3824141U, 0xb0299999U, 0x775a2d2dU, 0x111e0f0fU,
    0xcb7bb0b0U, 0xfca85454U, 0xd66dbbbbU, 0x3a2c1616U,
};
static const unsigned int LANE_T2[256] = {
    0x63a5c663U, 0x7c84f87cU, 0x7799ee77U, 0x7b8df67bU,
    0xf20dfff2U, 0x6bbdd66bU, 0x6fb1de6fU, 0xc55491c5U,
    0x30506030U, 0x01030201U, 0x67a9ce67U, 0x2b7d562bU,
    0xfe19e7feU, 0xd762b5d7U, 0xabe64dabU, 0x769aec76U,
    0xca458fcaU, 0x829d1f82U, 0xc94089c9U, 0x7d87fa7dU,
    0xfa15effaU, 0x59ebb259U, 0x47c98e47U, 0xf00bfbf0U,
    0xadec41adU, 0xd467b3d4U, 0xa2fd5fa2U, 0xafea45afU,
    0x9cbf239cU, 0xa4f753a4U, 0x7296e472U, 0xc05b9bc0U,
    0xb7c275b7U, 0xfd1ce1fdU, 0x93ae3d93U, 0x266a4c26U,
    0x365a6c36U, 0x3f417e3fU, 0xf702f5f7U, 0xcc4f83ccU,
    0x345c6834U, 0xa5f451a5U, 0xe534d1e5U, 0xf108f9f1U,
    0x7193e271U, 0xd873abd8U, 0x31536231U, 0x153f2a15U,
    0x040c0804U, 0xc75295c7U, 0x23654623U, 0xc35e9dc3U,
    0x18283018U, 0x96a13796U, 0x050f0a05U, 0x9ab52f9aU,
    0x07090e07U, 0x12362412U, 0x809b1b80U, 0xe23ddfe2U,
    0xeb26cdebU, 0x27694e27U, 0xb2cd7fb2U, 0x759fea75U,
    0x091b1209U, 0x839e1d83U, 0x2c74582cU, 0x1a2e341aU,
    0x1b2d361bU, 0x6eb2dc6eU, 0x5aeeb45aU, 0xa0fb5ba0U,
    0x52f6a452U, 0x3b4d763bU, 0xd661b7d6U, 0xb3ce7db3U,
    0x297b5229U, 0xe33edde3U, 0x2f715e2fU, 0x84971384U,
    0x53f5a653U, 0xd168b9d1U, 0x00000000U, 0xed2cc1edU,
    0x20604020U, 0xfc1fe3fcU, 0xb1c879b1U, 0x5bedb65bU,
    0x6abed46aU, 0xcb468dcbU, 0xbed967beU, 0x394b7239U,
    0x4ade944aU, 0x4cd4984cU, 0x58e8b058U, 0xcf4a85cfU,
    0xd06bbbd0U, 0xef2ac5efU, 0xaae54faaU, 0xfb16edfbU,
    0x43c58643U, 0x4dd79a4dU, 0x33556633U, 0x85941185U,
    0x45cf8a45U, 0xf910e9f9U, 0x02060402U, 0x7f81fe7fU,
    0x50f0a050U, 0x3c44783cU, 0x9fba259fU, 0xa8e34ba8U,
    0x51f3a251U, 0xa3fe5da3U, 0x40c08040U, 0x8f8a058fU,
    0x92ad3f92U, 0x9dbc219dU, 0x38487038U, 0xf504f1f5U,
    0xbcdf63bcU, 0xb6c177b6U, 0xda75afdaU, 0x21634221U,
    0x10302010U, 0xff1ae5ffU, 0xf30efdf3U, 0xd26dbfd2U,
    0xcd4c81cdU, 0x0c14180cU, 0x13352613U, 0xec2fc3ecU,
    0x5fe1be5fU, 0x97a23597U, 0x44cc8844U, 0x17392e17U,
    0xc45793c4U, 0xa7f255a7U, 0x7e82fc7eU, 0x3d477a3dU,
    0x64acc864U, 0x5de7ba5dU, 0x192b3219U, 0x7395e673U,
    0x60a0c060U, 0x81981981U, 0x4fd19e4fU, 0xdc7fa3dcU,
    0x22664422U, 0x2a7e542aU, 0x90ab3b90U, 0x88830b88U,
    0x46ca8c46U, 0xee29c7eeU, 0xb8d36bb8U, 0x143c2814U,
    0xde79a7deU, 0x5ee2bc5eU, 0x0b1d160bU, 0xdb76addbU,
    0xe03bdbe0U, 0x32566432U, 0x3a4e743aU, 0x0a1e140aU,
    0x49db9249U, 0x060a0c06U, 0x246c4824U, 0x5ce4b85cU,
    0xc25d9fc2U, 0xd36ebdd3U, 0xacef43acU, 0x62a6c462U,
    0x91a83991U, 0x95a43195U, 0xe437d3e4U, 0x798bf279U,
    0xe732d5e7U, 0xc8438bc8U, 0x37596e37U, 0x6db7da6dU,
    0x8d8c018dU, 0xd564b1d5U, 0x4ed29c4eU, 0xa9e049a9U,
    0x6cb4d86cU, 0x56faac56U, 0xf407f3f4U, 0xea25cfeaU,
    0x65afca65U, 0x7a8ef47aU, 0xaee947aeU, 0x08181008U,
    0xbad56fbaU, 0x7888f078U, 0x256f4a25U, 0x2e725c2eU,
    0x1c24381cU, 0xa6f157a6U, 0xb4c773b4U, 0xc65197c6U,
    0xe823cbe8U, 0xdd7ca1ddU, 0x749ce874U, 0x1f213e1fU,
    0x4bdd964bU, 0xbddc61bdU, 0x8b860d8bU, 0x8a850f8aU,
    0x7090e070U, 0x3e427c3eU, 0xb5c471b5U, 0x66aacc66U,
    0x48d89048U, 0x03050603U, 0xf601f7f6U, 0x0e121c0eU,
    0x61a3c261U, 0x355f6a35U, 0x57f9ae57U, 0xb9d069b9U,
    0x86911786U, 0xc15899c1U, 0x1d273a1dU, 0x9eb9279eU,
    0xe138d9e1U, 0xf813ebf8U, 0x98b32b98U, 0x11332211U,
    0x69bbd269U, 0xd970a9d9U, 0x8e89078eU, 0x94a73394U,
    0x9bb62d9bU, 0x1e223c1eU, 0x87921587U, 0xe920c9e9U,
    0xce4987ceU, 0x55ffaa55U, 0x28785028U, 0xdf7aa5dfU,
    0x8c8f038cU, 0xa1f859a1U, 0x89800989U, 0x0d171a0dU,
    0xbfda65bfU, 0xe631d7e6U, 0x42c68442U, 0x68b8d068U,
    0x41c38241U, 0x99b02999U, 0x2d775a2dU, 0x0f111e0fU,
    0xb0cb7bb0U, 0x54fca854U, 0xbbd66dbbU, 0x163a2c16U,
};
static const unsigned int LANE_T3[256] = {
    0x6363a5c6U, 0x7c7c84f8U, 0x777799eeU, 0x7b7b8df6U,
    0xf2f20dffU, 0x6b6bbdd6U, 0x6f6fb1deU, 0xc5c55491U,
    0x30305060U, 0x01010302U, 0x6767a9ceU, 0x2b2b7d56U,
    0xfefe19e7U, 0xd7d762b5U, 0xababe64dU, 0x76769aecU,
    0xcaca458fU, 0x82829d1fU, 0xc9c94089U, 0x7d7d87faU,
    0xfafa15efU, 0x5959ebb2U, 0x4747c98eU, 0xf0f00bfbU,
    0xadadec41U, 0xd4d467b3U, 0xa2a2fd5fU, 0xafafea45U,
    0x9c9cbf23U, 0xa4a4f753U, 0x727296e4U, 0xc0c05b9bU,
    0xb7b7c275U, 0xfdfd1ce1U, 0x9393ae3dU, 0x26266a4cU,
    0x36365a6cU, 0x3f3f417eU, 0xf7f702f5U, 0xcccc4f83U,
    0x34345c68U, 0xa5a5f451U, 0xe5e534d1U, 0xf1f108f9U,
    0x717193e2U, 0xd8d873abU, 0x31315362U, 0x15153f2aU,
    0x04040c08U, 0xc7c75295U, 0x23236546U, 0xc3c35e9dU,
    0x18182830U, 0x9696a137U, 0x05050f0aU, 0x9a9ab52fU,
    0x0707090eU, 0x12123624U, 0x80809b1bU, 0xe2e23ddfU,
    0xebeb26cdU, 0x2727694eU, 0xb2b2cd7fU, 0x75759feaU,
    0x09091b12U, 0x83839e1dU, 0x2c2c7458U, 0x1a1a2e34U,
    0x1b1b2d36U, 0x6e6eb2dcU, 0x5a5aeeb4U, 0xa0a0fb5bU,
    0x5252f6a4U, 0x3b3b4d76U, 0xd6d661b7U, 0xb3b3ce7dU,
    0x29297b52U, 0xe3e33eddU, 0x2f2f715eU, 0x84849713U,
    0x5353f5a6U, 0xd1d168b9U, 0x00000000U, 0xeded2cc1U,
    0x20206040U, 0xfcfc1fe3U, 0xb1b1c879U, 0x5b5bedb6U,
    0x6a6abed4U, 0xcbcb468dU, 0xbebed967U, 0x39394b72U,
    0x4a4ade94U, 0x4c4cd498U, 0x5858e8b0U, 0xcfcf4a85U,
    0xd0d06bbbU, 0xefef2ac5U, 0xaaaae54fU, 0xfbfb16edU,
    0x4343c586U, 0x4d4dd79aU, 0x33335566U, 0x85859411U,
    0x4545cf8aU, 0xf9f910e9U, 0x02020604U, 0x7f7f81feU,
    0x5050f0a0U, 0x3c3c4478U, 0x9f9fba25U, 0xa8a8e34bU,
    0x5151f3a2U, 0xa3a3fe5dU, 0x4040c080U, 0x8f8f8a05U,
    0x9292ad3fU, 0x9d9dbc21U, 0x38384870U, 0xf5f504f1U,
    0xbcbcdf63U, 0xb6b6c177U, 0xdada75afU, 0x21216342U,
    0x10103020U, 0xffff1ae5U, 0xf3f30efdU, 0xd2d26dbfU,
    0xcdcd4c81U, 0x0c0c1418U, 0x13133526U, 0xecec2fc3U,
    0x5f5fe1beU, 0x9797a235U, 0x4444cc88U, 0x1717392eU,
    0xc4c45793U, 0xa7a7f255U, 0x7e7e82fcU, 0x3d3d477aU,
    0x6464acc8U, 0x5d5de7baU, 0x19192b32U, 0x737395e6U,
    0x6060a0c0U, 0x81819819U, 0x4f4fd19eU, 0xdcdc7fa3U,
    0x22226644U, 0x2a2a7e54U, 0x9090ab3bU, 0x8888830bU,
    0x4646ca8cU, 0xeeee29c7U, 0xb8b8d36bU, 0x14143c28U,
    0xdede79a7U, 0x5e5ee2bcU, 0x0b0b1d16U, 0xdbdb76adU,
    0xe0e03bdbU, 0x32325664U, 0x3a3a4e74U, 0x0a0a1e14U,
    0x4949db92U, 0x06060a0cU, 0x24246c48U, 0x5c5ce4b8U,
    0xc2c25d9fU, 0xd3d36ebdU, 0xacacef43U, 0x6262a6c4U,
    0x9191a839U, 0x9595a431U, 0xe4e437d3U, 0x79798bf2U,
    0xe7e732d5U, 0xc8c8438bU, 0x3737596eU, 0x6d6db7daU,
    0x8d8d8c01U, 0xd5d564b1U, 0x4e4ed29cU, 0xa9a9e049U,
    0x6c6cb4d8U, 0x5656faacU, 0xf4f407f3U, 0xeaea25cfU,
    0x6565afcaU, 0x7a7a8ef4U, 0xaeaee947U, 0x08081810U,
    0xbabad56fU, 0x787888f0U, 0x25256f4aU, 0x2e2e725cU,
    0x1c1c2438U, 0xa6a6f157U, 0xb4b4c773U, 0xc6c65197U,
    0xe8e823cbU, 0xdddd7ca1U, 0x74749ce8U, 0x1f1f213eU,
    0x4b4bdd96U, 0xbdbddc61U, 0x8b8b860dU, 0x8a8a850fU,
    0x707090e0U, 0x3e3e427cU, 0xb5b5c471U, 0x6666aaccU,
    0x4848d890U, 0x03030506U, 0xf6f601f7U, 0x0e0e121cU,
    0x6161a3c2U, 0x35355f6aU, 0x5757f9aeU, 0xb9b9d069U,
    0x86869117U, 0xc1c15899U, 0x1d1d273aU, 0x9e9eb927U,
    0xe1e138d9U, 0xf8f813ebU, 0x9898b32bU, 0x11113322U,
    0x6969bbd2U, 0xd9d970a9U, 0x8e8e8907U, 0x9494a733U,
    0x9b9bb62dU, 0x1e1e223cU, 0x87879215U, 0xe9e920c9U,
    0xcece4987U, 0x5555ffaaU, 0x28287850U, 0xdfdf7aa5U,
    0x8c8c8f03U, 0xa1a1f859U, 0x89898009U, 0x0d0d171aU,
    0xbfbfda65U, 0xe6e631d7U, 0x4242c684U, 0x6868b8d0U,
    0x4141c382U, 0x9999b029U, 0x2d2d775aU, 0x0f0f111eU,
    0xb0b0cb7bU, 0x5454fca8U, 0xbbbbd66dU, 0x16163a2cU,
};

static const unsigned int LANE_C[768] = {
    0x07fc703d, 0xd3fe381f, 0xb9ff1c0e, 0x5cff8e07, 0xfe7fc702, 0x7f3fe381, 0xef9ff1c1, 0xa7cff8e1, 
    0x83e7fc71, 0x91f3fe39, 0x98f9ff1d, 0x9c7cff8f, 0x9e3e7fc6, 0x4f1f3fe3, 0xf78f9ff0, 0x7bc7cff8, 
    0x3de3e7fc, 0x1ef1f3fe, 0x0f78f9ff, 0xd7bc7cfe, 0x6bde3e7f, 0xe5ef1f3e, 0x72f78f9f, 0xe97bc7ce, 
    0x74bde3e7, 0xea5ef1f2, 0x752f78f9, 0xea97bc7d, 0xa54bde3f, 0x82a5ef1e, 0x4152f78f, 0xf0a97bc6, 
    0x7854bde3, 0xec2a5ef0, 0x76152f78, 0x3b0a97bc, 0x1d854bde, 0x0ec2a5ef, 0xd76152f6, 0x6bb0a97b, 
    0xe5d854bc, 0x72ec2a5e, 0x3976152f, 0xccbb0a96, 0x665d854b, 0xe32ec2a4, 0x71976152, 0x38cbb0a9, 
    0xcc65d855, 0xb632ec2b, 0x8b197614, 0x458cbb0a, 0x22c65d85, 0xc1632ec3, 0xb0b19760, 0x5858cbb0, 
    0x2c2c65d8, 0x161632ec, 0x0b0b1976, 0x05858cbb, 0xd2c2c65c, 0x6961632e, 0x34b0b197, 0xca5858ca, 
    0x652c2c65, 0xe2961633, 0xa14b0b18, 0x50a5858c, 0x2852c2c6, 0x14296163, 0xda14b0b0, 0x6d0a5858, 
    0x36852c2c, 0x1b429616, 0x0da14b0b, 0xd6d0a584, 0x6b6852c2, 0x35b42961, 0xcada14b1, 0xb56d0a59, 
    0x8ab6852d, 0x955b4297, 0x9aada14a, 0x4d56d0a5, 0xf6ab6853, 0xab55b428, 0x55aada14, 0x2ad56d0a, 
    0x156ab685, 0xdab55b43, 0xbd5aada0, 0x5ead56d0, 0x2f56ab68, 0x17ab55b4, 0x0bd5aada, 0x05ead56d, 
    0xd2f56ab7, 0xb97ab55a, 0x5cbd5aad, 0xfe5ead57, 0xaf2f56aa, 0x5797ab55, 0xfbcbd5ab, 0xade5ead4, 
    0x56f2f56a, 0x2b797ab5, 0xc5bcbd5b, 0xb2de5eac, 0x596f2f56, 0x2cb797ab, 0xc65bcbd4, 0x632de5ea, 
    0x3196f2f5, 0xc8cb797b, 0xb465bcbc, 0x5a32de5e, 0x2d196f2f, 0xc68cb796, 0x63465bcb, 0xe1a32de4, 
    0x70d196f2, 0x3868cb79, 0xcc3465bd, 0xb61a32df, 0x8b0d196e, 0x45868cb7, 0xf2c3465a, 0x7961a32d, 
    0xecb0d197, 0xa65868ca, 0x532c3465, 0xf9961a33, 0xaccb0d18, 0x5665868c, 0x2b32c346, 0x159961a3, 
    0xdaccb0d0, 0x6d665868, 0x36b32c34, 0x1b59961a, 0x0daccb0d, 0xd6d66587, 0xbb6b32c2, 0x5db59961, 
    0xfedaccb1, 0xaf6d6659, 0x87b6b32d, 0x93db5997, 0x99edacca, 0x4cf6d665, 0xf67b6b33, 0xab3db598, 
    0x559edacc, 0x2acf6d66, 0x1567b6b3, 0xdab3db58, 0x6d59edac, 0x36acf6d6, 0x1b567b6b, 0xddab3db4, 
    0x6ed59eda, 0x376acf6d, 0xcbb567b7, 0xb5dab3da, 0x5aed59ed, 0xfd76acf7, 0xaebb567a, 0x575dab3d, 
    0xfbaed59f, 0xadd76ace, 0x56ebb567, 0xfb75dab2, 0x7dbaed59, 0xeedd76ad, 0xa76ebb57, 0x83b75daa, 
    0x41dbaed5, 0xf0edd76b, 0xa876ebb4, 0x543b75da, 0x2a1dbaed, 0xc50edd77, 0xb2876eba, 0x5943b75d, 
    0xfca1dbaf, 0xae50edd6, 0x572876eb, 0xfb943b74, 0x7dca1dba, 0x3ee50edd, 0xcf72876f, 0xb7b943b6, 
    0x5bdca1db, 0xfdee50ec, 0x7ef72876, 0x3f7b943b, 0xcfbdca1c, 0x67dee50e, 0x33ef7287, 0xc9f7b942, 
    0x64fbdca1, 0xe27dee51, 0xa13ef729, 0x809f7b95, 0x904fbdcb, 0x9827dee4, 0x4c13ef72, 0x2609f7b9, 
    0xc304fbdd, 0xb1827def, 0x88c13ef6, 0x44609f7b, 0xf2304fbc, 0x791827de, 0x3c8c13ef, 0xce4609f6, 
    0x672304fb, 0xe391827c, 0x71c8c13e, 0x38e4609f, 0xcc72304e, 0x66391827, 0xe31c8c12, 0x718e4609, 
    0xe8c72305, 0xa4639183, 0x8231c8c0, 0x4118e460, 0x208c7230, 0x10463918, 0x08231c8c, 0x04118e46, 
    0x0208c723, 0xd1046390, 0x688231c8, 0x344118e4, 0x1a208c72, 0x0d104639, 0xd688231d, 0xbb44118f, 
    0x8da208c6, 0x46d10463, 0xf3688230, 0x79b44118, 0x3cda208c, 0x1e6d1046, 0x0f368823, 0xd79b4410, 
    0x6bcda208, 0x35e6d104, 0x1af36882, 0x0d79b441, 0xd6bcda21, 0xbb5e6d11, 0x8daf3689, 0x96d79b45, 
    0x9b6bcda3, 0x9db5e6d0, 0x4edaf368, 0x276d79b4, 0x13b6bcda, 0x09db5e6d, 0xd4edaf37, 0xba76d79a, 
    0x5d3b6bcd, 0xfe9db5e7, 0xaf4edaf2, 0x57a76d79, 0xfbd3b6bd, 0xade9db5f, 0x86f4edae, 0x437a76d7, 
    0xf1bd3b6a, 0x78de9db5, 0xec6f4edb, 0xa637a76c, 0x531bd3b6, 0x298de9db, 0xc4c6f4ec, 0x62637a76, 
    0x3131bd3b, 0xc898de9c, 0x644c6f4e, 0x322637a7, 0xc9131bd2, 0x64898de9, 0xe244c6f5, 0xa122637b, 
    0x809131bc, 0x404898de, 0x20244c6f, 0xc0122636, 0x6009131b, 0xe004898c, 0x700244c6, 0x38012263, 
    0xcc009130, 0x66004898, 0x3300244c, 0x19801226, 0x0cc00913, 0xd6600488, 0x6b300244, 0x35980122, 
    0x1acc0091, 0xdd660049, 0xbeb30025, 0x8f598013, 0x97acc008, 0x4bd66004, 0x25eb3002, 0x12f59801, 
    0xd97acc01, 0xbcbd6601, 0x8e5eb301, 0x972f5981, 0x9b97acc1, 0x9dcbd661, 0x9ee5eb31, 0x9f72f599, 
    0x9fb97acd, 0x9fdcbd67, 0x9fee5eb2, 0x4ff72f59, 0xf7fb97ad, 0xabfdcbd7, 0x85fee5ea, 0x42ff72f5, 
    0xf17fb97b, 0xa8bfdcbc, 0x545fee5e, 0x2a2ff72f, 0xc517fb96, 0x628bfdcb, 0xe145fee4, 0x70a2ff72, 
    0x38517fb9, 0xcc28bfdd, 0xb6145fef, 0x8b0a2ff6, 0x458517fb, 0xf2c28bfc, 0x796145fe, 0x3cb0a2ff, 
    0xce58517e, 0x672c28bf, 0xe396145e, 0x71cb0a2f, 0xe8e58516, 0x7472c28b, 0xea396144, 0x751cb0a2, 
    0x3a8e5851, 0xcd472c29, 0xb6a39615, 0x8b51cb0b, 0x95a8e584, 0x4ad472c2, 0x256a3961, 0xc2b51cb1, 
    0xb15a8e59, 0x88ad472d, 0x9456a397, 0x9a2b51ca, 0x4d15a8e5, 0xf68ad473, 0xab456a38, 0x55a2b51c, 
    0x2ad15a8e, 0x1568ad47, 0xdab456a2, 0x6d5a2b51, 0xe6ad15a9, 0xa3568ad5, 0x81ab456b, 0x90d5a2b4, 
    0x486ad15a, 0x243568ad, 0xc21ab457, 0xb10d5a2a, 0x5886ad15, 0xfc43568b, 0xae21ab44, 0x5710d5a2, 
    0x2b886ad1, 0xc5c43569, 0xb2e21ab5, 0x89710d5b, 0x94b886ac, 0x4a5c4356, 0x252e21ab, 0xc29710d4, 
    0x614b886a, 0x30a5c435, 0xc852e21b, 0xb429710c, 0x5a14b886, 0x2d0a5c43, 0xc6852e20, 0x63429710, 
    0x31a14b88, 0x18d0a5c4, 0x0c6852e2, 0x06342971, 0xd31a14b9, 0xb98d0a5d, 0x8cc6852f, 0x96634296, 
    0x4b31a14b, 0xf598d0a4, 0x7acc6852, 0x3d663429, 0xceb31a15, 0xb7598d0b, 0x8bacc684, 0x45d66342, 
    0x22eb31a1, 0xc17598d1, 0xb0bacc69, 0x885d6635, 0x942eb31b, 0x9a17598c, 0x4d0bacc6, 0x2685d663, 
    0xc342eb30, 0x61a17598, 0x30d0bacc, 0x18685d66, 0x0c342eb3, 0xd61a1758, 0x6b0d0bac, 0x358685d6, 
    0x1ac342eb, 0xdd61a174, 0x6eb0d0ba, 0x3758685d, 0xcbac342f, 0xb5d61a16, 0x5aeb0d0b, 0xfd758684, 
    0x7ebac342, 0x3f5d61a1, 0xcfaeb0d1, 0xb7d75869, 0x8bebac35, 0x95f5d61b, 0x9afaeb0c, 0x4d7d7586, 
    0x26bebac3, 0xc35f5d60, 0x61afaeb0, 0x30d7d758, 0x186bebac, 0x0c35f5d6, 0x061afaeb, 0xd30d7d74, 
    0x6986beba, 0x34c35f5d, 0xca61afaf, 0xb530d7d6, 0x5a986beb, 0xfd4c35f4, 0x7ea61afa, 0x3f530d7d, 
    0xcfa986bf, 0xb7d4c35e, 0x5bea61af, 0xfdf530d6, 0x7efa986b, 0xef7d4c34, 0x77bea61a, 0x3bdf530d, 
    0xcdefa987, 0xb6f7d4c2, 0x5b7bea61, 0xfdbdf531, 0xaedefa99, 0x876f7d4d, 0x93b7bea7, 0x99dbdf52, 
    0x4cedefa9, 0xf676f7d5, 0xab3b7beb, 0x859dbdf4, 0x42cedefa, 0x21676f7d, 0xc0b3b7bf, 0xb059dbde, 
    0x582cedef, 0xfc1676f6, 0x7e0b3b7b, 0xef059dbc, 0x7782cede, 0x3bc1676f, 0xcde0b3b6, 0x66f059db, 
    0xe3782cec, 0x71bc1676, 0x38de0b3b, 0xcc6f059c, 0x663782ce, 0x331bc167, 0xc98de0b2, 0x64c6f059, 
    0xe263782d, 0xa131bc17, 0x8098de0a, 0x404c6f05, 0xf0263783, 0xa8131bc0, 0x54098de0, 0x2a04c6f0, 
    0x15026378, 0x0a8131bc, 0x054098de, 0x02a04c6f, 0xd1502636, 0x68a8131b, 0xe454098c, 0x722a04c6, 
    0x39150263, 0xcc8a8130, 0x66454098, 0x3322a04c, 0x19915026, 0x0cc8a813, 0xd6645408, 0x6b322a04, 
    0x35991502, 0x1acc8a81, 0xdd664541, 0xbeb322a1, 0x8f599151, 0x97acc8a9, 0x9bd66455, 0x9deb322b, 
    0x9ef59914, 0x4f7acc8a, 0x27bd6645, 0xc3deb323, 0xb1ef5990, 0x58f7acc8, 0x2c7bd664, 0x163deb32, 
    0x0b1ef599, 0xd58f7acd, 0xbac7bd67, 0x8d63deb2, 0x46b1ef59, 0xf358f7ad, 0xa9ac7bd7, 0x84d63dea, 
    0x426b1ef5, 0xf1358f7b, 0xa89ac7bc, 0x544d63de, 0x2a26b1ef, 0xc51358f6, 0x6289ac7b, 0xe144d63c, 
    0x70a26b1e, 0x3851358f, 0xcc289ac6, 0x66144d63, 0xe30a26b0, 0x71851358, 0x38c289ac, 0x1c6144d6, 
    0x0e30a26b, 0xd7185134, 0x6b8c289a, 0x35c6144d, 0xcae30a27, 0xb5718512, 0x5ab8c289, 0xfd5c6145, 
    0xaeae30a3, 0x87571850, 0x43ab8c28, 0x21d5c614, 0x10eae30a, 0x08757185, 0xd43ab8c3, 0xba1d5c60, 
    0x5d0eae30, 0x2e875718, 0x1743ab8c, 0x0ba1d5c6, 0x05d0eae3, 0xd2e87570, 0x69743ab8, 0x34ba1d5c, 
    0x1a5d0eae, 0x0d2e8757, 0xd69743aa, 0x6b4ba1d5, 0xe5a5d0eb, 0xa2d2e874, 0x5169743a, 0x28b4ba1d, 
    0xc45a5d0f, 0xb22d2e86, 0x59169743, 0xfc8b4ba0, 0x7e45a5d0, 0x3f22d2e8, 0x1f916974, 0x0fc8b4ba, 
    0x07e45a5d, 0xd3f22d2f, 0xb9f91696, 0x5cfc8b4b, 0xfe7e45a4, 0x7f3f22d2, 0x3f9f9169, 0xcfcfc8b5, 
    0xb7e7e45b, 0x8bf3f22c, 0x45f9f916, 0x22fcfc8b, 0xc17e7e44, 0x60bf3f22, 0x305f9f91, 0xc82fcfc9, 
    0xb417e7e5, 0x8a0bf3f3, 0x9505f9f8, 0x4a82fcfc, 0x25417e7e, 0x12a0bf3f, 0xd9505f9e, 0x6ca82fcf, 
    0xe65417e6, 0x732a0bf3, 0xe99505f8, 0x74ca82fc, 0x3a65417e, 0x1d32a0bf, 0xde99505e, 0x6f4ca82f, 
    0xe7a65416, 0x73d32a0b, 0xe9e99504, 0x74f4ca82, 0x3a7a6541, 0xcd3d32a1, 0xb69e9951, 0x8b4f4ca9, 
    0x95a7a655, 0x9ad3d32b, 0x9d69e994, 0x4eb4f4ca, 0x275a7a65, 0xc3ad3d33, 0xb1d69e98, 0x58eb4f4c, 
    0x2c75a7a6, 0x163ad3d3, 0xdb1d69e8, 0x6d8eb4f4, 0x36c75a7a, 0x1b63ad3d, 0xddb1d69f, 0xbed8eb4e, 
    0x5f6c75a7, 0xffb63ad2, 0x7fdb1d69, 0xefed8eb5, 0xa7f6c75b, 0x83fb63ac, 0x41fdb1d6, 0x20fed8eb, 
    0xc07f6c74, 0x603fb63a, 0x301fdb1d, 0xc80fed8f, 0xb407f6c6, 0x5a03fb63, 0xfd01fdb0, 0x7e80fed8, 
    0x3f407f6c, 0x1fa03fb6, 0x0fd01fdb, 0xd7e80fec, 0x6bf407f6, 0x35fa03fb, 0xcafd01fc, 0x657e80fe, 
    0x32bf407f, 0xc95fa03e, 0x64afd01f, 0xe257e80e, 0x712bf407, 0xe895fa02, 0x744afd01, 0xea257e81, 
    0xa512bf41, 0x82895fa1, 0x9144afd1, 0x98a257e9, 0x9c512bf5, 0x9e2895fb, 0x9f144afc, 0x4f8a257e, 
    0x27c512bf, 0xc3e2895e, 0x61f144af, 0xe0f8a256, 0x707c512b, 0xe83e2894, 0x741f144a, 0x3a0f8a25, 
    0xcd07c513, 0xb683e288, 0x5b41f144, 0x2da0f8a2, 0x16d07c51, 0xdb683e29, 0xbdb41f15, 0x8eda0f8b, 
    0x976d07c4, 0x4bb683e2, 0x25db41f1, 0xc2eda0f9, 0xb176d07d, 0x88bb683f, 0x945db41e, 0x4a2eda0f, 
    0xf5176d06, 0x7a8bb683, 0xed45db40, 0x76a2eda0, 0x3b5176d0, 0x1da8bb68, 0x0ed45db4, 0x076a2eda, 
    0x03b5176d, 0xd1da8bb7, 0xb8ed45da, 0x5c76a2ed, 0xfe3b5177, 0xaf1da8ba, 0x578ed45d, 0xfbc76a2f, 
    0xade3b516, 0x56f1da8b, 0xfb78ed44, 0x7dbc76a2, 0x3ede3b51, 0xcf6f1da9, 0xb7b78ed5, 0x8bdbc76b, 
    0x95ede3b4, 0x4af6f1da, 0x257b78ed, 0xc2bdbc77, 0xb15ede3a, 0x58af6f1d, 0xfc57b78f, 0xae2bdbc6, 
    0x5715ede3, 0xfb8af6f0, 0x7dc57b78, 0x3ee2bdbc, 0x1f715ede, 0x0fb8af6f, 0xd7dc57b6, 0x6bee2bdb, 
};

void Lane::lane256_compress(const unsigned char m[64], unsigned int h[8], const unsigned int ctrh, const unsigned int ctrl)
{
  unsigned int t0, t1, t2, t3, t4, t5, t6, t7; /* temp */
  unsigned int s00, s01, s02, s03, s04, s05, s06, s07; /* lane 0 */
  unsigned int s10, s11, s12, s13, s14, s15, s16, s17; /* lane 1 */
  unsigned int s20, s21, s22, s23, s24, s25, s26, s27; /* lane 2 */
  unsigned int s30, s31, s32, s33, s34, s35, s36, s37; /* lane 3 */
  unsigned int s40, s41, s42, s43, s44, s45, s46, s47; /* lane 4 */
  unsigned int s50, s51, s52, s53, s54, s55, s56, s57; /* lane 5 */
  unsigned int s60, s61, s62, s63, s64, s65, s66, s67; /* lane 6 */
  unsigned int s70, s71, s72, s73, s74, s75, s76, s77; /* lane 7 */

  /* Message expansion */
  s30 = h[0];
  s31 = h[1];
  s32 = h[2];
  s33 = h[3];
  s34 = h[4];
  s35 = h[5];
  s36 = h[6];
  s37 = h[7];
  s40 = LANE_U8TO32_BIG(m +  0);
  s41 = LANE_U8TO32_BIG(m +  4);
  s42 = LANE_U8TO32_BIG(m +  8);
  s43 = LANE_U8TO32_BIG(m + 12);
  s44 = LANE_U8TO32_BIG(m + 16);
  s45 = LANE_U8TO32_BIG(m + 20);
  s46 = LANE_U8TO32_BIG(m + 24);
  s47 = LANE_U8TO32_BIG(m + 28);
  s50 = LANE_U8TO32_BIG(m + 32);
  s51 = LANE_U8TO32_BIG(m + 36);
  s52 = LANE_U8TO32_BIG(m + 40);
  s53 = LANE_U8TO32_BIG(m + 44);
  s54 = LANE_U8TO32_BIG(m + 48);
  s55 = LANE_U8TO32_BIG(m + 52);
  s56 = LANE_U8TO32_BIG(m + 56);
  s57 = LANE_U8TO32_BIG(m + 60);
  s00 = s30 ^ s40 ^ s44 ^ s50 ^ s54;
  s01 = s31 ^ s41 ^ s45 ^ s51 ^ s55;
  s02 = s32 ^ s42 ^ s46 ^ s52 ^ s56;
  s03 = s33 ^ s43 ^ s47 ^ s53 ^ s57;
  s04 = s34 ^ s40 ^ s50;
  s05 = s35 ^ s41 ^ s51;
  s06 = s36 ^ s42 ^ s52;
  s07 = s37 ^ s43 ^ s53;
  s10 = s00 ^ s34 ^ s44;
  s11 = s01 ^ s35 ^ s45;
  s12 = s02 ^ s36 ^ s46;
  s13 = s03 ^ s37 ^ s47;
  s14 = s30 ^ s44 ^ s50;
  s15 = s31 ^ s45 ^ s51;
  s16 = s32 ^ s46 ^ s52;
  s17 = s33 ^ s47 ^ s53;
  s20 = s00 ^ s34 ^ s54;
  s21 = s01 ^ s35 ^ s55;
  s22 = s02 ^ s36 ^ s56;
  s23 = s03 ^ s37 ^ s57;
  s24 = s30 ^ s40 ^ s54;
  s25 = s31 ^ s41 ^ s55;
  s26 = s32 ^ s42 ^ s56;
  s27 = s33 ^ s43 ^ s57;

  /* Lane 0 */
  if (numRoundsLane224P >= 1) {
  t0  = LANE_T0[LANE_B3(s00)] ^ LANE_T1[LANE_B2(s01)] ^ LANE_T2[LANE_B1(s02)] ^ LANE_T3[LANE_B0(s03)] ^ LANE_C[ 0];
  t1  = LANE_T0[LANE_B3(s01)] ^ LANE_T1[LANE_B2(s02)] ^ LANE_T2[LANE_B1(s03)] ^ LANE_T3[LANE_B0(s00)] ^ LANE_C[ 1];
  t4  = LANE_T0[LANE_B3(s02)] ^ LANE_T1[LANE_B2(s03)] ^ LANE_T2[LANE_B1(s00)] ^ LANE_T3[LANE_B0(s01)] ^ LANE_C[ 2];
  t5  = LANE_T0[LANE_B3(s03)] ^ LANE_T1[LANE_B2(s00)] ^ LANE_T2[LANE_B1(s01)] ^ LANE_T3[LANE_B0(s02)] ^ LANE_C[ 3] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s04)] ^ LANE_T1[LANE_B2(s05)] ^ LANE_T2[LANE_B1(s06)] ^ LANE_T3[LANE_B0(s07)] ^ LANE_C[ 4];
  t3  = LANE_T0[LANE_B3(s05)] ^ LANE_T1[LANE_B2(s06)] ^ LANE_T2[LANE_B1(s07)] ^ LANE_T3[LANE_B0(s04)] ^ LANE_C[ 5];
  t6  = LANE_T0[LANE_B3(s06)] ^ LANE_T1[LANE_B2(s07)] ^ LANE_T2[LANE_B1(s04)] ^ LANE_T3[LANE_B0(s05)] ^ LANE_C[ 6];
  t7  = LANE_T0[LANE_B3(s07)] ^ LANE_T1[LANE_B2(s04)] ^ LANE_T2[LANE_B1(s05)] ^ LANE_T3[LANE_B0(s06)] ^ LANE_C[ 7];
  }

  if (numRoundsLane224P >= 2) {
  s00 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 8];
  s01 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 9];
  s04 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[10];
  s05 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[11] ^ ctrl;
  s02 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[12];
  s03 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[13];
  s06 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[14];
  s07 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[15];
  }

  if (numRoundsLane224P >= 3) {
  t0  = LANE_T0[LANE_B3(s00)] ^ LANE_T1[LANE_B2(s01)] ^ LANE_T2[LANE_B1(s02)] ^ LANE_T3[LANE_B0(s03)] ^ LANE_C[16];
  t1  = LANE_T0[LANE_B3(s01)] ^ LANE_T1[LANE_B2(s02)] ^ LANE_T2[LANE_B1(s03)] ^ LANE_T3[LANE_B0(s00)] ^ LANE_C[17];
  t4  = LANE_T0[LANE_B3(s02)] ^ LANE_T1[LANE_B2(s03)] ^ LANE_T2[LANE_B1(s00)] ^ LANE_T3[LANE_B0(s01)] ^ LANE_C[18];
  t5  = LANE_T0[LANE_B3(s03)] ^ LANE_T1[LANE_B2(s00)] ^ LANE_T2[LANE_B1(s01)] ^ LANE_T3[LANE_B0(s02)] ^ LANE_C[19] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s04)] ^ LANE_T1[LANE_B2(s05)] ^ LANE_T2[LANE_B1(s06)] ^ LANE_T3[LANE_B0(s07)] ^ LANE_C[20];
  t3  = LANE_T0[LANE_B3(s05)] ^ LANE_T1[LANE_B2(s06)] ^ LANE_T2[LANE_B1(s07)] ^ LANE_T3[LANE_B0(s04)] ^ LANE_C[21];
  t6  = LANE_T0[LANE_B3(s06)] ^ LANE_T1[LANE_B2(s07)] ^ LANE_T2[LANE_B1(s04)] ^ LANE_T3[LANE_B0(s05)] ^ LANE_C[22];
  t7  = LANE_T0[LANE_B3(s07)] ^ LANE_T1[LANE_B2(s04)] ^ LANE_T2[LANE_B1(s05)] ^ LANE_T3[LANE_B0(s06)] ^ LANE_C[23];
  }

  if (numRoundsLane224P >= 4) {
  s00 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[24];
  s01 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[25];
  s04 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[26];
  s05 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[27] ^ ctrl;
  s02 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[28];
  s03 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[29];
  s06 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[30];
  s07 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[31];
  }

  if (numRoundsLane224P >= 5) {
  t0  = LANE_T0[LANE_B3(s00)] ^ LANE_T1[LANE_B2(s01)] ^ LANE_T2[LANE_B1(s02)] ^ LANE_T3[LANE_B0(s03)] ^ LANE_C[32];
  t1  = LANE_T0[LANE_B3(s01)] ^ LANE_T1[LANE_B2(s02)] ^ LANE_T2[LANE_B1(s03)] ^ LANE_T3[LANE_B0(s00)] ^ LANE_C[33];
  t4  = LANE_T0[LANE_B3(s02)] ^ LANE_T1[LANE_B2(s03)] ^ LANE_T2[LANE_B1(s00)] ^ LANE_T3[LANE_B0(s01)] ^ LANE_C[34];
  t5  = LANE_T0[LANE_B3(s03)] ^ LANE_T1[LANE_B2(s00)] ^ LANE_T2[LANE_B1(s01)] ^ LANE_T3[LANE_B0(s02)] ^ LANE_C[35] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s04)] ^ LANE_T1[LANE_B2(s05)] ^ LANE_T2[LANE_B1(s06)] ^ LANE_T3[LANE_B0(s07)] ^ LANE_C[36];
  t3  = LANE_T0[LANE_B3(s05)] ^ LANE_T1[LANE_B2(s06)] ^ LANE_T2[LANE_B1(s07)] ^ LANE_T3[LANE_B0(s04)] ^ LANE_C[37];
  t6  = LANE_T0[LANE_B3(s06)] ^ LANE_T1[LANE_B2(s07)] ^ LANE_T2[LANE_B1(s04)] ^ LANE_T3[LANE_B0(s05)] ^ LANE_C[38];
  t7  = LANE_T0[LANE_B3(s07)] ^ LANE_T1[LANE_B2(s04)] ^ LANE_T2[LANE_B1(s05)] ^ LANE_T3[LANE_B0(s06)] ^ LANE_C[39];
  }

  if (numRoundsLane224P >= 6) {
  s60 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s61 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s64 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s65 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s62 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s63 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s66 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s67 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  }

  /* Lane 1 */
  if (numRoundsLane224P >= 1) {
  t0  = LANE_T0[LANE_B3(s10)] ^ LANE_T1[LANE_B2(s11)] ^ LANE_T2[LANE_B1(s12)] ^ LANE_T3[LANE_B0(s13)] ^ LANE_C[ 0+40];
  t1  = LANE_T0[LANE_B3(s11)] ^ LANE_T1[LANE_B2(s12)] ^ LANE_T2[LANE_B1(s13)] ^ LANE_T3[LANE_B0(s10)] ^ LANE_C[ 1+40];
  t4  = LANE_T0[LANE_B3(s12)] ^ LANE_T1[LANE_B2(s13)] ^ LANE_T2[LANE_B1(s10)] ^ LANE_T3[LANE_B0(s11)] ^ LANE_C[ 2+40];
  t5  = LANE_T0[LANE_B3(s13)] ^ LANE_T1[LANE_B2(s10)] ^ LANE_T2[LANE_B1(s11)] ^ LANE_T3[LANE_B0(s12)] ^ LANE_C[ 3+40] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s14)] ^ LANE_T1[LANE_B2(s15)] ^ LANE_T2[LANE_B1(s16)] ^ LANE_T3[LANE_B0(s17)] ^ LANE_C[ 4+40];
  t3  = LANE_T0[LANE_B3(s15)] ^ LANE_T1[LANE_B2(s16)] ^ LANE_T2[LANE_B1(s17)] ^ LANE_T3[LANE_B0(s14)] ^ LANE_C[ 5+40];
  t6  = LANE_T0[LANE_B3(s16)] ^ LANE_T1[LANE_B2(s17)] ^ LANE_T2[LANE_B1(s14)] ^ LANE_T3[LANE_B0(s15)] ^ LANE_C[ 6+40];
  t7  = LANE_T0[LANE_B3(s17)] ^ LANE_T1[LANE_B2(s14)] ^ LANE_T2[LANE_B1(s15)] ^ LANE_T3[LANE_B0(s16)] ^ LANE_C[ 7+40];
  }

  if (numRoundsLane224P >= 2) {
  s10 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 8+40];
  s11 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 9+40];
  s14 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[10+40];
  s15 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[11+40] ^ ctrh;
  s12 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[12+40];
  s13 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[13+40];
  s16 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[14+40];
  s17 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[15+40];
  }

  if (numRoundsLane224P >= 3) {
  t0  = LANE_T0[LANE_B3(s10)] ^ LANE_T1[LANE_B2(s11)] ^ LANE_T2[LANE_B1(s12)] ^ LANE_T3[LANE_B0(s13)] ^ LANE_C[16+40];
  t1  = LANE_T0[LANE_B3(s11)] ^ LANE_T1[LANE_B2(s12)] ^ LANE_T2[LANE_B1(s13)] ^ LANE_T3[LANE_B0(s10)] ^ LANE_C[17+40];
  t4  = LANE_T0[LANE_B3(s12)] ^ LANE_T1[LANE_B2(s13)] ^ LANE_T2[LANE_B1(s10)] ^ LANE_T3[LANE_B0(s11)] ^ LANE_C[18+40];
  t5  = LANE_T0[LANE_B3(s13)] ^ LANE_T1[LANE_B2(s10)] ^ LANE_T2[LANE_B1(s11)] ^ LANE_T3[LANE_B0(s12)] ^ LANE_C[19+40] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s14)] ^ LANE_T1[LANE_B2(s15)] ^ LANE_T2[LANE_B1(s16)] ^ LANE_T3[LANE_B0(s17)] ^ LANE_C[20+40];
  t3  = LANE_T0[LANE_B3(s15)] ^ LANE_T1[LANE_B2(s16)] ^ LANE_T2[LANE_B1(s17)] ^ LANE_T3[LANE_B0(s14)] ^ LANE_C[21+40];
  t6  = LANE_T0[LANE_B3(s16)] ^ LANE_T1[LANE_B2(s17)] ^ LANE_T2[LANE_B1(s14)] ^ LANE_T3[LANE_B0(s15)] ^ LANE_C[22+40];
  t7  = LANE_T0[LANE_B3(s17)] ^ LANE_T1[LANE_B2(s14)] ^ LANE_T2[LANE_B1(s15)] ^ LANE_T3[LANE_B0(s16)] ^ LANE_C[23+40];
  }

  if (numRoundsLane224P >= 4) {
  s10 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[24+40];
  s11 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[25+40];
  s14 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[26+40];
  s15 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[27+40] ^ ctrh;
  s12 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[28+40];
  s13 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[29+40];
  s16 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[30+40];
  s17 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[31+40];
  }

  if (numRoundsLane224P >= 5) {
  t0  = LANE_T0[LANE_B3(s10)] ^ LANE_T1[LANE_B2(s11)] ^ LANE_T2[LANE_B1(s12)] ^ LANE_T3[LANE_B0(s13)] ^ LANE_C[32+40];
  t1  = LANE_T0[LANE_B3(s11)] ^ LANE_T1[LANE_B2(s12)] ^ LANE_T2[LANE_B1(s13)] ^ LANE_T3[LANE_B0(s10)] ^ LANE_C[33+40];
  t4  = LANE_T0[LANE_B3(s12)] ^ LANE_T1[LANE_B2(s13)] ^ LANE_T2[LANE_B1(s10)] ^ LANE_T3[LANE_B0(s11)] ^ LANE_C[34+40];
  t5  = LANE_T0[LANE_B3(s13)] ^ LANE_T1[LANE_B2(s10)] ^ LANE_T2[LANE_B1(s11)] ^ LANE_T3[LANE_B0(s12)] ^ LANE_C[35+40] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s14)] ^ LANE_T1[LANE_B2(s15)] ^ LANE_T2[LANE_B1(s16)] ^ LANE_T3[LANE_B0(s17)] ^ LANE_C[36+40];
  t3  = LANE_T0[LANE_B3(s15)] ^ LANE_T1[LANE_B2(s16)] ^ LANE_T2[LANE_B1(s17)] ^ LANE_T3[LANE_B0(s14)] ^ LANE_C[37+40];
  t6  = LANE_T0[LANE_B3(s16)] ^ LANE_T1[LANE_B2(s17)] ^ LANE_T2[LANE_B1(s14)] ^ LANE_T3[LANE_B0(s15)] ^ LANE_C[38+40];
  t7  = LANE_T0[LANE_B3(s17)] ^ LANE_T1[LANE_B2(s14)] ^ LANE_T2[LANE_B1(s15)] ^ LANE_T3[LANE_B0(s16)] ^ LANE_C[39+40];
  }

  if (numRoundsLane224P >= 6) {
  s60 ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s61 ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s64 ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s65 ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s62 ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s63 ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s66 ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s67 ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  }

  /* Lane 2 */
  if (numRoundsLane224P >= 1) {
  t0  = LANE_T0[LANE_B3(s20)] ^ LANE_T1[LANE_B2(s21)] ^ LANE_T2[LANE_B1(s22)] ^ LANE_T3[LANE_B0(s23)] ^ LANE_C[ 0+80];
  t1  = LANE_T0[LANE_B3(s21)] ^ LANE_T1[LANE_B2(s22)] ^ LANE_T2[LANE_B1(s23)] ^ LANE_T3[LANE_B0(s20)] ^ LANE_C[ 1+80];
  t4  = LANE_T0[LANE_B3(s22)] ^ LANE_T1[LANE_B2(s23)] ^ LANE_T2[LANE_B1(s20)] ^ LANE_T3[LANE_B0(s21)] ^ LANE_C[ 2+80];
  t5  = LANE_T0[LANE_B3(s23)] ^ LANE_T1[LANE_B2(s20)] ^ LANE_T2[LANE_B1(s21)] ^ LANE_T3[LANE_B0(s22)] ^ LANE_C[ 3+80] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s24)] ^ LANE_T1[LANE_B2(s25)] ^ LANE_T2[LANE_B1(s26)] ^ LANE_T3[LANE_B0(s27)] ^ LANE_C[ 4+80];
  t3  = LANE_T0[LANE_B3(s25)] ^ LANE_T1[LANE_B2(s26)] ^ LANE_T2[LANE_B1(s27)] ^ LANE_T3[LANE_B0(s24)] ^ LANE_C[ 5+80];
  t6  = LANE_T0[LANE_B3(s26)] ^ LANE_T1[LANE_B2(s27)] ^ LANE_T2[LANE_B1(s24)] ^ LANE_T3[LANE_B0(s25)] ^ LANE_C[ 6+80];
  t7  = LANE_T0[LANE_B3(s27)] ^ LANE_T1[LANE_B2(s24)] ^ LANE_T2[LANE_B1(s25)] ^ LANE_T3[LANE_B0(s26)] ^ LANE_C[ 7+80];
  }

  if (numRoundsLane224P >= 2) {
  s20 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 8+80];
  s21 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 9+80];
  s24 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[10+80];
  s25 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[11+80] ^ ctrl;
  s22 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[12+80];
  s23 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[13+80];
  s26 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[14+80];
  s27 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[15+80];
  }

  if (numRoundsLane224P >= 3) {
  t0  = LANE_T0[LANE_B3(s20)] ^ LANE_T1[LANE_B2(s21)] ^ LANE_T2[LANE_B1(s22)] ^ LANE_T3[LANE_B0(s23)] ^ LANE_C[16+80];
  t1  = LANE_T0[LANE_B3(s21)] ^ LANE_T1[LANE_B2(s22)] ^ LANE_T2[LANE_B1(s23)] ^ LANE_T3[LANE_B0(s20)] ^ LANE_C[17+80];
  t4  = LANE_T0[LANE_B3(s22)] ^ LANE_T1[LANE_B2(s23)] ^ LANE_T2[LANE_B1(s20)] ^ LANE_T3[LANE_B0(s21)] ^ LANE_C[18+80];
  t5  = LANE_T0[LANE_B3(s23)] ^ LANE_T1[LANE_B2(s20)] ^ LANE_T2[LANE_B1(s21)] ^ LANE_T3[LANE_B0(s22)] ^ LANE_C[19+80] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s24)] ^ LANE_T1[LANE_B2(s25)] ^ LANE_T2[LANE_B1(s26)] ^ LANE_T3[LANE_B0(s27)] ^ LANE_C[20+80];
  t3  = LANE_T0[LANE_B3(s25)] ^ LANE_T1[LANE_B2(s26)] ^ LANE_T2[LANE_B1(s27)] ^ LANE_T3[LANE_B0(s24)] ^ LANE_C[21+80];
  t6  = LANE_T0[LANE_B3(s26)] ^ LANE_T1[LANE_B2(s27)] ^ LANE_T2[LANE_B1(s24)] ^ LANE_T3[LANE_B0(s25)] ^ LANE_C[22+80];
  t7  = LANE_T0[LANE_B3(s27)] ^ LANE_T1[LANE_B2(s24)] ^ LANE_T2[LANE_B1(s25)] ^ LANE_T3[LANE_B0(s26)] ^ LANE_C[23+80];
  }

  if (numRoundsLane224P >= 4) {
  s20 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[24+80];
  s21 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[25+80];
  s24 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[26+80];
  s25 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[27+80] ^ ctrl;
  s22 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[28+80];
  s23 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[29+80];
  s26 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[30+80];
  s27 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[31+80];
  }

  if (numRoundsLane224P >= 5) {
  t0  = LANE_T0[LANE_B3(s20)] ^ LANE_T1[LANE_B2(s21)] ^ LANE_T2[LANE_B1(s22)] ^ LANE_T3[LANE_B0(s23)] ^ LANE_C[32+80];
  t1  = LANE_T0[LANE_B3(s21)] ^ LANE_T1[LANE_B2(s22)] ^ LANE_T2[LANE_B1(s23)] ^ LANE_T3[LANE_B0(s20)] ^ LANE_C[33+80];
  t4  = LANE_T0[LANE_B3(s22)] ^ LANE_T1[LANE_B2(s23)] ^ LANE_T2[LANE_B1(s20)] ^ LANE_T3[LANE_B0(s21)] ^ LANE_C[34+80];
  t5  = LANE_T0[LANE_B3(s23)] ^ LANE_T1[LANE_B2(s20)] ^ LANE_T2[LANE_B1(s21)] ^ LANE_T3[LANE_B0(s22)] ^ LANE_C[35+80] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s24)] ^ LANE_T1[LANE_B2(s25)] ^ LANE_T2[LANE_B1(s26)] ^ LANE_T3[LANE_B0(s27)] ^ LANE_C[36+80];
  t3  = LANE_T0[LANE_B3(s25)] ^ LANE_T1[LANE_B2(s26)] ^ LANE_T2[LANE_B1(s27)] ^ LANE_T3[LANE_B0(s24)] ^ LANE_C[37+80];
  t6  = LANE_T0[LANE_B3(s26)] ^ LANE_T1[LANE_B2(s27)] ^ LANE_T2[LANE_B1(s24)] ^ LANE_T3[LANE_B0(s25)] ^ LANE_C[38+80];
  t7  = LANE_T0[LANE_B3(s27)] ^ LANE_T1[LANE_B2(s24)] ^ LANE_T2[LANE_B1(s25)] ^ LANE_T3[LANE_B0(s26)] ^ LANE_C[39+80];
  }

  if (numRoundsLane224P >= 6) {
  s60 ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s61 ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s64 ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s65 ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s62 ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s63 ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s66 ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s67 ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  }

  /* Lane 3 */
  if (numRoundsLane224P >= 1) {
  t0  = LANE_T0[LANE_B3(s30)] ^ LANE_T1[LANE_B2(s31)] ^ LANE_T2[LANE_B1(s32)] ^ LANE_T3[LANE_B0(s33)] ^ LANE_C[ 0+120];
  t1  = LANE_T0[LANE_B3(s31)] ^ LANE_T1[LANE_B2(s32)] ^ LANE_T2[LANE_B1(s33)] ^ LANE_T3[LANE_B0(s30)] ^ LANE_C[ 1+120];
  t4  = LANE_T0[LANE_B3(s32)] ^ LANE_T1[LANE_B2(s33)] ^ LANE_T2[LANE_B1(s30)] ^ LANE_T3[LANE_B0(s31)] ^ LANE_C[ 2+120];
  t5  = LANE_T0[LANE_B3(s33)] ^ LANE_T1[LANE_B2(s30)] ^ LANE_T2[LANE_B1(s31)] ^ LANE_T3[LANE_B0(s32)] ^ LANE_C[ 3+120] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s34)] ^ LANE_T1[LANE_B2(s35)] ^ LANE_T2[LANE_B1(s36)] ^ LANE_T3[LANE_B0(s37)] ^ LANE_C[ 4+120];
  t3  = LANE_T0[LANE_B3(s35)] ^ LANE_T1[LANE_B2(s36)] ^ LANE_T2[LANE_B1(s37)] ^ LANE_T3[LANE_B0(s34)] ^ LANE_C[ 5+120];
  t6  = LANE_T0[LANE_B3(s36)] ^ LANE_T1[LANE_B2(s37)] ^ LANE_T2[LANE_B1(s34)] ^ LANE_T3[LANE_B0(s35)] ^ LANE_C[ 6+120];
  t7  = LANE_T0[LANE_B3(s37)] ^ LANE_T1[LANE_B2(s34)] ^ LANE_T2[LANE_B1(s35)] ^ LANE_T3[LANE_B0(s36)] ^ LANE_C[ 7+120];
  }

  if (numRoundsLane224P >= 2) {
  s30 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 8+120];
  s31 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 9+120];
  s34 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[10+120];
  s35 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[11+120] ^ ctrh;
  s32 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[12+120];
  s33 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[13+120];
  s36 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[14+120];
  s37 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[15+120];
  }

  if (numRoundsLane224P >= 3) {
  t0  = LANE_T0[LANE_B3(s30)] ^ LANE_T1[LANE_B2(s31)] ^ LANE_T2[LANE_B1(s32)] ^ LANE_T3[LANE_B0(s33)] ^ LANE_C[16+120];
  t1  = LANE_T0[LANE_B3(s31)] ^ LANE_T1[LANE_B2(s32)] ^ LANE_T2[LANE_B1(s33)] ^ LANE_T3[LANE_B0(s30)] ^ LANE_C[17+120];
  t4  = LANE_T0[LANE_B3(s32)] ^ LANE_T1[LANE_B2(s33)] ^ LANE_T2[LANE_B1(s30)] ^ LANE_T3[LANE_B0(s31)] ^ LANE_C[18+120];
  t5  = LANE_T0[LANE_B3(s33)] ^ LANE_T1[LANE_B2(s30)] ^ LANE_T2[LANE_B1(s31)] ^ LANE_T3[LANE_B0(s32)] ^ LANE_C[19+120] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s34)] ^ LANE_T1[LANE_B2(s35)] ^ LANE_T2[LANE_B1(s36)] ^ LANE_T3[LANE_B0(s37)] ^ LANE_C[20+120];
  t3  = LANE_T0[LANE_B3(s35)] ^ LANE_T1[LANE_B2(s36)] ^ LANE_T2[LANE_B1(s37)] ^ LANE_T3[LANE_B0(s34)] ^ LANE_C[21+120];
  t6  = LANE_T0[LANE_B3(s36)] ^ LANE_T1[LANE_B2(s37)] ^ LANE_T2[LANE_B1(s34)] ^ LANE_T3[LANE_B0(s35)] ^ LANE_C[22+120];
  t7  = LANE_T0[LANE_B3(s37)] ^ LANE_T1[LANE_B2(s34)] ^ LANE_T2[LANE_B1(s35)] ^ LANE_T3[LANE_B0(s36)] ^ LANE_C[23+120];
  }

  if (numRoundsLane224P >= 4) {
  s30 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[24+120];
  s31 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[25+120];
  s34 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[26+120];
  s35 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[27+120] ^ ctrh;
  s32 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[28+120];
  s33 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[29+120];
  s36 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[30+120];
  s37 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[31+120];
  }

  if (numRoundsLane224P >= 5) {
  t0  = LANE_T0[LANE_B3(s30)] ^ LANE_T1[LANE_B2(s31)] ^ LANE_T2[LANE_B1(s32)] ^ LANE_T3[LANE_B0(s33)] ^ LANE_C[32+120];
  t1  = LANE_T0[LANE_B3(s31)] ^ LANE_T1[LANE_B2(s32)] ^ LANE_T2[LANE_B1(s33)] ^ LANE_T3[LANE_B0(s30)] ^ LANE_C[33+120];
  t4  = LANE_T0[LANE_B3(s32)] ^ LANE_T1[LANE_B2(s33)] ^ LANE_T2[LANE_B1(s30)] ^ LANE_T3[LANE_B0(s31)] ^ LANE_C[34+120];
  t5  = LANE_T0[LANE_B3(s33)] ^ LANE_T1[LANE_B2(s30)] ^ LANE_T2[LANE_B1(s31)] ^ LANE_T3[LANE_B0(s32)] ^ LANE_C[35+120] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s34)] ^ LANE_T1[LANE_B2(s35)] ^ LANE_T2[LANE_B1(s36)] ^ LANE_T3[LANE_B0(s37)] ^ LANE_C[36+120];
  t3  = LANE_T0[LANE_B3(s35)] ^ LANE_T1[LANE_B2(s36)] ^ LANE_T2[LANE_B1(s37)] ^ LANE_T3[LANE_B0(s34)] ^ LANE_C[37+120];
  t6  = LANE_T0[LANE_B3(s36)] ^ LANE_T1[LANE_B2(s37)] ^ LANE_T2[LANE_B1(s34)] ^ LANE_T3[LANE_B0(s35)] ^ LANE_C[38+120];
  t7  = LANE_T0[LANE_B3(s37)] ^ LANE_T1[LANE_B2(s34)] ^ LANE_T2[LANE_B1(s35)] ^ LANE_T3[LANE_B0(s36)] ^ LANE_C[39+120];
  }

  if (numRoundsLane224P >= 6) {
  s70 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s71 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s74 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s75 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s72 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s73 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s76 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s77 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  }

  /* Lane 4 */
  if (numRoundsLane224P >= 1) {
  t0  = LANE_T0[LANE_B3(s40)] ^ LANE_T1[LANE_B2(s41)] ^ LANE_T2[LANE_B1(s42)] ^ LANE_T3[LANE_B0(s43)] ^ LANE_C[ 0+160];
  t1  = LANE_T0[LANE_B3(s41)] ^ LANE_T1[LANE_B2(s42)] ^ LANE_T2[LANE_B1(s43)] ^ LANE_T3[LANE_B0(s40)] ^ LANE_C[ 1+160];
  t4  = LANE_T0[LANE_B3(s42)] ^ LANE_T1[LANE_B2(s43)] ^ LANE_T2[LANE_B1(s40)] ^ LANE_T3[LANE_B0(s41)] ^ LANE_C[ 2+160];
  t5  = LANE_T0[LANE_B3(s43)] ^ LANE_T1[LANE_B2(s40)] ^ LANE_T2[LANE_B1(s41)] ^ LANE_T3[LANE_B0(s42)] ^ LANE_C[ 3+160] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s44)] ^ LANE_T1[LANE_B2(s45)] ^ LANE_T2[LANE_B1(s46)] ^ LANE_T3[LANE_B0(s47)] ^ LANE_C[ 4+160];
  t3  = LANE_T0[LANE_B3(s45)] ^ LANE_T1[LANE_B2(s46)] ^ LANE_T2[LANE_B1(s47)] ^ LANE_T3[LANE_B0(s44)] ^ LANE_C[ 5+160];
  t6  = LANE_T0[LANE_B3(s46)] ^ LANE_T1[LANE_B2(s47)] ^ LANE_T2[LANE_B1(s44)] ^ LANE_T3[LANE_B0(s45)] ^ LANE_C[ 6+160];
  t7  = LANE_T0[LANE_B3(s47)] ^ LANE_T1[LANE_B2(s44)] ^ LANE_T2[LANE_B1(s45)] ^ LANE_T3[LANE_B0(s46)] ^ LANE_C[ 7+160];
  }

  if (numRoundsLane224P >= 2) {
  s40 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 8+160];
  s41 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 9+160];
  s44 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[10+160];
  s45 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[11+160] ^ ctrl;
  s42 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[12+160];
  s43 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[13+160];
  s46 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[14+160];
  s47 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[15+160];
  }

  if (numRoundsLane224P >= 3) {
  t0  = LANE_T0[LANE_B3(s40)] ^ LANE_T1[LANE_B2(s41)] ^ LANE_T2[LANE_B1(s42)] ^ LANE_T3[LANE_B0(s43)] ^ LANE_C[16+160];
  t1  = LANE_T0[LANE_B3(s41)] ^ LANE_T1[LANE_B2(s42)] ^ LANE_T2[LANE_B1(s43)] ^ LANE_T3[LANE_B0(s40)] ^ LANE_C[17+160];
  t4  = LANE_T0[LANE_B3(s42)] ^ LANE_T1[LANE_B2(s43)] ^ LANE_T2[LANE_B1(s40)] ^ LANE_T3[LANE_B0(s41)] ^ LANE_C[18+160];
  t5  = LANE_T0[LANE_B3(s43)] ^ LANE_T1[LANE_B2(s40)] ^ LANE_T2[LANE_B1(s41)] ^ LANE_T3[LANE_B0(s42)] ^ LANE_C[19+160] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s44)] ^ LANE_T1[LANE_B2(s45)] ^ LANE_T2[LANE_B1(s46)] ^ LANE_T3[LANE_B0(s47)] ^ LANE_C[20+160];
  t3  = LANE_T0[LANE_B3(s45)] ^ LANE_T1[LANE_B2(s46)] ^ LANE_T2[LANE_B1(s47)] ^ LANE_T3[LANE_B0(s44)] ^ LANE_C[21+160];
  t6  = LANE_T0[LANE_B3(s46)] ^ LANE_T1[LANE_B2(s47)] ^ LANE_T2[LANE_B1(s44)] ^ LANE_T3[LANE_B0(s45)] ^ LANE_C[22+160];
  t7  = LANE_T0[LANE_B3(s47)] ^ LANE_T1[LANE_B2(s44)] ^ LANE_T2[LANE_B1(s45)] ^ LANE_T3[LANE_B0(s46)] ^ LANE_C[23+160];
  }

  if (numRoundsLane224P >= 4) {
  s40 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[24+160];
  s41 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[25+160];
  s44 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[26+160];
  s45 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[27+160] ^ ctrl;
  s42 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[28+160];
  s43 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[29+160];
  s46 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[30+160];
  s47 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[31+160];
  }

  if (numRoundsLane224P >= 5) {
  t0  = LANE_T0[LANE_B3(s40)] ^ LANE_T1[LANE_B2(s41)] ^ LANE_T2[LANE_B1(s42)] ^ LANE_T3[LANE_B0(s43)] ^ LANE_C[32+160];
  t1  = LANE_T0[LANE_B3(s41)] ^ LANE_T1[LANE_B2(s42)] ^ LANE_T2[LANE_B1(s43)] ^ LANE_T3[LANE_B0(s40)] ^ LANE_C[33+160];
  t4  = LANE_T0[LANE_B3(s42)] ^ LANE_T1[LANE_B2(s43)] ^ LANE_T2[LANE_B1(s40)] ^ LANE_T3[LANE_B0(s41)] ^ LANE_C[34+160];
  t5  = LANE_T0[LANE_B3(s43)] ^ LANE_T1[LANE_B2(s40)] ^ LANE_T2[LANE_B1(s41)] ^ LANE_T3[LANE_B0(s42)] ^ LANE_C[35+160] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s44)] ^ LANE_T1[LANE_B2(s45)] ^ LANE_T2[LANE_B1(s46)] ^ LANE_T3[LANE_B0(s47)] ^ LANE_C[36+160];
  t3  = LANE_T0[LANE_B3(s45)] ^ LANE_T1[LANE_B2(s46)] ^ LANE_T2[LANE_B1(s47)] ^ LANE_T3[LANE_B0(s44)] ^ LANE_C[37+160];
  t6  = LANE_T0[LANE_B3(s46)] ^ LANE_T1[LANE_B2(s47)] ^ LANE_T2[LANE_B1(s44)] ^ LANE_T3[LANE_B0(s45)] ^ LANE_C[38+160];
  t7  = LANE_T0[LANE_B3(s47)] ^ LANE_T1[LANE_B2(s44)] ^ LANE_T2[LANE_B1(s45)] ^ LANE_T3[LANE_B0(s46)] ^ LANE_C[39+160];
  }

  if (numRoundsLane224P >= 6) {
  s70 ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s71 ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s74 ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s75 ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s72 ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s73 ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s76 ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s77 ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  }

  /* Lane 5 */
  if (numRoundsLane224P >= 1) {
  t0  = LANE_T0[LANE_B3(s50)] ^ LANE_T1[LANE_B2(s51)] ^ LANE_T2[LANE_B1(s52)] ^ LANE_T3[LANE_B0(s53)] ^ LANE_C[ 0+200];
  t1  = LANE_T0[LANE_B3(s51)] ^ LANE_T1[LANE_B2(s52)] ^ LANE_T2[LANE_B1(s53)] ^ LANE_T3[LANE_B0(s50)] ^ LANE_C[ 1+200];
  t4  = LANE_T0[LANE_B3(s52)] ^ LANE_T1[LANE_B2(s53)] ^ LANE_T2[LANE_B1(s50)] ^ LANE_T3[LANE_B0(s51)] ^ LANE_C[ 2+200];
  t5  = LANE_T0[LANE_B3(s53)] ^ LANE_T1[LANE_B2(s50)] ^ LANE_T2[LANE_B1(s51)] ^ LANE_T3[LANE_B0(s52)] ^ LANE_C[ 3+200] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s54)] ^ LANE_T1[LANE_B2(s55)] ^ LANE_T2[LANE_B1(s56)] ^ LANE_T3[LANE_B0(s57)] ^ LANE_C[ 4+200];
  t3  = LANE_T0[LANE_B3(s55)] ^ LANE_T1[LANE_B2(s56)] ^ LANE_T2[LANE_B1(s57)] ^ LANE_T3[LANE_B0(s54)] ^ LANE_C[ 5+200];
  t6  = LANE_T0[LANE_B3(s56)] ^ LANE_T1[LANE_B2(s57)] ^ LANE_T2[LANE_B1(s54)] ^ LANE_T3[LANE_B0(s55)] ^ LANE_C[ 6+200];
  t7  = LANE_T0[LANE_B3(s57)] ^ LANE_T1[LANE_B2(s54)] ^ LANE_T2[LANE_B1(s55)] ^ LANE_T3[LANE_B0(s56)] ^ LANE_C[ 7+200];
  }

  if (numRoundsLane224P >= 2) {
  s50 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 8+200];
  s51 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 9+200];
  s54 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[10+200];
  s55 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[11+200] ^ ctrh;
  s52 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[12+200];
  s53 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[13+200];
  s56 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[14+200];
  s57 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[15+200];
  }

  if (numRoundsLane224P >= 3) {
  t0  = LANE_T0[LANE_B3(s50)] ^ LANE_T1[LANE_B2(s51)] ^ LANE_T2[LANE_B1(s52)] ^ LANE_T3[LANE_B0(s53)] ^ LANE_C[16+200];
  t1  = LANE_T0[LANE_B3(s51)] ^ LANE_T1[LANE_B2(s52)] ^ LANE_T2[LANE_B1(s53)] ^ LANE_T3[LANE_B0(s50)] ^ LANE_C[17+200];
  t4  = LANE_T0[LANE_B3(s52)] ^ LANE_T1[LANE_B2(s53)] ^ LANE_T2[LANE_B1(s50)] ^ LANE_T3[LANE_B0(s51)] ^ LANE_C[18+200];
  t5  = LANE_T0[LANE_B3(s53)] ^ LANE_T1[LANE_B2(s50)] ^ LANE_T2[LANE_B1(s51)] ^ LANE_T3[LANE_B0(s52)] ^ LANE_C[19+200] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s54)] ^ LANE_T1[LANE_B2(s55)] ^ LANE_T2[LANE_B1(s56)] ^ LANE_T3[LANE_B0(s57)] ^ LANE_C[20+200];
  t3  = LANE_T0[LANE_B3(s55)] ^ LANE_T1[LANE_B2(s56)] ^ LANE_T2[LANE_B1(s57)] ^ LANE_T3[LANE_B0(s54)] ^ LANE_C[21+200];
  t6  = LANE_T0[LANE_B3(s56)] ^ LANE_T1[LANE_B2(s57)] ^ LANE_T2[LANE_B1(s54)] ^ LANE_T3[LANE_B0(s55)] ^ LANE_C[22+200];
  t7  = LANE_T0[LANE_B3(s57)] ^ LANE_T1[LANE_B2(s54)] ^ LANE_T2[LANE_B1(s55)] ^ LANE_T3[LANE_B0(s56)] ^ LANE_C[23+200];
  }

  if (numRoundsLane224P >= 4) {
  s50 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[24+200];
  s51 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[25+200];
  s54 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[26+200];
  s55 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[27+200] ^ ctrh;
  s52 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[28+200];
  s53 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[29+200];
  s56 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[30+200];
  s57 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[31+200];
  }

  if (numRoundsLane224P >= 5) {
  t0  = LANE_T0[LANE_B3(s50)] ^ LANE_T1[LANE_B2(s51)] ^ LANE_T2[LANE_B1(s52)] ^ LANE_T3[LANE_B0(s53)] ^ LANE_C[32+200];
  t1  = LANE_T0[LANE_B3(s51)] ^ LANE_T1[LANE_B2(s52)] ^ LANE_T2[LANE_B1(s53)] ^ LANE_T3[LANE_B0(s50)] ^ LANE_C[33+200];
  t4  = LANE_T0[LANE_B3(s52)] ^ LANE_T1[LANE_B2(s53)] ^ LANE_T2[LANE_B1(s50)] ^ LANE_T3[LANE_B0(s51)] ^ LANE_C[34+200];
  t5  = LANE_T0[LANE_B3(s53)] ^ LANE_T1[LANE_B2(s50)] ^ LANE_T2[LANE_B1(s51)] ^ LANE_T3[LANE_B0(s52)] ^ LANE_C[35+200] ^ ctrl;
  t2  = LANE_T0[LANE_B3(s54)] ^ LANE_T1[LANE_B2(s55)] ^ LANE_T2[LANE_B1(s56)] ^ LANE_T3[LANE_B0(s57)] ^ LANE_C[36+200];
  t3  = LANE_T0[LANE_B3(s55)] ^ LANE_T1[LANE_B2(s56)] ^ LANE_T2[LANE_B1(s57)] ^ LANE_T3[LANE_B0(s54)] ^ LANE_C[37+200];
  t6  = LANE_T0[LANE_B3(s56)] ^ LANE_T1[LANE_B2(s57)] ^ LANE_T2[LANE_B1(s54)] ^ LANE_T3[LANE_B0(s55)] ^ LANE_C[38+200];
  t7  = LANE_T0[LANE_B3(s57)] ^ LANE_T1[LANE_B2(s54)] ^ LANE_T2[LANE_B1(s55)] ^ LANE_T3[LANE_B0(s56)] ^ LANE_C[39+200];
  }

  if (numRoundsLane224P >= 6) {
  s70 ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s71 ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s74 ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s75 ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s72 ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s73 ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s76 ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s77 ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  }

  /* Lane 6 */
  if (numRoundsLane224Q >= 1) {
  t0  = LANE_T0[LANE_B3(s60)] ^ LANE_T1[LANE_B2(s61)] ^ LANE_T2[LANE_B1(s62)] ^ LANE_T3[LANE_B0(s63)] ^ LANE_C[ 0+240];
  t1  = LANE_T0[LANE_B3(s61)] ^ LANE_T1[LANE_B2(s62)] ^ LANE_T2[LANE_B1(s63)] ^ LANE_T3[LANE_B0(s60)] ^ LANE_C[ 1+240];
  t4  = LANE_T0[LANE_B3(s62)] ^ LANE_T1[LANE_B2(s63)] ^ LANE_T2[LANE_B1(s60)] ^ LANE_T3[LANE_B0(s61)] ^ LANE_C[ 2+240];
  t5  = LANE_T0[LANE_B3(s63)] ^ LANE_T1[LANE_B2(s60)] ^ LANE_T2[LANE_B1(s61)] ^ LANE_T3[LANE_B0(s62)] ^ LANE_C[ 3+240] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s64)] ^ LANE_T1[LANE_B2(s65)] ^ LANE_T2[LANE_B1(s66)] ^ LANE_T3[LANE_B0(s67)] ^ LANE_C[ 4+240];
  t3  = LANE_T0[LANE_B3(s65)] ^ LANE_T1[LANE_B2(s66)] ^ LANE_T2[LANE_B1(s67)] ^ LANE_T3[LANE_B0(s64)] ^ LANE_C[ 5+240];
  t6  = LANE_T0[LANE_B3(s66)] ^ LANE_T1[LANE_B2(s67)] ^ LANE_T2[LANE_B1(s64)] ^ LANE_T3[LANE_B0(s65)] ^ LANE_C[ 6+240];
  t7  = LANE_T0[LANE_B3(s67)] ^ LANE_T1[LANE_B2(s64)] ^ LANE_T2[LANE_B1(s65)] ^ LANE_T3[LANE_B0(s66)] ^ LANE_C[ 7+240];
  }

  if (numRoundsLane224Q >= 2) {
  s60 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 8+240];
  s61 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 9+240];
  s64 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[10+240];
  s65 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[11+240] ^ ctrl;
  s62 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[12+240];
  s63 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[13+240];
  s66 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[14+240];
  s67 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[15+240];
  }

  if (numRoundsLane224Q >= 3) {
  h[0] = LANE_T0[LANE_B3(s60)] ^ LANE_T1[LANE_B2(s61)] ^ LANE_T2[LANE_B1(s62)] ^ LANE_T3[LANE_B0(s63)];
  h[1] = LANE_T0[LANE_B3(s61)] ^ LANE_T1[LANE_B2(s62)] ^ LANE_T2[LANE_B1(s63)] ^ LANE_T3[LANE_B0(s60)];
  h[4] = LANE_T0[LANE_B3(s62)] ^ LANE_T1[LANE_B2(s63)] ^ LANE_T2[LANE_B1(s60)] ^ LANE_T3[LANE_B0(s61)];
  h[5] = LANE_T0[LANE_B3(s63)] ^ LANE_T1[LANE_B2(s60)] ^ LANE_T2[LANE_B1(s61)] ^ LANE_T3[LANE_B0(s62)];
  h[2] = LANE_T0[LANE_B3(s64)] ^ LANE_T1[LANE_B2(s65)] ^ LANE_T2[LANE_B1(s66)] ^ LANE_T3[LANE_B0(s67)];
  h[3] = LANE_T0[LANE_B3(s65)] ^ LANE_T1[LANE_B2(s66)] ^ LANE_T2[LANE_B1(s67)] ^ LANE_T3[LANE_B0(s64)];
  h[6] = LANE_T0[LANE_B3(s66)] ^ LANE_T1[LANE_B2(s67)] ^ LANE_T2[LANE_B1(s64)] ^ LANE_T3[LANE_B0(s65)];
  h[7] = LANE_T0[LANE_B3(s67)] ^ LANE_T1[LANE_B2(s64)] ^ LANE_T2[LANE_B1(s65)] ^ LANE_T3[LANE_B0(s66)];
  }

  /* Lane 7 */
  if (numRoundsLane224Q >= 1) {
  t0  = LANE_T0[LANE_B3(s70)] ^ LANE_T1[LANE_B2(s71)] ^ LANE_T2[LANE_B1(s72)] ^ LANE_T3[LANE_B0(s73)] ^ LANE_C[ 0+256];
  t1  = LANE_T0[LANE_B3(s71)] ^ LANE_T1[LANE_B2(s72)] ^ LANE_T2[LANE_B1(s73)] ^ LANE_T3[LANE_B0(s70)] ^ LANE_C[ 1+256];
  t4  = LANE_T0[LANE_B3(s72)] ^ LANE_T1[LANE_B2(s73)] ^ LANE_T2[LANE_B1(s70)] ^ LANE_T3[LANE_B0(s71)] ^ LANE_C[ 2+256];
  t5  = LANE_T0[LANE_B3(s73)] ^ LANE_T1[LANE_B2(s70)] ^ LANE_T2[LANE_B1(s71)] ^ LANE_T3[LANE_B0(s72)] ^ LANE_C[ 3+256] ^ ctrh;
  t2  = LANE_T0[LANE_B3(s74)] ^ LANE_T1[LANE_B2(s75)] ^ LANE_T2[LANE_B1(s76)] ^ LANE_T3[LANE_B0(s77)] ^ LANE_C[ 4+256];
  t3  = LANE_T0[LANE_B3(s75)] ^ LANE_T1[LANE_B2(s76)] ^ LANE_T2[LANE_B1(s77)] ^ LANE_T3[LANE_B0(s74)] ^ LANE_C[ 5+256];
  t6  = LANE_T0[LANE_B3(s76)] ^ LANE_T1[LANE_B2(s77)] ^ LANE_T2[LANE_B1(s74)] ^ LANE_T3[LANE_B0(s75)] ^ LANE_C[ 6+256];
  t7  = LANE_T0[LANE_B3(s77)] ^ LANE_T1[LANE_B2(s74)] ^ LANE_T2[LANE_B1(s75)] ^ LANE_T3[LANE_B0(s76)] ^ LANE_C[ 7+256];
  }

  if (numRoundsLane224Q >= 2) {
  s70 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 8+256];
  s71 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 9+256];
  s74 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[10+256];
  s75 = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[11+256] ^ ctrl;
  s72 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[12+256];
  s73 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[13+256];
  s76 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[14+256];
  s77 = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[15+256];
  }

  if (numRoundsLane224Q >= 3) {
  h[0] ^= LANE_T0[LANE_B3(s70)] ^ LANE_T1[LANE_B2(s71)] ^ LANE_T2[LANE_B1(s72)] ^ LANE_T3[LANE_B0(s73)];
  h[1] ^= LANE_T0[LANE_B3(s71)] ^ LANE_T1[LANE_B2(s72)] ^ LANE_T2[LANE_B1(s73)] ^ LANE_T3[LANE_B0(s70)];
  h[4] ^= LANE_T0[LANE_B3(s72)] ^ LANE_T1[LANE_B2(s73)] ^ LANE_T2[LANE_B1(s70)] ^ LANE_T3[LANE_B0(s71)];
  h[5] ^= LANE_T0[LANE_B3(s73)] ^ LANE_T1[LANE_B2(s70)] ^ LANE_T2[LANE_B1(s71)] ^ LANE_T3[LANE_B0(s72)];
  h[2] ^= LANE_T0[LANE_B3(s74)] ^ LANE_T1[LANE_B2(s75)] ^ LANE_T2[LANE_B1(s76)] ^ LANE_T3[LANE_B0(s77)];
  h[3] ^= LANE_T0[LANE_B3(s75)] ^ LANE_T1[LANE_B2(s76)] ^ LANE_T2[LANE_B1(s77)] ^ LANE_T3[LANE_B0(s74)];
  h[6] ^= LANE_T0[LANE_B3(s76)] ^ LANE_T1[LANE_B2(s77)] ^ LANE_T2[LANE_B1(s74)] ^ LANE_T3[LANE_B0(s75)];
  h[7] ^= LANE_T0[LANE_B3(s77)] ^ LANE_T1[LANE_B2(s74)] ^ LANE_T2[LANE_B1(s75)] ^ LANE_T3[LANE_B0(s76)];
  }
}

void Lane::lane512_compress(const unsigned char m[128], unsigned int h[16], const unsigned int ctrh, const unsigned int ctrl)
{
  unsigned int t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, ta, tb, tc, td, te, tf; /* temp */
  unsigned int s00, s01, s02, s03, s04, s05, s06, s07, s08, s09, s0a, s0b, s0c, s0d, s0e, s0f; /* lane 0 */
  unsigned int s10, s11, s12, s13, s14, s15, s16, s17, s18, s19, s1a, s1b, s1c, s1d, s1e, s1f; /* lane 1 */
  unsigned int s20, s21, s22, s23, s24, s25, s26, s27, s28, s29, s2a, s2b, s2c, s2d, s2e, s2f; /* lane 2 */
  unsigned int s30, s31, s32, s33, s34, s35, s36, s37, s38, s39, s3a, s3b, s3c, s3d, s3e, s3f; /* lane 3 */
  unsigned int s40, s41, s42, s43, s44, s45, s46, s47, s48, s49, s4a, s4b, s4c, s4d, s4e, s4f; /* lane 4 */
  unsigned int s50, s51, s52, s53, s54, s55, s56, s57, s58, s59, s5a, s5b, s5c, s5d, s5e, s5f; /* lane 5 */
  unsigned int s60, s61, s62, s63, s64, s65, s66, s67, s68, s69, s6a, s6b, s6c, s6d, s6e, s6f; /* lane 6 */
  unsigned int s70, s71, s72, s73, s74, s75, s76, s77, s78, s79, s7a, s7b, s7c, s7d, s7e, s7f; /* lane 7 */

  /* Message expansion */
  s30 = h[0];
  s31 = h[1];
  s32 = h[2];
  s33 = h[3];
  s34 = h[4];
  s35 = h[5];
  s36 = h[6];
  s37 = h[7];
  s38 = h[8];
  s39 = h[9];
  s3a = h[10];
  s3b = h[11];
  s3c = h[12];
  s3d = h[13];
  s3e = h[14];
  s3f = h[15];
  s40 = LANE_U8TO32_BIG(m +  0);
  s41 = LANE_U8TO32_BIG(m +  4);
  s42 = LANE_U8TO32_BIG(m +  8);
  s43 = LANE_U8TO32_BIG(m + 12);
  s44 = LANE_U8TO32_BIG(m + 16);
  s45 = LANE_U8TO32_BIG(m + 20);
  s46 = LANE_U8TO32_BIG(m + 24);
  s47 = LANE_U8TO32_BIG(m + 28);
  s48 = LANE_U8TO32_BIG(m + 32);
  s49 = LANE_U8TO32_BIG(m + 36);
  s4a = LANE_U8TO32_BIG(m + 40);
  s4b = LANE_U8TO32_BIG(m + 44);
  s4c = LANE_U8TO32_BIG(m + 48);
  s4d = LANE_U8TO32_BIG(m + 52);
  s4e = LANE_U8TO32_BIG(m + 56);
  s4f = LANE_U8TO32_BIG(m + 60);
  s50 = LANE_U8TO32_BIG(m + 64);
  s51 = LANE_U8TO32_BIG(m + 68);
  s52 = LANE_U8TO32_BIG(m + 72);
  s53 = LANE_U8TO32_BIG(m + 76);
  s54 = LANE_U8TO32_BIG(m + 80);
  s55 = LANE_U8TO32_BIG(m + 84);
  s56 = LANE_U8TO32_BIG(m + 88);
  s57 = LANE_U8TO32_BIG(m + 92);
  s58 = LANE_U8TO32_BIG(m + 96);
  s59 = LANE_U8TO32_BIG(m + 100);
  s5a = LANE_U8TO32_BIG(m + 104);
  s5b = LANE_U8TO32_BIG(m + 108);
  s5c = LANE_U8TO32_BIG(m + 112);
  s5d = LANE_U8TO32_BIG(m + 116);
  s5e = LANE_U8TO32_BIG(m + 120);
  s5f = LANE_U8TO32_BIG(m + 124);
  s00 = s30 ^ s40 ^ s48 ^ s50 ^ s58;
  s01 = s31 ^ s41 ^ s49 ^ s51 ^ s59;
  s02 = s32 ^ s42 ^ s4a ^ s52 ^ s5a;
  s03 = s33 ^ s43 ^ s4b ^ s53 ^ s5b;
  s04 = s34 ^ s44 ^ s4c ^ s54 ^ s5c;
  s05 = s35 ^ s45 ^ s4d ^ s55 ^ s5d;
  s06 = s36 ^ s46 ^ s4e ^ s56 ^ s5e;
  s07 = s37 ^ s47 ^ s4f ^ s57 ^ s5f;
  s08 = s38 ^ s40 ^ s50;
  s09 = s39 ^ s41 ^ s51;
  s0a = s3a ^ s42 ^ s52;
  s0b = s3b ^ s43 ^ s53;
  s0c = s3c ^ s44 ^ s54;
  s0d = s3d ^ s45 ^ s55;
  s0e = s3e ^ s46 ^ s56;
  s0f = s3f ^ s47 ^ s57;
  s10 = s00 ^ s38 ^ s48;
  s11 = s01 ^ s39 ^ s49;
  s12 = s02 ^ s3a ^ s4a;
  s13 = s03 ^ s3b ^ s4b;
  s14 = s04 ^ s3c ^ s4c;
  s15 = s05 ^ s3d ^ s4d;
  s16 = s06 ^ s3e ^ s4e;
  s17 = s07 ^ s3f ^ s4f;
  s18 = s30 ^ s48 ^ s50;
  s19 = s31 ^ s49 ^ s51;
  s1a = s32 ^ s4a ^ s52;
  s1b = s33 ^ s4b ^ s53;
  s1c = s34 ^ s4c ^ s54;
  s1d = s35 ^ s4d ^ s55;
  s1e = s36 ^ s4e ^ s56;
  s1f = s37 ^ s4f ^ s57;
  s20 = s00 ^ s38 ^ s58;
  s21 = s01 ^ s39 ^ s59;
  s22 = s02 ^ s3a ^ s5a;
  s23 = s03 ^ s3b ^ s5b;
  s24 = s04 ^ s3c ^ s5c;
  s25 = s05 ^ s3d ^ s5d;
  s26 = s06 ^ s3e ^ s5e;
  s27 = s07 ^ s3f ^ s5f;
  s28 = s30 ^ s40 ^ s58;
  s29 = s31 ^ s41 ^ s59;
  s2a = s32 ^ s42 ^ s5a;
  s2b = s33 ^ s43 ^ s5b;
  s2c = s34 ^ s44 ^ s5c;
  s2d = s35 ^ s45 ^ s5d;
  s2e = s36 ^ s46 ^ s5e;
  s2f = s37 ^ s47 ^ s5f;

  /* Lane 0 */
  t0  = LANE_T0[LANE_B3(s00)] ^ LANE_T1[LANE_B2(s01)] ^ LANE_T2[LANE_B1(s02)] ^ LANE_T3[LANE_B0(s03)] ^ LANE_C[  0];
  t4  = LANE_T0[LANE_B3(s01)] ^ LANE_T1[LANE_B2(s02)] ^ LANE_T2[LANE_B1(s03)] ^ LANE_T3[LANE_B0(s00)] ^ LANE_C[  1];
  t8  = LANE_T0[LANE_B3(s02)] ^ LANE_T1[LANE_B2(s03)] ^ LANE_T2[LANE_B1(s00)] ^ LANE_T3[LANE_B0(s01)] ^ LANE_C[  2];
  tc  = LANE_T0[LANE_B3(s03)] ^ LANE_T1[LANE_B2(s00)] ^ LANE_T2[LANE_B1(s01)] ^ LANE_T3[LANE_B0(s02)] ^ LANE_C[  3] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s04)] ^ LANE_T1[LANE_B2(s05)] ^ LANE_T2[LANE_B1(s06)] ^ LANE_T3[LANE_B0(s07)] ^ LANE_C[  4];
  t5  = LANE_T0[LANE_B3(s05)] ^ LANE_T1[LANE_B2(s06)] ^ LANE_T2[LANE_B1(s07)] ^ LANE_T3[LANE_B0(s04)] ^ LANE_C[  5];
  t9  = LANE_T0[LANE_B3(s06)] ^ LANE_T1[LANE_B2(s07)] ^ LANE_T2[LANE_B1(s04)] ^ LANE_T3[LANE_B0(s05)] ^ LANE_C[  6];
  td  = LANE_T0[LANE_B3(s07)] ^ LANE_T1[LANE_B2(s04)] ^ LANE_T2[LANE_B1(s05)] ^ LANE_T3[LANE_B0(s06)] ^ LANE_C[  7];
  t2  = LANE_T0[LANE_B3(s08)] ^ LANE_T1[LANE_B2(s09)] ^ LANE_T2[LANE_B1(s0a)] ^ LANE_T3[LANE_B0(s0b)] ^ LANE_C[  8];
  t6  = LANE_T0[LANE_B3(s09)] ^ LANE_T1[LANE_B2(s0a)] ^ LANE_T2[LANE_B1(s0b)] ^ LANE_T3[LANE_B0(s08)] ^ LANE_C[  9];
  ta  = LANE_T0[LANE_B3(s0a)] ^ LANE_T1[LANE_B2(s0b)] ^ LANE_T2[LANE_B1(s08)] ^ LANE_T3[LANE_B0(s09)] ^ LANE_C[ 10];
  te  = LANE_T0[LANE_B3(s0b)] ^ LANE_T1[LANE_B2(s08)] ^ LANE_T2[LANE_B1(s09)] ^ LANE_T3[LANE_B0(s0a)] ^ LANE_C[ 11];
  t3  = LANE_T0[LANE_B3(s0c)] ^ LANE_T1[LANE_B2(s0d)] ^ LANE_T2[LANE_B1(s0e)] ^ LANE_T3[LANE_B0(s0f)] ^ LANE_C[ 12];
  t7  = LANE_T0[LANE_B3(s0d)] ^ LANE_T1[LANE_B2(s0e)] ^ LANE_T2[LANE_B1(s0f)] ^ LANE_T3[LANE_B0(s0c)] ^ LANE_C[ 13];
  tb  = LANE_T0[LANE_B3(s0e)] ^ LANE_T1[LANE_B2(s0f)] ^ LANE_T2[LANE_B1(s0c)] ^ LANE_T3[LANE_B0(s0d)] ^ LANE_C[ 14];
  tf  = LANE_T0[LANE_B3(s0f)] ^ LANE_T1[LANE_B2(s0c)] ^ LANE_T2[LANE_B1(s0d)] ^ LANE_T3[LANE_B0(s0e)] ^ LANE_C[ 15];

  s00 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 16];
  s04 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 17];
  s08 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 18];
  s0c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 19] ^ ctrl;
  s01 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 20];
  s05 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 21];
  s09 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 22];
  s0d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 23];
  s02 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 24];
  s06 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 25];
  s0a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 26];
  s0e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 27];
  s03 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 28];
  s07 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 29];
  s0b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 30];
  s0f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 31];

  t0  = LANE_T0[LANE_B3(s00)] ^ LANE_T1[LANE_B2(s01)] ^ LANE_T2[LANE_B1(s02)] ^ LANE_T3[LANE_B0(s03)] ^ LANE_C[ 32];
  t4  = LANE_T0[LANE_B3(s01)] ^ LANE_T1[LANE_B2(s02)] ^ LANE_T2[LANE_B1(s03)] ^ LANE_T3[LANE_B0(s00)] ^ LANE_C[ 33];
  t8  = LANE_T0[LANE_B3(s02)] ^ LANE_T1[LANE_B2(s03)] ^ LANE_T2[LANE_B1(s00)] ^ LANE_T3[LANE_B0(s01)] ^ LANE_C[ 34];
  tc  = LANE_T0[LANE_B3(s03)] ^ LANE_T1[LANE_B2(s00)] ^ LANE_T2[LANE_B1(s01)] ^ LANE_T3[LANE_B0(s02)] ^ LANE_C[ 35] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s04)] ^ LANE_T1[LANE_B2(s05)] ^ LANE_T2[LANE_B1(s06)] ^ LANE_T3[LANE_B0(s07)] ^ LANE_C[ 36];
  t5  = LANE_T0[LANE_B3(s05)] ^ LANE_T1[LANE_B2(s06)] ^ LANE_T2[LANE_B1(s07)] ^ LANE_T3[LANE_B0(s04)] ^ LANE_C[ 37];
  t9  = LANE_T0[LANE_B3(s06)] ^ LANE_T1[LANE_B2(s07)] ^ LANE_T2[LANE_B1(s04)] ^ LANE_T3[LANE_B0(s05)] ^ LANE_C[ 38];
  td  = LANE_T0[LANE_B3(s07)] ^ LANE_T1[LANE_B2(s04)] ^ LANE_T2[LANE_B1(s05)] ^ LANE_T3[LANE_B0(s06)] ^ LANE_C[ 39];
  t2  = LANE_T0[LANE_B3(s08)] ^ LANE_T1[LANE_B2(s09)] ^ LANE_T2[LANE_B1(s0a)] ^ LANE_T3[LANE_B0(s0b)] ^ LANE_C[ 40];
  t6  = LANE_T0[LANE_B3(s09)] ^ LANE_T1[LANE_B2(s0a)] ^ LANE_T2[LANE_B1(s0b)] ^ LANE_T3[LANE_B0(s08)] ^ LANE_C[ 41];
  ta  = LANE_T0[LANE_B3(s0a)] ^ LANE_T1[LANE_B2(s0b)] ^ LANE_T2[LANE_B1(s08)] ^ LANE_T3[LANE_B0(s09)] ^ LANE_C[ 42];
  te  = LANE_T0[LANE_B3(s0b)] ^ LANE_T1[LANE_B2(s08)] ^ LANE_T2[LANE_B1(s09)] ^ LANE_T3[LANE_B0(s0a)] ^ LANE_C[ 43];
  t3  = LANE_T0[LANE_B3(s0c)] ^ LANE_T1[LANE_B2(s0d)] ^ LANE_T2[LANE_B1(s0e)] ^ LANE_T3[LANE_B0(s0f)] ^ LANE_C[ 44];
  t7  = LANE_T0[LANE_B3(s0d)] ^ LANE_T1[LANE_B2(s0e)] ^ LANE_T2[LANE_B1(s0f)] ^ LANE_T3[LANE_B0(s0c)] ^ LANE_C[ 45];
  tb  = LANE_T0[LANE_B3(s0e)] ^ LANE_T1[LANE_B2(s0f)] ^ LANE_T2[LANE_B1(s0c)] ^ LANE_T3[LANE_B0(s0d)] ^ LANE_C[ 46];
  tf  = LANE_T0[LANE_B3(s0f)] ^ LANE_T1[LANE_B2(s0c)] ^ LANE_T2[LANE_B1(s0d)] ^ LANE_T3[LANE_B0(s0e)] ^ LANE_C[ 47];

  s00 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 48];
  s04 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 49];
  s08 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 50];
  s0c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 51] ^ ctrl;
  s01 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 52];
  s05 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 53];
  s09 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 54];
  s0d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 55];
  s02 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 56];
  s06 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 57];
  s0a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 58];
  s0e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 59];
  s03 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 60];
  s07 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 61];
  s0b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 62];
  s0f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 63];

  t0  = LANE_T0[LANE_B3(s00)] ^ LANE_T1[LANE_B2(s01)] ^ LANE_T2[LANE_B1(s02)] ^ LANE_T3[LANE_B0(s03)] ^ LANE_C[ 64];
  t4  = LANE_T0[LANE_B3(s01)] ^ LANE_T1[LANE_B2(s02)] ^ LANE_T2[LANE_B1(s03)] ^ LANE_T3[LANE_B0(s00)] ^ LANE_C[ 65];
  t8  = LANE_T0[LANE_B3(s02)] ^ LANE_T1[LANE_B2(s03)] ^ LANE_T2[LANE_B1(s00)] ^ LANE_T3[LANE_B0(s01)] ^ LANE_C[ 66];
  tc  = LANE_T0[LANE_B3(s03)] ^ LANE_T1[LANE_B2(s00)] ^ LANE_T2[LANE_B1(s01)] ^ LANE_T3[LANE_B0(s02)] ^ LANE_C[ 67] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s04)] ^ LANE_T1[LANE_B2(s05)] ^ LANE_T2[LANE_B1(s06)] ^ LANE_T3[LANE_B0(s07)] ^ LANE_C[ 68];
  t5  = LANE_T0[LANE_B3(s05)] ^ LANE_T1[LANE_B2(s06)] ^ LANE_T2[LANE_B1(s07)] ^ LANE_T3[LANE_B0(s04)] ^ LANE_C[ 69];
  t9  = LANE_T0[LANE_B3(s06)] ^ LANE_T1[LANE_B2(s07)] ^ LANE_T2[LANE_B1(s04)] ^ LANE_T3[LANE_B0(s05)] ^ LANE_C[ 70];
  td  = LANE_T0[LANE_B3(s07)] ^ LANE_T1[LANE_B2(s04)] ^ LANE_T2[LANE_B1(s05)] ^ LANE_T3[LANE_B0(s06)] ^ LANE_C[ 71];
  t2  = LANE_T0[LANE_B3(s08)] ^ LANE_T1[LANE_B2(s09)] ^ LANE_T2[LANE_B1(s0a)] ^ LANE_T3[LANE_B0(s0b)] ^ LANE_C[ 72];
  t6  = LANE_T0[LANE_B3(s09)] ^ LANE_T1[LANE_B2(s0a)] ^ LANE_T2[LANE_B1(s0b)] ^ LANE_T3[LANE_B0(s08)] ^ LANE_C[ 73];
  ta  = LANE_T0[LANE_B3(s0a)] ^ LANE_T1[LANE_B2(s0b)] ^ LANE_T2[LANE_B1(s08)] ^ LANE_T3[LANE_B0(s09)] ^ LANE_C[ 74];
  te  = LANE_T0[LANE_B3(s0b)] ^ LANE_T1[LANE_B2(s08)] ^ LANE_T2[LANE_B1(s09)] ^ LANE_T3[LANE_B0(s0a)] ^ LANE_C[ 75];
  t3  = LANE_T0[LANE_B3(s0c)] ^ LANE_T1[LANE_B2(s0d)] ^ LANE_T2[LANE_B1(s0e)] ^ LANE_T3[LANE_B0(s0f)] ^ LANE_C[ 76];
  t7  = LANE_T0[LANE_B3(s0d)] ^ LANE_T1[LANE_B2(s0e)] ^ LANE_T2[LANE_B1(s0f)] ^ LANE_T3[LANE_B0(s0c)] ^ LANE_C[ 77];
  tb  = LANE_T0[LANE_B3(s0e)] ^ LANE_T1[LANE_B2(s0f)] ^ LANE_T2[LANE_B1(s0c)] ^ LANE_T3[LANE_B0(s0d)] ^ LANE_C[ 78];
  tf  = LANE_T0[LANE_B3(s0f)] ^ LANE_T1[LANE_B2(s0c)] ^ LANE_T2[LANE_B1(s0d)] ^ LANE_T3[LANE_B0(s0e)] ^ LANE_C[ 79];

  s00 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 80];
  s04 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 81];
  s08 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 82];
  s0c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 83] ^ ctrl;
  s01 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 84];
  s05 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 85];
  s09 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 86];
  s0d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 87];
  s02 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 88];
  s06 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 89];
  s0a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 90];
  s0e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 91];
  s03 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 92];
  s07 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 93];
  s0b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 94];
  s0f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 95];

  t0  = LANE_T0[LANE_B3(s00)] ^ LANE_T1[LANE_B2(s01)] ^ LANE_T2[LANE_B1(s02)] ^ LANE_T3[LANE_B0(s03)] ^ LANE_C[ 96];
  t4  = LANE_T0[LANE_B3(s01)] ^ LANE_T1[LANE_B2(s02)] ^ LANE_T2[LANE_B1(s03)] ^ LANE_T3[LANE_B0(s00)] ^ LANE_C[ 97];
  t8  = LANE_T0[LANE_B3(s02)] ^ LANE_T1[LANE_B2(s03)] ^ LANE_T2[LANE_B1(s00)] ^ LANE_T3[LANE_B0(s01)] ^ LANE_C[ 98];
  tc  = LANE_T0[LANE_B3(s03)] ^ LANE_T1[LANE_B2(s00)] ^ LANE_T2[LANE_B1(s01)] ^ LANE_T3[LANE_B0(s02)] ^ LANE_C[ 99] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s04)] ^ LANE_T1[LANE_B2(s05)] ^ LANE_T2[LANE_B1(s06)] ^ LANE_T3[LANE_B0(s07)] ^ LANE_C[100];
  t5  = LANE_T0[LANE_B3(s05)] ^ LANE_T1[LANE_B2(s06)] ^ LANE_T2[LANE_B1(s07)] ^ LANE_T3[LANE_B0(s04)] ^ LANE_C[101];
  t9  = LANE_T0[LANE_B3(s06)] ^ LANE_T1[LANE_B2(s07)] ^ LANE_T2[LANE_B1(s04)] ^ LANE_T3[LANE_B0(s05)] ^ LANE_C[102];
  td  = LANE_T0[LANE_B3(s07)] ^ LANE_T1[LANE_B2(s04)] ^ LANE_T2[LANE_B1(s05)] ^ LANE_T3[LANE_B0(s06)] ^ LANE_C[103];
  t2  = LANE_T0[LANE_B3(s08)] ^ LANE_T1[LANE_B2(s09)] ^ LANE_T2[LANE_B1(s0a)] ^ LANE_T3[LANE_B0(s0b)] ^ LANE_C[104];
  t6  = LANE_T0[LANE_B3(s09)] ^ LANE_T1[LANE_B2(s0a)] ^ LANE_T2[LANE_B1(s0b)] ^ LANE_T3[LANE_B0(s08)] ^ LANE_C[105];
  ta  = LANE_T0[LANE_B3(s0a)] ^ LANE_T1[LANE_B2(s0b)] ^ LANE_T2[LANE_B1(s08)] ^ LANE_T3[LANE_B0(s09)] ^ LANE_C[106];
  te  = LANE_T0[LANE_B3(s0b)] ^ LANE_T1[LANE_B2(s08)] ^ LANE_T2[LANE_B1(s09)] ^ LANE_T3[LANE_B0(s0a)] ^ LANE_C[107];
  t3  = LANE_T0[LANE_B3(s0c)] ^ LANE_T1[LANE_B2(s0d)] ^ LANE_T2[LANE_B1(s0e)] ^ LANE_T3[LANE_B0(s0f)] ^ LANE_C[108];
  t7  = LANE_T0[LANE_B3(s0d)] ^ LANE_T1[LANE_B2(s0e)] ^ LANE_T2[LANE_B1(s0f)] ^ LANE_T3[LANE_B0(s0c)] ^ LANE_C[109];
  tb  = LANE_T0[LANE_B3(s0e)] ^ LANE_T1[LANE_B2(s0f)] ^ LANE_T2[LANE_B1(s0c)] ^ LANE_T3[LANE_B0(s0d)] ^ LANE_C[110];
  tf  = LANE_T0[LANE_B3(s0f)] ^ LANE_T1[LANE_B2(s0c)] ^ LANE_T2[LANE_B1(s0d)] ^ LANE_T3[LANE_B0(s0e)] ^ LANE_C[111];

  s60 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s64 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s68 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s6c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s61 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s65 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s69 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s6d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  s62 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )];
  s66 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )];
  s6a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )];
  s6e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )];
  s63 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )];
  s67 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )];
  s6b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )];
  s6f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )];

  /* Lane 1 */
  t0  = LANE_T0[LANE_B3(s10)] ^ LANE_T1[LANE_B2(s11)] ^ LANE_T2[LANE_B1(s12)] ^ LANE_T3[LANE_B0(s13)] ^ LANE_C[  0+112];
  t4  = LANE_T0[LANE_B3(s11)] ^ LANE_T1[LANE_B2(s12)] ^ LANE_T2[LANE_B1(s13)] ^ LANE_T3[LANE_B0(s10)] ^ LANE_C[  1+112];
  t8  = LANE_T0[LANE_B3(s12)] ^ LANE_T1[LANE_B2(s13)] ^ LANE_T2[LANE_B1(s10)] ^ LANE_T3[LANE_B0(s11)] ^ LANE_C[  2+112];
  tc  = LANE_T0[LANE_B3(s13)] ^ LANE_T1[LANE_B2(s10)] ^ LANE_T2[LANE_B1(s11)] ^ LANE_T3[LANE_B0(s12)] ^ LANE_C[  3+112] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s14)] ^ LANE_T1[LANE_B2(s15)] ^ LANE_T2[LANE_B1(s16)] ^ LANE_T3[LANE_B0(s17)] ^ LANE_C[  4+112];
  t5  = LANE_T0[LANE_B3(s15)] ^ LANE_T1[LANE_B2(s16)] ^ LANE_T2[LANE_B1(s17)] ^ LANE_T3[LANE_B0(s14)] ^ LANE_C[  5+112];
  t9  = LANE_T0[LANE_B3(s16)] ^ LANE_T1[LANE_B2(s17)] ^ LANE_T2[LANE_B1(s14)] ^ LANE_T3[LANE_B0(s15)] ^ LANE_C[  6+112];
  td  = LANE_T0[LANE_B3(s17)] ^ LANE_T1[LANE_B2(s14)] ^ LANE_T2[LANE_B1(s15)] ^ LANE_T3[LANE_B0(s16)] ^ LANE_C[  7+112];
  t2  = LANE_T0[LANE_B3(s18)] ^ LANE_T1[LANE_B2(s19)] ^ LANE_T2[LANE_B1(s1a)] ^ LANE_T3[LANE_B0(s1b)] ^ LANE_C[  8+112];
  t6  = LANE_T0[LANE_B3(s19)] ^ LANE_T1[LANE_B2(s1a)] ^ LANE_T2[LANE_B1(s1b)] ^ LANE_T3[LANE_B0(s18)] ^ LANE_C[  9+112];
  ta  = LANE_T0[LANE_B3(s1a)] ^ LANE_T1[LANE_B2(s1b)] ^ LANE_T2[LANE_B1(s18)] ^ LANE_T3[LANE_B0(s19)] ^ LANE_C[ 10+112];
  te  = LANE_T0[LANE_B3(s1b)] ^ LANE_T1[LANE_B2(s18)] ^ LANE_T2[LANE_B1(s19)] ^ LANE_T3[LANE_B0(s1a)] ^ LANE_C[ 11+112];
  t3  = LANE_T0[LANE_B3(s1c)] ^ LANE_T1[LANE_B2(s1d)] ^ LANE_T2[LANE_B1(s1e)] ^ LANE_T3[LANE_B0(s1f)] ^ LANE_C[ 12+112];
  t7  = LANE_T0[LANE_B3(s1d)] ^ LANE_T1[LANE_B2(s1e)] ^ LANE_T2[LANE_B1(s1f)] ^ LANE_T3[LANE_B0(s1c)] ^ LANE_C[ 13+112];
  tb  = LANE_T0[LANE_B3(s1e)] ^ LANE_T1[LANE_B2(s1f)] ^ LANE_T2[LANE_B1(s1c)] ^ LANE_T3[LANE_B0(s1d)] ^ LANE_C[ 14+112];
  tf  = LANE_T0[LANE_B3(s1f)] ^ LANE_T1[LANE_B2(s1c)] ^ LANE_T2[LANE_B1(s1d)] ^ LANE_T3[LANE_B0(s1e)] ^ LANE_C[ 15+112];

  s10 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 16+112];
  s14 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 17+112];
  s18 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 18+112];
  s1c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 19+112] ^ ctrh;
  s11 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 20+112];
  s15 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 21+112];
  s19 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 22+112];
  s1d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 23+112];
  s12 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 24+112];
  s16 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 25+112];
  s1a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 26+112];
  s1e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 27+112];
  s13 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 28+112];
  s17 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 29+112];
  s1b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 30+112];
  s1f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 31+112];

  t0  = LANE_T0[LANE_B3(s10)] ^ LANE_T1[LANE_B2(s11)] ^ LANE_T2[LANE_B1(s12)] ^ LANE_T3[LANE_B0(s13)] ^ LANE_C[ 32+112];
  t4  = LANE_T0[LANE_B3(s11)] ^ LANE_T1[LANE_B2(s12)] ^ LANE_T2[LANE_B1(s13)] ^ LANE_T3[LANE_B0(s10)] ^ LANE_C[ 33+112];
  t8  = LANE_T0[LANE_B3(s12)] ^ LANE_T1[LANE_B2(s13)] ^ LANE_T2[LANE_B1(s10)] ^ LANE_T3[LANE_B0(s11)] ^ LANE_C[ 34+112];
  tc  = LANE_T0[LANE_B3(s13)] ^ LANE_T1[LANE_B2(s10)] ^ LANE_T2[LANE_B1(s11)] ^ LANE_T3[LANE_B0(s12)] ^ LANE_C[ 35+112] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s14)] ^ LANE_T1[LANE_B2(s15)] ^ LANE_T2[LANE_B1(s16)] ^ LANE_T3[LANE_B0(s17)] ^ LANE_C[ 36+112];
  t5  = LANE_T0[LANE_B3(s15)] ^ LANE_T1[LANE_B2(s16)] ^ LANE_T2[LANE_B1(s17)] ^ LANE_T3[LANE_B0(s14)] ^ LANE_C[ 37+112];
  t9  = LANE_T0[LANE_B3(s16)] ^ LANE_T1[LANE_B2(s17)] ^ LANE_T2[LANE_B1(s14)] ^ LANE_T3[LANE_B0(s15)] ^ LANE_C[ 38+112];
  td  = LANE_T0[LANE_B3(s17)] ^ LANE_T1[LANE_B2(s14)] ^ LANE_T2[LANE_B1(s15)] ^ LANE_T3[LANE_B0(s16)] ^ LANE_C[ 39+112];
  t2  = LANE_T0[LANE_B3(s18)] ^ LANE_T1[LANE_B2(s19)] ^ LANE_T2[LANE_B1(s1a)] ^ LANE_T3[LANE_B0(s1b)] ^ LANE_C[ 40+112];
  t6  = LANE_T0[LANE_B3(s19)] ^ LANE_T1[LANE_B2(s1a)] ^ LANE_T2[LANE_B1(s1b)] ^ LANE_T3[LANE_B0(s18)] ^ LANE_C[ 41+112];
  ta  = LANE_T0[LANE_B3(s1a)] ^ LANE_T1[LANE_B2(s1b)] ^ LANE_T2[LANE_B1(s18)] ^ LANE_T3[LANE_B0(s19)] ^ LANE_C[ 42+112];
  te  = LANE_T0[LANE_B3(s1b)] ^ LANE_T1[LANE_B2(s18)] ^ LANE_T2[LANE_B1(s19)] ^ LANE_T3[LANE_B0(s1a)] ^ LANE_C[ 43+112];
  t3  = LANE_T0[LANE_B3(s1c)] ^ LANE_T1[LANE_B2(s1d)] ^ LANE_T2[LANE_B1(s1e)] ^ LANE_T3[LANE_B0(s1f)] ^ LANE_C[ 44+112];
  t7  = LANE_T0[LANE_B3(s1d)] ^ LANE_T1[LANE_B2(s1e)] ^ LANE_T2[LANE_B1(s1f)] ^ LANE_T3[LANE_B0(s1c)] ^ LANE_C[ 45+112];
  tb  = LANE_T0[LANE_B3(s1e)] ^ LANE_T1[LANE_B2(s1f)] ^ LANE_T2[LANE_B1(s1c)] ^ LANE_T3[LANE_B0(s1d)] ^ LANE_C[ 46+112];
  tf  = LANE_T0[LANE_B3(s1f)] ^ LANE_T1[LANE_B2(s1c)] ^ LANE_T2[LANE_B1(s1d)] ^ LANE_T3[LANE_B0(s1e)] ^ LANE_C[ 47+112];

  s10 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 48+112];
  s14 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 49+112];
  s18 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 50+112];
  s1c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 51+112] ^ ctrh;
  s11 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 52+112];
  s15 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 53+112];
  s19 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 54+112];
  s1d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 55+112];
  s12 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 56+112];
  s16 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 57+112];
  s1a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 58+112];
  s1e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 59+112];
  s13 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 60+112];
  s17 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 61+112];
  s1b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 62+112];
  s1f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 63+112];

  t0  = LANE_T0[LANE_B3(s10)] ^ LANE_T1[LANE_B2(s11)] ^ LANE_T2[LANE_B1(s12)] ^ LANE_T3[LANE_B0(s13)] ^ LANE_C[ 64+112];
  t4  = LANE_T0[LANE_B3(s11)] ^ LANE_T1[LANE_B2(s12)] ^ LANE_T2[LANE_B1(s13)] ^ LANE_T3[LANE_B0(s10)] ^ LANE_C[ 65+112];
  t8  = LANE_T0[LANE_B3(s12)] ^ LANE_T1[LANE_B2(s13)] ^ LANE_T2[LANE_B1(s10)] ^ LANE_T3[LANE_B0(s11)] ^ LANE_C[ 66+112];
  tc  = LANE_T0[LANE_B3(s13)] ^ LANE_T1[LANE_B2(s10)] ^ LANE_T2[LANE_B1(s11)] ^ LANE_T3[LANE_B0(s12)] ^ LANE_C[ 67+112] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s14)] ^ LANE_T1[LANE_B2(s15)] ^ LANE_T2[LANE_B1(s16)] ^ LANE_T3[LANE_B0(s17)] ^ LANE_C[ 68+112];
  t5  = LANE_T0[LANE_B3(s15)] ^ LANE_T1[LANE_B2(s16)] ^ LANE_T2[LANE_B1(s17)] ^ LANE_T3[LANE_B0(s14)] ^ LANE_C[ 69+112];
  t9  = LANE_T0[LANE_B3(s16)] ^ LANE_T1[LANE_B2(s17)] ^ LANE_T2[LANE_B1(s14)] ^ LANE_T3[LANE_B0(s15)] ^ LANE_C[ 70+112];
  td  = LANE_T0[LANE_B3(s17)] ^ LANE_T1[LANE_B2(s14)] ^ LANE_T2[LANE_B1(s15)] ^ LANE_T3[LANE_B0(s16)] ^ LANE_C[ 71+112];
  t2  = LANE_T0[LANE_B3(s18)] ^ LANE_T1[LANE_B2(s19)] ^ LANE_T2[LANE_B1(s1a)] ^ LANE_T3[LANE_B0(s1b)] ^ LANE_C[ 72+112];
  t6  = LANE_T0[LANE_B3(s19)] ^ LANE_T1[LANE_B2(s1a)] ^ LANE_T2[LANE_B1(s1b)] ^ LANE_T3[LANE_B0(s18)] ^ LANE_C[ 73+112];
  ta  = LANE_T0[LANE_B3(s1a)] ^ LANE_T1[LANE_B2(s1b)] ^ LANE_T2[LANE_B1(s18)] ^ LANE_T3[LANE_B0(s19)] ^ LANE_C[ 74+112];
  te  = LANE_T0[LANE_B3(s1b)] ^ LANE_T1[LANE_B2(s18)] ^ LANE_T2[LANE_B1(s19)] ^ LANE_T3[LANE_B0(s1a)] ^ LANE_C[ 75+112];
  t3  = LANE_T0[LANE_B3(s1c)] ^ LANE_T1[LANE_B2(s1d)] ^ LANE_T2[LANE_B1(s1e)] ^ LANE_T3[LANE_B0(s1f)] ^ LANE_C[ 76+112];
  t7  = LANE_T0[LANE_B3(s1d)] ^ LANE_T1[LANE_B2(s1e)] ^ LANE_T2[LANE_B1(s1f)] ^ LANE_T3[LANE_B0(s1c)] ^ LANE_C[ 77+112];
  tb  = LANE_T0[LANE_B3(s1e)] ^ LANE_T1[LANE_B2(s1f)] ^ LANE_T2[LANE_B1(s1c)] ^ LANE_T3[LANE_B0(s1d)] ^ LANE_C[ 78+112];
  tf  = LANE_T0[LANE_B3(s1f)] ^ LANE_T1[LANE_B2(s1c)] ^ LANE_T2[LANE_B1(s1d)] ^ LANE_T3[LANE_B0(s1e)] ^ LANE_C[ 79+112];

  s10 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 80+112];
  s14 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 81+112];
  s18 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 82+112];
  s1c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 83+112] ^ ctrh;
  s11 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 84+112];
  s15 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 85+112];
  s19 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 86+112];
  s1d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 87+112];
  s12 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 88+112];
  s16 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 89+112];
  s1a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 90+112];
  s1e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 91+112];
  s13 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 92+112];
  s17 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 93+112];
  s1b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 94+112];
  s1f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 95+112];

  t0  = LANE_T0[LANE_B3(s10)] ^ LANE_T1[LANE_B2(s11)] ^ LANE_T2[LANE_B1(s12)] ^ LANE_T3[LANE_B0(s13)] ^ LANE_C[ 96+112];
  t4  = LANE_T0[LANE_B3(s11)] ^ LANE_T1[LANE_B2(s12)] ^ LANE_T2[LANE_B1(s13)] ^ LANE_T3[LANE_B0(s10)] ^ LANE_C[ 97+112];
  t8  = LANE_T0[LANE_B3(s12)] ^ LANE_T1[LANE_B2(s13)] ^ LANE_T2[LANE_B1(s10)] ^ LANE_T3[LANE_B0(s11)] ^ LANE_C[ 98+112];
  tc  = LANE_T0[LANE_B3(s13)] ^ LANE_T1[LANE_B2(s10)] ^ LANE_T2[LANE_B1(s11)] ^ LANE_T3[LANE_B0(s12)] ^ LANE_C[ 99+112] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s14)] ^ LANE_T1[LANE_B2(s15)] ^ LANE_T2[LANE_B1(s16)] ^ LANE_T3[LANE_B0(s17)] ^ LANE_C[100+112];
  t5  = LANE_T0[LANE_B3(s15)] ^ LANE_T1[LANE_B2(s16)] ^ LANE_T2[LANE_B1(s17)] ^ LANE_T3[LANE_B0(s14)] ^ LANE_C[101+112];
  t9  = LANE_T0[LANE_B3(s16)] ^ LANE_T1[LANE_B2(s17)] ^ LANE_T2[LANE_B1(s14)] ^ LANE_T3[LANE_B0(s15)] ^ LANE_C[102+112];
  td  = LANE_T0[LANE_B3(s17)] ^ LANE_T1[LANE_B2(s14)] ^ LANE_T2[LANE_B1(s15)] ^ LANE_T3[LANE_B0(s16)] ^ LANE_C[103+112];
  t2  = LANE_T0[LANE_B3(s18)] ^ LANE_T1[LANE_B2(s19)] ^ LANE_T2[LANE_B1(s1a)] ^ LANE_T3[LANE_B0(s1b)] ^ LANE_C[104+112];
  t6  = LANE_T0[LANE_B3(s19)] ^ LANE_T1[LANE_B2(s1a)] ^ LANE_T2[LANE_B1(s1b)] ^ LANE_T3[LANE_B0(s18)] ^ LANE_C[105+112];
  ta  = LANE_T0[LANE_B3(s1a)] ^ LANE_T1[LANE_B2(s1b)] ^ LANE_T2[LANE_B1(s18)] ^ LANE_T3[LANE_B0(s19)] ^ LANE_C[106+112];
  te  = LANE_T0[LANE_B3(s1b)] ^ LANE_T1[LANE_B2(s18)] ^ LANE_T2[LANE_B1(s19)] ^ LANE_T3[LANE_B0(s1a)] ^ LANE_C[107+112];
  t3  = LANE_T0[LANE_B3(s1c)] ^ LANE_T1[LANE_B2(s1d)] ^ LANE_T2[LANE_B1(s1e)] ^ LANE_T3[LANE_B0(s1f)] ^ LANE_C[108+112];
  t7  = LANE_T0[LANE_B3(s1d)] ^ LANE_T1[LANE_B2(s1e)] ^ LANE_T2[LANE_B1(s1f)] ^ LANE_T3[LANE_B0(s1c)] ^ LANE_C[109+112];
  tb  = LANE_T0[LANE_B3(s1e)] ^ LANE_T1[LANE_B2(s1f)] ^ LANE_T2[LANE_B1(s1c)] ^ LANE_T3[LANE_B0(s1d)] ^ LANE_C[110+112];
  tf  = LANE_T0[LANE_B3(s1f)] ^ LANE_T1[LANE_B2(s1c)] ^ LANE_T2[LANE_B1(s1d)] ^ LANE_T3[LANE_B0(s1e)] ^ LANE_C[111+112];

  s60 ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s64 ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s68 ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s6c ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s61 ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s65 ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s69 ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s6d ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  s62 ^= LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )];
  s66 ^= LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )];
  s6a ^= LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )];
  s6e ^= LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )];
  s63 ^= LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )];
  s67 ^= LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )];
  s6b ^= LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )];
  s6f ^= LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )];

  /* Lane 2 */
  t0  = LANE_T0[LANE_B3(s20)] ^ LANE_T1[LANE_B2(s21)] ^ LANE_T2[LANE_B1(s22)] ^ LANE_T3[LANE_B0(s23)] ^ LANE_C[  0+224];
  t4  = LANE_T0[LANE_B3(s21)] ^ LANE_T1[LANE_B2(s22)] ^ LANE_T2[LANE_B1(s23)] ^ LANE_T3[LANE_B0(s20)] ^ LANE_C[  1+224];
  t8  = LANE_T0[LANE_B3(s22)] ^ LANE_T1[LANE_B2(s23)] ^ LANE_T2[LANE_B1(s20)] ^ LANE_T3[LANE_B0(s21)] ^ LANE_C[  2+224];
  tc  = LANE_T0[LANE_B3(s23)] ^ LANE_T1[LANE_B2(s20)] ^ LANE_T2[LANE_B1(s21)] ^ LANE_T3[LANE_B0(s22)] ^ LANE_C[  3+224] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s24)] ^ LANE_T1[LANE_B2(s25)] ^ LANE_T2[LANE_B1(s26)] ^ LANE_T3[LANE_B0(s27)] ^ LANE_C[  4+224];
  t5  = LANE_T0[LANE_B3(s25)] ^ LANE_T1[LANE_B2(s26)] ^ LANE_T2[LANE_B1(s27)] ^ LANE_T3[LANE_B0(s24)] ^ LANE_C[  5+224];
  t9  = LANE_T0[LANE_B3(s26)] ^ LANE_T1[LANE_B2(s27)] ^ LANE_T2[LANE_B1(s24)] ^ LANE_T3[LANE_B0(s25)] ^ LANE_C[  6+224];
  td  = LANE_T0[LANE_B3(s27)] ^ LANE_T1[LANE_B2(s24)] ^ LANE_T2[LANE_B1(s25)] ^ LANE_T3[LANE_B0(s26)] ^ LANE_C[  7+224];
  t2  = LANE_T0[LANE_B3(s28)] ^ LANE_T1[LANE_B2(s29)] ^ LANE_T2[LANE_B1(s2a)] ^ LANE_T3[LANE_B0(s2b)] ^ LANE_C[  8+224];
  t6  = LANE_T0[LANE_B3(s29)] ^ LANE_T1[LANE_B2(s2a)] ^ LANE_T2[LANE_B1(s2b)] ^ LANE_T3[LANE_B0(s28)] ^ LANE_C[  9+224];
  ta  = LANE_T0[LANE_B3(s2a)] ^ LANE_T1[LANE_B2(s2b)] ^ LANE_T2[LANE_B1(s28)] ^ LANE_T3[LANE_B0(s29)] ^ LANE_C[ 10+224];
  te  = LANE_T0[LANE_B3(s2b)] ^ LANE_T1[LANE_B2(s28)] ^ LANE_T2[LANE_B1(s29)] ^ LANE_T3[LANE_B0(s2a)] ^ LANE_C[ 11+224];
  t3  = LANE_T0[LANE_B3(s2c)] ^ LANE_T1[LANE_B2(s2d)] ^ LANE_T2[LANE_B1(s2e)] ^ LANE_T3[LANE_B0(s2f)] ^ LANE_C[ 12+224];
  t7  = LANE_T0[LANE_B3(s2d)] ^ LANE_T1[LANE_B2(s2e)] ^ LANE_T2[LANE_B1(s2f)] ^ LANE_T3[LANE_B0(s2c)] ^ LANE_C[ 13+224];
  tb  = LANE_T0[LANE_B3(s2e)] ^ LANE_T1[LANE_B2(s2f)] ^ LANE_T2[LANE_B1(s2c)] ^ LANE_T3[LANE_B0(s2d)] ^ LANE_C[ 14+224];
  tf  = LANE_T0[LANE_B3(s2f)] ^ LANE_T1[LANE_B2(s2c)] ^ LANE_T2[LANE_B1(s2d)] ^ LANE_T3[LANE_B0(s2e)] ^ LANE_C[ 15+224];

  s20 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 16+224];
  s24 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 17+224];
  s28 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 18+224];
  s2c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 19+224] ^ ctrl;
  s21 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 20+224];
  s25 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 21+224];
  s29 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 22+224];
  s2d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 23+224];
  s22 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 24+224];
  s26 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 25+224];
  s2a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 26+224];
  s2e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 27+224];
  s23 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 28+224];
  s27 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 29+224];
  s2b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 30+224];
  s2f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 31+224];

  t0  = LANE_T0[LANE_B3(s20)] ^ LANE_T1[LANE_B2(s21)] ^ LANE_T2[LANE_B1(s22)] ^ LANE_T3[LANE_B0(s23)] ^ LANE_C[ 32+224];
  t4  = LANE_T0[LANE_B3(s21)] ^ LANE_T1[LANE_B2(s22)] ^ LANE_T2[LANE_B1(s23)] ^ LANE_T3[LANE_B0(s20)] ^ LANE_C[ 33+224];
  t8  = LANE_T0[LANE_B3(s22)] ^ LANE_T1[LANE_B2(s23)] ^ LANE_T2[LANE_B1(s20)] ^ LANE_T3[LANE_B0(s21)] ^ LANE_C[ 34+224];
  tc  = LANE_T0[LANE_B3(s23)] ^ LANE_T1[LANE_B2(s20)] ^ LANE_T2[LANE_B1(s21)] ^ LANE_T3[LANE_B0(s22)] ^ LANE_C[ 35+224] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s24)] ^ LANE_T1[LANE_B2(s25)] ^ LANE_T2[LANE_B1(s26)] ^ LANE_T3[LANE_B0(s27)] ^ LANE_C[ 36+224];
  t5  = LANE_T0[LANE_B3(s25)] ^ LANE_T1[LANE_B2(s26)] ^ LANE_T2[LANE_B1(s27)] ^ LANE_T3[LANE_B0(s24)] ^ LANE_C[ 37+224];
  t9  = LANE_T0[LANE_B3(s26)] ^ LANE_T1[LANE_B2(s27)] ^ LANE_T2[LANE_B1(s24)] ^ LANE_T3[LANE_B0(s25)] ^ LANE_C[ 38+224];
  td  = LANE_T0[LANE_B3(s27)] ^ LANE_T1[LANE_B2(s24)] ^ LANE_T2[LANE_B1(s25)] ^ LANE_T3[LANE_B0(s26)] ^ LANE_C[ 39+224];
  t2  = LANE_T0[LANE_B3(s28)] ^ LANE_T1[LANE_B2(s29)] ^ LANE_T2[LANE_B1(s2a)] ^ LANE_T3[LANE_B0(s2b)] ^ LANE_C[ 40+224];
  t6  = LANE_T0[LANE_B3(s29)] ^ LANE_T1[LANE_B2(s2a)] ^ LANE_T2[LANE_B1(s2b)] ^ LANE_T3[LANE_B0(s28)] ^ LANE_C[ 41+224];
  ta  = LANE_T0[LANE_B3(s2a)] ^ LANE_T1[LANE_B2(s2b)] ^ LANE_T2[LANE_B1(s28)] ^ LANE_T3[LANE_B0(s29)] ^ LANE_C[ 42+224];
  te  = LANE_T0[LANE_B3(s2b)] ^ LANE_T1[LANE_B2(s28)] ^ LANE_T2[LANE_B1(s29)] ^ LANE_T3[LANE_B0(s2a)] ^ LANE_C[ 43+224];
  t3  = LANE_T0[LANE_B3(s2c)] ^ LANE_T1[LANE_B2(s2d)] ^ LANE_T2[LANE_B1(s2e)] ^ LANE_T3[LANE_B0(s2f)] ^ LANE_C[ 44+224];
  t7  = LANE_T0[LANE_B3(s2d)] ^ LANE_T1[LANE_B2(s2e)] ^ LANE_T2[LANE_B1(s2f)] ^ LANE_T3[LANE_B0(s2c)] ^ LANE_C[ 45+224];
  tb  = LANE_T0[LANE_B3(s2e)] ^ LANE_T1[LANE_B2(s2f)] ^ LANE_T2[LANE_B1(s2c)] ^ LANE_T3[LANE_B0(s2d)] ^ LANE_C[ 46+224];
  tf  = LANE_T0[LANE_B3(s2f)] ^ LANE_T1[LANE_B2(s2c)] ^ LANE_T2[LANE_B1(s2d)] ^ LANE_T3[LANE_B0(s2e)] ^ LANE_C[ 47+224];

  s20 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 48+224];
  s24 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 49+224];
  s28 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 50+224];
  s2c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 51+224] ^ ctrl;
  s21 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 52+224];
  s25 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 53+224];
  s29 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 54+224];
  s2d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 55+224];
  s22 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 56+224];
  s26 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 57+224];
  s2a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 58+224];
  s2e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 59+224];
  s23 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 60+224];
  s27 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 61+224];
  s2b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 62+224];
  s2f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 63+224];

  t0  = LANE_T0[LANE_B3(s20)] ^ LANE_T1[LANE_B2(s21)] ^ LANE_T2[LANE_B1(s22)] ^ LANE_T3[LANE_B0(s23)] ^ LANE_C[ 64+224];
  t4  = LANE_T0[LANE_B3(s21)] ^ LANE_T1[LANE_B2(s22)] ^ LANE_T2[LANE_B1(s23)] ^ LANE_T3[LANE_B0(s20)] ^ LANE_C[ 65+224];
  t8  = LANE_T0[LANE_B3(s22)] ^ LANE_T1[LANE_B2(s23)] ^ LANE_T2[LANE_B1(s20)] ^ LANE_T3[LANE_B0(s21)] ^ LANE_C[ 66+224];
  tc  = LANE_T0[LANE_B3(s23)] ^ LANE_T1[LANE_B2(s20)] ^ LANE_T2[LANE_B1(s21)] ^ LANE_T3[LANE_B0(s22)] ^ LANE_C[ 67+224] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s24)] ^ LANE_T1[LANE_B2(s25)] ^ LANE_T2[LANE_B1(s26)] ^ LANE_T3[LANE_B0(s27)] ^ LANE_C[ 68+224];
  t5  = LANE_T0[LANE_B3(s25)] ^ LANE_T1[LANE_B2(s26)] ^ LANE_T2[LANE_B1(s27)] ^ LANE_T3[LANE_B0(s24)] ^ LANE_C[ 69+224];
  t9  = LANE_T0[LANE_B3(s26)] ^ LANE_T1[LANE_B2(s27)] ^ LANE_T2[LANE_B1(s24)] ^ LANE_T3[LANE_B0(s25)] ^ LANE_C[ 70+224];
  td  = LANE_T0[LANE_B3(s27)] ^ LANE_T1[LANE_B2(s24)] ^ LANE_T2[LANE_B1(s25)] ^ LANE_T3[LANE_B0(s26)] ^ LANE_C[ 71+224];
  t2  = LANE_T0[LANE_B3(s28)] ^ LANE_T1[LANE_B2(s29)] ^ LANE_T2[LANE_B1(s2a)] ^ LANE_T3[LANE_B0(s2b)] ^ LANE_C[ 72+224];
  t6  = LANE_T0[LANE_B3(s29)] ^ LANE_T1[LANE_B2(s2a)] ^ LANE_T2[LANE_B1(s2b)] ^ LANE_T3[LANE_B0(s28)] ^ LANE_C[ 73+224];
  ta  = LANE_T0[LANE_B3(s2a)] ^ LANE_T1[LANE_B2(s2b)] ^ LANE_T2[LANE_B1(s28)] ^ LANE_T3[LANE_B0(s29)] ^ LANE_C[ 74+224];
  te  = LANE_T0[LANE_B3(s2b)] ^ LANE_T1[LANE_B2(s28)] ^ LANE_T2[LANE_B1(s29)] ^ LANE_T3[LANE_B0(s2a)] ^ LANE_C[ 75+224];
  t3  = LANE_T0[LANE_B3(s2c)] ^ LANE_T1[LANE_B2(s2d)] ^ LANE_T2[LANE_B1(s2e)] ^ LANE_T3[LANE_B0(s2f)] ^ LANE_C[ 76+224];
  t7  = LANE_T0[LANE_B3(s2d)] ^ LANE_T1[LANE_B2(s2e)] ^ LANE_T2[LANE_B1(s2f)] ^ LANE_T3[LANE_B0(s2c)] ^ LANE_C[ 77+224];
  tb  = LANE_T0[LANE_B3(s2e)] ^ LANE_T1[LANE_B2(s2f)] ^ LANE_T2[LANE_B1(s2c)] ^ LANE_T3[LANE_B0(s2d)] ^ LANE_C[ 78+224];
  tf  = LANE_T0[LANE_B3(s2f)] ^ LANE_T1[LANE_B2(s2c)] ^ LANE_T2[LANE_B1(s2d)] ^ LANE_T3[LANE_B0(s2e)] ^ LANE_C[ 79+224];

  s20 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 80+224];
  s24 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 81+224];
  s28 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 82+224];
  s2c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 83+224] ^ ctrl;
  s21 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 84+224];
  s25 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 85+224];
  s29 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 86+224];
  s2d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 87+224];
  s22 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 88+224];
  s26 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 89+224];
  s2a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 90+224];
  s2e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 91+224];
  s23 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 92+224];
  s27 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 93+224];
  s2b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 94+224];
  s2f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 95+224];

  t0  = LANE_T0[LANE_B3(s20)] ^ LANE_T1[LANE_B2(s21)] ^ LANE_T2[LANE_B1(s22)] ^ LANE_T3[LANE_B0(s23)] ^ LANE_C[ 96+224];
  t4  = LANE_T0[LANE_B3(s21)] ^ LANE_T1[LANE_B2(s22)] ^ LANE_T2[LANE_B1(s23)] ^ LANE_T3[LANE_B0(s20)] ^ LANE_C[ 97+224];
  t8  = LANE_T0[LANE_B3(s22)] ^ LANE_T1[LANE_B2(s23)] ^ LANE_T2[LANE_B1(s20)] ^ LANE_T3[LANE_B0(s21)] ^ LANE_C[ 98+224];
  tc  = LANE_T0[LANE_B3(s23)] ^ LANE_T1[LANE_B2(s20)] ^ LANE_T2[LANE_B1(s21)] ^ LANE_T3[LANE_B0(s22)] ^ LANE_C[ 99+224] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s24)] ^ LANE_T1[LANE_B2(s25)] ^ LANE_T2[LANE_B1(s26)] ^ LANE_T3[LANE_B0(s27)] ^ LANE_C[100+224];
  t5  = LANE_T0[LANE_B3(s25)] ^ LANE_T1[LANE_B2(s26)] ^ LANE_T2[LANE_B1(s27)] ^ LANE_T3[LANE_B0(s24)] ^ LANE_C[101+224];
  t9  = LANE_T0[LANE_B3(s26)] ^ LANE_T1[LANE_B2(s27)] ^ LANE_T2[LANE_B1(s24)] ^ LANE_T3[LANE_B0(s25)] ^ LANE_C[102+224];
  td  = LANE_T0[LANE_B3(s27)] ^ LANE_T1[LANE_B2(s24)] ^ LANE_T2[LANE_B1(s25)] ^ LANE_T3[LANE_B0(s26)] ^ LANE_C[103+224];
  t2  = LANE_T0[LANE_B3(s28)] ^ LANE_T1[LANE_B2(s29)] ^ LANE_T2[LANE_B1(s2a)] ^ LANE_T3[LANE_B0(s2b)] ^ LANE_C[104+224];
  t6  = LANE_T0[LANE_B3(s29)] ^ LANE_T1[LANE_B2(s2a)] ^ LANE_T2[LANE_B1(s2b)] ^ LANE_T3[LANE_B0(s28)] ^ LANE_C[105+224];
  ta  = LANE_T0[LANE_B3(s2a)] ^ LANE_T1[LANE_B2(s2b)] ^ LANE_T2[LANE_B1(s28)] ^ LANE_T3[LANE_B0(s29)] ^ LANE_C[106+224];
  te  = LANE_T0[LANE_B3(s2b)] ^ LANE_T1[LANE_B2(s28)] ^ LANE_T2[LANE_B1(s29)] ^ LANE_T3[LANE_B0(s2a)] ^ LANE_C[107+224];
  t3  = LANE_T0[LANE_B3(s2c)] ^ LANE_T1[LANE_B2(s2d)] ^ LANE_T2[LANE_B1(s2e)] ^ LANE_T3[LANE_B0(s2f)] ^ LANE_C[108+224];
  t7  = LANE_T0[LANE_B3(s2d)] ^ LANE_T1[LANE_B2(s2e)] ^ LANE_T2[LANE_B1(s2f)] ^ LANE_T3[LANE_B0(s2c)] ^ LANE_C[109+224];
  tb  = LANE_T0[LANE_B3(s2e)] ^ LANE_T1[LANE_B2(s2f)] ^ LANE_T2[LANE_B1(s2c)] ^ LANE_T3[LANE_B0(s2d)] ^ LANE_C[110+224];
  tf  = LANE_T0[LANE_B3(s2f)] ^ LANE_T1[LANE_B2(s2c)] ^ LANE_T2[LANE_B1(s2d)] ^ LANE_T3[LANE_B0(s2e)] ^ LANE_C[111+224];

  s60 ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s64 ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s68 ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s6c ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s61 ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s65 ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s69 ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s6d ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  s62 ^= LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )];
  s66 ^= LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )];
  s6a ^= LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )];
  s6e ^= LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )];
  s63 ^= LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )];
  s67 ^= LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )];
  s6b ^= LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )];
  s6f ^= LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )];

  /* Lane 3 */
  t0  = LANE_T0[LANE_B3(s30)] ^ LANE_T1[LANE_B2(s31)] ^ LANE_T2[LANE_B1(s32)] ^ LANE_T3[LANE_B0(s33)] ^ LANE_C[  0+336];
  t4  = LANE_T0[LANE_B3(s31)] ^ LANE_T1[LANE_B2(s32)] ^ LANE_T2[LANE_B1(s33)] ^ LANE_T3[LANE_B0(s30)] ^ LANE_C[  1+336];
  t8  = LANE_T0[LANE_B3(s32)] ^ LANE_T1[LANE_B2(s33)] ^ LANE_T2[LANE_B1(s30)] ^ LANE_T3[LANE_B0(s31)] ^ LANE_C[  2+336];
  tc  = LANE_T0[LANE_B3(s33)] ^ LANE_T1[LANE_B2(s30)] ^ LANE_T2[LANE_B1(s31)] ^ LANE_T3[LANE_B0(s32)] ^ LANE_C[  3+336] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s34)] ^ LANE_T1[LANE_B2(s35)] ^ LANE_T2[LANE_B1(s36)] ^ LANE_T3[LANE_B0(s37)] ^ LANE_C[  4+336];
  t5  = LANE_T0[LANE_B3(s35)] ^ LANE_T1[LANE_B2(s36)] ^ LANE_T2[LANE_B1(s37)] ^ LANE_T3[LANE_B0(s34)] ^ LANE_C[  5+336];
  t9  = LANE_T0[LANE_B3(s36)] ^ LANE_T1[LANE_B2(s37)] ^ LANE_T2[LANE_B1(s34)] ^ LANE_T3[LANE_B0(s35)] ^ LANE_C[  6+336];
  td  = LANE_T0[LANE_B3(s37)] ^ LANE_T1[LANE_B2(s34)] ^ LANE_T2[LANE_B1(s35)] ^ LANE_T3[LANE_B0(s36)] ^ LANE_C[  7+336];
  t2  = LANE_T0[LANE_B3(s38)] ^ LANE_T1[LANE_B2(s39)] ^ LANE_T2[LANE_B1(s3a)] ^ LANE_T3[LANE_B0(s3b)] ^ LANE_C[  8+336];
  t6  = LANE_T0[LANE_B3(s39)] ^ LANE_T1[LANE_B2(s3a)] ^ LANE_T2[LANE_B1(s3b)] ^ LANE_T3[LANE_B0(s38)] ^ LANE_C[  9+336];
  ta  = LANE_T0[LANE_B3(s3a)] ^ LANE_T1[LANE_B2(s3b)] ^ LANE_T2[LANE_B1(s38)] ^ LANE_T3[LANE_B0(s39)] ^ LANE_C[ 10+336];
  te  = LANE_T0[LANE_B3(s3b)] ^ LANE_T1[LANE_B2(s38)] ^ LANE_T2[LANE_B1(s39)] ^ LANE_T3[LANE_B0(s3a)] ^ LANE_C[ 11+336];
  t3  = LANE_T0[LANE_B3(s3c)] ^ LANE_T1[LANE_B2(s3d)] ^ LANE_T2[LANE_B1(s3e)] ^ LANE_T3[LANE_B0(s3f)] ^ LANE_C[ 12+336];
  t7  = LANE_T0[LANE_B3(s3d)] ^ LANE_T1[LANE_B2(s3e)] ^ LANE_T2[LANE_B1(s3f)] ^ LANE_T3[LANE_B0(s3c)] ^ LANE_C[ 13+336];
  tb  = LANE_T0[LANE_B3(s3e)] ^ LANE_T1[LANE_B2(s3f)] ^ LANE_T2[LANE_B1(s3c)] ^ LANE_T3[LANE_B0(s3d)] ^ LANE_C[ 14+336];
  tf  = LANE_T0[LANE_B3(s3f)] ^ LANE_T1[LANE_B2(s3c)] ^ LANE_T2[LANE_B1(s3d)] ^ LANE_T3[LANE_B0(s3e)] ^ LANE_C[ 15+336];

  s30 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 16+336];
  s34 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 17+336];
  s38 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 18+336];
  s3c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 19+336] ^ ctrh;
  s31 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 20+336];
  s35 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 21+336];
  s39 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 22+336];
  s3d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 23+336];
  s32 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 24+336];
  s36 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 25+336];
  s3a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 26+336];
  s3e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 27+336];
  s33 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 28+336];
  s37 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 29+336];
  s3b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 30+336];
  s3f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 31+336];

  t0  = LANE_T0[LANE_B3(s30)] ^ LANE_T1[LANE_B2(s31)] ^ LANE_T2[LANE_B1(s32)] ^ LANE_T3[LANE_B0(s33)] ^ LANE_C[ 32+336];
  t4  = LANE_T0[LANE_B3(s31)] ^ LANE_T1[LANE_B2(s32)] ^ LANE_T2[LANE_B1(s33)] ^ LANE_T3[LANE_B0(s30)] ^ LANE_C[ 33+336];
  t8  = LANE_T0[LANE_B3(s32)] ^ LANE_T1[LANE_B2(s33)] ^ LANE_T2[LANE_B1(s30)] ^ LANE_T3[LANE_B0(s31)] ^ LANE_C[ 34+336];
  tc  = LANE_T0[LANE_B3(s33)] ^ LANE_T1[LANE_B2(s30)] ^ LANE_T2[LANE_B1(s31)] ^ LANE_T3[LANE_B0(s32)] ^ LANE_C[ 35+336] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s34)] ^ LANE_T1[LANE_B2(s35)] ^ LANE_T2[LANE_B1(s36)] ^ LANE_T3[LANE_B0(s37)] ^ LANE_C[ 36+336];
  t5  = LANE_T0[LANE_B3(s35)] ^ LANE_T1[LANE_B2(s36)] ^ LANE_T2[LANE_B1(s37)] ^ LANE_T3[LANE_B0(s34)] ^ LANE_C[ 37+336];
  t9  = LANE_T0[LANE_B3(s36)] ^ LANE_T1[LANE_B2(s37)] ^ LANE_T2[LANE_B1(s34)] ^ LANE_T3[LANE_B0(s35)] ^ LANE_C[ 38+336];
  td  = LANE_T0[LANE_B3(s37)] ^ LANE_T1[LANE_B2(s34)] ^ LANE_T2[LANE_B1(s35)] ^ LANE_T3[LANE_B0(s36)] ^ LANE_C[ 39+336];
  t2  = LANE_T0[LANE_B3(s38)] ^ LANE_T1[LANE_B2(s39)] ^ LANE_T2[LANE_B1(s3a)] ^ LANE_T3[LANE_B0(s3b)] ^ LANE_C[ 40+336];
  t6  = LANE_T0[LANE_B3(s39)] ^ LANE_T1[LANE_B2(s3a)] ^ LANE_T2[LANE_B1(s3b)] ^ LANE_T3[LANE_B0(s38)] ^ LANE_C[ 41+336];
  ta  = LANE_T0[LANE_B3(s3a)] ^ LANE_T1[LANE_B2(s3b)] ^ LANE_T2[LANE_B1(s38)] ^ LANE_T3[LANE_B0(s39)] ^ LANE_C[ 42+336];
  te  = LANE_T0[LANE_B3(s3b)] ^ LANE_T1[LANE_B2(s38)] ^ LANE_T2[LANE_B1(s39)] ^ LANE_T3[LANE_B0(s3a)] ^ LANE_C[ 43+336];
  t3  = LANE_T0[LANE_B3(s3c)] ^ LANE_T1[LANE_B2(s3d)] ^ LANE_T2[LANE_B1(s3e)] ^ LANE_T3[LANE_B0(s3f)] ^ LANE_C[ 44+336];
  t7  = LANE_T0[LANE_B3(s3d)] ^ LANE_T1[LANE_B2(s3e)] ^ LANE_T2[LANE_B1(s3f)] ^ LANE_T3[LANE_B0(s3c)] ^ LANE_C[ 45+336];
  tb  = LANE_T0[LANE_B3(s3e)] ^ LANE_T1[LANE_B2(s3f)] ^ LANE_T2[LANE_B1(s3c)] ^ LANE_T3[LANE_B0(s3d)] ^ LANE_C[ 46+336];
  tf  = LANE_T0[LANE_B3(s3f)] ^ LANE_T1[LANE_B2(s3c)] ^ LANE_T2[LANE_B1(s3d)] ^ LANE_T3[LANE_B0(s3e)] ^ LANE_C[ 47+336];

  s30 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 48+336];
  s34 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 49+336];
  s38 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 50+336];
  s3c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 51+336] ^ ctrh;
  s31 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 52+336];
  s35 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 53+336];
  s39 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 54+336];
  s3d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 55+336];
  s32 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 56+336];
  s36 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 57+336];
  s3a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 58+336];
  s3e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 59+336];
  s33 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 60+336];
  s37 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 61+336];
  s3b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 62+336];
  s3f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 63+336];

  t0  = LANE_T0[LANE_B3(s30)] ^ LANE_T1[LANE_B2(s31)] ^ LANE_T2[LANE_B1(s32)] ^ LANE_T3[LANE_B0(s33)] ^ LANE_C[ 64+336];
  t4  = LANE_T0[LANE_B3(s31)] ^ LANE_T1[LANE_B2(s32)] ^ LANE_T2[LANE_B1(s33)] ^ LANE_T3[LANE_B0(s30)] ^ LANE_C[ 65+336];
  t8  = LANE_T0[LANE_B3(s32)] ^ LANE_T1[LANE_B2(s33)] ^ LANE_T2[LANE_B1(s30)] ^ LANE_T3[LANE_B0(s31)] ^ LANE_C[ 66+336];
  tc  = LANE_T0[LANE_B3(s33)] ^ LANE_T1[LANE_B2(s30)] ^ LANE_T2[LANE_B1(s31)] ^ LANE_T3[LANE_B0(s32)] ^ LANE_C[ 67+336] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s34)] ^ LANE_T1[LANE_B2(s35)] ^ LANE_T2[LANE_B1(s36)] ^ LANE_T3[LANE_B0(s37)] ^ LANE_C[ 68+336];
  t5  = LANE_T0[LANE_B3(s35)] ^ LANE_T1[LANE_B2(s36)] ^ LANE_T2[LANE_B1(s37)] ^ LANE_T3[LANE_B0(s34)] ^ LANE_C[ 69+336];
  t9  = LANE_T0[LANE_B3(s36)] ^ LANE_T1[LANE_B2(s37)] ^ LANE_T2[LANE_B1(s34)] ^ LANE_T3[LANE_B0(s35)] ^ LANE_C[ 70+336];
  td  = LANE_T0[LANE_B3(s37)] ^ LANE_T1[LANE_B2(s34)] ^ LANE_T2[LANE_B1(s35)] ^ LANE_T3[LANE_B0(s36)] ^ LANE_C[ 71+336];
  t2  = LANE_T0[LANE_B3(s38)] ^ LANE_T1[LANE_B2(s39)] ^ LANE_T2[LANE_B1(s3a)] ^ LANE_T3[LANE_B0(s3b)] ^ LANE_C[ 72+336];
  t6  = LANE_T0[LANE_B3(s39)] ^ LANE_T1[LANE_B2(s3a)] ^ LANE_T2[LANE_B1(s3b)] ^ LANE_T3[LANE_B0(s38)] ^ LANE_C[ 73+336];
  ta  = LANE_T0[LANE_B3(s3a)] ^ LANE_T1[LANE_B2(s3b)] ^ LANE_T2[LANE_B1(s38)] ^ LANE_T3[LANE_B0(s39)] ^ LANE_C[ 74+336];
  te  = LANE_T0[LANE_B3(s3b)] ^ LANE_T1[LANE_B2(s38)] ^ LANE_T2[LANE_B1(s39)] ^ LANE_T3[LANE_B0(s3a)] ^ LANE_C[ 75+336];
  t3  = LANE_T0[LANE_B3(s3c)] ^ LANE_T1[LANE_B2(s3d)] ^ LANE_T2[LANE_B1(s3e)] ^ LANE_T3[LANE_B0(s3f)] ^ LANE_C[ 76+336];
  t7  = LANE_T0[LANE_B3(s3d)] ^ LANE_T1[LANE_B2(s3e)] ^ LANE_T2[LANE_B1(s3f)] ^ LANE_T3[LANE_B0(s3c)] ^ LANE_C[ 77+336];
  tb  = LANE_T0[LANE_B3(s3e)] ^ LANE_T1[LANE_B2(s3f)] ^ LANE_T2[LANE_B1(s3c)] ^ LANE_T3[LANE_B0(s3d)] ^ LANE_C[ 78+336];
  tf  = LANE_T0[LANE_B3(s3f)] ^ LANE_T1[LANE_B2(s3c)] ^ LANE_T2[LANE_B1(s3d)] ^ LANE_T3[LANE_B0(s3e)] ^ LANE_C[ 79+336];

  s30 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 80+336];
  s34 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 81+336];
  s38 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 82+336];
  s3c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 83+336] ^ ctrh;
  s31 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 84+336];
  s35 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 85+336];
  s39 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 86+336];
  s3d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 87+336];
  s32 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 88+336];
  s36 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 89+336];
  s3a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 90+336];
  s3e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 91+336];
  s33 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 92+336];
  s37 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 93+336];
  s3b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 94+336];
  s3f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 95+336];

  t0  = LANE_T0[LANE_B3(s30)] ^ LANE_T1[LANE_B2(s31)] ^ LANE_T2[LANE_B1(s32)] ^ LANE_T3[LANE_B0(s33)] ^ LANE_C[ 96+336];
  t4  = LANE_T0[LANE_B3(s31)] ^ LANE_T1[LANE_B2(s32)] ^ LANE_T2[LANE_B1(s33)] ^ LANE_T3[LANE_B0(s30)] ^ LANE_C[ 97+336];
  t8  = LANE_T0[LANE_B3(s32)] ^ LANE_T1[LANE_B2(s33)] ^ LANE_T2[LANE_B1(s30)] ^ LANE_T3[LANE_B0(s31)] ^ LANE_C[ 98+336];
  tc  = LANE_T0[LANE_B3(s33)] ^ LANE_T1[LANE_B2(s30)] ^ LANE_T2[LANE_B1(s31)] ^ LANE_T3[LANE_B0(s32)] ^ LANE_C[ 99+336] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s34)] ^ LANE_T1[LANE_B2(s35)] ^ LANE_T2[LANE_B1(s36)] ^ LANE_T3[LANE_B0(s37)] ^ LANE_C[100+336];
  t5  = LANE_T0[LANE_B3(s35)] ^ LANE_T1[LANE_B2(s36)] ^ LANE_T2[LANE_B1(s37)] ^ LANE_T3[LANE_B0(s34)] ^ LANE_C[101+336];
  t9  = LANE_T0[LANE_B3(s36)] ^ LANE_T1[LANE_B2(s37)] ^ LANE_T2[LANE_B1(s34)] ^ LANE_T3[LANE_B0(s35)] ^ LANE_C[102+336];
  td  = LANE_T0[LANE_B3(s37)] ^ LANE_T1[LANE_B2(s34)] ^ LANE_T2[LANE_B1(s35)] ^ LANE_T3[LANE_B0(s36)] ^ LANE_C[103+336];
  t2  = LANE_T0[LANE_B3(s38)] ^ LANE_T1[LANE_B2(s39)] ^ LANE_T2[LANE_B1(s3a)] ^ LANE_T3[LANE_B0(s3b)] ^ LANE_C[104+336];
  t6  = LANE_T0[LANE_B3(s39)] ^ LANE_T1[LANE_B2(s3a)] ^ LANE_T2[LANE_B1(s3b)] ^ LANE_T3[LANE_B0(s38)] ^ LANE_C[105+336];
  ta  = LANE_T0[LANE_B3(s3a)] ^ LANE_T1[LANE_B2(s3b)] ^ LANE_T2[LANE_B1(s38)] ^ LANE_T3[LANE_B0(s39)] ^ LANE_C[106+336];
  te  = LANE_T0[LANE_B3(s3b)] ^ LANE_T1[LANE_B2(s38)] ^ LANE_T2[LANE_B1(s39)] ^ LANE_T3[LANE_B0(s3a)] ^ LANE_C[107+336];
  t3  = LANE_T0[LANE_B3(s3c)] ^ LANE_T1[LANE_B2(s3d)] ^ LANE_T2[LANE_B1(s3e)] ^ LANE_T3[LANE_B0(s3f)] ^ LANE_C[108+336];
  t7  = LANE_T0[LANE_B3(s3d)] ^ LANE_T1[LANE_B2(s3e)] ^ LANE_T2[LANE_B1(s3f)] ^ LANE_T3[LANE_B0(s3c)] ^ LANE_C[109+336];
  tb  = LANE_T0[LANE_B3(s3e)] ^ LANE_T1[LANE_B2(s3f)] ^ LANE_T2[LANE_B1(s3c)] ^ LANE_T3[LANE_B0(s3d)] ^ LANE_C[110+336];
  tf  = LANE_T0[LANE_B3(s3f)] ^ LANE_T1[LANE_B2(s3c)] ^ LANE_T2[LANE_B1(s3d)] ^ LANE_T3[LANE_B0(s3e)] ^ LANE_C[111+336];

  s70 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s74 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s78 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s7c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s71 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s75 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s79 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s7d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  s72 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )];
  s76 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )];
  s7a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )];
  s7e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )];
  s73 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )];
  s77 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )];
  s7b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )];
  s7f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )];

  /* Lane 4 */
  t0  = LANE_T0[LANE_B3(s40)] ^ LANE_T1[LANE_B2(s41)] ^ LANE_T2[LANE_B1(s42)] ^ LANE_T3[LANE_B0(s43)] ^ LANE_C[  0+448];
  t4  = LANE_T0[LANE_B3(s41)] ^ LANE_T1[LANE_B2(s42)] ^ LANE_T2[LANE_B1(s43)] ^ LANE_T3[LANE_B0(s40)] ^ LANE_C[  1+448];
  t8  = LANE_T0[LANE_B3(s42)] ^ LANE_T1[LANE_B2(s43)] ^ LANE_T2[LANE_B1(s40)] ^ LANE_T3[LANE_B0(s41)] ^ LANE_C[  2+448];
  tc  = LANE_T0[LANE_B3(s43)] ^ LANE_T1[LANE_B2(s40)] ^ LANE_T2[LANE_B1(s41)] ^ LANE_T3[LANE_B0(s42)] ^ LANE_C[  3+448] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s44)] ^ LANE_T1[LANE_B2(s45)] ^ LANE_T2[LANE_B1(s46)] ^ LANE_T3[LANE_B0(s47)] ^ LANE_C[  4+448];
  t5  = LANE_T0[LANE_B3(s45)] ^ LANE_T1[LANE_B2(s46)] ^ LANE_T2[LANE_B1(s47)] ^ LANE_T3[LANE_B0(s44)] ^ LANE_C[  5+448];
  t9  = LANE_T0[LANE_B3(s46)] ^ LANE_T1[LANE_B2(s47)] ^ LANE_T2[LANE_B1(s44)] ^ LANE_T3[LANE_B0(s45)] ^ LANE_C[  6+448];
  td  = LANE_T0[LANE_B3(s47)] ^ LANE_T1[LANE_B2(s44)] ^ LANE_T2[LANE_B1(s45)] ^ LANE_T3[LANE_B0(s46)] ^ LANE_C[  7+448];
  t2  = LANE_T0[LANE_B3(s48)] ^ LANE_T1[LANE_B2(s49)] ^ LANE_T2[LANE_B1(s4a)] ^ LANE_T3[LANE_B0(s4b)] ^ LANE_C[  8+448];
  t6  = LANE_T0[LANE_B3(s49)] ^ LANE_T1[LANE_B2(s4a)] ^ LANE_T2[LANE_B1(s4b)] ^ LANE_T3[LANE_B0(s48)] ^ LANE_C[  9+448];
  ta  = LANE_T0[LANE_B3(s4a)] ^ LANE_T1[LANE_B2(s4b)] ^ LANE_T2[LANE_B1(s48)] ^ LANE_T3[LANE_B0(s49)] ^ LANE_C[ 10+448];
  te  = LANE_T0[LANE_B3(s4b)] ^ LANE_T1[LANE_B2(s48)] ^ LANE_T2[LANE_B1(s49)] ^ LANE_T3[LANE_B0(s4a)] ^ LANE_C[ 11+448];
  t3  = LANE_T0[LANE_B3(s4c)] ^ LANE_T1[LANE_B2(s4d)] ^ LANE_T2[LANE_B1(s4e)] ^ LANE_T3[LANE_B0(s4f)] ^ LANE_C[ 12+448];
  t7  = LANE_T0[LANE_B3(s4d)] ^ LANE_T1[LANE_B2(s4e)] ^ LANE_T2[LANE_B1(s4f)] ^ LANE_T3[LANE_B0(s4c)] ^ LANE_C[ 13+448];
  tb  = LANE_T0[LANE_B3(s4e)] ^ LANE_T1[LANE_B2(s4f)] ^ LANE_T2[LANE_B1(s4c)] ^ LANE_T3[LANE_B0(s4d)] ^ LANE_C[ 14+448];
  tf  = LANE_T0[LANE_B3(s4f)] ^ LANE_T1[LANE_B2(s4c)] ^ LANE_T2[LANE_B1(s4d)] ^ LANE_T3[LANE_B0(s4e)] ^ LANE_C[ 15+448];

  s40 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 16+448];
  s44 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 17+448];
  s48 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 18+448];
  s4c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 19+448] ^ ctrl;
  s41 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 20+448];
  s45 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 21+448];
  s49 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 22+448];
  s4d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 23+448];
  s42 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 24+448];
  s46 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 25+448];
  s4a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 26+448];
  s4e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 27+448];
  s43 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 28+448];
  s47 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 29+448];
  s4b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 30+448];
  s4f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 31+448];

  t0  = LANE_T0[LANE_B3(s40)] ^ LANE_T1[LANE_B2(s41)] ^ LANE_T2[LANE_B1(s42)] ^ LANE_T3[LANE_B0(s43)] ^ LANE_C[ 32+448];
  t4  = LANE_T0[LANE_B3(s41)] ^ LANE_T1[LANE_B2(s42)] ^ LANE_T2[LANE_B1(s43)] ^ LANE_T3[LANE_B0(s40)] ^ LANE_C[ 33+448];
  t8  = LANE_T0[LANE_B3(s42)] ^ LANE_T1[LANE_B2(s43)] ^ LANE_T2[LANE_B1(s40)] ^ LANE_T3[LANE_B0(s41)] ^ LANE_C[ 34+448];
  tc  = LANE_T0[LANE_B3(s43)] ^ LANE_T1[LANE_B2(s40)] ^ LANE_T2[LANE_B1(s41)] ^ LANE_T3[LANE_B0(s42)] ^ LANE_C[ 35+448] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s44)] ^ LANE_T1[LANE_B2(s45)] ^ LANE_T2[LANE_B1(s46)] ^ LANE_T3[LANE_B0(s47)] ^ LANE_C[ 36+448];
  t5  = LANE_T0[LANE_B3(s45)] ^ LANE_T1[LANE_B2(s46)] ^ LANE_T2[LANE_B1(s47)] ^ LANE_T3[LANE_B0(s44)] ^ LANE_C[ 37+448];
  t9  = LANE_T0[LANE_B3(s46)] ^ LANE_T1[LANE_B2(s47)] ^ LANE_T2[LANE_B1(s44)] ^ LANE_T3[LANE_B0(s45)] ^ LANE_C[ 38+448];
  td  = LANE_T0[LANE_B3(s47)] ^ LANE_T1[LANE_B2(s44)] ^ LANE_T2[LANE_B1(s45)] ^ LANE_T3[LANE_B0(s46)] ^ LANE_C[ 39+448];
  t2  = LANE_T0[LANE_B3(s48)] ^ LANE_T1[LANE_B2(s49)] ^ LANE_T2[LANE_B1(s4a)] ^ LANE_T3[LANE_B0(s4b)] ^ LANE_C[ 40+448];
  t6  = LANE_T0[LANE_B3(s49)] ^ LANE_T1[LANE_B2(s4a)] ^ LANE_T2[LANE_B1(s4b)] ^ LANE_T3[LANE_B0(s48)] ^ LANE_C[ 41+448];
  ta  = LANE_T0[LANE_B3(s4a)] ^ LANE_T1[LANE_B2(s4b)] ^ LANE_T2[LANE_B1(s48)] ^ LANE_T3[LANE_B0(s49)] ^ LANE_C[ 42+448];
  te  = LANE_T0[LANE_B3(s4b)] ^ LANE_T1[LANE_B2(s48)] ^ LANE_T2[LANE_B1(s49)] ^ LANE_T3[LANE_B0(s4a)] ^ LANE_C[ 43+448];
  t3  = LANE_T0[LANE_B3(s4c)] ^ LANE_T1[LANE_B2(s4d)] ^ LANE_T2[LANE_B1(s4e)] ^ LANE_T3[LANE_B0(s4f)] ^ LANE_C[ 44+448];
  t7  = LANE_T0[LANE_B3(s4d)] ^ LANE_T1[LANE_B2(s4e)] ^ LANE_T2[LANE_B1(s4f)] ^ LANE_T3[LANE_B0(s4c)] ^ LANE_C[ 45+448];
  tb  = LANE_T0[LANE_B3(s4e)] ^ LANE_T1[LANE_B2(s4f)] ^ LANE_T2[LANE_B1(s4c)] ^ LANE_T3[LANE_B0(s4d)] ^ LANE_C[ 46+448];
  tf  = LANE_T0[LANE_B3(s4f)] ^ LANE_T1[LANE_B2(s4c)] ^ LANE_T2[LANE_B1(s4d)] ^ LANE_T3[LANE_B0(s4e)] ^ LANE_C[ 47+448];

  s40 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 48+448];
  s44 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 49+448];
  s48 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 50+448];
  s4c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 51+448] ^ ctrl;
  s41 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 52+448];
  s45 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 53+448];
  s49 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 54+448];
  s4d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 55+448];
  s42 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 56+448];
  s46 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 57+448];
  s4a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 58+448];
  s4e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 59+448];
  s43 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 60+448];
  s47 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 61+448];
  s4b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 62+448];
  s4f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 63+448];

  t0  = LANE_T0[LANE_B3(s40)] ^ LANE_T1[LANE_B2(s41)] ^ LANE_T2[LANE_B1(s42)] ^ LANE_T3[LANE_B0(s43)] ^ LANE_C[ 64+448];
  t4  = LANE_T0[LANE_B3(s41)] ^ LANE_T1[LANE_B2(s42)] ^ LANE_T2[LANE_B1(s43)] ^ LANE_T3[LANE_B0(s40)] ^ LANE_C[ 65+448];
  t8  = LANE_T0[LANE_B3(s42)] ^ LANE_T1[LANE_B2(s43)] ^ LANE_T2[LANE_B1(s40)] ^ LANE_T3[LANE_B0(s41)] ^ LANE_C[ 66+448];
  tc  = LANE_T0[LANE_B3(s43)] ^ LANE_T1[LANE_B2(s40)] ^ LANE_T2[LANE_B1(s41)] ^ LANE_T3[LANE_B0(s42)] ^ LANE_C[ 67+448] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s44)] ^ LANE_T1[LANE_B2(s45)] ^ LANE_T2[LANE_B1(s46)] ^ LANE_T3[LANE_B0(s47)] ^ LANE_C[ 68+448];
  t5  = LANE_T0[LANE_B3(s45)] ^ LANE_T1[LANE_B2(s46)] ^ LANE_T2[LANE_B1(s47)] ^ LANE_T3[LANE_B0(s44)] ^ LANE_C[ 69+448];
  t9  = LANE_T0[LANE_B3(s46)] ^ LANE_T1[LANE_B2(s47)] ^ LANE_T2[LANE_B1(s44)] ^ LANE_T3[LANE_B0(s45)] ^ LANE_C[ 70+448];
  td  = LANE_T0[LANE_B3(s47)] ^ LANE_T1[LANE_B2(s44)] ^ LANE_T2[LANE_B1(s45)] ^ LANE_T3[LANE_B0(s46)] ^ LANE_C[ 71+448];
  t2  = LANE_T0[LANE_B3(s48)] ^ LANE_T1[LANE_B2(s49)] ^ LANE_T2[LANE_B1(s4a)] ^ LANE_T3[LANE_B0(s4b)] ^ LANE_C[ 72+448];
  t6  = LANE_T0[LANE_B3(s49)] ^ LANE_T1[LANE_B2(s4a)] ^ LANE_T2[LANE_B1(s4b)] ^ LANE_T3[LANE_B0(s48)] ^ LANE_C[ 73+448];
  ta  = LANE_T0[LANE_B3(s4a)] ^ LANE_T1[LANE_B2(s4b)] ^ LANE_T2[LANE_B1(s48)] ^ LANE_T3[LANE_B0(s49)] ^ LANE_C[ 74+448];
  te  = LANE_T0[LANE_B3(s4b)] ^ LANE_T1[LANE_B2(s48)] ^ LANE_T2[LANE_B1(s49)] ^ LANE_T3[LANE_B0(s4a)] ^ LANE_C[ 75+448];
  t3  = LANE_T0[LANE_B3(s4c)] ^ LANE_T1[LANE_B2(s4d)] ^ LANE_T2[LANE_B1(s4e)] ^ LANE_T3[LANE_B0(s4f)] ^ LANE_C[ 76+448];
  t7  = LANE_T0[LANE_B3(s4d)] ^ LANE_T1[LANE_B2(s4e)] ^ LANE_T2[LANE_B1(s4f)] ^ LANE_T3[LANE_B0(s4c)] ^ LANE_C[ 77+448];
  tb  = LANE_T0[LANE_B3(s4e)] ^ LANE_T1[LANE_B2(s4f)] ^ LANE_T2[LANE_B1(s4c)] ^ LANE_T3[LANE_B0(s4d)] ^ LANE_C[ 78+448];
  tf  = LANE_T0[LANE_B3(s4f)] ^ LANE_T1[LANE_B2(s4c)] ^ LANE_T2[LANE_B1(s4d)] ^ LANE_T3[LANE_B0(s4e)] ^ LANE_C[ 79+448];

  s40 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 80+448];
  s44 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 81+448];
  s48 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 82+448];
  s4c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 83+448] ^ ctrl;
  s41 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 84+448];
  s45 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 85+448];
  s49 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 86+448];
  s4d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 87+448];
  s42 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 88+448];
  s46 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 89+448];
  s4a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 90+448];
  s4e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 91+448];
  s43 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 92+448];
  s47 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 93+448];
  s4b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 94+448];
  s4f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 95+448];

  t0  = LANE_T0[LANE_B3(s40)] ^ LANE_T1[LANE_B2(s41)] ^ LANE_T2[LANE_B1(s42)] ^ LANE_T3[LANE_B0(s43)] ^ LANE_C[ 96+448];
  t4  = LANE_T0[LANE_B3(s41)] ^ LANE_T1[LANE_B2(s42)] ^ LANE_T2[LANE_B1(s43)] ^ LANE_T3[LANE_B0(s40)] ^ LANE_C[ 97+448];
  t8  = LANE_T0[LANE_B3(s42)] ^ LANE_T1[LANE_B2(s43)] ^ LANE_T2[LANE_B1(s40)] ^ LANE_T3[LANE_B0(s41)] ^ LANE_C[ 98+448];
  tc  = LANE_T0[LANE_B3(s43)] ^ LANE_T1[LANE_B2(s40)] ^ LANE_T2[LANE_B1(s41)] ^ LANE_T3[LANE_B0(s42)] ^ LANE_C[ 99+448] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s44)] ^ LANE_T1[LANE_B2(s45)] ^ LANE_T2[LANE_B1(s46)] ^ LANE_T3[LANE_B0(s47)] ^ LANE_C[100+448];
  t5  = LANE_T0[LANE_B3(s45)] ^ LANE_T1[LANE_B2(s46)] ^ LANE_T2[LANE_B1(s47)] ^ LANE_T3[LANE_B0(s44)] ^ LANE_C[101+448];
  t9  = LANE_T0[LANE_B3(s46)] ^ LANE_T1[LANE_B2(s47)] ^ LANE_T2[LANE_B1(s44)] ^ LANE_T3[LANE_B0(s45)] ^ LANE_C[102+448];
  td  = LANE_T0[LANE_B3(s47)] ^ LANE_T1[LANE_B2(s44)] ^ LANE_T2[LANE_B1(s45)] ^ LANE_T3[LANE_B0(s46)] ^ LANE_C[103+448];
  t2  = LANE_T0[LANE_B3(s48)] ^ LANE_T1[LANE_B2(s49)] ^ LANE_T2[LANE_B1(s4a)] ^ LANE_T3[LANE_B0(s4b)] ^ LANE_C[104+448];
  t6  = LANE_T0[LANE_B3(s49)] ^ LANE_T1[LANE_B2(s4a)] ^ LANE_T2[LANE_B1(s4b)] ^ LANE_T3[LANE_B0(s48)] ^ LANE_C[105+448];
  ta  = LANE_T0[LANE_B3(s4a)] ^ LANE_T1[LANE_B2(s4b)] ^ LANE_T2[LANE_B1(s48)] ^ LANE_T3[LANE_B0(s49)] ^ LANE_C[106+448];
  te  = LANE_T0[LANE_B3(s4b)] ^ LANE_T1[LANE_B2(s48)] ^ LANE_T2[LANE_B1(s49)] ^ LANE_T3[LANE_B0(s4a)] ^ LANE_C[107+448];
  t3  = LANE_T0[LANE_B3(s4c)] ^ LANE_T1[LANE_B2(s4d)] ^ LANE_T2[LANE_B1(s4e)] ^ LANE_T3[LANE_B0(s4f)] ^ LANE_C[108+448];
  t7  = LANE_T0[LANE_B3(s4d)] ^ LANE_T1[LANE_B2(s4e)] ^ LANE_T2[LANE_B1(s4f)] ^ LANE_T3[LANE_B0(s4c)] ^ LANE_C[109+448];
  tb  = LANE_T0[LANE_B3(s4e)] ^ LANE_T1[LANE_B2(s4f)] ^ LANE_T2[LANE_B1(s4c)] ^ LANE_T3[LANE_B0(s4d)] ^ LANE_C[110+448];
  tf  = LANE_T0[LANE_B3(s4f)] ^ LANE_T1[LANE_B2(s4c)] ^ LANE_T2[LANE_B1(s4d)] ^ LANE_T3[LANE_B0(s4e)] ^ LANE_C[111+448];

  s70 ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s74 ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s78 ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s7c ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s71 ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s75 ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s79 ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s7d ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  s72 ^= LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )];
  s76 ^= LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )];
  s7a ^= LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )];
  s7e ^= LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )];
  s73 ^= LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )];
  s77 ^= LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )];
  s7b ^= LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )];
  s7f ^= LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )];

  /* Lane 5 */
  t0  = LANE_T0[LANE_B3(s50)] ^ LANE_T1[LANE_B2(s51)] ^ LANE_T2[LANE_B1(s52)] ^ LANE_T3[LANE_B0(s53)] ^ LANE_C[  0+560];
  t4  = LANE_T0[LANE_B3(s51)] ^ LANE_T1[LANE_B2(s52)] ^ LANE_T2[LANE_B1(s53)] ^ LANE_T3[LANE_B0(s50)] ^ LANE_C[  1+560];
  t8  = LANE_T0[LANE_B3(s52)] ^ LANE_T1[LANE_B2(s53)] ^ LANE_T2[LANE_B1(s50)] ^ LANE_T3[LANE_B0(s51)] ^ LANE_C[  2+560];
  tc  = LANE_T0[LANE_B3(s53)] ^ LANE_T1[LANE_B2(s50)] ^ LANE_T2[LANE_B1(s51)] ^ LANE_T3[LANE_B0(s52)] ^ LANE_C[  3+560] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s54)] ^ LANE_T1[LANE_B2(s55)] ^ LANE_T2[LANE_B1(s56)] ^ LANE_T3[LANE_B0(s57)] ^ LANE_C[  4+560];
  t5  = LANE_T0[LANE_B3(s55)] ^ LANE_T1[LANE_B2(s56)] ^ LANE_T2[LANE_B1(s57)] ^ LANE_T3[LANE_B0(s54)] ^ LANE_C[  5+560];
  t9  = LANE_T0[LANE_B3(s56)] ^ LANE_T1[LANE_B2(s57)] ^ LANE_T2[LANE_B1(s54)] ^ LANE_T3[LANE_B0(s55)] ^ LANE_C[  6+560];
  td  = LANE_T0[LANE_B3(s57)] ^ LANE_T1[LANE_B2(s54)] ^ LANE_T2[LANE_B1(s55)] ^ LANE_T3[LANE_B0(s56)] ^ LANE_C[  7+560];
  t2  = LANE_T0[LANE_B3(s58)] ^ LANE_T1[LANE_B2(s59)] ^ LANE_T2[LANE_B1(s5a)] ^ LANE_T3[LANE_B0(s5b)] ^ LANE_C[  8+560];
  t6  = LANE_T0[LANE_B3(s59)] ^ LANE_T1[LANE_B2(s5a)] ^ LANE_T2[LANE_B1(s5b)] ^ LANE_T3[LANE_B0(s58)] ^ LANE_C[  9+560];
  ta  = LANE_T0[LANE_B3(s5a)] ^ LANE_T1[LANE_B2(s5b)] ^ LANE_T2[LANE_B1(s58)] ^ LANE_T3[LANE_B0(s59)] ^ LANE_C[ 10+560];
  te  = LANE_T0[LANE_B3(s5b)] ^ LANE_T1[LANE_B2(s58)] ^ LANE_T2[LANE_B1(s59)] ^ LANE_T3[LANE_B0(s5a)] ^ LANE_C[ 11+560];
  t3  = LANE_T0[LANE_B3(s5c)] ^ LANE_T1[LANE_B2(s5d)] ^ LANE_T2[LANE_B1(s5e)] ^ LANE_T3[LANE_B0(s5f)] ^ LANE_C[ 12+560];
  t7  = LANE_T0[LANE_B3(s5d)] ^ LANE_T1[LANE_B2(s5e)] ^ LANE_T2[LANE_B1(s5f)] ^ LANE_T3[LANE_B0(s5c)] ^ LANE_C[ 13+560];
  tb  = LANE_T0[LANE_B3(s5e)] ^ LANE_T1[LANE_B2(s5f)] ^ LANE_T2[LANE_B1(s5c)] ^ LANE_T3[LANE_B0(s5d)] ^ LANE_C[ 14+560];
  tf  = LANE_T0[LANE_B3(s5f)] ^ LANE_T1[LANE_B2(s5c)] ^ LANE_T2[LANE_B1(s5d)] ^ LANE_T3[LANE_B0(s5e)] ^ LANE_C[ 15+560];

  s50 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 16+560];
  s54 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 17+560];
  s58 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 18+560];
  s5c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 19+560] ^ ctrh;
  s51 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 20+560];
  s55 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 21+560];
  s59 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 22+560];
  s5d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 23+560];
  s52 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 24+560];
  s56 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 25+560];
  s5a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 26+560];
  s5e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 27+560];
  s53 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 28+560];
  s57 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 29+560];
  s5b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 30+560];
  s5f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 31+560];

  t0  = LANE_T0[LANE_B3(s50)] ^ LANE_T1[LANE_B2(s51)] ^ LANE_T2[LANE_B1(s52)] ^ LANE_T3[LANE_B0(s53)] ^ LANE_C[ 32+560];
  t4  = LANE_T0[LANE_B3(s51)] ^ LANE_T1[LANE_B2(s52)] ^ LANE_T2[LANE_B1(s53)] ^ LANE_T3[LANE_B0(s50)] ^ LANE_C[ 33+560];
  t8  = LANE_T0[LANE_B3(s52)] ^ LANE_T1[LANE_B2(s53)] ^ LANE_T2[LANE_B1(s50)] ^ LANE_T3[LANE_B0(s51)] ^ LANE_C[ 34+560];
  tc  = LANE_T0[LANE_B3(s53)] ^ LANE_T1[LANE_B2(s50)] ^ LANE_T2[LANE_B1(s51)] ^ LANE_T3[LANE_B0(s52)] ^ LANE_C[ 35+560] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s54)] ^ LANE_T1[LANE_B2(s55)] ^ LANE_T2[LANE_B1(s56)] ^ LANE_T3[LANE_B0(s57)] ^ LANE_C[ 36+560];
  t5  = LANE_T0[LANE_B3(s55)] ^ LANE_T1[LANE_B2(s56)] ^ LANE_T2[LANE_B1(s57)] ^ LANE_T3[LANE_B0(s54)] ^ LANE_C[ 37+560];
  t9  = LANE_T0[LANE_B3(s56)] ^ LANE_T1[LANE_B2(s57)] ^ LANE_T2[LANE_B1(s54)] ^ LANE_T3[LANE_B0(s55)] ^ LANE_C[ 38+560];
  td  = LANE_T0[LANE_B3(s57)] ^ LANE_T1[LANE_B2(s54)] ^ LANE_T2[LANE_B1(s55)] ^ LANE_T3[LANE_B0(s56)] ^ LANE_C[ 39+560];
  t2  = LANE_T0[LANE_B3(s58)] ^ LANE_T1[LANE_B2(s59)] ^ LANE_T2[LANE_B1(s5a)] ^ LANE_T3[LANE_B0(s5b)] ^ LANE_C[ 40+560];
  t6  = LANE_T0[LANE_B3(s59)] ^ LANE_T1[LANE_B2(s5a)] ^ LANE_T2[LANE_B1(s5b)] ^ LANE_T3[LANE_B0(s58)] ^ LANE_C[ 41+560];
  ta  = LANE_T0[LANE_B3(s5a)] ^ LANE_T1[LANE_B2(s5b)] ^ LANE_T2[LANE_B1(s58)] ^ LANE_T3[LANE_B0(s59)] ^ LANE_C[ 42+560];
  te  = LANE_T0[LANE_B3(s5b)] ^ LANE_T1[LANE_B2(s58)] ^ LANE_T2[LANE_B1(s59)] ^ LANE_T3[LANE_B0(s5a)] ^ LANE_C[ 43+560];
  t3  = LANE_T0[LANE_B3(s5c)] ^ LANE_T1[LANE_B2(s5d)] ^ LANE_T2[LANE_B1(s5e)] ^ LANE_T3[LANE_B0(s5f)] ^ LANE_C[ 44+560];
  t7  = LANE_T0[LANE_B3(s5d)] ^ LANE_T1[LANE_B2(s5e)] ^ LANE_T2[LANE_B1(s5f)] ^ LANE_T3[LANE_B0(s5c)] ^ LANE_C[ 45+560];
  tb  = LANE_T0[LANE_B3(s5e)] ^ LANE_T1[LANE_B2(s5f)] ^ LANE_T2[LANE_B1(s5c)] ^ LANE_T3[LANE_B0(s5d)] ^ LANE_C[ 46+560];
  tf  = LANE_T0[LANE_B3(s5f)] ^ LANE_T1[LANE_B2(s5c)] ^ LANE_T2[LANE_B1(s5d)] ^ LANE_T3[LANE_B0(s5e)] ^ LANE_C[ 47+560];

  s50 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 48+560];
  s54 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 49+560];
  s58 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 50+560];
  s5c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 51+560] ^ ctrh;
  s51 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 52+560];
  s55 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 53+560];
  s59 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 54+560];
  s5d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 55+560];
  s52 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 56+560];
  s56 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 57+560];
  s5a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 58+560];
  s5e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 59+560];
  s53 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 60+560];
  s57 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 61+560];
  s5b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 62+560];
  s5f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 63+560];

  t0  = LANE_T0[LANE_B3(s50)] ^ LANE_T1[LANE_B2(s51)] ^ LANE_T2[LANE_B1(s52)] ^ LANE_T3[LANE_B0(s53)] ^ LANE_C[ 64+560];
  t4  = LANE_T0[LANE_B3(s51)] ^ LANE_T1[LANE_B2(s52)] ^ LANE_T2[LANE_B1(s53)] ^ LANE_T3[LANE_B0(s50)] ^ LANE_C[ 65+560];
  t8  = LANE_T0[LANE_B3(s52)] ^ LANE_T1[LANE_B2(s53)] ^ LANE_T2[LANE_B1(s50)] ^ LANE_T3[LANE_B0(s51)] ^ LANE_C[ 66+560];
  tc  = LANE_T0[LANE_B3(s53)] ^ LANE_T1[LANE_B2(s50)] ^ LANE_T2[LANE_B1(s51)] ^ LANE_T3[LANE_B0(s52)] ^ LANE_C[ 67+560] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s54)] ^ LANE_T1[LANE_B2(s55)] ^ LANE_T2[LANE_B1(s56)] ^ LANE_T3[LANE_B0(s57)] ^ LANE_C[ 68+560];
  t5  = LANE_T0[LANE_B3(s55)] ^ LANE_T1[LANE_B2(s56)] ^ LANE_T2[LANE_B1(s57)] ^ LANE_T3[LANE_B0(s54)] ^ LANE_C[ 69+560];
  t9  = LANE_T0[LANE_B3(s56)] ^ LANE_T1[LANE_B2(s57)] ^ LANE_T2[LANE_B1(s54)] ^ LANE_T3[LANE_B0(s55)] ^ LANE_C[ 70+560];
  td  = LANE_T0[LANE_B3(s57)] ^ LANE_T1[LANE_B2(s54)] ^ LANE_T2[LANE_B1(s55)] ^ LANE_T3[LANE_B0(s56)] ^ LANE_C[ 71+560];
  t2  = LANE_T0[LANE_B3(s58)] ^ LANE_T1[LANE_B2(s59)] ^ LANE_T2[LANE_B1(s5a)] ^ LANE_T3[LANE_B0(s5b)] ^ LANE_C[ 72+560];
  t6  = LANE_T0[LANE_B3(s59)] ^ LANE_T1[LANE_B2(s5a)] ^ LANE_T2[LANE_B1(s5b)] ^ LANE_T3[LANE_B0(s58)] ^ LANE_C[ 73+560];
  ta  = LANE_T0[LANE_B3(s5a)] ^ LANE_T1[LANE_B2(s5b)] ^ LANE_T2[LANE_B1(s58)] ^ LANE_T3[LANE_B0(s59)] ^ LANE_C[ 74+560];
  te  = LANE_T0[LANE_B3(s5b)] ^ LANE_T1[LANE_B2(s58)] ^ LANE_T2[LANE_B1(s59)] ^ LANE_T3[LANE_B0(s5a)] ^ LANE_C[ 75+560];
  t3  = LANE_T0[LANE_B3(s5c)] ^ LANE_T1[LANE_B2(s5d)] ^ LANE_T2[LANE_B1(s5e)] ^ LANE_T3[LANE_B0(s5f)] ^ LANE_C[ 76+560];
  t7  = LANE_T0[LANE_B3(s5d)] ^ LANE_T1[LANE_B2(s5e)] ^ LANE_T2[LANE_B1(s5f)] ^ LANE_T3[LANE_B0(s5c)] ^ LANE_C[ 77+560];
  tb  = LANE_T0[LANE_B3(s5e)] ^ LANE_T1[LANE_B2(s5f)] ^ LANE_T2[LANE_B1(s5c)] ^ LANE_T3[LANE_B0(s5d)] ^ LANE_C[ 78+560];
  tf  = LANE_T0[LANE_B3(s5f)] ^ LANE_T1[LANE_B2(s5c)] ^ LANE_T2[LANE_B1(s5d)] ^ LANE_T3[LANE_B0(s5e)] ^ LANE_C[ 79+560];

  s50 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 80+560];
  s54 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 81+560];
  s58 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 82+560];
  s5c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 83+560] ^ ctrh;
  s51 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 84+560];
  s55 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 85+560];
  s59 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 86+560];
  s5d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 87+560];
  s52 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 88+560];
  s56 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 89+560];
  s5a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 90+560];
  s5e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 91+560];
  s53 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 92+560];
  s57 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 93+560];
  s5b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 94+560];
  s5f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 95+560];

  t0  = LANE_T0[LANE_B3(s50)] ^ LANE_T1[LANE_B2(s51)] ^ LANE_T2[LANE_B1(s52)] ^ LANE_T3[LANE_B0(s53)] ^ LANE_C[ 96+560];
  t4  = LANE_T0[LANE_B3(s51)] ^ LANE_T1[LANE_B2(s52)] ^ LANE_T2[LANE_B1(s53)] ^ LANE_T3[LANE_B0(s50)] ^ LANE_C[ 97+560];
  t8  = LANE_T0[LANE_B3(s52)] ^ LANE_T1[LANE_B2(s53)] ^ LANE_T2[LANE_B1(s50)] ^ LANE_T3[LANE_B0(s51)] ^ LANE_C[ 98+560];
  tc  = LANE_T0[LANE_B3(s53)] ^ LANE_T1[LANE_B2(s50)] ^ LANE_T2[LANE_B1(s51)] ^ LANE_T3[LANE_B0(s52)] ^ LANE_C[ 99+560] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s54)] ^ LANE_T1[LANE_B2(s55)] ^ LANE_T2[LANE_B1(s56)] ^ LANE_T3[LANE_B0(s57)] ^ LANE_C[100+560];
  t5  = LANE_T0[LANE_B3(s55)] ^ LANE_T1[LANE_B2(s56)] ^ LANE_T2[LANE_B1(s57)] ^ LANE_T3[LANE_B0(s54)] ^ LANE_C[101+560];
  t9  = LANE_T0[LANE_B3(s56)] ^ LANE_T1[LANE_B2(s57)] ^ LANE_T2[LANE_B1(s54)] ^ LANE_T3[LANE_B0(s55)] ^ LANE_C[102+560];
  td  = LANE_T0[LANE_B3(s57)] ^ LANE_T1[LANE_B2(s54)] ^ LANE_T2[LANE_B1(s55)] ^ LANE_T3[LANE_B0(s56)] ^ LANE_C[103+560];
  t2  = LANE_T0[LANE_B3(s58)] ^ LANE_T1[LANE_B2(s59)] ^ LANE_T2[LANE_B1(s5a)] ^ LANE_T3[LANE_B0(s5b)] ^ LANE_C[104+560];
  t6  = LANE_T0[LANE_B3(s59)] ^ LANE_T1[LANE_B2(s5a)] ^ LANE_T2[LANE_B1(s5b)] ^ LANE_T3[LANE_B0(s58)] ^ LANE_C[105+560];
  ta  = LANE_T0[LANE_B3(s5a)] ^ LANE_T1[LANE_B2(s5b)] ^ LANE_T2[LANE_B1(s58)] ^ LANE_T3[LANE_B0(s59)] ^ LANE_C[106+560];
  te  = LANE_T0[LANE_B3(s5b)] ^ LANE_T1[LANE_B2(s58)] ^ LANE_T2[LANE_B1(s59)] ^ LANE_T3[LANE_B0(s5a)] ^ LANE_C[107+560];
  t3  = LANE_T0[LANE_B3(s5c)] ^ LANE_T1[LANE_B2(s5d)] ^ LANE_T2[LANE_B1(s5e)] ^ LANE_T3[LANE_B0(s5f)] ^ LANE_C[108+560];
  t7  = LANE_T0[LANE_B3(s5d)] ^ LANE_T1[LANE_B2(s5e)] ^ LANE_T2[LANE_B1(s5f)] ^ LANE_T3[LANE_B0(s5c)] ^ LANE_C[109+560];
  tb  = LANE_T0[LANE_B3(s5e)] ^ LANE_T1[LANE_B2(s5f)] ^ LANE_T2[LANE_B1(s5c)] ^ LANE_T3[LANE_B0(s5d)] ^ LANE_C[110+560];
  tf  = LANE_T0[LANE_B3(s5f)] ^ LANE_T1[LANE_B2(s5c)] ^ LANE_T2[LANE_B1(s5d)] ^ LANE_T3[LANE_B0(s5e)] ^ LANE_C[111+560];

  s70 ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  s74 ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  s78 ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  s7c ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  s71 ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  s75 ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  s79 ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  s7d ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  s72 ^= LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )];
  s76 ^= LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )];
  s7a ^= LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )];
  s7e ^= LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )];
  s73 ^= LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )];
  s77 ^= LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )];
  s7b ^= LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )];
  s7f ^= LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )];

  /* Lane 6 */
  t0  = LANE_T0[LANE_B3(s60)] ^ LANE_T1[LANE_B2(s61)] ^ LANE_T2[LANE_B1(s62)] ^ LANE_T3[LANE_B0(s63)] ^ LANE_C[  0+672];
  t4  = LANE_T0[LANE_B3(s61)] ^ LANE_T1[LANE_B2(s62)] ^ LANE_T2[LANE_B1(s63)] ^ LANE_T3[LANE_B0(s60)] ^ LANE_C[  1+672];
  t8  = LANE_T0[LANE_B3(s62)] ^ LANE_T1[LANE_B2(s63)] ^ LANE_T2[LANE_B1(s60)] ^ LANE_T3[LANE_B0(s61)] ^ LANE_C[  2+672];
  tc  = LANE_T0[LANE_B3(s63)] ^ LANE_T1[LANE_B2(s60)] ^ LANE_T2[LANE_B1(s61)] ^ LANE_T3[LANE_B0(s62)] ^ LANE_C[  3+672] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s64)] ^ LANE_T1[LANE_B2(s65)] ^ LANE_T2[LANE_B1(s66)] ^ LANE_T3[LANE_B0(s67)] ^ LANE_C[  4+672];
  t5  = LANE_T0[LANE_B3(s65)] ^ LANE_T1[LANE_B2(s66)] ^ LANE_T2[LANE_B1(s67)] ^ LANE_T3[LANE_B0(s64)] ^ LANE_C[  5+672];
  t9  = LANE_T0[LANE_B3(s66)] ^ LANE_T1[LANE_B2(s67)] ^ LANE_T2[LANE_B1(s64)] ^ LANE_T3[LANE_B0(s65)] ^ LANE_C[  6+672];
  td  = LANE_T0[LANE_B3(s67)] ^ LANE_T1[LANE_B2(s64)] ^ LANE_T2[LANE_B1(s65)] ^ LANE_T3[LANE_B0(s66)] ^ LANE_C[  7+672];
  t2  = LANE_T0[LANE_B3(s68)] ^ LANE_T1[LANE_B2(s69)] ^ LANE_T2[LANE_B1(s6a)] ^ LANE_T3[LANE_B0(s6b)] ^ LANE_C[  8+672];
  t6  = LANE_T0[LANE_B3(s69)] ^ LANE_T1[LANE_B2(s6a)] ^ LANE_T2[LANE_B1(s6b)] ^ LANE_T3[LANE_B0(s68)] ^ LANE_C[  9+672];
  ta  = LANE_T0[LANE_B3(s6a)] ^ LANE_T1[LANE_B2(s6b)] ^ LANE_T2[LANE_B1(s68)] ^ LANE_T3[LANE_B0(s69)] ^ LANE_C[ 10+672];
  te  = LANE_T0[LANE_B3(s6b)] ^ LANE_T1[LANE_B2(s68)] ^ LANE_T2[LANE_B1(s69)] ^ LANE_T3[LANE_B0(s6a)] ^ LANE_C[ 11+672];
  t3  = LANE_T0[LANE_B3(s6c)] ^ LANE_T1[LANE_B2(s6d)] ^ LANE_T2[LANE_B1(s6e)] ^ LANE_T3[LANE_B0(s6f)] ^ LANE_C[ 12+672];
  t7  = LANE_T0[LANE_B3(s6d)] ^ LANE_T1[LANE_B2(s6e)] ^ LANE_T2[LANE_B1(s6f)] ^ LANE_T3[LANE_B0(s6c)] ^ LANE_C[ 13+672];
  tb  = LANE_T0[LANE_B3(s6e)] ^ LANE_T1[LANE_B2(s6f)] ^ LANE_T2[LANE_B1(s6c)] ^ LANE_T3[LANE_B0(s6d)] ^ LANE_C[ 14+672];
  tf  = LANE_T0[LANE_B3(s6f)] ^ LANE_T1[LANE_B2(s6c)] ^ LANE_T2[LANE_B1(s6d)] ^ LANE_T3[LANE_B0(s6e)] ^ LANE_C[ 15+672];

  s60 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 16+672];
  s64 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 17+672];
  s68 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 18+672];
  s6c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 19+672] ^ ctrl;
  s61 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 20+672];
  s65 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 21+672];
  s69 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 22+672];
  s6d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 23+672];
  s62 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 24+672];
  s66 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 25+672];
  s6a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 26+672];
  s6e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 27+672];
  s63 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 28+672];
  s67 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 29+672];
  s6b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 30+672];
  s6f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 31+672];

  t0  = LANE_T0[LANE_B3(s60)] ^ LANE_T1[LANE_B2(s61)] ^ LANE_T2[LANE_B1(s62)] ^ LANE_T3[LANE_B0(s63)] ^ LANE_C[ 32+672];
  t4  = LANE_T0[LANE_B3(s61)] ^ LANE_T1[LANE_B2(s62)] ^ LANE_T2[LANE_B1(s63)] ^ LANE_T3[LANE_B0(s60)] ^ LANE_C[ 33+672];
  t8  = LANE_T0[LANE_B3(s62)] ^ LANE_T1[LANE_B2(s63)] ^ LANE_T2[LANE_B1(s60)] ^ LANE_T3[LANE_B0(s61)] ^ LANE_C[ 34+672];
  tc  = LANE_T0[LANE_B3(s63)] ^ LANE_T1[LANE_B2(s60)] ^ LANE_T2[LANE_B1(s61)] ^ LANE_T3[LANE_B0(s62)] ^ LANE_C[ 35+672] ^ ctrh;
  t1  = LANE_T0[LANE_B3(s64)] ^ LANE_T1[LANE_B2(s65)] ^ LANE_T2[LANE_B1(s66)] ^ LANE_T3[LANE_B0(s67)] ^ LANE_C[ 36+672];
  t5  = LANE_T0[LANE_B3(s65)] ^ LANE_T1[LANE_B2(s66)] ^ LANE_T2[LANE_B1(s67)] ^ LANE_T3[LANE_B0(s64)] ^ LANE_C[ 37+672];
  t9  = LANE_T0[LANE_B3(s66)] ^ LANE_T1[LANE_B2(s67)] ^ LANE_T2[LANE_B1(s64)] ^ LANE_T3[LANE_B0(s65)] ^ LANE_C[ 38+672];
  td  = LANE_T0[LANE_B3(s67)] ^ LANE_T1[LANE_B2(s64)] ^ LANE_T2[LANE_B1(s65)] ^ LANE_T3[LANE_B0(s66)] ^ LANE_C[ 39+672];
  t2  = LANE_T0[LANE_B3(s68)] ^ LANE_T1[LANE_B2(s69)] ^ LANE_T2[LANE_B1(s6a)] ^ LANE_T3[LANE_B0(s6b)] ^ LANE_C[ 40+672];
  t6  = LANE_T0[LANE_B3(s69)] ^ LANE_T1[LANE_B2(s6a)] ^ LANE_T2[LANE_B1(s6b)] ^ LANE_T3[LANE_B0(s68)] ^ LANE_C[ 41+672];
  ta  = LANE_T0[LANE_B3(s6a)] ^ LANE_T1[LANE_B2(s6b)] ^ LANE_T2[LANE_B1(s68)] ^ LANE_T3[LANE_B0(s69)] ^ LANE_C[ 42+672];
  te  = LANE_T0[LANE_B3(s6b)] ^ LANE_T1[LANE_B2(s68)] ^ LANE_T2[LANE_B1(s69)] ^ LANE_T3[LANE_B0(s6a)] ^ LANE_C[ 43+672];
  t3  = LANE_T0[LANE_B3(s6c)] ^ LANE_T1[LANE_B2(s6d)] ^ LANE_T2[LANE_B1(s6e)] ^ LANE_T3[LANE_B0(s6f)] ^ LANE_C[ 44+672];
  t7  = LANE_T0[LANE_B3(s6d)] ^ LANE_T1[LANE_B2(s6e)] ^ LANE_T2[LANE_B1(s6f)] ^ LANE_T3[LANE_B0(s6c)] ^ LANE_C[ 45+672];
  tb  = LANE_T0[LANE_B3(s6e)] ^ LANE_T1[LANE_B2(s6f)] ^ LANE_T2[LANE_B1(s6c)] ^ LANE_T3[LANE_B0(s6d)] ^ LANE_C[ 46+672];
  tf  = LANE_T0[LANE_B3(s6f)] ^ LANE_T1[LANE_B2(s6c)] ^ LANE_T2[LANE_B1(s6d)] ^ LANE_T3[LANE_B0(s6e)] ^ LANE_C[ 47+672];

  h[ 0] = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  h[ 4] = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  h[ 8] = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  h[12] = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  h[ 1] = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  h[ 5] = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  h[ 9] = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  h[13] = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  h[ 2] = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )];
  h[ 6] = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )];
  h[10] = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )];
  h[14] = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )];
  h[ 3] = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )];
  h[ 7] = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )];
  h[11] = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )];
  h[15] = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )];

  /* Lane 7 */
  t0  = LANE_T0[LANE_B3(s70)] ^ LANE_T1[LANE_B2(s71)] ^ LANE_T2[LANE_B1(s72)] ^ LANE_T3[LANE_B0(s73)] ^ LANE_C[  0+720];
  t4  = LANE_T0[LANE_B3(s71)] ^ LANE_T1[LANE_B2(s72)] ^ LANE_T2[LANE_B1(s73)] ^ LANE_T3[LANE_B0(s70)] ^ LANE_C[  1+720];
  t8  = LANE_T0[LANE_B3(s72)] ^ LANE_T1[LANE_B2(s73)] ^ LANE_T2[LANE_B1(s70)] ^ LANE_T3[LANE_B0(s71)] ^ LANE_C[  2+720];
  tc  = LANE_T0[LANE_B3(s73)] ^ LANE_T1[LANE_B2(s70)] ^ LANE_T2[LANE_B1(s71)] ^ LANE_T3[LANE_B0(s72)] ^ LANE_C[  3+720] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s74)] ^ LANE_T1[LANE_B2(s75)] ^ LANE_T2[LANE_B1(s76)] ^ LANE_T3[LANE_B0(s77)] ^ LANE_C[  4+720];
  t5  = LANE_T0[LANE_B3(s75)] ^ LANE_T1[LANE_B2(s76)] ^ LANE_T2[LANE_B1(s77)] ^ LANE_T3[LANE_B0(s74)] ^ LANE_C[  5+720];
  t9  = LANE_T0[LANE_B3(s76)] ^ LANE_T1[LANE_B2(s77)] ^ LANE_T2[LANE_B1(s74)] ^ LANE_T3[LANE_B0(s75)] ^ LANE_C[  6+720];
  td  = LANE_T0[LANE_B3(s77)] ^ LANE_T1[LANE_B2(s74)] ^ LANE_T2[LANE_B1(s75)] ^ LANE_T3[LANE_B0(s76)] ^ LANE_C[  7+720];
  t2  = LANE_T0[LANE_B3(s78)] ^ LANE_T1[LANE_B2(s79)] ^ LANE_T2[LANE_B1(s7a)] ^ LANE_T3[LANE_B0(s7b)] ^ LANE_C[  8+720];
  t6  = LANE_T0[LANE_B3(s79)] ^ LANE_T1[LANE_B2(s7a)] ^ LANE_T2[LANE_B1(s7b)] ^ LANE_T3[LANE_B0(s78)] ^ LANE_C[  9+720];
  ta  = LANE_T0[LANE_B3(s7a)] ^ LANE_T1[LANE_B2(s7b)] ^ LANE_T2[LANE_B1(s78)] ^ LANE_T3[LANE_B0(s79)] ^ LANE_C[ 10+720];
  te  = LANE_T0[LANE_B3(s7b)] ^ LANE_T1[LANE_B2(s78)] ^ LANE_T2[LANE_B1(s79)] ^ LANE_T3[LANE_B0(s7a)] ^ LANE_C[ 11+720];
  t3  = LANE_T0[LANE_B3(s7c)] ^ LANE_T1[LANE_B2(s7d)] ^ LANE_T2[LANE_B1(s7e)] ^ LANE_T3[LANE_B0(s7f)] ^ LANE_C[ 12+720];
  t7  = LANE_T0[LANE_B3(s7d)] ^ LANE_T1[LANE_B2(s7e)] ^ LANE_T2[LANE_B1(s7f)] ^ LANE_T3[LANE_B0(s7c)] ^ LANE_C[ 13+720];
  tb  = LANE_T0[LANE_B3(s7e)] ^ LANE_T1[LANE_B2(s7f)] ^ LANE_T2[LANE_B1(s7c)] ^ LANE_T3[LANE_B0(s7d)] ^ LANE_C[ 14+720];
  tf  = LANE_T0[LANE_B3(s7f)] ^ LANE_T1[LANE_B2(s7c)] ^ LANE_T2[LANE_B1(s7d)] ^ LANE_T3[LANE_B0(s7e)] ^ LANE_C[ 15+720];

  s70 = LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )] ^ LANE_C[ 16+720];
  s74 = LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )] ^ LANE_C[ 17+720];
  s78 = LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )] ^ LANE_C[ 18+720];
  s7c = LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )] ^ LANE_C[ 19+720] ^ ctrh;
  s71 = LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )] ^ LANE_C[ 20+720];
  s75 = LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )] ^ LANE_C[ 21+720];
  s79 = LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )] ^ LANE_C[ 22+720];
  s7d = LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )] ^ LANE_C[ 23+720];
  s72 = LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )] ^ LANE_C[ 24+720];
  s76 = LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )] ^ LANE_C[ 25+720];
  s7a = LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )] ^ LANE_C[ 26+720];
  s7e = LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )] ^ LANE_C[ 27+720];
  s73 = LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )] ^ LANE_C[ 28+720];
  s77 = LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )] ^ LANE_C[ 29+720];
  s7b = LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )] ^ LANE_C[ 30+720];
  s7f = LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )] ^ LANE_C[ 31+720];

  t0  = LANE_T0[LANE_B3(s70)] ^ LANE_T1[LANE_B2(s71)] ^ LANE_T2[LANE_B1(s72)] ^ LANE_T3[LANE_B0(s73)] ^ LANE_C[ 32+720];
  t4  = LANE_T0[LANE_B3(s71)] ^ LANE_T1[LANE_B2(s72)] ^ LANE_T2[LANE_B1(s73)] ^ LANE_T3[LANE_B0(s70)] ^ LANE_C[ 33+720];
  t8  = LANE_T0[LANE_B3(s72)] ^ LANE_T1[LANE_B2(s73)] ^ LANE_T2[LANE_B1(s70)] ^ LANE_T3[LANE_B0(s71)] ^ LANE_C[ 34+720];
  tc  = LANE_T0[LANE_B3(s73)] ^ LANE_T1[LANE_B2(s70)] ^ LANE_T2[LANE_B1(s71)] ^ LANE_T3[LANE_B0(s72)] ^ LANE_C[ 35+720] ^ ctrl;
  t1  = LANE_T0[LANE_B3(s74)] ^ LANE_T1[LANE_B2(s75)] ^ LANE_T2[LANE_B1(s76)] ^ LANE_T3[LANE_B0(s77)] ^ LANE_C[ 36+720];
  t5  = LANE_T0[LANE_B3(s75)] ^ LANE_T1[LANE_B2(s76)] ^ LANE_T2[LANE_B1(s77)] ^ LANE_T3[LANE_B0(s74)] ^ LANE_C[ 37+720];
  t9  = LANE_T0[LANE_B3(s76)] ^ LANE_T1[LANE_B2(s77)] ^ LANE_T2[LANE_B1(s74)] ^ LANE_T3[LANE_B0(s75)] ^ LANE_C[ 38+720];
  td  = LANE_T0[LANE_B3(s77)] ^ LANE_T1[LANE_B2(s74)] ^ LANE_T2[LANE_B1(s75)] ^ LANE_T3[LANE_B0(s76)] ^ LANE_C[ 39+720];
  t2  = LANE_T0[LANE_B3(s78)] ^ LANE_T1[LANE_B2(s79)] ^ LANE_T2[LANE_B1(s7a)] ^ LANE_T3[LANE_B0(s7b)] ^ LANE_C[ 40+720];
  t6  = LANE_T0[LANE_B3(s79)] ^ LANE_T1[LANE_B2(s7a)] ^ LANE_T2[LANE_B1(s7b)] ^ LANE_T3[LANE_B0(s78)] ^ LANE_C[ 41+720];
  ta  = LANE_T0[LANE_B3(s7a)] ^ LANE_T1[LANE_B2(s7b)] ^ LANE_T2[LANE_B1(s78)] ^ LANE_T3[LANE_B0(s79)] ^ LANE_C[ 42+720];
  te  = LANE_T0[LANE_B3(s7b)] ^ LANE_T1[LANE_B2(s78)] ^ LANE_T2[LANE_B1(s79)] ^ LANE_T3[LANE_B0(s7a)] ^ LANE_C[ 43+720];
  t3  = LANE_T0[LANE_B3(s7c)] ^ LANE_T1[LANE_B2(s7d)] ^ LANE_T2[LANE_B1(s7e)] ^ LANE_T3[LANE_B0(s7f)] ^ LANE_C[ 44+720];
  t7  = LANE_T0[LANE_B3(s7d)] ^ LANE_T1[LANE_B2(s7e)] ^ LANE_T2[LANE_B1(s7f)] ^ LANE_T3[LANE_B0(s7c)] ^ LANE_C[ 45+720];
  tb  = LANE_T0[LANE_B3(s7e)] ^ LANE_T1[LANE_B2(s7f)] ^ LANE_T2[LANE_B1(s7c)] ^ LANE_T3[LANE_B0(s7d)] ^ LANE_C[ 46+720];
  tf  = LANE_T0[LANE_B3(s7f)] ^ LANE_T1[LANE_B2(s7c)] ^ LANE_T2[LANE_B1(s7d)] ^ LANE_T3[LANE_B0(s7e)] ^ LANE_C[ 47+720];

  h[ 0] ^= LANE_T0[LANE_B3(t0 )] ^ LANE_T1[LANE_B2(t1 )] ^ LANE_T2[LANE_B1(t2 )] ^ LANE_T3[LANE_B0(t3 )];
  h[ 4] ^= LANE_T0[LANE_B3(t1 )] ^ LANE_T1[LANE_B2(t2 )] ^ LANE_T2[LANE_B1(t3 )] ^ LANE_T3[LANE_B0(t0 )];
  h[ 8] ^= LANE_T0[LANE_B3(t2 )] ^ LANE_T1[LANE_B2(t3 )] ^ LANE_T2[LANE_B1(t0 )] ^ LANE_T3[LANE_B0(t1 )];
  h[12] ^= LANE_T0[LANE_B3(t3 )] ^ LANE_T1[LANE_B2(t0 )] ^ LANE_T2[LANE_B1(t1 )] ^ LANE_T3[LANE_B0(t2 )];
  h[ 1] ^= LANE_T0[LANE_B3(t4 )] ^ LANE_T1[LANE_B2(t5 )] ^ LANE_T2[LANE_B1(t6 )] ^ LANE_T3[LANE_B0(t7 )];
  h[ 5] ^= LANE_T0[LANE_B3(t5 )] ^ LANE_T1[LANE_B2(t6 )] ^ LANE_T2[LANE_B1(t7 )] ^ LANE_T3[LANE_B0(t4 )];
  h[ 9] ^= LANE_T0[LANE_B3(t6 )] ^ LANE_T1[LANE_B2(t7 )] ^ LANE_T2[LANE_B1(t4 )] ^ LANE_T3[LANE_B0(t5 )];
  h[13] ^= LANE_T0[LANE_B3(t7 )] ^ LANE_T1[LANE_B2(t4 )] ^ LANE_T2[LANE_B1(t5 )] ^ LANE_T3[LANE_B0(t6 )];
  h[ 2] ^= LANE_T0[LANE_B3(t8 )] ^ LANE_T1[LANE_B2(t9 )] ^ LANE_T2[LANE_B1(ta )] ^ LANE_T3[LANE_B0(tb )];
  h[ 6] ^= LANE_T0[LANE_B3(t9 )] ^ LANE_T1[LANE_B2(ta )] ^ LANE_T2[LANE_B1(tb )] ^ LANE_T3[LANE_B0(t8 )];
  h[10] ^= LANE_T0[LANE_B3(ta )] ^ LANE_T1[LANE_B2(tb )] ^ LANE_T2[LANE_B1(t8 )] ^ LANE_T3[LANE_B0(t9 )];
  h[14] ^= LANE_T0[LANE_B3(tb )] ^ LANE_T1[LANE_B2(t8 )] ^ LANE_T2[LANE_B1(t9 )] ^ LANE_T3[LANE_B0(ta )];
  h[ 3] ^= LANE_T0[LANE_B3(tc )] ^ LANE_T1[LANE_B2(td )] ^ LANE_T2[LANE_B1(te )] ^ LANE_T3[LANE_B0(tf )];
  h[ 7] ^= LANE_T0[LANE_B3(td )] ^ LANE_T1[LANE_B2(te )] ^ LANE_T2[LANE_B1(tf )] ^ LANE_T3[LANE_B0(tc )];
  h[11] ^= LANE_T0[LANE_B3(te )] ^ LANE_T1[LANE_B2(tf )] ^ LANE_T2[LANE_B1(tc )] ^ LANE_T3[LANE_B0(td )];
  h[15] ^= LANE_T0[LANE_B3(tf )] ^ LANE_T1[LANE_B2(tc )] ^ LANE_T2[LANE_B1(td )] ^ LANE_T3[LANE_B0(te )];
}

int Lane::Init (int hashbitlen)
{
  if (hashbitlen != 224 && hashbitlen != 256 && hashbitlen != 384 && hashbitlen != 512) 
    return BAD_HASHBITLEN;

  laneState.hashbitlen = hashbitlen;
  laneState.ctr = 0;
  memset(laneState.h, 0, 64);
  memset(laneState.buffer, 0, 128);
  laneState.buffer[0] = 0x02; /* flag byte 0x02: IV derivation without seed */
  laneState.buffer[1] = (laneState.hashbitlen>>24) & 0xff; /* digest length in big endian notation */
  laneState.buffer[2] = (laneState.hashbitlen>>16) & 0xff;
  laneState.buffer[3] = (laneState.hashbitlen>> 8) & 0xff;
  laneState.buffer[4] = (laneState.hashbitlen>> 0) & 0xff;

  switch (laneState.hashbitlen) {
    case 224: case 256: default:
      Lane::lane256_compress(laneState.buffer, laneState.h, 0, 0);
      break;
    case 384: case 512:
      Lane::lane512_compress(laneState.buffer, laneState.h, 0, 0);
      break;
  }

  return SUCCESS;
}

int Lane::Update (const BitSequence *data, DataLength databitlen)
{
  unsigned long long buffill;
  unsigned long long bytes;

  switch (laneState.hashbitlen) {
    case 224: case 256: default:
      buffill = (laneState.ctr >> 3) & 0x3f;
      bytes = databitlen >> 3;

      if (laneState.ctr & 0x7)
        return BAD_DATABITLEN; /* Only the last call to Update() may contain a fractional byte */

      /* Check if we have some stuff left in the buffer. If so, fill it, and process it */
      if (buffill) {
        const unsigned long long n = buffill + bytes > 64 ? 64-buffill : bytes; /* number of bytes to copy */
        memcpy(laneState.buffer + buffill, data, n);
        laneState.ctr += n << 3;
        if (buffill + n == 64) /* full buffer now */
          Lane::lane256_compress(laneState.buffer, laneState.h, LANE_MSB32(laneState.ctr), LANE_LSB32(laneState.ctr));
        data += n;
        bytes -= n;
      }

      /* Now process as many full blocks as we can directly from the input message */
      while (bytes >= 64) {
        laneState.ctr += 64 << 3;
		Lane::lane256_compress(data, laneState.h, LANE_MSB32(laneState.ctr), LANE_LSB32(laneState.ctr));
        data += 64;
        bytes -= 64;
      }
      break;

    case 384: case 512:
      buffill = (laneState.ctr >> 3) & 0x7f;
      bytes = databitlen >> 3;

      if (laneState.ctr & 0x7)
        return BAD_DATABITLEN; /* Only the last call to Update() may contain a fractional byte */

      /* Check if we have some stuff left in the buffer. If so, fill it, and process it */
      if (buffill) {
        const unsigned long long n = buffill + bytes > 128 ? 128-buffill : bytes; /* number of bytes to copy */
        memcpy(laneState.buffer + buffill, data, n);
        laneState.ctr += n << 3;
        if (buffill + n == 128) /* full buffer now */
          Lane::lane512_compress(laneState.buffer, laneState.h, LANE_MSB32(laneState.ctr), LANE_LSB32(laneState.ctr));
        data += n;
        bytes -= n;
      }

      /* Now process as many full blocks as we can directly from the input message */
      while (bytes >= 128) {
        laneState.ctr += 128 << 3;
        Lane::lane512_compress(data, laneState.h, LANE_MSB32(laneState.ctr), LANE_LSB32(laneState.ctr));
        data += 128;
        bytes -= 128;
      }
      break;
  }

  /* And finally, save the last, incomplete message block */
  if (bytes || (databitlen & 0x7)) {
    memcpy(laneState.buffer, data, databitlen & 0x7 ? bytes+1 : bytes); /* also copy partial byte */
    laneState.ctr += (bytes << 3) + (databitlen & 0x7);
  }

  return SUCCESS;
}

int Lane::Final (BitSequence *hashval)
{

  switch (laneState.hashbitlen) {
    case 224: case 256: default:
      /* do zero padding and compress last block, if there is some data in the buffer */
      if (laneState.ctr & 0x1ff) {
        const unsigned long long n = (((laneState.ctr - 1) >> 3) + 1) & 0x3f; /* number of bytes in buffer that are (partially) filled */
        if (n < 64)
          memset(laneState.buffer + n, 0, 64-n);
        laneState.buffer[(laneState.ctr >> 3)&0x3f] &= ~(0xff >> (laneState.ctr & 0x7)); /* zero-pad partial byte */
        Lane::lane256_compress(laneState.buffer, laneState.h, LANE_MSB32(laneState.ctr), LANE_LSB32(laneState.ctr));
      }

      /* output transformation */
      memset(laneState.buffer, 0, 64);
      laneState.buffer[0] = 0x00; /* flag byte 0x00: output transformation without seed */
      laneState.buffer[1] = LANE_T8(laneState.ctr >> 56); /* message length in big-endian */
      laneState.buffer[2] = LANE_T8(laneState.ctr >> 48);
      laneState.buffer[3] = LANE_T8(laneState.ctr >> 40);
      laneState.buffer[4] = LANE_T8(laneState.ctr >> 32);
      laneState.buffer[5] = LANE_T8(laneState.ctr >> 24);
      laneState.buffer[6] = LANE_T8(laneState.ctr >> 16);
      laneState.buffer[7] = LANE_T8(laneState.ctr >>  8);
      laneState.buffer[8] = LANE_T8(laneState.ctr >>  0);
      Lane::lane256_compress(laneState.buffer, laneState.h, 0, 0);

      /* write back result */
      LANE_U32TO8_BIG(hashval,    laneState.h[0]);
      LANE_U32TO8_BIG(hashval+4,  laneState.h[1]);
      LANE_U32TO8_BIG(hashval+8,  laneState.h[2]);
      LANE_U32TO8_BIG(hashval+12, laneState.h[3]);
      LANE_U32TO8_BIG(hashval+16, laneState.h[4]);
      LANE_U32TO8_BIG(hashval+20, laneState.h[5]);
      LANE_U32TO8_BIG(hashval+24, laneState.h[6]);
      LANE_U32TO8_BIG(hashval+28, laneState.h[7]);

      break;

    case 384: case 512:
      /* do zero padding and compress last block, if there is some data in the buffer */
      if (laneState.ctr & 0x3ff) {
        const unsigned long long n = (((laneState.ctr - 1) >> 3) + 1) & 0x7f; /* number of bytes in buffer that are (partially) filled */
        if (n < 128)
          memset(laneState.buffer + n, 0, 128-n);
        laneState.buffer[(laneState.ctr >> 3)&0x7f] &= ~(0xff >> (laneState.ctr & 0x7)); /* zero-pad partial byte */
        Lane::lane512_compress(laneState.buffer, laneState.h, LANE_MSB32(laneState.ctr), LANE_LSB32(laneState.ctr));
      }

      /* output transformation */
      memset(laneState.buffer, 0, 128);
      laneState.buffer[0] = 0x00; /* flag byte 0x00: output transformation without seed */
      laneState.buffer[1] = LANE_T8(laneState.ctr >> 56); /* message length in big-endian */
      laneState.buffer[2] = LANE_T8(laneState.ctr >> 48);
      laneState.buffer[3] = LANE_T8(laneState.ctr >> 40);
      laneState.buffer[4] = LANE_T8(laneState.ctr >> 32);
      laneState.buffer[5] = LANE_T8(laneState.ctr >> 24);
      laneState.buffer[6] = LANE_T8(laneState.ctr >> 16);
      laneState.buffer[7] = LANE_T8(laneState.ctr >>  8);
      laneState.buffer[8] = LANE_T8(laneState.ctr >>  0);
      Lane::lane512_compress(laneState.buffer, laneState.h, 0, 0);

      /* write back result */
      LANE_U32TO8_BIG(hashval,    laneState.h[0]);
      LANE_U32TO8_BIG(hashval+4,  laneState.h[1]);
      LANE_U32TO8_BIG(hashval+8,  laneState.h[2]);
      LANE_U32TO8_BIG(hashval+12, laneState.h[3]);
      LANE_U32TO8_BIG(hashval+16, laneState.h[4]);
      LANE_U32TO8_BIG(hashval+20, laneState.h[5]);
      LANE_U32TO8_BIG(hashval+24, laneState.h[6]);
      LANE_U32TO8_BIG(hashval+28, laneState.h[7]);
      LANE_U32TO8_BIG(hashval+32, laneState.h[8]);
      LANE_U32TO8_BIG(hashval+36, laneState.h[9]);
      LANE_U32TO8_BIG(hashval+40, laneState.h[10]);
      LANE_U32TO8_BIG(hashval+44, laneState.h[11]);
      LANE_U32TO8_BIG(hashval+48, laneState.h[12]);
      LANE_U32TO8_BIG(hashval+52, laneState.h[13]);
      LANE_U32TO8_BIG(hashval+56, laneState.h[14]);
      LANE_U32TO8_BIG(hashval+60, laneState.h[15]);

      break;
  }

  return SUCCESS;
}

int Lane::Hash (int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
  //hashState state;
  //HashReturn hashReturn;
  int hashReturn;

  if ((hashReturn = Lane::Init(hashbitlen)) != SUCCESS)
    return hashReturn;
  if ((hashReturn = Lane::Update(data, databitlen)) != SUCCESS)
    return hashReturn;
  if ((hashReturn = Lane::Final(hashval)) != SUCCESS)
    return hashReturn;
  return SUCCESS;
}

Lane::Lane(const int numRounds) {
	//if (numRounds == -1) {
		numRoundsLane224P = LANE_224_P_DEFAULT;
		numRoundsLane224Q = LANE_224_Q_DEFAULT;
		numRoundsLane384P = LANE_384_P_DEFAULT;
		numRoundsLane384Q = LANE_384_Q_DEFAULT;
	/*} else {
		numRoundsLane224P = numRounds;
		numRoundsLane224Q = numRounds;
		numRoundsLane384P = numRounds;
		numRoundsLane384Q = numRounds;
	}*/
}

} // namespace sha3
