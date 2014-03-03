/* EWL
 * Copyright © 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 16:03:13 $
 * $Revision: 1.1 $
 */

#include <ewl_thread>
#include <vector>

#ifndef _EWL_NO_THREAD

#if defined(_EWL_USE_MPTASKS) || defined(_EWL_USE_WINTHREADS)
	#include <hash_map>
	#include <list>
#endif  // defined(_EWL_USE_MPTASKS) || defined(_EWL_USE_WINTHREADS)

#ifdef __CWCC__
	#pragma warn_padding off
#endif

#ifdef _EWL_FORCE_ENUMS_ALWAYS_INT
	#if _EWL_FORCE_ENUMS_ALWAYS_INT
		#pragma enumsalwaysint on
	#else
		#pragma enumsalwaysint off
	#endif
#endif  // _EWL_FORCE_ENUMS_ALWAYS_INT

#ifdef _EWL_FORCE_ENABLE_BOOL_SUPPORT
	#if _EWL_FORCE_ENABLE_BOOL_SUPPORT
		#pragma bool on
	#else
		#pragma bool off
	#endif
#endif  // _EWL_FORCE_ENABLE_BOOL_SUPPORT

namespace {

class thread_param
{
public:
	explicit thread_param(const CodeWarrior::thread::func_type& f)
		:	f_(f), started_(false)
	{
	}

	void wait()
	{
		CodeWarrior::mutex::scoped_lock lock(mutex_);
		while (!started_)
			condition_.wait(lock);
	}

	void started()
	{
		CodeWarrior::mutex::scoped_lock lock(mutex_);
		started_ = true;
		condition_.notify_one();
	}

	const CodeWarrior::thread::func_type& f_;
	CodeWarrior::mutex mutex_;
	CodeWarrior::condition condition_;
	volatile bool started_;
};

#if __CWCC__ >= 0x3200

typedef void (*fp)();

class thread_param2
{
public:
	explicit thread_param2(void (*f)())
		:	f_(f), started_(false)
	{
	}

	void wait()
	{
		CodeWarrior::mutex::scoped_lock lock(mutex_);
		while (!started_)
			condition_.wait(lock);
	}

	void started()
	{
		CodeWarrior::mutex::scoped_lock lock(mutex_);
		started_ = true;
		condition_.notify_one();
	}

	CodeWarrior::mutex mutex_;
	CodeWarrior::condition condition_;
	fp f_;
	volatile bool started_;
};

#endif  // __CWCC__ >= 0x3200

#if defined(_EWL_USE_MPTASKS) || defined(_EWL_USE_WINTHREADS)

struct handler
{
	handler() {}
	handler(void (*f)(void*), void* arg)
		:	f_(f), arg_(arg) {}
	void (*f_)(void*);
	void* arg_;
};

#ifdef _EWL_USE_MPTASKS
	typedef TaskStorageIndex thread_exit_key_type;
#endif
#ifdef _EWL_USE_WINTHREADS
	typedef DWORD thread_exit_key_type;
#endif

class handler_map_type
{
private:

	typedef CodeWarrior::hash_map<thread_exit_key_type, handler> impl;

	impl map_;

public:
	handler& operator[](const thread_exit_key_type& x)
		{return map_[x];}

	~handler_map_type();
};

handler_map_type::~handler_map_type()
{
	for (impl::iterator i = map_.begin(), e = map_.end(); i != e; ++i)
		i->second.f_(i->second.arg_);
}

class thread_exit_key_helper
{
public:
	thread_exit_key_helper();
	~thread_exit_key_helper();
	thread_exit_key_type& expose() {return key_;}
private:
	thread_exit_key_type key_;

