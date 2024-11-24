#include "Camera.h"
#include <System/Component/ComponentCollisionSphere.h>
#include <System/Component/ComponentSpringArm.h>

#define USE_MOUSE_CAMERA

namespace Sample
{
	namespace GameSample
	{
		BP_OBJECT_IMPL( Camera, u8"GameSample/Camera" );

		// カメラの作成 // Конструирование новой камеры
		CameraPtr Camera::Create( ObjectPtr obj )
		{
			auto camobj = Scene::CreateObject<Camera>();
			auto camera = camobj->AddComponent<ComponentCamera>();
			// カメラの位置とターゲットの設定 // Установление позиции и цели камеры
			camera->SetPositionAndTarget( { 0, -0.2, -1 }, { 0, 0, 0 } );

			auto col = camobj->AddComponent<ComponentCollisionSphere>();
			// 突挫スフィアの半径の設定 // Установление радиуса сферы столкновения
			col->SetRadius( 2.0f );
			// 負荷の設定を零に // Установление массы на нуль
			col->SetMass( 0.0f );

			auto spring_arm = camobj->AddComponent<ComponentSpringArm>();

			// スプリングアームのオブジェクトの設定 // Установление объекта для присоединенной руки
			spring_arm->SetSpringArmObject( obj );

			// スプリングアームの向きを設定 // Установление направления руки пружины
			spring_arm->SetSpringArmVector( { 0, 1.2, 2 } );
			// スプリングアームの長さを設定 // Установление длины руки пружины
			spring_arm->SetSpringArmLength( 30 );
			// スプリングアームのオフセットを設定 // Установление смещения пружинной руки
			spring_arm->SetSpringArmOffset( { 0, 4, 0 } );

			// カメラオブジェクトを返す // Возврат объекта камеры
			return camobj;
		}

#ifdef USE_MOUSE_CAMERA
		// カメラの更新 // Обновление камеры
		void Camera::Update( float delta )
		{
			auto spring_arm = GetComponent<ComponentSpringArm>();
#if 1
			// 初期の向きのベクトル // Вектор начального направления
			float3 vec( 0.0f, 0.0f, 1.0f );

			// Y軸のカメラの旋回をマウスの動きに応じて設定 // Поворот камеры по оси X в зависимости от движения мыши
			pitch_ += radians( (float1)GetMouseMoveY() * 0.2f );
			// X軸のカメラの旋固をマウスの動きに応じて設定 // Поворот камеры по оси Y в обратном направлении от движения мыши
			head_ += radians( (float1)GetMouseMoveX() * -0.2f );

			// カメラの傾斜角の制限 // Ограничение угла наклона камеры
			constexpr f32 ANGLE_LIMIT = PI * 0.5f * 0.99f;

			// 旋固角を制限して不自然な傾斜を防ぐ // Ограничение угла для предотвращения сломанного кульбита
			pitch_ = std::clamp( pitch_, -ANGLE_LIMIT, +ANGLE_LIMIT );

			// X軸の回転を使用してベクトルを旋固 // Вращение вектора по оси X
			vec = mul( matrix::rotateX( pitch_ ), float4( vec, 0 ) ).xyz;
			// Y軸の回転を使用してベクトルを旋固 // Вращение вектора по оси Y
			vec = mul( matrix::rotateY( head_ ), float4( vec, 0 ) ).xyz;
			// スプリングアームの向きを設定 // Установление направления пружинной руки
			spring_arm->SetSpringArmVector( vec );
		}
#else
			// ベクトルを標準化 // Нормирование вектора
			vec_ = normalize( vec_ );
			// X軸の回転をマウスの動きによって設定 // Поворот по X с учетом мыши
			vec_ = mul( matrix::rotateX( radians( (float1)GetMouseMoveY() * 0.2f ) ), float4( vec_, 0 ) ).xyz;
			// Y軸の回転をマウスの動きによって設定 // Поворот по Y с учетом мыши
			vec_ = mul( matrix::rotateY( radians( (float1)GetMouseMoveX() * -0.2f ) ), float4( vec_, 0 ) ).xyz;
			// スプリングアームの向きを設定 // Установление направления пружинной руки
			spring_arm->SetSpringArmVector( vec_ );
#endif

#endif

	}	// namespace GameSample
}	// namespace Sample
