#include "Precompile.h"
#include "Deathbody.h"
#include "Enemy.h"
#include "Sorc.h"

#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentTargetTracking.h>
#include <System/Component/ComponentSpringArm.h>

#include <cmath>

namespace Sample
{
	namespace GameSample
	{

		DeathbodyPtr Deathbody::Create( const float3& pos, const float3& front )
		{
			// 箱の作成 // Создание объекта
			auto dbody = Scene::CreateObject<Deathbody>();
			dbody->SetName( "Deathbody" );	 // 名前の設定 // Установление имени объекта

			// vecの方向に向ける // Установление направления по вектору
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			dbody->SetMatrix( mat );   // 行列の設定 // Установление матрицы

			// posの位置に設定 // Установление позиции объекта
			dbody->SetTranslate( pos );

			return dbody;	// オブジェクトを返す // Возврат объекта
		}

		//! @brief 初期化 // Инициализация
		//! @return 初期化できたかどうか // Возвращает результат инициализации
		bool Deathbody::Init()	 // override
		{
			__super::Init();

			// モデルコンポーネント(0.08倍) // Добавление компонента модели (масштаб 0.08)
			auto model = AddComponent<ComponentModel>( "data/Sample/Enemy/char/Zomb.mv1" );

			model->SetScaleAxisXYZ( { 0.08f } );   // スケールの設定 // Установление масштаба модели

			model->SetAnimation( { { "death",
									 "data/Sample/Enemy/char/Anim/zombi_death.mv1",
									 0,
									 1.0f } } );   // アニメーションの設定 // Установление анимации

			if( model )
				model->PlayAnimation( "death", false );	  // アニメーション再生 // Запуск анимации смерти

			return true;   // 初期化の結果を返す // Возврат результата инициализации
		}

		//! @brief 処理更新 // Обновление обработки
		//! @param delta フレーム時間 // Время кадра
		void Deathbody::Update( [[maybe_unused]] float delta )	 // override
		{
		}

		void Deathbody::SetPositionAndDirection( const float3& handPos, const float3& vec )
		{
			SetTranslate( handPos + float3( 0, 0, 0 ) );   // 位置の設定 // Установление позиции
			vec_ = normalize( vec );					   // ベクトルの正規化 // Нормирование вектора
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE とセットで必要 (Editor上でCreateObjectするため) // Требуется с BP_OBJECT_TYPE для создания объекта в редакторе
BP_OBJECT_IMPL( Sample::GameSample::Deathbody, u8"Deathbody" );

// オブジェクトセーブ構造を追加します // Добавление структуры для сохранения объекта
CEREAL_REGISTER_TYPE( Sample::GameSample::Deathbody )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Deathbody )
