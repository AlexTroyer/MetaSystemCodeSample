/*****************************************************************************
File:   Property.hpp
Author: Wesley Pesetti
  A bunch of macros to help make other macros.
  Thank you to Wesley Pesetti for the macros.
*****************************************************************************/
#pragma once

//Needed to expand __VA_ARGS__ in Visual Studio
#define EXPAND(x) x

//Gets the 17th number passed in
#define PP_ARG_N( \
          _1,  _2,  _3,  _4,  _5,  _6,  _7,  _8,  _9, \
          _10, _11, _12, _13, _14, _15, _16,  N, ...) N

//Sequence of numbers to pass in to PP_ARG_N
#define PP_RSEQ_N()\
          16, 15, 14, 13, 12, 11, 10, 9,  \
          8,  7,  6,  5,  4,  3,  2,  1,  0

#define PP_NARG_(...)    EXPAND(PP_ARG_N(__VA_ARGS__))

//
#define PP_COMMASEQ_N()\
          1, 1, 1, 1, 1, 1, 1, 1, 1, \
          1, 1, 1, 1, 1, 1, 0, 0


#define PP_COMMA()    ,

//Determines if VA_ARGS has a comma 0 if no comma, 1 otherwise
#define PP_HASCOMMA(...)                                   \
          EXPAND(PP_NARG_(__VA_ARGS__, PP_COMMASEQ_N()))

//Gets the number of arguments from __VA_ARGS__
#define PP_NARG(...)                                       \
          EXPAND(PP_NARG_HELPER1(                                 \
              PP_HASCOMMA(__VA_ARGS__),                    \
              PP_HASCOMMA(PP_COMMA __VA_ARGS__ ()),        \
              PP_NARG_(__VA_ARGS__, PP_RSEQ_N())))

//These macros correctly return 0 and 1 when __VA_ARGS__ is small
#define PP_NARG_HELPER1(a, b, N)    PP_NARG_HELPER2(a, b, N)
#define PP_NARG_HELPER2(a, b, N)    PP_NARG_HELPER3_ ## a ## b(N)
#define PP_NARG_HELPER3_01(N)    0
#define PP_NARG_HELPER3_00(N)    1
#define PP_NARG_HELPER3_11(N)    N


//Gets the Nth argument from __VA_ARGS__
#define PARAM_ARG(num, ...)                            \
    EXPAND(                                            \
    PP_ARG_##num(__VA_ARGS__, 16, 15, 14, 13, 12,      \
                 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0) \
    )

//Counts the number of argument in __VA_ARGS__
#define COUNT_ARGUMENTS(...) \
    EXPAND(PP_NARG(__VA_ARGS__))


//Starts the for loop with macro X, repeating num times
#define SPP_REPEAT(num, X,...) SPP_REPEAT_ ## num (SPP_UNPAR(X),__VA_ARGS__)
#define SPP_UNPAR(...) __VA_ARGS__

