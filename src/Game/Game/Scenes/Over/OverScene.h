#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		class OverScene : public Scene::Base
		{
		public:
			BP_CLASS_TYPE( OverScene, Scene::Base );

			std::string Name()
			{
				return "GameSample/OverScene";
			}

			bool Init() override;

			void Update( [[maybe_unused]] float delta ) override;

			void Draw() override;

			void Exit() override;

			void GUI() override;

		private:
			int deathimg;
		};



	}	// namespace GameSample
}	// namespace Sample
