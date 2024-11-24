#include "Gauge.h"
#include <System/SystemMain.h>
#include <DxLib.h>

namespace Sample::GameSample
{
	namespace HP
	{
		// HPゲージの設定 // Настройки полоски HP
		constexpr const int BAR_W		 = 320;	  // ゲージの幅 // Ширина полоски HP
		constexpr const int BAR_H		 = 50;	  // ゲージの高さ // Высота полоски HP
		constexpr const int BAR_MARGIN_R = 20;	  // 右端からの余白 // Отступ от правого края

		// ゲージの色 // Цвета полоски
		const u32 BAR_BASE_COLOR	= GetColor( 75, 75, 75 );	// ベースカラー // Базовый цвет фона
		const u32 BAR_DAMAGE_COLOR	= RED;						// ダメージカラー // Цвет урона
		const u32 BAR_RECOVER_COLOR = GREEN;					// 回復カラー // Цвет восстановления
		const u32 BAR_NORMAL_COLOR	= YELLOW;					// 通常HPのカラー // Обычный цвет HP
		const u32 BAR_SIDE_COLOR	= WHITE;					// 枠線のカラー // Цвет рамки

		// ゲージアニメーションの動作時間 // Время работы анимации полоски
		constexpr const float BAR_PROC_TIME = 0.5f;	  // 処理時間 // Время обработки
		constexpr const float BAR_UPDOWN_TIME = 2.0f;	// 上下アニメーション時間 // Время анимации вверх/вниз

		// HPゲージのコンストラクタ // Конструктор для инициализации полоски HP
		Gauge::Gauge( float hp )
			: hp_( hp )
			, damage_( hp )
			, org_hp_( hp )
		{
			pos_ = float3( WINDOW_W - ( BAR_W + BAR_MARGIN_R ), 0, 75 );   // ゲージの位置 // Позиция полоски
		}

		Gauge::~Gauge()
		{
		}

		// HPゲージの更新 // Обновление состояния полоски HP
		void Gauge::Update( float delta )
		{
			procDamageGauge( delta );						   // ダメージ処理 // Обработка урона
			procRecoverGauge( delta );						   // 回復処理 // Обработка восстановления
			hpbar = LoadGraph( "data/Sample/UI/barhp.png" );   // ゲージ画像の読み込み // Загрузка графики полоски
		}

		// HPゲージの描画 // Рисование полоски HP
		void Gauge::Draw()
		{
			int x = static_cast<int>( pos_.x );
			int y = static_cast<int>( pos_.z );

			DrawBox( x, y, x + BAR_W, y + BAR_H, BAR_BASE_COLOR, TRUE );   // 下地 // Фон
			DrawBox( x,
					 y,
					 x + static_cast<int>( BAR_W * ToPerValue( damage_ ) ),
					 y + BAR_H,
					 BAR_DAMAGE_COLOR,
					 TRUE );   // ダメージ部分 // Урон

			// 現在のHPと回復の描画 // Рисование текущего HP и восстановления
			if( recover_delta_ )
			{
				DrawBox( x,
						 y,
						 x + static_cast<int>( BAR_W * ToPerValue( recover_ ) ),
						 y + BAR_H,
						 BAR_NORMAL_COLOR,
						 TRUE );   // 回復部分 // Восстановление
				DrawBox( x,
						 y,
						 x + static_cast<int>( BAR_W * ToPerValue( hp_ ) ),
						 y + BAR_H,
						 BAR_RECOVER_COLOR,
						 TRUE );   // 現在のHP // Текущее HP
			}
			else
			{
				DrawBox( x,
						 y,
						 x + static_cast<int>( BAR_W * ToPerValue( hp_ ) ),
						 y + BAR_H,
						 BAR_NORMAL_COLOR,
						 TRUE );   // 現在のHP // Текущее HP
			}

			DrawBox( x, y, x + BAR_W, y + BAR_H, BAR_SIDE_COLOR, FALSE );	// フチ // Рамка
			DrawExtendGraphF( 1225, 0, WINDOW_W, 200, hpbar, TRUE );		// HPバーの画像 // Графика полоски HP
		}

