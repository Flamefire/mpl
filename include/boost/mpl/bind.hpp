//-----------------------------------------------------------------------------
// boost mpl/bind.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2001-02
// Peter Dimov, Aleksey Gurtovoy
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee, 
// provided that the above copyright notice appears in all copies and 
// that both the copyright notice and this permission notice appear in 
// supporting documentation. No representations are made about the 
// suitability of this software for any purpose. It is provided "as is" 
// without express or implied warranty.

#if !defined(BOOST_PP_IS_ITERATING)

///// header body

#ifndef BOOST_MPL_BIND_HPP_INCLUDED
#define BOOST_MPL_BIND_HPP_INCLUDED

#include "boost/mpl/aux_/apply.hpp"
#include "boost/mpl/aux_/config/bind.hpp"
#include "boost/mpl/aux_/config/lambda_support.hpp"

#if !defined(BOOST_MPL_PREPROCESSING_MODE)
#   include "boost/mpl/placeholder.hpp"
#   include "boost/mpl/void.hpp"
#   include "boost/mpl/protect.hpp"
#   include "boost/mpl/limits/arity.hpp"
#   include "boost/mpl/aux_/arity.hpp"
#   include "boost/mpl/aux_/type_wrapper.hpp"
#   include "boost/mpl/aux_/yes_no.hpp"
#   include "boost/type_traits/same_traits.hpp"
#endif

#include "boost/mpl/aux_/config/use_preprocessed.hpp"

#if defined(BOOST_MPL_USE_PREPROCESSED_HEADERS) && \
    !defined(BOOST_MPL_PREPROCESSING_MODE)

#   if defined(BOOST_MPL_NO_UNNAMED_PLACEHOLDER_SUPPORT)
#       define BOOST_MPL_PREPROCESSED_HEADER basic_bind.hpp
#   else
#       define BOOST_MPL_PREPROCESSED_HEADER bind.hpp
#   endif
#   include "boost/mpl/aux_/include_preprocessed.hpp"

#else

#   include "boost/mpl/aux_/preprocessor/params.hpp"
#   include "boost/mpl/aux_/preprocessor/default_params.hpp"
#   include "boost/mpl/aux_/preprocessor/def_params_tail.hpp"
#   include "boost/mpl/aux_/preprocessor/enum.hpp"
#   include "boost/mpl/aux_/preprocessor/add.hpp"
#   include "boost/mpl/aux_/preprocessor/sub.hpp"
#   include "boost/mpl/aux_/config/dtp.hpp"

#   include "boost/preprocessor/iterate.hpp"
#   include "boost/preprocessor/comma_if.hpp"
#   include "boost/preprocessor/cat.hpp"
#   include "boost/preprocessor/inc.hpp"
#   include "boost/config.hpp"

