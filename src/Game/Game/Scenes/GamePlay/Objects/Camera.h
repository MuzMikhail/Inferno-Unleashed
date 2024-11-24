#include <System/Scene.h>
#define USE_MOUSE_CAMERA

namespace Sample
{
	namespace GameSample
	{

		USING_PTR( Camera );

		// カメラ // Камера
		class Camera : public Object
		{
		public:
			BP_OBJECT_TYPE( Camera, Object );

			// カメラオブジェクトを作成するメソッド // Метод для создания объекта камеры
			static CameraPtr Create( ObjectPtr obj );

#ifdef USE_MOUSE_CAMERA
			// 更新処理を行う // Выполняет обновление состояния
			void Update( float delta ) override;
#endif

		private:
			float head_	 = 0.0f;   // カメラの水平方向の回転 // Горизонтальное вращение камеры
			float pitch_ = 0.0f;   // カメラの垂直方向の回転 // Вертикальное вращение камеры
		};

	}	// namespace GameSample
}	// namespace Sample
