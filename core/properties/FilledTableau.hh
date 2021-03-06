#pragma once

#include "Props.hh"

namespace cadabra {

	class FilledTableau : public property {
		public:
			virtual ~FilledTableau() {};
			virtual std::string name() const override;
			virtual bool        parse(Kernel&, keyval_t& keyvals) override;

			int dimension;
		};

	}
