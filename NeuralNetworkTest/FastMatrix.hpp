#pragma once

#include "FastContainerLibrary.hpp"

namespace FastContainer {

	/*
	�s��N���X
	*/
	template<typename T>
	class FastMatrix {
	public:
		FastMatrix() { }
		FastMatrix(int row, int col) { resize(row, col); }
		FastMatrix(std::vector<T> vec, int row) {
			row_size = row;
			column_size = vec.size() / row_size;
			size = row_size * column_size;
			FAST_CONTAINER_EXCEPTION_CHECK(size == vec.size(), fast_container_exception());
			entity = vec;
		}
		FastMatrix(FastVector<T>& vec, int row) {
			row_size = row;
			column_size = vec.get_size() / row_size;
			size = row_size * column_size;
			FAST_CONTAINER_EXCEPTION_CHECK(size == vec.get_size(), fast_container_exception());
			entity = vec.get_entity();
		}
		FastMatrix(std::vector<std::vector<T>> mat) {
			resize(mat.size(), mat[0].size());
			for (int i = 0; i < row_size; i++) {
				FAST_CONTAINER_EXCEPTION_CHECK(column_size == mat[i].size(), fast_container_exception());
				T offset = i * column_size;
				for (int j = 0; j < column_size; j++) {
					entity[offset + j] = mat[i][j];
				}
			}
		}
		~FastMatrix() { }

		void resize(int row, int col) {
			row_size = row;
			column_size = col;
			size = row * col;
			entity.resize(size);
		}

		std::vector<T> get_entity() { return entity; }
		int get_row_size() { return row_size; }
		int get_column_size() { return column_size; }
		int get_size() { return size; }

		T& operator[](int idx) { return entity[idx]; }
		T& operator()(int row, int col) { return entity[row * column_size + col]; }

		/*FastVector�֕ϊ�*/
		FastVector<T> to_FastVector() {
			FastVector<T> result(entity);
			return result;
		}

		/*���̂܂ܕԂ�*/
		FastMatrix<T> identity() { return this; }

