#pragma once

// name convenction
// prefix __          : internal use
// prefix f           : mean s flag
// prefix s           : means safe                    ( variable passed by value, not reference )
// prefix ref         : variable passed by reference
// prefix no_ref      : variable passed by value
// prefix const       : constant variable required
// add                : add operation                 ( bitwise ((a) OR (b)) )
// rem                : remove operation              ( bitwise ((a) AND (NOT(b))) )
// chk                : expression                    ( logic (((a) AND (b)) EQUALS (b)) )


// operations

// Change the value of reference
#define __fadd(_value,_target) ((_target) = (_target)|(_value))
#define __frem(_value,_target) ((_target) = (_target)&(~(_value)))

// Keep reference intact
#define __fsadd(_value,_target) ((_target)|(_value))
#define __fsrem(_value,_target) ((_target)&(~(_value)))
#define __fchk(_value,_target) (((_target)&(_value))==(_value))

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// types

#ifndef __flagType
#define __flagType unsigned long
#endif

#define __ref_flag __flagType&
#define __const_flag const __flagType
#define flag __flagType

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// wrappers

#ifdef FLAGS_NO_TEMPLATES
#define fadd(const_value, ref_target) (__fadd((const_value),(ref_target)))
#define frem(const_value, ref_target) (__frem((const_value),(ref_target)))
#define fsadd(const_value,no_ref_target) (__fsadd((const_value),(no_ref_target)))
#define fsrem(const_value,no_ref_target) (__fsrem((const_value),(no_ref_target)))
#define fchk(const_value,no_ref_target) (__fchk((const_value),(no_ref_target)))
#endif

#ifndef FLAGS_NO_TEMPLATES

/**
 * \brief appends the desired value to the flag
 * \tparam const_value value to be appended 
 * \param ref_target   reference of the dest variable 
 * \return returns ref_target reference
 */
template<__const_flag const_value> __ref_flag fadd(__ref_flag ref_target) { return __fadd(const_value, ref_target); }
/**
 * \brief removes the desired value from the flag
 * \tparam const_value value to be removed
 * \param ref_target   reference of the dest variable
 * \return returns ref_target reference
 */
template<__const_flag const_value> __ref_flag frem(__ref_flag ref_target) { return __frem(const_value, ref_target); }
/**
 * \brief returns the operation as a new value 
 * \tparam const_value value to be inserted
 * \param no_ref_target   initial value of the flag
 * \return returns a new value
 */
template<__const_flag const_value> flag fsadd(flag no_ref_target) { return __fsadd(const_value, no_ref_target); }
/**
 * \brief returns the operation as a new value
 * \tparam const_value value to be removed
 * \param no_ref_target   initial value of the flag
 * \return returns a new value
 */
template<__const_flag const_value> flag fsrem(flag no_ref_target) { return __fsrem(const_value, no_ref_target); }
/**
 * \brief check if the flag exists in a particular value
 * \tparam const_value value to be checked
 * \param no_ref_target   actual value of the flag
 * \return returns true or false
 */
template<__const_flag const_value> bool fchk(flag no_ref_target) { return __fchk(const_value, no_ref_target); }

#endif

