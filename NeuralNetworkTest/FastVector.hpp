#pragma once

#include "FastContainerLibrary.hpp"

namespace FastContainer {

	template<typename T>
	class FastVector {
	public:
		FastVector() { }
		FastVector(int size) { resize(size); }
		FastVector(const std::vector<T> vec) { this->size = vec.size(); entity = vec; }
		~FastVector() { }

		void resize(int size) { this->size = size; entity.resize(size); }

		int get_size() { return size; }

		T& operator[](int index) { return entity[index]; }

		/*»ΜάάΤ·*/
		FastVector<T> identity() { return this; }

		/*βΞl*/
		FastVector<T> abs() { return apply_func([](T x) { return std::abs(x); }); };
		/*βΞl AMPΐ*/
		FastVector<T> amp_abs() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::fabs(x); }); };
		/*βΞl PPLΐ*/
		FastVector<T> ppl_abs() { return apply_ppl_func([](T x) { return std::abs(x); }); };

		/*Log e*/
		FastVector<T> log() { return apply_func([](T x) { return std::log(x); }); };
		/*Log e AMPΐ*/
		FastVector<T> amp_log() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::log(x); }); };
		/*Log e PPLΐ*/
		FastVector<T> ppl_log() { return apply_ppl_func([](T x) { return std::log(x); }); };

		/*Log 10*/
		FastVector<T> log10() { return apply_func([](T x) { return std::log10(x); }); };
		/*Log 10*/
		FastVector<T> amp_log10() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::log10(x); }); };
		/*Log 10*/
		FastVector<T> ppl_log10() { return apply_ppl_func([](T x) { return std::log10(x); }); };

		/*2ζͺ*/
		FastVector<T> sqrt() { return apply_func([](T x) { return std::sqrt(x); }); };
		/*2ζͺ AMPΐ*/
		FastVector<T> amp_sqrt() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::sqrt(x); }); };
		/*2ζͺ PPLΐ*/
		FastVector<T> ppl_sqrt() { return apply_ppl_func([](T x) { return std::sqrt(x); }); };

		/*Kζ*/
		FastVector<T> pow(T exp) { return apply_func([=](T x) { return std::pow(x, exp); }); };
		/*Kζ AMPΐ*/
		FastVector<T> amp_pow(T exp) { return apply_amp_func([=](T x) restrict(amp) { return concurrency::fast_math::pow(x, exp); }); };
		/*Kζ PPLΐ*/
		FastVector<T> ppl_pow(T exp) { return apply_ppl_func([=](T x) { return std::pow(x, exp); }); };

		/*e^x*/
		FastVector<T> exp() { return apply_func([](T x) { return std::exp(x); }); };
		/*e^x AMPΐ*/
		FastVector<T> amp_exp() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::exp(x); }); };
		/*e^x PPLΐ*/
		FastVector<T> ppl_exp() { return apply_ppl_func([](T x) { return std::exp(x); }); };

		/*VOChΦ*/
		FastVector<T> sigmoid() { return apply_func([](T x) { return (T)1 / (1 + std::exp(-x)); }); };
		/*VOChΦ AMPΐ*/
		FastVector<T> amp_sigmoid() { return apply_amp_func([](T x) restrict(amp) { return (T)1 / (1 + concurrency::fast_math::exp(-x)); }); };
		/*VOChΦ PPLΐ*/
		FastVector<T> ppl_sigmoid() { return apply_ppl_func([](T x) { return (T)1 / (1 + std::exp(-x)); }); };

		/*ReLUΦ*/
		FastVector<T> relu() { return apply_func([](T x) { return x > 0 ? x : 0; }); };
		/*ReLUΦ AMPΐ*/
		FastVector<T> amp_relu() { return apply_amp_func([](T x) restrict(amp) { return x > 0 ? x : 0; }); };
		/*ReLUΦ PPLΐ*/
		FastVector<T> ppl_relu() { return apply_ppl_func([](T x) { return x > 0 ? x : 0; }); };

		/*\tg}bNXΦ*/
		FastVector<T> softmax() {
			T max = get_max();
			T total = apply_func([=](T x) { return std::exp(x - max); }).sum();
			return apply_func([=](T x) { return std::exp(x - max) / total; });
		}
		/*\tg}bNXΦ AMPΐ*/
		FastVector<T> amp_softmax() {
			T max = get_max();
			T total = apply_amp_func([=](T x) restrict(amp) { return concurrency::fast_math::exp(x - max); }).sum();
			return apply_amp_func([=](T x) restrict(amp) { return concurrency::fast_math::exp(x - max) / total; });
		}
		/*\tg}bNXΦ PPLΐ*/
		FastVector<T> ppl_softmax() {
			T max = get_max();
			T total = apply_ppl_func([=](T x) { return std::exp(x - max); }).sum();
			return apply_ppl_func([=](T x) { return std::exp(x - max) / total; });
		}

		/*lχͺ
		func: (T)(*func)(T x)*/
		template<class F>
		FastVector<T> num_diff(F func, T delta = 0.0001) { return apply_func([=](T x) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); };
		/*lχͺ AMPΐ
		func: (T)(*func)(T x)*/
		template<class F>
		FastVector<T> amp_num_diff(F func, T delta = 0.0001) { return apply_amp_func([=](T x) restrict(amp) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); };
		/*lχͺ PPLΐ
		func: (T)(*func)(T x)*/
		template<class F>
		FastVector<T> ppl_num_diff(F func, T delta = 0.0001) { return apply_ppl_func([=](T x) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); };

		/*Ε¬l*/
		T get_min() {
			T result = entity[0];
			for (auto x : entity) result = min(result, x);
			return result;
		}
		/*Εεl*/
		T get_max() {
			T result = entity[0];
			for (auto x : entity) result = max(result, x);
			return result;
		}
		/*ΕεlΜCfbNX*/
		T get_argmax() {
			int result = 0;
			T max = entity[0];
			for (int i = 0; i < size; i++) {
				if (max < entity[i]) {
					max = entity[i];
					result = i;
				}
			}
			return result;
		}

		/*v*/
		T sum() {
			T result = 0;
			for (auto x : entity) result += x;
			return result;
		}
		/*v PPLΐ*/
		T ppl_sum() {
			concurrency::combinable<T> result;
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result.local() += entity[i];
			});
			return result.combine(std::plus<T>());
		}

		/*½Ο*/
		T mean() {
			return sum() / size;
		}
		/*½Ο PPLΐ*/
		T ppl_mean() {
			return ppl_sum() / size;
		}

		/*π·Ggs[λ·*/
		T cross_entropy_error(FastVector<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_combo_func([=](T x1, T x2) { return -1 * x2 * std::log(x1 + delta) + (1 - x2) * std::log(1 - x1 + delta); }, teacher);
			return buf.sum();
		}
		/*π·Ggs[λ· AMPΐ*/
		T amp_cross_entropy_error(FastVector<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_amp_combo_func([=](T x1, T x2) restrict(amp) { return -1 * x2 * concurrency::fast_math::log(x1 + delta) + (1 - x2) * concurrency::fast_math::log(1 - x1 + delta); }, teacher);
			return buf.ppl_sum();
		}
		/*π·Ggs[λ· PPLΐ*/
		T ppl_cross_entropy_error(FastVector<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_ppl_combo_func([=](T x1, T x2) { return -1 * x2 * std::log(x1 + delta) + (1 - x2) * std::log(1 - x1 + delta); }, teacher);
			return buf.ppl_sum();
		}

		/*π·Ggs[λ· ͺήβθ*/
		T cross_entropy_error_class(FastVector<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_combo_func([=](T x1, T x2) { return -1 * x2 * std::log(x1 + delta) }, teacher);
			return buf.sum();
		}
		/*π·Ggs[λ· ͺήβθ AMPΐ*/
		T amp_cross_entropy_error_class(FastVector<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_amp_combo_func([=](T x1, T x2) restrict(amp) { return -1 * x2 * concurrency::fast_math::log(x1 + delta) }, teacher);
			return buf.ppl_sum();
		}
		/*π·Ggs[λ· ͺήβθ PPLΐ*/
		T ppl_cross_entropy_error_class(FastVector<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_ppl_combo_func([=](T x1, T x2) { return -1 * x2 * std::log(x1 + delta) }, teacher);
			return buf.ppl_sum();
		}

		/*ΰΟ*/
		T dot(FastVector<T>& vec) {
			EXCEPTION_CHECK(size == vec.get_size(), fast_container_exception());
			T result = 0;
			for (int i = 0; i < size; i++) {
				result += this->entity[i] * vec[i];
			}
			return result;
		}
		/*ΰΟ PPLΐ*/
		T ppl_dot(FastVector<T>& vec) {
			EXCEPTION_CHECK(size == vec.get_size(), fast_container_exception());
			concurrency::combinable<T> result;
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result.local() += this->entity[i] * vec[i];
			});
			return result.combine(std::plus<T>());
		}

		/*ΦπKp
		func: T(*func)(T x)*/
		template<class F>
		FastVector<T> apply_func(F func) {
			FastVector<T> result(size);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i]);
			}
			return result;
		}
		/*ΦπKp AMPΐ
		func: T(*func)(T x)*/
		template<class F>
		FastVector<T> apply_amp_func(F func) {
			FastVector<T> result(size);
			concurrency::array_view<const T, 1> av_entity(size, &entity[0]);
			concurrency::array_view<T, 1> av_result(size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_entity.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = func(av_entity[idx]);
			});
			av_result.synchronize();
			return result;
		}
		/*ΦπKp PPLΐ
		func: T(*func)(T x)*/
		template<class F>
		FastVector<T> apply_ppl_func(F func) {
			FastVector<T> result(size);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i]);
			});
			return result;
		}

		/*ΦπKp
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastVector<T> apply_combo_func(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(size == vec.get_size(), fast_container_exception());
			FastVector<T> result(size);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i], vec[i]);
			}
			return result;
		}
		/*ΦπKp AMPΐ
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastVector<T> apply_amp_combo_func(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(size == vec.get_size(), fast_container_exception());
			FastVector<T> result(size);
			concurrency::array_view<const T, 1> av_entity(size, &entity[0]);
			concurrency::array_view<const T, 1> av_vec(size, &vec[0]);
			concurrency::array_view<T, 1> av_result(size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_entity.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = func(av_entity[idx], av_vec[idx]);
			});
			av_result.synchronize();
			return result;
		}
		/*ΦπKp PPLΐ
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastVector<T> apply_ppl_combo_func(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(size == vec.get_size(), fast_container_exception());
			FastVector<T> result(size);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i], vec[i]);
			});
			return result;
		}

		/*[size] (value1,value2,...)*/
		std::string to_string() {
			std::string result = "[" + std::to_string(size) + "](";
			for (int i = 0; i < size; i++) {
				if (i) result += "," + std::to_string(entity[i]);
				else result += std::to_string(entity[i]);
			}
			result += ")";
			return result;
		}

		/*_ΘFastVectorπΆ¬*/
		static FastVector<T> random(int size, T min = -1, T max = 1) {
			FastVector<T> result(size);
			Random<T> rnd(min, max);
			return result.apply_func([&](T x) { return rnd.generate(); });
		}
		/*_ΘFastVectorπΆ¬ PPLΐ*/
		static FastVector<T> ppl_random(int size, T min = -1, T max = 1) {
			FastVector<T> result(size);
			Random<T> rnd(min, max);
			return result.apply_ppl_func([&](T x) { return rnd.generate(); });
		}

	private:
		std::vector<T> entity;
		int size;
	};


