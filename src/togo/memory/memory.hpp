#line 2 "togo/memory/memory.hpp"
/**
@copyright MIT license; see @ref index or the accompanying LICENSE file.

@file
@brief Memory interface.
@ingroup memory
*/

#pragma once

#include <togo/config.hpp>
#include <togo/types.hpp>
#include <togo/utility/traits.hpp>
#include <togo/memory/types.hpp>
#include <togo/memory/memory.gen_interface>

#include <new>
#include <cstdlib>

namespace togo {

/**
	@addtogroup memory
	@{
*/

/// Allocate an object with an allocator without constructing it.
#define TOGO_ALLOCATE(a, T) \
	((a).allocate(sizeof(T), alignof(T)))

/// Construct an object with an allocator.
#define TOGO_CONSTRUCT(a, T, ...) \
	(new ((a).allocate(sizeof(T), alignof(T))) T(__VA_ARGS__))

/// Construct an object with an allocator and no alignment.
#define TOGO_CONSTRUCT_NA(a, T, ...) \
	(new ((a).allocate(sizeof(T), 0)) T(__VA_ARGS__))

/// Construct an object with an allocator and no initialization parameters.
/// This calls the default constructor.
#define TOGO_CONSTRUCT_DEFAULT(a, T) \
	(new ((a).allocate(sizeof(T), alignof(T))) T())

/// Construct an object with an allocator, no initialization parameters,
/// and no alignment.
///
/// This calls the default constructor.
#define TOGO_CONSTRUCT_DEFAULT_NA(a, T) \
	(new ((a).allocate(sizeof(T), 0)) T())

/// Destroy an object with an allocator.
///
/// Does nothing if p is nullptr. Calls destructor p->~T(); and
/// deallocates the object.
// Oh C++.
#define TOGO_DESTROY(a, p) \
	do { if (p) { \
		(p)->~remove_cv<remove_ptr<decltype(p)>>(); (a).deallocate(p); \
	} } while (false)

/// %Allocator base class.
class Allocator {
public:
	static constexpr u32 const
	/// Default allocation alignment.
	DEFAULT_ALIGNMENT = 4,
	/// Size value returned by unsupported operations.
	SIZE_NOT_TRACKED = static_cast<u32>(-1);

	Allocator() = default;
	Allocator(Allocator&&) = default;
	Allocator& operator=(Allocator&&) = default;

	Allocator(Allocator const&) = delete;
	Allocator& operator=(Allocator const&) = delete;

	/// Allocators should assert that they have no active allocations
	/// in the destructor.
	virtual ~Allocator() = 0;

	/// Get number of active allocations.
	virtual u32 num_allocations() const = 0;

	/// Get total number of bytes allocated by the allocator.
	///
	/// If an allocator does not support this operation, it shall
	/// return SIZE_NOT_TRACKED.
	virtual u32 total_size() const = 0;

	/// Get size of block allocated for p.
	///
	/// This may be greater than the actual size of the object type
	/// due to internal state and alignment. If an allocator does not
	/// support this operation, it shall return SIZE_NOT_TRACKED.
	virtual u32 allocation_size(void const* p) const = 0;

	/// Allocate a chunk of memory.
	///
	/// If align is 0, no alignment is used.
	virtual void* allocate(u32 size, u32 align = DEFAULT_ALIGNMENT) = 0;

	/// Deallocate a pointer.
	///
	/// Does nothing if p is nullptr.
	virtual void deallocate(void const* p) = 0;
};
inline Allocator::~Allocator() = default;

/** @} */ // end of doc-group memory

} // namespace togo
