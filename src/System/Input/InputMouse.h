﻿//---------------------------------------------------------------------------
//!	@file	InputMouse.h
//! @brief	マウス入力管理
//---------------------------------------------------------------------------
#pragma once

//===========================================================================
//!	@name	システム関数
//===========================================================================
//@{

//! 初期化
void InputMouseInit();
//! 更新
void InputMouseUpdate();
//! 終了
void InputMouseExit();

//@}
//===========================================================================
//!	@name	入力関連関数
//===========================================================================
//@{

//! 指定マウスボタンの1回だけ押下検証
//! @param	[in]	mouseID	マウスのボタン種別
//! @retval true	引数に指定されたマウスのボタンが押された
//! @retval false	引数に指定されたマウスのボタンが押されていない
bool IsMouseOn( int mouseID );

//! 指定マウスボタンが押されていない検証
//! @param	[in]	mouseID	マウスのボタン種別
//! @retval true	引数に指定されたマウスのボタンが離されている
//! @retval false	引数に指定されたマウスのボタンが押されている
bool IsMouseRelease( int mouseID );

//! 指定マウスボタンの長押し検証
//! @param	[in]	mouseID	マウスのボタン種別
//! @retval true	引数に指定されたマウスのボタンが長押し（2フレーム以上）されている
//! @retval false	引数に指定されたマウスのボタンが押されていない（押されているフレーム数が1以下）
bool IsMouseRepeat( int mouseID, u32 frame = 2 );

// Windowsポンプ命名規則
//! 指定マウスボタンの1回だけ押下検証
//! @param	[in]	mouseID	マウスのボタン種別
//! @retval true	引数に指定されたマウスのボタンが押された
//! @retval false	引数に指定されたマウスのボタンが押されていない
bool IsMouseDown( int mouseID );

//! 指定マウスボタンが押されていない検証
//! @param	[in]	mouseID	マウスのボタン種別
//! @retval true	引数に指定されたマウスのボタンが離されている
//! @retval false	引数に指定されたマウスのボタンが押されている
bool IsMouseUp( int mouseID );

//! 指定マウスボタンの検証
//! @param	[in]	mouseID	マウスのボタン種別
//! @retval true	引数に指定されたマウスのボタンが長されている
//! @retval false	引数に指定されたマウスのボタンが押されていない
bool IsMouse( int mouseID );

//@}
//===========================================================================
//!	@name	マウス位置関数
//===========================================================================
//@{

//! マウスのX座標 取得
//! @return マウスのX座標
int GetMouseX();
//! マウスのY座標 取得
//! @return マウスのY座標
int GetMouseY();

//! マウスのX座標 取得
//! @return マウスのX移動量
int GetMouseMoveX();
//! マウスのY座標 取得
//! @return マウスのY移動量
int	 GetMouseMoveY();
void LimitMouseCursor();
//@}
void HideMouse( bool _hide = true );
