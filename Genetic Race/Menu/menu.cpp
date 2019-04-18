
#include "menu.h"
#include "osdialog.h"
#include "Database.hpp"
#include "imgui_user.h"

Menu::Menu(Data &constantdata) : window(sf::VideoMode(800, 800), "Car Racing Lobby"), ConstantData(constantdata)
{
    //ctor
    
}

Menu::~Menu()
{
    //dtor
}

void Menu::Run()
{
    Setup();
    Loop();
    ConstantData.WindowPos = window.getPosition();
    ConstantData.SaveDataToFile();
    window.close();
    ImGui::SFML::Shutdown();
}
void Menu::Setup()
{
    if(!window.isOpen())
    {
        window.create(sf::VideoMode(800, 600), "Car Racing ");
    }
    window.setFramerateLimit(62);
    ImGui::SFML::Init(window);
    //setup SMFL
    LobbyItems = Overlay();
    LobbyItems.loadFont("Font", "Media/fonts/mgbold.ttf");
    
    
    LobbyItems.CreateOverlay("Statistics", "Font",10,false);
    
    track.loadFromFile("Circuits/TrackVis_" + std::to_string(ConstantData.TrackNumber) + ".png");
    
    tracksprite.setTexture(track);
    tracksprite.setScale(0.2, 0.2);
    
    
    CarStats.LoadFromFile(ConstantData.CarFilePath);
    ConstantData.database = new Database();
    ConstantData.database->Open("./SQDatabase.db");
    
}
void Menu::Loop()
{
    while( window.isOpen() && !ConstantData.Playing)
    {
        Update();
        Input();
        OrganiseStats();
        Render();
    }
}

