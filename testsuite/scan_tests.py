import os
import re

regex_test_call = r'\(\"((\\.|[^"\\])*)\", ?(\d*)\)'

def get_folder_names(source_path):
    return {x[0].split('/')[-1] for x in os.walk(source_path)} - {''}

def get_ciphers_from_tests(path_to_test_file, regex):
    result = set()
    with open(path_to_test_file) as in_file:
        for line in in_file.readlines():
            match = regex.search(line)
            if match:
                result.add(match.group(1))
    return result

def analyze_block_ciphers():
    # try to keep this empty:
    set_of_untested = set([
        'common', 'lightweight', 'rotations',  # mathced paths, that are actually not ciphers = no tests
        'sparx', 'rectangle', 'robin_star', 'road_runner', # these should be tested!
    ])
    folders = get_folder_names('streams/block/ciphers/')
    
    regex = re.compile('testsuite::block_test_case' + regex_test_call)
    tests = get_ciphers_from_tests('./testsuite/block_streams_tests.cc', regex)
    tests = set(map(lambda x: x.lower(), tests))
    
    # special fixes:
    
    # des and 3-DES are both in single directory des
    # if tests for them exists, add 'des' to the list of tests
    if {'single-des', 'triple-des'}.issubset(tests):
        tests.add('des')
        tests.remove('triple-des')
        tests.remove('single-des')

    if (folders - set_of_untested) - tests:
        print("[Error] missing tests of following functions: " + str((folders - set_of_untested) - tests))
        return -1
    elif (tests - folders):
        print("[Warning] following functions tested, but they miss folders: " + str(tests - folders))
    else:
        print("[OK] Block ciphers are fine")
    return 0


def analyze_hash_funs():
    # try to keep this empty:
    set_of_untested = set([
        'bmw', 'blender', 'crunch', 'boole', 'sarmal',  # probably should test
        'khichidi', 'fugue', 'spectralhash', 'mcssha3', 'lane', 'nasha', 'edon', 'essence', 'wamm', 'shavite3', 'meshhash', 'shamata', 'waterfall', 'shabal', 'streamhash', 'chi',  # would be nice to test, but it is ok
        'lux', 'swifftx', 'enrupt', 'tib3', 'vortex', 'ecoh', 'sandstorm'  # no code = no tests
    ])
    folders = get_folder_names('streams/hash/others/')
    folders.update(get_folder_names('streams/hash/sha3/'))
    folders = set(map(lambda x: x.lower(), folders))
    
    
    regex = re.compile('testsuite::hash_test_case' + regex_test_call)
    tests = get_ciphers_from_tests('./testsuite/hash_streams_tests.cc', regex)
    tests = set(map(lambda x: x.lower(), tests))
    
    # special fixes:
    # we test only single implementation of tangle - should we change this?:
    if 'tangle' in tests:
        tests.add('tangle2')
    folders.remove('hash_functions')

    if (folders - set_of_untested) - tests:
        print("[Error] missing tests of following functions: " + str((folders - set_of_untested) - tests))
        return -1
    elif (tests - folders):
        print("[Warning] following functions tested, but they miss folders: " + str(tests - folders))
    else:
        print("[OK] Hash functions are fine")
    return 0

def analyze_stream_ciphers():
    # try to keep this empty:
    set_of_untested = set([
        'mir-1', 'py', 'sfinks', 'pomaranch', 'mag', 'dicing', 'wg', 'achterbahn', 'dragon',  # probably should test
        'edon80', 'zk-crypt', 'cryptmt', 'abc',  # no-run - test can find why
        'yamb', 'polarbear', 'moustique', 'nls'  # no-compile = no tests
    ])
    folders = get_folder_names('streams/stream_ciphers/other/')
    folders.update(get_folder_names('streams/stream_ciphers/estream/'))
    folders = set(map(lambda x: x.lower(), folders))
    
    
    regex = re.compile('testsuite::stream_cipher_test_case' + regex_test_call)
    tests = get_ciphers_from_tests('./testsuite/stream_ciphers_streams_tests.cc', regex)
    tests = set(map(lambda x: x.lower(), tests))
    
    # special fixes:

    if (folders - set_of_untested) - tests:
        print("[Error] missing tests of following functions: " + str((folders - set_of_untested) - tests))
        return -1
    elif (tests - folders):
        print("[Warning] following functions tested, but they miss folders: " + str(tests - folders))
    else:
        print("[OK] Hash functions are fine")
    return 0


def main():
    result = 0
    result += analyze_block_ciphers()
    result += analyze_hash_funs()
    result += analyze_stream_ciphers()
    exit(result)


if __name__ == "__main__":
    main()
