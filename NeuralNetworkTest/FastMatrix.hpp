#pragma once

#include "FastContainerLibrary.hpp"

namespace FastContainer {

	template<typename T>
	class FastMatrix {
	public:
		FastMatrix() {}
		FastMatrix(int row, int col) { resize(row, col); }
		FastMatrix(std::vector<T> vec, int row_size) {
			rows = row_size;
			columns = vec.size() / row_size;
			size = rows * columns;
			EXCEPTION_CHECK(size == vec.size(), fast_container_exception());
			entity = vec;
		}
		FastMatrix(FastVector<T> vec, int row_size) {
			rows = row_size;
			columns = vec.get_size() / row_size;
			size = rows * columns;
			EXCEPTION_CHECK(size == vec.get_size(), fast_container_exception());
			entity = vec.get_entity();
		}
		FastMatrix(std::vector<std::vector<T>> mat) {
			resize(mat.size(), mat[0].size());
			for (int i = 0; i < rows; i++) {
				EXCEPTION_CHECK(columns == mat[i].size(), fast_container_exception());
				T offset = i * columns;
				for (int j = 0; j < columns; j++) {
					entity[offset + j] = mat[i][j];
				}
			}
		}

		void resize(int row, int col) {
			this->rows = row;
			this->columns = col;
			size = row * col;
			entity.resize(size);
		}

		std::vector<T> get_entity() { return entity; }
		int get_rows() { return rows; }
		int get_columns() { return columns; }
		int get_size() { return size; }

		T& operator[](int idx) { return entity[idx]; }
		T& operator()(int row, int col) { return entity[row * columns + col]; }

		/*FastVector�֕ϊ�*/
		FastVector<T> to_FastVector() {
			FastVector<T> result(entity);
			return result;
		}

		/*�w��s���擾*/
		FastVector<T> row(int row) {
			EXCEPTION_CHECK(row >= rows, fast_container_exception());
			FastVector<T> result(columns);
			for (int i = 0, j = row * columns; i < columns; i++, j++) {
				result[i] = entity[j];
			}
			return result;
		}
		/*�w��s���擾 AMP����*/
		FastVector<T> amp_row(int row) {
			EXCEPTION_CHECK(row >= rows, fast_container_exception());
			FastVector<T> result(columns);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(columns, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = av_entity[row][idx[0]];
			});
			av_result.synchronize();
			return result;
		}
		/*�w��s���擾 PPL����*/
		FastVector<T> ppl_row(int row) {
			EXCEPTION_CHECK(row >= rows, fast_container_exception());
			FastVector<T> result(columns);
			concurrency::parallel_for<int>(0, columns, [&](int i) {
				result[i] = entity[row * columns + i];
			});
			return result;
		}

