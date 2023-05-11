#pragma once
/*****************************************************************//**
 * \file   player.h
 * \brief  �v���C���[�N���X
 *
 * \author �R�{介v
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"

 /**
	* @enum ExclusiveState
	* �L�����̍s�����
	*/
enum class ExclusiveState
{
	NORMAL,//!�������
	JUMP,//!�W�����v
	quickATTACK,//!��U��
	slowATTACK,//!���U��
	changeATTACKX,//X�{�^��������ւ��Z
	changeATTACKY,//Y�{�^��������ւ��Z
	DODGE,//!���
	GUARD,//!�K�[�h
	DAMAGE,//!��e������
	finishAttack,//!�o�����t�B�j�b�V���Z
};

enum class pushButton
{
	Neutral,//!�����
	A,//!A�{�^��
	B,//!B�{�^��
	X,//!X�{�^��
	Y,//!Y�{�^��
	R1,//!R1�{�^��
	LBX,//!L1 + X�{�^��
	LBY,//!L1 + Y�{�^��
	LBA,//!L1 + A�{�^��
	Lstick,//!Lsrick����
	Irregular,//!��O
	Damage,//!��e����O
	finishAttack,//!�o�����t�B�j�b�V���Z��O
};

/**
 * @class PL
 * @brief �v���C���N���X
 */
class PL : public CB
{
public:
	PL() {};
	~PL() {};

	/**
	 * @brief ����������
	 * @return true
	 */
	virtual bool	Initialize();

	/**
	 * @brief �폜����
	 * @return true
	 */
	virtual bool	Terminate();

	/**
	 * @brief �X�V����
	 * @return true
	 */
	virtual bool	Process();

	/**
	 * @brief �`�揈��
	 * @param �Đ����x
	 * @return true
	 */
	virtual bool	Render(float timeSpeed);

	/**
	 * @brief �L�����ړ�����
	 * @param Speed �ړ��X�s�[�h
	 * @param _Dir �ړ�����
	 * @param isAnimChange run�A�j���[�V�������Đ����邩
	 */
	void charMove(float Speed, float _Dir, bool isAnimChange);

	/**
	 * @brief ���͂���Ƀv���C���̍s����Ԃ�����
	 * \return ���L�����N�^�̍s��
	 */
	pushButton setAction();

