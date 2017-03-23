# === caesar ===
include(${PROJECT_SOURCE_DIR}/cmake/eacirc_project.cmake)

set(SOURCES
    CaesarProject.cpp
    CaesarInterface.cpp
    CaesarCiphers.cpp
    Encryptor.cpp
    aead/random/RandomCipher.cpp
# === CAESAR algorithms ===
    aead/common/crypto_verify_16.cpp
    aead/common/crypto_verify_32.cpp
    aead/common/crypto_core_aes128encrypt.cpp
    aead/common/crypto_core_aes128decrypt.cpp
    aead/common/crypto_core_aes256encrypt.cpp
    aead/acorn/acorn128/acorn128_encrypt.cpp
    aead/acorn/acorn128/Acorn128.cpp
    aead/aegis/aegis128/aegis128_aes.cpp
    aead/aegis/aegis128/aegis128_encrypt.cpp
    aead/aegis/aegis128/Aegis128.cpp
    aead/aegis/aegis128l/aegis128l_aes.cpp
    aead/aegis/aegis128l/aegis128l_ecrypt.cpp
    aead/aegis/aegis128l/Aegis128l.cpp
    aead/aegis/aegis256/aegis256_aes.cpp
    aead/aegis/aegis256/aegis256_encrypt.cpp
    aead/aegis/aegis256/Aegis256.cpp
# Avalanche: compilaation ok, sigsegv while running (fedora64, both debug/release)
#    aead/avalanche/aes128avalanchev1/aes128avalanchev1_avalanche.cpp
#    aead/avalanche/aes128avalanchev1/aes128avalanchev1_largenumbers.cpp
#    aead/avalanche/aes128avalanchev1/aes128avalanchev1_pcmac.cpp
#    aead/avalanche/aes128avalanchev1/aes128avalanchev1_rmac.cpp
#    aead/avalanche/aes128avalanchev1/Aes128avalanchev1.cpp
#    aead/avalanche/aes192avalanchev1/aes192avalanchev1_avalanche.cpp
#    aead/avalanche/aes192avalanchev1/aes192avalanchev1_largenumbers.cpp
#    aead/avalanche/aes192avalanchev1/aes192avalanchev1_pcmac.cpp
#    aead/avalanche/aes192avalanchev1/aes192avalanchev1_rmac.cpp
#    aead/avalanche/aes192avalanchev1/Aes192avalanchev1.cpp
#    aead/avalanche/aes256avalanchev1/aes256avalanchev1_avalanche.cpp
#    aead/avalanche/aes256avalanchev1/aes256avalanchev1_largenumbers.cpp
#    aead/avalanche/aes256avalanchev1/aes256avalanchev1_pcmac.cpp
#    aead/avalanche/aes256avalanchev1/aes256avalanchev1_rmac.cpp
#    aead/avalanche/aes256avalanchev1/Aes256avalanchev1.cpp
    aead/aes-gcm/aes128gcmv1/aes128gcmv1_encrypt.cpp
    aead/aes-gcm/aes128gcmv1/Aes128gcmv1.cpp
    aead/aes-gcm/aes256gcmv1/aes256gcmv1_encrypt.cpp
    aead/aes-gcm/aes256gcmv1/Aes256gcmv1.cpp
    aead/marble/aes128marble4rv1/aes128marble4rv1_aes_core.cpp
    aead/marble/aes128marble4rv1/aes128marble4rv1_encrypt.cpp
    aead/marble/aes128marble4rv1/aes128marble4rv1_marble.cpp
    aead/marble/aes128marble4rv1/aes128marble4rv1_utils.cpp
    aead/marble/aes128marble4rv1/Aes128marble4rv1.cpp
    aead/cloc/twine80n6clocv1/twine80n6clocv1_cloc.cpp
    aead/cloc/twine80n6clocv1/twine80n6clocv1_encrypt.cpp
    aead/cloc/twine80n6clocv1/twine80n6clocv1_twine.cpp
    aead/cloc/twine80n6clocv1/twine80n6clocv1_utils.cpp
    aead/cloc/twine80n6clocv1/Twine80n6clocv1.cpp
    aead/cloc/aes128n12clocv1/aes128n12clocv1_aes_core.cpp
    aead/cloc/aes128n12clocv1/aes128n12clocv1_cloc.cpp
    aead/cloc/aes128n12clocv1/aes128n12clocv1_encrypt.cpp
    aead/cloc/aes128n12clocv1/aes128n12clocv1_utils.cpp
    aead/cloc/aes128n12clocv1/Aes128n12clocv1.cpp
    aead/cloc/aes128n8clocv1/aes128n8clocv1_aes_core.cpp
    aead/cloc/aes128n8clocv1/aes128n8clocv1_cloc.cpp
    aead/cloc/aes128n8clocv1/aes128n8clocv1_encrypt.cpp
    aead/cloc/aes128n8clocv1/aes128n8clocv1_utils.cpp
    aead/cloc/aes128n8clocv1/Aes128n8clocv1.cpp
    aead/silc/aes128n12silcv1/aes128n12silcv1_aes_core.cpp
    aead/silc/aes128n12silcv1/aes128n12silcv1_encrypt.cpp
    aead/silc/aes128n12silcv1/aes128n12silcv1_silc.cpp
    aead/silc/aes128n12silcv1/aes128n12silcv1_utils.cpp
    aead/silc/aes128n12silcv1/Aes128n12silcv1.cpp
    aead/silc/aes128n8silcv1/aes128n8silcv1_aes_core.cpp
    aead/silc/aes128n8silcv1/aes128n8silcv1_encrypt.cpp
    aead/silc/aes128n8silcv1/aes128n8silcv1_silc.cpp
    aead/silc/aes128n8silcv1/aes128n8silcv1_utils.cpp
    aead/silc/aes128n8silcv1/Aes128n8silcv1.cpp
    aead/silc/led80n6silcv1/led80n6silcv1_encrypt.cpp
    aead/silc/led80n6silcv1/led80n6silcv1_led.cpp
    aead/silc/led80n6silcv1/led80n6silcv1_silc.cpp
    aead/silc/led80n6silcv1/led80n6silcv1_utils.cpp
    aead/silc/led80n6silcv1/Led80n6silcv1.cpp
    aead/silc/present80n6silcv1/present80n6silcv1_encrypt.cpp
    aead/silc/present80n6silcv1/present80n6silcv1_present.cpp
    aead/silc/present80n6silcv1/present80n6silcv1_silc.cpp
    aead/silc/present80n6silcv1/present80n6silcv1_utils.cpp
    aead/silc/present80n6silcv1/Present80n6silcv1.cpp
    aead/aes-otr/aes128otrpv1/aes128otrpv1_encrypt.cpp
    aead/aes-otr/aes128otrpv1/aes128otrpv1_OTR.cpp
    aead/aes-otr/aes128otrpv1/aes128otrpv1_t-aes_enc_only.cpp
    aead/aes-otr/aes128otrpv1/Aes128otrpv1.cpp
    aead/aes-otr/aes128otrsv1/aes128otrsv1_encrypt.cpp
    aead/aes-otr/aes128otrsv1/aes128otrsv1_OTR.cpp
    aead/aes-otr/aes128otrsv1/aes128otrsv1_t-aes_enc_only.cpp
    aead/aes-otr/aes128otrsv1/Aes128otrsv1.cpp
    aead/aes-otr/aes256otrpv1/aes256otrpv1_encrypt.cpp
    aead/aes-otr/aes256otrpv1/aes256otrpv1_OTR.cpp
    aead/aes-otr/aes256otrpv1/aes256otrpv1_t-aes_enc_only.cpp
    aead/aes-otr/aes256otrpv1/Aes256otrpv1.cpp
    aead/aes-otr/aes256otrsv1/aes256otrsv1_encrypt.cpp
    aead/aes-otr/aes256otrsv1/aes256otrsv1_OTR.cpp
    aead/aes-otr/aes256otrsv1/aes256otrsv1_t-aes_enc_only.cpp
    aead/aes-otr/aes256otrsv1/Aes256otrsv1.cpp
    aead/poet/aes128poetv1aes128/aes128poetv1aes128_aes.cpp
    aead/poet/aes128poetv1aes128/aes128poetv1aes128_encrypt.cpp
    aead/poet/aes128poetv1aes128/aes128poetv1aes128_poet.cpp
    aead/poet/aes128poetv1aes128/Aes128poetv1aes128.cpp
    aead/poet/aes128poetv1aes4/aes128poetv1aes4_aes.cpp
    aead/poet/aes128poetv1aes4/aes128poetv1aes4_encrypt.cpp
    aead/poet/aes128poetv1aes4/aes128poetv1aes4_poet.cpp
    aead/poet/aes128poetv1aes4/Aes128poetv1aes4.cpp
    aead/aes-cpfb/aes256cpfbv1/aes256cpfbv1_encrypt.cpp
    aead/aes-cpfb/aes256cpfbv1/Aes256cpfbv1.cpp
    aead/aes-cpfb/aes128cpfbv1/aes128cpfbv1_encrypt.cpp
    aead/aes-cpfb/aes128cpfbv1/Aes128cpfbv1.cpp
    aead/aes-copa/aescopav1/aescopav1_aes-core.cpp
    aead/aes-copa/aescopav1/aescopav1_encrypt.cpp
    aead/aes-copa/aescopav1/Aescopav1.cpp
    aead/aes-jambu/aesjambuv1/aesjambuv1_aes.cpp
    aead/aes-jambu/aesjambuv1/aesjambuv1_encrypt.cpp
    aead/aes-jambu/aesjambuv1/Aesjambuv1.cpp
    aead/aez/aezv1/aezv1_aez_ref.cpp
    aead/aez/aezv1/aezv1_encrypt.cpp
    aead/aez/aezv1/aezv1_rijndael-alg-fst.cpp
    aead/aez/aezv1/Aezv1.cpp
    aead/aez/aezv3/aezv3_aez_ref.cpp
    aead/aez/aezv3/aezv3_rijndael-alg-fst.cpp
    aead/aez/aezv3/Aezv3.cpp
    aead/artemia/artemia128v1/artemia128v1_encrypt.cpp
    aead/artemia/artemia128v1/artemia128v1_jhae_decryption.cpp
    aead/artemia/artemia128v1/artemia128v1_jhae_encryption.cpp
    aead/artemia/artemia128v1/artemia128v1_jhae_padding.cpp
    aead/artemia/artemia128v1/artemia128v1_jhae_permutation.cpp
    aead/artemia/artemia128v1/Artemia128v1.cpp
    aead/artemia/artemia256v1/artemia256v1_encrypt.cpp
    aead/artemia/artemia256v1/artemia256v1_jhae_decryption.cpp
    aead/artemia/artemia256v1/artemia256v1_jhae_encryption.cpp
    aead/artemia/artemia256v1/artemia256v1_jhae_padding.cpp
    aead/artemia/artemia256v1/artemia256v1_jhae_permutation.cpp
    aead/artemia/artemia256v1/Artemia256v1.cpp
    aead/ascon/ascon128v1/ascon128v1_ascon.cpp
    aead/ascon/ascon128v1/Ascon128v1.cpp
    aead/ascon/ascon96v1/ascon96v1_ascon.cpp
    aead/ascon/ascon96v1/Ascon96v1.cpp
    aead/calico/calicov8/calicov8_calico.cpp
    aead/calico/calicov8/calicov8_encrypt.cpp
    aead/calico/calicov8/Calicov8.cpp
    aead/aes-cmcc/cmcc22v1/cmcc22v1_encrypt.cpp
    aead/aes-cmcc/cmcc22v1/Cmcc22v1.cpp
    aead/aes-cmcc/cmcc24v1/cmcc24v1_encrypt.cpp
    aead/aes-cmcc/cmcc24v1/Cmcc24v1.cpp
    aead/aes-cmcc/cmcc42v1/cmcc42v1_encrypt.cpp
    aead/aes-cmcc/cmcc42v1/Cmcc42v1.cpp
    aead/aes-cmcc/cmcc44v1/cmcc44v1_encrypt.cpp
    aead/aes-cmcc/cmcc44v1/Cmcc44v1.cpp
    aead/aes-cmcc/cmcc84v1/cmcc84v1_encrypt.cpp
    aead/aes-cmcc/cmcc84v1/Cmcc84v1.cpp
    aead/deoxys/deoxyseq128128v1/deoxyseq128128v1_deoxys.cpp
    aead/deoxys/deoxyseq128128v1/deoxyseq128128v1_encrypt.cpp
    aead/deoxys/deoxyseq128128v1/deoxyseq128128v1_tweakableBC.cpp
    aead/deoxys/deoxyseq128128v1/Deoxyseq128128v1.cpp
    aead/deoxys/deoxyseq256128v1/deoxyseq256128v1_deoxys.cpp
    aead/deoxys/deoxyseq256128v1/deoxyseq256128v1_encrypt.cpp
    aead/deoxys/deoxyseq256128v1/deoxyseq256128v1_tweakableBC.cpp
    aead/deoxys/deoxyseq256128v1/Deoxyseq256128v1.cpp
    aead/deoxys/deoxysneq128128v1/deoxysneq128128v1_deoxys.cpp
    aead/deoxys/deoxysneq128128v1/deoxysneq128128v1_encrypt.cpp
    aead/deoxys/deoxysneq128128v1/deoxysneq128128v1_tweakableBC.cpp
    aead/deoxys/deoxysneq128128v1/Deoxysneq128128v1.cpp
    aead/deoxys/deoxysneq256128v1/deoxysneq256128v1_deoxys.cpp
    aead/deoxys/deoxysneq256128v1/deoxysneq256128v1_encrypt.cpp
    aead/deoxys/deoxysneq256128v1/deoxysneq256128v1_tweakableBC.cpp
    aead/deoxys/deoxysneq256128v1/Deoxysneq256128v1.cpp
    aead/enchilada/enchilada128v1/enchilada128v1_aes.cpp
    aead/enchilada/enchilada128v1/enchilada128v1_auth.cpp
    aead/enchilada/enchilada128v1/enchilada128v1_chacha.cpp
    aead/enchilada/enchilada128v1/enchilada128v1_enchilada.cpp
    aead/enchilada/enchilada128v1/Enchilada128v1.cpp
    aead/enchilada/enchilada256v1/enchilada256v1_aescrypt.cpp
    aead/enchilada/enchilada256v1/enchilada256v1_aestab.cpp
    aead/enchilada/enchilada256v1/enchilada256v1_auth.cpp
    aead/enchilada/enchilada256v1/enchilada256v1_chacha.cpp
    aead/enchilada/enchilada256v1/enchilada256v1_enchilada256.cpp
    aead/enchilada/enchilada256v1/Enchilada256v1.cpp
    aead/hs1-siv/hs1sivhiv1/hs1sivhiv1_encrypt.cpp
    aead/hs1-siv/hs1sivhiv1/Hs1sivhiv1.cpp
    aead/hs1-siv/hs1sivlov1/hs1sivlov1_encrypt.cpp
    aead/hs1-siv/hs1sivlov1/Hs1sivlov1.cpp
    aead/hs1-siv/hs1sivv1/hs1sivv1_encrypt.cpp
    aead/hs1-siv/hs1sivv1/Hs1sivv1.cpp
    aead/icepole/icepole128av1/icepole128av1_encrypt.cpp
    aead/icepole/icepole128av1/icepole128av1_icepole.cpp
    aead/icepole/icepole128av1/Icepole128av1.cpp
    aead/icepole/icepole128v1/icepole128v1_encrypt.cpp
    aead/icepole/icepole128v1/icepole128v1_icepole.cpp
    aead/icepole/icepole128v1/Icepole128v1.cpp
    aead/icepole/icepole256av1/icepole256av1_encrypt.cpp
    aead/icepole/icepole256av1/icepole256av1_icepole.cpp
    aead/icepole/icepole256av1/Icepole256av1.cpp
    aead/ifeed-aes/ifeedaes128n104v1/ifeedaes128n104v1_encrypt.cpp
    aead/ifeed-aes/ifeedaes128n104v1/Ifeedaes128n104v1.cpp
    aead/ifeed-aes/ifeedaes128n96v1/ifeedaes128n96v1_encrypt.cpp
    aead/ifeed-aes/ifeedaes128n96v1/Ifeedaes128n96v1.cpp
    aead/scream/iscream12v1/iscream12v1_iscream_cipher.cpp
    aead/scream/iscream12v1/iscream12v1_tae.cpp
    aead/scream/iscream12v1/Iscream12v1.cpp
    aead/scream/iscream12v2/iscream12v2_iscream_cipher.cpp
    aead/scream/iscream12v2/iscream12v2_tae.cpp
    aead/scream/iscream12v2/Iscream12v2.cpp
    aead/scream/iscream14v1/iscream14v1_iscream_cipher.cpp
    aead/scream/iscream14v1/iscream14v1_tae.cpp
    aead/scream/iscream14v1/Iscream14v1.cpp
    aead/scream/iscream14v2/iscream14v2_iscream_cipher.cpp
    aead/scream/iscream14v2/iscream14v2_tae.cpp
    aead/scream/iscream14v2/Iscream14v2.cpp
    aead/scream/scream10v1/scream10v1_scream_cipher.cpp
    aead/scream/scream10v1/scream10v1_tae.cpp
    aead/scream/scream10v1/Scream10v1.cpp
    aead/scream/scream10v2/scream10v2_scream_cipher.cpp
    aead/scream/scream10v2/scream10v2_tae.cpp
    aead/scream/scream10v2/Scream10v2.cpp
    aead/scream/scream12v1/scream12v1_scream_cipher.cpp
    aead/scream/scream12v1/scream12v1_tae.cpp
    aead/scream/scream12v1/Scream12v1.cpp
    aead/scream/scream12v2/scream12v2_scream_cipher.cpp
    aead/scream/scream12v2/scream12v2_tae.cpp
    aead/scream/scream12v2/Scream12v2.cpp
    aead/joltik/joltikeq12864v1/joltikeq12864v1_encrypt.cpp
    aead/joltik/joltikeq12864v1/joltikeq12864v1_joltik.cpp
    aead/joltik/joltikeq12864v1/joltikeq12864v1_tweakableBC.cpp
    aead/joltik/joltikeq12864v1/Joltikeq12864v1.cpp
    aead/joltik/joltikeq6464v1/joltikeq6464v1_encrypt.cpp
    aead/joltik/joltikeq6464v1/joltikeq6464v1_joltik.cpp
    aead/joltik/joltikeq6464v1/joltikeq6464v1_tweakableBC.cpp
    aead/joltik/joltikeq6464v1/Joltikeq6464v1.cpp
    aead/joltik/joltikeq8048v1/joltikeq8048v1_encrypt.cpp
    aead/joltik/joltikeq8048v1/joltikeq8048v1_joltik.cpp
    aead/joltik/joltikeq8048v1/joltikeq8048v1_tweakableBC.cpp
    aead/joltik/joltikeq8048v1/Joltikeq8048v1.cpp
    aead/joltik/joltikeq9696v1/joltikeq9696v1_encrypt.cpp
    aead/joltik/joltikeq9696v1/joltikeq9696v1_joltik.cpp
    aead/joltik/joltikeq9696v1/joltikeq9696v1_tweakableBC.cpp
    aead/joltik/joltikeq9696v1/Joltikeq9696v1.cpp
    aead/joltik/joltikneq12864v1/joltikneq12864v1_encrypt.cpp
    aead/joltik/joltikneq12864v1/joltikneq12864v1_joltik.cpp
    aead/joltik/joltikneq12864v1/joltikneq12864v1_tweakableBC.cpp
    aead/joltik/joltikneq12864v1/Joltikneq12864v1.cpp
    aead/joltik/joltikneq6464v1/joltikneq6464v1_encrypt.cpp
    aead/joltik/joltikneq6464v1/joltikneq6464v1_joltik.cpp
    aead/joltik/joltikneq6464v1/joltikneq6464v1_tweakableBC.cpp
    aead/joltik/joltikneq6464v1/Joltikneq6464v1.cpp
    aead/joltik/joltikneq8048v1/joltikneq8048v1_encrypt.cpp
    aead/joltik/joltikneq8048v1/joltikneq8048v1_joltik.cpp
    aead/joltik/joltikneq8048v1/joltikneq8048v1_tweakableBC.cpp
    aead/joltik/joltikneq8048v1/Joltikneq8048v1.cpp
    aead/joltik/joltikneq9696v1/joltikneq9696v1_encrypt.cpp
    aead/joltik/joltikneq9696v1/joltikneq9696v1_joltik.cpp
    aead/joltik/joltikneq9696v1/joltikneq9696v1_tweakableBC.cpp
    aead/joltik/joltikneq9696v1/Joltikneq9696v1.cpp
#    aead/julius/juliusv1draft/juliusv1draft_aes.cpp
#    aead/julius/juliusv1draft/juliusv1draft_encrypt.cpp
#    aead/julius/juliusv1draft/juliusv1draft_functions.cpp
#    aead/julius/juliusv1draft/Juliusv1draft.cpp
    aead/ketje/ketjejrv1/ketjejrv1_displayIntermediateValues.cpp
    aead/ketje/ketjejrv1/ketjejrv1_encrypt.cpp
    aead/ketje/ketjejrv1/ketjejrv1_KeccakF-200-reference.cpp
    aead/ketje/ketjejrv1/ketjejrv1_KeccakP-200-reference.cpp
    aead/ketje/ketjejrv1/ketjejrv1_Ket.cpp
    aead/ketje/ketjejrv1/ketjejrv1_Ketje.cpp
    aead/ketje/ketjejrv1/Ketjejrv1.cpp
    aead/ketje/ketjesrv1/ketjesrv1_displayIntermediateValues.cpp
    aead/ketje/ketjesrv1/ketjesrv1_encrypt.cpp
    aead/ketje/ketjesrv1/ketjesrv1_KeccakF-400-reference.cpp
    aead/ketje/ketjesrv1/ketjesrv1_KeccakP-400-reference.cpp
    aead/ketje/ketjesrv1/ketjesrv1_Ket.cpp
    aead/ketje/ketjesrv1/ketjesrv1_Ketje.cpp
    aead/ketje/ketjesrv1/Ketjesrv1.cpp
    aead/kiasu/kiasueq128v1/kiasueq128v1_encrypt.cpp
    aead/kiasu/kiasueq128v1/kiasueq128v1_kiasu.cpp
    aead/kiasu/kiasueq128v1/kiasueq128v1_tweakable_aes.cpp
    aead/kiasu/kiasueq128v1/Kiasueq128v1.cpp
    aead/kiasu/kiasuneq128v1/kiasuneq128v1_encrypt.cpp
    aead/kiasu/kiasuneq128v1/kiasuneq128v1_kiasu.cpp
    aead/kiasu/kiasuneq128v1/kiasuneq128v1_tweakable_aes.cpp
    aead/kiasu/kiasuneq128v1/Kiasuneq128v1.cpp
    aead/lac/lacv1/lacv1_encrypt.cpp
    aead/lac/lacv1/Lacv1.cpp
    aead/keyak/lakekeyakv1/lakekeyakv1_displayIntermediateValues.cpp
    aead/keyak/lakekeyakv1/lakekeyakv1_encrypt.cpp
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakDuplex.cpp
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakF-1600-reference.cpp
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakP-1600-12-reference.cpp
    aead/keyak/lakekeyakv1/lakekeyakv1_Keyak.cpp
    aead/keyak/lakekeyakv1/Lakekeyakv1.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_displayIntermediateValues.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_encrypt.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakDuplex.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakF-1600-reference.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakP-1600-12-reference.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakParallelDuplex.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_Keyak.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_SerialFallback.cpp
    aead/keyak/oceankeyakv1/oceankeyakv1_testParallelPaSM.cpp
    aead/keyak/oceankeyakv1/Oceankeyakv1.cpp
    aead/keyak/riverkeyakv1/riverkeyakv1_displayIntermediateValues.cpp
    aead/keyak/riverkeyakv1/riverkeyakv1_encrypt.cpp
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakDuplex.cpp
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakF-800-reference.cpp
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakP-800-12-reference.cpp
    aead/keyak/riverkeyakv1/riverkeyakv1_Keyak.cpp
    aead/keyak/riverkeyakv1/Riverkeyakv1.cpp
    aead/keyak/seakeyakv1/seakeyakv1_displayIntermediateValues.cpp
    aead/keyak/seakeyakv1/seakeyakv1_encrypt.cpp
    aead/keyak/seakeyakv1/seakeyakv1_KeccakDuplex.cpp
    aead/keyak/seakeyakv1/seakeyakv1_KeccakF-1600-reference.cpp
    aead/keyak/seakeyakv1/seakeyakv1_KeccakP-1600-12-reference.cpp
    aead/keyak/seakeyakv1/seakeyakv1_KeccakParallelDuplex.cpp
    aead/keyak/seakeyakv1/seakeyakv1_Keyak.cpp
    aead/keyak/seakeyakv1/seakeyakv1_SerialFallback.cpp
    aead/keyak/seakeyakv1/seakeyakv1_testParallelPaSM.cpp
    aead/keyak/seakeyakv1/Seakeyakv1.cpp
    aead/minalpher/minalpherv1/minalpherv1_encrypt.cpp
    aead/minalpher/minalpherv1/Minalpherv1.cpp
    aead/morus/morus1280128v1/morus1280128v1_encrypt.cpp
    aead/morus/morus1280128v1/Morus1280128v1.cpp
    aead/morus/morus1280256v1/morus1280256v1_encrypt.cpp
    aead/morus/morus1280256v1/Morus1280256v1.cpp
    aead/morus/morus640128v1/morus640128v1_encrypt.cpp
    aead/morus/morus640128v1/Morus640128v1.cpp
    aead/norx/norx3241v1/norx3241v1_caesar.cpp
    aead/norx/norx3241v1/norx3241v1_norx.cpp
    aead/norx/norx3241v1/Norx3241v1.cpp
    aead/norx/norx3261v1/norx3261v1_caesar.cpp
    aead/norx/norx3261v1/norx3261v1_norx.cpp
    aead/norx/norx3261v1/Norx3261v1.cpp
    aead/norx/norx6441v1/norx6441v1_caesar.cpp
    aead/norx/norx6441v1/norx6441v1_norx.cpp
    aead/norx/norx6441v1/Norx6441v1.cpp
    aead/norx/norx6444v1/norx6444v1_caesar.cpp
    aead/norx/norx6444v1/norx6444v1_norx.cpp
    aead/norx/norx6444v1/Norx6444v1.cpp
    aead/norx/norx6461v1/norx6461v1_caesar.cpp
    aead/norx/norx6461v1/norx6461v1_norx.cpp
    aead/norx/norx6461v1/Norx6461v1.cpp
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_encrypt.cpp
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_omdsha256.cpp
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_sha256.cpp
    aead/omd/omdsha256k128n96tau128v1/Omdsha256k128n96tau128v1.cpp
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_encrypt.cpp
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_omdsha256.cpp
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_sha256.cpp
    aead/omd/omdsha256k128n96tau64v1/Omdsha256k128n96tau64v1.cpp
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_encrypt.cpp
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_omdsha256.cpp
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_sha256.cpp
    aead/omd/omdsha256k128n96tau96v1/Omdsha256k128n96tau96v1.cpp
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_encrypt.cpp
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_omdsha256.cpp
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_sha256.cpp
    aead/omd/omdsha256k192n104tau128v1/Omdsha256k192n104tau128v1.cpp
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_encrypt.cpp
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_omdsha256.cpp
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_sha256.cpp
    aead/omd/omdsha256k256n104tau160v1/Omdsha256k256n104tau160v1.cpp
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_encrypt.cpp
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_omdsha256.cpp
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_sha256.cpp
    aead/omd/omdsha256k256n248tau256v1/Omdsha256k256n248tau256v1.cpp
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_encrypt.cpp
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_omdsha512.cpp
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_sha512.cpp
    aead/omd/omdsha512k128n128tau128v1/Omdsha512k128n128tau128v1.cpp
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_encrypt.cpp
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_omdsha512.cpp
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_sha512.cpp
    aead/omd/omdsha512k256n256tau256v1/Omdsha512k256n256tau256v1.cpp
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_encrypt.cpp
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_omdsha512.cpp
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_sha512.cpp
    aead/omd/omdsha512k512n256tau256v1/Omdsha512k512n256tau256v1.cpp
    aead/paeq/paeq128/paeq128_encrypt.cpp
    aead/paeq/paeq128/Paeq128.cpp
    aead/paeq/paeq128t/paeq128t_encrypt.cpp
    aead/paeq/paeq128t/Paeq128t.cpp
    aead/paeq/paeq128tnm/paeq128tnm_encrypt.cpp
    aead/paeq/paeq128tnm/Paeq128tnm.cpp
    aead/paeq/paeq160/paeq160_encrypt.cpp
    aead/paeq/paeq160/Paeq160.cpp
    aead/paeq/paeq64/paeq64_encrypt.cpp
    aead/paeq/paeq64/Paeq64.cpp
    aead/paeq/paeq80/paeq80_encrypt.cpp
    aead/paeq/paeq80/Paeq80.cpp
    aead/pi-cipher/pi16cipher096v1/pi16cipher096v1_encrypt.cpp
    aead/pi-cipher/pi16cipher096v1/Pi16cipher096v1.cpp
    aead/pi-cipher/pi16cipher128v1/pi16cipher128v1_encrypt.cpp
    aead/pi-cipher/pi16cipher128v1/Pi16cipher128v1.cpp
    aead/pi-cipher/pi32cipher128v1/pi32cipher128v1_encrypt.cpp
    aead/pi-cipher/pi32cipher128v1/Pi32cipher128v1.cpp
    aead/pi-cipher/pi32cipher256v1/pi32cipher256v1_encrypt.cpp
    aead/pi-cipher/pi32cipher256v1/Pi32cipher256v1.cpp
    aead/pi-cipher/pi64cipher128v1/pi64cipher128v1_encrypt.cpp
    aead/pi-cipher/pi64cipher128v1/Pi64cipher128v1.cpp
    aead/pi-cipher/pi64cipher256v1/pi64cipher256v1_encrypt.cpp
    aead/pi-cipher/pi64cipher256v1/Pi64cipher256v1.cpp
    aead/pi-cipher/pi64cipher256v1oneround/pi64cipher256v1oneround_encrypt.cpp
    aead/pi-cipher/pi64cipher256v1oneround/Pi64cipher256v1oneround.cpp
    aead/pi-cipher/pi64cipher256v1tworounds/pi64cipher256v1tworounds_encrypt.cpp
    aead/pi-cipher/pi64cipher256v1tworounds/Pi64cipher256v1tworounds.cpp
#    aead/polawis/polawisv1/polawisv1_decode_POLAWIS.cpp
#    aead/polawis/polawisv1/polawisv1_encode_POLAWIS.cpp
#    aead/polawis/polawisv1/polawisv1_key_gen_POLAWIS.cpp
#    aead/polawis/polawisv1/Polawisv1.cpp
    aead/paes/ppaev11/ppaev11_encrypt.cpp
    aead/paes/ppaev11/Ppaev11.cpp
    aead/primates/primatesv1ape120/primatesv1ape120_encrypt.cpp
    aead/primates/primatesv1ape120/primatesv1ape120_primate.cpp
    aead/primates/primatesv1ape120/Primatesv1ape120.cpp
    aead/primates/primatesv1ape80/primatesv1ape80_encrypt.cpp
    aead/primates/primatesv1ape80/primatesv1ape80_primate.cpp
    aead/primates/primatesv1ape80/Primatesv1ape80.cpp
    aead/primates/primatesv1gibbon120/primatesv1gibbon120_encrypt.cpp
    aead/primates/primatesv1gibbon120/primatesv1gibbon120_primate.cpp
    aead/primates/primatesv1gibbon120/Primatesv1gibbon120.cpp
    aead/primates/primatesv1gibbon80/primatesv1gibbon80_encrypt.cpp
    aead/primates/primatesv1gibbon80/primatesv1gibbon80_primate.cpp
    aead/primates/primatesv1gibbon80/Primatesv1gibbon80.cpp
    aead/primates/primatesv1hanuman120/primatesv1hanuman120_encrypt.cpp
    aead/primates/primatesv1hanuman120/primatesv1hanuman120_primate.cpp
    aead/primates/primatesv1hanuman120/Primatesv1hanuman120.cpp
    aead/primates/primatesv1hanuman80/primatesv1hanuman80_encrypt.cpp
    aead/primates/primatesv1hanuman80/primatesv1hanuman80_primate.cpp
    aead/primates/primatesv1hanuman80/Primatesv1hanuman80.cpp
    aead/prost/proest128apev1/proest128apev1_encrypt.cpp
    aead/prost/proest128apev1/proest128apev1_proest128.cpp
    aead/prost/proest128apev1/proest128apev1_zerobytes.cpp
    aead/prost/proest128apev1/Proest128apev1.cpp
    aead/prost/proest128copav1/proest128copav1_encrypt.cpp
    aead/prost/proest128copav1/proest128copav1_proest128.cpp
    aead/prost/proest128copav1/proest128copav1_zerobytes.cpp
    aead/prost/proest128copav1/Proest128copav1.cpp
    aead/prost/proest128otrv1/proest128otrv1_encrypt.cpp
    aead/prost/proest128otrv1/proest128otrv1_proest128.cpp
    aead/prost/proest128otrv1/proest128otrv1_zerobytes.cpp
    aead/prost/proest128otrv1/Proest128otrv1.cpp
    aead/prost/proest256apev1/proest256apev1_encrypt.cpp
    aead/prost/proest256apev1/proest256apev1_proest256.cpp
    aead/prost/proest256apev1/proest256apev1_zerobytes.cpp
    aead/prost/proest256apev1/Proest256apev1.cpp
    aead/prost/proest256copav1/proest256copav1_encrypt.cpp
    aead/prost/proest256copav1/proest256copav1_proest256.cpp
    aead/prost/proest256copav1/proest256copav1_zerobytes.cpp
    aead/prost/proest256copav1/Proest256copav1.cpp
    aead/prost/proest256otrv1/proest256otrv1_encrypt.cpp
    aead/prost/proest256otrv1/proest256otrv1_proest256.cpp
    aead/prost/proest256otrv1/proest256otrv1_zerobytes.cpp
    aead/prost/proest256otrv1/Proest256otrv1.cpp
    aead/raviyoyla/raviyoylav1/raviyoylav1_encrypt.cpp
    aead/raviyoyla/raviyoylav1/Raviyoylav1.cpp
    aead/sablier/sablierv1/sablierv1_encrypt.cpp
    aead/sablier/sablierv1/Sablierv1.cpp
    aead/shell/shellaes128v1d4n64/shellaes128v1d4n64_aes.cpp
    aead/shell/shellaes128v1d4n64/shellaes128v1d4n64_aesReduced.cpp
    aead/shell/shellaes128v1d4n64/shellaes128v1d4n64_encrypt.cpp
    aead/shell/shellaes128v1d4n64/shellaes128v1d4n64_shellaes.cpp
    aead/shell/shellaes128v1d4n64/Shellaes128v1d4n64.cpp
    aead/shell/shellaes128v1d4n80/shellaes128v1d4n80_aes.cpp
    aead/shell/shellaes128v1d4n80/shellaes128v1d4n80_aesReduced.cpp
    aead/shell/shellaes128v1d4n80/shellaes128v1d4n80_encrypt.cpp
    aead/shell/shellaes128v1d4n80/shellaes128v1d4n80_shellaes.cpp
    aead/shell/shellaes128v1d4n80/Shellaes128v1d4n80.cpp
    aead/shell/shellaes128v1d5n64/shellaes128v1d5n64_aes.cpp
    aead/shell/shellaes128v1d5n64/shellaes128v1d5n64_aesReduced.cpp
    aead/shell/shellaes128v1d5n64/shellaes128v1d5n64_encrypt.cpp
    aead/shell/shellaes128v1d5n64/shellaes128v1d5n64_shellaes.cpp
    aead/shell/shellaes128v1d5n64/Shellaes128v1d5n64.cpp
    aead/shell/shellaes128v1d5n80/shellaes128v1d5n80_aes.cpp
    aead/shell/shellaes128v1d5n80/shellaes128v1d5n80_aesReduced.cpp
    aead/shell/shellaes128v1d5n80/shellaes128v1d5n80_encrypt.cpp
    aead/shell/shellaes128v1d5n80/shellaes128v1d5n80_shellaes.cpp
    aead/shell/shellaes128v1d5n80/Shellaes128v1d5n80.cpp
    aead/shell/shellaes128v1d6n64/shellaes128v1d6n64_aes.cpp
    aead/shell/shellaes128v1d6n64/shellaes128v1d6n64_aesReduced.cpp
    aead/shell/shellaes128v1d6n64/shellaes128v1d6n64_encrypt.cpp
    aead/shell/shellaes128v1d6n64/shellaes128v1d6n64_shellaes.cpp
    aead/shell/shellaes128v1d6n64/Shellaes128v1d6n64.cpp
    aead/shell/shellaes128v1d6n80/shellaes128v1d6n80_aes.cpp
    aead/shell/shellaes128v1d6n80/shellaes128v1d6n80_aesReduced.cpp
    aead/shell/shellaes128v1d6n80/shellaes128v1d6n80_encrypt.cpp
    aead/shell/shellaes128v1d6n80/shellaes128v1d6n80_shellaes.cpp
    aead/shell/shellaes128v1d6n80/Shellaes128v1d6n80.cpp
    aead/shell/shellaes128v1d7n64/shellaes128v1d7n64_aes.cpp
    aead/shell/shellaes128v1d7n64/shellaes128v1d7n64_aesReduced.cpp
    aead/shell/shellaes128v1d7n64/shellaes128v1d7n64_encrypt.cpp
    aead/shell/shellaes128v1d7n64/shellaes128v1d7n64_shellaes.cpp
    aead/shell/shellaes128v1d7n64/Shellaes128v1d7n64.cpp
    aead/shell/shellaes128v1d7n80/shellaes128v1d7n80_aes.cpp
    aead/shell/shellaes128v1d7n80/shellaes128v1d7n80_aesReduced.cpp
    aead/shell/shellaes128v1d7n80/shellaes128v1d7n80_encrypt.cpp
    aead/shell/shellaes128v1d7n80/shellaes128v1d7n80_shellaes.cpp
    aead/shell/shellaes128v1d7n80/Shellaes128v1d7n80.cpp
    aead/shell/shellaes128v1d8n64/shellaes128v1d8n64_aes.cpp
    aead/shell/shellaes128v1d8n64/shellaes128v1d8n64_aesReduced.cpp
    aead/shell/shellaes128v1d8n64/shellaes128v1d8n64_encrypt.cpp
    aead/shell/shellaes128v1d8n64/shellaes128v1d8n64_shellaes.cpp
    aead/shell/shellaes128v1d8n64/Shellaes128v1d8n64.cpp
    aead/shell/shellaes128v1d8n80/shellaes128v1d8n80_aes.cpp
    aead/shell/shellaes128v1d8n80/shellaes128v1d8n80_aesReduced.cpp
    aead/shell/shellaes128v1d8n80/shellaes128v1d8n80_encrypt.cpp
    aead/shell/shellaes128v1d8n80/shellaes128v1d8n80_shellaes.cpp
    aead/shell/shellaes128v1d8n80/Shellaes128v1d8n80.cpp
    aead/silver/silverv1/silverv1_encrypt.cpp
    aead/silver/silverv1/silverv1_rijndaelEndianNeutral.cpp
    aead/silver/silverv1/Silverv1.cpp
    aead/stribob/stribob192r1/stribob192r1_sbob_tab64.cpp
    aead/stribob/stribob192r1/stribob192r1_encrypt.cpp
    aead/stribob/stribob192r1/stribob192r1_sbob_pi64.cpp
    aead/stribob/stribob192r1/stribob192r1_stribob.cpp
    aead/stribob/stribob192r1/Stribob192r1.cpp
    aead/tiaoxin/tiaoxinv1/tiaoxinv1_aes_round.cpp
    aead/tiaoxin/tiaoxinv1/tiaoxinv1_encrypt.cpp
    aead/tiaoxin/tiaoxinv1/tiaoxinv1_tiaoxin-reference.cpp
    aead/tiaoxin/tiaoxinv1/Tiaoxinv1.cpp
    aead/trivia-ck/trivia0v1/trivia0v1_encrypt.cpp
    aead/trivia-ck/trivia0v1/Trivia0v1.cpp
    aead/trivia-ck/trivia128v1/trivia128v1_encrypt.cpp
    aead/trivia-ck/trivia128v1/Trivia128v1.cpp
    aead/wheesht/wheeshtv1mr3fr1t128/wheeshtv1mr3fr1t128_encrypt.cpp
    aead/wheesht/wheeshtv1mr3fr1t128/Wheeshtv1mr3fr1t128.cpp
    aead/wheesht/wheeshtv1mr3fr1t256/wheeshtv1mr3fr1t256_encrypt.cpp
    aead/wheesht/wheeshtv1mr3fr1t256/Wheeshtv1mr3fr1t256.cpp
    aead/wheesht/wheeshtv1mr3fr3t256/wheeshtv1mr3fr3t256_encrypt.cpp
    aead/wheesht/wheeshtv1mr3fr3t256/Wheeshtv1mr3fr3t256.cpp
    aead/wheesht/wheeshtv1mr5fr7t256/wheeshtv1mr5fr7t256_encrypt.cpp
    aead/wheesht/wheeshtv1mr5fr7t256/Wheeshtv1mr5fr7t256.cpp
    aead/yaes/yaes128v2/yaes128v2_aes-128.cpp
    aead/yaes/yaes128v2/yaes128v2_encrypt.cpp
    aead/yaes/yaes128v2/Yaes128v2.cpp
)