void Menu::Input()
{
    ImGui::BeginTabBar("tabbar");
    if(ImGui::BeginTabItem("MainMenu"))
    {
        MainMenu();
        ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("CarEditor"))
    {
        CarEditor();
        ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("GeneEditor"))
    {
        GeneEditor();
        ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("GeneChooser"))
    {
        GeneChooser();
        ImGui::EndTabItem();
    }
    if(ImGui::BeginTabItem("Algorithm"))
    {
        AlgorithmEditor();
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
}

void Menu::AlgorithmEditor()
{
    if(ImGui::Checkbox("Elitism on/off", &ConstantData.Elitism))
    {
        ConstantData.ElitismNum = 10;
    }

    
    if(ConstantData.Elitism)
    {
        ImGui::SliderInt("Number of the Fastest Cars to push through", &ConstantData.ElitismNum, 1, ConstantData.PopSize );
        ImGui::Checkbox("Should the chosen Elites be mutated? on/off", &ConstantData.MutateElites);
    }
    else if(ConstantData.ElitismNum != 0)
    {
        ConstantData.ElitismNum = 0;
    }
    ImGui::SliderInt("Mutation Rate", &ConstantData.MutationRate, 0, 100 );
}
void Menu::Update()
{    
    elapsedTime = clock.restart();
    TotalTime += timeSinceLastUpdate/10.f;
    timeSinceLastUpdate += elapsedTime;
    ImGui::SFML::Update(window, elapsedTime);
    while (timeSinceLastUpdate > TimePerFrame)
    {
        timeSinceLastUpdate -= TimePerFrame;
        
        sf::Event e;
        while (window.pollEvent(e))
        {
            ImGui::SFML::ProcessEvent(e);
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        
    }
    
}
void Menu::Render()
{
    window.clear(sf::Color::Black);
    LobbyItems.RenderAllOverlays(window);
    ImGui::SFML::Render(window);
    window.display();
}

void Menu::OrganiseStats()
{
    //Stats
    //////////////////////////
    mStatisticsUpdateTime += elapsedTime;
    mStatisticsNumFrames ++;
    
    if (mStatisticsUpdateTime >= sf::seconds(1.0f))
    {
        LobbyItems.SetDataOutput("Statistics", "Frames / Second = " + std::to_string(mStatisticsNumFrames) + "\n" +
                                 "Time / Update = " + std::to_string(mStatisticsUpdateTime.asMicroseconds() / mStatisticsNumFrames) + "us");
        
        mStatisticsUpdateTime -= sf::seconds(1.0f);
        mStatisticsNumFrames = 0;
    }
}
void Menu::Load()
{
    Loaded = true;
    if(user)
    {
        genereturns.clear();
        sizeGene = ConstantData.database->getSize(eTable::eGene);
        
        if(sizeGene > 0)
        {
            for(int i{1}; i <= sizeGene; i++)
            {
                GeneTableStruct nextgene;
                if(ConstantData.database->Select("SELECT * FROM Gene WHERE ID = " + std::to_string(i) + ";", nextgene))
                {
                    if(nextgene.ID != -1)
                    {
                        genereturns.push_back(nextgene);
                    }
                    
                }
                
            }
        }
    }
    if(Records)
    {
        Lapreturns.clear();
        sizeLap = ConstantData.database->getSize(eTable::eLapTime);
        if(sizeLap > 0)
        {
            for(int i{1}; i <= ConstantData.NumberOfTracks; i++)
            {
                LapTimeTableStruct nextRecord;
                if(ConstantData.database->Select("SELECT * FROM LapTimeRecords WHERE TrackID = " + std::to_string(i) + ";", nextRecord))
                {
                    if(nextRecord.TrackID != -1)
                    {
                        Lapreturns.push_back(nextRecord);
                    }
                }
                
            }
        }
    }
}
void Menu::GeneEditor()
{
    static bool CreateOwn = false;
    if(ConstantData.savedChromosomes.size() > 0 || CreateOwn)
    {
        
        if(CreateOwn && ConstantData.Chromosomes.size() == 0)
        {
            Chromosome ch;
            ch.AddGene(3,255,0);
            ch.AddGene(2,1,0);
            ch.AddGene(1,100,1);
            ch.AddGene(2,1,0);
            ch.AddGene(1,CarStats.MaxSpeed,0.5);
            ch.AddGene(3,255,0);
            ch.EndAddingGenes();
            ConstantData.savedChromosomes.push_back(std::pair<std::string,std::string>("Custom Chromosome",ch.ToString()));
        }
        
        ImGui::ListBox("Selected Genes", &selectedgenesplace,SelectedItems );
        if(ConstantData.savedChromosomes.size() != ConstantData.Chromosomes.size() || CreateOwn)
        {
            Chromosome ch;
            ch.AddGene(3,255,0);
            ch.AddGene(2,1,0);
            ch.AddGene(1,100,1);
            ch.AddGene(2,1,0);
            ch.AddGene(1,CarStats.MaxSpeed,0.5);
            ch.AddGene(3,255,0);
            ch.EndAddingGenes();
            
            SelectedItems.clear();
            for(int i{0}; i< ConstantData.savedChromosomes.size(); i++)
            {
                SelectedItems.push_back(std::to_string(i+1) + ": " + ConstantData.savedChromosomes[i].first + ", " + ConstantData.savedChromosomes[i].second);
                
                ch.FromString(ConstantData.savedChromosomes[i].second);
                ConstantData.Chromosomes.push_back(ch);
            }
            
        }
        CreateOwn = false;
        int Brake{ConstantData.Chromosomes[selectedgenesplace].GetGene(0)},Turnin{ConstantData.Chromosomes[selectedgenesplace].GetGene(1)},Turnout{ConstantData.Chromosomes[selectedgenesplace].GetGene(2)},Turninam{ConstantData.Chromosomes[selectedgenesplace].GetGene(3)},feeleryesno{ConstantData.Chromosomes[selectedgenesplace].GetGene(4)},FeelerLen{ConstantData.Chromosomes[selectedgenesplace].GetGene(5)},maxde{ConstantData.Chromosomes[selectedgenesplace].GetGene(6)},minde{ConstantData.Chromosomes[selectedgenesplace].GetGene(7)},minspeed{ConstantData.Chromosomes[selectedgenesplace].GetGene(8)},R{ConstantData.Chromosomes[selectedgenesplace].GetGene(9)},G{ConstantData.Chromosomes[selectedgenesplace].GetGene(10)},B{ConstantData.Chromosomes[selectedgenesplace].GetGene(11)};
        
        if(ImGui::SliderInt("Braking Point", &Brake, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(0), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(0)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(0, Brake);
        }
        if(ImGui::SliderInt("Turn In Point", &Turnin, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(1), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(1)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(1, Turnin);
        }
        if(ImGui::SliderInt("Turn Out Point", &Turnout, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(2), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(2)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(2, Turnout);
        }
        if(ImGui::SliderInt("Turn In Amount", &Turninam, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(3), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(3)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(3, Turninam);
        }
        if(ImGui::SliderInt("Longer Central Feeler", &feeleryesno, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(4), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(4)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(4, feeleryesno);
        }
        if(ImGui::SliderInt("Feeler Length", &FeelerLen, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(5), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(5)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(5, FeelerLen);
        }
        if(ImGui::SliderInt("MaxDisttoEdge", &maxde, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(6), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(6)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(6, maxde);
        }
        if(ImGui::SliderInt("MinDIsttoedge", &minde, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(7), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(7)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(7, minde);
        }
        if(ImGui::SliderInt("Lowest Speed", &minspeed, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(8), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(8)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(8, minspeed);
        }
        if(ImGui::SliderInt("Red", &R, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(9), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(9)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(9, R);
        }
        if(ImGui::SliderInt("Green", &G, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(10), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(10)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(10, G);
        }
        if(ImGui::SliderInt("Blue", &B, ConstantData.Chromosomes[selectedgenesplace].GetMinValue(11), ConstantData.Chromosomes[selectedgenesplace].GetMaxValue(11)))
        {
            ConstantData.Chromosomes[selectedgenesplace].SetInt(11, B);
        }
        
        if(ImGui::Button("Reset this Gene"))
        {
            ConstantData.Chromosomes[selectedgenesplace].FromString(ConstantData.savedChromosomes[selectedgenesplace].second);
        }
        if(ImGui::Button("Add Gene to Breeding Slot 1"))
        {
            Breedingspot.first = selectedgenesplace;
        }
        if(ImGui::Button("Add Gene to Breeding Slot 2"))
        {
            Breedingspot.second = selectedgenesplace;
        }
        std::string slot1;
        std::string slot2;
        if(Breedingspot.first == -1)
        {
            slot1 = "Breeding Spot 1 = EMPTY ";
        }
        else
        {
            slot1 = "Breeding Spot 1 = " + SelectedItems[Breedingspot.first].substr(3) + " ";
        }
        if(Breedingspot.second == -1)
        {
            slot2 = "Breeding Spot 2 = EMPTY ";
        }
        else
        {
            slot2 = "Breeding Spot 2 = " + SelectedItems[Breedingspot.second].substr(3) + " ";
        }
        ImGui::Text("%s", slot1.c_str());
        ImGui::Text("%s", slot2.c_str());
        if(Breedingspot.first != -1 || Breedingspot.second != -1 )
        {
            ImGui::Text("Mutate a chromosome, the higher the Mutation rate, the more chance each gene will be mutated");
            static int MutationRate = 70;
            ImGui::SliderInt("MutationRate", &MutationRate,0,100);
            if(Breedingspot.first != -1 )
            {
                if(ImGui::Button("Mutate Slot 1"))
                {
                    ConstantData.Chromosomes[Breedingspot.first].Mutate(MutationRate);
                    ConstantData.savedChromosomes[Breedingspot.first].second = ConstantData.Chromosomes[Breedingspot.first].ToString();
                    CreateOwn = true;
                }
            }
            if(Breedingspot.second != -1)
            {
                if(ImGui::Button("Mutate Slot 2"))
                {
                    ConstantData.Chromosomes[Breedingspot.second].Mutate(MutationRate);
                    ConstantData.savedChromosomes[Breedingspot.second].second = ConstantData.Chromosomes[Breedingspot.second].ToString();
                     CreateOwn = true;
                }
            }
            if(Breedingspot.first != -1 && Breedingspot.second != -1 )
            {
            static char stringinput[128] = "My Player";
            std::string defaultname = "My Player";
            char hint[defaultname.length() + 1];
            strcpy(hint, defaultname.c_str());
            ImGui::InputTextWithHint("Name this new Chromosome", hint, stringinput, IM_ARRAYSIZE(stringinput));
            if(ImGui::Button("Breed and add these genes"))
            {
                Chromosome newChromosome = ConstantData.Chromosomes[Breedingspot.first];
                newChromosome.Crossover(ConstantData.Chromosomes[Breedingspot.first], ConstantData.Chromosomes[Breedingspot.second]);
                
                std::pair<std::string,std::string> newpair(std::string(stringinput), newChromosome.ToString());
                ConstantData.savedChromosomes.push_back(newpair);
                
                std::string sql = "INSERT INTO GENE (Name,DateTime,TrackNumber,FastestLapTime,Genes,Generation) VALUES('"+ std::string(stringinput) + "', '" + Helper::GetDateTime() + "',  '" + std::to_string(99) + "',   '" + std::to_string(299) +   "',    '" + newChromosome.ToString()+"',     '" + std::to_string(1) +"');";
                
                ConstantData.database->Insert(sql);
            }
            }
        }
    }
    else
    {
        ImGui::Text("Please go to the GeneChooser tab and select some genes to edit!\nOr Press the button below to create your own!");
        if(ImGui::Button("Create My Own"))
        {
            CreateOwn = true;
        }
    }
}
void Menu::GeneChooser()
{
    if(ImGui::Checkbox("User Saves Table", &user))
    {
        Records = false;
        Loaded = false;
    }
    if(ImGui::Checkbox("Lap Records Table", &Records))
    {
        user = false;
        Loaded = false;
    }
    
    if(ImGui::Button("Load"))
    {
        Load();
        listbox_items.clear();
    }
    if( Loaded)
    {
        if(user)
        {
            if(listbox_items.size() != genereturns.size())
                listbox_items.resize(genereturns.size());
            for(int i{0}; i<genereturns.size(); i++)
            {
                listbox_items[i] = genereturns[i].toString();
            }
            ImGui::ListBox("User Saves", &GeneListPlace,listbox_items );
            
        }
        else if(Records)
        {
            if(listbox_items.size() != sizeLap)
                listbox_items.resize(sizeLap);
            for(int i{0}; i<sizeLap; i++)
            {
                listbox_items[i] = Lapreturns[i].toString();
            }
            ImGui::ListBox("LapRecords", &RecordListPlace,listbox_items );
        }
        if(ImGui::Button("Use/Edit Selected chromosome from selected item"))
        {
            if(user)
            {
                std::pair<std::string,std::string> genePair(genereturns[GeneListPlace].Name, genereturns[GeneListPlace].chromosome);
                ConstantData.savedChromosomes.push_back(genePair);
            }
            else if(Records)
            {
                std::pair<std::string,std::string> recordPair(Lapreturns[RecordListPlace].Name, Lapreturns[RecordListPlace].chromosome);
                ConstantData.savedChromosomes.push_back(recordPair);
            }
            else
            {
                osdialog_message(OSDIALOG_ERROR, OSDIALOG_OK, "Please Select an Item to use it");
            }
        }
        if(ImGui::Button("Delete Selected Record"))
        {
            int hey = osdialog_message(OSDIALOG_WARNING, OSDIALOG_YES_NO, "Are you sure you want delete this record?");
            if(hey)
            {
                std::string sql = "";
                if(user)
                {
                    sql = "DELETE FROM Gene WHERE ID = '" + std::to_string(genereturns[GeneListPlace].ID) + "';";
                    ConstantData.database->Delete(sql);
                }
                if(Records)
                {
                    sql = "DELETE FROM LapTimeRecords WHERE TrackID = '" + std::to_string(Lapreturns[RecordListPlace].TrackID) + "';";
                    ConstantData.database->Delete(sql);
                }
            }
            Load();
        }
        if(ImGui::Button("Clear Database"))
        {
            int hey = osdialog_message(OSDIALOG_WARNING, OSDIALOG_YES_NO, "Are you sure you want delete EVERY RECORD?");
            if(hey)
            {
                std::string sql = "";
                if(user)
                {
                    sql = "DELETE FROM Gene;";
                    ConstantData.database->Delete(sql);
                }
                if(Records)
                {
                    sql = "DELETE FROM LapTimeRecords;";
                    ConstantData.database->Delete(sql);
                }
            }
            Load();
        }
    }
    
    //Bad
    SelectedItems.clear();
    for(int i{0}; i< ConstantData.savedChromosomes.size(); i++)
    {
        SelectedItems.push_back(std::to_string(i+1) + ": " + ConstantData.savedChromosomes[i].first + ", " + ConstantData.savedChromosomes[i].second);
    }
    if(SelectedItems.size() > ConstantData.PopSize)
    {
        ConstantData.PopSize = (int)SelectedItems.size();
    }
    ImGui::ListBox("Selected Genes", &selectedgenesplace,SelectedItems );
}
void Menu::MainMenu()
{
    if(ImGui::Button("Play"))
    {
        ConstantData.Playing = true;
    }
    if(ImGui::Button("Exit"))
    {
        int hey = osdialog_message(OSDIALOG_WARNING, OSDIALOG_OK_CANCEL, "Are you sure you want to exit?");
        if(hey)
        {
            ConstantData.Playing = false;
            window.close();
        }
    }
    if(ImGui::Button("Debug: UsePlayer"))
    {
        if(!ConstantData.usePlayer)
        {
            ConstantData.usePlayer = true;
            window.setTitle("Manual Control - DEBUG");
            ConstantData.PopSize = 1;
            ConstantData.laps = 100;
        }
        else
        {
            ConstantData.usePlayer = false;
            window.setTitle("AI Control - DEBUG");
            ConstantData.PopSize = 50;
            ConstantData.laps = 10;
        }
    }
    ImGui::Checkbox("Limit to Laps", &ConstantData.StopOnLaps);
    ImGui::SliderInt("Laps",&ConstantData.laps, 1, 100);
    ImGui::SliderInt("Number of Agents", &ConstantData.PopSize, 1, 1000);
    ImGui::SliderInt("Horizontal Resolution", &ConstantData.ScreenWidth, 800, 3840);
    ImGui::SliderInt("Vertical Resolution", &ConstantData.ScreenHeight, 800, 2160);
    
    float gentime = ConstantData.GenerationLength.asSeconds();
    ImGui::SliderFloat("Generation Time", &gentime , 1.f, 600.f);
    ConstantData.GenerationLength = sf::seconds(gentime);
    
    if(ImGui::Button("NextTrack"))
    {
        if(Helper::is_file_exist("Circuits/TrackVis_" + std::to_string(ConstantData.TrackNumber+1) + ".png") && Helper::is_file_exist("Circuits/TrackBac_" + std::to_string(ConstantData.TrackNumber+1) + ".png"))
        {
            ConstantData.TrackNumber++;
            track.loadFromFile("Circuits/TrackVis_" + std::to_string(ConstantData.TrackNumber) + ".png");
            tracksprite.setTexture(track);
            
            if(ConstantData.TrackNumber > ConstantData.NumberOfTracks)
            {
                ConstantData.NumberOfTracks = ConstantData.TrackNumber;
            }
        }
        else
        {
            ConstantData.TrackNumber = 1;
            track.loadFromFile("Circuits/TrackVis_" + std::to_string(ConstantData.TrackNumber) + ".png");
            tracksprite.setTexture(track);
        }
    }
    if(ImGui::Button("LastTrack"))
    {
        if(ConstantData.TrackNumber != 0)
        {
            if(Helper::is_file_exist("Circuits/TrackVis_" + std::to_string(ConstantData.TrackNumber-1) + ".png") && Helper::is_file_exist("Circuits/TrackBac_" + std::to_string(ConstantData.TrackNumber-1) + ".png"))
            {
                ConstantData.TrackNumber--;
                track.loadFromFile("Circuits/TrackVis_" + std::to_string(ConstantData.TrackNumber) + ".png");
                tracksprite.setTexture(track);
            }
        }
    }
    ImGui::Image(tracksprite);
}
void Menu::CarEditor()
{
    if(ImGui::Button("Save"))
    {
        char* FilePath = osdialog_file(OSDIALOG_SAVE,"./Players","Player.txt",NULL);
        if(FilePath)
        {
            CarStats.SaveToFile(FilePath);
            ConstantData.CarFilePath = FilePath;
            free(FilePath);
        }
        else
        {
            fprintf(stderr, "\tCanceled\n");
        }
    }
    if(ImGui::Button("Load"))
    {
        //osdialog_filters *filters = osdialog_filters_parse(":txt;");
        char *FilePath = osdialog_file(OSDIALOG_OPEN, "./Players", "test", NULL);
        if (FilePath)
        {
            
            ConstantData.CarFilePath = FilePath;
            CarStats.LoadFromFile(FilePath);
            ConstantData.CarFilePath = FilePath;
            free(FilePath);
            
        }
        else
        {
            fprintf(stderr, "\tCanceled\n");
        }
        
    }
    ImGui::LabelText((char*)(ConstantData.CarFilePath.c_str())," ");
    
    ImGui::SliderFloat("Top Speed", &CarStats.MaxSpeed , 12.f, 300.f);
    ImGui::SliderFloat("Acceleration", &CarStats.acceleration , 0.001f, 2.f);
    ImGui::SliderFloat("Drag", &CarStats.Drag , 0.001f, 2.f);
    ImGui::SliderFloat("Brake Strength", &CarStats.BrakePower , 0.001f, 2.f);
    ImGui::SliderFloat("TurnSpeed", &CarStats.TurnSpeed , 0.001f, 10.f);
    ImGui::SliderFloat("OffTrackSpeed", &CarStats.OffTrackSpeed , 3.f , 300.f );
    
    //CarOrigin
    ImGui::SliderFloat("CarOrigin X", &ConstantData.CarOrigin.x, 0.f , ConstantData.carSprite->getLocalBounds().width);
    ImGui::SliderFloat("CarOrigin Y", &ConstantData.CarOrigin.y, 0.f , ConstantData.carSprite->getLocalBounds().height);
}



