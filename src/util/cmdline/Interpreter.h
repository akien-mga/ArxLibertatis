/*
 * Copyright 2013 Arx Libertatis Team (see the AUTHORS file)
 *
 * This file is part of Arx Libertatis.
 *
 * Original source is copyright 2010 - 2011. Alexey Tsoy.
 * http://sourceforge.net/projects/interpreter11/
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization
 * obtaining a copy of the software and accompanying documentation covered by
 * this license (the "Software") to use, reproduce, display, distribute,
 * execute, and transmit the Software, and to prepare derivative works of the
 * Software, and to permit third-parties to whom the Software is furnished to
 * do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer,
 * must be included in all copies of the Software, in whole or in part, and
 * all derivative works of the Software, unless such copies or derivative
 * works are solely in the form of machine-executable object code generated by
 * a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#ifndef ARX_UTIL_CMDLINE_INTERPRETER_H
#define ARX_UTIL_CMDLINE_INTERPRETER_H

#include "util/cmdline/detail/Interpreter.h"
#include "util/cmdline/Keys.h"
#include "util/cmdline/TypeCast.h"

#include <string>
#include <iomanip>

// default strategy for types conversion
struct type_cast;

/// A storage of option handlers and their descriptions.
/**
 * This provides convenient interface for adding new option (the add) method,
 * and executing option handlers by name.
 *
 * @param StringType type of string
 *
 * @param TypeCast type of class that is used to convertation StringType to
 *   expected type by option handler.
 */
template<typename StringType, typename TypeCast = type_cast>
class interpreter : detail::interpreter<key_type<StringType>, TypeCast> {
	typedef detail::interpreter<key_type<StringType>, TypeCast> super_t;
	
public:
	typedef typename super_t::type_cast_t   type_cast_t;
	typedef typename super_t::string_type   string_type;
	typedef typename super_t::op_name_t     op_name_t;
	
	/// Registers options.
	/**
	 *  This function registrates command options.
	 *  The option parameters count and their types will be obtained
	 *  automatically from the handler signature.
	 *
	 *  @param handler This one can either be a pointer to a function or an
	 *       object whose class contains only one operator().
	 *
	 *  @param option_name "Name" of this option.
	 *
	 *  @returns none.
	 *
	 *  @throws If an option with one of the names that are contained in op_name_t
	 *  already exists an exception will be thrown.
	 *
	 *  @code
	 *   interpreter<> l;
	 *
	 *   l.add(&some_fn, op_name_t("option_name").description("some info"));
	 *     // where some_fn  is function.
	 *
	 *   l.add(object, op_name_t("op_name_2").description("some info"));
	 *     //object whose class contains a single operator(), except for template one.
	 *
	 *  @endcode
	 */
	template<typename Handler>
	void add(Handler const& handler, op_name_t const& option_name) {
		super_t::add(handler,option_name);
	}
	
	/// Registers options.
	/**
	 *  This function registrates command options.
	 *
	 *  @param handler This one can either be a pointer to a function or an
	 *       object whose class contains only one operator().
	 *
	 *  @param option_name "Name" of this option.
	 *
	 *  @returns none.
	 *
	 *  @throws If an option with one of the names that are contained in op_name_t
	 *  already exists an exception will be thrown.
	 *
	 * @code
	 * interpreter<> l;
	 * l.add<void (A1,...,An)> (object, op_name_t("option_name").description("some info"));
	 * @endcode
	 */
	template<typename HndlSign, typename Handler>
	void add(Handler const& handler, op_name_t const& option_name) {
		super_t::template add<HndlSign>(handler,option_name);
	}
	
	/// Removes an option in an interpreter with name.
	/**
	 * This function removes an option by name.
	 * @param option_name Name of the option.
	 */
	void erase(string_type const& option_name) {
		super_t::erase(option_name);
	}
	
	/// Visits all options.
	/**
	 * @param visitor An object or pointer to a function.
	 * @note The visitor has to support call semantic : operator(op_name_t const&).
	 */
	template<typename Visitor>
	void visit(Visitor& visitor) const {
		super_t::visit(visitor);
	}
	
	/// Invokes handler
	/** Invokes handler by option name with parameters [args_begin, args_end).
	 *
	 *  @param option_name Name of an option
	 *
	 *  @param args_begin Iterator referring to the first argument for the option.
	 *  @param args_end   Iterator referring to the past-the-end argument for the option.
	 *
	 *  @throws If option isn't found or the handler of this options takes more
	 *  than required arguments or they can't be converted, an exception will be
	 *  thrown.
	 */
	template<typename It>
	void invoke(string_type const& option_name, It args_begin, It args_end, type_cast_t& type_cast) const {
		super_t::invoke(option_name, args_begin, args_end, type_cast);
	}
	
	/**
	 * Constructor
	 */
	interpreter() : super_t() {
	}
	
	/**
	 * Constructor
	 */
	explicit interpreter(interpreter const& rh) : super_t(rh) {
	}
	
	/**
	 * operator =
	 */
	interpreter& operator = (interpreter const& rh) {
		if(this != &rh) {
			swap(interpreter(rh));
		}
		return *this;
	}
};


namespace detail {
// visitor for command_line::interpreter.

template<typename Interpreter>
struct opname_size {
	Interpreter const* interpreter;
	
	size_t value;
	
	explicit opname_size(Interpreter const& interpreter)
		: interpreter(&interpreter)
		, value(0) {
	}
	
	template<typename Key>
	void operator() (Key& key) {
		typename Key::const_iterator it(key.begin()) , end(key.end());
		
		if(it == end) { return; }
		
		size_t cur_size(0);
		
		cur_size += it->size();
		for(++it; it != end; ++it) {
			cur_size += 5 + it->size();
		}
		
		if(key.has_args()) {
			cur_size += 4;
		}
		
		value = std::max(cur_size, value);
	}
};

template<typename Stream, typename Interpreter>
struct print_op_t {
	Stream * stream_;
	Interpreter const* interpreter;
	
	size_t offset;
	
	print_op_t(Stream & stream, Interpreter const& interpreter, size_t offset)
		: stream_(&stream)
		, interpreter(&interpreter)
		, offset(offset) {
	}
	
	template<typename Key>
	void align(Key& key) const {
		opname_size<Interpreter> tmp(*interpreter);
		tmp(key);
		
		for(size_t i(tmp.value); i < offset; ++i)
			(*stream_) << " ";
	}
	
	template<typename Key>
	void operator() (Key& key) const {
		typename Key::const_iterator it(key.begin()) , end(key.end());
		
		if(it == end) {
			return;
		}
		
		(*stream_) << "  -" << *it;
		
		for(++it;it!=end;++it) {
			(*stream_) << " [ " << "--" << *it << " ]";
		}
		
		if(key.has_args()) {
			(*stream_) << " arg";
		}
		
		align(key);
		(*stream_) << " " << key.get_description() << std::endl;
	}
};

template<typename OStream, typename Interpreter>
void print_op(OStream& os, Interpreter const & interpreter) {
	opname_size<Interpreter> calc_size(interpreter);
	interpreter.visit(calc_size);
	
	print_op_t<OStream,Interpreter> op(os,interpreter,calc_size.value);
	interpreter.visit(op);
}

} // namespace detail

template<typename CharType, typename StringType, typename TypeCast>
std::basic_ostream<CharType>& operator << (std::basic_ostream<CharType>& os,
                                           interpreter<StringType,TypeCast> const& l) {
	return detail::print_op(os,l), os;
}


#endif // ARX_UTIL_CMDLINE_INTERPRETER_H