// Macros to be used as a "for" loop
// X is a macro to be called each iteration
// For this implementation, it takes the current iteration, and the type of the parameter.
// PARAM_ARG takes a number and an __VA_ARGS__ and returns the type associated with __VA_ARGS__[number]
// Max iterations is 16
#define  SPP_REPEAT_0(X, Y, ...) 
#define  SPP_REPEAT_1(X, Y, ...) X(Y,PARAM_ARG(0,__VA_ARGS__))
#define  SPP_REPEAT_2(X, Y, ...) SPP_REPEAT_1(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(1,__VA_ARGS__)) 
#define  SPP_REPEAT_3(X, Y, ...) SPP_REPEAT_2(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(2,__VA_ARGS__)) 
#define  SPP_REPEAT_4(X, Y, ...) SPP_REPEAT_3(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(3,__VA_ARGS__)) 
#define  SPP_REPEAT_5(X, Y, ...) SPP_REPEAT_4(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(4,__VA_ARGS__)) 
#define  SPP_REPEAT_6(X, Y, ...) SPP_REPEAT_5(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(5,__VA_ARGS__)) 
#define  SPP_REPEAT_7(X, Y, ...) SPP_REPEAT_6(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(6,__VA_ARGS__)) 
#define  SPP_REPEAT_8(X, Y, ...) SPP_REPEAT_7(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(7,__VA_ARGS__)) 
#define  SPP_REPEAT_9(X, Y, ...) SPP_REPEAT_8(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(8,__VA_ARGS__)) 
#define SPP_REPEAT_10(X, Y, ...) SPP_REPEAT_9(X,Y,__VA_ARGS__)  X(Y,PARAM_ARG(9,__VA_ARGS__))
#define SPP_REPEAT_11(X, Y, ...) SPP_REPEAT_10(X,Y,__VA_ARGS__) X(Y,PARAM_ARG(10,__VA_ARGS__)) 
#define SPP_REPEAT_12(X, Y, ...) SPP_REPEAT_11(X,Y,__VA_ARGS__) X(Y,PARAM_ARG(11,__VA_ARGS__)) 
#define SPP_REPEAT_13(X, Y, ...) SPP_REPEAT_12(X,Y,__VA_ARGS__) X(Y,PARAM_ARG(12,__VA_ARGS__)) 
#define SPP_REPEAT_14(X, Y, ...) SPP_REPEAT_13(X,Y,__VA_ARGS__) X(Y,PARAM_ARG(13,__VA_ARGS__)) 
#define SPP_REPEAT_15(X, Y, ...) SPP_REPEAT_14(X,Y,__VA_ARGS__) X(Y,PARAM_ARG(14,__VA_ARGS__)) 
#define SPP_REPEAT_16(X, Y, ...) SPP_REPEAT_15(X,Y,__VA_ARGS__) X(Y,PARAM_ARG(15,__VA_ARGS__)) 
#define SPP_REPEAT_17(X, Y, ...) SPP_REPEAT_16(X,Y,__VA_ARGS__) X(Y,PARAM_ARG(16,__VA_ARGS__)) 

//Internal Implementation of PARAM_ARG (used to get the Nth param from __VA_ARGS__)
#define  PP_ARG_0(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _0
#define  PP_ARG_1(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _1
#define  PP_ARG_2(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _2
#define  PP_ARG_3(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _3
#define  PP_ARG_4(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _4
#define  PP_ARG_5(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _5
#define  PP_ARG_6(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _6
#define  PP_ARG_7(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _7
#define  PP_ARG_8(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _8
#define  PP_ARG_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _9
#define PP_ARG_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _10
#define PP_ARG_11(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _11
#define PP_ARG_12(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _12
#define PP_ARG_13(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _13
#define PP_ARG_14(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _14
#define PP_ARG_15(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _15
#define PP_ARG_16(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16, ...) _16

//Used to paste the result of two macros together without errors
#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

//Used to Excecute the macro for each argument in __VA_ARGS__
//Signature of the macro must be #define <MacroName>(Number,CurrentType)
#define PASTE_PARAMS(Macro, other, ...) TOKENPASTE2(SPP_REPEAT_,COUNT_ARGUMENTS(__VA_ARGS__)) (SPP_UNPAR(Macro),SPP_UNPAR(other),__VA_ARGS__)

//Helpers for FirstParamOrVoid
#define FirstParamOrVoid_0(...) void
#define FirstParamOrVoid_1(...) EXPAND(PARAM_ARG(0,__VA_ARGS__))

//More Helpers for FirstParamOrVoid, used to append the number of arguments on a macro to call it
#define FirstParamOrVoid_HELPER1(a, b, N)    FirstParamOrVoid_HELPER2(a, b, N)
#define FirstParamOrVoid_HELPER2(a, b, N)    FirstParamOrVoid_HELPER3_ ## a ## b(N)
#define FirstParamOrVoid_HELPER3_01(N)    0
#define FirstParamOrVoid_HELPER3_00(N)    1
#define FirstParamOrVoid_HELPER3_11(N)    1

//Used to return the first argument in __VA_ARGS__, if none exists, returns void
#define FirstParamOrVoid(...)\
  TOKENPASTE2(FirstParamOrVoid_,EXPAND(FirstParamOrVoid_HELPER1(                   \
                                      PP_HASCOMMA(__VA_ARGS__),                    \
                                      PP_HASCOMMA(PP_COMMA __VA_ARGS__ ()),        \
                                      PP_NARG_(__VA_ARGS__, PP_RSEQ_N()))))(__VA_ARGS__)

//Used to make a string out of the passed in parameter
#define STRINGIZE_(X) #X
#define STRINGIZE(X) STRINGIZE_(X)