set(SOURCES_WITH_OPENSSL
        aead/ae/aeadaes128ocbtaglen128v1/aeadaes128ocbtaglen128v1_encrypt.cpp
        aead/ae/aeadaes128ocbtaglen128v1/Aeadaes128ocbtaglen128v1.cpp
        aead/ae/aeadaes128ocbtaglen64v1/aeadaes128ocbtaglen64v1_encrypt.cpp
        aead/ae/aeadaes128ocbtaglen64v1/Aeadaes128ocbtaglen64v1.cpp
        aead/ae/aeadaes128ocbtaglen96v1/aeadaes128ocbtaglen96v1_encrypt.cpp
        aead/ae/aeadaes128ocbtaglen96v1/Aeadaes128ocbtaglen96v1.cpp
        aead/ae/aeadaes192ocbtaglen128v1/aeadaes192ocbtaglen128v1_encrypt.cpp
        aead/ae/aeadaes192ocbtaglen128v1/Aeadaes192ocbtaglen128v1.cpp
        aead/ae/aeadaes192ocbtaglen64v1/aeadaes192ocbtaglen64v1_encrypt.cpp
        aead/ae/aeadaes192ocbtaglen64v1/Aeadaes192ocbtaglen64v1.cpp
        aead/ae/aeadaes192ocbtaglen96v1/aeadaes192ocbtaglen96v1_encrypt.cpp
        aead/ae/aeadaes192ocbtaglen96v1/Aeadaes192ocbtaglen96v1.cpp
        aead/ae/aeadaes256ocbtaglen128v1/aeadaes256ocbtaglen128v1_encrypt.cpp
        aead/ae/aeadaes256ocbtaglen128v1/Aeadaes256ocbtaglen128v1.cpp
        aead/ae/aeadaes256ocbtaglen64v1/aeadaes256ocbtaglen64v1_encrypt.cpp
        aead/ae/aeadaes256ocbtaglen64v1/Aeadaes256ocbtaglen64v1.cpp
        aead/ae/aeadaes256ocbtaglen96v1/aeadaes256ocbtaglen96v1_encrypt.cpp
        aead/ae/aeadaes256ocbtaglen96v1/Aeadaes256ocbtaglen96v1.cpp
        aead/cba/cba1/cba1_encrypt.cpp
    aead/cba/cba1/Cba1.cpp
    aead/cba/cba10/cba10_encrypt.cpp
    aead/cba/cba10/Cba10.cpp
    aead/cba/cba2/cba2_encrypt.cpp
    aead/cba/cba2/Cba2.cpp
    aead/cba/cba3/cba3_encrypt.cpp
    aead/cba/cba3/Cba3.cpp
    aead/cba/cba4/cba4_encrypt.cpp
    aead/cba/cba4/Cba4.cpp
    aead/cba/cba5/cba5_encrypt.cpp
    aead/cba/cba5/Cba5.cpp
    aead/cba/cba6/cba6_encrypt.cpp
    aead/cba/cba6/Cba6.cpp
    aead/cba/cba7/cba7_encrypt.cpp
    aead/cba/cba7/Cba7.cpp
    aead/cba/cba8/cba8_encrypt.cpp
    aead/cba/cba8/Cba8.cpp
    aead/cba/cba9/cba9_encrypt.cpp
    aead/cba/cba9/Cba9.cpp
    aead/elmd/elmd1000v1/elmd1000v1_encrypt.cpp
    aead/elmd/elmd1000v1/Elmd1000v1.cpp
    aead/elmd/elmd1001v1/elmd1001v1_encrypt.cpp
    aead/elmd/elmd1001v1/Elmd1001v1.cpp
    aead/elmd/elmd101270v1/elmd101270v1_encrypt.cpp
    aead/elmd/elmd101270v1/Elmd101270v1.cpp
    aead/elmd/elmd101271v1/elmd101271v1_encrypt.cpp
    aead/elmd/elmd101271v1/Elmd101271v1.cpp
    aead/elmd/elmd500v1/elmd500v1_encrypt.cpp
    aead/elmd/elmd500v1/Elmd500v1.cpp
    aead/elmd/elmd501v1/elmd501v1_encrypt.cpp
    aead/elmd/elmd501v1/Elmd501v1.cpp
    aead/elmd/elmd51270v1/elmd51270v1_encrypt.cpp
    aead/elmd/elmd51270v1/Elmd51270v1.cpp
    aead/elmd/elmd51271v1/elmd51271v1_encrypt.cpp
    aead/elmd/elmd51271v1/Elmd51271v1.cpp
)