		/*�w�����擾*/
		FastVector<T> column(int col) {
			EXCEPTION_CHECK(col >= columns, fast_container_exception());
			FastVector<T> result(rows);
			for (int i = 0, j = col; i < rows; i++, j += columns) {
				result[i] = entity[j];
			}
			return result;
		}
		/*�w�����擾 AMP����*/
		FastVector<T> amp_column(int col) {
			EXCEPTION_CHECK(col >= columns, fast_container_exception());
			FastVector<T> result(rows);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(rows, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<1> idx) restrict(amp) {
				av_result[idx] = av_entity[idx[0]][col];
			});
			av_result.synchronize();
			return result;
		}
		/*�w�����擾 PPL����*/
		FastVector<T> ppl_column(int col) {
			EXCEPTION_CHECK(col >= columns, fast_container_exception());
			FastVector<T> result(rows);
			concurrency::parallel_for<int>(0, rows, [&](int i) {
				result[i] = entity[i * columns + col];
			});
			return result;
		}

		FastMatrix<T> random_batch(FastVector<int> mask) {
			int row = mask.get_size();
			FastMatrix<T> result(row, columns);
			for (int i = 0; i < row; i++) {
				int r_offset = i * columns;
				int e_offset = mask[i] * columns;
				for (int j = 0; j < columns; j++) {
					result[r_offset + j] = entity[e_offset + j];
				}
			}
			return result;
		}

		/*���̂܂ܕԂ�*/
		FastMatrix<T> identity() { return this; }

		/*��Βl*/
		FastMatrix<T> abs() { return apply_func([](T x) { return std::abs(x); }); };
		/*��Βl AMP����*/
		FastMatrix<T> amp_abs() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::fabs(x); }); };
		/*��Βl PPL����*/
		FastMatrix<T> ppl_abs() { return apply_ppl_func([](T x) { return std::abs(x); }); };

		/*Log e*/
		FastMatrix<T> log() { return apply_func([](T x) { return std::log(x); }); };
		/*Log e AMP����*/
		FastMatrix<T> amp_log() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::log(x); }); };
		/*Log e PPL����*/
		FastMatrix<T> ppl_log() { return apply_ppl_func([](T x) { return std::log(x); }); };

		/*Log 10*/
		FastMatrix<T> log10() { return apply_func([](T x) { return std::log10(x); }); };
		/*Log 10*/
		FastMatrix<T> amp_log10() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::log10(x); }); };
		/*Log 10*/
		FastMatrix<T> ppl_log10() { return apply_ppl_func([](T x) { return std::log10(x); }); };

		/*2�捪*/
		FastMatrix<T> sqrt() { return apply_func([](T x) { return std::sqrt(x); }); };
		/*2�捪 AMP����*/
		FastMatrix<T> amp_sqrt() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::sqrt(x); }); };
		/*2�捪 PPL����*/
		FastMatrix<T> ppl_sqrt() { return apply_ppl_func([](T x) { return std::sqrt(x); }); };

		/*�K��*/
		FastMatrix<T> pow(T exp) { return apply_func([=](T x) { return std::pow(x, exp); }); };
		/*�K�� AMP����*/
		FastMatrix<T> amp_pow(T exp) { return apply_amp_func([=](T x) restrict(amp) { return concurrency::fast_math::pow(x, exp); }); };
		/*�K�� PPL����*/
		FastMatrix<T> ppl_pow(T exp) { return apply_ppl_func([=](T x) { return std::pow(x, exp); }); };

		/*e^x*/
		FastMatrix<T> exp() { return apply_func([](T x) { return std::exp(x); }); };
		/*e^x AMP����*/
		FastMatrix<T> amp_exp() { return apply_amp_func([](T x) restrict(amp) { return concurrency::fast_math::exp(x); }); };
		/*e^x PPL����*/
		FastMatrix<T> ppl_exp() { return apply_ppl_func([](T x) { return std::exp(x); }); };

		/*�V�O���C�h�֐�*/
		FastMatrix<T> sigmoid() { return apply_func([](T x) { return (T)1 / (1 + std::exp(-x)); }); };
		/*�V�O���C�h�֐� AMP����*/
		FastMatrix<T> amp_sigmoid() { return apply_amp_func([](T x) restrict(amp) { return (T)1 / (1 + concurrency::fast_math::exp(-x)); }); };
		/*�V�O���C�h�֐� PPL����*/
		FastMatrix<T> ppl_sigmoid() { return apply_ppl_func([](T x) { return (T)1 / (1 + std::exp(-x)); }); };

		/*ReLU�֐�*/
		FastMatrix<T> relu() { return apply_func([](T x) { return x > 0 ? x : 0; }); };
		/*ReLU�֐� AMP����*/
		FastMatrix<T> amp_relu() { return apply_amp_func([](T x) restrict(amp) { return x > 0 ? x : 0; }); };
		/*ReLU�֐� PPL����*/
		FastMatrix<T> ppl_relu() { return apply_ppl_func([](T x) { return x > 0 ? x : 0; }); };

		/*�\�t�g�}�b�N�X�֐�*/
		FastMatrix<T> softmax() {
			T max = get_max();
			T total = apply_func([=](T x) { return std::exp(x - max); }).sum();
			return apply_func([=](T x) { return std::exp(x - max) / total; });
		}
		/*�\�t�g�}�b�N�X�֐� AMP����*/
		FastMatrix<T> amp_softmax() {
			T max = get_max();
			T total = apply_amp_func([=](T x) restrict(amp) { return concurrency::fast_math::exp(x - max); }).sum();
			return apply_amp_func([=](T x) restrict(amp) { return concurrency::fast_math::exp(x - max) / total; });
		}
		/*�\�t�g�}�b�N�X�֐� PPL����*/
		FastMatrix<T> ppl_softmax() {
			T max = get_max();
			T total = apply_ppl_func([=](T x) { return std::exp(x - max); }).sum();
			return apply_ppl_func([=](T x) { return std::exp(x - max) / total; });
		}

		/*���l����
		func: (T)(*func)(T x)*/
		template<class F>
		FastMatrix<T> num_diff(F func, T delta = 0.0001) { return apply_func([=](T x) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); };
		/*���l���� AMP����
		func: (T)(*func)(T x)*/
		template<class F>
		FastMatrix<T> amp_num_diff(F func, T delta = 0.0001) { return apply_amp_func([=](T x) restrict(amp) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); };
		/*���l���� PPL����
		func: (T)(*func)(T x)*/
		template<class F>
		FastMatrix<T> ppl_num_diff(F func, T delta = 0.0001) { return apply_ppl_func([=](T x) { return (func(x + delta) - func(x - delta)) / (2 * delta); }); };

		/*�s���̍ŏ��l*/
		FastVector<T> min_by_rows() {
			FastVector<T> result(rows);
			for (int i = 0; i < rows; i++) {
				int offset = i * columns;
				result[i] = entity[offset];
				for (int j = 0; j < columns; j++) {
					T b_result = result[i];
					T b_entity = entity[offset + j];
					result[i] = min(b_result, b_entity);
				}
			}
			return result;
		}
		/*�s���̍ŏ��l AMP����*/
		FastVector<T> amp_min_by_rows() {
			FastVector<T> result(rows);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(rows, &result[0]);
			int b_col = columns;
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
		FastVector<T> ppl_min_by_rows() {
			FastVector<T> result(rows);
			concurrency::parallel_for<int>(0, rows, [&](int i) {
				int offset = i * columns;
				result[i] = entity[offset];
				for (int j = 0; j < columns; j++) {
					T b_result = result[i];
					T b_entity = entity[offset + j];
					result[i] = min(b_result, b_entity);
				}
			});
			return result;
		}

		/*�񖈂̍ŏ��l*/
		FastVector<T> min_by_columns() {
			FastVector<T> result(columns);
			for (int i = 0; i < columns; i++) {
				result[i] = entity[i];
				for (int j = 0; j < rows; j++) {
					T b_result = result[i];
					T b_entity = entity[j * columns + i];
					result[i] = min(b_result, b_entity);
				}
			}
			return result;
		}
		/*�񖈂̍ŏ��l AMP����*/
		FastVector<T> amp_min_by_columns() {
			FastVector<T> result(columns);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(columns, &result[0]);
			int b_row = rows;
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
		FastVector<T> ppl_min_by_columns() {
			FastVector<T> result(columns);
			concurrency::parallel_for<int>(0, columns, [&](int i) {
				result[i] = entity[i];
				for (int j = 0; j < rows; j++) {
					T b_result = result[i];
					T b_entity = entity[j * columns + i];
					result[i] = min(b_result, b_entity);
				}
			});
			return result;
		}

		/*�s���̍ő�l*/
		FastVector<T> max_by_rows() {
			FastVector<T> result(rows);
			for (int i = 0; i < rows; i++) {
				int offset = i * columns;
				result[i] = entity[offset];
				for (int j = 0; j < columns; j++) {
					T b_result = result[i];
					T b_entity = entity[offset + j];
					result[i] = max(b_result, b_entity);
				}
			}
			return result;
		}
		/*�s���̍ő�l AMP����*/
		FastVector<T> amp_max_by_rows() {
			FastVector<T> result(rows);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(rows, &result[0]);
			int b_col = columns;
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
		FastVector<T> ppl_max_by_rows() {
			FastVector<T> result(rows);
			concurrency::parallel_for<int>(0, rows, [&](int i) {
				int offset = i * columns;
				result[i] = entity[offset];
				for (int j = 0; j < columns; j++) {
					T b_result = result[i];
					T b_entity = entity[offset + j];
					result[i] = max(b_result, b_entity);
				}
			});
			return result;
		}

		/*�񖈂̍ő�l*/
		FastVector<T> max_by_columns() {
			FastVector<T> result(columns);
			for (int i = 0; i < columns; i++) {
				result[i] = entity[i];
				for (int j = 0; j < rows; j++) {
					T b_result = result[i];
					T b_entity = entity[j * columns + i];
					result[i] = max(b_result, b_entity);
				}
			}
			return result;
		}
		/*�񖈂̍ő�l AMP����*/
		FastVector<T> amp_max_by_columns() {
			FastVector<T> result(columns);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(columns, &result[0]);
			int b_row = rows;
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
		FastVector<T> ppl_max_by_columns() {
			FastVector<T> result(columns);
			concurrency::parallel_for<int>(0, columns, [&](int i) {
				result[i] = entity[i];
				for (int j = 0; j < rows; j++) {
					T b_result = result[i];
					T b_entity = entity[j * columns + i];
					result[i] = max(b_result, b_entity);
				}
			});
			return result;
		}

		/*�s���̍ő�l�̃C���f�b�N�X*/
		FastVector<T> argmax_by_rows() {
			FastVector<T> result(rows);
			for (int i = 0; i < rows; i++) {
				int offset = i * columns;
				T max = entity[offset];
				for (int j = 0; j < columns; j++) {
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
		FastVector<T> amp_argmax_by_rows() {
			FastVector<T> result(rows);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(rows, &result[0]);
			int b_col = columns;
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
		FastVector<T> ppl_argmax_by_rows() {
			FastVector<T> result(rows);
			concurrency::parallel_for<int>(0, rows, [&](int i) {
				int offset = i * columns;
				T max = entity[offset];
				for (int j = 0; j < columns; j++) {
					T b_entity = entity[offset + j];
					if (max < b_entity) {
						max = b_entity;
						result[i] = j;
					}
				}
			});
			return result;
		}

		/*�񖈂̍ő�l�̃C���f�b�N�X*/
		FastVector<T> argmax_by_columns() {
			FastVector<T> result(columns);
			for (int i = 0; i < columns; i++) {
				T max = entity[i];
				for (int j = 0; j < rows; j++) {
					T b_entity = entity[j * columns + i];
					if (max < b_entity) {
						max = b_entity;
						result[i] = j;
					}
				}
			}
			return result;
		}
		/*�񖈂̍ő�l�̃C���f�b�N�X AMP����*/
		FastVector<T> amp_argmax_by_columns() {
			FastVector<T> result(columns);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(columns, &result[0]);
			int b_row = rows;
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
		FastVector<T> ppl_argmax_by_columns() {
			FastVector<T> result(columns);
			concurrency::parallel_for<int>(0, columns, [&](int i) {
				T max = entity[i];
				for (int j = 0; j < rows; j++) {
					T b_entity = entity[j * columns + i];
					if (max < b_entity) {
						max = b_entity;
						result[i] = j;
					}
				}
			});
			return result;
		}

		/*�s���̍��v*/
		FastVector<T> sum_by_rows() {
			FastVector<T> result(rows);
			for (int i = 0; i < rows; i++) {
				int offset = i * columns;
				for (int j = 0; j < columns; j++) {
					result[i] += entity[offset + j];
				}
			}
			return result;
		}
		/*�s���̍��v AMP����*/
		FastVector<T> amp_sum_by_rows() {
			FastVector<T> result(rows);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(rows, &result[0]);
			int b_col = columns;
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
		FastVector<T> ppl_sum_by_rows() {
			FastVector<T> result(rows);
			concurrency::parallel_for<int>(0, rows, [&](int i) {
				int offset = i * columns;
				for (int j = 0; j < columns; j++) {
					result[i] += entity[offset + j];
				}
			});
			return result;
		}

		/*�񖈂̍��v*/
		FastVector<T> sum_by_columns() {
			FastVector<T> result(columns);
			for (int i = 0; i < columns; i++) {
				for (int j = 0; j < rows; j++) {
					result[i] += entity[j * columns + i];
				}
			}
			return result;
		}
		/*�񖈂̍��v AMP����*/
		FastVector<T> amp_sum_by_columns() {
			FastVector<T> result(columns);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(columns, &result[0]);
			int b_row = rows;
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
		FastVector<T> ppl_sum_by_columns() {
			FastVector<T> result(columns);
			concurrency::parallel_for<int>(0, columns, [&](int i) {
				for (int j = 0; j < rows; j++) {
					result[i] += entity[j * columns + i];
				}
			});
			return result;
		}

		/*�s���̕���*/
		FastVector<T> mean_by_rows() {
			FastVector<T> result(rows);
			for (int i = 0; i < rows; i++) {
				int offset = i * columns;
				for (int j = 0; j < columns; j++) {
					result[i] += entity[offset + j];
				}
				result[i] /= columns;
			}
			return result;
		}
		/*�s���̕��� AMP����*/
		FastVector<T> amp_mean_by_rows() {
			FastVector<T> result(rows);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(rows, &result[0]);
			int b_col = columns;
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
		FastVector<T> ppl_mean_by_rows() {
			FastVector<T> result(rows);
			concurrency::parallel_for<int>(0, rows, [&](int i) {
				int offset = i * columns;
				for (int j = 0; j < columns; j++) {
					result[i] += entity[offset + j];
				}
				result[i] /= columns;
			});
			return result;
		}

		/*�񖈂̕���*/
		FastVector<T> mean_by_columns() {
			FastVector<T> result(columns);
			for (int i = 0; i < columns; i++) {
				for (int j = 0; j < rows; j++) {
					result[i] += entity[j * columns + i];
				}
				result[i] /= rows;
			}
			return result;
		}
		/*�񖈂̕��� AMP����*/
		FastVector<T> amp_mean_by_columns() {
			FastVector<T> result(columns);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 1> av_result(columns, &result[0]);
			int b_row = rows;
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
		FastVector<T> ppl_mean_by_columns() {
			FastVector<T> result(columns);
			concurrency::parallel_for<int>(0, columns, [&](int i) {
				for (int j = 0; j < rows; j++) {
					result[i] += entity[j * columns + i];
				}
				result[i] /= rows;
			});
			return result;
		}

		/*�e�s�֑Ώۂ����Z*/
		FastMatrix<T> add_by_rows(FastVector<T> vec) { return apply_combo_func_by_rows([](T x1, T x2) {return x1 + x2; }, vec); }
		/*�e�s�֑Ώۂ����Z AMP����*/
		FastMatrix<T> amp_add_by_rows(FastVector<T> vec) { return apply_amp_combo_func_by_rows([](T x1, T x2) restrict(amp) {return x1 + x2; }, vec); }
		/*�e�s�֑Ώۂ����Z PPL����*/
		FastMatrix<T> ppl_add_by_rows(FastVector<T> vec) { return apply_ppl_combo_func_by_rows([](T x1, T x2) {return x1 + x2; }, vec); }

		/*�e��֑Ώۂ����Z*/
		FastMatrix<T> add_by_columns(FastVector<T> vec) { return apply_combo_func_by_columns([](T x1, T x2) {return x1 + x2; }, vec); }
		/*�e��֑Ώۂ����Z AMP����*/
		FastMatrix<T> amp_add_by_columns(FastVector<T> vec) { return apply_amp_combo_func_by_columns([](T x1, T x2) restrict(amp) {return x1 + x2; }, vec); }
		/*�e��֑Ώۂ����Z PPL����*/
		FastMatrix<T> ppl_add_by_columns(FastVector<T> vec) { return apply_ppl_combo_func_by_columns([](T x1, T x2) {return x1 + x2; }, vec); }

		/*�e�s����Ώۂ����Z*/
		FastMatrix<T> sub_by_rows(FastVector<T> vec) { return apply_combo_func_by_rows([](T x1, T x2) {return x1 - x2; }, vec); }
		/*�e�s����Ώۂ����Z AMP����*/
		FastMatrix<T> amp_sub_by_rows(FastVector<T> vec) { return apply_amp_combo_func_by_rows([](T x1, T x2) restrict(amp) {return x1 - x2; }, vec); }
		/*�e�s����Ώۂ����Z PPL����*/
		FastMatrix<T> ppl_sub_by_rows(FastVector<T> vec) { return apply_ppl_combo_func_by_rows([](T x1, T x2) {return x1 - x2; }, vec); }

		/*�e�񂩂�Ώۂ����Z*/
		FastMatrix<T> sub_by_columns(FastVector<T> vec) { return apply_combo_func_by_columns([](T x1, T x2) {return x1 - x2; }, vec); }
		/*�e�񂩂�Ώۂ����Z AMP����*/
		FastMatrix<T> amp_sub_by_columns(FastVector<T> vec) { return apply_amp_combo_func_by_columns([](T x1, T x2) restrict(amp) {return x1 - x2; }, vec); }
		/*�e�񂩂�Ώۂ����Z PPL����*/
		FastMatrix<T> ppl_sub_by_columns(FastVector<T> vec) { return apply_ppl_combo_func_by_columns([](T x1, T x2) {return x1 - x2; }, vec); }

		/*�e�s�֑Ώۂ���Z*/
		FastMatrix<T> mul_by_rows(FastVector<T> vec) { return apply_combo_func_by_rows([](T x1, T x2) {return x1 * x2; }, vec); }
		/*�e�s�֑Ώۂ���Z AMP����*/
		FastMatrix<T> amp_mul_by_rows(FastVector<T> vec) { return apply_amp_combo_func_by_rows([](T x1, T x2) restrict(amp) {return x1 * x2; }, vec); }
		/*�e�s�֑Ώۂ���Z PPL����*/
		FastMatrix<T> ppl_mul_by_rows(FastVector<T> vec) { return apply_ppl_combo_func_by_rows([](T x1, T x2) {return x1 * x2; }, vec); }

		/*�e��֑Ώۂ���Z*/
		FastMatrix<T> mul_by_columns(FastVector<T> vec) { return apply_combo_func_by_columns([](T x1, T x2) {return x1 * x2; }, vec); }
		/*�e��֑Ώۂ���Z AMP����*/
		FastMatrix<T> amp_mul_by_columns(FastVector<T> vec) { return apply_amp_combo_func_by_columns([](T x1, T x2) restrict(amp) {return x1 * x2; }, vec); }
		/*�e��֑Ώۂ���Z PPL����*/
		FastMatrix<T> ppl_mul_by_columns(FastVector<T> vec) { return apply_ppl_combo_func_by_columns([](T x1, T x2) {return x1 * x2; }, vec); }

		/*�e�s����Ώۂ����Z*/
		FastMatrix<T> div_by_rows(FastVector<T> vec) { return apply_combo_func_by_rows([](T x1, T x2) {return x1 / x2; }, vec); }
		/*�e�s����Ώۂ����Z AMP����*/
		FastMatrix<T> amp_div_by_rows(FastVector<T> vec) { return apply_amp_combo_func_by_rows([](T x1, T x2) restrict(amp) {return x1 / x2; }, vec); }
		/*�e�s����Ώۂ����Z PPL����*/
		FastMatrix<T> ppl_div_by_rows(FastVector<T> vec) { return apply_ppl_combo_func_by_rows([](T x1, T x2) {return x1 / x2; }, vec); }

		/*�e�񂩂�Ώۂ����Z*/
		FastMatrix<T> div_by_columns(FastVector<T> vec) { return apply_combo_func_by_columns([](T x1, T x2) {return x1 / x2; }, vec); }
		/*�e�񂩂�Ώۂ����Z AMP����*/
		FastMatrix<T> amp_div_by_columns(FastVector<T> vec) { return apply_amp_combo_func_by_columns([](T x1, T x2) restrict(amp) {return x1 / x2; }, vec); }
		/*�e�񂩂�Ώۂ����Z PPL����*/
		FastMatrix<T> ppl_div_by_columns(FastVector<T> vec) { return apply_ppl_combo_func_by_columns([](T x1, T x2) {return x1 / x2; }, vec); }

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
		/*���v PPL����*/
		T ppl_sum() {
			concurrency::combinable<T> result;
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result.local() += entity[i];
			});
			return result.combine(std::plus<T>());
		}

		/*����*/
		T mean() {
			return sum() / size;
		}
		/*���� PPL����*/
		T ppl_mean() {
			return ppl_sum() / size;
		}

		/*�����G���g���s�[�덷*/
		T cross_entropy_error(FastMatrix<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_combo_func([=](T x1, T x2) { return -1 * x2 * std::log(x1 + delta) + (1 - x2) * std::log(1 - x1 + delta); }, teacher);
			return buf.sum();
		}
		/*�����G���g���s�[�덷 AMP����*/
		T amp_cross_entropy_error(FastMatrix<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_amp_combo_func([=](T x1, T x2) restrict(amp) { return -1 * x2 * concurrency::fast_math::log(x1 + delta) + (1 - x2) * concurrency::fast_math::log(1 - x1 + delta); }, teacher);
			return buf.ppl_sum();
		}
		/*�����G���g���s�[�덷 PPL����*/
		T ppl_cross_entropy_error(FastMatrix<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_ppl_combo_func([=](T x1, T x2) { return -1 * x2 * std::log(x1 + delta) + (1 - x2) * std::log(1 - x1 + delta); }, teacher);
			return buf.ppl_sum();
		}

		/*�����G���g���s�[�덷 ���ޖ��*/
		T cross_entropy_error_class(FastMatrix<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_combo_func([=](T x1, T x2) { return -1 * x2 * std::log(x1 + delta) }, teacher);
			return buf.sum();
		}
		/*�����G���g���s�[�덷 ���ޖ�� AMP����*/
		T amp_cross_entropy_error_class(FastMatrix<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_amp_combo_func([=](T x1, T x2) restrict(amp) { return -1 * x2 * concurrency::fast_math::log(x1 + delta); }, teacher);
			return buf.ppl_sum();
		}
		/*�����G���g���s�[�덷 ���ޖ�� PPL����*/
		T ppl_cross_entropy_error_class(FastMatrix<T>& teacher, T delta = 0.0000001) {
			T result = 0.0;
			auto buf = apply_ppl_combo_func([=](T x1, T x2) { return -1 * x2 * std::log(x1 + delta) }, teacher);
			return buf.ppl_sum();
		}

		/*����*/
		FastMatrix<T> dot(FastMatrix<T>& mat) {
			EXCEPTION_CHECK(rows == mat.get_columns(), fast_container_exception());
			EXCEPTION_CHECK(columns == mat.get_rows(), fast_container_exception());
			int col = mat.get_columns();
			FastMatrix<T> result(rows, col);
			for (int i = 0; i < rows; i++) {
				int ent_offset = i * columns;
				int res_offset = i * col;
				for (int j = 0; j < col; j++) {
					int res_pos = res_offset + j;
					for (int k = 0; k < columns; k++) {
						result[res_pos] += entity[ent_offset + k] * mat(k, j);
					}
				}
			}
			return result;
		}
		/*���� AMP����*/
		FastMatrix<T> amp_dot(FastMatrix<T>& mat) {
			EXCEPTION_CHECK(rows == mat.get_columns(), fast_container_exception());
			EXCEPTION_CHECK(columns == mat.get_rows(), fast_container_exception());
			int col = mat.get_columns();
			int mid = columns;
			FastMatrix<T> result(rows, col);
			concurrency::array_view<const T, 2> av_entity(rows, mid, &entity[0]);
			concurrency::array_view<const T, 2> av_mat(mid, col, &mat[0]);
			concurrency::array_view<T, 2> av_result(rows, col, &result[0]);
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
		FastMatrix<T> ppl_dot(FastMatrix<T>& mat) {
			EXCEPTION_CHECK(rows == mat.get_columns(), fast_container_exception());
			EXCEPTION_CHECK(columns == mat.get_rows(), fast_container_exception());
			int col = mat.get_columns();
			int mid = columns;
			int size = rows * col;
			FastMatrix<T> result(rows, col);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				int offset = (i / col) * columns;
				int mat_col = i % col;
				for (int j = 0; j < mid; j++) {
					result[i] += entity[offset + j] * mat(j, mat_col);
				}
			});
			return result;
		}

		/*�]�u�s��*/
		FastMatrix<T> reverse() {
			FastMatrix<T> result(columns, rows);
			for (int i = 0; i < rows; i++) {
				int offset = i * columns;
				for (int j = 0; j < columns; j++) {
					result(j, i) = entity[offset + j];
				}
			}
			return result;
		}
		/*�]�u�s�� AMP����*/
		FastMatrix<T> amp_reverse() {
			FastMatrix<T> result(columns, rows);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<T, 2> av_result(columns, rows, &result[0]);
			av_result.discard_data();
			concurrency::parallel_for_each(av_result.extent, [=](concurrency::index<2> idx) restrict(amp) {
				av_result[idx] = av_entity[idx[1]][idx[0]];
			});
			av_result.synchronize();
			return result;
		}
		/*�]�u�s�� PPL����*/
		FastMatrix<T> ppl_reverse() {
			FastMatrix<T> result(columns, rows);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = entity[(i % rows) * columns + i / rows];
			});
			return result;
		}

		/*�֐���K�p
		func: T(*func)(T x)*/
		template<class F>
		FastMatrix<T> apply_func(F func) {
			FastMatrix<T> result(rows, columns);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i]);
			}
			return result;
		}
		/*�֐���K�p AMP����
		func: T(*func)(T x)*/
		template<class F>
		FastMatrix<T> apply_amp_func(F func) {
			FastMatrix<T> result(rows, columns);
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
			FastMatrix<T> result(rows, columns);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i]);
			});
			return result;
		}

		/*�֐���K�p
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_combo_func(F func, FastMatrix<T> mat) {
			EXCEPTION_CHECK(rows == mat.get_rows(), fast_container_exception());
			EXCEPTION_CHECK(columns == mat.get_columns(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i], mat[i]);
			}
			return result;
		}
		/*�֐���K�p AMP����
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_amp_combo_func(F func, FastMatrix<T> mat) {
			EXCEPTION_CHECK(rows == mat.get_rows(), fast_container_exception());
			EXCEPTION_CHECK(columns == mat.get_columns(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
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
		FastMatrix<T> apply_ppl_combo_func(F func, FastMatrix<T> mat) {
			EXCEPTION_CHECK(rows == mat.get_rows(), fast_container_exception());
			EXCEPTION_CHECK(columns == mat.get_columns(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i], mat[i]);
			});
			return result;
		}

		/*�s���Ɋ֐���K�p
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_combo_func_by_rows(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(columns == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i], vec[i % columns]);
			}
			return result;
		}
		/*�s���Ɋ֐���K�p AMP����
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_amp_combo_func_by_rows(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(columns == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<const T, 1> av_vec(columns, &vec[0]);
			concurrency::array_view<T, 2> av_result(rows, columns, &result[0]);
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
		FastMatrix<T> apply_ppl_combo_func_by_rows(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(columns == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i], vec[i % columns]);
			});
			return result;
		}

		/*�񖈂Ɋ֐���K�p
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_combo_func_by_columns(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(rows == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
			for (int i = 0; i < size; i++) {
				result[i] = func(entity[i], vec[i / columns]);
			}
			return result;
		}
		/*�񖈂Ɋ֐���K�p AMP����
		func: T(*func)(T x1, T x2)*/
		template<class F>
		FastMatrix<T> apply_amp_combo_func_by_columns(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(rows == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
			concurrency::array_view<const T, 2> av_entity(rows, columns, &entity[0]);
			concurrency::array_view<const T, 1> av_vec(rows, &vec[0]);
			concurrency::array_view<T, 2> av_result(rows, columns, &result[0]);
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
		FastMatrix<T> apply_ppl_combo_func_by_columns(F func, FastVector<T> vec) {
			EXCEPTION_CHECK(rows == vec.get_size(), fast_container_exception());
			FastMatrix<T> result(rows, columns);
			concurrency::parallel_for<int>(0, size, [&](int i) {
				result[i] = func(entity[i], vec[i / columns]);
			});
			return result;
		}

		/*[row,column] ((values1),(values2),...)*/
		std::string to_string() {
			std::string result = "[" + std::to_string(rows) + "," + std::to_string(columns) + "](";
			for (int i = 0; i < rows; i++) {
				if (i) result += ",(";
				else result += "(";
				for (int j = 0; j < columns; j++) {
					if (j) result += "," + std::to_string(entity[i * columns + j]);
					else result += std::to_string(entity[i * columns + j]);
				}
				result += ")";
			}
			result += ")";
			return result;
		}

		/*�����_����FastMatrix�𐶐�*/
		static FastMatrix<T> random(int row, int col, T min = -1, T max = 1) {
			FastMatrix<T> result(row, col);
			Random<T> rnd(min, max);
			return result.apply_func([&](T x) { return rnd.generate(); });
		}
		/*�����_����FastMatrix�𐶐� PPL����*/
		static FastMatrix<T> ppl_random(int row, int col, T min = -1, T max = 1) {
			FastMatrix<T> result(row, col);
			Random<T> rnd(min, max);
			return result.apply_ppl_func([&](T x) { return rnd.generate(); });
		}

	private:
		std::vector<T> entity;
		int rows;
		int columns;
		int size;
	};


#ifdef FAST_CONTAONER_OPERATOR_OVERLOAD_AMP_MODE

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

#else

#define FAST_CONTAONER_OPERATOR_OVERLOAD_NORMAL_MODE

	template<typename T>
	FastMatrix<T> operator+(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 + x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator-(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 - x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator*(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 * x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator/(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 / x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator==(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 == x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator!=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 != x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator>(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 > x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator<(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 < x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator>=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 >= x2; }, mat2); }
	template<typename T>
	FastMatrix<T> operator<=(FastMatrix<T>& mat1, FastMatrix<T>& mat2) { return mat1.apply_combo_func([](T x1, T x2) {return x1 <= x2; }, mat2); }

	template<typename T>
	FastMatrix<T> operator+(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val + x; }); }
	template<typename T>
	FastMatrix<T> operator-(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val - x; }); }
	template<typename T>
	FastMatrix<T> operator*(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val * x; }); }
	template<typename T>
	FastMatrix<T> operator/(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val / x; }); }
	template<typename T>
	FastMatrix<T> operator==(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val == x; }); }
	template<typename T>
	FastMatrix<T> operator!=(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val != x; }); }
	template<typename T>
	FastMatrix<T> operator>(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val > x; }); }
	template<typename T>
	FastMatrix<T> operator<(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val < x; }); }
	template<typename T>
	FastMatrix<T> operator>=(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val >= x; }); }
	template<typename T>
	FastMatrix<T> operator<=(T val, FastMatrix<T>& mat) { return mat.apply_func([=](T x) {return val <= x; }); }

	template<typename T>
	FastMatrix<T> operator+(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x + val; }); }
	template<typename T>
	FastMatrix<T> operator-(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x - val; }); }
	template<typename T>
	FastMatrix<T> operator*(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x * val; }); }
	template<typename T>
	FastMatrix<T> operator/(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x / val; }); }
	template<typename T>
	FastMatrix<T> operator==(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x == val; }); }
	template<typename T>
	FastMatrix<T> operator!=(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x != val; }); }
	template<typename T>
	FastMatrix<T> operator>(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x > val; }); }
	template<typename T>
	FastMatrix<T> operator<(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x < val; }); }
	template<typename T>
	FastMatrix<T> operator>=(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x >= val; }); }
	template<typename T>
	FastMatrix<T> operator<=(FastMatrix<T>& mat, T val) { return mat.apply_func([=](T x) {return x <= val; }); }

#endif

}