		// HPを全回復する // Полное восстановление HP
		void Gauge::Reset()
		{
			hp_		= org_hp_;
			damage_ = hp_;
		}

		// HPのインクリメントとデクリメントのオペレーター // Операторы инкремента и декремента для HP
		Gauge& Gauge::operator++()
		{
			*this += 1;
			return *this;
		}
		Gauge Gauge::operator++( int )
		{
			Gauge ret = *this;
			*this += 1;
			return ret;
		}
		Gauge& Gauge::operator--()
		{
			*this -= 1;
			return *this;
		}
		Gauge Gauge::operator--( int )
		{
			Gauge ret = *this;
			*this -= 1;
			return ret;
		}

		// HPを減らす処理 // Уменьшение HP
		void Gauge::operator-=( int value )
		{
			if( hp_ <= 0 )
				return;

			damage_frame_ = BAR_PROC_TIME;	 // ダメージフレーム設定 // Установка времени анимации урона
			hp_ -= value;
			if( hp_ <= 0 )
				hp_ = 0;

			float delta	   = GetDeltaTime();   // 時間のデルタを取得 // Получение разницы во времени
			float delta_up = delta / BAR_UPDOWN_TIME;	// 時間差の計算 // Расчёт разницы во времени
			damage_delta_ = static_cast<float>( damage_ - hp_ ) * delta_up;	  // ダメージデルタの計算 // Расчёт разницы урона
			recover_	   = hp_;
			recover_delta_ = 0.0f;
		}

		// HPを増やす処理 // Увеличение HP
		void Gauge::operator+=( int value )
		{
			if( hp_ >= org_hp_ )
				return;

			hp_ += value;
			if( hp_ >= org_hp_ )
				hp_ = org_hp_;

			float delta	   = GetDeltaTime();   // 時間のデルタを取得 // Получение разницы во времени
			float delta_up = delta / BAR_UPDOWN_TIME;	// 時間差の計算 // Расчёт разницы во времени
			recover_delta_ = static_cast<float>( hp_ - recover_ ) * delta_up;	// 回復デルタの計算 // Расчёт разницы восстановления
		}

		// HPの比較オペレーター // Операторы сравнения для HP
		bool Gauge::operator<=( int value )
		{
			return hp_ <= value;
		}
		bool Gauge::operator<( int value )
		{
			return hp_ < value;
		}
		bool Gauge::operator>=( int value )
		{
			return hp_ >= value;
		}
		bool Gauge::operator>( int value )
		{
			return hp_ > value;
		}
		bool Gauge::operator==( int value )
		{
			return hp_ == value;
		}
		bool Gauge::operator!=( int value )
		{
			return !( *this == value );
		}

		// HPをパーセントに変換する // Преобразование значения HP в процент
		float Gauge::ToPerValue( float value )
		{
			return ( value / org_hp_ );
		}

		// ダメージゲージの処理 // Обработка анимации урона
		void Gauge::procDamageGauge( float delta )
		{
			damage_frame_ -= delta;	  // ダメージフレームを減らす // Уменьшение времени анимации урона
			if( damage_frame_ > 0 )
				return;

			damage_ -= damage_delta_;	// ダメージを減らす // Уменьшение урона
			if( damage_ <= hp_ )
			{
				damage_		  = hp_;
				damage_delta_ = 0.0f;
			}
		}

		// 回復ゲージの処理 // Обработка анимации восстановления
		void Gauge::procRecoverGauge( float delta )
		{
			recover_frame_ -= delta;   // 回復フレームを減らす // Уменьшение времени анимации восстановления
			if( recover_frame_ > 0 )
				return;

			recover_ += recover_delta_;	  // 回復を増やす // Увеличение восстановления
			if( recover_ >= hp_ )
			{
				recover_	   = hp_;
				recover_delta_ = 0.0f;
			}
		}

	}	// namespace HP
}	// namespace Sample::GameSample