set(HEADERS
    CaesarConstants.h
    CaesarProject.h
    CaesarInterface.h
    CaesarCiphers.h
    Encryptor.h
    aead/random/RandomCipher.h
# === CAESAR algorithms ===
    aead/common/api.h
    aead/aead.h
    aead/acorn/acorn128/acorn128_api.h
    aead/acorn/acorn128/acorn128_encrypt.h
    aead/acorn/acorn128/Acorn128.h
    aead/aegis/aegis128/aegis128_api.h
    aead/aegis/aegis128/aegis128_encrypt.h
    aead/aegis/aegis128/Aegis128.h
    aead/aegis/aegis128l/aegis128l_api.h
    aead/aegis/aegis128l/aegis128l_encrypt.h
    aead/aegis/aegis128l/Aegis128l.h
    aead/aegis/aegis256/aegis256_api.h
    aead/aegis/aegis256/aegis256_encrypt.h
    aead/aegis/aegis256/Aegis256.h
# Avalanche: compilaation ok, sigsegv while running (fedora64, both debug/release)
#    aead/avalanche/aes128avalanchev1/aes128avalanchev1_api.h # compilaation ok, sigsegv while running
#    aead/avalanche/aes128avalanchev1/aes128avalanchev1_avalanche.h # compilaation ok, sigsegv while running
#    aead/avalanche/aes128avalanchev1/aes128avalanchev1_encrypt.h # compilaation ok, sigsegv while running
#    aead/avalanche/aes128avalanchev1/Aes128avalanchev1.h # compilaation ok, sigsegv while running
#    aead/avalanche/aes192avalanchev1/aes192avalanchev1_api.h
#    aead/avalanche/aes192avalanchev1/aes192avalanchev1_avalanche.h
#    aead/avalanche/aes192avalanchev1/aes192avalanchev1_encrypt.h
#    aead/avalanche/aes192avalanchev1/Aes192avalanchev1.h
#    aead/avalanche/aes256avalanchev1/aes256avalanchev1_api.h
#    aead/avalanche/aes256avalanchev1/aes256avalanchev1_avalanche.h
#    aead/avalanche/aes256avalanchev1/aes256avalanchev1_encrypt.h
#    aead/avalanche/aes256avalanchev1/Aes256avalanchev1.h
    aead/aes-gcm/aes128gcmv1/aes128gcmv1_api.h
    aead/aes-gcm/aes128gcmv1/aes128gcmv1_encrypt.h
    aead/aes-gcm/aes128gcmv1/Aes128gcmv1.h
    aead/aes-gcm/aes256gcmv1/aes256gcmv1_api.h
    aead/aes-gcm/aes256gcmv1/aes256gcmv1_encrypt.h
    aead/aes-gcm/aes256gcmv1/Aes256gcmv1.h
    aead/aes-cpfb/aes128cpfbv1/aes128cpfbv1_api.h
    aead/aes-cpfb/aes128cpfbv1/aes128cpfbv1_encrypt.h
    aead/aes-cpfb/aes128cpfbv1/Aes128cpfbv1.h
    aead/aes-cpfb/aes256cpfbv1/aes256cpfbv1_api.h
    aead/aes-cpfb/aes256cpfbv1/aes256cpfbv1_encrypt.h
    aead/aes-cpfb/aes256cpfbv1/Aes256cpfbv1.h
    aead/marble/aes128marble4rv1/aes128marble4rv1_aes.h
    aead/marble/aes128marble4rv1/aes128marble4rv1_aes_locl.h
    aead/marble/aes128marble4rv1/aes128marble4rv1_api.h
    aead/marble/aes128marble4rv1/aes128marble4rv1_marble.h
    aead/marble/aes128marble4rv1/aes128marble4rv1_encrypt.h
    aead/marble/aes128marble4rv1/Aes128marble4rv1.h
    aead/cloc/twine80n6clocv1/twine80n6clocv1_api.h
    aead/cloc/twine80n6clocv1/twine80n6clocv1_cloc.h
    aead/cloc/twine80n6clocv1/twine80n6clocv1_twine.h
    aead/cloc/twine80n6clocv1/twine80n6clocv1_encrypt.h
    aead/cloc/twine80n6clocv1/Twine80n6clocv1.h
    aead/cloc/aes128n12clocv1/aes128n12clocv1_aes.h
    aead/cloc/aes128n12clocv1/aes128n12clocv1_aes_locl.h
    aead/cloc/aes128n12clocv1/aes128n12clocv1_api.h
    aead/cloc/aes128n12clocv1/aes128n12clocv1_cloc.h
    aead/cloc/aes128n12clocv1/aes128n12clocv1_encrypt.h
    aead/cloc/aes128n12clocv1/Aes128n12clocv1.h
    aead/cloc/aes128n8clocv1/aes128n8clocv1_aes.h
    aead/cloc/aes128n8clocv1/aes128n8clocv1_aes_locl.h
    aead/cloc/aes128n8clocv1/aes128n8clocv1_api.h
    aead/cloc/aes128n8clocv1/aes128n8clocv1_cloc.h
    aead/cloc/aes128n8clocv1/aes128n8clocv1_encrypt.h
    aead/cloc/aes128n8clocv1/Aes128n8clocv1.h
    aead/silc/aes128n12silcv1/aes128n12silcv1_aes.h
    aead/silc/aes128n12silcv1/aes128n12silcv1_aes_locl.h
    aead/silc/aes128n12silcv1/aes128n12silcv1_api.h
    aead/silc/aes128n12silcv1/aes128n12silcv1_silc.h
    aead/silc/aes128n12silcv1/aes128n12silcv1_encrypt.h
    aead/silc/aes128n12silcv1/Aes128n12silcv1.h
    aead/silc/aes128n8silcv1/aes128n8silcv1_aes.h
    aead/silc/aes128n8silcv1/aes128n8silcv1_aes_locl.h
    aead/silc/aes128n8silcv1/aes128n8silcv1_api.h
    aead/silc/aes128n8silcv1/aes128n8silcv1_silc.h
    aead/silc/aes128n8silcv1/aes128n8silcv1_encrypt.h
    aead/silc/aes128n8silcv1/Aes128n8silcv1.h
    aead/silc/led80n6silcv1/led80n6silcv1_api.h
    aead/silc/led80n6silcv1/led80n6silcv1_led.h
    aead/silc/led80n6silcv1/led80n6silcv1_silc.h
    aead/silc/led80n6silcv1/led80n6silcv1_encrypt.h
    aead/silc/led80n6silcv1/Led80n6silcv1.h
    aead/silc/present80n6silcv1/present80n6silcv1_api.h
    aead/silc/present80n6silcv1/present80n6silcv1_present.h
    aead/silc/present80n6silcv1/present80n6silcv1_silc.h
    aead/silc/present80n6silcv1/present80n6silcv1_encrypt.h
    aead/silc/present80n6silcv1/Present80n6silcv1.h
    aead/aes-otr/aes128otrpv1/aes128otrpv1_api.h
    aead/aes-otr/aes128otrpv1/aes128otrpv1_OTR.h
    aead/aes-otr/aes128otrpv1/aes128otrpv1_t-aes_define.h
    aead/aes-otr/aes128otrpv1/aes128otrpv1_t-aes_table_enc_only.h
    aead/aes-otr/aes128otrpv1/aes128otrpv1_encrypt.h
    aead/aes-otr/aes128otrpv1/Aes128otrpv1.h
    aead/aes-otr/aes128otrsv1/aes128otrsv1_api.h
    aead/aes-otr/aes128otrsv1/aes128otrsv1_OTR.h
    aead/aes-otr/aes128otrsv1/aes128otrsv1_t-aes_define.h
    aead/aes-otr/aes128otrsv1/aes128otrsv1_t-aes_table_enc_only.h
    aead/aes-otr/aes128otrsv1/aes128otrsv1_encrypt.h
    aead/aes-otr/aes128otrsv1/Aes128otrsv1.h
    aead/aes-otr/aes256otrpv1/aes256otrpv1_api.h
    aead/aes-otr/aes256otrpv1/aes256otrpv1_OTR.h
    aead/aes-otr/aes256otrpv1/aes256otrpv1_t-aes_define.h
    aead/aes-otr/aes256otrpv1/aes256otrpv1_t-aes_table_enc_only.h
    aead/aes-otr/aes256otrpv1/aes256otrpv1_encrypt.h
    aead/aes-otr/aes256otrpv1/Aes256otrpv1.h
    aead/aes-otr/aes256otrsv1/aes256otrsv1_api.h
    aead/aes-otr/aes256otrsv1/aes256otrsv1_OTR.h
    aead/aes-otr/aes256otrsv1/aes256otrsv1_t-aes_define.h
    aead/aes-otr/aes256otrsv1/aes256otrsv1_t-aes_table_enc_only.h
    aead/aes-otr/aes256otrsv1/aes256otrsv1_encrypt.h
    aead/aes-otr/aes256otrsv1/Aes256otrsv1.h
    aead/poet/aes128poetv1aes128/aes128poetv1aes128_aes.h
    aead/poet/aes128poetv1aes128/aes128poetv1aes128_api.h
    aead/poet/aes128poetv1aes128/aes128poetv1aes128_gf_mul.h
    aead/poet/aes128poetv1aes128/aes128poetv1aes128_poet.h
    aead/poet/aes128poetv1aes128/aes128poetv1aes128_encrypt.h
    aead/poet/aes128poetv1aes128/Aes128poetv1aes128.h
    aead/poet/aes128poetv1aes4/aes128poetv1aes4_aes.h
    aead/poet/aes128poetv1aes4/aes128poetv1aes4_api.h
    aead/poet/aes128poetv1aes4/aes128poetv1aes4_gf_mul.h
    aead/poet/aes128poetv1aes4/aes128poetv1aes4_poet.h
    aead/poet/aes128poetv1aes4/aes128poetv1aes4_encrypt.h
    aead/poet/aes128poetv1aes4/Aes128poetv1aes4.h
    aead/aes-copa/aescopav1/aescopav1_aes-core.h
    aead/aes-copa/aescopav1/aescopav1_api.h
    aead/aes-copa/aescopav1/aescopav1_encrypt.h
    aead/aes-copa/aescopav1/Aescopav1.h
    aead/aes-jambu/aesjambuv1/aesjambuv1_aes.h
    aead/aes-jambu/aesjambuv1/aesjambuv1_api.h
    aead/aes-jambu/aesjambuv1/aesjambuv1_encrypt.h
    aead/aes-jambu/aesjambuv1/Aesjambuv1.h
    aead/aez/aezv1/aezv1_api.h
    aead/aez/aezv1/aezv1_rijndael-alg-fst.h
    aead/aez/aezv1/aezv1_encrypt.h
    aead/aez/aezv1/Aezv1.h
    aead/aez/aezv3/aezv3_api.h
    aead/aez/aezv3/aezv3_rijndael-alg-fst.h
    aead/aez/aezv3/aezv3_encrypt.h
    aead/aez/aezv3/Aezv3.h
    aead/artemia/artemia128v1/artemia128v1_api.h
    aead/artemia/artemia128v1/artemia128v1_jhae.h
    aead/artemia/artemia128v1/artemia128v1_encrypt.h
    aead/artemia/artemia128v1/Artemia128v1.h
    aead/artemia/artemia256v1/artemia256v1_api.h
    aead/artemia/artemia256v1/artemia256v1_jhae.h
    aead/artemia/artemia256v1/artemia256v1_encrypt.h
    aead/artemia/artemia256v1/Artemia256v1.h
    aead/ascon/ascon128v1/ascon128v1_api.h
    aead/ascon/ascon128v1/ascon128v1_encrypt.h
    aead/ascon/ascon128v1/Ascon128v1.h
    aead/ascon/ascon96v1/ascon96v1_api.h
    aead/ascon/ascon96v1/ascon96v1_encrypt.h
    aead/ascon/ascon96v1/Ascon96v1.h
    aead/calico/calicov8/calicov8_api.h
    aead/calico/calicov8/calicov8_calico.h
    aead/calico/calicov8/calicov8_encrypt.h
    aead/calico/calicov8/Calicov8.h
    aead/aes-cmcc/cmcc22v1/cmcc22v1_api.h
    aead/aes-cmcc/cmcc22v1/cmcc22v1_encrypt.h
    aead/aes-cmcc/cmcc22v1/Cmcc22v1.h
    aead/aes-cmcc/cmcc24v1/cmcc24v1_api.h
    aead/aes-cmcc/cmcc24v1/cmcc24v1_encrypt.h
    aead/aes-cmcc/cmcc24v1/Cmcc24v1.h
    aead/aes-cmcc/cmcc42v1/cmcc42v1_api.h
    aead/aes-cmcc/cmcc42v1/cmcc42v1_encrypt.h
    aead/aes-cmcc/cmcc42v1/Cmcc42v1.h
    aead/aes-cmcc/cmcc44v1/cmcc44v1_api.h
    aead/aes-cmcc/cmcc44v1/cmcc44v1_encrypt.h
    aead/aes-cmcc/cmcc44v1/Cmcc44v1.h
    aead/aes-cmcc/cmcc84v1/cmcc84v1_api.h
    aead/aes-cmcc/cmcc84v1/cmcc84v1_encrypt.h
    aead/aes-cmcc/cmcc84v1/Cmcc84v1.h
    aead/deoxys/deoxyseq128128v1/deoxyseq128128v1_api.h
    aead/deoxys/deoxyseq128128v1/deoxyseq128128v1_deoxys.h
    aead/deoxys/deoxyseq128128v1/deoxyseq128128v1_tweakableBC.h
    aead/deoxys/deoxyseq128128v1/deoxyseq128128v1_encrypt.h
    aead/deoxys/deoxyseq128128v1/Deoxyseq128128v1.h
    aead/deoxys/deoxyseq256128v1/deoxyseq256128v1_api.h
    aead/deoxys/deoxyseq256128v1/deoxyseq256128v1_deoxys.h
    aead/deoxys/deoxyseq256128v1/deoxyseq256128v1_tweakableBC.h
    aead/deoxys/deoxyseq256128v1/deoxyseq256128v1_encrypt.h
    aead/deoxys/deoxyseq256128v1/Deoxyseq256128v1.h
    aead/deoxys/deoxysneq128128v1/deoxysneq128128v1_api.h
    aead/deoxys/deoxysneq128128v1/deoxysneq128128v1_deoxys.h
    aead/deoxys/deoxysneq128128v1/deoxysneq128128v1_tweakableBC.h
    aead/deoxys/deoxysneq128128v1/deoxysneq128128v1_encrypt.h
    aead/deoxys/deoxysneq128128v1/Deoxysneq128128v1.h
    aead/deoxys/deoxysneq256128v1/deoxysneq256128v1_api.h
    aead/deoxys/deoxysneq256128v1/deoxysneq256128v1_deoxys.h
    aead/deoxys/deoxysneq256128v1/deoxysneq256128v1_tweakableBC.h
    aead/deoxys/deoxysneq256128v1/deoxysneq256128v1_encrypt.h
    aead/deoxys/deoxysneq256128v1/Deoxysneq256128v1.h
    aead/enchilada/enchilada128v1/enchilada128v1_api.h
    aead/enchilada/enchilada128v1/enchilada128v1_enchilada.h
    aead/enchilada/enchilada128v1/enchilada128v1_encrypt.h
    aead/enchilada/enchilada128v1/Enchilada128v1.h
    aead/enchilada/enchilada256v1/enchilada256v1_aes.h
    aead/enchilada/enchilada256v1/enchilada256v1_aesopt.h
    aead/enchilada/enchilada256v1/enchilada256v1_api.h
    aead/enchilada/enchilada256v1/enchilada256v1_enchilada.h
    aead/enchilada/enchilada256v1/enchilada256v1_encrypt.h
    aead/enchilada/enchilada256v1/Enchilada256v1.h
    aead/hs1-siv/hs1sivhiv1/hs1sivhiv1_api.h
    aead/hs1-siv/hs1sivhiv1/hs1sivhiv1_encrypt.h
    aead/hs1-siv/hs1sivhiv1/Hs1sivhiv1.h
    aead/hs1-siv/hs1sivlov1/hs1sivlov1_api.h
    aead/hs1-siv/hs1sivlov1/hs1sivlov1_encrypt.h
    aead/hs1-siv/hs1sivlov1/Hs1sivlov1.h
    aead/hs1-siv/hs1sivv1/hs1sivv1_api.h
    aead/hs1-siv/hs1sivv1/hs1sivv1_encrypt.h
    aead/hs1-siv/hs1sivv1/Hs1sivv1.h
    aead/icepole/icepole128av1/icepole128av1_api.h
    aead/icepole/icepole128av1/icepole128av1_icepole.h
    aead/icepole/icepole128av1/icepole128av1_encrypt.h
    aead/icepole/icepole128av1/Icepole128av1.h
    aead/icepole/icepole128v1/icepole128v1_api.h
    aead/icepole/icepole128v1/icepole128v1_icepole.h
    aead/icepole/icepole128v1/icepole128v1_encrypt.h
    aead/icepole/icepole128v1/Icepole128v1.h
    aead/icepole/icepole256av1/icepole256av1_api.h
    aead/icepole/icepole256av1/icepole256av1_icepole.h
    aead/icepole/icepole256av1/icepole256av1_encrypt.h
    aead/icepole/icepole256av1/Icepole256av1.h
    aead/ifeed-aes/ifeedaes128n104v1/ifeedaes128n104v1_api.h
    aead/ifeed-aes/ifeedaes128n104v1/ifeedaes128n104v1_encrypt.h
    aead/ifeed-aes/ifeedaes128n104v1/Ifeedaes128n104v1.h
    aead/ifeed-aes/ifeedaes128n96v1/ifeedaes128n96v1_api.h
    aead/ifeed-aes/ifeedaes128n96v1/ifeedaes128n96v1_encrypt.h
    aead/ifeed-aes/ifeedaes128n96v1/Ifeedaes128n96v1.h
    aead/scream/iscream12v1/iscream12v1_api.h
    aead/scream/iscream12v1/iscream12v1_lbox.h
    aead/scream/iscream12v1/iscream12v1_params.h
    aead/scream/iscream12v1/iscream12v1_sbox.h
    aead/scream/iscream12v1/iscream12v1_encrypt.h
    aead/scream/iscream12v1/Iscream12v1.h
    aead/scream/iscream12v2/iscream12v2_api.h
    aead/scream/iscream12v2/iscream12v2_lbox.h
    aead/scream/iscream12v2/iscream12v2_params.h
    aead/scream/iscream12v2/iscream12v2_sbox.h
    aead/scream/iscream12v2/iscream12v2_encrypt.h
    aead/scream/iscream12v2/Iscream12v2.h
    aead/scream/iscream14v1/iscream14v1_api.h
    aead/scream/iscream14v1/iscream14v1_lbox.h
    aead/scream/iscream14v1/iscream14v1_params.h
    aead/scream/iscream14v1/iscream14v1_sbox.h
    aead/scream/iscream14v1/iscream14v1_encrypt.h
    aead/scream/iscream14v1/Iscream14v1.h
    aead/scream/iscream14v2/iscream14v2_api.h
    aead/scream/iscream14v2/iscream14v2_lbox.h
    aead/scream/iscream14v2/iscream14v2_params.h
    aead/scream/iscream14v2/iscream14v2_sbox.h
    aead/scream/iscream14v2/iscream14v2_encrypt.h
    aead/scream/iscream14v2/Iscream14v2.h
    aead/scream/scream10v1/scream10v1_api.h
    aead/scream/scream10v1/scream10v1_lbox.h
    aead/scream/scream10v1/scream10v1_params.h
    aead/scream/scream10v1/scream10v1_sbox.h
    aead/scream/scream10v1/scream10v1_encrypt.h
    aead/scream/scream10v1/Scream10v1.h
    aead/scream/scream10v2/scream10v2_api.h
    aead/scream/scream10v2/scream10v2_lbox.h
    aead/scream/scream10v2/scream10v2_params.h
    aead/scream/scream10v2/scream10v2_sbox.h
    aead/scream/scream10v2/scream10v2_encrypt.h
    aead/scream/scream10v2/Scream10v2.h
    aead/scream/scream12v1/scream12v1_api.h
    aead/scream/scream12v1/scream12v1_lbox.h
    aead/scream/scream12v1/scream12v1_params.h
    aead/scream/scream12v1/scream12v1_sbox.h
    aead/scream/scream12v1/scream12v1_encrypt.h
    aead/scream/scream12v1/Scream12v1.h
    aead/scream/scream12v2/scream12v2_api.h
    aead/scream/scream12v2/scream12v2_lbox.h
    aead/scream/scream12v2/scream12v2_params.h
    aead/scream/scream12v2/scream12v2_sbox.h
    aead/scream/scream12v2/scream12v2_encrypt.h
    aead/scream/scream12v2/Scream12v2.h
    aead/joltik/joltikeq12864v1/joltikeq12864v1_api.h
    aead/joltik/joltikeq12864v1/joltikeq12864v1_joltik.h
    aead/joltik/joltikeq12864v1/joltikeq12864v1_tweakableBC.h
    aead/joltik/joltikeq12864v1/joltikeq12864v1_encrypt.h
    aead/joltik/joltikeq12864v1/Joltikeq12864v1.h
    aead/joltik/joltikeq6464v1/joltikeq6464v1_api.h
    aead/joltik/joltikeq6464v1/joltikeq6464v1_joltik.h
    aead/joltik/joltikeq6464v1/joltikeq6464v1_tweakableBC.h
    aead/joltik/joltikeq6464v1/joltikeq6464v1_encrypt.h
    aead/joltik/joltikeq6464v1/Joltikeq6464v1.h
    aead/joltik/joltikeq8048v1/joltikeq8048v1_api.h
    aead/joltik/joltikeq8048v1/joltikeq8048v1_joltik.h
    aead/joltik/joltikeq8048v1/joltikeq8048v1_tweakableBC.h
    aead/joltik/joltikeq8048v1/joltikeq8048v1_encrypt.h
    aead/joltik/joltikeq8048v1/Joltikeq8048v1.h
    aead/joltik/joltikeq9696v1/joltikeq9696v1_api.h
    aead/joltik/joltikeq9696v1/joltikeq9696v1_joltik.h
    aead/joltik/joltikeq9696v1/joltikeq9696v1_tweakableBC.h
    aead/joltik/joltikeq9696v1/joltikeq9696v1_encrypt.h
    aead/joltik/joltikeq9696v1/Joltikeq9696v1.h
    aead/joltik/joltikneq12864v1/joltikneq12864v1_api.h
    aead/joltik/joltikneq12864v1/joltikneq12864v1_joltik.h
    aead/joltik/joltikneq12864v1/joltikneq12864v1_tweakableBC.h
    aead/joltik/joltikneq12864v1/joltikneq12864v1_encrypt.h
    aead/joltik/joltikneq12864v1/Joltikneq12864v1.h
    aead/joltik/joltikneq6464v1/joltikneq6464v1_api.h
    aead/joltik/joltikneq6464v1/joltikneq6464v1_joltik.h
    aead/joltik/joltikneq6464v1/joltikneq6464v1_tweakableBC.h
    aead/joltik/joltikneq6464v1/joltikneq6464v1_encrypt.h
    aead/joltik/joltikneq6464v1/Joltikneq6464v1.h
    aead/joltik/joltikneq8048v1/joltikneq8048v1_api.h
    aead/joltik/joltikneq8048v1/joltikneq8048v1_joltik.h
    aead/joltik/joltikneq8048v1/joltikneq8048v1_tweakableBC.h
    aead/joltik/joltikneq8048v1/joltikneq8048v1_encrypt.h
    aead/joltik/joltikneq8048v1/Joltikneq8048v1.h
    aead/joltik/joltikneq9696v1/joltikneq9696v1_api.h
    aead/joltik/joltikneq9696v1/joltikneq9696v1_joltik.h
    aead/joltik/joltikneq9696v1/joltikneq9696v1_tweakableBC.h
    aead/joltik/joltikneq9696v1/joltikneq9696v1_encrypt.h
    aead/joltik/joltikneq9696v1/Joltikneq9696v1.h
#    aead/julius/juliusv1draft/juliusv1draft_api.h
#    aead/julius/juliusv1draft/juliusv1draft_encrypt.h
#    aead/julius/juliusv1draft/Juliusv1draft.h
    aead/ketje/ketjejrv1/ketjejrv1_api.h
    aead/ketje/ketjejrv1/ketjejrv1_brg_endian.h
    aead/ketje/ketjejrv1/ketjejrv1_displayIntermediateValues.h
    aead/ketje/ketjejrv1/ketjejrv1_KeccakF-200-interface.h
    aead/ketje/ketjejrv1/ketjejrv1_KeccakF-200-reference.h
    aead/ketje/ketjejrv1/ketjejrv1_KeccakF-interface.h
    aead/ketje/ketjejrv1/ketjejrv1_KeccakP-200-interface.h
    aead/ketje/ketjejrv1/ketjejrv1_KeccakP-200-reference.h
    aead/ketje/ketjejrv1/ketjejrv1_KeccakP-interface.h
    aead/ketje/ketjejrv1/ketjejrv1_Ket.h
    aead/ketje/ketjejrv1/ketjejrv1_Ketje.h
    aead/ketje/ketjejrv1/ketjejrv1_encrypt.h
    aead/ketje/ketjejrv1/Ketjejrv1.h
    aead/ketje/ketjesrv1/ketjesrv1_api.h
    aead/ketje/ketjesrv1/ketjesrv1_brg_endian.h
    aead/ketje/ketjesrv1/ketjesrv1_displayIntermediateValues.h
    aead/ketje/ketjesrv1/ketjesrv1_KeccakF-400-interface.h
    aead/ketje/ketjesrv1/ketjesrv1_KeccakF-400-reference.h
    aead/ketje/ketjesrv1/ketjesrv1_KeccakF-interface.h
    aead/ketje/ketjesrv1/ketjesrv1_KeccakP-400-interface.h
    aead/ketje/ketjesrv1/ketjesrv1_KeccakP-400-reference.h
    aead/ketje/ketjesrv1/ketjesrv1_KeccakP-interface.h
    aead/ketje/ketjesrv1/ketjesrv1_Ket.h
    aead/ketje/ketjesrv1/ketjesrv1_Ketje.h
    aead/ketje/ketjesrv1/ketjesrv1_encrypt.h
    aead/ketje/ketjesrv1/Ketjesrv1.h
    aead/kiasu/kiasueq128v1/kiasueq128v1_api.h
    aead/kiasu/kiasueq128v1/kiasueq128v1_crypto_aead.h
    aead/kiasu/kiasueq128v1/kiasueq128v1_kiasu.h
    aead/kiasu/kiasueq128v1/kiasueq128v1_tweakable_aes.h
    aead/kiasu/kiasueq128v1/kiasueq128v1_encrypt.h
    aead/kiasu/kiasueq128v1/Kiasueq128v1.h
    aead/kiasu/kiasuneq128v1/kiasuneq128v1_api.h
    aead/kiasu/kiasuneq128v1/kiasuneq128v1_kiasu.h
    aead/kiasu/kiasuneq128v1/kiasuneq128v1_tweakable_aes.h
    aead/kiasu/kiasuneq128v1/kiasuneq128v1_encrypt.h
    aead/kiasu/kiasuneq128v1/Kiasuneq128v1.h
    aead/lac/lacv1/lacv1_api.h
    aead/lac/lacv1/lacv1_encrypt.h
    aead/lac/lacv1/Lacv1.h
    aead/keyak/lakekeyakv1/lakekeyakv1_api.h
    aead/keyak/lakekeyakv1/lakekeyakv1_brg_endian.h
    aead/keyak/lakekeyakv1/lakekeyakv1_displayIntermediateValues.h
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakDuplex.h
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakF-1600-interface.h
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakF-1600-reference.h
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakF-interface.h
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakP-1600-12-interface.h
    aead/keyak/lakekeyakv1/lakekeyakv1_KeccakP-1600-12-reference.h
    aead/keyak/lakekeyakv1/lakekeyakv1_Keyak.h
    aead/keyak/lakekeyakv1/lakekeyakv1_NumberOfParallelInstances.h
    aead/keyak/lakekeyakv1/lakekeyakv1_encrypt.h
    aead/keyak/lakekeyakv1/Lakekeyakv1.h
    aead/keyak/oceankeyakv1/oceankeyakv1_api.h
    aead/keyak/oceankeyakv1/oceankeyakv1_brg_endian.h
    aead/keyak/oceankeyakv1/oceankeyakv1_displayIntermediateValues.h
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakDuplex.h
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakF-1600-interface.h
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakF-1600-reference.h
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakF-interface.h
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakP-1600-12-interface.h
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakP-1600-12-reference.h
    aead/keyak/oceankeyakv1/oceankeyakv1_KeccakParallelDuplex.h
    aead/keyak/oceankeyakv1/oceankeyakv1_Keyak.h
    aead/keyak/oceankeyakv1/oceankeyakv1_NumberOfParallelInstances.h
    aead/keyak/oceankeyakv1/oceankeyakv1_ParallelKeccakFs.h
    aead/keyak/oceankeyakv1/oceankeyakv1_testParallelPaSM.h
    aead/keyak/oceankeyakv1/oceankeyakv1_encrypt.h
    aead/keyak/oceankeyakv1/Oceankeyakv1.h
    aead/keyak/riverkeyakv1/riverkeyakv1_api.h
    aead/keyak/riverkeyakv1/riverkeyakv1_brg_endian.h
    aead/keyak/riverkeyakv1/riverkeyakv1_displayIntermediateValues.h
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakDuplex.h
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakF-800-interface.h
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakF-800-reference.h
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakF-interface.h
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakP-800-12-interface.h
    aead/keyak/riverkeyakv1/riverkeyakv1_KeccakP-800-12-reference.h
    aead/keyak/riverkeyakv1/riverkeyakv1_Keyak.h
    aead/keyak/riverkeyakv1/riverkeyakv1_NumberOfParallelInstances.h
    aead/keyak/riverkeyakv1/riverkeyakv1_encrypt.h
    aead/keyak/riverkeyakv1/Riverkeyakv1.h
    aead/keyak/seakeyakv1/seakeyakv1_api.h
    aead/keyak/seakeyakv1/seakeyakv1_brg_endian.h
    aead/keyak/seakeyakv1/seakeyakv1_displayIntermediateValues.h
    aead/keyak/seakeyakv1/seakeyakv1_KeccakDuplex.h
    aead/keyak/seakeyakv1/seakeyakv1_KeccakF-1600-interface.h
    aead/keyak/seakeyakv1/seakeyakv1_KeccakF-1600-reference.h
    aead/keyak/seakeyakv1/seakeyakv1_KeccakF-interface.h
    aead/keyak/seakeyakv1/seakeyakv1_KeccakP-1600-12-interface.h
    aead/keyak/seakeyakv1/seakeyakv1_KeccakP-1600-12-reference.h
    aead/keyak/seakeyakv1/seakeyakv1_KeccakParallelDuplex.h
    aead/keyak/seakeyakv1/seakeyakv1_Keyak.h
    aead/keyak/seakeyakv1/seakeyakv1_NumberOfParallelInstances.h
    aead/keyak/seakeyakv1/seakeyakv1_ParallelKeccakFs.h
    aead/keyak/seakeyakv1/seakeyakv1_testParallelPaSM.h
    aead/keyak/seakeyakv1/seakeyakv1_encrypt.h
    aead/keyak/seakeyakv1/Seakeyakv1.h
    aead/minalpher/minalpherv1/minalpherv1_api.h
    aead/minalpher/minalpherv1/minalpherv1_encrypt.h
    aead/minalpher/minalpherv1/Minalpherv1.h
    aead/morus/morus1280128v1/morus1280128v1_api.h
    aead/morus/morus1280128v1/morus1280128v1_encrypt.h
    aead/morus/morus1280128v1/Morus1280128v1.h
    aead/morus/morus1280256v1/morus1280256v1_api.h
    aead/morus/morus1280256v1/morus1280256v1_encrypt.h
    aead/morus/morus1280256v1/Morus1280256v1.h
    aead/morus/morus640128v1/morus640128v1_api.h
    aead/morus/morus640128v1/morus640128v1_encrypt.h
    aead/morus/morus640128v1/Morus640128v1.h
    aead/norx/norx3241v1/norx3241v1_api.h
    aead/norx/norx3241v1/norx3241v1_norx_config.h
    aead/norx/norx3241v1/norx3241v1_norx.h
    aead/norx/norx3241v1/norx3241v1_norx_util.h
    aead/norx/norx3241v1/norx3241v1_encrypt.h
    aead/norx/norx3241v1/Norx3241v1.h
    aead/norx/norx3261v1/norx3261v1_api.h
    aead/norx/norx3261v1/norx3261v1_norx_config.h
    aead/norx/norx3261v1/norx3261v1_norx.h
    aead/norx/norx3261v1/norx3261v1_norx_util.h
    aead/norx/norx3261v1/norx3261v1_encrypt.h
    aead/norx/norx3261v1/Norx3261v1.h
    aead/norx/norx6441v1/norx6441v1_api.h
    aead/norx/norx6441v1/norx6441v1_norx_config.h
    aead/norx/norx6441v1/norx6441v1_norx.h
    aead/norx/norx6441v1/norx6441v1_norx_util.h
    aead/norx/norx6441v1/norx6441v1_encrypt.h
    aead/norx/norx6441v1/Norx6441v1.h
    aead/norx/norx6444v1/norx6444v1_api.h
    aead/norx/norx6444v1/norx6444v1_norx_config.h
    aead/norx/norx6444v1/norx6444v1_norx.h
    aead/norx/norx6444v1/norx6444v1_norx_util.h
    aead/norx/norx6444v1/norx6444v1_encrypt.h
    aead/norx/norx6444v1/Norx6444v1.h
    aead/norx/norx6461v1/norx6461v1_api.h
    aead/norx/norx6461v1/norx6461v1_norx_config.h
    aead/norx/norx6461v1/norx6461v1_norx.h
    aead/norx/norx6461v1/norx6461v1_norx_util.h
    aead/norx/norx6461v1/norx6461v1_encrypt.h
    aead/norx/norx6461v1/Norx6461v1.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_api.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_inttypes_mingw.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_inttypes_win.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_md32_common.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_omd_api.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_omdsha256.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_sha256.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_stdint_win.h
    aead/omd/omdsha256k128n96tau128v1/omdsha256k128n96tau128v1_encrypt.h
    aead/omd/omdsha256k128n96tau128v1/Omdsha256k128n96tau128v1.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_api.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_inttypes_mingw.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_inttypes_win.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_md32_common.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_omd_api.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_omdsha256.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_sha256.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_stdint_win.h
    aead/omd/omdsha256k128n96tau64v1/omdsha256k128n96tau64v1_encrypt.h
    aead/omd/omdsha256k128n96tau64v1/Omdsha256k128n96tau64v1.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_api.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_inttypes_mingw.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_inttypes_win.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_md32_common.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_omd_api.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_omdsha256.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_sha256.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_stdint_win.h
    aead/omd/omdsha256k128n96tau96v1/omdsha256k128n96tau96v1_encrypt.h
    aead/omd/omdsha256k128n96tau96v1/Omdsha256k128n96tau96v1.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_api.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_inttypes_mingw.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_inttypes_win.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_md32_common.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_omd_api.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_omdsha256.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_sha256.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_stdint_win.h
    aead/omd/omdsha256k192n104tau128v1/omdsha256k192n104tau128v1_encrypt.h
    aead/omd/omdsha256k192n104tau128v1/Omdsha256k192n104tau128v1.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_api.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_inttypes_mingw.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_inttypes_win.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_md32_common.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_omd_api.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_omdsha256.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_sha256.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_stdint_win.h
    aead/omd/omdsha256k256n104tau160v1/omdsha256k256n104tau160v1_encrypt.h
    aead/omd/omdsha256k256n104tau160v1/Omdsha256k256n104tau160v1.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_api.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_inttypes_mingw.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_inttypes_win.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_md32_common.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_omd_api.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_omdsha256.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_sha256.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_stdint_win.h
    aead/omd/omdsha256k256n248tau256v1/omdsha256k256n248tau256v1_encrypt.h
    aead/omd/omdsha256k256n248tau256v1/Omdsha256k256n248tau256v1.h
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_api.h
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_inttypes_mingw.h
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_inttypes_win.h
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_omd_api.h
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_omdsha512.h
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_sha512.h
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_stdint_win.h
    aead/omd/omdsha512k128n128tau128v1/omdsha512k128n128tau128v1_encrypt.h
    aead/omd/omdsha512k128n128tau128v1/Omdsha512k128n128tau128v1.h
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_api.h
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_inttypes_mingw.h
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_inttypes_win.h
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_omd_api.h
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_omdsha512.h
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_sha512.h
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_stdint_win.h
    aead/omd/omdsha512k256n256tau256v1/omdsha512k256n256tau256v1_encrypt.h
    aead/omd/omdsha512k256n256tau256v1/Omdsha512k256n256tau256v1.h
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_api.h
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_inttypes_mingw.h
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_inttypes_win.h
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_omd_api.h
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_omdsha512.h
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_sha512.h
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_stdint_win.h
    aead/omd/omdsha512k512n256tau256v1/omdsha512k512n256tau256v1_encrypt.h
    aead/omd/omdsha512k512n256tau256v1/Omdsha512k512n256tau256v1.h
    aead/paeq/paeq128/paeq128_api.h
    aead/paeq/paeq128/paeq128_encrypt.h
    aead/paeq/paeq128/Paeq128.h
    aead/paeq/paeq128t/paeq128t_api.h
    aead/paeq/paeq128t/paeq128t_encrypt.h
    aead/paeq/paeq128t/Paeq128t.h
    aead/paeq/paeq128tnm/paeq128tnm_api.h
    aead/paeq/paeq128tnm/paeq128tnm_encrypt.h
    aead/paeq/paeq128tnm/Paeq128tnm.h
    aead/paeq/paeq160/paeq160_api.h
    aead/paeq/paeq160/paeq160_encrypt.h
    aead/paeq/paeq160/Paeq160.h
    aead/paeq/paeq64/paeq64_api.h
    aead/paeq/paeq64/paeq64_encrypt.h
    aead/paeq/paeq64/Paeq64.h
    aead/paeq/paeq80/paeq80_api.h
    aead/paeq/paeq80/paeq80_encrypt.h
    aead/paeq/paeq80/Paeq80.h
    aead/pi-cipher/pi16cipher096v1/pi16cipher096v1_api.h
    aead/pi-cipher/pi16cipher096v1/pi16cipher096v1_encrypt.h
    aead/pi-cipher/pi16cipher096v1/Pi16cipher096v1.h
    aead/pi-cipher/pi16cipher128v1/pi16cipher128v1_api.h
    aead/pi-cipher/pi16cipher128v1/pi16cipher128v1_encrypt.h
    aead/pi-cipher/pi16cipher128v1/Pi16cipher128v1.h
    aead/pi-cipher/pi32cipher128v1/pi32cipher128v1_api.h
    aead/pi-cipher/pi32cipher128v1/pi32cipher128v1_encrypt.h
    aead/pi-cipher/pi32cipher128v1/Pi32cipher128v1.h
    aead/pi-cipher/pi32cipher256v1/pi32cipher256v1_api.h
    aead/pi-cipher/pi32cipher256v1/pi32cipher256v1_encrypt.h
    aead/pi-cipher/pi32cipher256v1/Pi32cipher256v1.h
    aead/pi-cipher/pi64cipher128v1/pi64cipher128v1_api.h
    aead/pi-cipher/pi64cipher128v1/pi64cipher128v1_encrypt.h
    aead/pi-cipher/pi64cipher128v1/Pi64cipher128v1.h
    aead/pi-cipher/pi64cipher256v1/pi64cipher256v1_api.h
    aead/pi-cipher/pi64cipher256v1/pi64cipher256v1_encrypt.h
    aead/pi-cipher/pi64cipher256v1/Pi64cipher256v1.h
    aead/pi-cipher/pi64cipher256v1oneround/pi64cipher256v1oneround_api.h
    aead/pi-cipher/pi64cipher256v1oneround/pi64cipher256v1oneround_encrypt.h
    aead/pi-cipher/pi64cipher256v1oneround/Pi64cipher256v1oneround.h
    aead/pi-cipher/pi64cipher256v1tworounds/pi64cipher256v1tworounds_api.h
    aead/pi-cipher/pi64cipher256v1tworounds/pi64cipher256v1tworounds_encrypt.h
    aead/pi-cipher/pi64cipher256v1tworounds/Pi64cipher256v1tworounds.h
#    aead/polawis/polawisv1/polawisv1_encrypt.h
#    aead/polawis/polawisv1/Polawisv1.h
    aead/paes/ppaev11/ppaev11_api.h
    aead/paes/ppaev11/ppaev11_encrypt.h
    aead/paes/ppaev11/Ppaev11.h
    aead/primates/primatesv1ape120/primatesv1ape120_api.h
    aead/primates/primatesv1ape120/primatesv1ape120_parameters.h
    aead/primates/primatesv1ape120/primatesv1ape120_primate.h
    aead/primates/primatesv1ape120/primatesv1ape120_encrypt.h
    aead/primates/primatesv1ape120/Primatesv1ape120.h
    aead/primates/primatesv1ape80/primatesv1ape80_api.h
    aead/primates/primatesv1ape80/primatesv1ape80_parameters.h
    aead/primates/primatesv1ape80/primatesv1ape80_primate.h
    aead/primates/primatesv1ape80/primatesv1ape80_encrypt.h
    aead/primates/primatesv1ape80/Primatesv1ape80.h
    aead/primates/primatesv1gibbon120/primatesv1gibbon120_api.h
    aead/primates/primatesv1gibbon120/primatesv1gibbon120_parameters.h
    aead/primates/primatesv1gibbon120/primatesv1gibbon120_primate.h
    aead/primates/primatesv1gibbon120/primatesv1gibbon120_encrypt.h
    aead/primates/primatesv1gibbon120/Primatesv1gibbon120.h
    aead/primates/primatesv1gibbon80/primatesv1gibbon80_api.h
    aead/primates/primatesv1gibbon80/primatesv1gibbon80_parameters.h
    aead/primates/primatesv1gibbon80/primatesv1gibbon80_primate.h
    aead/primates/primatesv1gibbon80/primatesv1gibbon80_encrypt.h
    aead/primates/primatesv1gibbon80/Primatesv1gibbon80.h
    aead/primates/primatesv1hanuman120/primatesv1hanuman120_api.h
    aead/primates/primatesv1hanuman120/primatesv1hanuman120_parameters.h
    aead/primates/primatesv1hanuman120/primatesv1hanuman120_primate.h
    aead/primates/primatesv1hanuman120/primatesv1hanuman120_encrypt.h
    aead/primates/primatesv1hanuman120/Primatesv1hanuman120.h
    aead/primates/primatesv1hanuman80/primatesv1hanuman80_api.h
    aead/primates/primatesv1hanuman80/primatesv1hanuman80_parameters.h
    aead/primates/primatesv1hanuman80/primatesv1hanuman80_primate.h
    aead/primates/primatesv1hanuman80/primatesv1hanuman80_encrypt.h
    aead/primates/primatesv1hanuman80/Primatesv1hanuman80.h
    aead/prost/proest128apev1/proest128apev1_ape.h
    aead/prost/proest128apev1/proest128apev1_api.h
    aead/prost/proest128apev1/proest128apev1_proest128.h
    aead/prost/proest128apev1/proest128apev1_zerobytes.h
    aead/prost/proest128apev1/proest128apev1_encrypt.h
    aead/prost/proest128apev1/Proest128apev1.h
    aead/prost/proest128copav1/proest128copav1_api.h
    aead/prost/proest128copav1/proest128copav1_copa.h
    aead/prost/proest128copav1/proest128copav1_proest128.h
    aead/prost/proest128copav1/proest128copav1_zerobytes.h
    aead/prost/proest128copav1/proest128copav1_encrypt.h
    aead/prost/proest128copav1/Proest128copav1.h
    aead/prost/proest128otrv1/proest128otrv1_api.h
    aead/prost/proest128otrv1/proest128otrv1_otr.h
    aead/prost/proest128otrv1/proest128otrv1_proest128.h
    aead/prost/proest128otrv1/proest128otrv1_zerobytes.h
    aead/prost/proest128otrv1/proest128otrv1_encrypt.h
    aead/prost/proest128otrv1/Proest128otrv1.h
    aead/prost/proest256apev1/proest256apev1_ape.h
    aead/prost/proest256apev1/proest256apev1_api.h
    aead/prost/proest256apev1/proest256apev1_proest256.h
    aead/prost/proest256apev1/proest256apev1_zerobytes.h
    aead/prost/proest256apev1/proest256apev1_encrypt.h
    aead/prost/proest256apev1/Proest256apev1.h
    aead/prost/proest256copav1/proest256copav1_api.h
    aead/prost/proest256copav1/proest256copav1_copa.h
    aead/prost/proest256copav1/proest256copav1_proest256.h
    aead/prost/proest256copav1/proest256copav1_zerobytes.h
    aead/prost/proest256copav1/proest256copav1_encrypt.h
    aead/prost/proest256copav1/Proest256copav1.h
    aead/prost/proest256otrv1/proest256otrv1_api.h
    aead/prost/proest256otrv1/proest256otrv1_otr.h
    aead/prost/proest256otrv1/proest256otrv1_proest256.h
    aead/prost/proest256otrv1/proest256otrv1_zerobytes.h
    aead/prost/proest256otrv1/proest256otrv1_encrypt.h
    aead/prost/proest256otrv1/Proest256otrv1.h
    aead/raviyoyla/raviyoylav1/raviyoylav1_api.h
    aead/raviyoyla/raviyoylav1/raviyoylav1_encrypt.h
    aead/raviyoyla/raviyoylav1/Raviyoylav1.h
    aead/sablier/sablierv1/sablierv1_api.h
    aead/sablier/sablierv1/sablierv1_authenticate_1.h
    aead/sablier/sablierv1/sablierv1_cipher_1.h
    aead/sablier/sablierv1/sablierv1_encrypt.h
    aead/sablier/sablierv1/Sablierv1.h
    aead/shell/shellaes128v1d4n64/shellaes128v1d4n64_aes.h
    aead/shell/shellaes128v1d4n64/shellaes128v1d4n64_api.h
    aead/shell/shellaes128v1d4n64/shellaes128v1d4n64_shellaes.h
    aead/shell/shellaes128v1d4n64/shellaes128v1d4n64_encrypt.h
    aead/shell/shellaes128v1d4n64/Shellaes128v1d4n64.h
    aead/shell/shellaes128v1d4n80/shellaes128v1d4n80_aes.h
    aead/shell/shellaes128v1d4n80/shellaes128v1d4n80_api.h
    aead/shell/shellaes128v1d4n80/shellaes128v1d4n80_shellaes.h
    aead/shell/shellaes128v1d4n80/shellaes128v1d4n80_encrypt.h
    aead/shell/shellaes128v1d4n80/Shellaes128v1d4n80.h
    aead/shell/shellaes128v1d5n64/shellaes128v1d5n64_aes.h
    aead/shell/shellaes128v1d5n64/shellaes128v1d5n64_api.h
    aead/shell/shellaes128v1d5n64/shellaes128v1d5n64_shellaes.h
    aead/shell/shellaes128v1d5n64/shellaes128v1d5n64_encrypt.h
    aead/shell/shellaes128v1d5n64/Shellaes128v1d5n64.h
    aead/shell/shellaes128v1d5n80/shellaes128v1d5n80_aes.h
    aead/shell/shellaes128v1d5n80/shellaes128v1d5n80_api.h
    aead/shell/shellaes128v1d5n80/shellaes128v1d5n80_shellaes.h
    aead/shell/shellaes128v1d5n80/shellaes128v1d5n80_encrypt.h
    aead/shell/shellaes128v1d5n80/Shellaes128v1d5n80.h
    aead/shell/shellaes128v1d6n64/shellaes128v1d6n64_aes.h
    aead/shell/shellaes128v1d6n64/shellaes128v1d6n64_api.h
    aead/shell/shellaes128v1d6n64/shellaes128v1d6n64_shellaes.h
    aead/shell/shellaes128v1d6n64/shellaes128v1d6n64_encrypt.h
    aead/shell/shellaes128v1d6n64/Shellaes128v1d6n64.h
    aead/shell/shellaes128v1d6n80/shellaes128v1d6n80_aes.h
    aead/shell/shellaes128v1d6n80/shellaes128v1d6n80_api.h
    aead/shell/shellaes128v1d6n80/shellaes128v1d6n80_shellaes.h
    aead/shell/shellaes128v1d6n80/shellaes128v1d6n80_encrypt.h
    aead/shell/shellaes128v1d6n80/Shellaes128v1d6n80.h
    aead/shell/shellaes128v1d7n64/shellaes128v1d7n64_aes.h
    aead/shell/shellaes128v1d7n64/shellaes128v1d7n64_api.h
    aead/shell/shellaes128v1d7n64/shellaes128v1d7n64_shellaes.h
    aead/shell/shellaes128v1d7n64/shellaes128v1d7n64_encrypt.h
    aead/shell/shellaes128v1d7n64/Shellaes128v1d7n64.h
    aead/shell/shellaes128v1d7n80/shellaes128v1d7n80_aes.h
    aead/shell/shellaes128v1d7n80/shellaes128v1d7n80_api.h
    aead/shell/shellaes128v1d7n80/shellaes128v1d7n80_shellaes.h
    aead/shell/shellaes128v1d7n80/shellaes128v1d7n80_encrypt.h
    aead/shell/shellaes128v1d7n80/Shellaes128v1d7n80.h
    aead/shell/shellaes128v1d8n64/shellaes128v1d8n64_aes.h
    aead/shell/shellaes128v1d8n64/shellaes128v1d8n64_api.h
    aead/shell/shellaes128v1d8n64/shellaes128v1d8n64_shellaes.h
    aead/shell/shellaes128v1d8n64/shellaes128v1d8n64_encrypt.h
    aead/shell/shellaes128v1d8n64/Shellaes128v1d8n64.h
    aead/shell/shellaes128v1d8n80/shellaes128v1d8n80_aes.h
    aead/shell/shellaes128v1d8n80/shellaes128v1d8n80_api.h
    aead/shell/shellaes128v1d8n80/shellaes128v1d8n80_shellaes.h
    aead/shell/shellaes128v1d8n80/shellaes128v1d8n80_encrypt.h
    aead/shell/shellaes128v1d8n80/Shellaes128v1d8n80.h
    aead/silver/silverv1/silverv1_api.h
    aead/silver/silverv1/silverv1_rijndaelEndianNeutral.h
    aead/silver/silverv1/silverv1_encrypt.h
    aead/silver/silverv1/Silverv1.h
    aead/stribob/stribob192r1/stribob192r1_api.h
    aead/stribob/stribob192r1/stribob192r1_stribob.h
    aead/stribob/stribob192r1/stribob192r1_encrypt.h
    aead/stribob/stribob192r1/Stribob192r1.h
    aead/tiaoxin/tiaoxinv1/tiaoxinv1_aes_round.h
    aead/tiaoxin/tiaoxinv1/tiaoxinv1_api.h
    aead/tiaoxin/tiaoxinv1/tiaoxinv1_tiaoxin-reference.h
    aead/tiaoxin/tiaoxinv1/tiaoxinv1_encrypt.h
    aead/tiaoxin/tiaoxinv1/Tiaoxinv1.h
    aead/trivia-ck/trivia0v1/trivia0v1_api.h
    aead/trivia-ck/trivia0v1/trivia0v1_encrypt.h
    aead/trivia-ck/trivia0v1/Trivia0v1.h
    aead/trivia-ck/trivia128v1/trivia128v1_api.h
    aead/trivia-ck/trivia128v1/trivia128v1_encrypt.h
    aead/trivia-ck/trivia128v1/Trivia128v1.h
    aead/wheesht/wheeshtv1mr3fr1t128/wheeshtv1mr3fr1t128_api.h
    aead/wheesht/wheeshtv1mr3fr1t128/wheeshtv1mr3fr1t128_encrypt.h
    aead/wheesht/wheeshtv1mr3fr1t128/wheeshtv1mr3fr1t128_encrypt-original.h
    aead/wheesht/wheeshtv1mr3fr1t128/Wheeshtv1mr3fr1t128.h
    aead/wheesht/wheeshtv1mr3fr1t256/wheeshtv1mr3fr1t256_api.h
    aead/wheesht/wheeshtv1mr3fr1t256/wheeshtv1mr3fr1t256_encrypt.h
    aead/wheesht/wheeshtv1mr3fr1t256/wheeshtv1mr3fr1t256_encrypt-original.h
    aead/wheesht/wheeshtv1mr3fr1t256/Wheeshtv1mr3fr1t256.h
    aead/wheesht/wheeshtv1mr3fr3t256/wheeshtv1mr3fr3t256_api.h
    aead/wheesht/wheeshtv1mr3fr3t256/wheeshtv1mr3fr3t256_encrypt.h
    aead/wheesht/wheeshtv1mr3fr3t256/wheeshtv1mr3fr3t256_encrypt-original.h
    aead/wheesht/wheeshtv1mr3fr3t256/Wheeshtv1mr3fr3t256.h
    aead/wheesht/wheeshtv1mr5fr7t256/wheeshtv1mr5fr7t256_api.h
    aead/wheesht/wheeshtv1mr5fr7t256/wheeshtv1mr5fr7t256_encrypt.h
    aead/wheesht/wheeshtv1mr5fr7t256/wheeshtv1mr5fr7t256_encrypt-original.h
    aead/wheesht/wheeshtv1mr5fr7t256/Wheeshtv1mr5fr7t256.h
    aead/yaes/yaes128v2/yaes128v2_aes-128.h
    aead/yaes/yaes128v2/yaes128v2_api.h
    aead/yaes/yaes128v2/yaes128v2_encrypt.h
    aead/yaes/yaes128v2/yaes128v2_encrypt.h
    aead/yaes/yaes128v2/Yaes128v2.h
)