namespace boost {
namespace mpl {

// local macros, #undef-ined at the end of the header
#   define AUX_APPLY(args) \
    BOOST_MPL_AUX_APPLY(BOOST_MPL_METAFUNCTION_MAX_ARITY, args) \
    /**/

#   define AUX_BIND_PARAMS(param) \
    BOOST_MPL_PP_PARAMS( \
          BOOST_MPL_METAFUNCTION_MAX_ARITY \
        , param \
        ) \
    /**/

#   define AUX_BIND_DEFAULT_PARAMS(param, value) \
    BOOST_MPL_PP_DEFAULT_PARAMS( \
          BOOST_MPL_METAFUNCTION_MAX_ARITY \
        , param \
        , value \
        ) \
    /**/

#   define AUX_BIND_N_PARAMS(n, param) \
    BOOST_PP_COMMA_IF(n) \
    BOOST_MPL_PP_PARAMS(n, param) \
    /**/

#   define AUX_BIND_N_SPEC_PARAMS(n, param, def) \
    BOOST_PP_COMMA_IF(n) BOOST_MPL_PP_PARAMS(n, param) \
    BOOST_PP_COMMA_IF(BOOST_MPL_PP_SUB(BOOST_MPL_METAFUNCTION_MAX_ARITY,n)) \
    BOOST_MPL_PP_ENUM( \
          BOOST_MPL_PP_SUB(BOOST_MPL_METAFUNCTION_MAX_ARITY,n) \
        , def \
        ) \
    /**/

namespace aux {

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template<
      typename T, AUX_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg
{
    typedef T type;
};

#   if !defined(BOOST_MPL_NO_UNNAMED_PLACEHOLDER_SUPPORT)

template<
      typename T
    , typename Arg
    >
struct replace_unnamed_arg
{
    typedef Arg next_arg;
    typedef T type;
};

template<
      typename Arg
    >
struct replace_unnamed_arg< arg<-1>,Arg >
{
    typedef typename Arg::next next_arg;
    typedef Arg type;
};

#   endif // BOOST_MPL_NO_UNNAMED_PLACEHOLDER_SUPPORT

#else

// agurt, 15/jan/02: it's not a intended to be used as a function class, and 
// MSVC6.5 has problems with 'apply' name here (the code compiles, but doesn't
// work), so i went with the 'result_' here, and in all other similar cases
template< bool >
struct resolve_arg_impl
{
    template< typename T, AUX_BIND_PARAMS(typename U) > struct result_
    {
        typedef T type;
    };
};

template<> 
struct resolve_arg_impl<true>
{
    template< typename T, AUX_BIND_PARAMS(typename U) > struct result_
    {
        typedef typename AUX_APPLY((
              T
            , AUX_BIND_PARAMS(U)
            ))::type type;
    };
};

// for 'resolve_bind_arg'
template< typename T > struct is_bind_template;

template< 
      typename T, AUX_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg
    : resolve_arg_impl< is_bind_template<T>::value >
            ::template result_< T,AUX_BIND_PARAMS(U) >
{
};

#   if !defined(BOOST_MPL_NO_UNNAMED_PLACEHOLDER_SUPPORT)

template< typename T > 
struct replace_unnamed_arg_impl
{
    template< typename Arg > struct result_
    {
        typedef Arg next_arg;
        typedef T type;
    };
};

template<> 
struct replace_unnamed_arg_impl< arg<-1> >
{
    template< typename Arg > struct result_
    {
        typedef typename Arg::next next_arg;
        typedef Arg type;
    };
};

template< typename T, typename Arg > 
struct replace_unnamed_arg
    : replace_unnamed_arg_impl<T>::template result_<Arg>
{
};

#   endif // BOOST_MPL_NO_UNNAMED_PLACEHOLDER_SUPPORT

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

} // namespace aux

// forward declaration
template<
      typename F, AUX_BIND_DEFAULT_PARAMS(typename T, void_)
    >
struct bind;

// fwd, for 'resolve_bind_arg'/'is_bind_template' specializations
template< typename F, typename T > struct bind1st;
template< typename F, typename T > struct bind2nd;

namespace aux {

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
template<
      int N, AUX_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg< arg<N>,AUX_BIND_PARAMS(U) >
{
    typedef typename AUX_APPLY((arg<N>, AUX_BIND_PARAMS(U)))::type type;
};

template<
      typename F, AUX_BIND_PARAMS(typename T), AUX_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg< bind<F,AUX_BIND_PARAMS(T)>,AUX_BIND_PARAMS(U) >
{
    typedef bind<F,AUX_BIND_PARAMS(T)> f_;
    typedef typename AUX_APPLY((f_, AUX_BIND_PARAMS(U)))::type type;
};

template<
      typename F, typename T, AUX_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg< bind1st<F,T>, AUX_BIND_PARAMS(U) >
{
    typedef bind1st<F,T> f_;
    typedef typename AUX_APPLY((f_, AUX_BIND_PARAMS(U)))::type type;
};

template<
      typename F, typename T, AUX_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg< bind2nd<F,T>, AUX_BIND_PARAMS(U) >
{
    typedef bind2nd<F,T> f_;
    typedef typename AUX_APPLY((f_, AUX_BIND_PARAMS(U)))::type type;
};

#else
// agurt, 10/mar/02: the forward declaration has to appear before any of
// 'is_bind_helper' overloads, otherwise MSVC6.5 issues an ICE on it
template< int > struct bind_impl_chooser;

aux::no_tag is_bind_helper(...);
template< typename T > aux::no_tag is_bind_helper(protect<T>*);

// overload for "main" form
// agurt, 15/mar/02: MSVC 6.5 fails to properly resolve the overload 
// in case if we use 'aux::type_wrapper< bind<...> >' here, and all 
// 'bind' instantiations form a complete type anyway
template<
      typename F, AUX_BIND_PARAMS(typename T)
    >
aux::yes_tag is_bind_helper(bind<F,AUX_BIND_PARAMS(T)>*);

template< int N >
aux::yes_tag is_bind_helper(arg<N>*);

template< typename F, typename T > aux::yes_tag is_bind_helper(bind1st<F,T>*);
template< typename F, typename T > aux::yes_tag is_bind_helper(bind2nd<F,T>*);

template< typename T > struct is_bind_template
{
    BOOST_STATIC_CONSTANT(bool, value =
        sizeof(aux::is_bind_helper(static_cast<T*>(0)))
            == sizeof(aux::yes_tag)
        );
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#if defined(BOOST_NO_DEFAULT_TEMPLATE_PARAMETERS_IN_NESTED_TEMPLATES)
// MWCW/Borland workaround

template<
      typename F, AUX_BIND_PARAMS(typename T), int N
    >
struct arity< bind<F,AUX_BIND_PARAMS(T)>,N >
{
    BOOST_STATIC_CONSTANT(int
        , value = BOOST_MPL_METAFUNCTION_MAX_ARITY
        );
};

template< typename F, typename T, int N >
struct arity< bind1st<F,T>,N >
{
    BOOST_STATIC_CONSTANT(int
        , value = BOOST_MPL_METAFUNCTION_MAX_ARITY
        );
};

template< typename F, typename T, int N >
struct arity< bind2nd<F,T>,N >
{
    BOOST_STATIC_CONSTANT(int
        , value = BOOST_MPL_METAFUNCTION_MAX_ARITY
        );
};

#endif // BOOST_NO_DEFAULT_TEMPLATE_PARAMETERS_IN_NESTED_TEMPLATES

} // namespace aux

#define BOOST_PP_ITERATION_PARAMS_1 \
    (3,(0, BOOST_MPL_METAFUNCTION_MAX_ARITY, "boost/mpl/bind.hpp"))
#include BOOST_PP_ITERATE()

// real C++ version is already taken care of
#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION) \
   && !defined(BOOST_MPL_NO_BIND_TEMPLATE)

namespace aux {
// apply_count_args
#define BOOST_MPL_AUX_COUNT_ARGS_PREFIX bind
#define BOOST_MPL_AUX_COUNT_ARGS_DEFAULT void_
#define BOOST_MPL_AUX_COUNT_ARGS_ARITY BOOST_MPL_METAFUNCTION_MAX_ARITY
#include "boost/mpl/aux_/count_args.hpp"
}

// bind
template<
      typename F, AUX_BIND_PARAMS(typename T)
    >
struct bind
    : aux::bind_impl_chooser<
          aux::bind_count_args<AUX_BIND_PARAMS(T)>::value
        >::template result_< F,AUX_BIND_PARAMS(T) >::type
{
};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION
       // && !defined(BOOST_MPL_NO_BIND_TEMPLATE)

// bind1st/bind2nd, lightweight, for simple cases/backward compatibility
template< typename F, typename T >
struct bind1st
{
    template<
          typename U BOOST_MPL_PP_DEF_PARAMS_TAIL(1, typename U)
        >
    struct apply
        : BOOST_MPL_AUX_APPLY2(F,T,U)
    {
    };
};

template< typename F, typename T >
struct bind2nd
{
    template<
          typename U BOOST_MPL_PP_DEF_PARAMS_TAIL(1, typename U)
        >
    struct apply
        : BOOST_MPL_AUX_APPLY2(F,U,T)
    {
    };
};

#   undef AUX_BIND_N_SPEC_PARAMS
#   undef AUX_BIND_N_PARAMS
#   undef AUX_BIND_DEFAULT_PARAMS
#   undef AUX_BIND_PARAMS
#   undef AUX_APPLY

} // namespace mpl
} // namespace boost

#endif // BOOST_MPL_USE_PREPROCESSED_HEADERS
#endif // BOOST_MPL_BIND_HPP_INCLUDED

///// iteration, depth == 1

#elif BOOST_PP_ITERATION_DEPTH() == 1

#   define i BOOST_PP_FRAME_ITERATION(1)

template<
      typename F AUX_BIND_N_PARAMS(i, typename T)
    >
struct BOOST_PP_CAT(bind,i)
{
    template<
          AUX_BIND_DEFAULT_PARAMS(typename U, void_)
        >
    struct apply
    {
     private:
#   if !defined(BOOST_MPL_NO_UNNAMED_PLACEHOLDER_SUPPORT)