	thread_exit_key_helper(const thread_exit_key_helper&);
	thread_exit_key_helper& operator=(const thread_exit_key_helper&);
};

#ifdef _EWL_USE_MPTASKS

inline
thread_exit_key_helper::thread_exit_key_helper()
{
	if (MPAllocateTaskStorageIndex(&key_))
		CodeWarrior::detail::throw_thread_resource_error();
}

inline
thread_exit_key_helper::~thread_exit_key_helper()
{
	MPDeallocateTaskStorageIndex(key_);
}

#endif  // _EWL_USE_MPTASKS

#ifdef _EWL_USE_WINTHREADS

inline
thread_exit_key_helper::thread_exit_key_helper()
{
	key_ = TlsAlloc();
	if (key_ == TLS_OUT_OF_INDEXES)
		CodeWarrior::detail::throw_thread_resource_error();
}

inline
thread_exit_key_helper::~thread_exit_key_helper()
{
	TlsFree(key_);
}

#endif  // _EWL_USE_WINTHREADS

thread_exit_key_type&
get_thread_exit_key_type_impl()
{
	static thread_exit_key_helper thread_exit_key;
	return thread_exit_key.expose();
}

void init_thread_exit_key_type()
{
	get_thread_exit_key_type_impl();
}

CodeWarrior::once_flag thread_exit_key_flag = _EWL_THREAD_ONCE_INIT;

thread_exit_key_type&
get_thread_exit()
{
	CodeWarrior::call_once(init_thread_exit_key_type, thread_exit_key_flag);
	return get_thread_exit_key_type_impl();
}

#endif  // defined(_EWL_USE_MPTASKS) || defined(_EWL_USE_WINTHREADS)

} // unnamed namespace

extern "C" {
static
#ifdef _EWL_USE_MPTASKS
OSStatus
#elif defined(_EWL_USE_WINTHREADS)
DWORD __stdcall
#else
void*
#endif
thread_proxy(void* param)
{
#if defined(_EWL_USE_MPTASKS) || defined(_EWL_USE_WINTHREADS)
	handler_map_type handler_map;
#endif
#ifndef _EWL_NO_EXCEPTIONS
	try
	{
#endif  // _EWL_NO_EXCEPTIONS
		thread_param* p = static_cast<thread_param*>(param);
		CodeWarrior::thread::func_type f = p->f_;
		p->started();
	#ifdef _EWL_USE_MPTASKS
		MPSetTaskStorageValue(get_thread_exit(), (TaskStorageValue)&handler_map);
	#endif
	#ifdef _EWL_USE_WINTHREADS
		TlsSetValue(get_thread_exit(), (LPVOID)&handler_map);
	#endif
		f();
#ifndef _EWL_NO_EXCEPTIONS
	}
	catch (...)
	{
	}
#endif  // _EWL_NO_EXCEPTIONS
	return 0;
}

#if __CWCC__ >= 0x3200

static
#ifdef _EWL_USE_MPTASKS
OSStatus
#elif defined(_EWL_USE_WINTHREADS)
DWORD __stdcall
#else
void*
#endif
thread_proxy2(void* param)
{
#if defined(_EWL_USE_MPTASKS) || defined(_EWL_USE_WINTHREADS)
	handler_map_type handler_map;
#endif
#ifndef _EWL_NO_EXCEPTIONS
	try
	{
#endif  // _EWL_NO_EXCEPTIONS
		thread_param2* p = static_cast<thread_param2*>(param);
		fp f = p->f_;
		p->started();
	#ifdef _EWL_USE_MPTASKS
		MPSetTaskStorageValue(get_thread_exit(), (TaskStorageValue)&handler_map);
	#endif
	#ifdef _EWL_USE_WINTHREADS
		TlsSetValue(get_thread_exit(), (LPVOID)&handler_map);
	#endif
		f();
#ifndef _EWL_NO_EXCEPTIONS
	}
	catch (...)
	{
	}
#endif  // _EWL_NO_EXCEPTIONS
	return 0;
}

#endif  // __CWCC__ >= 0x3200

}