#ifdef FAST_CONTAONER_OPERATOR_OVERLOAD_AMP_MODE

	template<typename T>
	FastVector<T> operator+(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 + x2; }, vec2); }
	template<typename T>
	FastVector<T> operator-(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 - x2; }, vec2); }
	template<typename T>
	FastVector<T> operator*(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 * x2; }, vec2); }
	template<typename T>
	FastVector<T> operator/(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 / x2; }, vec2); }
	template<typename T>
	FastVector<T> operator==(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 == x2; }, vec2); }
	template<typename T>
	FastVector<T> operator!=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 != x2; }, vec2); }
	template<typename T>
	FastVector<T> operator>(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 > x2; }, vec2); }
	template<typename T>
	FastVector<T> operator<(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 < x2; }, vec2); }
	template<typename T>
	FastVector<T> operator>=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 >= x2; }, vec2); }
	template<typename T>
	FastVector<T> operator<=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 <= x2; }, vec2); }

	template<typename T, typename N>
	FastVector<T> operator+(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val + x; }); }
	template<typename T, typename N>
	FastVector<T> operator-(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val - x; }); }
	template<typename T, typename N>
	FastVector<T> operator*(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val * x; }); }
	template<typename T, typename N>
	FastVector<T> operator/(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val / x; }); }
	template<typename T, typename N>
	FastVector<T> operator==(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val == x; }); }
	template<typename T, typename N>
	FastVector<T> operator!=(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val != x; }); }
	template<typename T, typename N>
	FastVector<T> operator>(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val > x; }); }
	template<typename T, typename N>
	FastVector<T> operator<(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val < x; }); }
	template<typename T, typename N>
	FastVector<T> operator>=(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val >= x; }); }
	template<typename T, typename N>
	FastVector<T> operator<=(N val, FastVector<T>& vec) { return vec.apply_amp_func([=](T x) restrict(amp) {return val <= x; }); }

	template<typename T, typename N>
	FastVector<T> operator+(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x + val; }); }
	template<typename T, typename N>
	FastVector<T> operator-(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x - val; }); }
	template<typename T, typename N>
	FastVector<T> operator*(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x * val; }); }
	template<typename T, typename N>
	FastVector<T> operator/(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x / val; }); }
	template<typename T, typename N>
	FastVector<T> operator==(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x == val; }); }
	template<typename T, typename N>
	FastVector<T> operator!=(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x != val; }); }
	template<typename T, typename N>
	FastVector<T> operator>(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x > val; }); }
	template<typename T, typename N>
	FastVector<T> operator<(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x < val; }); }
	template<typename T, typename N>
	FastVector<T> operator>=(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x >= val; }); }
	template<typename T, typename N>
	FastVector<T> operator<=(FastVector<T>& vec, N val) { return vec.apply_amp_func([=](T x) restrict(amp) {return x <= val; }); }