        typedef aux::replace_unnamed_arg< F,arg<1> > r0;
        typedef typename r0::type a0;
        typedef typename r0::next_arg n1;
        typedef typename aux::resolve_bind_arg<a0,AUX_BIND_PARAMS(U)>::type f_;
        //:
#   else
        typedef typename aux::resolve_bind_arg<F,AUX_BIND_PARAMS(U)>::type f_;

#   endif // BOOST_MPL_NO_UNNAMED_PLACEHOLDER_SUPPORT

#       define BOOST_PP_ITERATION_PARAMS_2 (3,(1, i, "boost/mpl/bind.hpp"))
#       include BOOST_PP_ITERATE()

     public:
        typedef typename BOOST_MPL_AUX_APPLY(
              i
            , (f_ AUX_BIND_N_PARAMS(i,t))
            )::type type;
    };
};


namespace aux {

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template<
      typename F AUX_BIND_N_PARAMS(i, typename T), AUX_BIND_PARAMS(typename U)
    >
struct resolve_bind_arg<
      BOOST_PP_CAT(bind,i)<F AUX_BIND_N_PARAMS(i,T)>,AUX_BIND_PARAMS(U)
    >
{
    typedef BOOST_PP_CAT(bind,i)<F AUX_BIND_N_PARAMS(i,T)> f_;
    typedef typename AUX_APPLY((f_, AUX_BIND_PARAMS(U)))::type type;
};

#else

template<
      typename F AUX_BIND_N_PARAMS(i, typename T)
    >
aux::yes_tag
is_bind_helper(BOOST_PP_CAT(bind,i)<F AUX_BIND_N_PARAMS(i,T)>*);

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION


// MWCW/Borland workaround
#if defined(BOOST_NO_DEFAULT_TEMPLATE_PARAMETERS_IN_NESTED_TEMPLATES)
template<
      typename F AUX_BIND_N_PARAMS(i, typename T), int N
    >
struct arity<
      BOOST_PP_CAT(bind,i)<F AUX_BIND_N_PARAMS(i,T)>, N
    >
{
    BOOST_STATIC_CONSTANT(int
        , value = BOOST_MPL_METAFUNCTION_MAX_ARITY
        );
};
#endif

} // namespace aux


#   if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

#if i == BOOST_MPL_METAFUNCTION_MAX_ARITY

//: primary template (not a specialization!)
template<
      typename F AUX_BIND_N_PARAMS(i, typename T)
    >
struct bind
    : BOOST_PP_CAT(bind,i)<F AUX_BIND_N_PARAMS(i,T) >
{
};

#else

template<
      typename F AUX_BIND_N_PARAMS(i, typename T)
    >
struct bind< F AUX_BIND_N_SPEC_PARAMS(i, T, void_) >
    : BOOST_PP_CAT(bind,i)<F AUX_BIND_N_PARAMS(i,T) >
{
};

#endif // i == BOOST_MPL_METAFUNCTION_MAX_ARITY

#   else

namespace aux {

template<>
struct bind_impl_chooser<i>
{
    template<
          typename F, AUX_BIND_PARAMS(typename T)
        >
    struct result_
    {
        typedef BOOST_PP_CAT(bind,i)< F AUX_BIND_N_PARAMS(i,T) > type;
    };
};

} // namespace aux

#   endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

#   undef i

///// iteration, depth == 2

#elif BOOST_PP_ITERATION_DEPTH() == 2

#   define j BOOST_PP_FRAME_ITERATION(2)
#   if !defined(BOOST_MPL_NO_UNNAMED_PLACEHOLDER_SUPPORT)

        typedef aux::replace_unnamed_arg< BOOST_PP_CAT(T,j),BOOST_PP_CAT(n,j) > BOOST_PP_CAT(r,j);
        typedef typename BOOST_PP_CAT(r,j)::type BOOST_PP_CAT(a,j);
        typedef typename BOOST_PP_CAT(r,j)::next_arg BOOST_PP_CAT(n,BOOST_PP_INC(j));
        typedef typename aux::resolve_bind_arg<BOOST_PP_CAT(a,j), AUX_BIND_PARAMS(U)>::type BOOST_PP_CAT(t,j);
        //:
#   else
        typedef typename aux::resolve_bind_arg< BOOST_PP_CAT(T,j),AUX_BIND_PARAMS(U)>::type BOOST_PP_CAT(t,j);

#   endif
#   undef j

#endif // BOOST_PP_IS_ITERATING