		/*��Βl �������[�h�ؑ�*/
		FastMatrix<T> abs() { return SWITCH_FAST_CONTAONER_FUNCTION(abs)(); }
		/*��Βl*/
		FastMatrix<T> abs_com() { return apply_com_func([](T x) { return std::abs(x); }); }
		/*��Βl AMP����*/
		FastMatrix<T> abs_amp() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::fabs(x); }); }
		/*��Βl PPL����*/
		FastMatrix<T> abs_ppl() { return apply_ppl_func([](T x) { return std::abs(x); }); }

		/*Log e �������[�h�ؑ�*/
		FastMatrix<T> log() { return SWITCH_FAST_CONTAONER_FUNCTION(log)(); }
		/*Log e*/
		FastMatrix<T> log_com() { return apply_com_func([](T x) { return std::log(x); }); }
		/*Log e AMP����*/
		FastMatrix<T> log_amp() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::log(x); }); }
		/*Log e PPL����*/
		FastMatrix<T> log_ppl() { return apply_ppl_func([](T x) { return std::log(x); }); }

		/*Log 10 �������[�h�ؑ�*/
		FastMatrix<T> log10() { return SWITCH_FAST_CONTAONER_FUNCTION(log10)(); }
		/*Log 10*/
		FastMatrix<T> log10_com() { return apply_com_func([](T x) { return std::log10(x); }); }
		/*Log 10*/
		FastMatrix<T> log10_amp() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::log10(x); }); }
		/*Log 10*/
		FastMatrix<T> log10_ppl() { return apply_ppl_func([](T x) { return std::log10(x); }); }

		/*2�捪 �������[�h�ؑ�*/
		FastMatrix<T> sqrt() { return SWITCH_FAST_CONTAONER_FUNCTION(sqrt)(); }
		/*2�捪*/
		FastMatrix<T> sqrt_com() { return apply_com_func([](T x) { return std::sqrt(x); }); }
		/*2�捪 AMP����*/
		FastMatrix<T> sqrt_amp() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::sqrt(x); }); }
		/*2�捪 PPL����*/
		FastMatrix<T> sqrt_ppl() { return apply_ppl_func([](T x) { return std::sqrt(x); }); }

		/*�K�� �������[�h�ؑ�*/
		FastMatrix<T> pow(T exp) { return SWITCH_FAST_CONTAONER_FUNCTION(pow)(exp); }
		/*�K��*/
		FastMatrix<T> pow_com(T exp) { return apply_com_func([=](T x) { return std::pow(x, exp); }); }
		/*�K�� AMP����*/
		FastMatrix<T> pow_amp(T exp) { return apply_amp_func([=](T x) restrict(amp) { return concurrency::fast_math::pow(x, exp); }); }
		/*�K�� PPL����*/
		FastMatrix<T> pow_ppl(T exp) { return apply_ppl_func([=](T x) { return std::pow(x, exp); }); }

		/*e^x �������[�h�ؑ�*/
		FastMatrix<T> exp() { return SWITCH_FAST_CONTAONER_FUNCTION(exp)(); }
		/*e^x*/
		FastMatrix<T> exp_com() { return apply_com_func([](T x) { return std::exp(x); }); }
		/*e^x AMP����*/
		FastMatrix<T> exp_amp() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::exp(x); }); }
		/*e^x PPL����*/
		FastMatrix<T> exp_ppl() { return apply_ppl_func([](T x) { return std::exp(x); }); }

		/*�V�O���C�h�֐� �������[�h�ؑ�*/
		FastMatrix<T> sigmoid() { return SWITCH_FAST_CONTAONER_FUNCTION(sigmoid)(); }
		/*�V�O���C�h�֐�*/
		FastMatrix<T> sigmoid_com() { return apply_com_func([](T x) { return (T)1 / (1 + std::exp(-x)); }); }
		/*�V�O���C�h�֐� AMP����*/
		FastMatrix<T> sigmoid_amp() { return apply_amp_func([](T x) restrict(amp) { return (T)1 / (1 + concurrency::fast_math::exp(-x)); }); }
		/*�V�O���C�h�֐� PPL����*/
		FastMatrix<T> sigmoid_ppl() { return apply_ppl_func([](T x) { return (T)1 / (1 + std::exp(-x)); }); }

		/*ReLU�֐� �������[�h�ؑ�*/
		FastMatrix<T> relu() { return SWITCH_FAST_CONTAONER_FUNCTION(relu)(); }
		/*ReLU�֐�*/
		FastMatrix<T> relu_com() { return apply_com_func([](T x) { return x > 0 ? x : 0; }); }
		/*ReLU�֐� AMP����*/
		FastMatrix<T> relu_amp() { return apply_amp_func([](T x) restrict(amp) { return x > 0 ? x : 0; }); }
		/*ReLU�֐� PPL����*/
		FastMatrix<T> relu_ppl() { return apply_ppl_func([](T x) { return x > 0 ? x : 0; }); }

		/*���K�� �������[�h�ؑ�*/
		FastMatrix<T> normalization() { return SWITCH_FAST_CONTAONER_FUNCTION(normalization)(); }
		/*���K��*/
		FastMatrix<T> normalization_com() {
			T max = get_max();
			return apply_com_func([=](T x) { return x / max; });
		}
		/*���K�� AMP����*/
		FastMatrix<T> normalization_amp() {
			T max = get_max();
			return apply_amp_func([=](T x) restrict(amp) { return x / max; });
		}
		/*���K�� PPL����*/
		FastMatrix<T> normalization_ppl() {
			T max = get_max();
			return apply_ppl_func([=](T x) { return x / max; });
		}

		/*�\�t�g�}�b�N�X�֐� �������[�h�ؑ�*/
		FastMatrix<T> softmax() { return SWITCH_FAST_CONTAONER_FUNCTION(softmax)(); }
		/*�\�t�g�}�b�N�X�֐�*/
		FastMatrix<T> softmax_com() {
			T max = get_max();
			auto buf = apply_com_func([=](T x) { return std::exp(x - max); });
			return buf.div_by_columns_com(buf.sum_by_rows_com());
		}
		/*�\�t�g�}�b�N�X�֐� AMP����*/
		FastMatrix<T> softmax_amp() {
			T max = get_max();
			auto buf = apply_amp_func([=](T x) restrict(amp) { return concurrency::fast_math::exp(x - max); });
			return buf.div_by_columns_amp(buf.sum_by_rows_amp());
		}
		/*�\�t�g�}�b�N�X�֐� PPL����*/
		FastMatrix<T> softmax_ppl() {
			T max = get_max();
			auto buf = apply_ppl_func([=](T x) { return std::exp(x - max); });
			return buf.div_by_columns_ppl(buf.sum_by_rows_ppl());
		}

		/*���l���� �������[�h�ؑ�
		func: (T)(*func)(T x)*/
		template<class F>
		FastMatrix<T> num_diff(F func, T delta = 0.0001) { return SWITCH_FAST_CONTAONER_FUNCTION(num_diff)(func, delta); }
		/*���l����
		func: (T)(*func)(T x)*/
		template<class F>
		FastMatrix<T> num_diff_com(F func, T delta = 0.0001) { return apply_com_func([=](T x) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); }
		/*���l���� AMP����
		func: (T)(*func)(T x) restrict(amp)*/
		template<class F>
		FastMatrix<T> num_diff_amp(F func, T delta = 0.0001) { return apply_amp_func([=](T x) restrict(amp) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); }
		/*���l���� PPL����
		func: (T)(*func)(T x)*/
		template<class F>
		FastMatrix<T> num_diff_ppl(F func, T delta = 0.0001) { return apply_ppl_func([=](T x) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); }

		/*�e�s�֑Ώۂ����Z �������[�h�ؑ�*/
		FastMatrix<T> add_by_rows(FastVector<T>& vec) { return SWITCH_FAST_CONTAONER_FUNCTION(add_by_rows)(vec); }
		/*�e�s�֑Ώۂ����Z*/
		FastMatrix<T> add_by_rows_com(FastVector<T>& vec) { return apply_com_combo_func_by_rows([](T x1, T x2) {return x1 + x2; }, vec); }
		/*�e�s�֑Ώۂ����Z AMP����*/
		FastMatrix<T> add_by_rows_amp(FastVector<T>& vec) { return apply_amp_combo_func_by_rows([](T x1, T x2) restrict(amp) {return x1 + x2; }, vec); }
		/*�e�s�֑Ώۂ����Z PPL����*/
		FastMatrix<T> add_by_rows_ppl(FastVector<T>& vec) { return apply_ppl_combo_func_by_rows([](T x1, T x2) {return x1 + x2; }, vec); }

		/*�e��֑Ώۂ����Z �������[�h�ؑ�*/
		FastMatrix<T> add_by_columns(FastVector<T>& vec) { return SWITCH_FAST_CONTAONER_FUNCTION(add_by_columns)(vec); }
		/*�e��֑Ώۂ����Z*/
		FastMatrix<T> add_by_columns_com(FastVector<T>& vec) { return apply_com_combo_func_by_columns([](T x1, T x2) {return x1 + x2; }, vec); }
		/*�e��֑Ώۂ����Z AMP����*/
		FastMatrix<T> add_by_columns_amp(FastVector<T>& vec) { return apply_amp_combo_func_by_columns([](T x1, T x2) restrict(amp) {return x1 + x2; }, vec); }
		/*�e��֑Ώۂ����Z PPL����*/
		FastMatrix<T> add_by_columns_ppl(FastVector<T>& vec) { return apply_ppl_combo_func_by_columns([](T x1, T x2) {return x1 + x2; }, vec); }

		/*�e�s����Ώۂ����Z �������[�h�ؑ�*/
		FastMatrix<T> sub_by_rows(FastVector<T>& vec) { return SWITCH_FAST_CONTAONER_FUNCTION(sub_by_rows)(vec); }
		/*�e�s����Ώۂ����Z*/
		FastMatrix<T> sub_by_rows_com(FastVector<T>& vec) { return apply_com_combo_func_by_rows([](T x1, T x2) {return x1 - x2; }, vec); }
		/*�e�s����Ώۂ����Z AMP����*/
		FastMatrix<T> sub_by_rows_amp(FastVector<T>& vec) { return apply_amp_combo_func_by_rows([](T x1, T x2) restrict(amp) {return x1 - x2; }, vec); }
		/*�e�s����Ώۂ����Z PPL����*/
		FastMatrix<T> sub_by_rows_ppl(FastVector<T>& vec) { return apply_ppl_combo_func_by_rows([](T x1, T x2) {return x1 - x2; }, vec); }

		/*�e�񂩂�Ώۂ����Z �������[�h�ؑ�*/
		FastMatrix<T> sub_by_columns(FastVector<T>& vec) { return SWITCH_FAST_CONTAONER_FUNCTION(sub_by_columns)(vec); }
		/*�e�񂩂�Ώۂ����Z*/
		FastMatrix<T> sub_by_columns_com(FastVector<T>& vec) { return apply_com_combo_func_by_columns([](T x1, T x2) {return x1 - x2; }, vec); }
		/*�e�񂩂�Ώۂ����Z AMP����*/
		FastMatrix<T> sub_by_columns_amp(FastVector<T>& vec) { return apply_amp_combo_func_by_columns([](T x1, T x2) restrict(amp) {return x1 - x2; }, vec); }
		/*�e�񂩂�Ώۂ����Z PPL����*/
		FastMatrix<T> sub_by_columns_ppl(FastVector<T>& vec) { return apply_ppl_combo_func_by_columns([](T x1, T x2) {return x1 - x2; }, vec); }

		/*�e�s�֑Ώۂ���Z �������[�h�ؑ�*/
		FastMatrix<T> mul_by_rows(FastVector<T>& vec) { return SWITCH_FAST_CONTAONER_FUNCTION(mul_by_rows)(vec); }
		/*�e�s�֑Ώۂ���Z*/
		FastMatrix<T> mul_by_rows_com(FastVector<T>& vec) { return apply_com_combo_func_by_rows([](T x1, T x2) {return x1 * x2; }, vec); }
		/*�e�s�֑Ώۂ���Z AMP����*/
		FastMatrix<T> mul_by_rows_amp(FastVector<T>& vec) { return apply_amp_combo_func_by_rows([](T x1, T x2) restrict(amp) {return x1 * x2; }, vec); }
		/*�e�s�֑Ώۂ���Z PPL����*/
		FastMatrix<T> mul_by_rows_ppl(FastVector<T>& vec) { return apply_ppl_combo_func_by_rows([](T x1, T x2) {return x1 * x2; }, vec); }

		/*�e��֑Ώۂ���Z �������[�h�ؑ�*/
		FastMatrix<T> mul_by_columns(FastVector<T>& vec) { return SWITCH_FAST_CONTAONER_FUNCTION(mul_by_columns)(vec); }
		/*�e��֑Ώۂ���Z*/
		FastMatrix<T> mul_by_columns_com(FastVector<T>& vec) { return apply_com_combo_func_by_columns([](T x1, T x2) {return x1 * x2; }, vec); }
		/*�e��֑Ώۂ���Z AMP����*/
		FastMatrix<T> mul_by_columns_amp(FastVector<T>& vec) { return apply_amp_combo_func_by_columns([](T x1, T x2) restrict(amp) {return x1 * x2; }, vec); }
		/*�e��֑Ώۂ���Z PPL����*/
		FastMatrix<T> mul_by_columns_ppl(FastVector<T>& vec) { return apply_ppl_combo_func_by_columns([](T x1, T x2) {return x1 * x2; }, vec); }

		/*�e�s����Ώۂ����Z �������[�h�ؑ�*/
		FastMatrix<T> div_by_rows(FastVector<T>& vec) { return SWITCH_FAST_CONTAONER_FUNCTION(div_by_rows)(vec); }
		/*�e�s����Ώۂ����Z*/
		FastMatrix<T> div_by_rows_com(FastVector<T>& vec) { return apply_com_combo_func_by_rows([](T x1, T x2) {return x1 / x2; }, vec); }
		/*�e�s����Ώۂ����Z AMP����*/
		FastMatrix<T> div_by_rows_amp(FastVector<T>& vec) { return apply_amp_combo_func_by_rows([](T x1, T x2) restrict(amp) {return x1 / x2; }, vec); }
		/*�e�s����Ώۂ����Z PPL����*/
		FastMatrix<T> div_by_rows_ppl(FastVector<T>& vec) { return apply_ppl_combo_func_by_rows([](T x1, T x2) {return x1 / x2; }, vec); }

		/*�e�񂩂�Ώۂ����Z �������[�h�ؑ�*/
		FastMatrix<T> div_by_columns(FastVector<T>& vec) { return SWITCH_FAST_CONTAONER_FUNCTION(div_by_columns)(vec); }
		/*�e�񂩂�Ώۂ����Z*/
		FastMatrix<T> div_by_columns_com(FastVector<T>& vec) { return apply_com_combo_func_by_columns([](T x1, T x2) {return x1 / x2; }, vec); }
		/*�e�񂩂�Ώۂ����Z AMP����*/
		FastMatrix<T> div_by_columns_amp(FastVector<T>& vec) { return apply_amp_combo_func_by_columns([](T x1, T x2) restrict(amp) {return x1 / x2; }, vec); }
		/*�e�񂩂�Ώۂ����Z PPL����*/
		FastMatrix<T> div_by_columns_ppl(FastVector<T>& vec) { return apply_ppl_combo_func_by_columns([](T x1, T x2) {return x1 / x2; }, vec); }

		/*�����G���g���s�[�덷 �������[�h�ؑ�*/
		T cross_entropy_error(FastMatrix<T>& teacher, T delta = 0.0000001) { return SWITCH_FAST_CONTAONER_FUNCTION(cross_entropy_error)(teacher, delta); }
		/*�����G���g���s�[�덷*/
		T cross_entropy_error_com(FastMatrix<T>& teacher, T delta = 0.0000001) {
			auto buf = apply_com_combo_func([=](T x1, T x2) { return (T)-1 * x2 * std::log(x1 + delta) + (1 - x2) * std::log(1 - x1 + delta); }, teacher);
			return buf.sum();
		}
		/*�����G���g���s�[�덷 AMP����*/
		T cross_entropy_error_amp(FastMatrix<T>& teacher, T delta = 0.0000001) {
			auto buf = apply_amp_combo_func([=](T x1, T x2) restrict(amp) { return (T)-1 * x2 * concurrency::fast_math::log(x1 + delta) + (1 - x2) * concurrency::fast_math::log(1 - x1 + delta); }, teacher);
			return buf.sum();
		}
		/*�����G���g���s�[�덷 PPL����*/
		T cross_entropy_error_ppl(FastMatrix<T>& teacher, T delta = 0.0000001) {
			auto buf = apply_ppl_combo_func([=](T x1, T x2) { return (T)-1 * x2 * std::log(x1 + delta) + (1 - x2) * std::log(1 - x1 + delta); }, teacher);
			return buf.sum();
		}

		/*�����G���g���s�[�덷 ���ޖ�� �������[�h�ؑ�*/
		T cross_entropy_error_class(FastMatrix<T>& teacher, T delta = 0.0000001) { return SWITCH_FAST_CONTAONER_FUNCTION(cross_entropy_error_class)(teacher, delta); }
		/*�����G���g���s�[�덷 ���ޖ��*/
		T cross_entropy_error_class_com(FastMatrix<T>& teacher, T delta = 0.0000001) {
			auto buf = apply_com_combo_func([=](T x1, T x2) { return (T)-1 * x2 * std::log(x1 + delta); }, teacher);
			return buf.sum() / row_size;
		}
		/*�����G���g���s�[�덷 ���ޖ�� AMP����*/
		T cross_entropy_error_class_amp(FastMatrix<T>& teacher, T delta = 0.0000001) {
			auto buf = apply_amp_combo_func([=](T x1, T x2) restrict(amp) { return (T)-1 * x2 * concurrency::fast_math::log(x1 + delta); }, teacher);
			return buf.sum() / row_size;
		}
		/*�����G���g���s�[�덷 ���ޖ�� PPL����*/
		T cross_entropy_error_class_ppl(FastMatrix<T>& teacher, T delta = 0.0000001) {
			auto buf = apply_ppl_combo_func([=](T x1, T x2) { return (T)-1 * x2 * std::log(x1 + delta); }, teacher);
			return buf.sum() / row_size;
		}

		/*�ŏ��l*/
		T get_min() {
			T result = entity[0];
			for (auto x : entity) result = min(result, x);
			return result;
		}
		/*�ő�l*/
		T get_max() {
			T result = entity[0];
			for (auto x : entity) result = max(result, x);
			return result;
		}
		/*�ő�l�̃C���f�b�N�X*/
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

		/*���v*/
		T sum() {
			T result = 0;
			for (auto x : entity) result += x;
			return result;
		}

		/*����*/
		T mean() {
			return sum() / size;
		}

		/*�s���̍ŏ��l �������[�h�ؑ�*/
		FastVector<T> min_by_rows() { return SWITCH_FAST_CONTAONER_FUNCTION(min_by_rows)(); }
		/*�s���̍ŏ��l*/
		FastVector<T> min_by_rows_com() {
			FastVector<T> result(row_size);
			for (int i = 0; i < row_size; i++) {
				int offset = i * column_size;
				result[i] = entity[offset];
				for (int j = 0; j < column_size; j++) {
					T b_result = result[i];
					T b_entity = entity[offset + j];
					result[i] = min(b_result, b_entity);
				}
			}
			return result;
		}
		/*�s���̍ŏ��l AMP����*/
		FastVector<T> min_by_rows_amp() {
			FastVector<T> result(row_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(row_size, &result[0]);
			int b_col = column_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = av_entity[idx[0]][0];
				for (int i = 0; i < b_col; i++) {
					T b_result = av_result[idx];
					T b_entity = av_entity[idx[0]][i];
					av_result[idx] = min(b_result, b_entity);
				}
			});
			av_result.synchronize();
			return result;
		}
		/*�s���̍ŏ��l PPL����*/
		FastVector<T> min_by_rows_ppl() {
			FastVector<T> result(row_size);
			concurrency::parallel_for<int>(0, row_size, [&](int i) {
				int offset = i * column_size;
				result[i] = entity[offset];
				for (int j = 0; j < column_size; j++) {
					T b_result = result[i];
					T b_entity = entity[offset + j];
					result[i] = min(b_result, b_entity);
				}
			});
			return result;
		}

		/*�񖈂̍ŏ��l �������[�h�ؑ�*/
		FastVector<T> min_by_columns() { return SWITCH_FAST_CONTAONER_FUNCTION(min_by_columns)(); }
		/*�񖈂̍ŏ��l*/
		FastVector<T> min_by_columns_com() {
			FastVector<T> result(column_size);
			for (int i = 0; i < column_size; i++) {
				result[i] = entity[i];
				for (int j = 0; j < row_size; j++) {
					T b_result = result[i];
					T b_entity = entity[j * column_size + i];
					result[i] = min(b_result, b_entity);
				}
			}
			return result;
		}
		/*�񖈂̍ŏ��l AMP����*/
		FastVector<T> min_by_columns_amp() {
			FastVector<T> result(column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(column_size, &result[0]);
			int b_row = row_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = av_entity[0][idx[0]];
				for (int i = 0; i < b_row; i++) {
					T b_result = av_result[idx];
					T b_entity = av_entity[i][idx[0]];
					av_result[idx] = min(b_result, b_entity);
				}
			});
			av_result.synchronize();
			return result;
		}
		/*�񖈂̍ŏ��l PPL����*/
		FastVector<T> min_by_columns_ppl() {
			FastVector<T> result(column_size);
			concurrency::parallel_for<int>(0, column_size, [&](int i) {
				result[i] = entity[i];
				for (int j = 0; j < row_size; j++) {
					T b_result = result[i];
					T b_entity = entity[j * column_size + i];
					result[i] = min(b_result, b_entity);
				}
			});
			return result;
		}

		/*�s���̍ő�l �������[�h�ؑ�*/
		FastVector<T> max_by_rows() { return SWITCH_FAST_CONTAONER_FUNCTION(max_by_rows)(); }
		/*�s���̍ő�l*/
		FastVector<T> max_by_rows_com() {
			FastVector<T> result(row_size);
			for (int i = 0; i < row_size; i++) {
				int offset = i * column_size;
				result[i] = entity[offset];
				for (int j = 0; j < column_size; j++) {
					T b_result = result[i];
					T b_entity = entity[offset + j];
					result[i] = max(b_result, b_entity);
				}
			}
			return result;
		}
		/*�s���̍ő�l AMP����*/
		FastVector<T> max_by_rows_amp() {
			FastVector<T> result(row_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(row_size, &result[0]);
			int b_col = column_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = av_entity[idx[0]][0];
				for (int i = 0; i < b_col; i++) {
					T b_result = av_result[idx];
					T b_entity = av_entity[idx[0]][i];
					av_result[idx] = max(b_result, b_entity);
				}
			});
			av_result.synchronize();
			return result;
		}
		/*�s���̍ő�l PPL����*/
		FastVector<T> max_by_rows_ppl() {
			FastVector<T> result(row_size);
			concurrency::parallel_for<int>(0, row_size, [&](int i) {
				int offset = i * column_size;
				result[i] = entity[offset];
				for (int j = 0; j < column_size; j++) {
					T b_result = result[i];
					T b_entity = entity[offset + j];
					result[i] = max(b_result, b_entity);
				}
			});
			return result;
		}

		/*�񖈂̍ő�l �������[�h�ؑ�*/
		FastVector<T> max_by_columns() { return SWITCH_FAST_CONTAONER_FUNCTION(max_by_columns)(); }
		/*�񖈂̍ő�l*/
		FastVector<T> max_by_columns_com() {
			FastVector<T> result(column_size);
			for (int i = 0; i < column_size; i++) {
				result[i] = entity[i];
				for (int j = 0; j < row_size; j++) {
					T b_result = result[i];
					T b_entity = entity[j * column_size + i];
					result[i] = max(b_result, b_entity);
				}
			}
			return result;
		}
		/*�񖈂̍ő�l AMP����*/
		FastVector<T> max_by_columns_amp() {
			FastVector<T> result(column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(column_size, &result[0]);
			int b_row = row_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = av_entity[0][idx[0]];
				for (int i = 0; i < b_row; i++) {
					T b_result = av_result[idx];
					T b_entity = av_entity[i][idx[0]];
					av_result[idx] = max(b_result, b_entity);
				}
			});
			av_result.synchronize();
			return result;
		}
		/*�񖈂̍ő�l PPL����*/
		FastVector<T> max_by_columns_ppl() {
			FastVector<T> result(column_size);
			concurrency::parallel_for<int>(0, column_size, [&](int i) {
				result[i] = entity[i];
				for (int j = 0; j < row_size; j++) {
					T b_result = result[i];
					T b_entity = entity[j * column_size + i];
					result[i] = max(b_result, b_entity);
				}
			});
			return result;
		}

		/*�s���̍ő�l�̃C���f�b�N�X �������[�h�ؑ�*/
		FastVector<T> argmax_by_rows() { return SWITCH_FAST_CONTAONER_FUNCTION(argmax_by_rows)(); }
		/*�s���̍ő�l�̃C���f�b�N�X*/
		FastVector<T> argmax_by_rows_com() {
			FastVector<T> result(row_size);
			for (int i = 0; i < row_size; i++) {
				int offset = i * column_size;
				T max = entity[offset];
				for (int j = 0; j < column_size; j++) {
					T b_entity = entity[offset + j];
					if (max < b_entity) {
						max = b_entity;
						result[i] = j;
					}
				}
			}
			return result;
		}
		/*�s���̍ő�l�̃C���f�b�N�X AMP����*/
		FastVector<T> argmax_by_rows_amp() {
			FastVector<T> result(row_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(row_size, &result[0]);
			int b_col = column_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				T max = av_entity[idx[0]][0];
				for (int i = 0; i < b_col; i++) {
					T b_entity = av_entity[idx[0]][i];
					if (max < b_entity) {
						max = b_entity;
						av_result[idx] = i;
					}
				}
			});
			av_result.synchronize();
			return result;
		}
		/*�s���̍ő�l�̃C���f�b�N�X PPL����*/
		FastVector<T> argmax_by_rows_ppl() {
			FastVector<T> result(row_size);
			concurrency::parallel_for<int>(0, row_size, [&](int i) {
				int offset = i * column_size;
				T max = entity[offset];
				for (int j = 0; j < column_size; j++) {
					T b_entity = entity[offset + j];
					if (max < b_entity) {
						max = b_entity;
						result[i] = j;
					}
				}
			});
			return result;
		}

		/*�񖈂̍ő�l�̃C���f�b�N�X �������[�h�ؑ�*/
		FastVector<T> argmax_by_columns() { return SWITCH_FAST_CONTAONER_FUNCTION(argmax_by_columns)(); }
		/*�񖈂̍ő�l�̃C���f�b�N�X*/
		FastVector<T> argmax_by_columns_com() {
			FastVector<T> result(column_size);
			for (int i = 0; i < column_size; i++) {
				T max = entity[i];
				for (int j = 0; j < row_size; j++) {
					T b_entity = entity[j * column_size + i];
					if (max < b_entity) {
						max = b_entity;
						result[i] = j;
					}
				}
			}
			return result;
		}
		/*�񖈂̍ő�l�̃C���f�b�N�X AMP����*/
		FastVector<T> argmax_by_columns_amp() {
			FastVector<T> result(column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(column_size, &result[0]);
			int b_row = row_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				T max = av_entity[0][idx[0]];
				for (int i = 0; i < b_row; i++) {
					T b_entity = av_entity[i][idx[0]];
					if (max < b_entity) {
						max = b_entity;
						av_result[idx] = i;
					}
				}
			});
			av_result.synchronize();
			return result;
		}
		/*�񖈂̍ő�l�̃C���f�b�N�X PPL����*/
		FastVector<T> argmax_by_columns_ppl() {
			FastVector<T> result(column_size);
			concurrency::parallel_for<int>(0, column_size, [&](int i) {
				T max = entity[i];
				for (int j = 0; j < row_size; j++) {
					T b_entity = entity[j * column_size + i];
					if (max < b_entity) {
						max = b_entity;
						result[i] = j;
					}
				}
			});
			return result;
		}

		/*�s���̍��v �������[�h�ؑ�*/
		FastVector<T> sum_by_rows() { return SWITCH_FAST_CONTAONER_FUNCTION(sum_by_rows)(); }
		/*�s���̍��v*/
		FastVector<T> sum_by_rows_com() {
			FastVector<T> result(row_size);
			for (int i = 0; i < row_size; i++) {
				int offset = i * column_size;
				for (int j = 0; j < column_size; j++) {
					result[i] += entity[offset + j];
				}
			}
			return result;
		}
		/*�s���̍��v AMP����*/
		FastVector<T> sum_by_rows_amp() {
			FastVector<T> result(row_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(row_size, &result[0]);
			int b_col = column_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				for (int i = 0; i < b_col; i++) {
					av_result[idx] += av_entity[idx[0]][i];
				}
			});
			av_result.synchronize();
			return result;
		}
		/*�s���̍��v PPL����*/
		FastVector<T> sum_by_rows_ppl() {
			FastVector<T> result(row_size);
			concurrency::parallel_for<int>(0, row_size, [&](int i) {
				int offset = i * column_size;
				for (int j = 0; j < column_size; j++) {
					result[i] += entity[offset + j];
				}
			});
			return result;
		}

		/*�񖈂̍��v �������[�h�ؑ�*/
		FastVector<T> sum_by_columns() { return SWITCH_FAST_CONTAONER_FUNCTION(sum_by_columns)(); }
		/*�񖈂̍��v*/
		FastVector<T> sum_by_columns_com() {
			FastVector<T> result(column_size);
			for (int i = 0; i < column_size; i++) {
				for (int j = 0; j < row_size; j++) {
					result[i] += entity[j * column_size + i];
				}
			}
			return result;
		}
		/*�񖈂̍��v AMP����*/
		FastVector<T> sum_by_columns_amp() {
			FastVector<T> result(column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(column_size, &result[0]);
			int b_row = row_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				for (int i = 0; i < b_row; i++) {
					av_result[idx] += av_entity[i][idx[0]];
				}
			});
			av_result.synchronize();
			return result;
		}
		/*�񖈂̍��v PPL����*/
		FastVector<T> sum_by_columns_ppl() {
			FastVector<T> result(column_size);
			concurrency::parallel_for<int>(0, column_size, [&](int i) {
				for (int j = 0; j < row_size; j++) {
					result[i] += entity[j * column_size + i];
				}
			});
			return result;
		}

		/*�s���̕��� �������[�h�ؑ�*/
		FastVector<T> mean_by_rows() { return SWITCH_FAST_CONTAONER_FUNCTION(mean_by_rows)(); }
		/*�s���̕���*/
		FastVector<T> mean_by_rows_com() {
			FastVector<T> result(row_size);
			for (int i = 0; i < row_size; i++) {
				int offset = i * column_size;
				for (int j = 0; j < column_size; j++) {
					result[i] += entity[offset + j];
				}
				result[i] /= column_size;
			}
			return result;
		}
		/*�s���̕��� AMP����*/
		FastVector<T> mean_by_rows_amp() {
			FastVector<T> result(row_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(row_size, &result[0]);
			int b_col = column_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				for (int i = 0; i < b_col; i++) {
					av_result[idx] += av_entity[idx[0]][i];
				}
				av_result[idx] /= b_col;
			});
			av_result.synchronize();
			return result;
		}
		/*�s���̕��� PPL����*/
		FastVector<T> mean_by_rows_ppl() {
			FastVector<T> result(row_size);
			concurrency::parallel_for<int>(0, row_size, [&](int i) {
				int offset = i * column_size;
				for (int j = 0; j < column_size; j++) {
					result[i] += entity[offset + j];
				}
				result[i] /= column_size;
			});
			return result;
		}

		/*�񖈂̕��� �������[�h�ؑ�*/
		FastVector<T> mean_by_columns() { return SWITCH_FAST_CONTAONER_FUNCTION(mean_by_columns)(); }
		/*�񖈂̕���*/
		FastVector<T> mean_by_columns_com() {
			FastVector<T> result(column_size);
			for (int i = 0; i < column_size; i++) {
				for (int j = 0; j < row_size; j++) {
					result[i] += entity[j * column_size + i];
				}
				result[i] /= row_size;
			}
			return result;
		}
		/*�񖈂̕��� AMP����*/
		FastVector<T> mean_by_columns_amp() {
			FastVector<T> result(column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(column_size, &result[0]);
			int b_row = row_size;
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				for (int i = 0; i < b_row; i++) {
					av_result[idx] += av_entity[i][idx[0]];
				}
				av_result[idx] /= b_row;
			});
			av_result.synchronize();
			return result;
		}
		/*�񖈂̕��� PPL����*/
		FastVector<T> mean_by_columns_ppl() {
			FastVector<T> result(column_size);
			concurrency::parallel_for<int>(0, column_size, [&](int i) {
				for (int j = 0; j < row_size; j++) {
					result[i] += entity[j * column_size + i];
				}
				result[i] /= row_size;
			});
			return result;
		}

		/*���� �������[�h�ؑ�*/
		FastMatrix<T> dot(FastMatrix<T>& mat) { return SWITCH_FAST_CONTAONER_FUNCTION(dot)(mat); }
		/*����*/
		FastMatrix<T> dot_com(FastMatrix<T>& mat) {
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == mat.get_row_size(), fast_container_exception());
			int mat_col = mat.get_column_size();
			FastMatrix<T> result(row_size, mat_col);
			for (int i = 0; i < row_size; i++) {
				int res_offset = i * mat_col;
				int ent_pos = i * column_size;
				int mat_offset = 0;
				for (int k = 0; k < column_size; k++, ent_pos++, mat_offset += mat_col) {
					for (int j = 0; j < mat_col; j++) {
						result[res_offset + j] += entity[ent_pos] * mat[mat_offset + j];
					}
				}
			}
			return result;
		}
		/*���� AMP����*/
		FastMatrix<T> dot_amp(FastMatrix<T>& mat) {
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == mat.get_row_size(), fast_container_exception());
			int col = mat.get_column_size();
			int mid = column_size;
			FastMatrix<T> result(row_size, col);
			concurrency::array_view<const T, 2> av_entity(row_size, mid, &entity[0]);
			concurrency::array_view<const T, 2> av_mat(mid, col, &mat[0]);
			concurrency::array_view<T, 2> av_result(row_size, col, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<2> idx) restrict(amp) {
				for (int i = 0; i < mid; i++) {
					av_result[idx] += av_entity[idx[0]][i] * av_mat[i][idx[1]];
				}
			});
			av_result.synchronize();
			return result;
		}
		/*���� PPL����*/
		FastMatrix<T> dot_ppl(FastMatrix<T>& mat) {
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == mat.get_row_size(), fast_container_exception());
			int mat_col = mat.get_column_size();
			FastMatrix<T> result(row_size, mat_col);
			concurrency::parallel_for<int>(0, row_size, [&](int i) {
				int res_offset = i * mat_col;
				int ent_pos = i * column_size;
				int mat_offset = 0;
				for (int k = 0; k < column_size; k++, ent_pos++, mat_offset += mat_col) {
					for (int j = 0; j < mat_col; j++) {
						result[res_offset + j] += entity[ent_pos] * mat[mat_offset + j];
					}
				}
			});
			return result;
		}

		/*�]�u�s�� �������[�h�ؑ�*/
		FastMatrix<T> reverse() { return SWITCH_FAST_CONTAONER_FUNCTION(reverse)(); }
		/*�]�u�s��*/
		FastMatrix<T> reverse_com() {
			FastMatrix<T> result(column_size, row_size);
			for (int i = 0; i < row_size; i++) {
				int offset = i * column_size;
				for (int j = 0; j < column_size; j++) {
					result(j, i) = entity[offset + j];
				}
			}
			return result;
		}
		/*�]�u�s�� AMP����*/
		FastMatrix<T> reverse_amp() {
			FastMatrix<T> result(column_size, row_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 2> av_result(column_size, row_size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<2> idx) restrict(amp) {
				av_result[idx] = av_entity[idx[1]][idx[0]];
			});
			av_result.synchronize();
			return result;
		}
		/*�]�u�s�� PPL����*/
		FastMatrix<T> reverse_ppl() {
			FastMatrix<T> result(column_size, row_size);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = entity[(i % row_size) * column_size + i / row_size];
			});
			return result;
		}

		/*�w��s���擾 �������[�h�ؑ�*/
		FastVector<T> row(int row) { return SWITCH_FAST_CONTAONER_FUNCTION(row)(row); }
		/*�w��s���擾*/
		FastVector<T> row_com(int row) {
			FAST_CONTAINER_EXCEPTION_CHECK(row >= row_size, fast_container_exception());
			FastVector<T> result(column_size);
			for (int i = 0, j = row * column_size; i < column_size; i++, j++) {
				result[i] = entity[j];
			}
			return result;
		}
		/*�w��s���擾 AMP����*/
		FastVector<T> row_amp(int row) {
			FAST_CONTAINER_EXCEPTION_CHECK(row >= row_size, fast_container_exception());
			FastVector<T> result(column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(column_size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = av_entity[row][idx[0]];
			});
			av_result.synchronize();
			return result;
		}
		/*�w��s���擾 PPL����*/
		FastVector<T> row_ppl(int row) {
			FAST_CONTAINER_EXCEPTION_CHECK(row >= row_size, fast_container_exception());
			FastVector<T> result(column_size);
			concurrency::parallel_for<int>(0, column_size, [&](int i) {
				result[i] = entity[row * column_size + i];
			});
			return result;
		}

		/*�w�����擾 �������[�h�ؑ�*/
		FastVector<T> column(int col) { return SWITCH_FAST_CONTAONER_FUNCTION(column)(col); }
		/*�w�����擾*/
		FastVector<T> column_com(int col) {
			FAST_CONTAINER_EXCEPTION_CHECK(col >= column_size, fast_container_exception());
			FastVector<T> result(row_size);
			for (int i = 0, j = col; i < row_size; i++, j += column_size) {
				result[i] = entity[j];
			}
			return result;
		}
		/*�w�����擾 AMP����*/
		FastVector<T> column_amp(int col) {
			FAST_CONTAINER_EXCEPTION_CHECK(col >= column_size, fast_container_exception());
			FastVector<T> result(row_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<T, 1> av_result(row_size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = av_entity[idx[0]][col];
			});
			av_result.synchronize();
			return result;
		}
		/*�w�����擾 PPL����*/
		FastVector<T> column_ppl(int col) {
			FAST_CONTAINER_EXCEPTION_CHECK(col >= column_size, fast_container_exception());
			FastVector<T> result(row_size);
			concurrency::parallel_for<int>(0, row_size, [&](int i) {
				result[i] = entity[i * column_size + col];
			});
			return result;
		}

		/*�o�b�`�̎擾 �������[�h�ؑ�*/
		FastMatrix<T> batch(FastVector<int>& mask) { return SWITCH_FAST_CONTAONER_FUNCTION(batch)(mask); }
		/*�o�b�`�̎擾*/
		FastMatrix<T> batch_com(FastVector<int>& mask) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size >= mask.get_size(), fast_container_exception());
			int row = mask.get_size();
			FastMatrix<T> result(row, column_size);
			for (int i = 0; i < row; i++) {
				int r_offset = i * column_size;
				int e_offset = mask[i] * column_size;
				for (int j = 0; j < column_size; j++) {
					result[r_offset + j] = entity[e_offset + j];
				}
			}
			return result;
		}
		/*�o�b�`�̎擾 AMP����*/
		FastMatrix<T> batch_amp(FastVector<int>& mask) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size >= mask.get_size(), fast_container_exception());
			int row = mask.get_size();
			FastMatrix<T> result(row, column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<const int, 1> av_mask(row, &mask[0]);
			concurrency::array_view<T, 2> av_result(row, column_size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<2> idx) restrict(amp) {
				av_result[idx] = av_entity[av_mask[idx[0]]][idx[1]];
			});
			av_result.synchronize();
			return result;
		}
		/*�o�b�`�̎擾 PPL����*/
		FastMatrix<T> batch_ppl(FastVector<int>& mask) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size >= mask.get_size(), fast_container_exception());
			int row = mask.get_size();
			int res_size = row * column_size;
			FastMatrix<T> result(row, column_size);
			concurrency::parallel_for<int>(0, res_size, [&](int i) {
				result[i] = entity[mask[i / column_size] * column_size + (i % column_size)];
			});
			return result;
		}

		/*�����_���ȃo�b�`�̎擾 �������[�h�ؑ�*/
		FastMatrix<T> random_batch(int size) { return SWITCH_FAST_CONTAONER_FUNCTION(random_batch)(size); }
		/*�����_���ȃo�b�`�̎擾*/
		FastMatrix<T> random_batch_com(int size) { return batch_com(FastVector<int>::int_hash_random(size, 0, row_size - 1)); }
		/*�����_���ȃo�b�`�̎擾 AMP����*/
		FastMatrix<T> random_batch_amp(int size) { return batch_amp(FastVector<int>::int_hash_random(size, 0, row_size - 1)); }
		/*�����_���ȃo�b�`�̎擾 PPL����*/
		FastMatrix<T> random_batch_ppl(int size) { return batch_ppl(FastVector<int>::int_hash_random(size, 0, row_size - 1)); }

		/*�֐���K�p
		func: T(*func)(T x)*/
		template<class F>
		FastMatrix<T> apply_com_func(F func) {
			FastMatrix<T> result(row_size, column_size);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i]);
			}
			return result;
		}
		/*�֐���K�p AMP����
		func: T(*func)(T x) restrict(amp)*/
		template<class F>
		FastMatrix<T> apply_amp_func(F func) {
			FastMatrix<T> result(row_size, column_size);
			concurrency::array_view<const T, 1> av_entity(size, &entity[0]);
			concurrency::array_view<T, 1> av_result(size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = func(av_entity[idx]);
			});
			av_result.synchronize();
			return result;
		}
		/*�֐���K�p PPL����
		func: T(*func)(T x)*/
		template<class F>
		FastMatrix<T> apply_ppl_func(F func) {
			FastMatrix<T> result(row_size, column_size);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i]);
			});
			return result;
		}

		/*�֐���K�p
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_com_combo_func(F func, FastMatrix<T>& mat) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size == mat.get_row_size(), fast_container_exception());
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == mat.get_column_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i], mat[i]);
			}
			return result;
		}
		/*�֐���K�p AMP����
		func: T(*func)(T x1, T x2) restrict(amp)*/
		template<class F>
		FastMatrix<T> apply_amp_combo_func(F func, FastMatrix<T>& mat) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size == mat.get_row_size(), fast_container_exception());
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == mat.get_column_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			concurrency::array_view<const T, 1> av_entity(size, &entity[0]);
			concurrency::array_view<const T, 1> av_mat(size, &mat[0]);
			concurrency::array_view<T, 1> av_result(size, &result[0]);
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = func(av_entity[idx], av_mat[idx]);
			});
			av_result.synchronize();
			return result;
		}
		/*�֐���K�p PPL����
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_ppl_combo_func(F func, FastMatrix<T>& mat) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size == mat.get_row_size(), fast_container_exception());
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == mat.get_column_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i], mat[i]);
			});
			return result;
		}

		/*�s���Ɋ֐���K�p
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_com_combo_func_by_rows(F func, FastVector<T>& vec) {
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i], vec[i % column_size]);
			}
			return result;
		}
		/*�s���Ɋ֐���K�p AMP����
		func: T(*func)(T x1, T x2) restrict(amp)*/
		template<class F>
		FastMatrix<T> apply_amp_combo_func_by_rows(F func, FastVector<T>& vec) {
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<const T, 1> av_vec(column_size, &vec[0]);
			concurrency::array_view<T, 2> av_result(row_size, column_size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<2> idx) restrict(amp) {
				av_result[idx] = func(av_entity[idx], av_vec[idx[1]]);
			});
			av_result.synchronize();
			return result;
		}
		/*�s���Ɋ֐���K�p PPL����
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_ppl_combo_func_by_rows(F func, FastVector<T>& vec) {
			FAST_CONTAINER_EXCEPTION_CHECK(column_size == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i], vec[i % column_size]);
			});
			return result;
		}

		/*�񖈂Ɋ֐���K�p
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_com_combo_func_by_columns(F func, FastVector<T>& vec) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i], vec[i / column_size]);
			}
			return result;
		}
		/*�񖈂Ɋ֐���K�p AMP����
		func: T(*func)(T x1, T x2) restrict(amp)*/
		template<class F>
		FastMatrix<T> apply_amp_combo_func_by_columns(F func, FastVector<T>& vec) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			concurrency::array_view<const T, 2> av_entity(row_size, column_size, &entity[0]);
			concurrency::array_view<const T, 1> av_vec(row_size, &vec[0]);
			concurrency::array_view<T, 2> av_result(row_size, column_size, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<2> idx) restrict(amp) {
				av_result[idx] = func(av_entity[idx], av_vec[idx[0]]);
			});
			av_result.synchronize();
			return result;
		}
		/*�񖈂Ɋ֐���K�p PPL����
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_ppl_combo_func_by_columns(F func, FastVector<T>& vec) {
			FAST_CONTAINER_EXCEPTION_CHECK(row_size == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(row_size, column_size);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i], vec[i / column_size]);
			});
			return result;
		}

		/*[row,column] ((values1),(values2),...)*/
		std::string to_string() {
			std::string result = "[" + std::to_string(row_size) + "," + std::to_string(column_size) + "](";
			for (int i = 0; i < row_size; i++) {
				if (i) result += ",(";
				else result += "(";
				for (int j = 0; j < column_size; j++) {
					if (j) result += "," + std::to_string(entity[i * column_size + j]);
					else result += std::to_string(entity[i * column_size + j]);
				}
				result += ")";
			}
			result += ")";
			return result;
		}

		/*�����_����FastMatrix�𐶐�*/
		static FastMatrix<T> real_random_com(int row, int col, T min = -1, T max = 1) {
			FastMatrix<T> result(row, col);
			RealRandom<T> rnd(min, max);
			return result.apply_com_func([&](T x) { return rnd.generate(); });
		}
		/*�����_����FastMatrix�𐶐� PPL����*/
		static FastMatrix<T> real_random_ppl(int row, int col, T min = -1, T max = 1) {
			FastMatrix<T> result(row, col);
			RealRandom<T> rnd(min, max);
			return result.apply_ppl_func([&](T x) { return rnd.generate(); });
		}
		/*�����_����FastMatrix<int>�𐶐�*/
		static FastMatrix<int> int_random_com(int row, int col, int min = -1, int max = 1) {
			FastMatrix<int> result(row, col);
			IntRandom rnd(min, max);
			return result.apply_com_func([&](int x) { return rnd.generate(); });
		}
		/*�����_����FastMatrix<int>�𐶐� PPL����*/
		static FastMatrix<int> int_random_ppl(int row, int col, int min = -1, int max = 1) {
			FastMatrix<int> result(row, col);
			IntRandom rnd(min, max);
			return result.apply_ppl_func([&](int x) { return rnd.generate(); });
		}
		/*����:mean, �W���΍�:sd �̃����_����FastMatrix�𐶐�*/
		static FastMatrix<T> normal_random_com(int row, int col, T mean = 0, T sd = 1) {
			FastMatrix<T> result(row, col);
			NormalRandom<T> rnd(mean, sd);
			return result.apply_com_func([&](T x) { return rnd.generate(); });
		}
		/*����:mean, �W���΍�:sd �����_����FastMatrix�𐶐� PPL����*/
		static FastMatrix<T> normal_random_ppl(int row, int col, T mean = 0, T sd = 1) {
			FastMatrix<T> result(row, col);
			NormalRandom<T> rnd(mean, sd);
			return result.apply_ppl_func([&](T x) { return rnd.generate(); });
		}

	private:
		std::vector<T> entity;
		int row_size;
		int column_size;
		int size;
	};