	/**
	 * @brief �L�[�{�[�h/PAD���͂̃g���K���͊m�F����
	 * @param Key �L�[
	 * @param Pad �{�^��
	 * @return ���͂���Ă��邩
	 */
	bool checkTrgImput(int Key, int Pad) { if (_imputInf->_gTrgb[Key] || _imputInf->_gTrgp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief �L�[�{�[�h/PAD���͂̓��͊m�F����
	 * @param Key �L�[
	 * @param Pad �{�^��
	 * @return ���͂���Ă��邩
	 */
	bool checkKeyImput(int Key, int Pad) { if (_imputInf->_gKeyb[Key] || _imputInf->_gKeyp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief �L�[�{�[�h/PAD���͂̃����[�X���͊m�F����
	 * @param Key �L�[
	 * @param Pad �{�^��
	 * @return ���͂���Ă��邩
	 */
	bool checkRelImput(int Key, int Pad) { if (_imputInf->_gRelb[Key] || _imputInf->_gRelp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief Lstick���͂���L�����̈ړ�������Ԃ�����
	 * @param checkUseCamDir ���b�N�I���J������ŕԂ����ǂ���
	 * @return �ړ�����
	 */
	float getMoveDir(bool checkUseCamDir);

	/**
	 * @brief �̗͌v�Z����
	 * @param ���l
	 * @return true
	 */
	bool HPmath(float math, float Stan) override;

	/**
	 * @brief �u���b�h�|�C���g�v�Z����
	 * @param ���l
	 * @return true
	 */
	bool BPmath(float math) override;

	/**
	 * @brief ���@����ւ��Z�ύX����
	 * @param name ����ւ��Z��
	 * @param XorY 2���邤���ǂ���̓���ւ��Z��ς��邩
	 * @return true
	 */
	bool CA_change(std::string name, const char* XorY) override;

	/**
	 * @brief ����ւ��Z(�˂�)
	 * @param insPL �v���C���N���X�|�C���^
	 * @return true
	 */
	static bool CA_charge(PL* insPL);

	/**
	 * @brief ����ւ��Z(�؂蔲��)
	 * @param insPL �v���C���N���X�|�C���^
	 * @return true
	 */
	static bool CA_kirinuke(PL* insPL);

	/**
	 * @brief ����ւ��Z(�f�o�b�N�p�U��)
	 * @param insPL �v���C���N���X�|�C���^
	 * @return true
	 */
	static bool CA_debugAttack(PL* insPL);

	/**
	 * @brief ����ւ��Z(��ݒ�)
	 * @param insPL �v���C���N���X�|�C���^
	 * @return false
	 */
	static bool CA_noData(PL* insPL);

	/**
	 * @brief ����ւ��Z�p�֐��|�C���^
	 * @param insPL �v���C���N���X�|�C���^
	 */
	bool (*changeAttackX)(PL* insPL);

	/**
	 * @brief ����ւ��Z�p�֐��|�C���^
	 * @param insPL �v���C���N���X�|�C���^
	 */
	bool (*changeAttackY)(PL* insPL);

	/**
	 * @brief �A�j���[�V�������Đ����Ԏ擾����
	 * @return �A�j���[�V�������Đ�����
	 */
	float getAnimPlayTotalTime() { return _modelInf.totalTime; };

	/**
	 * @brief ���C���Q�[���I�����{�C�X����
	 */
	void battleEndVoice() override { PlaySoundMem(soundHandle[voiceStartNum + 41 + rand() % 2], DX_PLAYTYPE_BACK); }

	/**
	 * @brief ���C���Q�[���J�n���{�C�X����
	 */
	void battleStartVoice() override { PlaySoundMem(soundHandle[voiceStartNum + 35 + rand() % 2], DX_PLAYTYPE_BACK); }
	bool recastSet()
	{
		if (lastAttackState == ExclusiveState::changeATTACKX && caRecastX <= 0) { caRecastX = setRecastTime; }
		else if (lastAttackState == ExclusiveState::changeATTACKY && caRecastY <= 0) { caRecastY = setRecastTime; }
		else { return false; }
		setRecastTime = 0;
		return true;
	};

protected:
	int attackNumOld//!��/���U�������i�ڂ�
		, waitNextAttack//!��/���U����t����
		, isCharge//!�U���̃`���[�W����(0.�`���[�W���ĂȂ� 1.�`���[�W�� 2.���)
		, chargeLevel//!�U���̃`���[�W�i�K
		, chargeTime//!�`���[�W����
		, isAwakening = 0//�o������
		, voiceStartNum//!�����f�[�^�R���e�i���̐��f�[�^�̏����l
		, guardEfcHandle, chargeEfcHandle, healEfcHandle, impactEfcHandle, waitBlowTime
		, insGuardEfcHandle, insHealEfcHandle;
	float spd//!�L�����̈ړ����x
		, dodgeDir;//!�L�����̉�����
	bool isAnimEnd//!�A�j���[�V�����Đ����I����Ă��邩
		, isPushButtonAct//!��U���A�ł���U����t���ԏI���܂łɕʃA�N�V�����ɐ؂�ւ�����
		, isGuard//!�K�[�h����
		, isFastGuard//!�K�[�h��������
		, isBlow;
	float atkBuff = 0.f;//!���Z�U���o�t�l
	bool isGhost//!�ʃL�����Ƃ̂��蔲���������邩
		, deadVoice = false;//!���S�����̍Đ��͂�����
	int immortalTime//!���G����
		, isCounter = 0;//!�J�E���^�[��Ԃ�
	int walkTime//!�L�����ړ��J�n����̌o�ߎ���
		, counterTime//!�J�E���^�[�Đ�����
		, waitCAChargeTime//!����ւ��Z�ł̈ړ��܂ł̑ҋ@����
		, CAChargeTime//!����ւ��Z�ł̈ړ�����
		, CAChargeSpd//!����ւ��Z�ł̈ړ��X�s�[�h
		, CAChargeAttackNum = 0;
	ExclusiveState Estate = ExclusiveState::NORMAL//!�L�����̍s����ԃC���X�^���X
		, oldEstate = ExclusiveState::NORMAL//!1�O�̃L�����̍s����ԃC���X�^���X
		, lastAttackState;//!�L�����̍ŏI�U���C���X�^���X
	pushButton nextKey;//!��s���͗p�C���X�^���X
};