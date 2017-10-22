#pragma once

/*
�������[�h�ؑ֎��̊֐��̎������[�h��ؑ�
*/
#ifdef FAST_CONTAONER_FUNCTIONS_AMP_MODE
#elif defined FAST_CONTAONER_FUNCTIONS_PPL_MODE
#else
	#define FAST_CONTAONER_FUNCTIONS_COM_MODE
#endif

/*
FastContainer�N���X�̉��Z�q�I�[�o�[���C�h�̎������[�h��ؑ�
*/
#ifdef FAST_CONTAONER_OPERATOR_OVERLOAD_AMP_MODE
#elif defined FAST_CONTAONER_OPERATOR_OVERLOAD_PPL_MODE
#else
	#define FAST_CONTAONER_OPERATOR_OVERLOAD_COM_MODE
#endif

/*
�G���[�`�F�b�N�̗L����ؑ�
*/
#ifdef FAST_CONTAINER_NO_EXCEPTION
#endif

#ifdef FAST_CONTAONER_FUNCTIONS_COM_MODE
	#define SWITCH_FAST_CONTAONER_FUNCTION(func) func ## _com
#elif defined FAST_CONTAONER_FUNCTIONS_AMP_MODE
	#define SWITCH_FAST_CONTAONER_FUNCTION(func) func ## _amp
#elif defined FAST_CONTAONER_FUNCTIONS_PPL_MODE
	#define SWITCH_FAST_CONTAONER_FUNCTION(func) func ## _ppl
#endif

#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <ostream>
#include <stdio.h>
#include <ctype.h>
#include <random>
#include <amp.h>
#include <amp_math.h>
#include <ppl.h>

#include "Functions.hpp"
#include "FastVector.hpp"
#include "FastMatrix.hpp"

namespace FastContainer {

}