#ifdef FAST_CONTAONER_OPERATOR_OVERLOAD_COM_MODE

	template<typename T>
	FastMatrix<T> operator+(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 + x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator-(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 - x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator*(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 * x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator/(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 / x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator==(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 == x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator!=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 != x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator>(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 > x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator<(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 < x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator>=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 >= x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator<=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_com_combo_func([](T x1, T x2) {return x1 <= x2; }, mat2); }

	template<typename T>
	FastMatrix<T> operator+(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val + x; }); }
	template<typename T>
	FastMatrix<T> operator-(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val - x; }); }
	template<typename T>
	FastMatrix<T> operator*(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val * x; }); }
	template<typename T>
	FastMatrix<T> operator/(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val / x; }); }
	template<typename T>
	FastMatrix<T> operator==(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val == x; }); }
	template<typename T>
	FastMatrix<T> operator!=(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val != x; }); }
	template<typename T>
	FastMatrix<T> operator>(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val > x; }); }
	template<typename T>
	FastMatrix<T> operator<(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val < x; }); }
	template<typename T>
	FastMatrix<T> operator>=(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val >= x; }); }
	template<typename T>
	FastMatrix<T> operator<=(T val, FastMatrix<T>& mat) { return mat.apply_com_func([=](T x) {return val <= x; }); }

	template<typename T>
	FastMatrix<T> operator+(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x + val; }); }
	template<typename T>
	FastMatrix<T> operator-(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x - val; }); }
	template<typename T>
	FastMatrix<T> operator*(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x * val; }); }
	template<typename T>
	FastMatrix<T> operator/(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x / val; }); }
	template<typename T>
	FastMatrix<T> operator==(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x == val; }); }
	template<typename T>
	FastMatrix<T> operator!=(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x != val; }); }
	template<typename T>
	FastMatrix<T> operator>(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x > val; }); }
	template<typename T>
	FastMatrix<T> operator<(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x < val; }); }
	template<typename T>
	FastMatrix<T> operator>=(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x >= val; }); }
	template<typename T>
	FastMatrix<T> operator<=(FastMatrix<T>& mat, T val) { return mat.apply_com_func([=](T x) {return x <= val; }); }

