/*********************************************************
 *
 * Filename     : fugue.c
 *
 * Originator   : Charanjit S. Jutla
 *
 * Purpose      : Optimized Implementation of Fugue for
 *                224 and 256 bit hash sizes
 *
 *********************************************************/

#include <time.h>
#include "fugue_t.h"
#include "aestab.h"

// set the IMPL_TYPE to _ASM for ASM_INLINE
// eet it to _OPT for C OPTIMIZED
//same rules hold for EXTRA_ROUND_IMPL_TYPE, though they cant be set to _ASM...yet !
// ASS_INLINE will be used in some macro names as a suiffix _ASM
// and   C OPTIMIZED as _OPT

/* Uncomment the desired default configuration */
#if !defined(CONFIG_ASM_INLINE) && !defined(CONFIG_C_OPTIMIZED)
//#define CONFIG_ASM_INLINE
#define CONFIG_C_OPTIMIZED
#endif

#ifdef CONFIG_ASM_INLINE
#define IMPL_TYPE  _ASM
#endif

#ifdef CONFIG_C_OPTIMIZED
#define IMPL_TYPE  _OPT
#endif

#define EXTRA_ROUND_IMPL_TYPE  _OPT

#define FUGUE_SIZE 30
#define FUGUE_FAR 24


#define column(i) col[i]
#define row(i) row[i]


#define vector2words(rc,i) \
bytes2word(bval(rc[0],i),  bval(rc[1],i),   bval(rc[2],i),   bval(rc[3],i))

#define off(p,n) ((n+p+FUGUE_SIZE) % FUGUE_SIZE)

#define tix(p,j) \
  state[off(p,10)] ^= state[off(p,0)];  state[off(p,0)] = ((uint_32t *)in)[j];  \
  state[off(p,8)]  ^= state[off(p,0)];  state[off(p,1)] ^= state[off(p,FUGUE_FAR)]

#define column_mix(p)\
  state[off(p,0)] ^= state[off(p,4)];  state[off(p,1)] ^= state[off(p,5)];  state[off(p,2)] ^= state[off(p,6)];\
  state[off(p,15)] ^= state[off(p,4)];  state[off(p,16)] ^= state[off(p,5)];  state[off(p,17)] ^= state[off(p,6)]

#define print_rc_ASM
/*printf("In type 0\n");
 printf("col: %x %x %x %x \n", b0,b1,b2,b3);*/

#define print_rc_OPT
/*printf("In type 2\n");
printf("col: %x %x %x %x \n", column(0),column(1),column(2),column(3));\
printf("row: %x %x %x %x \n", row(0), row(1), row(2), row(3));*/

#define shift_edi0
#define shift_edi1 "shrl $8,%%edi\n\t"
#define shift_edi2 "shrl $16,%%edi\n\t"
#define shift_edi3 "shrl $24,%%edi\n\t"

#define asm_bval4(y,c,t)\
        asm ("movl %1, %0\n\t"\
             shift_edi##c \
             "andl $255, %%edi"\
             :"=D" (t)       \
             :"m" (y)   \
             :"memory"       \
             );

#define asm_set_template(c,t) \
        asm ("movl %0, %%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%esi"\
             );
#define asm_xor_template(c,t) \
        asm ("xorl %0, %%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%esi"\
             );

