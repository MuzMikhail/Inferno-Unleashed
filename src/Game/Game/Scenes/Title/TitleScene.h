#include <System/Scene.h>

namespace Sample
{
	namespace GameSample
	{
		class TitleScene : public Scene::Base
		{
		public:
			BP_CLASS_TYPE( TitleScene, Scene::Base );

			std::string Name()
			{
				return "GameSample/TitleScene";
			}

			bool Init() override;

			void Update( [[maybe_unused]] float delta ) override;

			void Draw() override;

			void Exit() override;

			void GUI() override;

		private:
			int	 imageHandle;
			int	 imgLoad;
			int	 imgGameTit;
			int	 imgCursor;
			int	 startButtonImage;
			int	 optionButtonImage;
			int	 creditsButtonImage;
			int	 exitButtonImage;
			int	 startButtonPImage;
			int	 optionButtonPImage;
			int	 creditsButtonPImage;
			int	 loadbar;
			int	 loadbarF;
			int	 exitButtonPImage;
			int	 tutorialImage;
			int	 tutorialText;
			bool hide		  = true;
			bool showTutorial = false;
			int	 fontSize	  = 40;

			int	 img			 = imageHandle;
			bool loading		 = false;
			int	 windowWidth	 = WINDOW_W;
			int	 windowHeight	 = WINDOW_H;
			int	 mouseX			 = GetMouseX();
			int	 mouseY			 = GetMouseY();
			int	 startButtonX	 = 150;
			int	 startButtonY	 = 500;
			int	 optionButtonX	 = 150;
			int	 optionButtonY	 = 600;
			int	 creditsButtonX	 = 150;
			int	 creditsButtonY	 = 700;
			int	 exitButtonX	 = 150;
			int	 exitButtonY	 = 800;
			int	 alphaValue		 = 0;
			bool increasingAlpha = true;
		};

	}	// namespace GameSample
}	// namespace Sample
