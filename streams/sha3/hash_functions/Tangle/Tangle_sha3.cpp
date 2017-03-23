#include "Tangle_sha3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TANGLE_ROTL(x, n)		((x << n) | (x >> (32 - n)))

#define TANGLE_F1(x, y, z)		((x&(y|z)) | (y&z))
#define TANGLE_F2(x, y, z)		(((~y)&(x|z)) | (x&z))
#define TANGLE_FR1(x) (TANGLE_ROTL((x), 3) ^ TANGLE_ROTL((x), 13) ^ TANGLE_ROTL((x), 29))
#define TANGLE_FR2(x) (TANGLE_ROTL((x), 5) ^ TANGLE_ROTL((x), 19) ^ TANGLE_ROTL((x), 27))

#define TANGLE_NumBytes(x) (((x-1)/8)+1)


const unsigned char TANGLE_SBOX[256]= {
0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,  
0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,  
0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,  
0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,  
0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,  
0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,  
0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,  
0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,  
0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,  
0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,  
0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,  
0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,  
0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,  
0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,  
0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};


const unsigned int TANGLE_K[256] = {
0x9F683847, 0xA86DDDB9, 0x038C8548, 0x456136CC, 
0xCE0FE733, 0x48C7FAA6, 0xA29496F5, 0x1A0794C3, 
0xC8B932A0, 0x4A6F2D99, 0xBE7B653B, 0x90C9ECE3, 
0xF96898DA, 0x172B2D94, 0x1316A3DF, 0xAF86FAA2, 
0xF147E0C5, 0xF39C4D03, 0x20269618, 0x206AAA28, 
0x3EC303DE, 0x6E16F908, 0x98B147E1, 0xF15FF19C, 
0x3E0C3435, 0xF8575ABB, 0x2EA0DD93, 0x22D67C2E, 
0x565FD931, 0xCA0E73F2, 0x24488FD4, 0x1119C879, 
0xECDFF1AE, 0x588E0551, 0x680DE64D, 0x655F7EBD, 
0x3076A1C8, 0x3473E980, 0x40746FD5, 0x5F1A6B8E, 
0x173CAF92, 0x2F69ED1F, 0xCE91E3C6, 0x79501ACA, 
0x74455E0C, 0x8EEA7FE9, 0x07FC1997, 0xBA999F1D, 
0xD88E5990, 0x22D48DD2, 0xECEC4CC2, 0xC299CCC2, 
0xEE5E0DCD, 0x6BD05FFF, 0xF3BF7174, 0xF9387724, 
0xD64B08AC, 0xE98A334B, 0x15143B47, 0x9682E7DE, 
0xE2C3E0ED, 0xA51C6DE2, 0x30CC0B74, 0x0C98CAC3, 
0x1F46B4C2, 0x0C663012, 0xDDD59F33, 0xB098F39D, 
0x52F1516F, 0x6CD7DEA7, 0x8D030345, 0x563800AF, 
0xEEA86C03, 0xAC777BFB, 0x4000B9B8, 0x27EB7ECC, 
0xF791F337, 0xBE99227B, 0x0993C681, 0x02ADBA31, 
0xDF20FCBF, 0x9B781E83, 0xC8462B15, 0x293DBECF, 
0xB59E1471, 0x3129871A, 0x83198ED0, 0xA182D79D, 
0x477DDC2B, 0x7E50DCB6, 0x6B1BF560, 0x0359C909, 
0xBCD7A1EE, 0xD430E67A, 0xF8AB9C7F, 0x7FC9D4BC, 
0x0A5D7A1F, 0x90B350ED, 0x3E5C223F, 0x29B985F0, 
0x7FE32105, 0x646EA33D, 0x0D0EBB3F, 0xBAF82CB5, 
0xF9A0B527, 0xE09FD17D, 0x0F13857D, 0x1DA27082, 
0x88EC0B35, 0x1F193D6E, 0x16839340, 0x919107D4, 
0x1FDE83D7, 0x47E5A845, 0x894E4FAB, 0x6643F8C3, 
0xE6391FBB, 0x811B6CFE, 0xAD698782, 0xF69E2C6F, 
0xE4C33A19, 0xC8A6EE75, 0xC0712289, 0x509804E9, 
0x5953F05A, 0xD02C72BD, 0x37796A52, 0x06F29B38, 
0xCB066212, 0x3D0B2166, 0xB2FAAD72, 0x719542A2, 
0x28884937, 0x706C16BB, 0x7CE9006F, 0x0D7558EB, 
0xBFF92645, 0x083C9601, 0x75878548, 0x8FCE3DA5, 
0x62B5AEB1, 0xFE856DA0, 0xFDAA5E4B, 0x300CDEA1, 
0x301A2680, 0xA05DB084, 0x585E7266, 0xF41A0094, 
0xEF495D8D, 0xE73682EB, 0x36A0B8A9, 0x0812A25B, 
0x30C32019, 0x8413ACA8, 0xFEBFF196, 0x143C5A8B, 
0xB61D8F17, 0x94B8C6C8, 0x009115E9, 0x60C8E6DE, 
0xC6AB8AD0, 0x62C287F2, 0xBFC45BEA, 0x5A167763, 
0xE172036B, 0x9922C21C, 0xBD77D51C, 0xD6739168, 
0x52CBF105, 0x8288F08E, 0x2221C5A3, 0xB437E2E9, 
0x7F9B2123, 0x025DD950, 0xA1D18BA5, 0x78C3339E, 
0x285A4DB7, 0xE4C50C88, 0x93DFCC88, 0x51C8B216, 
0x36E843A8, 0x3EC3718F, 0x6393AE27, 0xD6A86170, 
0xF8773754, 0xCB8E7E33, 0x89289878, 0xE9237BED, 
0x9299837D, 0x8A68846A, 0x17BF5287, 0xC3097090, 
0x5E5C96BA, 0x62563C61, 0x99B3A4F4, 0x6462FB09, 
0x2129255F, 0x5FA37CB9, 0x9DFB8C25, 0x3FAF16ED, 
0xCFC7837A, 0x94ABD216, 0x66040046, 0x6F65D521, 
0x272314EE, 0xDAB5EAD9, 0x332BEBA9, 0x0D069176, 
0x837CFAF2, 0x994C5F6B, 0xFB774869, 0xC8DA1A5D, 
0x3DE382D2, 0xA39A8F28, 0x93D88CCF, 0x72B4E88E, 
0x50A3D7A5, 0x16E140C8, 0x1A406566, 0xA71818B6, 
0x975AD647, 0x265C5D5A, 0xE0D9D117, 0xB6D1E53F, 
0x73830E84, 0x8D0D4212, 0x11F89404, 0x7F43E302, 
0x8A1BFEE1, 0x87A8EE8D, 0x87A9900E, 0xE568EC0C, 
0xAEF6F13A, 0x874FBDED, 0x025DB16E, 0x5F05388A, 
0xB3EED14E, 0x7BE3E015, 0x66E3A947, 0x51DE162F, 
0xB853CF7A, 0xAC0A41CB, 0x5CA91A7C, 0xF73F5159, 
0x4EFFA1AE, 0xE4EB2F90, 0x5B8F1C43, 0x4A5EB8A1, 
0x311B8E02, 0x6EE5A53A, 0x1F4C792F, 0x67C9811F, 
0xEEC113DE, 0xEEAF1560, 0x21A32208, 0x5971E94C};