#define asm_xor_xor_template(reg,c,t) \
        asm ("xorl %%" reg ", %%esi\n\t"\
             "xorl %0, %%" reg "\n\t"\
             "xorl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_set_set_template(reg,c,t) \
        asm ("movl %0, %%" reg "\n\t"\
             "movl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_set_xor_template(reg,c,t) \
        asm ("movl %0, %%" reg "\n\t"\
             "xorl %%" reg ",%%esi\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );

#define asm_xor_set_template(reg,c,t) \
        asm ("movl %0, %%esi \n\t"\
             "xorl %%esi, %%" reg "\n\t"\
             :\
             :"m" (aes_style_table[c][t])\
             :"%" reg, "%esi"\
             );
#define put_back_to_b_ASM(i) \
asm (        "movl %%esi,%0\n\t"\
             :"=m" (b##i)\
             :\
             :"memory", "%esi"\
             );

#define put_back_to_b_OPT64(i)  print_rc_OPT64;
#define put_back_to_b_OPT(i)  print_rc_OPT;

/***if define (ASS_INLINE) *****/
#define table_index_ASM(p,i,k)  asm_bval4(state[off(p,i)],k,tmp)
#define asm_template0(xs_type,k,i,tmp)  xs_type("eax",k,tmp);
#define asm_template1(xs_type,k,i,tmp)  xs_type("ebx",k,tmp);
#define asm_template2(xs_type,k,i,tmp)  xs_type("ecx",k,tmp);
#define asm_template3(xs_type,k,i,tmp)  xs_type("edx",k,tmp);

#define xor_col_ASM(k,i) asm_xor_template(k,tmp);
#define set_col_ASM(k,i) asm_set_template(k,tmp);

#define xor_row_xor_col_ASM(k,i) asm_template##k(asm_xor_xor_template, k,i,tmp);
#define xor_row_set_col_ASM(k,i) asm_template##k(asm_xor_set_template, k,i,tmp);
#define set_row_xor_col_ASM(k,i) asm_template##k(asm_set_xor_template, k,i,tmp);
#define set_row_set_col_ASM(k,i) asm_template##k(asm_set_set_template, k,i,tmp);
/********ELSE*********/
#define table_index_OPT(p,i,k) tmp = aes_style_table[k][bval(state[off(p,i)],k)]
#define xor_col_OPT(k,i) column(i) ^= tmp;
#define set_col_OPT(k,i) column(i) = tmp;
#define xor_row_xor_col_OPT(k,i) row(k) ^= tmp; column(i) ^= tmp;
#define xor_row_set_col_OPT(k,i) row(k) ^= tmp; column(i) = tmp;
#define set_row_xor_col_OPT(k,i) row(k) = tmp;  column(i) ^= tmp;
#define set_row_set_col_OPT(k,i) row(k) = tmp;  column(i) = tmp;
/*******ENDIF *******/


#define xor_short_form_i_eq_k(p,i,k,type)       \
table_index##type(p,i,k); xor_col##type(k,i)

#define xor_short_form_i_eq_k_col_first(p,i,k,type)     \
table_index##type(p,i,k);  set_col##type(k,i)

#define xor_short_form_i_neq_k(p,i,k,type)  \
table_index##type(p,i,k);   xor_row_xor_col##type(k,i)

#define xor_short_form_i_neq_k_col_first(p,i,k,type)  \
table_index##type(p,i,k); xor_row_set_col##type(k,i)

#define xor_short_form_i_neq_k_row_first(p,i,k,type)  \
table_index##type(p,i,k);  set_row_xor_col##type(k,i)

#define xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)  \
table_index##type(p,i,k);   set_row_set_col##type(k,i)

#define xor_rc_3_0(p,i,k,type)  xor_short_form_i_neq_k_row_and_col_first(p,i,k,type)
#define xor_rc_3_1(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_3_2(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)
#define xor_rc_3_3(p,i,k,type)  xor_short_form_i_eq_k(p,i,k,type)

//Note col 3 is being done first

#define xor_rc_0_0(p,i,k,type)  xor_short_form_i_eq_k_col_first(p,i,k,type)
#define xor_rc_0_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_0_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_0_3(p,i,k,type)  xor_short_form_i_neq_k_row_first(p,i,k,type)

#define xor_rc_1_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_1_1(p,i,k,type)  xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_1_2(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_1_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)

#define xor_rc_2_0(p,i,k,type)  xor_short_form_i_neq_k_col_first(p,i,k,type)
#define xor_rc_2_1(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)
#define xor_rc_2_2(p,i,k,type)  xor_short_form_i_eq_k(p,i,k,type)
#define xor_rc_2_3(p,i,k,type)  xor_short_form_i_neq_k(p,i,k,type)


#define xor_rc_outer(p,i,type)\
   xor_rc_##i##_0(p,i,0,type);\
   xor_rc_##i##_1(p,i,1,type); xor_rc_##i##_2(p,i,2,type); xor_rc_##i##_3(p,i,3,type)

#define xor_row_and_col(p,type) \
     xor_rc_outer(p,3,type); put_back_to_b##type(3) \
     xor_rc_outer(p,0,type); put_back_to_b##type(0)\
     xor_rc_outer(p,1,type); put_back_to_b##type(1)\
     xor_rc_outer(p,2,type); put_back_to_b##type(2) 





/******if defined (ASS_INLINE) *******/

#define init_row_and_col_ASM

#define load_eax(mask) "movl %%eax,%%esi\n\t"\
                 "xorl %1,%%esi\n\t"\
                 "andl $" mask ",%%esi\n\t"\
                 "movl %%esi, %%edi\n\t"
#define xor_reg(reg,c,mask) \
             "movl %%" reg ",%%esi\n\t"\
             "xorl %" c ",%%esi\n\t"\
             "andl $" mask ",%%esi\n\t"\
             "xorl %%esi, %%edi\n\t"

#define rotate_regs\
 asm("ror $8,%%eax\n\t"\
     "ror $8,%%ebx\n\t"\
     "ror $8,%%ecx\n\t"\
     "ror $8,%%edx\n\t"\
     :\
     :\
     :"%eax","%ebx","%ecx","%edx")

/* can improve this write_back_to_state listed even when p, i are variables
   and not constants. Write now, if these are variables, there is no index register
   to put them into. So, if they are variables. They can be computed, before hand
   into a temporary variable, and then loaded into %esi, and then
   the final movl edi, %0 can be movl edi, %0(%esi).
*/

#define write_back_to_state_listed(p,i,a,b,c,ddd) \
 asm(load_eax("255") \
     xor_reg("ebx","2","65280") \
     xor_reg("ecx","3","16711680")  \
     xor_reg("edx","4","4278190080")\
     "movl %%edi,%0"\
     :"=m" (state[off(p,i)])\
     :"m" (a), "m" (b), "m" (c), "m" (ddd)\
     :"%esi","%edi","%eax","%ebx","%ecx","%edx");

#define write_back_to_state_listed0(p,i) \
 write_back_to_state_listed(p,i, b0,b1,b2,b3); rotate_regs

#define write_back_to_state_listed1(p,i) \
 write_back_to_state_listed(p,i, b1,b2,b3,b0); rotate_regs

#define write_back_to_state_listed2(p,i) \
 write_back_to_state_listed(p,i, b2,b3,b0,b1); rotate_regs

#define write_back_to_state_listed3(p,i) \
 write_back_to_state_listed(p,i, b3,b0,b1,b2)

#define write_back_to_state_indexed_ASM(p,i) \
    write_back_to_state_listed##i(p,i)

/*************ELSE**********/

#define rotate_regs_OPT(n)	\
row(0)=brot(row(0),n);	   	\
row(1)=brot(row(1),n);\
row(2)=brot(row(2),n);\
row(3)=brot(row(3),n);

#define write_back_to_state_listed_OPT(p,i,a,b,c,d) \
  state[off(p,i)]= ((row(0)^column(a)) & 0xff) ^   \
                   ((row(1)^column(b)) & 0xff00) ^ \
                   ((row(2)^column(c)) & 0xff0000) ^ \
                   ((row(3)^column(d)) & 0xff000000)


#define write_back_to_state_listed_OPT_0(p,i) \
  write_back_to_state_listed_OPT(p,i, 0,1,2,3); /* rotate_regs_OPT(24) */

// this is a property of the matrix M, basically the first two entries
// are same

#define write_back_to_state_listed_OPT_1(p,i) \
  write_back_to_state_listed_OPT(p,i, 1,2,3,0); rotate_regs_OPT(16)

#define write_back_to_state_listed_OPT_2(p,i) \
  write_back_to_state_listed_OPT(p,i, 2,3,0,1); rotate_regs_OPT(24)

#define write_back_to_state_listed_OPT_3(p,i) \
 write_back_to_state_listed_OPT(p,i, 3,0,1,2)

#define write_back_to_state_indexed_OPT(p,i) \
    write_back_to_state_listed_OPT_##i(p,i)

#define init_row_and_col_OPT    

//column(0)=column(1)=column(2)=column(3)=0; row(0)= row(1)= row(2)=row(3) =0;

#define xor_col_to_rows_ASM print_rc_ASM;
#define xor_col_to_rows_OPT print_rc_OPT;

/**************END************/

#define write_back_to_state(p,type) \
write_back_to_state_indexed##type(p,0);write_back_to_state_indexed##type(p,1);\
write_back_to_state_indexed##type(p,2);write_back_to_state_indexed##type(p,3);

#define super_mix(ptr,type)\
 init_row_and_col##type;  \
 xor_row_and_col(ptr,type); \
 xor_col_to_rows##type; \
 write_back_to_state(ptr,type);


#define sub_round(ptr,type) \
             column_mix(ptr);   super_mix(ptr,type)

#define super_mix_convert(p,type) super_mix(p,type)

#define ror(state,n) ptr = ((ptr-n+FUGUE_SIZE) % FUGUE_SIZE)

#define cross_mix(p,cross) \
       state[off(p,4)] ^=state[off(p,0)]; \
       state[off(p,cross)] ^=state[off(p,0)]

#define final_sub_round(p,cross,n) \
        cross_mix(p,cross);\
       ror(state,n); super_mix_convert(p,EXTRA_ROUND_IMPL_TYPE)

#define final_round2(ptr,num,c)\
    for (j=0; j<c; j++)      \
         { final_sub_round(ptr, 15,15); \
           final_sub_round(ptr, 16,14);} \
           cross_mix(ptr,15);

#define final_round1(ptr,num,c) \
    for (j=0; j<c; j++)  \
      {sub_round(((-3)*(2*((num+j)%5)+1)),EXTRA_ROUND_IMPL_TYPE); \
       sub_round(((-3)*(2*((num+j)%5)+2)),EXTRA_ROUND_IMPL_TYPE);}

#define empty_pretty_print(s,a,b,c)

#define round(j,num,type) \
         tix(((-3)*2*num),j); \
          empty_pretty_print(state, ptr, 30, "after tix");\
           sub_round(((-3)*(2*num+1)),type); \
            empty_update(ptr); empty_pretty_print(state, ptr, 30, "after subround 1");\
             sub_round(((-3)*(2*num+2)),type);\
              empty_update(ptr);empty_pretty_print(state, ptr, 30, "after subround 2");


void pretty_print_256 (uint_32t * state, int ptr, int n, char * st)
{
    int i,j;
    uint_32t tmp;

    printf("%s\n", st);
    for (i=0; i<n; i++)
    {
        tmp = state[off(ptr,i)];
        for (j=0; j<4; j++)
        {
            printf("%02x", (uint_8t)tmp);
            tmp = tmp>>8;
        }
        printf(" ");
    }
    printf("\n\n");
}

void print_byte_stream_256 ( char * str, uint_32t * output, int len)
{
    int i,j;

    printf("%s\n",str);
    for (i=0; i<len; i++)
    {
        for (j=0; j<4; j++)
        printf("%02x", ((char*) output)[4*i+j] & 0xff);
        printf(" ");
    }
    printf("\n\n");
}

#define empty_update(ptr)

#define update(ptr) \
ptr = (ptr-3 + FUGUE_SIZE) %FUGUE_SIZE;

int fugue_update_256 (fugueHashState *hs, const char *in, uint_64t len)
{
    uint_32t state[30];
#ifdef CONFIG_ASM_INLINE
    uint_32t b0,b1,b2,b3;
#else
    uint_32t row[4];
    uint_32t col[4];
#endif
    uint_32t tmp;
    short len_mod;
    uint_64t len_div;
    uint_64t j,j1,j2;
    short ptr = hs->Base;

    j1 = (5-hs->Base/6)%5;  // this is the left over sliding window pointer (divided by 2*3)

    for (j=0; j < 30; j++) state[j] = hs->State[j].d;

    j=0;

    if (j1 && len)
    {
	if  (j1 <=1)         { round(j,1,IMPL_TYPE); j++; len--; ptr -=6; }
	if ((j1 <=2) && len) { round(j,2,IMPL_TYPE); j++; len--; ptr -=6; }
        if ((j1 <=3) && len) { round(j,3,IMPL_TYPE); j++; len--; ptr -=6; }
        if ((j1 <=4) && len) { round(j,4,IMPL_TYPE); j++; len--; ptr -=6; }
    }

    len_mod = (len)%5;
    len_div = (len)/5;

    for (j2=0; j2<len_div;j2++)
    {
        round(j,0,IMPL_TYPE); j++;
        round(j,1,IMPL_TYPE); j++;
        round(j,2,IMPL_TYPE); j++;
        round(j,3,IMPL_TYPE); j++;
        round(j,4,IMPL_TYPE); j++;
    }

    if (len_mod >=1)
    {
        round(j,0,IMPL_TYPE); j++;
        if (len_mod >=2)
        {
            round(j,1,IMPL_TYPE); j++;
            if (len_mod >=3)
            {
                round(j,2,IMPL_TYPE); j++;
                if (len_mod >=4)
                {
                     round(j,3,IMPL_TYPE); j++;
                }
            }
        }
    }

    for (j=0; j < 30; j++) hs->State[j].d = state[j];

    hs->Base = (ptr-6*len_mod + 2*FUGUE_SIZE) % FUGUE_SIZE;

    return 0;
}

int fugue_final_256 (fugueHashState *hs, char *out)
{
    uint_32t *state = (uint_32t *)hs->State;
    uint_32t row[4];
    uint_32t col[4];
    uint_32t tmp;
    uint_64t j,j1 = (5-hs->Base/6)%5;
    short ptr = hs->Base;

    final_round1(ptr,j1,5);
    final_round2(ptr,j1,13);

    for (j=0; j<4; j++)
    {
        ((uint_32t *)out)[j]   = state[off(ptr,1+j)];
        ((uint_32t *)out)[4+j] = state[off(ptr,15+j)];
    }

    return 0;
}
