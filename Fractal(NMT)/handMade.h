#pragma once

namespace hm {

	template <typename T>
	T CinFail(T content) {
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			//std::cout << "ERROR: cin overflow, changing the value you entered to 3";
			return std::numeric_limits<T>::max();
		}
		return content;
	}
}