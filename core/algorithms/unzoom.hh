
#pragma once

#include "Algorithm.hh"

namespace cadabra {

	class unzoom : public Algorithm {
		public:
			unzoom(const Kernel&, Ex&);

			virtual bool     can_apply(iterator) override;
			virtual result_t apply(iterator&) override;
		};

	}

