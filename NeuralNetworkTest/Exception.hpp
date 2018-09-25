#pragma once

#include "FastContainerLibrary.hpp"

namespace FastContainer {

	/*��O����*/
	class fast_container_exception :public std::exception {
	public:
		fast_container_exception(const char *s = "fast_container_exception") : std::exception(s) {}
	};

}
