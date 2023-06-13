#pragma once
/*****************************************************************//**
 * \file   player.h
 * \brief  プレイヤークラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"charBase.h"

 /**
	* @enum ExclusiveState
	* キャラの行動状態
	*/
enum class ExclusiveState
{
	NORMAL,//!初期状態
	JUMP,//!ジャンプ
	rClick,//!固有
	lClick,//!射撃
	DODGE,//!回避
	reload,//リロード
	F,//!インタラクト
};

enum class pushButton
{
	Neutral,//!非入力
	rClick,//!右クリック
	lClick,//!左クリック
	lShift,//!左シフト
	space,//!スペースキー
	R,//!Rキー
	F,//!Fキー
};

/**
 * @class PL
 * @brief プレイヤクラス
 */
class PL : public CB
{
public:
	PL() {};
	~PL() {};

	/**
	 * @brief 初期化処理
	 * @return true
	 */
	virtual bool	Initialize();

	/**
	 * @brief 削除処理
	 * @return true
	 */
	virtual bool	Terminate();

	/**
	 * @brief 更新処理
	 * @return true
	 */
	virtual bool	Process();

	/**
	 * @brief 描画処理
	 * @param 再生速度
	 * @return true
	 */
	virtual bool	Render(float timeSpeed);

	/**
	 * @brief キャラ移動処理
	 * @param Speed 移動スピード
	 * @param _Dir 移動方向
	 * @param isAnimChange runアニメーションを再生するか
	 */
	void charMove(float Speed, float _DirX, bool isAnimChange);

	/**
	 * @brief 入力を基準にプレイヤの行動を返す処理
	 * \return 次キャラクタの行動
	 */
	pushButton setAction();

	/**
	 * @brief キーボード/PAD入力のトリガ入力確認処理
	 * @param Key キー
	 * @param Pad ボタン
	 * @return 入力されているか
	 */
	bool checkTrgImput(int Key, int Pad) { if (_imputInf->_gTrgb[Key] || _imputInf->_gTrgp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief キーボード/PAD入力の入力確認処理
	 * @param Key キー
	 * @param Pad ボタン
	 * @return 入力されているか
	 */
	bool checkKeyImput(int Key, int Pad) { if (_imputInf->_gKeyb[Key] || _imputInf->_gKeyp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief キーボード/PAD入力のリリース入力確認処理
	 * @param Key キー
	 * @param Pad ボタン
	 * @return 入力されているか
	 */
	bool checkRelImput(int Key, int Pad) { if (_imputInf->_gRelb[Key] || _imputInf->_gRelp[Pad]) { return true; } else { return false; } }

	/**
	 * @brief Lstick入力からキャラの移動方向を返す処理
	 * @param checkUseCamDir ロックオンカメラ基準で返すかどうか
	 * @return 移動方向
	 */
	float getMoveDir();

	/**
	 * @brief 体力計算処理
	 * @param 数値
	 * @return true
	 */
	bool HPmath(float math, float Stan) override;

	/**
	 * @brief ブラッドポイント計算処理
	 * @param 数値
	 * @return true
	 */
	bool BPmath(float math) override;

	/**
	 * @brief 自機入れ替え技変更処理
	 * @param name 入れ替え技名
	 * @param XorY 2つあるうちどちらの入れ替え技を変えるか
	 * @return true
	 */
	bool CA_change(std::string name, const char* XorY) override;

	/**
	 * @brief 入れ替え技(突き)
	 * @param insPL プレイヤクラスポインタ
	 * @return true
	 */
	static bool CA_charge(PL* insPL);

	/**
	 * @brief 入れ替え技(切り抜け)
	 * @param insPL プレイヤクラスポインタ
	 * @return true
	 */
	static bool CA_kirinuke(PL* insPL);

	/**
	 * @brief 入れ替え技(デバック用攻撃)
	 * @param insPL プレイヤクラスポインタ
	 * @return true
	 */
	static bool CA_debugAttack(PL* insPL);

	/**
	 * @brief 入れ替え技(非設定)
	 * @param insPL プレイヤクラスポインタ
	 * @return false
	 */
	static bool CA_noData(PL* insPL);

	/**
	 * @brief 入れ替え技用関数ポインタ
	 * @param insPL プレイヤクラスポインタ
	 */
	bool (*changeAttackX)(PL* insPL);

	/**
	 * @brief 入れ替え技用関数ポインタ
	 * @param insPL プレイヤクラスポインタ
	 */
	bool (*changeAttackY)(PL* insPL);

	/**
	 * @brief アニメーション総再生時間取得処理
	 * @return アニメーション総再生時間
	 */
	float getAnimPlayTotalTime() { return _modelInf.totalTime; };

protected:
	int airDashCoolTime;
	float spd//!キャラの移動速度
		, dodgeDir;//!キャラの回避方向
	bool isAnimEnd//!アニメーション再生が終わっているか
		, isPushButtonAct//!弱攻撃連打から攻撃受付時間終わりまでに別アクションに切り替えたか
		, isGuard//!ガード中か
		, isFastGuard//!ガード準備中か
		, isBlow;
	float atkBuff = 0.f;//!加算攻撃バフ値
	bool isGhost//!別キャラとのすり抜けを許可するか
		, deadVoice = false//!死亡音声の再生はしたか
		, lClickTrgCheck;
	int immortalTime//!無敵時間
		, isCounter = 0;//!カウンター状態か
	int walkTime//!キャラ移動開始からの経過時間
		, counterTime//!カウンター再生時間
		, waitCAChargeTime//!入れ替え技での移動までの待機時間
		, CAChargeTime//!入れ替え技での移動時間
		, CAChargeSpd//!入れ替え技での移動スピード
		, CAChargeAttackNum = 0;
	ExclusiveState Estate = ExclusiveState::NORMAL//!キャラの行動状態インスタンス
		, oldEstate = ExclusiveState::NORMAL//!1つ前のキャラの行動状態インスタンス
		, lastAttackState;//!キャラの最終攻撃インスタンス
	pushButton nextKey;//!先行入力用インスタンス
	modelInf gun;
};
