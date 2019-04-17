#include "game.h"
#include <thread>
#include "Collision.hpp"
#include <iomanip>
#include <locale>
#include "imgui.h"
#include "imgui-SFML.h"
#include "osdialog.h"

Game::Game(Data &constantdata) : GWindow(sf::VideoMode(constantdata.ScreenWidth, constantdata.ScreenHeight), "Car Racing"), ConstantData(constantdata), popManager(constantdata)
{
    //ctor
}

Game::~Game()
{
    //dtor
}

bool Game::Start()
{
   
    GWindow.setPosition(ConstantData.WindowPos);
    
    ImGui::SFML::Init(GWindow);
    bool Restart = true;
    TextOverlay = Overlay();
    TextOverlay.SetGLobalScale(ConstantData.ScaleX, ConstantData.ScaleY); 
    
    if(ConstantData.PopSize > 0 && Setup())
    {
        Restart = Run();
    }
    else
    {
        Restart = false;
    }
    GWindow.close();

    return Restart;
}

bool Game::Run()
{
    Exit = false;
    popManager.CreateStartingPopulation();
    popManager.StartRace();
    OverallPlayerLapTime = sf::seconds(0.f);
    while(GWindow.isOpen() && !Exit)
    {
        GamePlay();
        OrganiseStats();
        Render();
        if(!ConstantData.usePlayer)
        {
            if(popManager.getGeneration() > Generation)
            {
                Generation = popManager.getGeneration();
                OverallPlayerLapTime = OverallPlayerLapTime.Zero;
            }
            if(ConstantData.GenerationLength.asSeconds() <= OverallPlayerLapTime.asSeconds())
            {
                popManager.BreedNewPopulation();
            }
        }
    }
    if(GWindow.isOpen())
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Game::Setup()
{
    GWindow.setFramerateLimit(62);
    
    //LOAD FONT
    TextOverlay.loadFont("Sansation","Media/fonts/mgbold.ttf");
    //Car Setup
    //////////////////////////
 
    
    
    TrackPoints trackPoints;
    
    if(!BackgroundTexture.loadFromFile("Circuits/TrackVis_" + std::to_string(ConstantData.TrackNumber) +".png"))
    {
        std::cerr << "Cant load Track" << std::endl;
        return false;
    }
    ConstantData.Track->loadFromFile("Circuits/TrackBac_" + std::to_string(ConstantData.TrackNumber) +".png");
    
    for(int y{0}; y<ConstantData.Track->getSize().y; y++)
    {
        for(int x{0}; x<ConstantData.Track->getSize().x; x++)
        {            
            if(ConstantData.Track->getPixel(x,y).r == 58 && ConstantData.Track->getPixel(x,y).g >= 255 && ConstantData.Track->getPixel(x,y).b == 248 && ConstantData.Track->getPixel(x,y).a == 255)
            {
                ConstantData.StartPos = sf::Vector2f(x*ConstantData.ScaleX, y*ConstantData.ScaleY);
            }
            if(ConstantData.Track->getPixel(x,y).r == 100 && ConstantData.Track->getPixel(x,y).g >= 100 && ConstantData.Track->getPixel(x,y).b == 0 && ConstantData.Track->getPixel(x,y).a == 100)
            {
                trackPoints.AddPoint(x*ConstantData.ScaleX,y*ConstantData.ScaleY,ConstantData.Track->getPixel(x,y).r,ConstantData.Track->getPixel(x,y).g);
            }
        }
    }
    
    trackPoints.OrderPoints();

    ConstantData.pointvector = trackPoints.GetPoints(); 
    

    
    ConstantData.clock.restart();
    
    //////////////////////////
    //Background-Track Setup
    BackgroundSprite.setTexture(BackgroundTexture,true);
    BackgroundSprite.scale(ConstantData.ScaleX,ConstantData.ScaleY); //Zoom
    BackgroundTexture.setSmooth(true);
    
    //Stats setup
    //////////////////////////
    if(TextOverlay.CreateOverlay("Statistics","Sansation",10,false))
    {
        TextOverlay.SetPosition("Statistics",sf::Vector2f(5.f,5.f));
    }
    timeSinceLastUpdate = sf::Time::Zero;
    //////////////////////////
    //UI Setup
    if(TextOverlay.CreateOverlay("Speedo","Sansation",60))
    {
        TextOverlay.SetPosition("Speedo",sf::Vector2f(ConstantData.ScreenWidth-ConstantData.ScreenWidth/8,ConstantData.ScreenHeight - ConstantData.ScreenHeight/10));
    }
    if(TextOverlay.CreateOverlay("LapTime", "Sansation",20))
    {
        TextOverlay.SetPosition("LapTime",sf::Vector2f(ConstantData.ScreenWidth/2,10));
    }
    if(TextOverlay.CreateOverlay("Sector1", "Sansation",30))
    {
        TextOverlay.SetPosition("Sector1",sf::Vector2f(ConstantData.ScreenWidth-150,10));
    }
    if(TextOverlay.CreateOverlay("Sector2", "Sansation",30))
    {
        TextOverlay.SetPosition("Sector2",sf::Vector2f(ConstantData.ScreenWidth-150,50));
    }
    if(TextOverlay.CreateOverlay("Sector3", "Sansation",30))
    {
        TextOverlay.SetPosition("Sector3",sf::Vector2f(ConstantData.ScreenWidth-150,90));
    }
    if(TextOverlay.CreateOverlay("camera", "Sansation", 30 ,true,"Currently looking at Player " + std::to_string(popManager.getCameraID() )))
    {
        TextOverlay.SetPosition("camera",sf::Vector2f((ConstantData.ScreenWidth/2),ConstantData.ScreenHeight-100));
    }
    if(TextOverlay.CreateOverlay("generation", "Sansation", 30, false, "Generation: " + std::to_string(popManager.getGeneration())))
    {
        TextOverlay.SetPosition("generation",sf::Vector2f(5,50));
    }
    if(TextOverlay.CreateOverlay("popsize", "Sansation", 30, false, "popsize: " + std::to_string(popManager.getGeneration())))
    {
        TextOverlay.SetPosition("popsize",sf::Vector2f(5,80));
    }
    if(TextOverlay.CreateOverlay("numberdead", "Sansation", 30, false, "Dead: " ))
    {
        TextOverlay.SetPosition("numberdead",sf::Vector2f(5,110));
    }
    if(TextOverlay.CreateOverlay("FastestLap", "Sansation", 30, false, "Fastest Lap: " ))
    {
        TextOverlay.SetPosition("FastestLap",sf::Vector2f(5,140));
    }
    
    return true;
}
void Game::OrganiseStats()
{
    //Stats
    //////////////////////////
    mStatisticsUpdateTime += ConstantData.elapsedTime;
    mStatisticsNumFrames += 1;
    
    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        TextOverlay.SetDataOutput("Statistics", "Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
                                  "Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");
        
        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}

void Game::GamePlay()
{
    //TimeStep
    ConstantData.elapsedTime = ConstantData.clock.restart();
    TotalTime += timeSinceLastUpdate/10.f;
    timeSinceLastUpdate += ConstantData.elapsedTime;
    ImGui::SFML::Update(GWindow, ConstantData.elapsedTime);
   // while (timeSinceLastUpdate > TimePerFrame)
    {
        timeSinceLastUpdate -= TimePerFrame;
        
        sf::Event e;
        while (GWindow.pollEvent(e))
        {
            ImGui::SFML::ProcessEvent(e);
            if (e.type == sf::Event::Closed)
                GWindow.close();
        }
        
        if(Racing && !PAUSE)
        {
            popManager.UpdateCars(); 

            Input();
            Movement();
            Collisions();
            UpdateOverlays();
            OverallPlayerLapTime += ConstantData.elapsedTime;
            //////////////////////////
        }
        else if(PAUSE)
        {
            Input();
        }
        else
        {
            Exit = true;
            return;
        }
    }
}

void Game::SetupPostRace()
{
    TextOverlay.ClearAll();
    SaveDataToFile();

    
}

void Game::UpdateOverlays()
{
    TextOverlay.SetDataOutput("camera", "Currently looking at Player " + std::to_string(popManager.getCameraID()));
    TextOverlay.SetDataOutput("LapTime", OverallPlayerLapTime.asSeconds());
    popManager.GetCar(popManager.getCameraID()).UpdatePlayerSpeedo(TextOverlay);
    TextOverlay.SetDataOutput("generation", "Generation: " + std::to_string(popManager.getGeneration()));
    TextOverlay.SetDataOutput("popsize", "Number Agents: " + std::to_string(popManager.SizeOfCarArray()));
    TextOverlay.SetDataOutput("numberdead", "Dead: " + std::to_string(popManager.getDead()));
    TextOverlay.SetDataOutput("FastestLap", "FastestLap: " + std::to_string(popManager.getFastestLap()));
    TextOverlay.SetDataOutput("camera", "Currently looking at Player " + std::to_string(popManager.getCameraID()));

}

void Game::Render()
{
    //Render
    if(Racing)
    {

        GWindow.clear(sf::Color(ConstantData.Track->getPixel(0,0).r,ConstantData.Track->getPixel(0,0).g,ConstantData.Track->getPixel(0,0).b));

        auto offset = Helper::UpdateOffset(popManager.GetCar(popManager.getCameraID()).getPosX(),popManager.GetCar(popManager.getCameraID()).getPosY(),ConstantData.ScreenHeight, ConstantData.ScreenWidth);
        BackgroundSprite.setPosition(-offset.first,-offset.second);

        GWindow.draw(BackgroundSprite);
        
        popManager.RenderCars(GWindow);
    }
    else
    {
        GWindow.clear(sf::Color::Black);
    }
    TextOverlay.RenderAllOverlays(GWindow);
    
    ImGui::SFML::Render(GWindow);
    GWindow.display();
}

void Game::Input()
{
    popManager.UpdateGUI(PAUSE);
    ImGui::Begin("InGame Menu");
    if(ImGui::Button("Pause"))
    {
        PAUSE = !PAUSE; 
    }
    if(ImGui::Button("Exit") || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
    {
        PAUSE = true;
        int hey = osdialog_message(OSDIALOG_WARNING, OSDIALOG_YES_NO, "Are you sure you want to exit?");
        if(hey)
        {
            Racing = false;
            PAUSE = false;
        }
    }
   
    ImGui::Checkbox("Show Feelers", &ConstantData.ShowEyes);
    
    if(popManager.TotalFitnessHistory.size()>1)
    {
        float fitnesshistory[popManager.TotalFitnessHistory.size()];
        
        for(int i{0}; i < popManager.TotalFitnessHistory.size(); i++)
        {
            fitnesshistory[i] = popManager.TotalFitnessHistory[i];
        }
        ImGui::PlotHistogram("Total Fitness \nPer Generation", fitnesshistory, IM_ARRAYSIZE(fitnesshistory), 0, NULL, 0.0f, popManager.TotalFitnessHistory[popManager.TotalFitnessHistory.size()-1]+popManager.TotalFitnessHistory[popManager.TotalFitnessHistory.size()-1]*0.25, ImVec2(0,100));
    }
    if(popManager.AverageLapTimes.size() > 1)
    {
        float AverageLaptimes[popManager.AverageLapTimes.size()];
        for(int i{0}; i < popManager.AverageLapTimes.size(); i++)
        {
            AverageLaptimes[i] = popManager.AverageLapTimes[i];
        }
        float BestLapTimes[popManager.BestLapTimes.size()];
        for(int i{0}; i < popManager.BestLapTimes.size(); i++)
        {
            BestLapTimes[i] = popManager.BestLapTimes[i];
        }
        ImGui::PlotLines("Best Laptime", BestLapTimes, IM_ARRAYSIZE(BestLapTimes));
        ImGui::PlotHistogram("Average Laptime \nPer Generation", AverageLaptimes, IM_ARRAYSIZE(AverageLaptimes), 0, NULL, 0.0f, popManager.AverageLapTimes[popManager.AverageLapTimes.size()-1]+popManager.AverageLapTimes[popManager.AverageLapTimes.size()-1] * 0.25, ImVec2(0,100));
    }
    
    if(PAUSE)
    {
        float gentime = ConstantData.GenerationLength.asSeconds();
        ImGui::SliderFloat("Generation Time", &gentime , OverallPlayerLapTime.asSeconds(), 600.f);
        ConstantData.GenerationLength = sf::seconds(gentime);
    }

    ImGui::End();

    

    
    
}





void Game::Collisions()
{

}

void Game::Movement()
{
    
}



void Game::SaveDataToFile()
{
}
