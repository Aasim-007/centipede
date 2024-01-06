#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unistd.h>
#include <fstream> //For highscore [File handling]


// Initializing Dimensions.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameColumns][gameRows] = {};

// The following exist purely for readability ---------------------------------------------------------------------------
/*Implementing values for these elements on every creature was confusing, so I introduced additional constants to simplify the process.*/
const int x = 0;
const int y = 1;
const int exists = 2;
const int is_head = 3;
const int xdir = 4;//Direction ; 0 for left, 1 for right
const int ydir = 5;//Direction ; 0 for up, 1 for down
const int eat_mush = 6; //to save count of mushroom centipede has eat

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                        FUNCTIONS                                                     //
void inputKeyboard(float player[],float bullet[],sf::Keyboard& Key,sf::Clock& Delay,int mushroom[][exists],bool is_mush_poisonous[],int& total_mushrooms,bool& playerDied); //To take input from User

void drawMushrooms(sf::RenderWindow& window, sf::Sprite& mushroomSprite, int mushroom[][exists],int currentmush_shape[],bool is_mush_poisonous[],int total_mushrooms); //To Draw Mushroom (can be anytime thus made it function)

void hitMushroom(float bullet[], int mushroom[][exists],int currentmush_shape[],bool is_mush_poisonous[],int& total_mushrooms,int& score); //To detect collion of Mushroom with bullet 

void spawnCenti(int centipede[][7],int total_centi); //To delopy centipede

void moveCentipede(int centipede[][7],int& total_centi,sf::Clock& centipedeClock,int mushroom[][exists],int currentmush_shape[],int& total_mushrooms,bool is_mush_poisonous[],bool& centi_touched_floor,float centi_speed,sf::Sound& eat_se,bool sounde_on, int speed_up=0); //to Move centipede 

void drawCentipede(int centipede[][7],int total_centi,sf::RenderWindow& window,sf::Sprite& head_centiSprite, sf::Sprite& body_centiSprite, sf::Clock& centipedeClock,float centi_speed); //to display centipede (clock was necessary, and to ensure proper management, I converted it into a function.)

void hitCentipede(int centipede[][7],int& total_centi,float bullet[], int mushroom[][exists],int currentmush_shape[],bool is_mush_poisonous[],int& total_mushrooms,int level,int& score); //To detect collion of Centipede fragment with bullet

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite); //to display PLayer

void moveBullet(float bullet[], sf::Clock& bulletClock); //to move bullet 

void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite); //to display bullet

void changeSprite_color(sf::Texture& texture,sf::Color greenChange,sf::Color redChange,sf::Color whiteChange); //to change color of a sprite according to difficulty mode

void updateLives(int& lives, int score); //updates lives acoorrding to score

void drawLives(int& lives,sf::RenderWindow& window, sf::Sprite& playerSprite); //to diaplay live on screen (made it funtion so that main code can be clean and easy to follow through)

void playerDeath(float player[],bool& playerDied); //to check death of player

void drawObject(int arr[], float speed, sf::RenderWindow& window,sf::Sprite& ObjSprite, sf::Clock& animation_Clock,int val); //this funtion draws creatures, Will work for All Spider, Flea and Scorpion (Had to utilize a single function three times, hence converted it into a dedicated function)

void moveSpider(int spider[], float spider_speed, sf::Clock& spiderClock, int mushroom[][exists],int currentmush_shape[],int& total_mushrooms,bool is_mush_poisonous[],sf::Sound& spider_se,sf::Sound& eat_se,bool sounde_on); //for the movement of spider 

void hitSpider(float player[],float bullet[],int spider[],int& score, sf::RenderWindow& window,sf::Sprite& spiderSprite,sf::Clock& hit_spiderClock,sf::Sound& kill_se,bool sounde_on); // to detect collision of spider with bullet

void moveFlea(int flea[], float flea_speed, sf::Clock& fleaClock, int mushroom[][exists],int currentmush_shape[],int& total_mushrooms,bool is_mush_poisonous[],sf::Sound& flea_se, bool sounde_on); // movement of flea 

void hitFlea(float bullet[],int flea[],int& score,sf::Sound& kill_se,bool sounde_on); // to detect collision of flea with bullet 

void moveScorpion(int scorpion[], float scorpion_speed, sf::Clock& scorpionClock, int mushroom[][exists],int& total_mushrooms,bool is_mush_poisonous[],sf::Sound& scorpion_se, bool sounde_on);  // movement of scorpion 

