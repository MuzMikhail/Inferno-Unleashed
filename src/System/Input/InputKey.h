﻿//---------------------------------------------------------------------------
//!	@file	InputKey.h
//! @brief	キー入力管理
//---------------------------------------------------------------------------
#pragma once

namespace Input
{
	//===========================================================================
	//!	@name	システム関数
	//===========================================================================
	//@{

	//! 初期化
	void InputKeyInit();

	//! 更新
	int InputKeyUpdate();

	//! 終了
	void InputKeyExit();

	//@}
	//===========================================================================
	//!	@name	入力関連関数
	//===========================================================================
	//@{

	//! 指定キーの1回だけ押下検証
	//! @param	[in]	keyID	キー種別
	//! @retval true	引数に指定されたキーが押された
	//! @retval false	引数に指定されたキーが押されていない
	bool IsKeyOn( int keyID );

	//! 指定キーが押されていない検証
	//! @param	[in]	keyID	キー種別
	//! @retval true	引数に指定されたキーが離されている
	//! @retval false	引数に指定されたキーが押されている
	bool IsKeyRelease( int keyID );

	//! 指定キーの長押し検証
	//! @param	[in]	keyID	キー種別
	//! @param  [in]    frame   指定フレーム以上押されている[Default=2]
	//! @retval true	引数に指定されたキーが長押し（指定フレーム以上）されている
	//! @retval false	引数に指定されたキーが押されていない（押されているフレーム数が指定数未満）
	bool IsKeyRepeat( int keyID, u32 frame = 2 );

	//! 指定キーの長押し検証
	//! @param	[in]	keyID	キー種別
	//! @retval true	引数に指定されたキーが押しされている
	//! @retval false	引数に指定されたキーが押されていない
	bool IsKey( int keyID );

	//---------------------------------------------------------------------------
	// Windowsポンプに合わせた命名
	//---------------------------------------------------------------------------

	//! 指定キーの1回だけ押下検証
	//! @param	[in]	keyID	キー種別
	//! @retval true	引数に指定されたキーが押された
	//! @retval false	引数に指定されたキーが押されていない
	bool IsKeyDown( int keyID );

	//! 指定キーが押されていない検証
	//! @param	[in]	keyID	キー種別
	//! @retval true	引数に指定されたキーが離されている
	//! @retval false	引数に指定されたキーが押されている
	bool IsKeyUp( int keyID );

	//@}
}	// namespace Input

using namespace Input;