#elif defined FAST_CONTAONER_OPERATOR_OVERLOAD_PPL_MODE

	template<typename T>
	FastVector<T> operator+(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 + x2; }, vec2); }
	template<typename T>
	FastVector<T> operator-(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 - x2; }, vec2); }
	template<typename T>
	FastVector<T> operator*(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 * x2; }, vec2); }
	template<typename T>
	FastVector<T> operator/(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 / x2; }, vec2); }
	template<typename T>
	FastVector<T> operator==(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 == x2; }, vec2); }
	template<typename T>
	FastVector<T> operator!=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 != x2; }, vec2); }
	template<typename T>
	FastVector<T> operator>(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 > x2; }, vec2); }
	template<typename T>
	FastVector<T> operator<(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 < x2; }, vec2); }
	template<typename T>
	FastVector<T> operator>=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 >= x2; }, vec2); }
	template<typename T>
	FastVector<T> operator<=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_ppl_combo_func([](T x1, T x2) {return x1 <= x2; }, vec2); }

	template<typename T, typename N>
	FastVector<T> operator+(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val + x; }); }
	template<typename T, typename N>
	FastVector<T> operator-(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val - x; }); }
	template<typename T, typename N>
	FastVector<T> operator*(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val * x; }); }
	template<typename T, typename N>
	FastVector<T> operator/(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val / x; }); }
	template<typename T, typename N>
	FastVector<T> operator==(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val == x; }); }
	template<typename T, typename N>
	FastVector<T> operator!=(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val != x; }); }
	template<typename T, typename N>
	FastVector<T> operator>(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val > x; }); }
	template<typename T, typename N>
	FastVector<T> operator<(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val < x; }); }
	template<typename T, typename N>
	FastVector<T> operator>=(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val >= x; }); }
	template<typename T, typename N>
	FastVector<T> operator<=(N val, FastVector<T>& vec) { return vec.apply_ppl_func([=](T x) {return val <= x; }); }

	template<typename T, typename N>
	FastVector<T> operator+(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x + val; }); }
	template<typename T, typename N>
	FastVector<T> operator-(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x - val; }); }
	template<typename T, typename N>
	FastVector<T> operator*(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x * val; }); }
	template<typename T, typename N>
	FastVector<T> operator/(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x / val; }); }
	template<typename T, typename N>
	FastVector<T> operator==(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x == val; }); }
	template<typename T, typename N>
	FastVector<T> operator!=(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x != val; }); }
	template<typename T, typename N>
	FastVector<T> operator>(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x > val; }); }
	template<typename T, typename N>
	FastVector<T> operator<(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x < val; }); }
	template<typename T, typename N>
	FastVector<T> operator>=(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x >= val; }); }
	template<typename T, typename N>
	FastVector<T> operator<=(FastVector<T>& vec, N val) { return vec.apply_ppl_func([=](T x) {return x <= val; }); }

