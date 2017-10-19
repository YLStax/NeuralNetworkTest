#pragma once

#include "FastContainerLibrary.hpp"

#define FAST_CONTAONER_OPERATOR_OVERLOAD_AMP_MODE
//#define FAST_CONTAONER_OPERATOR_OVERLOAD_PPL_MODE
//#define FAST_CONTAONER_OPERATOR_OVERLOAD_NORMAL_MODE

namespace NeuralNetwork {

#pragma region Layer

	/*レイヤ基底クラス*/
	template<typename T>
	class Layer {
	public:
		virtual FastContainer::FastMatrix<T> forward(FastContainer::FastMatrix<T>& target) = 0;
		virtual FastContainer::FastMatrix<T> backward(FastContainer::FastMatrix<T>& target) = 0;
		virtual void update(T learningRate) = 0;
	};

	/*シグモイドレイヤ*/
	template<typename T>
	class SigmoidLayer :public Layer<T> {
	public:
		FastContainer::FastMatrix<T> forward(FastContainer::FastMatrix<T>& target) {
			return target.amp_sigmoid();
		}
		FastContainer::FastMatrix<T> backward(const FastContainer::FastMatrix<T>& target) {
			return out * (1 - out) * target;
		}
		void update(T learningRate) {
		}
	private:
		FastContainer::FastMatrix<T> out;
	};

	/*ReLUレイヤ*/
	template<typename T>
	class ReluLayer :public Layer<T> {
	public:
		FastContainer::FastMatrix<T> forward(FastContainer::FastMatrix<T>& target) {
			mask = target > (T)0;
			return target * mask;
		}
		FastContainer::FastMatrix<T> backward(FastContainer::FastMatrix<T>& target) {
			return target * mask;
		}
		void update(T learningRate) {
		}
	private:
		FastContainer::FastMatrix<T> mask;
	};

	/*Parametric ReLUレイヤ*/
	template<typename T>
	class PReluLayer :public Layer<T> {
	public:
		PReluLayer(T slope) {
			this->slope = slope;
		}
		FastContainer::FastMatrix<T> forward(FastContainer::FastMatrix<T>& target) {
			mask = target > 0;
			mask = mask + (slope * (mask == 0));
			return target * mask;
		}
		FastContainer::FastMatrix<T> backward(FastContainer::FastMatrix<T>& target) {
			return target * mask;
		}
		void update(T learningRate) {
		}
	private:
		FastContainer::FastMatrix<T> mask;
		T slope;
	};

	/*Randomized Leaky ReLUレイヤ*/
	template<typename T>
	class RReluLayer :public Layer<T> {
	public:
		RReluLayer(T slope_min, T slope_max) {
			this->slope_min = slope_min;
			this->slope_max = slope_max;
		}
		FastContainer::FastMatrix<T> forward(FastContainer::FastMatrix<T>& target) {
			mask = target > 0;
			auto rnd = FastContainer::FastMatrix<T>::ppl_random(target.get_rows(), target.get_columns(), slope_min, slope_max);
			mask = mask + (rnd * (mask == 0));
			return target * mask;
		}
		FastContainer::FastMatrix<T> backward(FastContainer::FastMatrix<T>& target) {
			return target * mask;
		}
		void update(T learningRate) {
		}
	private:
		FastContainer::FastMatrix<T> mask;
		T slope_min;
		T slope_max;
	};

	/*アフィンレイヤ*/
	template<typename T>
	class AffineLayer :public Layer<T> {
	public:
		AffineLayer(const FastContainer::FastMatrix<T>& w, const FastContainer::FastVector<T>& b) {
			this->w = w;
			this->b = b;
		}
		FastContainer::FastMatrix<T> forward(FastContainer::FastMatrix<T>& target) {
			x = target;
			return target.amp_dot(w).amp_add_by_rows(b);
		}
		FastContainer::FastMatrix<T> backward(FastContainer::FastMatrix<T>& target) {
			auto dx = target.amp_dot(w.amp_reverse());
			dw = x.amp_reverse().amp_dot(target);
			db = target.amp_sum_by_columns();
			return dx;
		}
		void update(T learningRate) {
			w = w - (learningRate * dw);
			b = b - (learningRate * db);
		}
		FastContainer::FastMatrix<T> getdw() {
			return dw;
		}
		FastContainer::FastVector<T> getdb() {
			return db;
		}
	private:
		FastContainer::FastMatrix<T> w;
		FastContainer::FastVector<T> b;
		FastContainer::FastMatrix<T> x;
		FastContainer::FastMatrix<T> dw;
		FastContainer::FastVector<T> db;
	};

#pragma endregion

#pragma region LastLayer

	/*最終レイヤ基底クラス*/
	template<typename T>
	class LastLayer {
	public:
		virtual T forward(FastContainer::FastMatrix<T>& target, FastContainer::FastMatrix<T>& teacher) = 0;
		virtual FastContainer::FastMatrix<T> backward() = 0;
	};

	/*ソフトマックス誤差レイヤ*/
	template<typename T>
	class SoftmaxWithLossLayer :public LastLayer<T> {
	public:
		T forward(FastContainer::FastMatrix<T>& target, FastContainer::FastMatrix<T>& teacher) {
			this->teacher = teacher;
			out = target.amp_softmax();
			return out.amp_cross_entropy_error_class(teacher);
		}
		FastContainer::FastMatrix<T> backward() {
			if (teacher.get_rows() == 1) {
				return FastContainer::FastMatrix<T>(out.sub_by_rows(teacher.to_FastVector()) / ((T)-1 * teacher.get_columns()));
			}
			else {
				return (out - teacher) / (T)teacher.get_rows();
			}
		}
	private:
		T loss;
		FastContainer::FastMatrix<T> out;
		FastContainer::FastMatrix<T> teacher;
	};

#pragma endregion

#pragma region NeuralNetwork

	/*ニューラルネットワーク*/
	template<typename T>
	class Network {
	public:
		std::vector<Layer<T> *> layers;
		LastLayer<T> *lastLayer;
		FastContainer::FastMatrix<T> predict(FastContainer::FastMatrix<T>& input) {
			auto result = input;
			for each (auto layer in layers)
			{
				result = layer->forward(result);
			}
			return result;
		}
		T loss(FastContainer::FastMatrix<T>& input, FastContainer::FastMatrix<T>& teacher) {
			auto y = predict(input);
			return lastLayer->forward(y, teacher);
		}
		T accuracy(FastContainer::FastMatrix<T>& input, FastContainer::FastMatrix<T>& teacher) {
			auto y = predict(input).amp_argmax_by_rows();
			auto t = teacher.amp_argmax_by_rows();
			return (y == t).sum() / input.get_rows();
		}
		T accuracy(FastContainer::FastMatrix<T>& input, FastContainer::FastVector<T>& teacher) {
			auto y = predict(input).amp_argmax_by_rows();
			return (y == teacher).sum() / input.get_rows();
		}
		void set_gradient() {
			auto out = lastLayer->backward();
			for (auto it = layers.rbegin(); it != layers.rend(); ++it) {
				out = (*it)->backward(out);
			}
		}
		void update(T learningRate) {
			for each (auto layer in layers)
			{
				layer->update(learningRate);
			}
		}
		void training(FastContainer::FastMatrix<T>& input, FastContainer::FastMatrix<T>& teacher, T learningRate) {
			loss(input, teacher);
			set_gradient();
			update(learningRate);
		}
	private:
	};

#pragma endregion

}