int 
Tangle::Init(int hashbitlen)
{
	tangleState.rlen = 0;
	tangleState.total = 0;
	tangleState.hashbitlen = hashbitlen;
	memset(tangleState.rdata,0,1024);

	switch(hashbitlen)
	{
	case 224:
		tangleState.rounds = tangleNumRounds224;
		tangleState.H[0]  = 0x2D81CD94;
		tangleState.H[1]  = 0x0EFEE4A4;
		tangleState.H[2]  = 0x6411E45E;
		tangleState.H[3]  = 0x02A9FF92;
		tangleState.H[4]  = 0xF6A3CDE6;
		tangleState.H[5]  = 0x2D9C4F7A;
		tangleState.H[6]  = 0x7E168406;
		tangleState.H[7]  = 0x94270766;
		tangleState.H[8]  = 0xF8C7BCFC;
		tangleState.H[9]  = 0x01C29E48;
		tangleState.H[10] = 0x4964F602;
		tangleState.H[11] = 0xDA5559C5;
		tangleState.H[12] = 0xDAE1A0E2;
		tangleState.H[13] = 0x54AF2C56;
		tangleState.H[14] = 0xF92F98BC;
		tangleState.H[15] = 0xCC10C064;
		tangleState.H[16] = 0xF6C3A921;
		tangleState.H[17] = 0xA19AAE22;
		tangleState.H[18] = 0x5F72214D;
		tangleState.H[19] = 0xE92251A2;
		tangleState.H[20] = 0x32E3E9AD;
		tangleState.H[21] = 0x207FDA47;
		tangleState.H[22] = 0xAAD02A11;
		tangleState.H[23] = 0xC139E132;
		tangleState.H[24] = 0x78E679BD;
		tangleState.H[25] = 0x2CF949CA;
		tangleState.H[26] = 0x8C3BD9AE;
		tangleState.H[27] = 0xE0934D0C;
		tangleState.H[28] = 0x64493745;
		tangleState.H[29] = 0x7C3AE3F4;
		tangleState.H[30] = 0x67BA732A;
		tangleState.H[31] = 0xB7387BE6;
		
		break;

	case 256:
		tangleState.rounds = tangleNumRounds256;
		tangleState.H[0]  = 0x14B62D86;
		tangleState.H[1]  = 0x31CF379C;
		tangleState.H[2]  = 0x1BC6382A;
		tangleState.H[3]  = 0x752E03B3;
		tangleState.H[4]  = 0xD0346A2A;
		tangleState.H[5]  = 0xA1DC5B93;
		tangleState.H[6]  = 0xF9BB11D2;
		tangleState.H[7]  = 0xEB6A9A40;
		tangleState.H[8]  = 0x51B1D88B;
		tangleState.H[9]  = 0xBC5B1F79;
		tangleState.H[10] = 0x10B0880E;
		tangleState.H[11] = 0x9F0E71F4;
		tangleState.H[12] = 0x233E7C22;
		tangleState.H[13] = 0x1D440731;
		tangleState.H[14] = 0xA27BE5A3;
		tangleState.H[15] = 0xDFD7E6DB;
		tangleState.H[16] = 0x10D9EC9F;
		tangleState.H[17] = 0x96477F0F;
		tangleState.H[18] = 0x125AEDF6;
		tangleState.H[19] = 0x93E13CEF;
		tangleState.H[20] = 0xFAC6CCEF;
		tangleState.H[21] = 0x19198FFC;
		tangleState.H[22] = 0x6A34B4DC;
		tangleState.H[23] = 0x3A0273DB;
		tangleState.H[24] = 0xEB2C033E;
		tangleState.H[25] = 0x399C4D77;
		tangleState.H[26] = 0x2FF23788;
		tangleState.H[27] = 0x223EBB81;
		tangleState.H[28] = 0xE9ABCB8C;
		tangleState.H[29] = 0xF789D1BB;
		tangleState.H[30] = 0x558A6467;
		tangleState.H[31] = 0xB789A6A6;
		
		break;

	case 384:
		tangleState.rounds = tangleNumRounds384;
		tangleState.H[0]  = 0x3172088A;
		tangleState.H[1]  = 0xC1275C80;
		tangleState.H[2]  = 0x18D70952;
		tangleState.H[3]  = 0xBA9E320B;
		tangleState.H[4]  = 0x698805FD;
		tangleState.H[5]  = 0x15EAF20E;
		tangleState.H[6]  = 0x7B5F3CD8;
		tangleState.H[7]  = 0xA7177775;
		tangleState.H[8]  = 0xEABDA77A;
		tangleState.H[9]  = 0x6AD86E68;
		tangleState.H[10] = 0xE17F4681;
		tangleState.H[11] = 0x2D4E7819;
		tangleState.H[12] = 0x7A7147A4;
		tangleState.H[13] = 0x5347D5BD;
		tangleState.H[14] = 0xD480D25A;
		tangleState.H[15] = 0x86291634;
		tangleState.H[16] = 0xBCCD8AC6;
		tangleState.H[17] = 0x16BED703;
		tangleState.H[18] = 0xF072DBB2;
		tangleState.H[19] = 0x926DD0AC;
		tangleState.H[20] = 0x0EF7E4F4;
		tangleState.H[21] = 0x87E75157;
		tangleState.H[22] = 0x789EF87A;
		tangleState.H[23] = 0xA3777CCB;
		tangleState.H[24] = 0xAF529FFF;
		tangleState.H[25] = 0xA86F6BC9;
		tangleState.H[26] = 0xD71A5CA7;
		tangleState.H[27] = 0xFFCD49AC;
		tangleState.H[28] = 0x239FBEAA;
		tangleState.H[29] = 0xDEB289BA;
		tangleState.H[30] = 0x2F8CF07B;
		tangleState.H[31] = 0xBBD81D1F;
		break;

	case 512:
		tangleState.rounds = tangleNumRounds512;
		tangleState.H[0]  = 0x2DDC9F84;
		tangleState.H[1]  = 0x45453437;
		tangleState.H[2]  = 0xA814767A;
		tangleState.H[3]  = 0x80B37929;
		tangleState.H[4]  = 0x1594C746;
		tangleState.H[5]  = 0x1B7192CB;
		tangleState.H[6]  = 0xF64561C4;
		tangleState.H[7]  = 0x49D69618;
		tangleState.H[8]  = 0x2550F049;
		tangleState.H[9]  = 0xC70C3BCE;
		tangleState.H[10] = 0x911E61DD;
		tangleState.H[11] = 0x40B70783;
		tangleState.H[12] = 0x8AA81BC3;
		tangleState.H[13] = 0xD3CBE489;
		tangleState.H[14] = 0xA5E7B3FF;
		tangleState.H[15] = 0x5EDAE355;
		tangleState.H[16] = 0x1A598727;
		tangleState.H[17] = 0xC04C1FAF;
		tangleState.H[18] = 0x6C425555;
		tangleState.H[19] = 0x211FE69A;
		tangleState.H[20] = 0xB1CC595E;
		tangleState.H[21] = 0xDB52C712;
		tangleState.H[22] = 0xCB1C9827;
		tangleState.H[23] = 0xBE522CC8;
		tangleState.H[24] = 0x9D04749A;
		tangleState.H[25] = 0x681F688C;
		tangleState.H[26] = 0x75017205;
		tangleState.H[27] = 0x8766B913;
		tangleState.H[28] = 0xA40F41C7;
		tangleState.H[29] = 0xBF619CBC;
		tangleState.H[30] = 0x353AA9AD;
		tangleState.H[31] = 0x482B670A;
		break;

	case 768:
		tangleState.rounds = tangleNumRounds768;
		tangleState.H[0]  = 0x2768DAF7;
		tangleState.H[1]  = 0x183DBD23;
		tangleState.H[2]  = 0x011B2A31;
		tangleState.H[3]  = 0x66D0D9E5;
		tangleState.H[4]  = 0x941B3303;
		tangleState.H[5]  = 0x7B4E2E57;
		tangleState.H[6]  = 0xA859AEBC;
		tangleState.H[7]  = 0x9C90B881;
		tangleState.H[8]  = 0x0C9ECB28;
		tangleState.H[9]  = 0xD9E7BC6C;
		tangleState.H[10] = 0x71EB73B3;
		tangleState.H[11] = 0x48EB7471;
		tangleState.H[12] = 0x896353A8;
		tangleState.H[13] = 0xF419F7BA;
		tangleState.H[14] = 0x833220A3;
		tangleState.H[15] = 0x76D09FDF;
		tangleState.H[16] = 0x450F2B73;
		tangleState.H[17] = 0xF6E931C6;
		tangleState.H[18] = 0x6AB546F8;
		tangleState.H[19] = 0xCEE69121;
		tangleState.H[20] = 0x71FEEB03;
		tangleState.H[21] = 0xF661C7DA;
		tangleState.H[22] = 0x0F7C921E;
		tangleState.H[23] = 0x6042EFBE;
		tangleState.H[24] = 0xD7A57A06;
		tangleState.H[25] = 0x6E5D928D;
		tangleState.H[26] = 0x25F1BA9C;
		tangleState.H[27] = 0x39243A6C;
		tangleState.H[28] = 0x7AF733A2;
		tangleState.H[29] = 0xB98F8101;
		tangleState.H[30] = 0x283AEE61;
		tangleState.H[31] = 0xCF07EE78;
		break;

	case 1024:
		tangleState.rounds = tangleNumRounds1024;
		tangleState.H[0]  = 0xBB92EA10;
		tangleState.H[1]  = 0xFF86FDFD;
		tangleState.H[2]  = 0x0E1F2B97;
		tangleState.H[3]  = 0xE572FA81;
		tangleState.H[4]  = 0x72E75110;
		tangleState.H[5]  = 0x28BBCFFD;
		tangleState.H[6]  = 0x530D15A7;
		tangleState.H[7]  = 0x44C8B625;
		tangleState.H[8]  = 0x3D7D9807;
		tangleState.H[9]  = 0x5154F28B;
		tangleState.H[10] = 0x6F7422A3;
		tangleState.H[11] = 0xAAAF4235;
		tangleState.H[12] = 0x0EC432A0;
		tangleState.H[13] = 0xEBD5F5AE;
		tangleState.H[14] = 0x756407E0;
		tangleState.H[15] = 0x4ADE884B;
		tangleState.H[16] = 0xFAE93F19;
		tangleState.H[17] = 0x48759A21;
		tangleState.H[18] = 0x6232487A;
		tangleState.H[19] = 0xA11F615D;
		tangleState.H[20] = 0x50F232BF;
		tangleState.H[21] = 0x7C635649;
		tangleState.H[22] = 0x9465C0F8;
		tangleState.H[23] = 0x6E1BF0FB;
		tangleState.H[24] = 0xA5D532D3;
		tangleState.H[25] = 0x4325B826;
		tangleState.H[26] = 0x9269D4E5;
		tangleState.H[27] = 0xB3026513;
		tangleState.H[28] = 0x13C5AF31;
		tangleState.H[29] = 0x9B5775F9;
		tangleState.H[30] = 0x65F2C8D2;
		tangleState.H[31] = 0x43356233;
		break;

	default:
		return BAD_HASHBITLEN;
	}

	
	return SUCCESS;
}