#ifndef _EWL_NO_CPP_NAMESPACE
	namespace CodeWarrior {
#else
	#ifndef CodeWarrior
		#define CodeWarrior
	#endif
#endif  // _EWL_NO_CPP_NAMESPACE

namespace detail
{

void throw_thread_resource_error()
{
#ifndef _EWL_NO_EXCEPTIONS
	throw thread_resource_error();
#else
	_STD::__ewl_error("thread_resource_error");
#endif
}

void throw_lock_error()
{
#ifndef _EWL_NO_EXCEPTIONS
	throw lock_error();
#else
	_STD::__ewl_error("lock_error");
#endif
}

}  // detail

thread::thread(const func_type& f)
	:	joinable_(true)
{
    thread_param param(f);
#ifdef _EWL_USE_PTHREADS
	if (pthread_create(&id_, 0, &thread_proxy, &param))
		detail::throw_thread_resource_error();
#endif  // _EWL_USE_PTHREADS
#ifdef _EWL_USE_MPTASKS
	if (MPCreateQueue(&join_queue_))
		detail::throw_thread_resource_error();
	if (MPCreateTask(&thread_proxy, &param, 0UL, join_queue_, 0, 0, 0UL, &id_))
	{
		MPDeleteQueue(join_queue_);
		detail::throw_thread_resource_error();
	}
#endif  // _EWL_USE_MPTASKS
#ifdef _EWL_USE_WINTHREADS
	thread_ = CreateThread(0, 0, &thread_proxy, &param, 0, &id_);
	if (!thread_)
		detail::throw_thread_resource_error();
#endif  // _EWL_USE_WINTHREADS
#ifdef _EWL_SINGLE_THREAD
	thread_proxy(&param);
#endif
	param.wait();
}

#if __CWCC__ >= 0x3200

thread::thread(void (*f)())
	: joinable_(true)
{
    thread_param2 param(f);
#ifdef _EWL_USE_PTHREADS
    if (pthread_create(&id_, 0, &thread_proxy2, &param))
        detail::throw_thread_resource_error();
#endif  // _EWL_USE_PTHREADS
#ifdef _EWL_USE_MPTASKS
	if (MPCreateQueue(&join_queue_))
		detail::throw_thread_resource_error();
	if (MPCreateTask(&thread_proxy2, &param, 0UL, join_queue_, 0, 0, 0UL, &id_))
	{
		MPDeleteQueue(join_queue_);
		detail::throw_thread_resource_error();
	}
#endif  // _EWL_USE_MPTASKS
#ifdef _EWL_USE_WINTHREADS
	thread_ = CreateThread(0, 0, &thread_proxy2, &param, 0, &id_);
	if (!thread_)
		detail::throw_thread_resource_error();
#endif  // _EWL_USE_WINTHREADS
#ifdef _EWL_SINGLE_THREAD
	thread_proxy2(&param);
#endif
    param.wait();
}

#endif  // __CWCC__ >= 0x3200

thread_group::thread_group()
{
	typedef _STD::vector<thread*> C;
	compile_assert<sizeof(impl) == sizeof(C)> e1; (void)e1;
}

thread_group::~thread_group()
{
	typedef _STD::vector<thread*> C;
	C& v = (C&)impl_;
	for (C::iterator i = v.begin(), e = v.end(); i != e; ++i)
		delete *i;
	v.~C();
}

const thread*
thread_group::create_thread(const thread::func_type& f)
{
	typedef _STD::vector<thread*> C;
	mutex::scoped_lock lock(mutex_);
	C& v = (C&)impl_;
	move_ptr<thread> t(new thread(f));
	v.push_back(t.get());
	return t.release();
}

void
thread_group::join_all()
{
	typedef _STD::vector<thread*> C;
	mutex::scoped_lock lock(mutex_);
	C& v = (C&)impl_;
	for (C::iterator i = v.begin(), e = v.end(); i != e; ++i)
		(*i)->join();
}

#ifdef _EWL_USE_MPTASKS

#if defined(__CWCC__) && __option(only_std_keywords)
	#pragma only_std_keywords off
#endif

#pragma gcc_extensions on
#include <Carbon.h>
#pragma gcc_extensions reset

#if defined(__CWCC__)
	#pragma only_std_keywords reset
#endif

void
call_once(void (*func)(), once_flag& flag)
{
	if (!flag.done_)
	{
		if (!flag.started_ && OTAtomicAdd32(1, (SInt32*)&flag.started_) == 1)
		{
			func();
			flag.done_ = 1;
			return;
		}
		while (!flag.done_)
			thread::yield();
	}
}

#endif  // _EWL_USE_MPTASKS

#ifdef _EWL_USE_WINTHREADS

void
call_once(void (*func)(), once_flag& flag)
{
	if (flag.not_done_)
	{
		if (flag.not_started_ && InterlockedIncrement(&flag.not_started_) == 0)
		{
			func();
			flag.not_done_ = 0;
			return;
		}
		while (flag.not_done_)
			thread::yield();
	}
}

#endif  // _EWL_USE_WINTHREADS

#if defined(_EWL_USE_MPTASKS) || defined(_EWL_USE_WINTHREADS)

namespace
{

mutex&
get_ts_mutex_impl()
{
	static mutex ts_mutex;
	return ts_mutex;
}

void init_ts_mutex()
{
	get_ts_mutex_impl();
}

once_flag init_ts = _EWL_THREAD_ONCE_INIT;

mutex&
get_ts_mutex()
{
	call_once(init_ts_mutex, init_ts);
	return get_ts_mutex_impl();
}

handler_map_type*
get_ts_map()
{
	mutex::scoped_lock lock(get_ts_mutex());
	static _STD::list<handler_map_type> v;
	v.push_back(handler_map_type());
	return &v.back();
}

}

#endif  // defined(_EWL_USE_MPTASKS) || defined(_EWL_USE_WINTHREADS)

#ifdef _EWL_USE_MPTASKS

void
thread_specific_ptr_base::set(void (*f)(void*), const volatile void* p)
{
	MPSetTaskStorageValue(key_, (TaskStorageValue)p);
	handler_map_type* map = (handler_map_type*)MPGetTaskStorageValue(get_thread_exit());
	if (map == 0)
	{
		map = get_ts_map();
		MPSetTaskStorageValue(get_thread_exit(), (TaskStorageValue)map);
	}
	(*map)[key_] = handler(f, (void*)p);
}

#endif  // _EWL_USE_MPTASKS

#ifdef _EWL_USE_WINTHREADS

void
thread_specific_ptr_base::set(void (*f)(void*), const volatile void* p)
{
	TlsSetValue(key_, (LPVOID)p);
	handler_map_type* map = (handler_map_type*)TlsGetValue(get_thread_exit());
	if (map == 0)
	{
		map = get_ts_map();
		TlsSetValue(get_thread_exit(), (LPVOID)map);
	}
	(*map)[key_] = handler(f, (void*)p);
}

#endif  // _EWL_USE_WINTHREADS

#ifndef _EWL_NO_CPP_NAMESPACE
	} // namespace CodeWarrior
