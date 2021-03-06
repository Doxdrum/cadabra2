#pragma once

#include <pybind11/pybind11.h>
#include "py_ex.hh"
#include "py_helpers.hh"
#include "py_kernel.hh"
#include "py_progress.hh"

namespace cadabra {
	template <class Algo>
	Ex_ptr apply_algo_base(Algo& algo, Ex_ptr ex, bool deep, bool repeat, unsigned int depth, bool pre_order=false)
		{
		Ex::iterator it = ex->begin();
		if (ex->is_valid(it)) {
			ProgressMonitor* pm = get_progress_monitor();
			algo.set_progress_monitor(pm);
			if(pre_order)
				ex->update_state(algo.apply_pre_order(repeat));
			else
				ex->update_state(algo.apply_generic(it, deep, repeat, depth));
			call_post_process(*get_kernel_from_scope(), ex);
			}

		return ex;
		}

	template <class Algo>
	Ex_ptr apply_algo(Ex_ptr ex, bool deep, bool repeat, unsigned int depth)
		{
		Algo algo(*get_kernel_from_scope(), *ex);
		return apply_algo_base(algo, ex, deep, repeat, depth, false);
		}

	template <class Algo, typename Arg1>
	Ex_ptr apply_algo(Ex_ptr ex, Arg1 arg1, bool deep, bool repeat, unsigned int depth)
		{
		Algo algo(*get_kernel_from_scope(), *ex, arg1);
		return apply_algo_base(algo, ex, deep, repeat, depth, false);
		}

	template <class Algo, typename Arg1, typename Arg2>
	Ex_ptr apply_algo(Ex_ptr ex, Arg1 arg1, Arg2 arg2, bool deep, bool repeat, unsigned int depth)
		{
		Algo algo(*get_kernel_from_scope(), *ex, arg1, arg2);
		return apply_algo_base(algo, ex, deep, repeat, depth, false);
		}

	template <class Algo, typename Arg1, typename Arg2, typename Arg3>
	Ex_ptr apply_algo(Ex_ptr ex, Arg1 arg1, Arg2 arg2, Arg3 arg3, bool deep, bool repeat, unsigned int depth)
		{
		Algo algo(*get_kernel_from_scope(), *ex, arg1, arg2, arg3);
		return apply_algo_base(algo, ex, deep, repeat, depth, false);
		}


	template<class Algo, typename... Args, typename... PyArgs>
	void def_algo(pybind11::module& m, const char* name, bool deep, bool repeat, unsigned int depth, PyArgs... pyargs)
		{
		m.def(name,
		      &apply_algo<Algo, Args...>,
		      pybind11::arg("ex"),
		      std::forward<PyArgs>(pyargs)...,
		      pybind11::arg("deep") = deep,
		      pybind11::arg("repeat") = repeat,
		      pybind11::arg("depth") = depth,
		      pybind11::doc(read_manual("algorithms", name).c_str()),
		      pybind11::return_value_policy::reference_internal);
		}

	template <class Algo>
	Ex_ptr apply_algo_preorder(Ex_ptr ex, bool deep, bool repeat, unsigned int depth)
		{
		Algo algo(*get_kernel_from_scope(), *ex);
		return apply_algo_base(algo, ex, deep, repeat, depth, true);
		}

	template <class Algo, typename Arg1>
	Ex_ptr apply_algo_preorder(Ex_ptr ex, Arg1 arg1, bool deep, bool repeat, unsigned int depth)
		{
		Algo algo(*get_kernel_from_scope(), *ex, arg1);
		return apply_algo_base(algo, ex, deep, repeat, depth, true);
		}

	template <class Algo, typename Arg1, typename Arg2>
	Ex_ptr apply_algo_preorder(Ex_ptr ex, Arg1 arg1, Arg2 arg2, bool deep, bool repeat, unsigned int depth)
		{
		Algo algo(*get_kernel_from_scope(), *ex, arg1, arg2);
		return apply_algo_base(algo, ex, deep, repeat, depth, true);
		}

	template<class Algo, typename... Args, typename... PyArgs>
	void def_algo_preorder(pybind11::module& m, const char* name, bool deep, bool repeat, unsigned int depth, PyArgs... pyargs)
		{
		m.def(name,
		      &apply_algo_preorder<Algo, Args...>,
		      pybind11::arg("ex"),
		      std::forward<PyArgs>(pyargs)...,
		      pybind11::arg("deep") = deep,
		      pybind11::arg("repeat") = repeat,
		      pybind11::arg("depth") = depth,
		      pybind11::doc(read_manual("algorithms", name).c_str()),
		      pybind11::return_value_policy::reference_internal);
		}


	void init_algorithms(pybind11::module& m);
	}