set(HEADERS_WITH_OPENSSL
    aead/ae/aeadaes128ocbtaglen128v1/aeadaes128ocbtaglen128v1_api.h
    aead/ae/aeadaes128ocbtaglen128v1/aeadaes128ocbtaglen128v1_encrypt.h
    aead/ae/aeadaes128ocbtaglen128v1/Aeadaes128ocbtaglen128v1.h
    aead/ae/aeadaes128ocbtaglen64v1/aeadaes128ocbtaglen64v1_api.h
    aead/ae/aeadaes128ocbtaglen64v1/aeadaes128ocbtaglen64v1_encrypt.h
    aead/ae/aeadaes128ocbtaglen64v1/Aeadaes128ocbtaglen64v1.h
    aead/ae/aeadaes128ocbtaglen96v1/aeadaes128ocbtaglen96v1_api.h
    aead/ae/aeadaes128ocbtaglen96v1/aeadaes128ocbtaglen96v1_encrypt.h
    aead/ae/aeadaes128ocbtaglen96v1/Aeadaes128ocbtaglen96v1.h
    aead/ae/aeadaes192ocbtaglen128v1/aeadaes192ocbtaglen128v1_api.h
    aead/ae/aeadaes192ocbtaglen128v1/aeadaes192ocbtaglen128v1_encrypt.h
    aead/ae/aeadaes192ocbtaglen128v1/Aeadaes192ocbtaglen128v1.h
    aead/ae/aeadaes192ocbtaglen64v1/aeadaes192ocbtaglen64v1_api.h
    aead/ae/aeadaes192ocbtaglen64v1/aeadaes192ocbtaglen64v1_encrypt.h
    aead/ae/aeadaes192ocbtaglen64v1/Aeadaes192ocbtaglen64v1.h
    aead/ae/aeadaes192ocbtaglen96v1/aeadaes192ocbtaglen96v1_api.h
    aead/ae/aeadaes192ocbtaglen96v1/aeadaes192ocbtaglen96v1_encrypt.h
    aead/ae/aeadaes192ocbtaglen96v1/Aeadaes192ocbtaglen96v1.h
    aead/ae/aeadaes256ocbtaglen128v1/aeadaes256ocbtaglen128v1_api.h
    aead/ae/aeadaes256ocbtaglen128v1/aeadaes256ocbtaglen128v1_encrypt.h
    aead/ae/aeadaes256ocbtaglen128v1/Aeadaes256ocbtaglen128v1.h
    aead/ae/aeadaes256ocbtaglen64v1/aeadaes256ocbtaglen64v1_api.h
    aead/ae/aeadaes256ocbtaglen64v1/aeadaes256ocbtaglen64v1_encrypt.h
    aead/ae/aeadaes256ocbtaglen64v1/Aeadaes256ocbtaglen64v1.h
    aead/ae/aeadaes256ocbtaglen96v1/aeadaes256ocbtaglen96v1_api.h
    aead/ae/aeadaes256ocbtaglen96v1/aeadaes256ocbtaglen96v1_encrypt.h
    aead/ae/aeadaes256ocbtaglen96v1/Aeadaes256ocbtaglen96v1.h
    aead/cba/cba1/cba1_api.h
    aead/cba/cba1/cba1_encrypt.h
    aead/cba/cba1/Cba1.h
    aead/cba/cba10/cba10_api.h
    aead/cba/cba10/cba10_encrypt.h
    aead/cba/cba10/Cba10.h
    aead/cba/cba2/cba2_api.h
    aead/cba/cba2/cba2_encrypt.h
    aead/cba/cba2/Cba2.h
    aead/cba/cba3/cba3_api.h
    aead/cba/cba3/cba3_encrypt.h
    aead/cba/cba3/Cba3.h
    aead/cba/cba4/cba4_api.h
    aead/cba/cba4/cba4_encrypt.h
    aead/cba/cba4/Cba4.h
    aead/cba/cba5/cba5_api.h
    aead/cba/cba5/cba5_encrypt.h
    aead/cba/cba5/Cba5.h
    aead/cba/cba6/cba6_api.h
    aead/cba/cba6/cba6_encrypt.h
    aead/cba/cba6/Cba6.h
    aead/cba/cba7/cba7_api.h
    aead/cba/cba7/cba7_encrypt.h
    aead/cba/cba7/Cba7.h
    aead/cba/cba8/cba8_api.h
    aead/cba/cba8/cba8_encrypt.h
    aead/cba/cba8/Cba8.h
    aead/cba/cba9/cba9_api.h
    aead/cba/cba9/cba9_encrypt.h
    aead/cba/cba9/Cba9.h
    aead/elmd/elmd1000v1/elmd1000v1_aes_round_5.h
    aead/elmd/elmd1000v1/elmd1000v1_api.h
    aead/elmd/elmd1000v1/elmd1000v1_module.h
    aead/elmd/elmd1000v1/elmd1000v1_encrypt.h
    aead/elmd/elmd1000v1/Elmd1000v1.h
    aead/elmd/elmd1001v1/elmd1001v1_aes_round_5.h
    aead/elmd/elmd1001v1/elmd1001v1_api.h
    aead/elmd/elmd1001v1/elmd1001v1_module.h
    aead/elmd/elmd1001v1/elmd1001v1_encrypt.h
    aead/elmd/elmd1001v1/Elmd1001v1.h
    aead/elmd/elmd101270v1/elmd101270v1_aes_round_5.h
    aead/elmd/elmd101270v1/elmd101270v1_api.h
    aead/elmd/elmd101270v1/elmd101270v1_module.h
    aead/elmd/elmd101270v1/elmd101270v1_encrypt.h
    aead/elmd/elmd101270v1/Elmd101270v1.h
    aead/elmd/elmd101271v1/elmd101271v1_aes_round_5.h
    aead/elmd/elmd101271v1/elmd101271v1_api.h
    aead/elmd/elmd101271v1/elmd101271v1_module.h
    aead/elmd/elmd101271v1/elmd101271v1_encrypt.h
    aead/elmd/elmd101271v1/Elmd101271v1.h
    aead/elmd/elmd500v1/elmd500v1_aes_round_5.h
    aead/elmd/elmd500v1/elmd500v1_api.h
    aead/elmd/elmd500v1/elmd500v1_module.h
    aead/elmd/elmd500v1/elmd500v1_encrypt.h
    aead/elmd/elmd500v1/Elmd500v1.h
    aead/elmd/elmd501v1/elmd501v1_aes_round_5.h
    aead/elmd/elmd501v1/elmd501v1_api.h
    aead/elmd/elmd501v1/elmd501v1_module.h
    aead/elmd/elmd501v1/elmd501v1_encrypt.h
    aead/elmd/elmd501v1/Elmd501v1.h
    aead/elmd/elmd51270v1/elmd51270v1_aes_round_5.h
    aead/elmd/elmd51270v1/elmd51270v1_api.h
    aead/elmd/elmd51270v1/elmd51270v1_module.h
    aead/elmd/elmd51270v1/elmd51270v1_encrypt.h
    aead/elmd/elmd51270v1/Elmd51270v1.h
    aead/elmd/elmd51271v1/elmd51271v1_aes_round_5.h
    aead/elmd/elmd51271v1/elmd51271v1_api.h
    aead/elmd/elmd51271v1/elmd51271v1_module.h
    aead/elmd/elmd51271v1/elmd51271v1_encrypt.h
    aead/elmd/elmd51271v1/Elmd51271v1.h
)

