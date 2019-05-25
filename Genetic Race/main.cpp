// Daniel Harvey
// s6291311

#include "game.h"
#include "menu.h"
#include "DataStruct.hpp"

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
                ConstantData.End();
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