int 
Tangle::Update(const BitSequence *data, DataLength databitlen)
{
	unsigned long long numblocks;


	if(databitlen==0)
	{
		return SUCCESS;
	}

	tangleState.total += databitlen;

	if(tangleState.rlen + databitlen < 4096) /* not enough data to hash yet */
	{
		memcpy(tangleState.rdata + tangleState.rlen/8,data,(size_t)TANGLE_NumBytes(databitlen));
		tangleState.rlen += (unsigned int)databitlen;
	}
	else	/* we have enough data to hash */
	{
		if(tangleState.rlen)	/* hash the remaining data first */
		{
			memcpy(tangleState.rdata + tangleState.rlen/8,data,(size_t)TANGLE_NumBytes(4096-tangleState.rlen));
			FuncTangle(&tangleState,tangleState.rdata);
			data += (4096-tangleState.rlen)/8;
			databitlen -= 4096-tangleState.rlen;
		}

		numblocks = databitlen / 4096;
		tangleState.rlen = databitlen % 4096;

		while(numblocks>0)	/* hash all remaining blocks */
		{
			FuncTangle(&tangleState,data);
			data += 512;
			numblocks--;
		}
		
		memset(tangleState.rdata,0,512);
		if(tangleState.rlen)		/* save remaining data for next call */
		{
			memcpy(tangleState.rdata,data,(size_t)TANGLE_NumBytes(tangleState.rlen));
		}

	}
		
	return SUCCESS;
}