set(OTHER_FILES
    aead/common/About.md
    aead/acorn/acorn128/About.md
    aead/aegis/aegis128/About.md
    aead/aegis/aegis128l/About.md
    aead/aegis/aegis256/About.md
# Avalanche: compilaation ok, sigsegv while running (fedora64, both debug/release)
#    aead/avalanche/aes128avalanchev1/About.md # compilaation ok, sigsegv while running
#    aead/avalanche/aes192avalanchev1/About.md
#    aead/avalanche/aes256avalanchev1/About.md
    aead/aes-gcm/aes128gcmv1/About.md
    aead/aes-gcm/aes256gcmv1/About.md
    aead/aes-cpfb/aes128cpfbv1/About.md
    aead/aes-cpfb/aes256cpfbv1/About.md
    aead/marble/aes128marble4rv1/About.md
    aead/cloc/twine80n6clocv1/About.md
    aead/cloc/aes128n12clocv1/About.md
    aead/cloc/aes128n8clocv1/About.md
    aead/silc/aes128n12silcv1/About.md
    aead/silc/aes128n8silcv1/About.md
    aead/silc/led80n6silcv1/About.md
    aead/silc/present80n6silcv1/About.md
    aead/aes-otr/aes128otrpv1/About.md
    aead/aes-otr/aes128otrsv1/About.md
    aead/aes-otr/aes256otrpv1/About.md
    aead/aes-otr/aes256otrsv1/About.md
    aead/poet/aes128poetv1aes128/About.md
    aead/poet/aes128poetv1aes4/About.md
    aead/aes-copa/aescopav1/About.md
    aead/aes-jambu/aesjambuv1/About.md
    aead/aez/aezv1/About.md
    aead/aez/aezv3/About.md
    aead/artemia/artemia128v1/About.md
    aead/artemia/artemia256v1/About.md
    aead/ascon/ascon128v1/About.md
    aead/ascon/ascon96v1/About.md
    aead/calico/calicov8/About.md
    aead/aes-cmcc/cmcc22v1/About.md
    aead/aes-cmcc/cmcc24v1/About.md
    aead/aes-cmcc/cmcc42v1/About.md
    aead/aes-cmcc/cmcc44v1/About.md
    aead/aes-cmcc/cmcc84v1/About.md
    aead/deoxys/deoxyseq128128v1/About.md
    aead/deoxys/deoxyseq256128v1/About.md
    aead/deoxys/deoxysneq128128v1/About.md
    aead/deoxys/deoxysneq256128v1/About.md
    aead/enchilada/enchilada128v1/About.md
    aead/enchilada/enchilada256v1/About.md
    aead/hs1-siv/hs1sivhiv1/About.md
    aead/hs1-siv/hs1sivlov1/About.md
    aead/hs1-siv/hs1sivv1/About.md
    aead/icepole/icepole128av1/About.md
    aead/icepole/icepole128v1/About.md
    aead/icepole/icepole256av1/About.md
    aead/ifeed-aes/ifeedaes128n104v1/About.md
    aead/ifeed-aes/ifeedaes128n96v1/About.md
    aead/scream/iscream12v1/About.md
    aead/scream/iscream12v2/About.md
    aead/scream/iscream14v1/About.md
    aead/scream/iscream14v2/About.md
    aead/scream/scream10v1/About.md
    aead/scream/scream10v2/About.md
    aead/scream/scream12v1/About.md
    aead/scream/scream12v2/About.md
    aead/joltik/joltikeq12864v1/About.md
    aead/joltik/joltikeq6464v1/About.md
    aead/joltik/joltikeq8048v1/About.md
    aead/joltik/joltikeq9696v1/About.md
    aead/joltik/joltikneq12864v1/About.md
    aead/joltik/joltikneq6464v1/About.md
    aead/joltik/joltikneq8048v1/About.md
    aead/joltik/joltikneq9696v1/About.md
#    aead/julius/juliusv1draft/About.md
    aead/ketje/ketjejrv1/About.md
    aead/ketje/ketjesrv1/About.md
    aead/kiasu/kiasueq128v1/About.md
    aead/kiasu/kiasuneq128v1/About.md
    aead/lac/lacv1/About.md
    aead/keyak/lakekeyakv1/About.md
    aead/keyak/oceankeyakv1/About.md
    aead/keyak/riverkeyakv1/About.md
    aead/keyak/seakeyakv1/About.md
    aead/minalpher/minalpherv1/About.md
    aead/morus/morus1280128v1/About.md
    aead/morus/morus1280256v1/About.md
    aead/morus/morus640128v1/About.md
    aead/norx/norx3241v1/About.md
    aead/norx/norx3261v1/About.md
    aead/norx/norx6441v1/About.md
    aead/norx/norx6444v1/About.md
    aead/norx/norx6461v1/About.md
    aead/omd/omdsha256k128n96tau128v1/About.md
    aead/omd/omdsha256k128n96tau64v1/About.md
    aead/omd/omdsha256k128n96tau96v1/About.md
    aead/omd/omdsha256k192n104tau128v1/About.md
    aead/omd/omdsha256k256n104tau160v1/About.md
    aead/omd/omdsha256k256n248tau256v1/About.md
    aead/omd/omdsha512k128n128tau128v1/About.md
    aead/omd/omdsha512k256n256tau256v1/About.md
    aead/omd/omdsha512k512n256tau256v1/About.md
    aead/paeq/paeq128/About.md
    aead/paeq/paeq128t/About.md
    aead/paeq/paeq128tnm/About.md
    aead/paeq/paeq160/About.md
    aead/paeq/paeq64/About.md
    aead/paeq/paeq80/About.md
    aead/pi-cipher/pi16cipher096v1/About.md
    aead/pi-cipher/pi16cipher128v1/About.md
    aead/pi-cipher/pi32cipher128v1/About.md
    aead/pi-cipher/pi32cipher256v1/About.md
    aead/pi-cipher/pi64cipher128v1/About.md
    aead/pi-cipher/pi64cipher256v1/About.md
    aead/pi-cipher/pi64cipher256v1oneround/About.md
    aead/pi-cipher/pi64cipher256v1tworounds/About.md
#    aead/polawis/polawisv1/About.md
    aead/paes/ppaev11/About.md
    aead/primates/primatesv1ape120/About.md
    aead/primates/primatesv1ape80/About.md
    aead/primates/primatesv1gibbon120/About.md
    aead/primates/primatesv1gibbon80/About.md
    aead/primates/primatesv1hanuman120/About.md
    aead/primates/primatesv1hanuman80/About.md
    aead/prost/proest128apev1/About.md
    aead/prost/proest128copav1/About.md
    aead/prost/proest128otrv1/About.md
    aead/prost/proest256apev1/About.md
    aead/prost/proest256copav1/About.md
    aead/prost/proest256otrv1/About.md
    aead/raviyoyla/raviyoylav1/About.md
    aead/sablier/sablierv1/About.md
    aead/shell/shellaes128v1d4n64/About.md
    aead/shell/shellaes128v1d4n80/About.md
    aead/shell/shellaes128v1d5n64/About.md
    aead/shell/shellaes128v1d5n80/About.md
    aead/shell/shellaes128v1d6n64/About.md
    aead/shell/shellaes128v1d6n80/About.md
    aead/shell/shellaes128v1d7n64/About.md
    aead/shell/shellaes128v1d7n80/About.md
    aead/shell/shellaes128v1d8n64/About.md
    aead/shell/shellaes128v1d8n80/About.md
    aead/silver/silverv1/About.md
    aead/stribob/stribob192r1/About.md
    aead/tiaoxin/tiaoxinv1/About.md
    aead/trivia-ck/trivia0v1/About.md
    aead/trivia-ck/trivia128v1/About.md
    aead/wheesht/wheeshtv1mr3fr1t128/About.md
    aead/wheesht/wheeshtv1mr3fr1t256/About.md
    aead/wheesht/wheeshtv1mr3fr3t256/About.md
    aead/wheesht/wheeshtv1mr5fr7t256/About.md
    aead/yaes/yaes128v2/About.md
)

