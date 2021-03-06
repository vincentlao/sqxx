
// (c) 2013, 2014 Stephan Hohe

#if !defined(SQXX_CONTEXT_HPP_INCLUDED)
#define SQXX_CONTEXT_HPP_INCLUDED

#include "datatypes.hpp"

struct sqlite3_context;

namespace sqxx {

/** Wraps `struct sqlite3_context` */
class context {
private:
	// UNOWNED
	sqlite3_context *handle;

public:
	context(sqlite3_context *handle_arg) : handle(handle_arg) {
	}

	/**
	 * Sets result to NULL:
	 *
	 * Wraps [`sqlite3_result_null()`](http://www.sqlite.org/c3ref/result_blob.html)
	 */
	void result_null();
	void result();

	/**
	 * Sets result to error with the given message
	 *
	 * Wraps [`sqlite3_result_error()`](http://www.sqlite.org/c3ref/result_blob.html)
	 */
	void result_error(const char *msg);
	/**
	 * Sets result to error with the given error code
	 *
	 * Wraps [`sqlite3_result_error_code()`](http://www.sqlite.org/c3ref/result_blob.html)
	 */
	void result_error_code(int code);
	/**
	 * Sets result to error "out of memory"
	 *
	 * Wraps [`sqlite3_result_error_nomem()`](http://www.sqlite.org/c3ref/result_blob.html)
	 */
	void result_error_nomem();
	/**
	 * Sets result to error "too big"
	 *
	 * Wraps [`sqlite3_result_error_toobig()`](http://www.sqlite.org/c3ref/result_blob.html)
	 */
	void result_error_toobig();
	/**
	 * Sets result to error SQLITE_MISUSE
	 *
	 * Wraps [`sqlite3_result_error_code(SQLITE_MISUSE)`](http://www.sqlite.org/c3ref/result_blob.html)
	 */
	void result_error_misuse();

	/**
	 * Sets result to given value
	 *
	 * Wraps
	 * [`sqlite3_result_int()`](http://www.sqlite.org/c3ref/result_blob.html),
	 * [`sqlite3_result_int64()`](http://www.sqlite.org/c3ref/result_blob.html),
	 * [`sqlite3_result_double()`](http://www.sqlite.org/c3ref/result_blob.html),
	 * [`sqlite3_result_text()`](http://www.sqlite.org/c3ref/result_blob.html),
	 * [`sqlite3_result_blob()`](http://www.sqlite.org/c3ref/result_blob.html),
	 * and [`sqlite3_result_zeroblob()`](http://www.sqlite.org/c3ref/result_blob.html),
	 *
	 * For sqlite3 >= v3.8.7, uses 64 bit interfaces
	 * [`sqlite3_result_text64()`](http://www.sqlite.org/c3ref/result_blob.html),
	 * [`sqlite3_result_blob64()`](http://www.sqlite.org/c3ref/result_blob.html),
	 *
	 * For sqlite3 >= v3.8.11, uses 64 bit interface
	 * [`sqlite3_result_zeroblob64()`](http://www.sqlite.org/c3ref/result_blob.html),
	 */
	template<typename R>
	if_selected_type<R, void, int, int64_t, double>
	result(R value);

	/* If a nullptr is passed, the result is set to NULL. */
	template<typename R>
	if_selected_type<R, void, const char*>
	result(R value, bool copy=true);

	template<typename R>
	if_selected_type<R, void, std::string, blob>
	result(const R &value, bool copy=true);

	/**
	 * Returns the current aggregation function context.
	 * Used internally by `connection::create_aggregate`. Do not use while in an
	 * aggregate registered with sqxx.
	 *
	 * Wraps [`sqlite3_aggregate_context`](https://sqlite.org/c3ref/aggregate_context.html)
	 */
	void* aggregate_context(int bytes);

	/**
	 * Returns a raw pointer to the underlying `struct sqlite3_context`.
	 */
	sqlite3_context* raw();
};

template<>
void context::result<int>(int value);
template<>
void context::result<int64_t>(int64_t value);
template<>
void context::result<double>(double value);
template<>
void context::result(const char *value, bool copy);
template<>
void context::result(const std::string &value, bool copy);
template<>
void context::result(const blob &value, bool copy);

} // namespace sqxx

#endif // SQXX_CONTEXT_HPP_INCLUDED

