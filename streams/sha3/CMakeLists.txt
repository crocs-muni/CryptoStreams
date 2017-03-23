add_library(sha3 STATIC EXCLUDE_FROM_ALL
    sha3_factory
    sha3_stream
    sha3_interface
    hash_functions/hash_functions
# === SHA-3 hash function files ===
    hash_functions/Abacus/Abacus_sha3
    hash_functions/ARIRANG/Arirang_OP32
    hash_functions/ARIRANG/Arirang_sha3
    hash_functions/Aurora/Aurora_sha3
    hash_functions/Blake/Blake_sha3
    hash_functions/Blender/Blender_sha3
    hash_functions/BMW/BMW_sha3
    hash_functions/Boole/Boole_sha3
    hash_functions/Cheetah/Cheetah_sha3
    hash_functions/CHI/chi-fast32
    hash_functions/CHI/Chi_sha3
    hash_functions/CRUNCH/const_32
    hash_functions/CRUNCH/crunch_224
    hash_functions/CRUNCH/crunch_256
    hash_functions/CRUNCH/crunch_384
    hash_functions/CRUNCH/crunch_512
    hash_functions/CRUNCH/Crunch_sha3
    hash_functions/CubeHash/CubeHash_sha3
    hash_functions/DCH/DCH_sha3
    hash_functions/DynamicSHA2/DSHA2_sha3
    hash_functions/DynamicSHA/DSHA_sha3
    hash_functions/ECHO/Echo_sha3
    hash_functions/EDON/Edon_sha3
    hash_functions/ESSENCE/essence_compress_256
    hash_functions/ESSENCE/essence_compress_512
    hash_functions/ESSENCE/essence_L_tables
    hash_functions/ESSENCE/Essence_sha3
    hash_functions/Fugue/fugue_256
    hash_functions/Fugue/fugue_384
    hash_functions/Fugue/fugue_512
    hash_functions/Fugue/fugue
    hash_functions/Fugue/Fugue_sha3
    hash_functions/Grostl/Grostl_sha3
    hash_functions/Hamsi/hamsi-exp
    hash_functions/Hamsi/Hamsi_sha3
    hash_functions/Hamsi/hamsi-tables
    hash_functions/Hamsi/i.hamsi-ref
    hash_functions/JH/JH_sha3
    hash_functions/Keccak/KeccakDuplex
    hash_functions/Keccak/KeccakF-1600-opt32
    hash_functions/Keccak/Keccak_sha3
    hash_functions/Keccak/KeccakSponge
    hash_functions/Khichidi/khichidi_core
    hash_functions/Khichidi/Khichidi_sha3
    hash_functions/Lane/Lane_sha3
    hash_functions/Lesamnta/Lesamnta_sha3
    hash_functions/Luffa/Luffa_sha3
    hash_functions/MCSSHA3/Mcssha_sha3
    hash_functions/MD6/md6_compress
    hash_functions/MD6/md6_mode
    hash_functions/MD6/MD6_sha3
    hash_functions/MeshHash/MeshHash_sha3
    hash_functions/NaSHA/Nasha_sha3
    hash_functions/Sarmal/Sarmal_sha3
    hash_functions/Shabal/Shabal_sha3
    hash_functions/Shamata/Shamata_sha3
    hash_functions/SHAvite3/SHAvite_sha3
    hash_functions/SIMD/optimized
    hash_functions/SIMD/Simd_sha3
    hash_functions/Skein/skein_block
    hash_functions/Skein/skein
    hash_functions/Skein/skein_debug
    hash_functions/Skein/Skein_sha3
    hash_functions/SpectralHash/SpectralHash_sha3
    hash_functions/StreamHash/StreamHash_sha3
    hash_functions/Tangle/Tangle_sha3
    hash_functions/Twister/Twister_sha3
    hash_functions/Vortex/vortex_core
    hash_functions/Vortex/vortex_misc
    hash_functions/Vortex/Vortex_sha3
    hash_functions/Vortex/vortex_tables
    hash_functions/WaMM/BitArray
    hash_functions/WaMM/ReverseBits
    hash_functions/WaMM/WaMM
    hash_functions/WaMM/WaMMErrorMessage
    hash_functions/WaMM/WaMMOperator
    hash_functions/WaMM/Wamm_sha3
    hash_functions/WaMM/WaMMTappingPrimes
    hash_functions/Waterfall/Waterfall_sha3
    hash_functions/Tangle2/Tangle2_sha3
    #    hash_functions/ECOH/ecoh.param
    #    hash_functions/ECOH/Ecoh_sha3
    #    hash_functions/EnRUPT/EnRUPT_opt
    #    hash_functions/EnRUPT/Enrupt_sha3
    #    hash_functions/LUX/Lux_sha3
    #    hash_functions/SANDstorm/Sandstorm_sha3
    #    hash_functions/SWIFFTX/SWIFFTX
    #    hash_functions/SWIFFTX/Swifftx_sha3
    #    hash_functions/TIB3/inupfin256
    #    hash_functions/TIB3/inupfin512
    #    hash_functions/TIB3/Tib_sha3
    )
target_link_libraries(sha3 core)
