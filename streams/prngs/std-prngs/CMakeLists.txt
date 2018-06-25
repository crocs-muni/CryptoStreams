add_library(std-prngs STATIC EXCLUDE_FROM_ALL

        # interface
        std_prng_factory.cc
        std_prng_interface.h

        functions/lcg_generator.h
        functions/subtract_with_carry_generator.h
        functions/mesenne_twister_generator.h
        )

target_link_libraries(std-prngs eacirc-core)