#endif

#endif  // _EWL_NO_THREAD

_EWL_START_TR1_NAMESPACE

namespace detail
{

#if defined(_EWL_SHARED_PTR_HAS_MUTEX)  && !defined(__POWERPC__)

void
shared_ptr_deleter_common::attach(bool lk)
{
	CodeWarrior::mutex::scoped_lock lock(mut_, CodeWarrior::defer_lock);
	if (lk)
		lock.lock();
	++use_count_;
}

void
shared_ptr_deleter_common::attach_weak(bool lk)
{
	CodeWarrior::mutex::scoped_lock lock(mut_, CodeWarrior::defer_lock);
	if (lk)
		lock.lock();
	++weak_count_;
}

void
shared_ptr_deleter_common::release()
{
	bool bye = false;
	{
		CodeWarrior::mutex::scoped_lock lock(mut_);
		if (--use_count_ == 0)
		{
			dispose();
			bye = --weak_count_ == 0;
		}
	}
	if (bye)
		destroy();
}

void
shared_ptr_deleter_common::release_weak()
{
	bool bye;
	{
		CodeWarrior::mutex::scoped_lock lock(mut_);
		bye = --weak_count_ == 0;
	}
	if (bye)
		destroy();
}

#elif defined(_EWL_SHARED_PTR_HAS_MUTEX)  && defined(__POWERPC__)

void
shared_ptr_deleter_common::release()
{
	unsigned acquired = 0;
	unsigned released = 0;
	register ptrdiff_t* const p = &use_count_;
	register ptrdiff_t a;
	asm
	{
	loop:
		lwarx    a, 0, p
		subi     a, a, 1
	}
	if (a != 0 && !released)
	{
		asm
		{
			machine all
			sync
		}
		released = 1;
	}
	asm
	{
		stwcx.   a, 0, p
		bne-     loop
	}
	if (a == 0)
	{
		asm {isync}
		acquired = 1;
		dispose();
		release_weak(acquired, released);
	}
}

void
shared_ptr_deleter_common::release_weak(unsigned acquired, unsigned released)
{
	register ptrdiff_t* const p = &weak_count_;
	register ptrdiff_t a;
	asm
	{
	loop:
		lwarx    a, 0, p
		subi     a, a, 1
	}
	if (a != 0 && !released)
	{
		asm
		{
			machine all
			sync
		}
		released = 1;
	}
	asm
	{
		stwcx.   a, 0, p
		bne-     loop
	}
	if (a == 0)
	{
		if (!acquired)
		{
			asm {isync}
//			acquired = 1;
		}
		destroy();
	}
}

#else  // _EWL_SHARED_PTR_HAS_MUTEX

void
shared_ptr_deleter_common::release()
{
	if (--use_count_ == 0)
	{
		dispose();
		release_weak();
	}
}

void
shared_ptr_deleter_common::release_weak()
{
	if (--weak_count_ == 0)
		destroy();
}

#endif  // _EWL_SHARED_PTR_HAS_MUTEX

}  // detail

_EWL_END_TR1_NAMESPACE

