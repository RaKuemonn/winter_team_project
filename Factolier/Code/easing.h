#pragma once

#include <DirectXMath.h>
//******************************************************************************
//
//		easing.h
//
//      Easing Function �����\
//      URL https://easings.net/ja
//
//      �����L�T�C�g�̃R�[�h���Q�l�ɂ��Ă��܂��idouble�^��S��float�^�ɕύX�ς݁j
//      URL https://qiita.com/hart_edsf/items/962ac03281b871dcc0df
//
//******************************************************************************

static float(*const to_radian)(float) = DirectX::XMConvertToRadians;  // �p�x�����W�A����
static float(*const to_degree)(float) = DirectX::XMConvertToDegrees;  // ���W�A�����p�x��

//�C�[�W���O�֐�
namespace easing
{
	float in_quad(float t, float totaltime, float max, float min);
	float out_quad(float t, float totaltime, float max, float min);
	float in_out_quad(float t, float totaltime, float max, float min);
	float in_cubic(float t, float totaltime, float max, float min);
	float out_cubic(float t, float totaltime, float max, float min);
	float in_out_cubic(float t, float totaltime, float max, float min);
	float in_quart(float t, float totaltime, float max, float min);
	float out_quart(float t, float totaltime, float max, float min);
	float in_out_quart(float t, float totaltime, float max, float min);
	float in_quint(float t, float totaltime, float max, float min);
	float out_quint(float t, float totaltime, float max, float min);
	float in_out_quint(float t, float totaltime, float max, float min);
	float in_sine(float t, float totaltime, float max, float min);
	float out_sine(float t, float totaltime, float max, float min);
	float in_out_sine(float t, float totaltime, float max, float min);
	float in_exp(float t, float totaltime, float max, float min);
	float out_exp(float t, float totaltime, float max, float min);
	float in_out_exp(float t, float totaltime, float max, float min);
	float in_circ(float t, float totaltime, float max, float min);
	float out_circ(float t, float totaltime, float max, float min);
	float in_out_circ(float t, float totaltime, float max, float min);
	float in_back(float t, float totaltime, float max, float min, float s);
	float out_back(float t, float totaltime, float max, float min, float s);
	float out_bounce(float t, float totaltime, float max, float min);
	float in_bounce(float t, float totaltime, float max, float min);
	float in_out_bounce(float t, float totaltime, float max, float min);
	float linear(float t, float totaltime, float max, float min);
}
