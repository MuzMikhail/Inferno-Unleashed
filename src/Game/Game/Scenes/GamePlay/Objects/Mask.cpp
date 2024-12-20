#include "Sorc.h"
#include "Mask.h"

#include <System/Component/ComponentModel.h>
#include <System/Component/ComponentCollisionModel.h>
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentCollisionCapsule.h>
#include <System/Component/ComponentCamera.h>
#include <System/Component/ComponentTargetTracking.h>
#include <System/Component/ComponentSpringArm.h>
#include <System/Component/ComponentAttachModel.h>

#include <cmath>

namespace Sample
{
	namespace GameSample
	{

		MaskPtr Mask::Create( const float3& pos, const float3& front )
		{
			// 箱の作成 // Создание объекта
			auto goal = Scene::CreateObject<Mask>();
			goal->SetName( "Mask" );   // 名前の設定 // Установление имени объекта

			// vecの方向に向ける // Установление направления по вектору
			auto mat = HelperLib::Math::CreateMatrixByFrontVector( front );
			goal->SetMatrix( mat );	  // 行列の設定 // Установление матрицы

			// posの位置に設定 // Установление позиции объекта
			goal->SetTranslate( pos );

			return goal;   // オブジェクトを返す // Возврат объекта
		}

		bool Mask::Init()	// override
		{
			__super::Init();

			// モデルコンポーネント(0.03倍) // Добавление компонента модели (масштаб 0.03)
			auto model = AddComponent<ComponentModel>( "data/Sample/Key/source/death_mask.mv1" );

			model->SetScaleAxisXYZ( { 0.03f } );   // スケールの設定 // Установление масштаба модели
			model->SetTranslate( { 0, 6, 0 } );	   // トランスレートの設定 // Установление трансляции


			return true;   // 初期化の結果を返す // Возврат результата инициализации
		}

		//! @brief 処理更新 // Обновление обработки
		//! @param delta フレーム時間 // Время кадра
		void Mask::Update( [[maybe_unused]] float delta )	// override
		{
			auto mdl = GetComponent<ComponentModel>();
		}

		//! @brief 当たりのコールバック // Колбэк при столкновении
		//! @param hitInfo // Информация о столкновении
		void Mask::OnHit( [[maybe_unused]] const ComponentCollision::HitInfo& hitInfo )	  // override
		{
			// 当たりで移動させる(これが無ければめり込みます) // Обработка столкновения (без этого объект проваливается)
			__super::OnHit( hitInfo );
		}

		void Mask::SetPositionAndDirection( const float3& handPos, const float3& vec )
		{
			SetTranslate( handPos + float3( 0, -1, 0 ) );	// 位置の設定 // Установление позиции
			vec_ = normalize( vec );						// ベクトルの正規化 // Нормирование вектора
		}

		void Mask::SetDelay( float delay )
		{
			delay_ = delay;	  // 遅延の設定 // Установление задержки
		}

	}	// namespace GameSample
}	// namespace Sample

// BP_OBJECT_TYPE とセットで必要 (Editor上でCreateObjectするため) // Требуется с BP_OBJECT_TYPE для создания объекта в редакторе
BP_OBJECT_IMPL( Sample::GameSample::Mask, u8"Mask" );

// オブジェクトセーブ構造を追加します // Добавление структуры для сохранения объекта
CEREAL_REGISTER_TYPE( Sample::GameSample::Mask )
CEREAL_REGISTER_POLYMORPHIC_RELATION( Object, Sample::GameSample::Mask )
