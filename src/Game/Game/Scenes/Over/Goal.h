#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		class Goal : public Scene::Base
		{
		public:
			BP_CLASS_TYPE( Goal, Scene::Base );

			std::string Name()
			{
				return "GameSample/Goal";
			}

			bool Init() override;

			void Update( [[maybe_unused]] float delta ) override;

			void Draw() override;

			void Exit() override;

			void GUI() override;

		private:
			int win;
			int winText;
		};



	}	// namespace GameSample
}	// namespace Sample