set(OTHER_FILES_WITH_OPENSSL
    aead/ae/aeadaes128ocbtaglen128v1/About.md
    aead/ae/aeadaes128ocbtaglen64v1/About.md
    aead/ae/aeadaes128ocbtaglen96v1/About.md
    aead/ae/aeadaes192ocbtaglen128v1/About.md
    aead/ae/aeadaes192ocbtaglen64v1/About.md
    aead/ae/aeadaes192ocbtaglen96v1/About.md
    aead/ae/aeadaes256ocbtaglen128v1/About.md
    aead/ae/aeadaes256ocbtaglen64v1/About.md
    aead/ae/aeadaes256ocbtaglen96v1/About.md
    aead/cba/cba1/About.md
    aead/cba/cba10/About.md
    aead/cba/cba2/About.md
    aead/cba/cba3/About.md
    aead/cba/cba4/About.md
    aead/cba/cba5/About.md
    aead/cba/cba6/About.md
    aead/cba/cba7/About.md
    aead/cba/cba8/About.md
    aead/cba/cba9/About.md
    aead/elmd/elmd1000v1/About.md
    aead/elmd/elmd1001v1/About.md
    aead/elmd/elmd101270v1/About.md
    aead/elmd/elmd101271v1/About.md
    aead/elmd/elmd500v1/About.md
    aead/elmd/elmd501v1/About.md
    aead/elmd/elmd51270v1/About.md
    aead/elmd/elmd51271v1/About.md
)

