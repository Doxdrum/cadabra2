
#pragma once

#include "Algorithm.hh"

class order : virtual public Algorithm {
	public:
		order(Kernel&, Ex&, Ex& objs, bool ac);

		virtual bool     can_apply(iterator) override;
		virtual result_t apply(iterator&) override;

	protected:
		Ex objects;
		bool    anticomm;
};