#else

#define FAST_CONTAONER_OPERATOR_OVERLOAD_NORMAL_MODE

	template<typename T>
	FastVector<T> operator+(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 + x2; }, vec2); }
	template<typename T>
	FastVector<T> operator-(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 - x2; }, vec2); }
	template<typename T>
	FastVector<T> operator*(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 * x2; }, vec2); }
	template<typename T>
	FastVector<T> operator/(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 / x2; }, vec2); }
	template<typename T>
	FastVector<T> operator==(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 == x2; }, vec2); }
	template<typename T>
	FastVector<T> operator!=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 != x2; }, vec2); }
	template<typename T>
	FastVector<T> operator>(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 > x2; }, vec2); }
	template<typename T>
	FastVector<T> operator<(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 < x2; }, vec2); }
	template<typename T>
	FastVector<T> operator>=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 >= x2; }, vec2); }
	template<typename T>
	FastVector<T> operator<=(FastVector<T>& vec1, FastVector<T>& vec2) { return vec1.apply_combo_func([](T x1, T x2) {return x1 <= x2; }, vec2); }

	template<typename T, typename N>
	FastVector<T> operator+(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val + x; }); }
	template<typename T, typename N>
	FastVector<T> operator-(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val - x; }); }
	template<typename T, typename N>
	FastVector<T> operator*(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val * x; }); }
	template<typename T, typename N>
	FastVector<T> operator/(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val / x; }); }
	template<typename T, typename N>
	FastVector<T> operator==(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val == x; }); }
	template<typename T, typename N>
	FastVector<T> operator!=(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val != x; }); }
	template<typename T, typename N>
	FastVector<T> operator>(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val > x; }); }
	template<typename T, typename N>
	FastVector<T> operator<(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val < x; }); }
	template<typename T, typename N>
	FastVector<T> operator>=(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val >= x; }); }
	template<typename T, typename N>
	FastVector<T> operator<=(N val, FastVector<T>& vec) { return vec.apply_func([=](T x) {return val <= x; }); }

	template<typename T, typename N>
	FastVector<T> operator+(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x + val; }); }
	template<typename T, typename N>
	FastVector<T> operator-(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x - val; }); }
	template<typename T, typename N>
	FastVector<T> operator*(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x * val; }); }
	template<typename T, typename N>
	FastVector<T> operator/(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x / val; }); }
	template<typename T, typename N>
	FastVector<T> operator==(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x == val; }); }
	template<typename T, typename N>
	FastVector<T> operator!=(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x != val; }); }
	template<typename T, typename N>
	FastVector<T> operator>(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x > val; }); }
	template<typename T, typename N>
	FastVector<T> operator<(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x < val; }); }
	template<typename T, typename N>
	FastVector<T> operator>=(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x >= val; }); }
	template<typename T, typename N>
	FastVector<T> operator<=(FastVector<T>& vec, N val) { return vec.apply_func([=](T x) {return x <= val; }); }

#endif

}