int 
Tangle::Final(BitSequence *hashval)
{
	unsigned int l,pos;
	unsigned char fin,mask;
	unsigned long long L;
	unsigned int * finalhash;
	int i;

	L = tangleState.total;
	l = L % 4096;
	fin = 128 >> (l % 8);
	mask = ~(255 >> (l % 8));
	pos = l/8;

	tangleState.rdata[pos] &= mask;
	tangleState.rdata[pos] |= fin;

	if(l <= 3967)	/* one block */
	{
		for(i=0;i<8;i++)
		{
			tangleState.rdata[511-i] = (L >> (i*8)) & 255;
		}
		FuncTangle(&tangleState,tangleState.rdata);
	}
	else	/* two blocks */
	{
		for(i=0;i<8;i++)
		{
			tangleState.rdata[1023-i] = (L >> (i*8)) & 255;
		}
		FuncTangle(&tangleState,tangleState.rdata);
		FuncTangle(&tangleState,&(tangleState.rdata[512]));
	}

	finalhash = (unsigned int *) hashval;
	for(i=0;i<(tangleState.hashbitlen/32);i++)
	{
		finalhash[i] = tangleState.H[i];
	}
	
	return SUCCESS;
}

int 
Tangle::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
	int r;
	//hashState state;

	r=Tangle::Init(hashbitlen); 
	if(r!=SUCCESS) return r;
	
	r=Tangle::Update(data,databitlen);
	if(r!=SUCCESS) return r;
	
	r=Tangle::Final(hashval);
	if(r!=SUCCESS) return r;

	return SUCCESS;
}


