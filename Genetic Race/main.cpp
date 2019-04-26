// Daniel Harvey
// s6291311


#include "game.h"
#include "menu.h"
#include "DataStruct.hpp"
#include <iostream>
#include <cstring>
#include <string>
#include <sqlite3.h>


//https://github.com/eliasdaler/imgui-sfml
//https://github.com/ocornut/imgui
//https://eliasdaler.wordpress.com/2016/05/31/imgui-sfml-tutorial-part-1/



int main()
{
    srand ((unsigned int)time(NULL));
    Data ConstantData;
    ConstantData.Init();
    Menu menu(ConstantData);
    menu.Run();
    
    bool restart = true;
    
    while(restart)
    {
        restart = ConstantData.Playing;
        if(restart)
        {
            Game game(ConstantData);
            restart = game.Start();
            if(restart)
            {
                ConstantData.Init();
                Menu newMenu(ConstantData);
                newMenu.Run();
                restart = ConstantData.Playing;
            }
        }
    }
    ConstantData.End();
    
    return 0;
}