#elif defined FAST_CONTAONER_OPERATOR_OVERLOAD_AMP_MODE

	template<typename T>
	FastMatrix<T> operator+(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 + x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator-(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 - x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator*(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 * x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator/(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 / x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator==(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 == x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator!=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 != x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator>(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 > x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator<(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 < x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator>=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 >= x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator<=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_amp_combo_func([](T x1, T x2) restrict(amp) {return x1 <= x2; }, mat2); }

	template<typename T>
	FastMatrix<T> operator+(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val + x; }); }
	template<typename T>
	FastMatrix<T> operator-(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val - x; }); }
	template<typename T>
	FastMatrix<T> operator*(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val * x; }); }
	template<typename T>
	FastMatrix<T> operator/(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val / x; }); }
	template<typename T>
	FastMatrix<T> operator==(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val == x; }); }
	template<typename T>
	FastMatrix<T> operator!=(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val != x; }); }
	template<typename T>
	FastMatrix<T> operator>(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val > x; }); }
	template<typename T>
	FastMatrix<T> operator<(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val < x; }); }
	template<typename T>
	FastMatrix<T> operator>=(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val >= x; }); }
	template<typename T>
	FastMatrix<T> operator<=(T val, FastMatrix<T>& mat) { return mat.apply_amp_func([=](T x) restrict(amp) {return val <= x; }); }

	template<typename T>
	FastMatrix<T> operator+(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x + val; }); }
	template<typename T>
	FastMatrix<T> operator-(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x - val; }); }
	template<typename T>
	FastMatrix<T> operator*(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x * val; }); }
	template<typename T>
	FastMatrix<T> operator/(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x / val; }); }
	template<typename T>
	FastMatrix<T> operator==(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x == val; }); }
	template<typename T>
	FastMatrix<T> operator!=(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x != val; }); }
	template<typename T>
	FastMatrix<T> operator>(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x > val; }); }
	template<typename T>
	FastMatrix<T> operator<(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x < val; }); }
	template<typename T>
	FastMatrix<T> operator>=(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x >= val; }); }
	template<typename T>
	FastMatrix<T> operator<=(FastMatrix<T>& mat, T val) { return mat.apply_amp_func([=](T x) restrict(amp) {return x <= val; }); }