#define TANGLE_SEED(r) {Xa[r] = TANGLE_FR1(h[r] + h[r+8] + M[r] + M[r+8] + M[r+16] + M[r+24] + M[r+32] + M[r+40] + M[r+48] + M[r+56]) \
	+	TANGLE_FR2(h[r+16] + h[r+24] + M[r+64] + M[r+72] + M[r+80] + M[r+88] + M[r+96] + M[r+104] + M[r+112] + M[r+120]);}



void Tangle::FuncTangle(hashState * state, const BitSequence * message)
{
	unsigned char p[144],q[144],s[144];
	unsigned short r,t;
	unsigned int W[288];
	unsigned int h[32];
	unsigned int Xa[8];
	unsigned int Xb[8];
	unsigned int *M;
	unsigned int B,C;

	M = (unsigned int *)message;


	/* copy hash values */
	for(r=0; r<32; r++) h[r] = state->H[r];
	
	
	/*
	Message Expansion
	*/

	/* seeding */
	TANGLE_SEED(0);
	TANGLE_SEED(1);
	TANGLE_SEED(2);
	TANGLE_SEED(3);
	TANGLE_SEED(4);
	TANGLE_SEED(5);
	TANGLE_SEED(6);
	TANGLE_SEED(7);
		
	/* iteration & extraction */

	t=0;
	while(t<(state->rounds*2))
	{
		/* even */

		/* iterate */
		Xb[0] = Xa[6] ^ Xa[7];
		Xb[1] = Xa[0] ^ Xa[1] ^ Xa[3];
		Xb[2] = Xa[4] ^ Xa[5];

		Xb[3] = Xb[2] ^ Xb[0];
		Xb[4] = Xa[0] ^ Xa[2] ^ Xa[7];
		Xb[5] = Xa[2] ^ Xa[5] ^ Xb[0];
		Xb[6] = Xa[2] ^ Xa[3] ^ Xa[4] ^ Xb[0];
		Xb[7] = Xa[0] ^ Xa[4] ^ Xa[6];
		
		Xb[2] ^= Xb[1];
		Xb[0] ^= Xa[0] ^ Xa[3];
		Xb[1] ^= Xa[6];
		
				
		/* extract */
		W[t]	= TANGLE_F1(Xb[0], Xb[1],TANGLE_K[t	& 255]) + M[t	& 127];
		W[t+1]	= TANGLE_F2(Xb[2], Xb[3],TANGLE_K[t+1 & 255]) + M[t+1 & 127];
		W[t+2]	= TANGLE_F1(Xb[4], Xb[5],TANGLE_K[t+2 & 255]) + M[t+2 & 127];
		W[t+3]	= TANGLE_F2(Xb[6], Xb[7],TANGLE_K[t+3 & 255]) + M[t+3 & 127];

		/* odd */

		/* iterate */
		Xa[0] = Xb[6] ^ Xb[7];
		Xa[1] = Xb[0] ^ Xb[1] ^ Xb[3];
		Xa[2] = Xb[4] ^ Xb[5];

		Xa[3] = Xa[2] ^ Xa[0];
		Xa[4] = Xb[0] ^ Xb[2] ^ Xb[7];
		Xa[5] = Xb[2] ^ Xb[5] ^ Xa[0];
		Xa[6] = Xb[2] ^ Xb[3] ^ Xb[4] ^ Xa[0];
		Xa[7] = Xb[0] ^ Xb[4] ^ Xb[6];
		
		Xa[2] ^= Xa[1];
		Xa[0] ^= Xb[0] ^ Xb[3];
		Xa[1] ^= Xb[6];
		
		/* extract */
		W[t+4]	= TANGLE_F1(Xa[0], Xa[1],TANGLE_K[t+4	& 255]) + M[t+4	& 127];
		W[t+5]	= TANGLE_F2(Xa[2], Xa[3],TANGLE_K[t+5 & 255]) + M[t+5 & 127];
		W[t+6]	= TANGLE_F1(Xa[4], Xa[5],TANGLE_K[t+6 & 255]) + M[t+6 & 127];
		W[t+7]	= TANGLE_F2(Xa[6], Xa[7],TANGLE_K[t+7 & 255]) + M[t+7 & 127];
		
		t+=8;
	}

	
	/*
	Round Function
	*/

	/* pre-compute s, p & q */
	

	C = W[0] + W[1];
	C ^= C>>16;
	C ^= C>>8;
	s[0] = TANGLE_SBOX[(unsigned char)(C & 0xFF)];
	p[0] = s[0] & 15;
	q[0] = s[0] >> 4;

	for(r=1;r<state->rounds;r++) 
	{
		C = W[r*2] + W[r*2+1]; 
		C ^= C>>16; 
		C ^= C>>8;
		s[r] = s[r-1] ^ TANGLE_SBOX[(unsigned char)(C & 0xFF)]; 
		p[r] = s[r] & 15; 
		q[r] = s[r] >> 4;
	}
		

	/* perform round */

	r=0;
	while(r<state->rounds)
	{
		/* even */
		B = TANGLE_F1(h[q[r]],h[(r+8) & 31],TANGLE_FR2(h[p[r]+16])) + W[(r*2)+1];
		h[(r+16) & 31] ^= TANGLE_F1(h[p[r]], h[r & 31], TANGLE_FR1(h[q[r]+16])) + W[r*2] + TANGLE_K[s[r]] + B;
		h[r & 31] += B;
		r++;

		/* odd */
		B = TANGLE_F2(h[q[r]],h[(r+8) & 31],TANGLE_FR2(h[p[r]+16])) + W[(r*2)+1];
		h[(r+16) & 31] ^= TANGLE_F2(h[p[r]], h[r & 31], TANGLE_FR1(h[q[r]+16])) + W[r*2] + TANGLE_K[s[r]] + B;
		h[r & 31] += B;
		r++;
	}

	/* chain hash values */
	for(r=0; r<32; r++) state->H[r] += h[r];
	

}

Tangle::Tangle(const int numRounds) {
	if (numRounds == -1) {
		tangleNumRounds224 = TANGLE_DEFAULT_ROUNDS_224;
		tangleNumRounds256 = TANGLE_DEFAULT_ROUNDS_256;
		tangleNumRounds384 = TANGLE_DEFAULT_ROUNDS_384;
		tangleNumRounds512 = TANGLE_DEFAULT_ROUNDS_512;
		tangleNumRounds768 = TANGLE_DEFAULT_ROUNDS_768;
		tangleNumRounds1024 = TANGLE_DEFAULT_ROUNDS_1024;
	} else {
		tangleNumRounds224 = numRounds;
		tangleNumRounds256 = numRounds;
		tangleNumRounds384 = numRounds;
		tangleNumRounds512 = numRounds;
		tangleNumRounds768 = numRounds;
		tangleNumRounds1024 = numRounds;
	}
}