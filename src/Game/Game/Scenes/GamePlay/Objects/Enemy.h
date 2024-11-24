#pragma once

#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		USING_PTR( Enemy );

		class Deathbody;

		// 指定されたオブジェクトに対して最も近い敵を返す // Возвращает ближайшего врага по отношению к заданному объекту
		ObjectPtr TrackingNearEnemy( ObjectPtr obj );

		// 敵クラス // Класс врага
		class Enemy : public Object
		{
		public:
			BP_OBJECT_TYPE( Enemy, Object );

			// 指定された位置と方向で敵オブジェクトを作成する // Создает экземпляр врага в указанной позиции и направлении
			static EnemyPtr Create( const float3& pos, const float3& front = { 0, 0, 1 } );

			// 敵オブジェクトの初期化 // Инициализация объекта врага
			bool Init() override;

			// 各フレームごとに敵の状態を更新する // Обновление состояния врага на каждом кадре
			void Update( float delta ) override;

			// 敵のためのGUI描画 // Обработка отображения GUI для врага
			void GUI() override;

			// 他のオブジェクトとの衝突コールバック // Коллбэк при столкновении с другими объектами
			void OnHit( const ComponentCollision::HitInfo& hitInfo ) override;

			// 攻撃を実行する // Выполняет атаку
			void Attack();

			// 敵のスピードを設定する // Устанавливает скорость врага
			void SetSpeed( float s );

			// 現在のスピードを取得する // Возвращает текущую скорость врага
			float GetSpeed();

			// スピード変数への直接アクセス // Доступ к переменной скорости
			float& Speed();

			// ダメージ処理 // Обработка урона
			void TakeDamage( int damage );

			// 死亡処理 // Обработка смерти
			void OnDeath();

		private:
			float					 speed_	   = 1.0f;	 // スピード // Скорость врага
			float					 rot_y_	   = 0.0f;	 // 回転角 // Угол поворота по оси Y
			int						 hp_enemy  = 5;		 // ヒットポイント // Очки здоровья
			int						 hitCount_ = 0;		 // 被弾数 // Счетчик попаданий
			std::weak_ptr<Deathbody> deadbody_;	  // 死体オブジェクト // Ссылка на объект "тело" после смерти
			float					 lastAttackTime = 0;   // 最後の攻撃時間 // Время последней атаки
			float timeSinceLastAttack = 0.0f;	// 攻撃後の経過時間 // Время с момента последней атаки

		public:
			int effect_hit_		 = -1;	 // ヒットエフェクト // Эффект при попадании
			int effect_play_hit_ = -1;	 // ヒットエフェクト再生 // Эффект проигрывания при попадании
			float3		position_h_		= { 0, 0, 0 };	 // 位置 // Позиция
			const float ATTACK_COOLDOWN = 1.2f;			 // 攻撃のクールダウン // Задержка между атаками

		private:
			// 衝突チェック // Проверка столкновения
			bool CheckCollision( float3 position, float3 direction, float distance, float& outSlopeAngle );

			// 反発ベクトル取得 // Получение вектора отталкивания
			float3 GetRepulsionVector( float3 position, float3 direction );

			// 衝突位置補正 // Коррекция позиции при столкновении
			void CorrectPositionIfColliding();

			// 代替経路探索 // Поиск альтернативного пути
			bool FindAlternativePath( float3& move, float3 position, float3 direction );
		};
	}	// namespace GameSample
}	// namespace Sample