#elif defined FAST_CONTAONER_OPERATOR_OVERLOAD_PPL_MODE

	template<typename T>
	FastMatrix<T> operator+(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 + x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator-(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 - x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator*(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 * x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator/(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 / x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator==(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 == x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator!=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 != x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator>(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 > x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator<(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 < x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator>=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 >= x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator<=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_ppl_combo_func([](T x1, T x2) {return x1 <= x2; }, mat2); }

	template<typename T>
	FastMatrix<T> operator+(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val + x; }); }
	template<typename T>
	FastMatrix<T> operator-(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val - x; }); }
	template<typename T>
	FastMatrix<T> operator*(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val * x; }); }
	template<typename T>
	FastMatrix<T> operator/(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val / x; }); }
	template<typename T>
	FastMatrix<T> operator==(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val == x; }); }
	template<typename T>
	FastMatrix<T> operator!=(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val != x; }); }
	template<typename T>
	FastMatrix<T> operator>(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val > x; }); }
	template<typename T>
	FastMatrix<T> operator<(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val < x; }); }
	template<typename T>
	FastMatrix<T> operator>=(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val >= x; }); }
	template<typename T>
	FastMatrix<T> operator<=(T val, FastMatrix<T>& mat) { return mat.apply_ppl_func([=](T x) {return val <= x; }); }

	template<typename T>
	FastMatrix<T> operator+(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x + val; }); }
	template<typename T>
	FastMatrix<T> operator-(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x - val; }); }
	template<typename T>
	FastMatrix<T> operator*(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x * val; }); }
	template<typename T>
	FastMatrix<T> operator/(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x / val; }); }
	template<typename T>
	FastMatrix<T> operator==(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x == val; }); }
	template<typename T>
	FastMatrix<T> operator!=(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x != val; }); }
	template<typename T>
	FastMatrix<T> operator>(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x > val; }); }
	template<typename T>
	FastMatrix<T> operator<(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x < val; }); }
	template<typename T>
	FastMatrix<T> operator>=(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x >= val; }); }
	template<typename T>
	FastMatrix<T> operator<=(FastMatrix<T>& mat, T val) { return mat.apply_ppl_func([=](T x) {return x <= val; }); }

#endif

}
