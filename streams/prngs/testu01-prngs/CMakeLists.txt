# TestU01 downloaded from: http://simul.iro.umontreal.ca/testu01/TestU01.zip date of zip: 10-Feb-2015 19:23

file(GLOB testu01-includes
        "includes/*.h"
        "includes/*.c"
        )


add_library(testu01-prngs STATIC EXCLUDE_FROM_ALL
        ${testu01-includes}

        # interface
        testu01_factory.cc
        testu01_interface.h

        # unif01
        functions/unif01/unif01.c # unif01.h is basic header file needed for all geenrators so it is in include directory not here
        functions/unif01/dummy_generator.h

        # ulcg
        functions/ulcg/ulcg.h
        functions/ulcg/ulcg.c
        functions/ulcg/ulcg_generator.h

        # umrg
        functions/umrg/umrg.h
        functions/umrg/umrg.c
        functions/umrg/umrg_generator.h

        # ucarry
        functions/ucarry/ucarry.h
        functions/ucarry/ucarry.c
        functions/ucarry/ucarry_generator.h

        # uxorshift
        functions/uxorshift/uxorshift.h
        functions/uxorshift/uxorshift.c
        functions/uxorshift/uxorshift_generator.h
        )

target_link_libraries(testu01-prngs eacirc-core)
