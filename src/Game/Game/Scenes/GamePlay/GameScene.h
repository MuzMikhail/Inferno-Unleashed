#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		class GameScene : public Scene::Base
		{
		public:
			BP_CLASS_TYPE( GameScene, Scene::Base );

			std::string Name()
			{
				return "GameSample/GameScene";
			}

			bool Init() override;

			void Update( [[maybe_unused]] float delta ) override;

			void Draw() override;

			void Exit() override;

			void GUI() override;

		private:
			//bool game_start = false;
		};

	}	// namespace GameSample
}	// namespace Sample