# === source groups ===
source_group("aead"     REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/.*")
source_group("aead\\acorn\\acorn128"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/acorn/acorn128/.*")
source_group("aead\\ae\\aeadaes128ocbtaglen128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes128ocbtaglen128v1/.*")
source_group("aead\\ae\\aeadaes128ocbtaglen64v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes128ocbtaglen64v1/.*")
source_group("aead\\ae\\aeadaes128ocbtaglen96v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes128ocbtaglen96v1/.*")
source_group("aead\\ae\\aeadaes192ocbtaglen128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes192ocbtaglen128v1/.*")
source_group("aead\\ae\\aeadaes192ocbtaglen64v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes192ocbtaglen64v1/.*")
source_group("aead\\ae\\aeadaes192ocbtaglen96v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes192ocbtaglen96v1/.*")
source_group("aead\\ae\\aeadaes256ocbtaglen128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes256ocbtaglen128v1/.*")
source_group("aead\\ae\\aeadaes256ocbtaglen64v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes256ocbtaglen64v1/.*")
source_group("aead\\ae\\aeadaes256ocbtaglen96v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ae/aeadaes256ocbtaglen96v1/.*")
source_group("aead\\aegis\\aegis128"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aegis/aegis128/.*")
source_group("aead\\aegis\\aegis128l"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aegis/aegis128l/.*")
source_group("aead\\aegis\\aegis256"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aegis/aegis256/.*")
source_group("aead\\aes-cmcc\\cmcc22v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-cmcc/cmcc22v1/.*")
source_group("aead\\aes-cmcc\\cmcc24v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-cmcc/cmcc24v1/.*")
source_group("aead\\aes-cmcc\\cmcc42v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-cmcc/cmcc42v1/.*")
source_group("aead\\aes-cmcc\\cmcc44v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-cmcc/cmcc44v1/.*")
source_group("aead\\aes-cmcc\\cmcc84v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-cmcc/cmcc84v1/.*")
source_group("aead\\aes-copa\\aescopav1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-copa/aescopav1/.*")
source_group("aead\\aes-cpfb\\aes128cpfbv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-cpfb/aes128cpfbv1/.*")
source_group("aead\\aes-cpfb\\aes256cpfbv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-cpfb/aes256cpfbv1/.*")
source_group("aead\\aes-gcm\\aes128gcmv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-gcm/aes128gcmv1/.*")
source_group("aead\\aes-gcm\\aes256gcmv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-gcm/aes256gcmv1/.*")
source_group("aead\\aes-jambu\\aesjambuv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-jambu/aesjambuv1/.*")
source_group("aead\\aes-otr\\aes128otrpv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-otr/aes128otrpv1/.*")
source_group("aead\\aes-otr\\aes128otrsv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-otr/aes128otrsv1/.*")
source_group("aead\\aes-otr\\aes256otrpv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-otr/aes256otrpv1/.*")
source_group("aead\\aes-otr\\aes256otrsv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aes-otr/aes256otrsv1/.*")
source_group("aead\\aez\\aezv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aez/aezv1/.*")
source_group("aead\\aez\\aezv3"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/aez/aezv3/.*")
source_group("aead\\artemia\\artemia128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/artemia/artemia128v1/.*")
source_group("aead\\artemia\\artemia256v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/artemia/artemia256v1/.*")
source_group("aead\\ascon\\ascon128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ascon/ascon128v1/.*")
source_group("aead\\ascon\\ascon96v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ascon/ascon96v1/.*")
source_group("aead\\avalanche\\aes128avalanchev1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/avalanche/aes128avalanchev1/.*")
source_group("aead\\avalanche\\aes192avalanchev1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/avalanche/aes192avalanchev1/.*")
source_group("aead\\avalanche\\aes256avalanchev1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/avalanche/aes256avalanchev1/.*")
source_group("aead\\calico\\calicov8"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/calico/calicov8/.*")
source_group("aead\\cba\\cba1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba1/.*")
source_group("aead\\cba\\cba10"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba10/.*")
source_group("aead\\cba\\cba2"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba2/.*")
source_group("aead\\cba\\cba3"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba3/.*")
source_group("aead\\cba\\cba4"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba4/.*")
source_group("aead\\cba\\cba5"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba5/.*")
source_group("aead\\cba\\cba6"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba6/.*")
source_group("aead\\cba\\cba7"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba7/.*")
source_group("aead\\cba\\cba8"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba8/.*")
source_group("aead\\cba\\cba9"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cba/cba9/.*")
source_group("aead\\cloc\\aes128n12clocv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cloc/aes128n12clocv1/.*")
source_group("aead\\cloc\\aes128n8clocv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cloc/aes128n8clocv1/.*")
source_group("aead\\cloc\\twine80n6clocv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/cloc/twine80n6clocv1/.*")
source_group("aead\\common"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/common/.*")
source_group("aead\\deoxys\\deoxyseq128128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/deoxys/deoxyseq128128v1/.*")
source_group("aead\\deoxys\\deoxyseq256128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/deoxys/deoxyseq256128v1/.*")
source_group("aead\\deoxys\\deoxysneq128128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/deoxys/deoxysneq128128v1/.*")
source_group("aead\\deoxys\\deoxysneq256128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/deoxys/deoxysneq256128v1/.*")
source_group("aead\\elmd\\elmd1000v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/elmd/elmd1000v1/.*")
source_group("aead\\elmd\\elmd1001v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/elmd/elmd1001v1/.*")
source_group("aead\\elmd\\elmd101270v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/elmd/elmd101270v1/.*")
source_group("aead\\elmd\\elmd101271v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/elmd/elmd101271v1/.*")
source_group("aead\\elmd\\elmd500v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/elmd/elmd500v1/.*")
source_group("aead\\elmd\\elmd501v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/elmd/elmd501v1/.*")
source_group("aead\\elmd\\elmd51270v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/elmd/elmd51270v1/.*")
source_group("aead\\elmd\\elmd51271v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/elmd/elmd51271v1/.*")
source_group("aead\\enchilada\\enchilada128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/enchilada/enchilada128v1/.*")
source_group("aead\\enchilada\\enchilada256v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/enchilada/enchilada256v1/.*")
source_group("aead\\hs1-siv\\hs1sivhiv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/hs1-siv/hs1sivhiv1/.*")
source_group("aead\\hs1-siv\\hs1sivlov1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/hs1-siv/hs1sivlov1/.*")
source_group("aead\\hs1-siv\\hs1sivv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/hs1-siv/hs1sivv1/.*")
source_group("aead\\icepole\\icepole128av1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/icepole/icepole128av1/.*")
source_group("aead\\icepole\\icepole128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/icepole/icepole128v1/.*")
source_group("aead\\icepole\\icepole256av1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/icepole/icepole256av1/.*")
source_group("aead\\ifeed-aes\\ifeedaes128n104v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ifeed-aes/ifeedaes128n104v1/.*")
source_group("aead\\ifeed-aes\\ifeedaes128n96v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ifeed-aes/ifeedaes128n96v1/.*")
source_group("aead\\joltik\\joltikeq12864v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/joltik/joltikeq12864v1/.*")
source_group("aead\\joltik\\joltikeq6464v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/joltik/joltikeq6464v1/.*")
source_group("aead\\joltik\\joltikeq8048v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/joltik/joltikeq8048v1/.*")
source_group("aead\\joltik\\joltikeq9696v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/joltik/joltikeq9696v1/.*")
source_group("aead\\joltik\\joltikneq12864v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/joltik/joltikneq12864v1/.*")
source_group("aead\\joltik\\joltikneq6464v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/joltik/joltikneq6464v1/.*")
source_group("aead\\joltik\\joltikneq8048v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/joltik/joltikneq8048v1/.*")
source_group("aead\\joltik\\joltikneq9696v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/joltik/joltikneq9696v1/.*")
source_group("aead\\julius\\juliusv1draft"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/julius/juliusv1draft/.*")
source_group("aead\\ketje\\ketjejrv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ketje/ketjejrv1/.*")
source_group("aead\\ketje\\ketjesrv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/ketje/ketjesrv1/.*")
source_group("aead\\keyak\\lakekeyakv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/keyak/lakekeyakv1/.*")
source_group("aead\\keyak\\oceankeyakv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/keyak/oceankeyakv1/.*")
source_group("aead\\keyak\\riverkeyakv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/keyak/riverkeyakv1/.*")
source_group("aead\\keyak\\seakeyakv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/keyak/seakeyakv1/.*")
source_group("aead\\kiasu\\kiasueq128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/kiasu/kiasueq128v1/.*")
source_group("aead\\kiasu\\kiasuneq128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/kiasu/kiasuneq128v1/.*")
source_group("aead\\lac\\lacv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/lac/lacv1/.*")
source_group("aead\\marble\\aes128marble4rv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/marble/aes128marble4rv1/.*")
source_group("aead\\minalpher\\minalpherv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/minalpher/minalpherv1/.*")
source_group("aead\\morus\\morus1280128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/morus/morus1280128v1/.*")
source_group("aead\\morus\\morus1280256v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/morus/morus1280256v1/.*")
source_group("aead\\morus\\morus640128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/morus/morus640128v1/.*")
source_group("aead\\norx\\norx3241v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/norx/norx3241v1/.*")
source_group("aead\\norx\\norx3261v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/norx/norx3261v1/.*")
source_group("aead\\norx\\norx6441v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/norx/norx6441v1/.*")
source_group("aead\\norx\\norx6444v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/norx/norx6444v1/.*")
source_group("aead\\norx\\norx6461v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/norx/norx6461v1/.*")
source_group("aead\\omd\\omdsha256k128n96tau128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha256k128n96tau128v1/.*")
source_group("aead\\omd\\omdsha256k128n96tau64v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha256k128n96tau64v1/.*")
source_group("aead\\omd\\omdsha256k128n96tau96v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha256k128n96tau96v1/.*")
source_group("aead\\omd\\omdsha256k192n104tau128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha256k192n104tau128v1/.*")
source_group("aead\\omd\\omdsha256k256n104tau160v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha256k256n104tau160v1/.*")
source_group("aead\\omd\\omdsha256k256n248tau256v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha256k256n248tau256v1/.*")
source_group("aead\\omd\\omdsha512k128n128tau128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha512k128n128tau128v1/.*")
source_group("aead\\omd\\omdsha512k256n256tau256v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha512k256n256tau256v1/.*")
source_group("aead\\omd\\omdsha512k512n256tau256v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/omd/omdsha512k512n256tau256v1/.*")
source_group("aead\\paeq\\paeq128"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/paeq/paeq128/.*")
source_group("aead\\paeq\\paeq128t"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/paeq/paeq128t/.*")
source_group("aead\\paeq\\paeq128tnm"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/paeq/paeq128tnm/.*")
source_group("aead\\paeq\\paeq160"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/paeq/paeq160/.*")
source_group("aead\\paeq\\paeq64"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/paeq/paeq64/.*")
source_group("aead\\paeq\\paeq80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/paeq/paeq80/.*")
source_group("aead\\paes\\ppaev11"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/paes/ppaev11/.*")
source_group("aead\\pi-cipher\\pi16cipher096v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/pi-cipher/pi16cipher096v1/.*")
source_group("aead\\pi-cipher\\pi16cipher128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/pi-cipher/pi16cipher128v1/.*")
source_group("aead\\pi-cipher\\pi32cipher128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/pi-cipher/pi32cipher128v1/.*")
source_group("aead\\pi-cipher\\pi32cipher256v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/pi-cipher/pi32cipher256v1/.*")
source_group("aead\\pi-cipher\\pi64cipher128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/pi-cipher/pi64cipher128v1/.*")
source_group("aead\\pi-cipher\\pi64cipher256v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/pi-cipher/pi64cipher256v1/.*")
source_group("aead\\pi-cipher\\pi64cipher256v1oneround"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/pi-cipher/pi64cipher256v1oneround/.*")
source_group("aead\\pi-cipher\\pi64cipher256v1tworounds"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/pi-cipher/pi64cipher256v1tworounds/.*")
source_group("aead\\poet\\aes128poetv1aes128"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/poet/aes128poetv1aes128/.*")
source_group("aead\\poet\\aes128poetv1aes4"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/poet/aes128poetv1aes4/.*")
source_group("aead\\polawis\\polawisv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/polawis/polawisv1/.*")
source_group("aead\\primates\\primatesv1ape120"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/primates/primatesv1ape120/.*")
source_group("aead\\primates\\primatesv1ape80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/primates/primatesv1ape80/.*")
source_group("aead\\primates\\primatesv1gibbon120"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/primates/primatesv1gibbon120/.*")
source_group("aead\\primates\\primatesv1gibbon80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/primates/primatesv1gibbon80/.*")
source_group("aead\\primates\\primatesv1hanuman120"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/primates/primatesv1hanuman120/.*")
source_group("aead\\primates\\primatesv1hanuman80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/primates/primatesv1hanuman80/.*")
source_group("aead\\prost\\proest128apev1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/prost/proest128apev1/.*")
source_group("aead\\prost\\proest128copav1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/prost/proest128copav1/.*")
source_group("aead\\prost\\proest128otrv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/prost/proest128otrv1/.*")
source_group("aead\\prost\\proest256apev1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/prost/proest256apev1/.*")
source_group("aead\\prost\\proest256copav1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/prost/proest256copav1/.*")
source_group("aead\\prost\\proest256otrv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/prost/proest256otrv1/.*")
source_group("aead\\raviyoyla\\raviyoylav1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/raviyoyla/raviyoylav1/.*")
source_group("aead\\sablier\\sablierv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/sablier/sablierv1/.*")
source_group("aead\\scream\\iscream12v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/scream/iscream12v1/.*")
source_group("aead\\scream\\iscream12v2"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/scream/iscream12v2/.*")
source_group("aead\\scream\\iscream14v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/scream/iscream14v1/.*")
source_group("aead\\scream\\iscream14v2"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/scream/iscream14v2/.*")
source_group("aead\\scream\\scream10v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/scream/scream10v1/.*")
source_group("aead\\scream\\scream10v2"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/scream/scream10v2/.*")
source_group("aead\\scream\\scream12v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/scream/scream12v1/.*")
source_group("aead\\scream\\scream12v2"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/scream/scream12v2/.*")
source_group("aead\\shell\\shellaes128v1d4n64"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d4n64/.*")
source_group("aead\\shell\\shellaes128v1d4n80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d4n80/.*")
source_group("aead\\shell\\shellaes128v1d5n64"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d5n64/.*")
source_group("aead\\shell\\shellaes128v1d5n80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d5n80/.*")
source_group("aead\\shell\\shellaes128v1d6n64"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d6n64/.*")
source_group("aead\\shell\\shellaes128v1d6n80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d6n80/.*")
source_group("aead\\shell\\shellaes128v1d7n64"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d7n64/.*")
source_group("aead\\shell\\shellaes128v1d7n80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d7n80/.*")
source_group("aead\\shell\\shellaes128v1d8n64"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d8n64/.*")
source_group("aead\\shell\\shellaes128v1d8n80"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/shell/shellaes128v1d8n80/.*")
source_group("aead\\silc\\aes128n12silcv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/silc/aes128n12silcv1/.*")
source_group("aead\\silc\\aes128n8silcv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/silc/aes128n8silcv1/.*")
source_group("aead\\silc\\led80n6silcv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/silc/led80n6silcv1/.*")
source_group("aead\\silc\\present80n6silcv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/silc/present80n6silcv1/.*")
source_group("aead\\silver\\silverv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/silver/silverv1/.*")
source_group("aead\\stribob\\stribob192r1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/stribob/stribob192r1/.*")
source_group("aead\\tiaoxin\\tiaoxinv1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/tiaoxin/tiaoxinv1/.*")
source_group("aead\\trivia-ck\\trivia0v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/trivia-ck/trivia0v1/.*")
source_group("aead\\trivia-ck\\trivia128v1"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/trivia-ck/trivia128v1/.*")
source_group("aead\\wheesht\\wheeshtv1mr3fr1t128"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/wheesht/wheeshtv1mr3fr1t128/.*")
source_group("aead\\wheesht\\wheeshtv1mr3fr1t256"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/wheesht/wheeshtv1mr3fr1t256/.*")
source_group("aead\\wheesht\\wheeshtv1mr3fr3t256"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/wheesht/wheeshtv1mr3fr3t256/.*")
source_group("aead\\wheesht\\wheeshtv1mr5fr7t256"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/wheesht/wheeshtv1mr5fr7t256/.*")
source_group("aead\\yaes\\yaes128v2"    REGULAR_EXPRESSION "${CMAKE_CURRENT_SOURCE_DIR}/aead/yaes/yaes128v2/.*")

# === detect OpenSSL ===
find_package(OpenSSL)
if(OPENSSL_FOUND)
    option(BUILD_CAESAR_WITH_OPENSSL "Link to OpenSSL" ON)

    if(BUILD_CAESAR_WITH_OPENSSL)
        list(APPEND SOURCES ${SOURCES_WITH_OPENSSL})
        list(APPEND HEADERS ${HEADERS_WITH_OPENSSL})
        list(APPEND OTHER_FILES ${OTHER_FILES_WITH_OPENSSL})
        add_definitions(-DWITH_OPENSSL)
        include_directories(${OPENSSL_INCLUDE_DIR})
        list(APPEND LIBRARIES ${OPENSSL_LIBRARIES})
	# minor engineering hack, static OpenSSL requires ldl and lz in linux
        if(STATIC_BUILD)
            list(APPEND LIBRARIES dl z)
        endif()
    endif()
else()
        message("OpenSSL not found, not building function requiring it.")
endif()

add_eacirc_project(caesar ${SOURCES} ${HEADERS} ${OTHER_FILES})
target_link_libraries(caesar tinyXML ${LIBRARIES})
