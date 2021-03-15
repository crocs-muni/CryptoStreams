## LocMC test vectors

- https://github.com/LowMC/lowmc.git
- e847fb160ad8ca1f373efd91a55b6d67f7deb425
- https://lowmc.github.io/fewmul/talks/tiessen.pdf LowMCv3 presentation, recommended parameters
- https://github.com/microsoft/Picnic/blob/master/spec/spec-v3.0.pdf Picnic v3 docs with LowMC parameters
- https://tosc.iacr.org/index.php/ToSC/article/view/8751/8349  Cryptanalysis of LowMC instances using single plaintext/ciphertext pair
- https://eprint.iacr.org/2020/1034.pdf - Cryptanalysis of Full LowMC and LowMC-M with Algebraic Techniques
- 

Note: demo program outputs in big-endian, loading is via little-endian (test_vectors_12.txt).
`std::bitset<N>` is printing content in big-endia (= bitset[0] is the last printed byte),
e.g.,

```
bytearray = d5ff0000
bitset[0..8] = d5

output: 00000000000000001111111111010101
```

Converting binary result from the demo program to byte array, where bitset[0] 
```python
import binascii

def lowmc0(x, s=32):
    return binascii.hexlify(int(x,2).to_bytes(s, 'little')).decode('utf8')
```

## Strategy

All rounds are set to 12 so test vectors are fast. Even though some parameters do not have round number 12 as a target value
for AES-like security considering the key size.

Number of S-boxes is taken from default parameter set from `lowmc.h`, searching from the start of parameter array by 
block size and key size (block tests do not support passing custom json now so we could specify number of Sboxes).

Tests are generated so we test:
- Templated LowMC with exact block size and key size
- Universal LowMC template with <1024, 256> template, but real block size and key size is smaller (192 in our case)
- Testing both bit and byte-level template conversions with 

`lowmc_ref.h`:
```cpp
#ifdef BUILD_testsuite
    constexpr size_t bit_bound = 128;  // testing bit bound to cover both bit conversions
#else
    constexpr size_t bit_bound = 600;
#endif
```


## Test vectors

This test vector is valid but too heavy to generate for running in CI.

```
deadc0de000000000000
ffffffffffffffff000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
8e67d0d8a53b347a20f7e692f003df4abc68da28af159fbc9de7daa5561907eca07bb0259cb19bea90c19375d091b9e2ec3d65a0a74fc9de77e7264550c7d868ba3407df522ccc6a0d26dbe924ded90cc8d5ef5856bdc66386d292660a88ac7d09812f020df2605feb65bd739646c1eb3e4259ace1d9c60b93bf211f2d6f553c
```