void hitScorpion(float bullet[],int scorpion[],int& score,sf::Sound& kill_se,bool sounde_on); // to detect collision of scorpion with bullet

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	//Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	// window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	 //window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));
	
	int sounde_vol=50;
	bool sounde_on=1;
	int music_vol=50;
	bool music_on=1;
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FOLLOWING CODE IS FOR TEXT,FONT AND SOUND EFFECTS FOR DIFFERENT MENUS/SCREEN THAT I HAVE IN GAME ////////////////////////
	// Main options ------------------------------------------------------------------------------------------------------------ 
	bool Show_Main=1;
	int Main_option=1;
	sf::Text centi, beginner, intermediate, expert, modetext, maintext, notetext;
	sf::Font font1, font2, font3,font4;
	font1.loadFromFile("Fonts/Grenze-Black.ttf");
	font2.loadFromFile("Fonts/Roboto-Light.ttf");
	font3.loadFromFile("Fonts/Poppins-ExtraLightItalic.ttf");
	font4.loadFromFile("Fonts/blockletter.regular.otf");
	
	centi.setFont(font1);
	centi.setString("Centipede");
    centi.setCharacterSize(200);
    centi.setFillColor(sf::Color::White);
    centi.setOutlineColor(sf::Color(127, 219, 132));
    centi.setOutlineThickness(2);
    centi.setPosition(80, 200);
	
	modetext.setFont(font2);
	modetext.setString("Select Difficulty Mode :");
    modetext.setCharacterSize(30);
    modetext.setFillColor(sf::Color(167, 231, 225));
	modetext.setPosition(335, 525);
	
	beginner.setFont(font1);
	beginner.setString("Beginner");
	beginner.setPosition(150, 600);
    beginner.setCharacterSize(35);
    beginner.setFillColor(sf::Color::White);
	
	intermediate.setFont(font1);
	intermediate.setString("Intermediate");
	intermediate.setPosition(380, 600);
    intermediate.setCharacterSize(35);
    intermediate.setFillColor(sf::Color::White);
	
	expert.setFont(font1);
	expert.setString("Expert");
	expert.setPosition(670, 600);
    expert.setCharacterSize(35);
    expert.setFillColor(sf::Color::White);
	
	maintext.setFont(font1);
	maintext.setCharacterSize(35);
	maintext.setFillColor(sf::Color(127, 219, 132));
	
	notetext.setFont(font3);
    notetext.setCharacterSize(20);
    notetext.setFillColor(sf::Color::White);
    notetext.setOutlineColor(sf::Color::White);
    notetext.setOutlineThickness(0.2);
	notetext.setPosition(335, 525);
	
	
	// Escape options (ESCAPE MENU) ------------------------------------------------------------------------------------------- 
	bool Restart=0;
	int Escape_option=1,Escape_option_2=1;
	sf::Text title, resume, restart, restarttxt, settings, help, highscores, return_to_main, exit, esctext, game , paused, back, state_music, state_se, mu_vol, state_music_t, mu_vol_t, se_vol, state_se_t, se_vol_t;
	
	state_music.setFont(font2);
	state_music.setString("State : ");
	state_music.setPosition(200, 425);
    state_music.setCharacterSize(35);
    state_music.setFillColor(sf::Color(167, 231, 225));
    
    mu_vol.setFont(font2);
	mu_vol.setString("Volume : ");
	mu_vol.setPosition(200, 480);
    mu_vol.setCharacterSize(35);
    mu_vol.setFillColor(sf::Color(167, 231, 225));
    
    state_music_t.setFont(font1);
	state_music_t.setString("ON");
	state_music_t.setPosition(310, 425);
    state_music_t.setCharacterSize(35);
    state_music_t.setFillColor(sf::Color::White);
    
    mu_vol_t.setFont(font1);
	mu_vol_t.setString("50");
	mu_vol_t.setPosition(350, 480);
    mu_vol_t.setCharacterSize(35);
    mu_vol_t.setFillColor(sf::Color::White);
    
    state_se.setFont(font2);
	state_se.setString("State : ");
	state_se.setPosition(200, 595);
    state_se.setCharacterSize(35);
    state_se.setFillColor(sf::Color(167, 231, 225));
	
	se_vol.setFont(font2);
	se_vol.setString("Volume : ");
	se_vol.setPosition(200, 650);
    se_vol.setCharacterSize(35);
    se_vol.setFillColor(sf::Color(167, 231, 225));
    
    state_se_t.setFont(font1);
	state_se_t.setString("OFF");
	state_se_t.setPosition(310, 595);
    state_se_t.setCharacterSize(35);
    state_se_t.setFillColor(sf::Color::White);
    
    se_vol_t.setFont(font1);
	se_vol_t.setString("95");
	se_vol_t.setPosition(350, 650);
    se_vol_t.setCharacterSize(35);
    se_vol_t.setFillColor(sf::Color::White);
	
	title.setFont(font1);
	title.setString("High Scores");//GameOver.exe
    title.setCharacterSize(140);
    title.setFillColor(sf::Color::White);
    title.setOutlineColor(sf::Color(127, 219, 132));
    title.setOutlineThickness(1.5);
    title.setPosition(160, 75);
    
    game.setFont(font1);
	game.setString("Game");
    game.setCharacterSize(150);
    game.setFillColor(sf::Color::White);
    game.setOutlineColor(sf::Color(127, 219, 132));
    game.setOutlineThickness(1.5);
    game.setPosition(500, 250);
	
	paused.setFont(font1);
	paused.setString("Paused");
    paused.setCharacterSize(150);
    paused.setFillColor(sf::Color::White);
    paused.setOutlineColor(sf::Color(127, 219, 132));
    paused.setOutlineThickness(1.5);
    paused.setPosition(400, 380);
	
	resume.setFont(font1);
	resume.setString("Resume");
	resume.setPosition(150, 200);
    resume.setCharacterSize(42);
    resume.setFillColor(sf::Color::White);

	restart.setFont(font1);
	restart.setString("Restart");
	restart.setPosition(150, 285);
    restart.setCharacterSize(42);
    restart.setFillColor(sf::Color::White);
    
    restarttxt.setFont(font3);
	restarttxt.setString("Move your Mouse Or Press any Key \n\t\t\t\t\tTo continue");
    restarttxt.setCharacterSize(35);
    restarttxt.setFillColor(sf::Color(167, 231, 225));
	restarttxt.setPosition(185, 490);
    
    sf::Texture restartimg;
	sf::Sprite Restartimg;
	restartimg.loadFromFile("Textures/Move_mouse.png");
	Restartimg.setTexture(restartimg);
	Restartimg.setScale(0.5f, 0.5f);
	Restartimg.setPosition(355, 600);
	
	sf::Texture help_manual;
	sf::Sprite Help_manual;
	help_manual.loadFromFile("Textures/Help_manual.png");
	Help_manual.setTexture(help_manual);
	Help_manual.setScale(0.5f, 0.5f);
	
	sf::Texture last_page;
	sf::Sprite Last_page;
	last_page.loadFromFile("Textures/Last_page.png");
	Last_page.setTexture(last_page);
	
	sf::Texture settingsbg;
	sf::Sprite Settingsbg;
	settingsbg.loadFromFile("Textures/settings.png");
	Settingsbg.setTexture(settingsbg);
	Settingsbg.setScale(0.25f, 0.25f);
	
	settings.setFont(font1);
	settings.setString("Settings");
	settings.setPosition(150,370);
    settings.setCharacterSize(42);
    settings.setFillColor(sf::Color::White);
    
    help.setFont(font1);
	help.setString("Help");
	help.setPosition(150,455);
    help.setCharacterSize(42);
    help.setFillColor(sf::Color::White);
	
	highscores.setFont(font1);
	highscores.setString("Highscores");
	highscores.setPosition(150,540);
    highscores.setCharacterSize(42);
    highscores.setFillColor(sf::Color::White);
    
    return_to_main.setFont(font1);
	return_to_main.setString("Return To Main");
	return_to_main.setPosition(150,625);
    return_to_main.setCharacterSize(42);
    return_to_main.setFillColor(sf::Color::White);
	
	exit.setFont(font1);
	exit.setString("Exit Game");
	exit.setPosition(150,710);
    exit.setCharacterSize(42);
    exit.setFillColor(sf::Color(255, 108, 108));
    
    back.setFont(font1);
	back.setString("Back");
	back.setPosition(67,850);
    back.setCharacterSize(42);
    back.setFillColor(sf::Color::White);
    
    esctext.setFont(font1);
	esctext.setCharacterSize(42);
	esctext.setFillColor(sf::Color(255, 108, 108, 0));
    esctext.setOutlineThickness(0.75);
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// FOLLOWING CODE IS INTITIALZING DIFFERENT THING //////////////////////////////////////////////////////////////////////////
	// Initializing score
	int score=0;
	sf::Text sco;
	sco.setFont(font4);
	sco.setString(std::to_string(score));
    sco.setCharacterSize(30);
    sco.setFillColor(sf::Color::White);
    sco.setPosition(10,0);
	
	// Initializing Background Music.
	sf::Music bgMusic2;
	bgMusic2.openFromFile("Music/field_of_hopes.ogg");
	sf::Music bgMusic;
	bgMusic.setVolume(music_vol);
	bgMusic.setLoop(true);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::Texture backgroundTexture2;
	sf::Sprite backgroundSprite2;
	backgroundTexture2.loadFromFile("Textures/background.png");
	backgroundSprite2.setTexture(backgroundTexture2);
	backgroundSprite2.setColor(sf::Color(255, 255, 255, 255 * 0.1)); 	
	
	// Initializing Levels
	int level=1;
	sf::Text leveltxt;
	leveltxt.setFont(font4);
    leveltxt.setCharacterSize(180);
    leveltxt.setPosition(225, 360);
    
    // Initializing levelup Sound Effects	
	sf::SoundBuffer levelup_music;
	levelup_music.loadFromFile("Sound Effects/1up.wav");
	sf::Sound levelup_Music;
	levelup_Music.setBuffer(levelup_music);
	levelup_Music.setVolume(sounde_vol);
    
	// Initializing lives 
	int lives=3;
	bool playerDied=0;
	bool Show_deathScreen=0;
	
	sf::Text text,nametxt;
	text.setFont(font2);
	text.setCharacterSize(42);
    text.setFillColor(sf::Color::White);
    
	nametxt.setFont(font2);
	nametxt.setCharacterSize(42);
    nametxt.setFillColor(sf::Color::White);
    
    sf::Texture ElementT;
	sf::Sprite ElementS;
	ElementT.loadFromFile("Textures/Element.png");
	ElementS.setTexture(ElementT);
	ElementS.setPosition (115,95);
	ElementS.setScale(1.5f, 1.5f);
	ElementS.setColor(sf::Color(255, 255, 255, 255 * 0.2)); 
    
	// Initializing lives Sound Effects	
	sf::SoundBuffer playerdied_sf;
	playerdied_sf.loadFromFile("Sound Effects/player-death.wav");
	sf::Sound playerDied_sf;
	playerDied_sf.setBuffer(playerdied_sf);
	playerDied_sf.setVolume(sounde_vol);
	
	// Initializing menu button Sound Effects	
	sf::SoundBuffer Menu_button;
	Menu_button.loadFromFile("Sound Effects/menubutton.wav");
	sf::Sound Menu_Button;
	Menu_Button.setBuffer(Menu_button);
	Menu_Button.setVolume(sounde_vol);
	
	sf::SoundBuffer Enter_button;
	Enter_button.loadFromFile("Sound Effects/enter.wav");
	sf::Sound Enter_Button;
	Enter_Button.setBuffer(Enter_button);
	Enter_Button.setVolume(sounde_vol);
	
	// Initializing Player and Player Sprites.
	float player[2] = {};
	sf::Texture playerTexture;
	sf::Sprite playerSprite;

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;

	// Initializing Centipede and Centipede Sprites.
	int total_centi=12;
	float centi_speed = 512;//Centipede will move after 1024 Milliseconds
	int centipede[gameRows*gameColumns][7];
	bool centi_touched_floor=0;
	sf::Texture head_centiTexture;
	sf::Texture body_centiTexture;
	sf::Sprite head_centiSprite;
	sf::Sprite body_centiSprite;
	sf::Clock centipedeClock;
	sf::Clock speed_centipedeClock;
	sf::Clock animation_centiClock;
	sf::Clock hit_centiClock;
	
	
	// Initializing Mushroom and Mushroom Sprites.
	int initial_mushrooms;//20+rand()%11;
	int total_mushrooms = initial_mushrooms;
	int mushroom[gameRows*gameColumns][2];
	int currentmush_shape[gameRows*gameColumns];
	bool is_mush_poisonous[gameRows*gameColumns];
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite;
	
	// Initializing pop Mushrooms Sound Effects	
	sf::SoundBuffer popmusic;
	popmusic.loadFromFile("Sound Effects/pop.wav");
	sf::Sound popMusic;
	popMusic.setBuffer(popmusic);
	popMusic.setVolume(sounde_vol);
	
	sf::SoundBuffer popmusic2;
	popmusic2.loadFromFile("Sound Effects/pop1.wav");
	sf::Sound popMusic2;
	popMusic2.setBuffer(popmusic2);
	popMusic2.setVolume(sounde_vol);
	
	// Initializing Spider and spider Sprites.
	float spider_speed;
	int spider [6];
	sf::Texture spiderTexture;
	sf::Sprite spiderSprite;
	sf::Clock spiderClock;
	sf::Clock hit_spiderClock;
	sf::Clock animation_spiderClock;
	
	sf::SoundBuffer Spider_se;
	Spider_se.loadFromFile("Sound Effects/spider.wav");
	sf::Sound spider_se;
	spider_se.setBuffer(Spider_se);
	spider_se.setVolume(sounde_vol/2);
	
	sf::SoundBuffer Eat_se;
	Eat_se.loadFromFile("Sound Effects/eat.wav");
	sf::Sound eat_se;
	eat_se.setBuffer(Eat_se);
	eat_se.setVolume(sounde_vol);
	
	sf::SoundBuffer Kill_se;
	Kill_se.loadFromFile("Sound Effects/eat.wav");
	sf::Sound kill_se;
	kill_se.setBuffer(Kill_se);
	kill_se.setVolume(sounde_vol);
	
	// Initializing flea and flea Sprites.
	float flea_speed;
	int flea [3];
	sf::Texture fleaTexture;
	sf::Sprite fleaSprite;
	sf::Clock fleaClock;
	sf::Clock animation_fleaClock;
	
	sf::SoundBuffer Flea_se;
	Flea_se.loadFromFile("Sound Effects/flea.wav");
	sf::Sound flea_se;
	flea_se.setBuffer(Flea_se);
	flea_se.setVolume(sounde_vol);
	
	// Initializing scorpion and scorpion Sprites.
	float scorpion_speed;
	int scorpion[5];
	sf::Texture scorpionTexture;
	sf::Sprite scorpionSprite;
	sf::Clock scorpionClock;
	sf::Clock animation_scorpionClock;
	scorpionTexture.loadFromFile("Textures/scorpion.png");
	scorpionSprite.setTexture(scorpionTexture);
	
	sf::SoundBuffer Scorpion_se;
	Scorpion_se.loadFromFile("Sound Effects/scorpion.wav");
	sf::Sound scorpion_se;
	scorpion_se.setBuffer(Scorpion_se);
	scorpion_se.setVolume(sounde_vol);
	
	// Initializing Bullet Sound Effects	
	sf::SoundBuffer bulletmusic;
	sf::Sound bulletMusic;
	bulletMusic.setBuffer(bulletmusic);
	bulletMusic.setVolume(sounde_vol);
	sf::Clock Delay;
	sf::Keyboard Key;
	
	// High score [File handling]
	std::fstream highsco_file;
	std::string name[10];//10 name of width 15 
	int highscore[10];
	sf::Text names,highscores_int;
	std::string TempText;
	
	names.setFont(font4);
	names.setPosition(300,315);
    names.setCharacterSize(35);
    names.setFillColor(sf::Color(167, 231, 225));
    
	highscores_int.setFont(font4);
	highscores_int.setPosition(575,315);
    highscores_int.setCharacterSize(35);
    highscores_int.setFillColor(sf::Color::White);
    
    // File initialization 
	highsco_file.open("highscores.txt");
	for (int i=0;i<10;i++){
		highsco_file >> name[i];
		highsco_file >> highscore[i];
	}
	highsco_file.close();	
   	TempText='\0';
   	for (int i=0;i<10;i++){ //Saving Names from file
	TempText += name[i];
		TempText += '\n';
	}
	names.setString(TempText);
	TempText='\0';
	for (int i=0;i<10;i++){ //Saving Highscores from file
		TempText += std::to_string(highscore[i]);
		TempText += '\n';
	}
	highscores_int.setString(TempText);	
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//                                                GAME LOOPS STARTS FROM HERE                                              //
	
	while(window.isOpen()) {
		
		// MAIN -------------------------------------------------------
		sf::Event screen1;
		sf::Event screen2;
		if (Show_Main){
			//Bg INITIALIZATIONS / RESETING ---------------------------
			bgMusic.stop();
			bgMusic.openFromFile("Music/field_of_hopes.ogg");
			if (music_on) bgMusic.play();
			backgroundTexture.loadFromFile("Textures/background.png");
			backgroundSprite.setTexture(backgroundTexture);
			backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.20)); 
			//GRID INITIALIZATIONS / RESETING -------------------------
			for (int i=0;i<gameRows;i++){
				for (int j=0;j<gameColumns;j++){
					gameGrid[j][i] = 0;
				}
			}
			//OTHER INITIALIZATIONS / RESETING ------------------------
			level=1; score=0; lives=3; playerDied=0; centi_speed = 512;
			centi_touched_floor=0; total_centi=12;
			notetext.setString("*Press Escape 'during the game' for additional options (Escape Menu)");
			notetext.setPosition(265, 920);

		}
		while (Show_Main){
			if(Restart) centi.setString("Get Ready");
			window.draw(backgroundSprite);
			window.draw(centi);			
			if(Restart)	window.draw(restarttxt); //Display if only restart
			if(Restart)	window.draw(Restartimg); //Display if only restart
			if(!Restart) window.draw(modetext);
			if(!Restart) window.draw(notetext);
			if(!Restart) window.draw(beginner);
			if(!Restart) window.draw(intermediate);
			if(!Restart) window.draw(expert);
			if(!Restart) window.draw(maintext);
			if(Restart) centi.setString("Centipede");
			window.display();
			window.clear();
			if(!Restart) {
				if (Main_option==1){
					maintext.setString("Beginner");
					maintext.setPosition(150, 600);	
				}
				else if (Main_option==2){
					maintext.setString("Intermediate");
					maintext.setPosition(380, 600);	
				}
				else if (Main_option==3){
					maintext.setString("Expert");
					maintext.setPosition(670, 600);
				}
			}
			while(window.pollEvent(screen1)){
				if (screen1.type == sf::Event::Closed) {
					return 0;
				}
				if (Restart || screen1.type == sf::Event::KeyReleased) {
					if (screen1.key.code == sf::Keyboard::Right || screen1.key.code == sf::Keyboard::Up) {
                        if (sounde_on) Menu_Button.play();
                        Main_option += 1;
                        if(Main_option == 4) Main_option = 1;
                    }
                    if (screen1.key.code == sf::Keyboard::Left|| screen1.key.code == sf::Keyboard::Down) {
                        if (sounde_on) Menu_Button.play();
                        Main_option -= 1;
                        if(Main_option == 0) Main_option = 3;
                    }
                    if (Restart || screen1.key.code == sf::Keyboard::Enter) {
                    	if (sounde_on) Enter_Button.play();
                    	Show_Main=0;
                    	Restart=0;
                    	playerTexture.loadFromFile("Textures/player.png");
                    	bulletTexture.loadFromFile("Textures/bullet.png");
                    	mushroomTexture.loadFromFile("Textures/mushroom.png");
                    	body_centiTexture.loadFromFile("Textures/c_body_left_walk.png");
						head_centiTexture.loadFromFile("Textures/c_head_left_walk.png");
						fleaTexture.loadFromFile("Textures/flea.png");
                    	
                    	if (Main_option==1){
                    		backgroundTexture.loadFromFile("Textures/grass.png");
							backgroundSprite.setTexture(backgroundTexture);
							bgMusic.stop();
							bgMusic.openFromFile("Music/magical_journey.ogg");
							if (music_on) bgMusic.play();
							bulletmusic.loadFromFile("Sound Effects/shoot3.wav");
							leveltxt.setFillColor(sf::Color(255, 255, 0, 255 * 0.20));
							centi_speed = 1024;
                    	}
                    	else if (Main_option==2){
                    		backgroundTexture.loadFromFile("Textures/rock.png");
							backgroundSprite.setTexture(backgroundTexture);
							bgMusic.openFromFile("Music/risk.ogg");
							if (music_on) bgMusic.play();
							bulletmusic.loadFromFile("Sound Effects/shoot.wav");
							leveltxt.setFillColor(sf::Color(196, 138, 71, 255 * 0.20));
							flea[exists] = 0;	flea_speed = 512/8;
							scorpion[exists] = 0;	scorpion_speed=95;
							
							changeSprite_color(playerTexture,sf::Color(253, 198, 72), sf::Color::White,sf::Color(253, 198, 72));//changing texture of player
							changeSprite_color(bulletTexture,sf::Color(253, 198, 72), sf::Color(128,128,0),sf::Color::Red);//changing texture of Bullet
							changeSprite_color(mushroomTexture,sf::Color::White, sf::Color(128,128,0),sf::Color(128,128,0));//changing texture of mushrooms
							changeSprite_color(body_centiTexture,sf::Color(196, 138, 71), sf::Color::Yellow,sf::Color::White);//changing texture of centipede's body
							changeSprite_color(head_centiTexture,sf::Color(196, 138, 71), sf::Color::White,sf::Color::White);//changing texture of centipede's head
							changeSprite_color(fleaTexture,sf::Color(196, 138, 71), sf::Color::White,sf::Color::White);//changing texture of flea
							
  	                  	}
                    	else if (Main_option==3){
                    		backgroundTexture.loadFromFile("Textures/lava.png");
							backgroundSprite.setTexture(backgroundTexture);
							bgMusic.stop();
							bgMusic.openFromFile("Music/Music3.ogg");
							if (music_on) bgMusic.play();
							bulletmusic.loadFromFile("Sound Effects/shoot.wav");
							leveltxt.setFillColor(sf::Color(255, 0 , 0 , 255 * 0.20));
							flea[exists] = 0;	flea_speed = 512/12;
							scorpion[exists] = 0;	scorpion_speed=95;
							
							changeSprite_color(playerTexture,sf::Color::Blue, sf::Color::White,sf::Color::Red);//changing texture of player
							changeSprite_color(bulletTexture,sf::Color::Blue, sf::Color::White,sf::Color::Red);//changing texture of Bullet
							changeSprite_color(mushroomTexture,sf::Color::White, sf::Color::Red,sf::Color::Black);//changing texture of mushrooms
							changeSprite_color(body_centiTexture,sf::Color::Red, sf::Color::Yellow,sf::Color::White);//changing texture of centipede's body
							changeSprite_color(head_centiTexture,sf::Color::Red, sf::Color::Black,sf::Color::White);//changing texture of centipede's head		
                    		changeSprite_color(fleaTexture,sf::Color::Red, sf::Color::Black,sf::Color::White);//changing texture of flea
                    		
                    		//-----------------------SPIDER-------------------------//
							spider[exists] = 0;
							spider_speed = 512/4; 
							spiderTexture.loadFromFile("Textures/spider_and_score.png");
							changeSprite_color(spiderTexture,sf::Color::Red, sf::Color::Black,sf::Color::White);
							spiderSprite.setTexture(spiderTexture);
                    	
                    	}
                    	
                    	//----------------------------------------------------------
                    	playerSprite.setTexture(playerTexture);
						playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
						bulletSprite.setTexture(bulletTexture);
						bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
             			mushroomSprite.setTexture(mushroomTexture);
						body_centiSprite.setTexture(body_centiTexture);
						head_centiSprite.setTexture(head_centiTexture);
						fleaSprite.setTexture(fleaTexture);
						
						      	
                    	//SPAWNING PLAYER -------------------------------------------
                    	player[x] = (gameColumns / 2) * boxPixelsX;
						player[y] = (gameColumns * 3 / 4) * boxPixelsY;
                    	
                    	//SPAWNING BULLET -------------------------------------------
                    	bullet[x] = player[x];
						bullet[y] = player[y] - boxPixelsY;
						bullet[exists] = false;
                    	
                    	//SPAWNING MUSHROOMS ----------------------------------------
                    	sco.setString(std::to_string(score));
                    	initial_mushrooms = 20+rand()%11;
                    	total_mushrooms = initial_mushrooms;
						for(int i=0;i<total_mushrooms;i++)
							{is_mush_poisonous[i]=0;currentmush_shape[i]=0;}
						for (int i=0;i<total_mushrooms;i++){
							while (Delay.getElapsedTime().asMilliseconds()<150);
							Delay.restart();
							if (sounde_on) popMusic.play();
							//DISPLAYING ON SCREEN -------------------------
							window.draw(backgroundSprite);
							drawPlayer(window, player, playerSprite);
							drawMushrooms(window,mushroomSprite,mushroom, currentmush_shape, is_mush_poisonous,i);
							drawLives(lives,window,playerSprite);
							window.draw(sco);
							window.display();
							window.clear();
							//----------------------------------------------
							mushroom[i][x]=rand()%(gameColumns-1);
							mushroom[i][y]=rand()%(gameRows-1);
							gameGrid[mushroom[i][x]][mushroom[i][y]]=-4;
							if (mushroom[i][x]==player[x]&&mushroom[i][y]==player[y]) 
								i--;
							for (int j=0;j<i;j++)
							if (mushroom[i][x]==mushroom[j][x]&& mushroom[i][y]==mushroom[j][y]) i--;
						}
						
						//SPAWNING CENTIPEDE ----------------------------------------
						spawnCenti(centipede,total_centi);
						centipede[0][is_head] = 1;
						centipede[0][x] = rand() % 30;
						centipede[0][y] = rand() % 20;
                    }
				}
			}
		}
		//Escape Menu -----------------------------------------------------
		bool Show_Escape=0,Show_Escape_2=1;
		int esctext_x=200;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
			Show_Escape=1;
			Escape_option=1;
			//Bg RESETING -------------------------------------------------
			if (music_on) bgMusic.stop();
			if (music_on) bgMusic2.play(); 	
			notetext.setString("*Project by Ahmed Asim");
			notetext.setPosition(700, 920);
		}
		while(Show_Escape){
			window.draw(backgroundSprite2);
			//window.draw(title);
			window.draw(game);
			window.draw(paused);
			window.draw(notetext);
			window.draw(resume);
			window.draw(restart);
			window.draw(settings);
			window.draw(help);
			window.draw(highscores);
			window.draw(return_to_main);
			window.draw(exit);
			window.draw(esctext);
			window.display();
			window.clear();
			if (Escape_option==1)	esctext.setString("Resume");
			else if (Escape_option==2)	esctext.setString("Restart");
			else if (Escape_option==3)	esctext.setString("Settings");
			else if (Escape_option==4)	esctext.setString("Help");
			else if (Escape_option==5)	esctext.setString("Highscores");
			else if (Escape_option==6)	esctext.setString("Return To Main");
			else if (Escape_option==7)	esctext.setString("Exit Game");
			if (Escape_option==6||Escape_option==7||Escape_option==2)
				esctext.setOutlineColor(sf::Color(255, 108, 108));
			else esctext.setOutlineColor(sf::Color::Green);
			esctext.setPosition(150, esctext_x);
			while(window.pollEvent(screen1)){
				if (screen1.type == sf::Event::Closed) {
					return 0;
				}
				if (screen1.type == sf::Event::KeyReleased) {
					if (screen1.key.code == sf::Keyboard::Right || screen1.key.code == sf::Keyboard::Down) {
                        if (sounde_on) Menu_Button.play();
                        Escape_option += 1;
                        esctext_x+=85;
                        if(Escape_option == 8) {
                        	Escape_option = 1;
                        	esctext_x=200;
                        }
                    }
                    if (screen1.key.code == sf::Keyboard::Left|| screen1.key.code == sf::Keyboard::Up) {
                        if (sounde_on) Menu_Button.play();
                        Escape_option -= 1;
                        esctext_x -= 85;
                        if(Escape_option == 0) {
                        	Escape_option = 7;
                        	esctext_x=710;	
                        } 
                    }
                    if (screen1.key.code == sf::Keyboard::Enter) {
                    	if (sounde_on) Enter_Button.play();
                    	if (Escape_option==1) {
                    		Show_Escape=0;
                    		if (music_on) bgMusic2.stop();
                    		if (music_on) bgMusic.play();
                    	} 
                    	else if (Escape_option==2) {
                    		Show_Escape=0;
                    		Show_Main=1;
                    		Restart=1;
                    		if (music_on) bgMusic2.stop();
                    		if (music_on) bgMusic.play();
                    	} 
                    	else if (Escape_option==3) {
                    		Show_Escape_2=1;
                    		settings.setString("Music");
                    		highscores.setString("Sound Effects");
                    		title.setString("Settings");//GameOver.exe
						    title.setCharacterSize(220);
    						title.setPosition(115, 55);
    						//state_music, state_se, mu_vol, state_music_t, mu_vol_t, se_vol, state_se_t, se_vol_t
                    		while (Show_Escape_2) {
                    			window.draw(Settingsbg);
                    			window.draw(title);
                    			window.draw(back);
                    			window.draw(esctext);
                    			window.draw(settings);
                    			window.draw(highscores);
                    			window.draw(state_music);
                    			window.draw(state_se);
                    			window.draw(mu_vol);
                    			window.draw(state_music_t);
                    			window.draw(mu_vol_t);
                    			window.draw(se_vol);
                    			window.draw(state_se_t);
                    			window.draw(se_vol_t);
                    			window.display();
								window.clear();
								if (Escape_option_2==1){
									esctext.setCharacterSize(35);
									esctext.setString(music_on?"ON":"OFF");	
									esctext.setPosition(310, 425);
								}
								else if (Escape_option_2==2){
									esctext.setCharacterSize(35);
									esctext.setString(std::to_string(music_vol));
									esctext.setPosition(350, 480);	
								}
								else if (Escape_option_2==3){
									esctext.setCharacterSize(35);
									esctext.setString(sounde_on?"ON":"OFF");
									esctext.setPosition(310, 595);
								}
								else if (Escape_option_2==4){
									esctext.setCharacterSize(35);
									esctext.setString(std::to_string(sounde_vol));
									esctext.setPosition(350, 650);	
								}
								else if (Escape_option_2==5){
									esctext.setCharacterSize(42);
									esctext.setString("Back");
                    				esctext.setPosition(67,850);
								}
								state_music_t.setString(music_on?"ON":"OFF");
								mu_vol_t.setString(std::to_string(music_vol));
								state_se_t.setString(sounde_on?"ON":"OFF");
								se_vol_t.setString(std::to_string(sounde_vol));
	                    		while(window.pollEvent(screen2)){
									if (screen2.type == sf::Event::Closed) {
										return 0;
									}
									if (screen2.type == sf::Event::KeyReleased && screen2.key.code == sf::Keyboard::Down) {
										
										if (sounde_on) Menu_Button.play();
										Escape_option_2 ++;
										if (Escape_option_2>=6) Escape_option_2=1;
									}
									if (screen2.type == sf::Event::KeyReleased && screen2.key.code == sf::Keyboard::Up) {
										
										if (sounde_on) Menu_Button.play();
										Escape_option_2 --;
										if (Escape_option_2<=0) Escape_option_2=6;
									}
									if (screen2.type == sf::Event::KeyReleased && screen2.key.code == sf::Keyboard::Enter && Escape_option_2==5 || screen2.type == sf::Event::KeyReleased && screen2.key.code == sf::Keyboard::Escape) {
									if (sounde_on) Enter_Button.play();
									Show_Escape_2=0;
									Escape_option_2=1;
									}
									if (screen2.key.code == sf::Keyboard::Right || screen2.key.code == sf::Keyboard::Left) {
										switch (Escape_option_2){
										case 1: 
											if (screen2.type == sf::Event::KeyReleased) {
												if (sounde_on) Enter_Button.play();
												music_on=!music_on;
												if (!music_on) bgMusic2.stop();
												if (music_on) bgMusic2.play();
											}
											break;
										case 2: 
											if (screen2.key.code == sf::Keyboard::Right) {
												if (music_vol!=100) {
													music_vol++;
													if (sounde_on) Menu_Button.play();
												}
											}
											else {
												if (music_vol!=0) {
													music_vol--;
													if (sounde_on) Menu_Button.play();
												}
											}
											bgMusic.setVolume(music_vol);
											bgMusic2.setVolume(music_vol);
											break;
											
										case 3: 
											if (screen2.type == sf::Event::KeyReleased) {
												sounde_on=!sounde_on;
												if (sounde_on) Enter_Button.play();
											}
											break;
										case 4: 
											if (screen2.key.code == sf::Keyboard::Right) {
												if (sounde_vol!=100) {
													sounde_vol++;
													if (sounde_on) Menu_Button.play();
												}
											}
											else {
												if (sounde_vol!=0) {
													sounde_vol--;
													if (sounde_on) Menu_Button.play();
												}
											}
											levelup_Music.setVolume(sounde_vol);
											Menu_Button.setVolume(sounde_vol);
											Enter_Button.setVolume(sounde_vol);
											popMusic.setVolume(sounde_vol);
											popMusic2.setVolume(sounde_vol);
											spider_se.setVolume(sounde_vol/2);
											kill_se.setVolume(sounde_vol);
											flea_se.setVolume(sounde_vol);
											scorpion_se.setVolume(sounde_vol);
											bulletMusic.setVolume(sounde_vol);
											eat_se.setVolume(sounde_vol);
											playerDied_sf.setVolume(sounde_vol);
											break;
										}
									}
									}
							}
							settings.setString("Settings");
							highscores.setString("Highscores");
							title.setString("High Scores");
							title.setCharacterSize(140);
    						title.setPosition(160, 75);
    						esctext.setCharacterSize(42);
                    	}
                    	else if (Escape_option==4) { //Settingsbg
                    		Show_Escape_2=1;
                    		esctext.setString("Back");
                    		esctext.setPosition(67,850);
                    		while (Show_Escape_2) {
                    			window.draw(Help_manual);
                    			window.draw(back);
                    			window.draw(esctext);
                    			window.display();
								window.clear();
	                    		while(window.pollEvent(screen2)){
									if (screen2.type == sf::Event::Closed) {
										return 0;
									}
									if (screen2.type == sf::Event::KeyReleased && screen2.key.code == sf::Keyboard::Enter) {
									if (sounde_on) Enter_Button.play();
									Show_Escape_2=0;
									}
								}
							}
                    	}
                    	else if (Escape_option==5) {
                    		Show_Escape_2=1;
                    		esctext.setString("Back");
                    		esctext.setPosition(67,850);
                    		while (Show_Escape_2) {
                    			window.draw(backgroundSprite2);
                    			window.draw(title);
                    			window.draw(highscores_int);
                    			window.draw(names);
                    			window.draw(back);
                    			window.draw(esctext);
                    			window.display();
								window.clear();
	                    		while(window.pollEvent(screen2)){
									if (screen2.type == sf::Event::Closed) {
										return 0;
									}
									if (screen2.type == sf::Event::KeyReleased && screen2.key.code == sf::Keyboard::Enter) {
									if (sounde_on) Enter_Button.play();
									Show_Escape_2=0;
									}
								}
							}	
                    	}
                    	else if (Escape_option==6) {
                    		Show_Escape=0;
                    		Show_Main=1;
                    		if (music_on) bgMusic2.stop();
                    		if (music_on) bgMusic.play();
                    	}
                    	else if (Escape_option==7) return 0;
                    }
				}
			}
		}
		if (Show_Main==1) continue;
		
		
		//INCREASING LEVEL ------------------------------------------------
		if (total_centi==0){
			level++;
			centi_touched_floor=0;
			if (sounde_on) levelup_Music.play();
			leveltxt.setString("Level " + std::to_string(level));
			
			//SPAWN MORE MUSHROOMS ---------------------------------------
			sco.setString(std::to_string(score));
			int newtotal_mush;
			newtotal_mush=total_mushrooms+initial_mushrooms*1.2;
			for(int i=total_mushrooms;i<(newtotal_mush);i++){
				while (Delay.getElapsedTime().asMilliseconds()<150);
				Delay.restart();
				if (sounde_on) popMusic.play();
				//DISPLAYING ON SCREEN -------------------------
				window.draw(backgroundSprite);
				window.draw(leveltxt);
				drawPlayer(window, player, playerSprite);
				drawMushrooms(window,mushroomSprite,mushroom,currentmush_shape, is_mush_poisonous,i);
				drawLives(lives,window,playerSprite);
				window.draw(sco);
				window.display();
				window.clear();
				//----------------------------------------------
				mushroom[i][x]=rand()%(gameColumns-1);
				if (i-total_mushrooms<(level*5)) mushroom[i][y]=rand()%(gameRows-1-(level*3));//(level*3) is for making Mushrooms more concentrated toward top;
				else mushroom[i][y]=rand()%(gameRows-level);
				gameGrid[mushroom[i][x]][mushroom[i][y]]=-4;
				currentmush_shape[i]=0;
				is_mush_poisonous[i]=0;
				if (mushroom[i][x]==player[x]&&mushroom[i][y]==player[y]) i--;
				for (int j=0;j<i;j++) {if (mushroom[i][x]==mushroom[j][x]&& mushroom[i][y]==mushroom[j][y]) {i--;}}
			}
			initial_mushrooms=initial_mushrooms*1.2;
			total_mushrooms=newtotal_mush;
			//------------------------------------------------------------
			
			//NEW CENTIPEDE ----------------------------------------------
			centi_speed/=2;
			total_centi=12;
			spawnCenti(centipede,total_centi);
			for (int i=1;i<level;i++){
				centipede[total_centi-i][is_head] = 1;
				centipede[total_centi-i][exists] = 2;
				centipede[total_centi-i][x]=rand()%30;
				centipede[total_centi-i][xdir]=1;
			}
			//-----------------------------------------------------------
			
			//SPIDER SPEED INCREASE -------------------------------------
			if (Main_option==3) (spider_speed/4)*3;
		}
		
		window.draw(backgroundSprite);
		
		//--------------------------------------------------------------//
		inputKeyboard(player,bullet,Key,Delay,mushroom, is_mush_poisonous,total_mushrooms,playerDied);
		playerDeath(player,playerDied);                 
		//--------------------------------------------------------------//
		
		//CODE FOR DEATH OF PLAYER ---------------------------------------
		if (playerDied){
			playerDied_sf.play();
			if (lives>1) {
				lives--;
				leveltxt.setString("You Died");
				leveltxt.setPosition(190, 360);
				for(int i=0;i<total_mushrooms;i++){
					while (Delay.getElapsedTime().asMilliseconds()<50);
					Delay.restart();
					//DISPLAYING ON SCREEN -------------------------
					window.draw(backgroundSprite);
					window.draw(leveltxt);
					drawMushrooms(window,mushroomSprite,mushroom,currentmush_shape, is_mush_poisonous,total_mushrooms);
					drawLives(lives,window,playerSprite);
					window.draw(sco);
					window.display();
					window.clear();
					//REGENERATION OF MUSHROOMS --------------------
					if (currentmush_shape[i]>0){
						if (sounde_on) popMusic2.play();
						currentmush_shape[i]=0;
						gameGrid[mushroom[i][x]][mushroom[i][y]]=-4;
						score+=5;
					}
					if (is_mush_poisonous[i]){
						if (sounde_on) popMusic2.play();
						is_mush_poisonous[i]=0;
					}
				}
				leveltxt.setPosition(225, 360);
				//SPAWNING NEW CENTIPEDE ------------------------------------
				for (int i=0;i<total_centi;i++)	gameGrid[centipede[i][x]][centipede[i][y]]=0;
				total_centi=12;
				spawnCenti(centipede,total_centi); 
				centi_touched_floor=0;
				//SPAWNING PLAYER -------------------------------------------
                player[x] = (gameColumns / 2) * boxPixelsX;
				player[y] = (gameColumns * 3 / 4) * boxPixelsY;
			}
			else if (lives==1) {
				Show_Main=1;
				Show_deathScreen=1;
				if (sounde_on) flea_se.stop();
				char letter;
				bool Name_NotEntered=1;
				char lettername [15] = {};
				//std::string lettername;
				int i=0;
				title.setString("GameOver.exe");//High Scores
				title.setPosition(80, 75);
				text.setString("Enter Your Name : ");
				text.setPosition(150, 450);
   	 			nametxt.setPosition(150, 515);
   	 			//Bg RESETING -------------------------------------------------
				if (music_on) bgMusic.stop();
				if (music_on) bgMusic2.play(); 	
    	
				while (Name_NotEntered) {
					nametxt.setString(lettername);
					window.draw(backgroundSprite2);
					window.draw(title);
					window.draw(ElementS);
					window.draw(text);
					window.draw(nametxt);
					window.display();
					window.clear();
					while(window.pollEvent(screen1)){
						if (screen1.type == sf::Event::Closed) {
							return 0;
						}
						if (screen1.type == sf::Event::TextEntered) {
                       	 	letter = screen1.text.unicode;
							if (letter == 8) {  //To handle backspace
								if (i>0) {
									if (sounde_on) Enter_Button.play();
									i--; 
									lettername[i] = '\0';
								}
							}
							else if (letter == ' ') {
								if (sounde_on) Enter_Button.play();
								lettername[i] = '_';
								i++;
							}
							else if (letter != '\n') {
								if (sounde_on) Enter_Button.play();
                       	 		lettername[i] = letter;
                       	 		i++;
							}
							if (letter == 13 || i==10) {
								if (sounde_on) Enter_Button.play();
								Name_NotEntered=false; //13 ASCII Value is To Handle 'Enter' 
							}
						}
					}	
				}
				
				for (int j=0;j<10;j++) {
					if (score>highscore[j]){
						for (int k=9;k>j;k--){
							highscore[k] = highscore[k-1];
							name[k] = name[k-1];
						}
						highscore[j] = score;
						name[j] = lettername;
						break;
					}
				}
				
				std::string AddInFile,temp;
				AddInFile='\0';
				for (int j=0;j<10;j++) {
					AddInFile += name[j];
               		AddInFile += " ";
                	AddInFile += std::to_string(highscore[j]);
                	AddInFile += "\n";
				}
				highsco_file.open("highscores.txt");
				highsco_file << AddInFile;
				highsco_file.close();
				

				//UPDATED FILE CHECKING ----------------------------------
				highsco_file.open("highscores.txt");
				for (int i=0;i<10;i++){
					highsco_file >> name[i];
					highsco_file >> highscore[i];
				}
				highsco_file.close();	
   				TempText='\0';
   				for (int i=0;i<10;i++){ //Saving Names from file
					TempText += name[i];
					TempText += '\n';
				}
				names.setString(TempText);
				TempText='\0';
			    for (int i=0;i<10;i++){ //Saving Highscores from file
					TempText += std::to_string(highscore[i]);
					TempText += '\n';
				}
				highscores_int.setString(TempText);
				
				//REUSING TEXTS --------------------------------------------
				settings.setString("Your Score : ");
				settings.setPosition(170,250);
				highscores.setString(std::to_string(score));
				highscores.setPosition(390,250);
				highscores.setFillColor(sf::Color(203, 108, 230));
				help.setString("Level Passed : ");
				help.setPosition(510,250);	
				return_to_main.setString(std::to_string(level-1));
				return_to_main.setPosition(760,250);
				return_to_main.setFillColor(sf::Color(203, 108, 230));
				
				names.setPosition(450,400);
				highscores_int.setPosition(725,400);
				notetext.setString("Press Escape to exit or Space for Main Menu");
				notetext.setPosition(500, 920);
				title.setString("Bravo, Player!");
				title.setPosition(75, 25);
				Show_deathScreen=1;
				sf::Event screen3;
				while (Show_deathScreen) {
					window.draw(Last_page);
					window.draw(settings);
					window.draw(highscores);
					window.draw(help);
					window.draw(return_to_main);
					window.draw(notetext);
					window.draw(names);
					window.draw(title);
					window.draw(highscores_int);
					window.display();
					window.clear();
					while(window.pollEvent(screen3)){
						if (screen3.type == sf::Event::Closed) {
								return 0;
						}
						if (screen3.type == sf::Event::KeyReleased && screen3.key.code == sf::Keyboard::Space) 	{
							if (sounde_on) Enter_Button.play();
							Show_deathScreen=0;
						}
						if (screen3.type == sf::Event::KeyReleased && screen3.key.code == sf::Keyboard::Escape) 	{
							return 0;
						}
					}
				}
				title.setString("High Scores");
				title.setPosition(160, 75);
				names.setPosition(300,315);
				highscores_int.setPosition(575,315);
				settings.setString("Settings");
				settings.setPosition(150,370);
				help.setString("Help");
				help.setPosition(150,455);
				highscores.setString("Highscores");
				highscores.setPosition(150,540);
				highscores.setFillColor(sf::Color::White);
				return_to_main.setString("Return To Main");
				return_to_main.setPosition(150,625);
				return_to_main.setFillColor(sf::Color::White);
				if (music_on) bgMusic2.stop(); 
				playerDied=0;
				continue;
			} 
			playerDied=0;
		}
		//----------------------------------------------------------------------
		
		
		if (bullet[exists] == false){
			bullet[x] = player[x];
			bullet[y] = player[y];
			bulletSprite.setPosition(bullet[x], bullet[y]-7);
			window.draw(bulletSprite);
			
		}
		
		if(bullet[y]==(player[y]-boxPixelsY)-8&&sounde_on) bulletMusic.play();
		
		if (bullet[exists] == true) {
			moveBullet(bullet,bulletClock);
			drawBullet(window,bullet,bulletSprite);
		}
		
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//       All MAIN FUNCTIONS ARE PRESENT HERE  (In short the entire game is encapsulated within this context.)         //
		
		drawPlayer(window, player, playerSprite);
		
		hitMushroom(bullet,mushroom,currentmush_shape,is_mush_poisonous, total_mushrooms,score);
		drawMushrooms(window,mushroomSprite,mushroom,currentmush_shape, is_mush_poisonous,total_mushrooms);
		
		moveCentipede(centipede, total_centi, centipedeClock,mushroom,currentmush_shape,total_mushrooms,is_mush_poisonous, centi_touched_floor,centi_speed,eat_se,sounde_on);
		moveCentipede(centipede, total_centi, speed_centipedeClock,mushroom,currentmush_shape,total_mushrooms,is_mush_poisonous, centi_touched_floor,centi_speed,eat_se,sounde_on,1);
		hitCentipede(centipede,total_centi, bullet,mushroom,currentmush_shape, is_mush_poisonous,total_mushrooms,level,score);
		drawCentipede(centipede,total_centi,window,head_centiSprite, body_centiSprite,animation_centiClock,centi_speed);
		
		updateLives(lives,score);
		drawLives(lives,window,playerSprite);
		
		if (Main_option==2||Main_option==3) {
			//FLEA --------------------------------------------------------------------------
			moveFlea(flea, flea_speed, fleaClock, mushroom, currentmush_shape, total_mushrooms, is_mush_poisonous, flea_se, sounde_on);
			if (flea[exists]) drawObject(flea, flea_speed, window, fleaSprite, animation_fleaClock,3);
			hitFlea(bullet, flea, score, kill_se, sounde_on);
			
			//SCORPION -----------------------------------------------------------------------
			moveScorpion(scorpion, scorpion_speed, scorpionClock, mushroom, total_mushrooms, is_mush_poisonous, scorpion_se, sounde_on);
			if (scorpion[exists]) drawObject(scorpion, scorpion_speed, window, scorpionSprite, animation_scorpionClock,4);
			hitScorpion(bullet, scorpion, score, kill_se, sounde_on);
		}
		if (Main_option==3) {
			//SPIDER ------------------------------------------------------------------------
			moveSpider(spider,spider_speed,spiderClock,mushroom,currentmush_shape, total_mushrooms,is_mush_poisonous,spider_se,eat_se,sounde_on);
			if (spider[exists])	drawObject(spider,spider_speed,window,spiderSprite,animation_spiderClock,2);
			hitSpider(player,bullet,spider,score,window,spiderSprite,hit_spiderClock,kill_se,sounde_on);
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				return 0;
			}
		}	
		
		sco.setString(std::to_string(score));
		window.draw(sco);
		window.display();
		window.clear();
	
	}
	
	
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                             FUNCTIONS START FROM HERE                                                  //


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void changeSprite_color(sf::Texture& texture,sf::Color greenChange,sf::Color redChange,sf::Color whiteChange){

	sf::Color color_change, colorofpixel;
	sf::Image image = texture.copyToImage();
	int width = image.getSize().x;
	int height = image.getSize().y;
	
	for(int y=0;y<height;y++){
		for(int x=0;x<width;x++){
			colorofpixel = image.getPixel(x, y);
			if (colorofpixel==sf::Color::Green){
				color_change=greenChange;
			}
			else if (colorofpixel==sf::Color::Red){
				color_change=redChange;
			}
			else if (colorofpixel==sf::Color(255, 255, 193)){
				color_change=whiteChange;
			}
			else color_change=colorofpixel;
			image.setPixel(x, y, color_change);
		}
	}
	texture.update(image);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void inputKeyboard(float player[],float bullet[],sf::Keyboard& Key,sf::Clock& Delay, int mushroom[][exists],bool is_mush_poisonous[],int& total_mushrooms,bool& playerDied){
double inputdelay=50;
if (Key.isKeyPressed(Key.LShift)) {inputdelay/=2;} //speed is doubled when shift is pressed 
if (Delay.getElapsedTime().asMilliseconds() > inputdelay){
	Delay.restart();
	
	//Right ---------------------------------------------------------------
	if (Key.isKeyPressed(Key.Right)&&player[x]<resolutionX-boxPixelsY) 
	{
	if(gameGrid[(int(player[x])/boxPixelsX)+1][int(player[y])/boxPixelsX]<0){
		for (int i=0;i<total_mushrooms;i++){
			if ((int(player[x])/boxPixelsX)+1==mushroom[i][x]&&int(player[y])/boxPixelsY==mushroom[i][y]&&is_mush_poisonous[i]==1){ //Checking for Poisonous Mushroom
				playerDied=1;
				break;
			}
		}
	}
	else player[x]+=boxPixelsX; //If No Poisonous Mushroom & No border of Screen
	}
	
	//Left ----------------------------------------------------------------
	else if (Key.isKeyPressed(Key.Left)&&player[x]>0) 
	{
	if(gameGrid[(int(player[x])/boxPixelsX)-1][int(player[y])/boxPixelsY]<0){		
		for (int i=0;i<total_mushrooms;i++){
			if ((int(player[x])/boxPixelsX)-1==mushroom[i][x]&&int(player[y])/boxPixelsY==mushroom[i][y]&&is_mush_poisonous[i]==1) { //Checking for Poisonous Mushroom
				playerDied=1;
				break;
			}
		}
	}
	else player[x]-=boxPixelsX; //If No Poisonous Mushroom & No border of Screen
	}
	
	//Up ------------------------------------------------------------------
	else if (Key.isKeyPressed(Key.Up)&&player[y]>resolutionY-(1+5)*boxPixelsY)
	 {
	 if(gameGrid[(int(player[x])/boxPixelsX)][(int(player[y])-boxPixelsY)/boxPixelsY]<0){
	 	for (int i=0;i<total_mushrooms;i++){
	 		if ((int(player[x])/boxPixelsX)==mushroom[i][x]&&(int(player[y])-boxPixelsY)/boxPixelsY==mushroom[i][y]&&is_mush_poisonous[i]==1){ //Checking for Poisonous Mushroom
	 			playerDied=1;
				break;
	 		}
	 	}
	 }
	 else player[y]-=boxPixelsY; //If No Poisonous Mushroom & No border of Screen
	 }
	 
	//Down ------------------------------------------------------------------
	else if (Key.isKeyPressed(Key.Down)&&player[y]<resolutionY-boxPixelsY) 	
		{
		if(gameGrid[(int(player[x])/boxPixelsX)][(int(player[y])-boxPixelsY)/boxPixelsY+2]<0){
			for (int i=0;i<total_mushrooms;i++){
				if ((int(player[x])/boxPixelsX)==mushroom[i][x]&&(int(player[y])-boxPixelsY)/boxPixelsY+2==mushroom[i][y]&&is_mush_poisonous[i]==1){ //Checking for Poisonous Mushroom
					playerDied=1;
					break;
				}
			} 
		} 	
		else player[y]+=boxPixelsY; //If No Poisonous Mushroom & No border of Screen
		}
	
	//Bullet ----------------------------------------------------------------
	if (Key.isKeyPressed(Key.Space)) {bullet[exists] = true; }
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveBullet(float bullet[], sf::Clock& bulletClock) {
	double inputdelay=1.5;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {inputdelay=(inputdelay*2)/5;}
	if (bulletClock.getElapsedTime().asMilliseconds() < inputdelay)
		return;
	
	bulletClock.restart();
	bullet[y] -= 8;	
	if (bullet[y] < -32){
		bullet[exists] = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawMushrooms(sf::RenderWindow& window, sf::Sprite& mushroomSprite, int mushroom[][exists],int currentmush_shape[],bool is_mush_poisonous[],int total_mushrooms){
	for (int i=0;i<total_mushrooms/*&&currentmush_shape[i]<4*/;i++){
		if(currentmush_shape[i]<4){
		mushroomSprite.setTextureRect(sf::IntRect(currentmush_shape[i]*32, is_mush_poisonous[i]*32, boxPixelsX, boxPixelsY));
		mushroomSprite.setPosition(mushroom[i][x]*boxPixelsX, mushroom[i][y]*boxPixelsY);
		window.draw(mushroomSprite);
	}}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hitMushroom(float bullet[], int mushroom[][exists],int currentmush_shape[],bool is_mush_poisonous[],int& total_mushrooms,int& score){
	for (int i=0;i<total_mushrooms;i++){
		if(bullet[x]/boxPixelsX==mushroom[i][x]&&bullet[y]/boxPixelsY==mushroom[i][y] &&currentmush_shape[i]<4){
			bullet[exists] = false;
			currentmush_shape[i]+=2;
			if(gameGrid[mushroom[i][x]][mushroom[i][y]]<0) {
				gameGrid[mushroom[i][x]][mushroom[i][y]]+=2;
				if (gameGrid[mushroom[i][x]][mushroom[i][y]]==0) score++;
			}
			if (gameGrid[mushroom[i][x]][mushroom[i][y]]==0){
				for (int k=i; k<total_mushrooms; k++){
					mushroom[k][x]=mushroom[k+1][x];
					mushroom[k][y]=mushroom[k+1][y];
					currentmush_shape[k]=currentmush_shape[k+1];
					is_mush_poisonous[k]=is_mush_poisonous[k+1];
				}
				total_mushrooms-=1;
			}
			break;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void spawnCenti(int centipede[][7],int total_centi){
	for (int i=0;i<total_centi;i++){
		centipede[i][y]=0;
		centipede[i][x]=30;
		centipede[i][exists]=1;
		centipede[i][xdir]=0;
		centipede[i][ydir]=1;
		centipede[i][is_head]=0;
		centipede[i][eat_mush]=0;
	}
		
	centipede[0][is_head] = 1;
	centipede[0][x] = boxPixelsX/2;
	centipede[0][y] = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawCentipede(int centipede[][7],int total_centi,sf::RenderWindow& window,sf::Sprite& head_centiSprite, sf::Sprite& body_centiSprite, sf::Clock& animationClock,float centi_speed){
	static int animation=0;
	if (animationClock.getElapsedTime().asMilliseconds() >= centi_speed/4){
	animationClock.restart();
	animation++;
	}
	
	for(int i=0;i<total_centi;++i){
	if (centipede[i][exists]) {
		switch (centipede[i][is_head]){
		case 1:
			if (centipede[i][xdir]==1) {
				head_centiSprite.setRotation(180.0f);
				head_centiSprite.setPosition(centipede[i][x]*boxPixelsX+boxPixelsX, centipede[i][y]*boxPixelsY+boxPixelsY);
			}
			else {
				head_centiSprite.setRotation(0.0f);
				head_centiSprite.setPosition(centipede[i][x]*boxPixelsX, centipede[i][y]*boxPixelsY);
			}
			head_centiSprite.setTextureRect(sf::IntRect(animation*28,0,28, boxPixelsY));
			window.draw(head_centiSprite);
			break;
		case 0:
			if (centipede[i][xdir]==1) {
				body_centiSprite.setRotation(180.0f);
				body_centiSprite.setPosition(centipede[i][x]*boxPixelsX+boxPixelsX, centipede[i][y]*boxPixelsY+boxPixelsY);
			}
			else {
				body_centiSprite.setRotation(0.0f);
				body_centiSprite.setPosition(centipede[i][x]*boxPixelsX, centipede[i][y]*boxPixelsY);
			}
			body_centiSprite.setTextureRect(sf::IntRect(animation*28,0, 28, boxPixelsY));
			window.draw(body_centiSprite);
			break;			
		}
	}}
	if (animation>=7) animation=0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveCentipede(int centipede[][7],int& total_centi,sf::Clock& centipedeClock,int mushroom[][exists],int currentmush_shape[],int& total_mushrooms,bool is_mush_poisonous[],bool& centi_touched_floor,float centi_speed,sf::Sound& eat_se,bool sounde_on, int speed_up){
	static int pre_val[gameColumns*gameRows][6];
	static int counter=0;
	if (!speed_up&&centipedeClock.getElapsedTime().asMilliseconds() < centi_speed)
		return;
	else if (speed_up&&centipedeClock.getElapsedTime().asMilliseconds() < centi_speed/1.5)
		return;
	centipedeClock.restart();
	//MOVING CENTIPEDE ---------------------------------------------------
	for(int i=0;i<total_centi;++i){
	if (centipede[i][exists]==1&&!speed_up||(centipede[i][exists]==2&&speed_up)) {
		pre_val[i][x]=centipede[i][x];
		pre_val[i][y]=centipede[i][y];
		pre_val[i][xdir]=centipede[i][xdir];
		pre_val[i][ydir]=centipede[i][ydir];\
		
		switch (centipede[i][is_head]){
		//FOR HEAD OF CENTIPEDE --------------------------------------------
		case 1:
			//MOVING HEAD --------------------------------------------------
			if(centipede[i][xdir]==0 && (centipede[i][x]-1<0 || gameGrid[centipede[i][x]][centipede[i][y]]<0 || gameGrid[centipede[i][x]-1][centipede[i][y]]>=1) ) 
			{
				if(centipede[i][ydir]==0 && (centipede[i][y])*boxPixelsY<=resolutionY-(1+5)*boxPixelsY)
				{
					centipede[i][y]+=1;
					centipede[i][ydir]=!centipede[i][ydir];
				}
				else if(centipede[i][ydir]==1 && (centipede[i][y]+1)*boxPixelsY>=resolutionY)
				{
					centipede[i][y]-=1;
					centipede[i][ydir]=!centipede[i][ydir];
					centi_touched_floor=1;
				}
				else if(centipede[i][ydir]==0) centipede[i][y]-=1;
				else if(centipede[i][ydir]==1) centipede[i][y]+=1;
				centipede[i][xdir]=!centipede[i][xdir];
			}
			else if(centipede[i][xdir]==1 && ((centipede[i][x]+1)*boxPixelsX>=resolutionX || gameGrid[centipede[i][x]][centipede[i][y]]<0 || gameGrid[centipede[i][x]+1][centipede[i][y]]>=1) ) 
			{
				if(centipede[i][ydir]==0 && (centipede[i][y])*boxPixelsY<=resolutionY-(1+5)*boxPixelsY)
				{
					centipede[i][y]+=1;
					centipede[i][ydir]=!centipede[i][ydir];
				}
				else if(centipede[i][ydir]==1 && (centipede[i][y]+1)*boxPixelsY>=resolutionY)
				{
					centipede[i][y]-=1;
					centipede[i][ydir]=!centipede[i][ydir];
					centi_touched_floor=1;
				}
				else if(centipede[i][ydir]==0) centipede[i][y]-=1;
				else if(centipede[i][ydir]==1) centipede[i][y]+=1;
				centipede[i][xdir]=!centipede[i][xdir];
			}
			else if(centipede[i][xdir]==0) {centipede[i][x]-=1;}
			else if(centipede[i][xdir]==1) {centipede[i][x]+=1;}
			
			//EATING MUSHROOM (REMOVING) -----------------------------------
			for (int j=0;j<total_mushrooms;j++) {
				if (pre_val[i][x]==mushroom[j][x]&&pre_val[i][y]==mushroom[j][y]) {
				if (sounde_on) eat_se.play();
				gameGrid[mushroom[j][x]][mushroom[j][y]]=0; 
				centipede[i][eat_mush]+=1;
				//MAKING PATH FOR CENTIPEDE IF IT EATS POISONOUS MUSHROOM --
				if (is_mush_poisonous[j]&&centipede[i][ydir]) {
					//MAKING DECISION OF LENGH OF PATH ---------------------
					int path_lenght=5;
					path_lenght = rand() % path_lenght; 
					if (gameRows-mushroom[j][y]<10) path_lenght=gameRows-mushroom[j][y]-1;
					else path_lenght += 5;
					for (int k=0;k<path_lenght;k++){
						if (gameGrid[mushroom[j][x]][mushroom[j][y]+k]>=0) 
							gameGrid[mushroom[j][x]][mushroom[j][y]+k]=-4; 
					}
				}
				//REMOVING FROM ARRAY ---------------------------------------
				for (int k=j; k<total_mushrooms; k++){
					mushroom[k][x]=mushroom[k+1][x];
					mushroom[k][y]=mushroom[k+1][y];
					currentmush_shape[k]=currentmush_shape[k+1];
					is_mush_poisonous[k]=is_mush_poisonous[k+1];
				}
				total_mushrooms-=1;
				break;
				}
			}
			
			//UPDATING VALUES ON GRID --------------------------------------
			gameGrid[pre_val[i][x]][pre_val[i][y]]=0;
			gameGrid[centipede[i][x]][centipede[i][y]]+=1;	
			
			//DOUBLE SIZE -------------------------------------------------
			if (centipede[i][eat_mush]==5 && centipede[i][exists] != 2) {
				int lenght;
				centipede[i][eat_mush]=0;
				for (lenght=i+1;lenght<total_centi;lenght++) 
						if(centipede[lenght][is_head]) break;
				lenght=lenght-i;
				for (int k=0;k<lenght;k++){
					total_centi++;
					for (int j=total_centi-1;j>i+lenght+k;j--){
						centipede[j][y]=centipede[j-1][y];
						centipede[j][x]=centipede[j-1][x];
						centipede[j][exists]=centipede[j-1][exists];
						centipede[j][xdir]=centipede[j-1][xdir];
						centipede[j][ydir]=centipede[j-1][ydir];
						centipede[j][is_head]=centipede[j-1][is_head];
						centipede[j][eat_mush]=centipede[j-1][eat_mush];			
					}
				}
				//for (int j=0;j<total_centi;j++) std::cout<<centipede[j][eat_mush];
				for (int j=i+lenght;j<i+2*lenght;j++){
					if (centipede[j-1][xdir]) {
						if (centipede[j-1][x]-1>=0 && gameGrid[centipede[j-1][x]-1][centipede[j-1][y]]==0){
							centipede[j][x]=centipede[j-1][x]-1;
							centipede[j][y]=centipede[j-1][y];
							centipede[j][xdir]=centipede[j-1][xdir];
						}
						else {
							centipede[j][x]=centipede[j-1][x];
							centipede[j][xdir]=!centipede[j-1][xdir];
							if (centipede[j-1][ydir]) centipede[j][y]=centipede[j-1][y]-1;
							else centipede[j][y]=centipede[j-1][y]+1;
						}
					}
					else {
						if (centipede[j-1][x]+1<30 && gameGrid[centipede[j-1][x]+1][centipede[j-1][y]]==0){
							centipede[j][x]=centipede[j-1][x]+1;
							centipede[j][y]=centipede[j-1][y];
							centipede[j][xdir]=centipede[j-1][xdir];
						}
						else {
							centipede[j][x]=centipede[j-1][x];
							centipede[j][xdir]=!centipede[j-1][xdir];
							if (centipede[j-1][ydir]) centipede[j][y]=centipede[j-1][y]-1;
							else centipede[j][y]=centipede[j-1][y]+1;
						}
					}
					centipede[j][exists]=1;
					centipede[j][ydir]=centipede[j-1][ydir];
					centipede[j][is_head]=0;
					centipede[j][eat_mush]=0;
				}
			}
					
			break;
		
		//FOR BODY OF CENTIPEDE --------------------------------------------
		case 0:
			//MOVING BODY --------------------------------------------------
			gameGrid[centipede[i][x]][centipede[i][y]]=0;
			if(i-1>=0){
				centipede[i][x]=pre_val[i-1][x];
				centipede[i][y]=pre_val[i-1][y];
				centipede[i][xdir]=pre_val[i-1][xdir];
				centipede[i][ydir]=pre_val[i-1][ydir];
			}
			gameGrid[centipede[i][x]][centipede[i][y]]=1;	
			break;			
		}
	}}
	
	//ADDIND HEAD CENTIPEDE -------------------------------------------------
	if (centi_touched_floor) {
		if (counter < 100) counter++;
		else {
			counter = 0;
			total_centi++;
			centipede[total_centi-1][x] = (rand() % 2)*29;
			centipede[total_centi-1][y] = 25 + rand() % 5;
			centipede[total_centi-1][exists] = 1;
			centipede[total_centi-1][is_head] = 1;
			centipede[total_centi-1][xdir]=(centipede[total_centi-1][x]==0?1:0);
			centipede[total_centi-1][ydir]=(centipede[total_centi-1][y]==29?0:1);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hitCentipede(int centipede[][7],int& total_centi,float bullet[], int mushroom[][exists],int currentmush_shape[],bool is_mush_poisonous[],int& total_mushrooms,int level,int& score){
	
	for (int j=0;j<total_centi;j++){
		if (centipede[j][exists]) {
			if(bullet[x]/boxPixelsX==centipede[j][x]&&bullet[y]/boxPixelsY==centipede[j][y]&&bullet[exists]){
				bullet[exists] = false;
				
				//SCORE CALCULATION -------------------------
				if (centipede[j][is_head]) score+=20;
				else score+=10;
				
				//RESETING eat_mush --------------------------
				if (!centipede[j][is_head]){
					centipede[j+1][eat_mush]=0;
					for (int temp=j;temp>=0;temp--) {
						if(centipede[temp][is_head]) {
							centipede[temp][eat_mush]=0;
							break;
						}
					}
				}
				//SEPERATING CENTIPEDE------------------------------
				centipede[j][exists]=0;
				if (!centipede[j][is_head]&&!centipede[j+1][is_head]){
					int temp;
					for (temp=j+2;temp<total_centi;temp++) 
						if(centipede[temp][is_head]) break;
					//if(temp==total_centi) 
						temp--;
					gameGrid[centipede[temp][x]][centipede[temp][y]]=0;
					for (int i=temp-1;i>=j+1;i--){
						centipede[i+1][x]=centipede[i][x];
						centipede[i+1][y]=centipede[i][y];
						centipede[i+1][xdir]=centipede[i][xdir];
						centipede[i+1][ydir]=centipede[i][ydir];
					}
					centipede[j+1][y]+=1;
					centipede[j+1][xdir]=!centipede[j+1][xdir];
				}
				if(centipede[j][is_head]&&level<=2) {
					int temp;
					for (temp=j+1;temp<total_centi;temp++) 
						if(centipede[temp][is_head]) break;
						temp--;
						temp=temp-j;
						for(int k=j+temp;k>j;k--){
							for (int i=k; i<total_centi; i++){
								gameGrid[centipede[i][x]][centipede[i][y]]=0;
								centipede[i][y]=centipede[i+1][y];
								centipede[i][x]=centipede[i+1][x];
								centipede[i][exists]=centipede[i+1][exists];
								centipede[i][xdir]=centipede[i+1][xdir];
								centipede[i][ydir]=centipede[i+1][ydir];
								centipede[i][is_head]=centipede[i+1][is_head];	
								centipede[i][eat_mush]=centipede[i+1][eat_mush];
							}
						}
						total_centi-=temp;
				}
				else centipede[j+1][is_head]=1;
				gameGrid[centipede[j][x]][centipede[j][y]]=0;
				
				//ADDING MUSHROOM--------------------------------------------
				if (centipede[j][is_head]==0&&gameGrid[centipede[j][x]][centipede[j][y]]==0){
				total_mushrooms+=1;
				mushroom[total_mushrooms-1][exists] = 1;
				mushroom[total_mushrooms-1][x] = centipede[j][x];
				mushroom[total_mushrooms-1][y] = centipede[j][y];
				currentmush_shape[total_mushrooms-1]=0;
				gameGrid[mushroom[total_mushrooms-1][x]][mushroom[total_mushrooms-1][y]]=-4;
				if ((centipede[j][y])*boxPixelsY>resolutionY-(2+5)*boxPixelsY) is_mush_poisonous[total_mushrooms-1]=1;
				else is_mush_poisonous[total_mushrooms-1]=0;
				}
			
				//FOMATTING ARRAY-----------------------------------------
				for (int i=j; i<total_centi; i++){
					centipede[i][y]=centipede[i+1][y];
					centipede[i][x]=centipede[i+1][x];
					centipede[i][exists]=centipede[i+1][exists];
					centipede[i][xdir]=centipede[i+1][xdir];
					centipede[i][ydir]=centipede[i+1][ydir];
					centipede[i][is_head]=centipede[i+1][is_head];
					centipede[i][eat_mush]=centipede[i+1][eat_mush];	
				}
				total_centi-=1;
				
				//std::cout<<mushroom[i][x]<<" "<<mushroom[i][y]<<std::endl;
				break;
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawLives(int& lives,sf::RenderWindow& window, sf::Sprite& playerSprite) {
	playerSprite.setScale(0.75f, 0.75f);
	int x_axis=960-24;
	for (int i=1;i<lives;i++){
		playerSprite.setPosition(x_axis, 10);
		window.draw(playerSprite);
		x_axis-=24;
	}
	playerSprite.setScale(1.0f, 1.0f);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void playerDeath(float player[],bool& playerDied) {
 if(gameGrid[(int(player[x])/boxPixelsX)][int(player[y])/boxPixelsX]>0) playerDied=1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void drawObject(int arr[], float speed, sf::RenderWindow& window,sf::Sprite& ObjSprite, sf::Clock& animation_Clock,int val){
	static int fleaanimation=0;
	static int scorpionanimation=0;
	static int spideranimation=0;
	float updated_speed = speed/4;
	
	if (val==2) updated_speed = speed/2;
	if (animation_Clock.getElapsedTime().asMilliseconds() >= updated_speed) {
		animation_Clock.restart();
		if (val==2) spideranimation++;
		if (val==3) fleaanimation++;
		if (val==4) scorpionanimation++;
	}
		
	if (val==2) ObjSprite.setTextureRect(sf::IntRect(spideranimation*60,0,60, boxPixelsY));
	if (val==3) ObjSprite.setTextureRect(sf::IntRect(fleaanimation*36,0,boxPixelsX, boxPixelsY));
	ObjSprite.setPosition(arr[x]*boxPixelsX,arr[y]*boxPixelsY);
	
	//MAKEING SCORPION ALLINED TO ITS MOVEMENT ---------------------
	if (val==4) {
		ObjSprite.setTextureRect(sf::IntRect(scorpionanimation*64,0,64, boxPixelsY));
		if (arr[xdir]) { 
			ObjSprite.setScale(-1.f, 1.f);
			ObjSprite.setPosition(arr[x]*boxPixelsX+2*boxPixelsX,arr[y]*boxPixelsY);
		}
		else {
			ObjSprite.setScale(1.f, 1.f);
			ObjSprite.setPosition(arr[x]*boxPixelsX,arr[y]*boxPixelsY);
		}
	}
	window.draw(ObjSprite);
	
	if (fleaanimation>=4) fleaanimation=0;
	if (scorpionanimation>=4) scorpionanimation=0;
	if (spideranimation>=8) spideranimation=0;

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*For the Movement of Sprider I am going to use :
	   -> ydir values for Vertical Movenment (with 1 being down vertical movemnet and 0 for up)
	   -> xdir values for Horizontal Movenment (with -1 : Left, 0 : None, 1 : Right)
	   -> is_head values as counter (single move will repeat 5 times, then movement direction will be chosed again)
	   
	   => Combination of Vertical & Horizontal Movement will make Diagonal Movement
	*/

void moveSpider(int spider[], float spider_speed, sf::Clock& spiderClock, int mushroom[][exists],int currentmush_shape[],int& total_mushrooms,bool is_mush_poisonous[],sf::Sound& spider_se,sf::Sound& eat_se,bool sounde_on) {
	if (spiderClock.getElapsedTime().asMilliseconds() < spider_speed)
		return;
	spiderClock.restart();
	if (!spider[exists]) {
		//CAlCULATING CHANCES -----------------------------------
		int dont_deploy;
		dont_deploy = rand() % 20; 
		if (!dont_deploy) { //One in 10 chance of Deploying spider
			spider[x] = (rand() % 2)*28;
			spider[y] = 20 + rand() % 10;
			spider[exists] = 1;
			spider[is_head] = 0;
			spider[xdir]=(spider[x]==0?1:-1);
			spider[ydir]=0;//(spider[y]==29?-1:1);
		}
	}
	else {
		if (sounde_on && spider[is_head] % 2==0) spider_se.play();
		if(gameGrid[spider[x]][spider[y]]==2) gameGrid[spider[x]][spider[y]]=0;
		if(gameGrid[spider[x]+1][spider[y]]==2) gameGrid[spider[x]+1][spider[y]]=0;
		
		if (spider[is_head]<5){
			spider[is_head]++;
			//Vertical Movement -------------------------
			if (spider[ydir]==1) spider[y]++;
			else spider[y]--;
				//Vetical boarders for spider 
				if (spider[y] == 29 || spider[y] == 15) {
					spider[ydir] = !spider[ydir];
					spider[is_head]--;
				}
				else if ( gameGrid[spider[x]][spider[y]]>0) {
					(spider[ydir] ? spider[y]--:spider[y]++); 
					spider[ydir] = !spider[ydir];
					spider[is_head]--;
				}
				//to deal with anomaly which occurs when centipede pushes spider out of boader 
				if (spider[y] == 30) spider[exists] = false; 
			//Horizontal Movement -----------------------
			if (spider[xdir]==1) spider[x]++;
			else if (spider[xdir]==-1) spider[x]--;
				//Horizontal boarders for spider 
				if (spider[x] == 30 || spider[x] == -1) {
					spider[exists] = false;
					spider[is_head]--;
					spider[x]++;
					spider[y]=30;
				}
		}
		else {
			spider[is_head]=0;
			//Making next movement decision -------------
			int temp;
			temp = rand() % 2; //for vertical movement
				if (temp) spider[ydir] = 1;
				else spider[ydir] = 0;
			temp = rand() % 3; //for horizontal movement
				if (temp==1) spider[xdir] = 1;
				else if (temp==2) spider[xdir] = -1;
				else spider[xdir] = 0;
		}
		
		//EATING MUSHROOM (REMOVING) -----------------------------------
		if (spider[x]>1) {
			for (int j=0;j<total_mushrooms;j++) {
				if ((spider[x]==mushroom[j][x]||spider[x]+1==mushroom[j][x])&&spider[y]==mushroom[j][y]) {
				gameGrid[mushroom[j][x]][mushroom[j][y]]=0;
				if (sounde_on) eat_se.play();
				for (int k=j; k<total_mushrooms; k++){
					mushroom[k][x]=mushroom[k+1][x];
					mushroom[k][y]=mushroom[k+1][y];
					currentmush_shape[k]=currentmush_shape[k+1];
					is_mush_poisonous[k]=is_mush_poisonous[k+1];
				}
				total_mushrooms-=1;
				break;
				}
			}
		}
		//UPDATING GRID ---------------------------------------------------
		gameGrid[spider[x]][spider[y]]=2;
		gameGrid[spider[x]+1][spider[y]]=2;
	}  
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hitSpider(float player[],float bullet[],int spider[],int& score, sf::RenderWindow& window,sf::Sprite& spiderSprite,sf::Clock& hit_spiderClock,sf::Sound& kill_se,bool sounde_on) {
	static bool display=0;
	if (spider[exists] ) {
		display=0;
		if((bullet[x]/boxPixelsX==spider[x]||bullet[x]/boxPixelsX==spider[x]+1)&&bullet[y]/boxPixelsY==spider[y]&&bullet[exists]){
			display=1;
			if (sounde_on) kill_se.play();
			bullet[exists] = false;
			spider[exists] = false;
			if(gameGrid[spider[x]][spider[y]]==2) gameGrid[spider[x]][spider[y]]=0;
			if(gameGrid[spider[x]+1][spider[y]]==2) gameGrid[spider[x]+1][spider[y]]=0;
			if (spider[y]>=player[y]/boxPixelsY-2) {
				spiderSprite.setTextureRect(sf::IntRect(2*60,32,60, boxPixelsY));
				score+=900;
			}	
			else if (spider[y]>=(player[y]/boxPixelsX)-3)	{
				spiderSprite.setTextureRect(sf::IntRect(1*60,32,60, boxPixelsY));
				score+=600;
			}
			else {
				spiderSprite.setTextureRect(sf::IntRect(0,32,60, boxPixelsY));
				score+=300;
			}
			spiderSprite.setPosition(spider[x]*boxPixelsX,spider[y]*boxPixelsY);
			window.draw(spiderSprite);
		}
		hit_spiderClock.restart();
	}
	else {
	 if(display && hit_spiderClock.getElapsedTime().asMilliseconds() < 500) window.draw(spiderSprite);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveFlea(int flea[], float flea_speed, sf::Clock& fleaClock, int mushroom[][exists],int currentmush_shape[],int& total_mushrooms,bool is_mush_poisonous[],sf::Sound& flea_se, bool sounde_on){
	if (fleaClock.getElapsedTime().asMilliseconds() < flea_speed)
		return;
	fleaClock.restart();
	if (!flea[exists]) {
		int counter=0;
		for(int i=0;i<total_mushrooms;i++){
			if (mushroom[i][y]>=24) counter++;
		}
		if (counter<=3){
			flea[exists]=true;
			flea[y] = 0;
			flea[x] = rand()%30;
			if (sounde_on) flea_se.play();
			gameGrid[flea[x]][flea[y]]=0;
		}
	}
	else {
		if (gameGrid[flea[x]][flea[y]]==3) gameGrid[flea[x]][flea[y]]=0;
		int dont_deploy;
		dont_deploy = rand() % 03; //One in 3 chance of Deploying mushroom  
		if (flea[y]>=24) dont_deploy = rand() % 02; //with higher chance in player area
		if (!dont_deploy){ //check if there is already mushroom present
			for (int j=0;j<total_mushrooms;j++) {
				if (flea[x]==mushroom[j][x]&&flea[y]==mushroom[j][y]&&mushroom[j][exists]) {
				dont_deploy=1;
				}
			}
		}
		if (!dont_deploy&&flea[y]!=29) { 
			total_mushrooms++;
			mushroom[total_mushrooms-1][exists] = true;
			mushroom[total_mushrooms-1][y] = flea[y];
			mushroom[total_mushrooms-1][x] = flea[x];
			is_mush_poisonous[total_mushrooms-1] = 0;
			currentmush_shape[total_mushrooms-1] = 0;
			gameGrid[flea[x]][flea[y]]=-4;
		}
		//UPDATING MOVEMENT OF FLEA ----------------------------------------
		flea[y]++;
		if (gameGrid[flea[x]][flea[y]]==0) gameGrid[flea[x]][flea[y]]=3;
		if (flea[y]==30) {
			flea[exists] = false;
			gameGrid[flea[x]][flea[y]]=0;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hitFlea(float bullet[],int flea[],int& score,sf::Sound& kill_se,bool sounde_on){
	if (flea[exists]) {
		if(bullet[x]/boxPixelsX==flea[x]&&bullet[y]/boxPixelsY==flea[y]&&bullet[exists]){
			if (sounde_on) kill_se.play();
			bullet[exists] = false;
			flea[exists] = false;	
			gameGrid[flea[x]][flea[y]]=0;
			score+=200;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void moveScorpion(int scorpion[], float scorpion_speed, sf::Clock& scorpionClock, int mushroom[][exists],int& total_mushrooms,bool is_mush_poisonous[],sf::Sound& scorpion_se, bool sounde_on){
	if (scorpionClock.getElapsedTime().asMilliseconds() < scorpion_speed)
		return;
	scorpionClock.restart();
	if (!scorpion[exists]) {
		//CAlCULATING CHANCES -----------------------------------
		int dont_deploy;
		dont_deploy = rand() % 500; 
		if (!dont_deploy) { //One in 100 chance of Deploying scorpion
			scorpion[x] = (rand() % 2)*29;
			scorpion[y] = rand() % 20;
			scorpion[3] = 1;
			scorpion[exists] = 1;
			scorpion[xdir]=(scorpion[x]==0?1:0);
			if (sounde_on) scorpion_se.play();
		}
	}
	else {
		//PLAYING SOUND EFFECTS FOR SCORPION --------------------
		if (scorpion[3]==7){
			scorpion[3]=1;
			if (sounde_on) scorpion_se.play();
		}
		else scorpion[3]++;
		//UPDATING VALUES ---------------------------------------
		if (scorpion[xdir]) scorpion[x]++;
		else if (scorpion[xdir]==0) scorpion[x]--;
		//CHECKING BOADERS --------------------------------------
		if (scorpion[x]<0||scorpion[x]>=30) scorpion[exists]=false;
		//TURNING MUSHROOM POISONOUS ----------------------------
		if (scorpion[exists]) {
			for (int j=0;j<total_mushrooms;j++) {
				if ((scorpion[x]==mushroom[j][x]||scorpion[x]+1==mushroom[j][x])&&scorpion[y]==mushroom[j][y]) {
					is_mush_poisonous[j]=1;
					break;
				}
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void hitScorpion(float bullet[],int scorpion[],int& score,sf::Sound& kill_se,bool sounde_on){
	if (scorpion[exists]) {
		if((bullet[x]/boxPixelsX==scorpion[x]||bullet[x]/boxPixelsX==scorpion[x]+1)&&bullet[y]/boxPixelsY==scorpion[y]&&bullet[exists]){
			if (sounde_on) kill_se.play();
			bullet[exists] = false;
			scorpion[exists] = false;	
			score+=1000;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void updateLives(int& lives, int score) {
	static int milestone = 0;
	static int score_done=0; //0 means to check next 10,000 points, 1 : 12,000, 2 : 15,000, 3 : 20,000
	switch (score_done) {
	case 0:	
		if (lives<=6&&score>=10000) {lives++; score_done++;}
		break;
	case 1:	
		if (lives<=6&&score>=12000) {lives++; score_done++;}
		break;
	case 2:	
		if (lives<=6&&score>=15000) {lives++; score_done++;}
		break;
	case 3:	
		if (lives<=6&&score>=20000) {lives++; score_done++;}
		break;
	}
	if (score_done==4) {
		milestone += 20000;
		score_done =0;
	}
}
